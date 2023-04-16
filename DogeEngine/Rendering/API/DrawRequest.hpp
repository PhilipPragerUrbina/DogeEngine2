//
// Created by Philip on 4/1/2023.
//

#pragma once

#include "Material.hpp"
#include "../OpenGL/OpenGLMesh.hpp"

namespace Doge {

    /**
     * A request to draw something
     */
    class DrawRequest {
    private:
        OpenGLMesh* mesh;
        Material* material;
        glm::mat4 transform;
    public:
        /**
         * Create a request that can be sent to the renderer to render
         * @param mesh Mesh to render
         * @param material Material to render.
         * @param transform The transform of the object that should be drawn
         */
        DrawRequest(OpenGLMesh *mesh, Material *material, glm::mat4 transform) : mesh(mesh), material(material) , transform(transform){}

        /**
         * Get the transform of the object that should be drawn
         */
        glm::mat4 getTransform() const {
            return transform;
        }

        /**
         * Get the mesh to be rendered
         */
        OpenGLMesh *getMesh() const {
            return mesh;
        }

        /**
         * Get the material to use for rendering
         */
        Material *getMaterial() const {
            return material;
        }
    };

} // Doge
