//
// Created by Philip on 4/2/2023.
//

#pragma once

#include "glad/glad.h"
#include "../../IO/Image.hpp"

namespace Doge {

    /**
     * OpenGL image data
     */
    class OpenGLTexture {
    private:
        unsigned int texture_id;

    public:
        /**
         * Load an OpenGL texture from an image
         * @param image Image data to load from
         * Supports rgb and rgba images
         * @throws runtime_error Problems loading image
         */
        OpenGLTexture(const Image& image){
            //Bind texture
            glGenTextures(1, &texture_id);
            glBindTexture(GL_TEXTURE_2D, texture_id);

            //Set texture settings
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            //Pick correct format
            if(image.getChannelCount() == 3){
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getWidth(), image.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.getImageData());
            }else if(image.getChannelCount() == 4){
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getImageData());
            }else{
                throw std::runtime_error("Unsupported channel count.");
            }
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        /**
         * Get the texture id that can be used for using this texture in shaders
         */
        unsigned int getTextureID(){
            return texture_id;
        }



        ~OpenGLTexture(){
            glDeleteTextures(1, &texture_id);
        }

    };

} // Doge
