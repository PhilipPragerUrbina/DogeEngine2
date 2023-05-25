//
// Created by Philip on 4/6/2023.
//

#pragma once
#include <string>
#include <vector>
#include <typeinfo>
#include "../ECSManager.hpp"
namespace Doge {

    /**
     * Applies operations on entities with specific combination of components
     * @tparam Component_types Required components in order.
     * @example Access components in process(). Call run() when process should be called for each entity
     * @details Tries to make memory as contiguous as possible for best cache performance.
     */
    template<typename... Component_types>
    class System {
    private:
        std::tuple<std::vector<Component_types>*...> vectors; //Contains references to the needed arrays

        /**
         * Recursive metaprogramming loop. Collects the components corresponding to and entity.
         * @tparam N Which component array to search. Loop index.
         * @tparam T Components collected so far
         * @param entity_id Entity to search for
         * @param stuff Components collected so far
         */
        template<int N,typename... T>
        void collectComponentsLoop(int entity_id, T&... stuff) {
            int idx = entity_id; //Start in array position corresponding to entity index
            if(idx >= std::get<N>(vectors)->size()) idx = std::get<N>(vectors)->size()-1; //Make sure not out of bounds

            //Search backwards for the component with the requested entity
            while (idx > -1 && entity_id <= std::get<N>(vectors)->operator[](idx).getEntity().getID() ){ //components are sorted by entity id
                if(entity_id == std::get<N>(vectors)->operator[](idx).getEntity().getID() ){ //Found component
                    collectComponentsLoop<N - 1>(entity_id, std::get<N>(vectors)->operator[](idx), stuff...); //Add to params and move one
                    return ;
                }
                idx--;
            }
            //Not found, entity does not have the right components for this system
        }

        /**
         * The last iteration in the loop, achieved through overloading
         * Once the correct components have been collected, they can be passed into the process as arguments
         * @tparam N Should be -1.
         * @tparam T Components collected so far. Now matches Component_types.
         * @param stuff Components collected so far as arguments.
         */
        template<int N,typename... T> void collectComponentsLoop ( int entity_id , Component_types&... stuff) {
            process(stuff...);
        }

        /**
         * Recursive metaprogramming loop. Get pointers to all the needed component arrays.
         * @tparam N Loop index.
         */
        template<int N>
        void fetchArraysLoop() {
            typedef typename std::tuple_element<N,std::tuple<Component_types...>>::type CurrentType; //Grab the current component type
            std::vector<CurrentType>* vector = ECS_manager->template getComponentBuffer<CurrentType>();
            if(vector != nullptr){
                std::get<N>(vectors) = vector; //Get pointer to contiguous array
            }
            fetchArraysLoop<N-1>(); //Recurse
        }

        /**
         * End of loop iteration. Achieved through template specialization.
         */
        template<>void fetchArraysLoop<-1>() {}
    protected:

        /**
         * Will iterate over each entity on run(). Use this to modify or gather information on entities.
         */
        virtual void process(Component_types&...){}

        /**
         * The Entity component system manager
         * Use this to get additional components, or to add new components,etc.
         */
        ECSManager* ECS_manager;

        /**
        * Run the system on all available entities with the correct components
         * Every time this is called, all matching entities will be run through the process method.
        */
        void run() {
            fetchArraysLoop<sizeof...(Component_types)-1>(); //Get pointers to required arrays
            //Loop through one of the component arrays
            for (int i = 0; i < std::get<0>(vectors)->size(); ++i) {
                int first_id = std::get<0>(vectors)->at(i).getEntity().getID();
                collectComponentsLoop<sizeof...(Component_types)-1>(first_id);
            }
        }
    public:
        /**
         * Create a new system to iterate over an ECS
         * @param ECS_manager The manager that contains the components/entities
         */
        explicit System(ECSManager* ECS_manager) : ECS_manager(ECS_manager){}

    }; // Doge
}