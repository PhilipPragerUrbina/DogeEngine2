//
// Created by Philip on 4/9/2023.
//

#pragma once

#include "Component.hpp"
#include "glm.hpp"
namespace Doge {

    class TransformComponent : public Component{
    public:
        glm::vec3 pos;
    };

} // Doge
