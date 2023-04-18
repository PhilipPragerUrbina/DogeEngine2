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

        glm::mat4 local;
        glm::mat4  world;
        bool world_out_of_date;//change with system

        //todo setlocal
        //todo translate,rotate,scale,etc.
    };

} // Doge
