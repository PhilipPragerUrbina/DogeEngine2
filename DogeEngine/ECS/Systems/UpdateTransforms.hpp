//
// Created by Philip on 4/15/2023.
//

#pragma once

#include "System.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/HierarchyComponent.hpp"


namespace Doge {

    /**
     * Updates the global transforms based on a hierarchy(scene graph)
     */
    class UpdateTransforms : public System<TransformComponent, HierarchyComponent>{
    private:
        std::vector<Entity> top_level;


        void recurse(const glm::mat4& last_transform, Entity current ){
            HierarchyComponent* current_node = ECS_manager->getComponent<HierarchyComponent>(current);
            if(current_node == nullptr){
                return;
            }

            TransformComponent* current_transform = ECS_manager->getComponent<TransformComponent>(current);
           if(current_transform != nullptr){
               current_transform->updateGlobal(  last_transform* current_transform->getLocalTransform() );

               for (Entity child : current_node->getChildren()) {
                   recurse(current_transform->getGlobalTransform(), child);
               }
           }else{
               for (Entity child : current_node->getChildren()) {
                   recurse(last_transform, child);
               }
           }



        }
    public:
        //todo rethink how the system interface works to make this better

        //Find the highest level nodes that have been marked out of dat
        void process(TransformComponent& transform, HierarchyComponent& node) override {
                if(transform.globalNeedsUpdating()){
                    if(!node.getParent() ){
                        top_level.push_back(node.getEntity()); //todo collect ones on boundary between updated and out of date
                    }else{
                        TransformComponent* parent_transform = ECS_manager->getComponent<TransformComponent>(node.getParent().value());
                        if(parent_transform != nullptr && !parent_transform->globalNeedsUpdating()){
                            top_level.push_back(node.getEntity()); //todo collect ones on boundary between updated and out of date
                        }
                    }
                }
        }

        UpdateTransforms(ECSManager* manager) : System<TransformComponent, HierarchyComponent>(manager){}

        void update(){

            top_level.clear();
            run();
            for (Entity entity : top_level) {
                recurse(glm::mat4(1.0f),entity );
            }

        }


    };

} // Doge
