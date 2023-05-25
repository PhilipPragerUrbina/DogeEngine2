//
// Created by Philip on 4/9/2023.
//

#pragma once

#include "Component.hpp"
#include "glm.hpp"
namespace Doge {

    /**
     * Represents the transform of an entity
     */
    class TransformComponent : public Component{
    private:
        glm::mat4 local; //Relative to something else
        glm::mat4  world{}; //Relative to world
        bool world_out_of_date; //If the world transform needs to be updated
        //https://www.reddit.com/r/cpp/comments/3xmpz9/holding_reference_or_pointer_of_a_vector_element/
    public:
        //https://www.reddit.com/r/gamedev/comments/1uka6h/suggestions_on_how_to_integrate_a_parented_and/
        /**
         * Create a transform
         * @param local_transform Transform relative to parent
         */
        explicit TransformComponent(const glm::mat4& local_transform) : local(local_transform) , world_out_of_date(true), world(local_transform){}

        /**
         * Set the local transform
         * @param new_local_transform Transform relative to parent
         */
        void setLocal(const glm::mat4& new_local_transform ){
            local = new_local_transform;
            world_out_of_date = true;
        }

        //todo doc and check naming
        glm::mat4 getLocalTransform() const {

            return local;
        }

        /**
         * Get transform relative to world
         * @warning Make sure to update the global transform based on the local transform
         */
        glm::mat4 getGlobalTransform() const {
            return world;
        }

        /**
        * Update the global transform
        * Should be based on local transform and a hierarchy
        * @param new_global_transform Transform relative world
        */
        void updateGlobal(const glm::mat4& new_global_transform ){
            world = new_global_transform;
            world_out_of_date = false;
        }

        bool globalNeedsUpdating() const {
            return world_out_of_date; //todo switch world naming to global
        }

        //todo translate,rotate,scale,etc.
    };

} // Doge
