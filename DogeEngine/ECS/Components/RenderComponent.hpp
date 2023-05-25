//
// Created by Philip on 4/9/2023.
//

#pragma once

#include "Component.hpp"
#include "../../Rendering/OpenGL/OpenGLMesh.hpp"
#include "../../Rendering/API/Material.hpp"
#include <vector>
#include <optional>

namespace Doge {

    /**
     * Contains rendering info
     */
    class RenderComponent : public Component{
    private:
        Resource<OpenGLMesh> mesh;
        Resource<Material> material;
    public:
        const Resource<OpenGLMesh> &getMesh() const {
            return mesh;
        }

        Resource<Material> getMaterial()  {
            return material;
        }

        RenderComponent(const Resource<OpenGLMesh> &mesh,  Resource<Material> material) : mesh(mesh),
                                                                                                material(material) {}



    };

} // Doge
