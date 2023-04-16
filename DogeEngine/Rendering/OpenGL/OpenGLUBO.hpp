//
// Created by Philip on 4/1/2023.
//

#pragma once
#include "glad/glad.h"
namespace Doge {

     /**
      * A uniform buffer
     * Used to easily set shared uniforms in shaders
      * @tparam UBO_struct The actual data struct.
      * @warning Make sure to properly align your data struct
      */
    template <typename UBO_struct> class OpenGLUBO {
    private:
         unsigned int ubo_id;
         unsigned int binding_point;

    public:
        /**
         * Create a uniform buffer
         * @param binding_point Which binding point to put it at
         */
            OpenGLUBO(unsigned int binding_point) : binding_point(binding_point){
                glGenBuffers(1, &ubo_id);
                glBindBuffer(GL_UNIFORM_BUFFER, ubo_id);
                glBufferData(GL_UNIFORM_BUFFER, sizeof(UBO_struct), nullptr, GL_STATIC_DRAW);
                glBindBuffer(GL_UNIFORM_BUFFER, 0);
                glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, ubo_id);
        }

        /**
         * Update the uniform data
         * @param data The new data
         */
            void setData(UBO_struct data){
                glBindBuffer(GL_UNIFORM_BUFFER, ubo_id);
                glBufferSubData(ubo_id, 0, sizeof(UBO_struct), &data );
                glBindBuffer(GL_UNIFORM_BUFFER, 0);
            }

         //todo  uniform buffer object API for shader program: https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL. Such as a standard camera UBO, and optional additional UBO's
    };

} // Doge
