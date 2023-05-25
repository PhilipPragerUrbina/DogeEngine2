//
// Created by Philip on 4/1/2023.
//

#pragma once

#include "Camera.hpp"
#include "../../Data/ResourceData.hpp"
#include "../../Data/ResourceManager.hpp"
#include "../OpenGL/OpenGLShaderProgram.hpp"

namespace Doge {

    //There are three kinds of data. Material specific(contained within material class), object specific, and world specific
    //todo set world specific data like Camera or lights with UBO

    /**
     * Additional data that is useful for rendering
     */
    struct RenderData{
        Camera camera;
        glm::mat4 object_transform{};
    };

    /**
     * A material defines a uniform based interface between c++ and a shader program
     * Each instance of a given material shares the same shader program, and set's it's uniforms each frame to whatever that specific instance desires(state is stored within material instance until draw call).
     * @example A metal material would be linked with a shader that has a roughness uniform. Then you can create a copper and aluminium instance of the metal material with different roughness values.
     * @warning This is an interface, make sure to implement all methods for your specific material type
     */
    class Material : public ResourceData{
    private:
        Resource<OpenGLShaderProgram> shader_program; //Private because it should only be accessed during setUniforms()
    public:
        Material(ResourceManager* manager, const std::string& shader_stem) : shader_program(manager->requestResource<OpenGLShaderProgram>(shader_stem)){}


        Resource<OpenGLShaderProgram> getShaderProgram() const {
            return shader_program;
        }

       /**
        * Get the file stem of the spir-V binaries making up this material shader
        * @details The resource directories will be searched for the corresponding .vert.spv and .frag.spv with this stem
        */
        virtual std::string getShaderName() const { return ""; };


        /**
         * Set the shader uniforms to what they need to be for this specific material instance
         * @param render_data Settings shared by all materials
         */
        void setUniforms(const RenderData& render_data){
            setUniforms(shader_program,render_data);
        }

    protected:
        /**
       * Set the shader uniforms to what they need to be for this specific material instance
         * @warning Make sure to set all available uniforms and textures
       * @param shader Use this to set uniforms
         * @param render_data Additional scene information that is useful for most materials. Such as camera and object transforms.
       */
        virtual void setUniforms(const Resource<OpenGLShaderProgram>& shader,const RenderData& render_data){}
    };

} // Doge
