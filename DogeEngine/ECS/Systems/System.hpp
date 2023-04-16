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
     * @tparam Component_types Required components.
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
            if(idx > std::get<N>(vectors)->size()) idx = std::get<N>(vectors)->size()-1; //Make sure not out of bounds

            //Search backwards for the component with the requested entity
            while (entity_id <= std::get<N>(vectors)->at(idx).getEntity().getID() ){ //components are sorted by entity id
                if(entity_id == std::get<N>(vectors)->at(idx).getEntity().getID() ){ //Found component
                    collectComponentsLoop<N - 1>(entity_id, std::get<N>(vectors)->at(idx), stuff...); //Add to params and move one
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
        template<int N,typename... T> void collectComponentsLoop (int entity_id , Component_types&... stuff) {
            process(stuff...);
        }

        /**
         * Recursive metaprogramming loop. Get pointers to all the needed component arrays.
         * @tparam N Loop index.
         * @param ECS Where to get components from.
         */
        template<int N>
        void fetchArraysLoop(ECSManager *ECS) {
            typedef typename std::tuple_element<N-1,std::tuple<Component_types...>>::type CurrentType; //Grab the current component type
            std::get<N>(vectors) = ECS->template getComponentBuffer<CurrentType>(); //Get pointer to contiguous array
            fetchArraysLoop<N-1>(ECS); //Recurse
        }

        /**
         * End of loop iteration. Achieved through template specialization.
         */
        template<>void fetchArraysLoop<-1>(ECSManager *ECS) {}
    protected:
        //todo allow multiple operations that run at different times.(eg. setup, every frame, etc)
        virtual void process(Component_types&...){} //Operation performed on entities
    public:

        /**
         * Run the system on all available entities with the correct components
         * @param ECS The manager that contains the components/entities
         */
        void run(ECSManager *ECS) {
            fetchArraysLoop<sizeof...(Component_types)-1>(ECS); //Get pointers to required arrays

            //Loop through one of the component arrays
            for (int i = 0; i < std::get<0>(vectors)->size(); ++i) {
                int first_id = std::get<0>(vectors)->at(i).getEntity().getID();
                collectComponentsLoop<sizeof...(Component_types)-1>(first_id);
                //todo check cache locality. Make some stuff inline.
            }
        }
    }; // Doge
}