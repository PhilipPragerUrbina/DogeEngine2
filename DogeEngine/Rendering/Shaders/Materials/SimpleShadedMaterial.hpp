//
// Created by Philip on 4/2/2023.
//

#pragma once

#include "../../API/Material.hpp"
#include "../../../IO/Image.hpp"
#include "../../OpenGL/OpenGLTexture.hpp"

namespace Doge {

    class SimpleShadedMaterial : public Material{
    private:
        Resource<OpenGLTexture> albedo_texture;
    public:

        std::string getFragmentLocation() const override {
            return "CompiledShaders/simple_shaded.frag.spv";
        }

        std::string getVertexLocation() const override {
            return "CompiledShaders/simple_shaded.vert.spv";
        }

        SimpleShadedMaterial(OpenGLShaderManager* shader_manager,  Resource<OpenGLTexture> albedo_image) : Material(shader_manager, getVertexLocation(), getFragmentLocation()),
                                                                                                albedo_texture(albedo_image){

        }

        //todo wrap these things in a 'resource'. A smart reference counting pointer you get from the resource manager, to avoid redundant, copying,destructing, and re-loading data.

        /**
         * Position of the light
         */
        glm::vec3  light_pos;

    protected:
        void setUniforms(const std::shared_ptr<OpenGLShaderProgram> &shader, const RenderData &render_data) override {
            shader->setUniform("model", render_data.object_transform);
            shader->setUniform("view", render_data.camera.getTransform());
            shader->setUniform("projection", render_data.camera.getProjection());
            shader->setTexture("albedo", albedo_texture->getTextureID(),0);
            shader->setUniform("light_pos", light_pos);

        }


    };

} // Doge
