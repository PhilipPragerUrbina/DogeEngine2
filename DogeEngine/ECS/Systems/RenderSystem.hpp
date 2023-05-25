//
// Created by Philip on 4/15/2023.
//

#pragma once

#include "System.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/HierarchyComponent.hpp"
#include "../Components/RenderComponent.hpp"
#include "../../Rendering/OpenGL/OpenGLApplication.hpp"


namespace Doge {

    /**
     * Creates draw calls
     */
    class RenderSystem : public System<TransformComponent, RenderComponent>{
    private:
        OpenGLApplication* renderer;
    public:

        RenderSystem(OpenGLApplication *renderer, ECSManager* manager) : renderer(renderer),
                                                                         System<TransformComponent, RenderComponent>(manager) {}


        void process(TransformComponent& transform, RenderComponent& settings) override {
            renderer->draw(settings.getMesh().operator->(),settings.getMaterial().operator->(),transform.getGlobalTransform());
        }

        void update() {
            run();
        }
    };

} // Doge
