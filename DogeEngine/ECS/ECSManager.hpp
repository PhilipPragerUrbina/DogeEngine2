//
// Created by Philip on 4/6/2023.
//

#pragma once
#include <vector>
#include <unordered_map>
#include "Components/Component.hpp"
#include "../Utils/GenericVector.hpp"
#include <typeinfo>
#include <optional>
namespace Doge {

    /**
     * Manages parts related to an Entity Component System
     * @related https://t-machine.org/index.php/2014/03/08/data-structures-for-entity-systems-contiguous-memory/
     */
    class ECSManager {
    private:
        unsigned int latest_available_entity_id = 0; //Incrementing unique entity id

        //Contains a list of components in order of entity id for each component type
        //This allows components to exist in contiguous memory within a vector. Making sequential access from a system very cache friendly.
        //At the same time, having a separate vector for each type allows easy access to any combination of multiple component types(eg: MeshComponent & RenderComponent).
        std::unordered_map<std::string,GenericVector*> data_table;

        /**
         * Add a component to the list of components for that type
         * @param entity_id ID of entity the component corresponds to. Determines its position in the list.
         * @tparam ComponentType Should be derived from Component
         * @param component Contains data relevant to an entity. Make sure to set what entity it belongs to before adding.
         * @throws runtime_error Same component can not be added twice to the same entity.
         * @details Best case: O(1) if adding to the most recent entity. Worst case: O(n) if adding to an entity that was created at the beginning.
         */
        template <typename ComponentType >void addComponent(const ComponentType& component, unsigned int entity_id){
            std::string id = typeid(ComponentType).name(); //Get id for the component type

            if(data_table.find(id) == data_table.end()){
                data_table[id] = new GenericVectorImplementation<ComponentType>; //Create a list for that type if it does not exist yet
            }

            std::vector<ComponentType>* list = &((GenericVectorImplementation<ComponentType>*)data_table[id])->vector; //Get component type list

            if(list->empty() || entity_id > (&(*list)[list->size()-1])->getEntity().getID()) {
                list->push_back(component); //trivial case
            } else{
                //Not in order. Must find correct placement of component.
                int idx = list->size()-1;    //Start at end of array
                unsigned int current_id = (&(*list)[idx])->getEntity().getID();

                while (entity_id <= current_id) { //Must be ordered by entity
                    if (entity_id == current_id) {
                        throw std::runtime_error("Same component can not be added twice to the same entity.");
                    }
                    idx--;
                    if(idx == -1){break;} //All the way at end
                    current_id = (&(*list)[idx])->getEntity().getID();
                }
                //Edge case. todo find better way
                if(idx == -1){
                    list->insert(list->begin(), component);
                    return;
                }
                list->insert(list->begin() + idx + 1, component);
            }
        }

        /**
         * Access a component directly.
         * @tparam ComponentType The type of component to fetch
         * @param i The internal index of the component in its list. Does not correspond to entity id.
         * @return Pointer to component so one can access the data
         * @warning If a component does not exist, undefined behavior for performance reasons.
         */
        template <typename ComponentType > ComponentType* getComponent(int i){
            std::string id = typeid(ComponentType).name();
            return &((GenericVectorImplementation<ComponentType>*)data_table[id])->vector[i];
        }


    public:

        /**
        * Get the list of all components of a type.
        * @tparam ComponentType The type of component to get.
        * @return A pointer to the list of components for that given type. nullptr if component list does not exist.
        */
        template <typename ComponentType > std::vector<ComponentType>* getComponentBuffer(){
            std::string id = typeid(ComponentType).name();
            if(data_table.find(id) == data_table.end()) return nullptr;
            return &((GenericVectorImplementation<ComponentType>*)data_table[id])->vector;
        }

        /**
         * Get an entity using this ECS manager
         * @warning All entities must be created using an ECS manager. And should only be used with the ECS manager of origin.
         */
        Entity createEntity(){
            latest_available_entity_id++;
            return Entity{latest_available_entity_id-1};
        }


        /**
         * Add a component to an entity
         * @param entity Entity from this ECS to add the component to.
         * @tparam ComponentType Should be derived from Component.
         * @param component Contains data relevant to an entity. Will be copied.
         * @throws runtime_error Same component can not be added twice to the same entity. If you want to add two eyes, make a list of eyes in a single component and add that. Components should be like traits!
         * @details Best case: O(1) if adding to the most recent entity. Worst case: O(n) if adding to an entity that was created at the beginning.
       */
        template <typename ComponentType >void addComponent(const Entity& entity, ComponentType component){
            component.setEntity(entity);
            addComponent(component,entity.getID());
        }

        /**
         * Get a component from an entity.
         * @param entity Entity to get component from
         * @tparam ComponentType The type of component to fetch.
         * @return Pointer to component so one can access the data. Nullptr if component does not exist on this entity.
         * @warning Not recommender for most cases. Use a system instead.
         * @details Best case: O(1) if all entities have a component. Worst case: O(n) if half of all entities have a component and the entity id is opposite to the actual component location.
         */
        template <typename ComponentType > ComponentType* getComponent(const Entity& entity){
            //Get list of all components of that type
            std::vector<ComponentType>* list = getComponentBuffer<ComponentType>();
            if(list == nullptr) return nullptr;

            //Start at position in array corresponding to the entity id
            int id = (int)entity.getID();
            int idx = id;
            if(idx > list->size()) idx = list->size()-1; //Make sure not out of bounds
            //todo binary search
            ComponentType* current = &(*list)[idx];
            //Search for the component with the requested entity.
            while (id <= current->getEntity().getID() ){ //Move backwards since components are sorted by entity
                if(id == current->getEntity().getID()){
                    return current;
                }
                idx--;
                if(idx == -1){break;} //Out of bounds
                current = &(*list)[idx];
            }
            return nullptr;
        }

        ~ECSManager(){
            //Clean up vectors
            for (const auto& pair : data_table) {
                delete pair.second;
            }
        }
    };

} // Doge
