//
// Created by Philip on 4/2/2023.
//

#pragma once

#include "../../API/Material.hpp"
#include "../../../IO/Image.hpp"
#include "../../OpenGL/OpenGLTexture.hpp"

namespace Doge {

    /**
     * Non textured material
     */
    class SimpleShadedMaterial : public Material{
    private:
        glm::vec3 color;
    public:

        std::string getShaderName() const override {
            return "simple_shaded"; // CompiledShaders/simple_shaded.vert.spv & CompiledShaders/simple_shaded.frag.spv
        }

        SimpleShadedMaterial(ResourceManager* shader_manager,  glm::vec3 color) : Material(shader_manager, getShaderName()),color(color){}

        /**
         * Position of the light
         */
        glm::vec3 light_pos{ 0,0,0 };

    protected:
        void setUniforms(const Resource<OpenGLShaderProgram> &shader, const RenderData &render_data) override {
            shader->setUniform("model", render_data.object_transform);
            shader->setUniform("view", render_data.camera.getTransform());
            shader->setUniform("projection", render_data.camera.getProjection());
            shader->setUniform("color", color);
            shader->setUniform("light_pos", light_pos);
        }


    };

} // Doge
