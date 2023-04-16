//
// Created by Philip on 4/2/2023.
//

#pragma once

#include "vec3.hpp"
#include "vec2.hpp"

namespace Doge {

    /**
     * The standard vertex attributes
     * Position, Normal, and texture coordinates
     */
    struct VertexAttributes{
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 tex_coords;
    };

} // Doge
