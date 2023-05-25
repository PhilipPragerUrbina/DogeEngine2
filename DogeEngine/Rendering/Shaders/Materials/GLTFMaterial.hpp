//
// Created by Philip on 4/2/2023.
//

#pragma once

#include "../../API/Material.hpp"
#include "../../../IO/Image.hpp"
#include "../../OpenGL/OpenGLTexture.hpp"

namespace Doge {

    /**
     * Material for GLTF models
     */
    class GLTFMaterial : public Material{
    private:
        Resource<OpenGLTexture> albedo_texture;
        glm::vec3 color_factor; //Multiplied with texture

    public:
        std::string getShaderName() const override {
            return "gltf"; // CompiledShaders/gltf.vert.spv & CompiledShaders/gltf.frag.spv
        }

        /**
         * Position of the light
         */
        glm::vec3  light_pos {0,-10,0};

        GLTFMaterial(ResourceManager* shader_manager,  Resource<OpenGLTexture> albedo_texture, const glm::vec3& color_factor = {1,1,1}) : Material(shader_manager, getShaderName()),
        albedo_texture(albedo_texture), color_factor(color_factor) {}

    protected:
        void setUniforms(const Resource<OpenGLShaderProgram> &shader, const RenderData &render_data) override {
            shader->setUniform("model", render_data.object_transform);
            shader->setUniform("view", render_data.camera.getTransform());
            shader->setUniform("projection", render_data.camera.getProjection());
            shader->setTexture("albedo", albedo_texture->getTextureID(),0);
            shader->setUniform("light_pos", light_pos);
            shader->setUniform("color_factor", color_factor);
        }


    };

} // Doge
