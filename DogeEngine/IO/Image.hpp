//
// Created by Philip on 4/3/2023.
//

#pragma once
#include "stb_image.h"
#include <string>
#include <stdexcept>
#include "InFile.hpp"
namespace Doge {

    /**
     * Load an image into memory
     */
    class Image {
    private:
        int width, height, channels;
        unsigned char* data;
    public:

        /**
         * Load an image from a file
         * @param file The image file
         * Supported formats: jpg,png,bmp,tga,and hdr
         * @throws runtime_error Problems reading image
         */
        Image(const InFile& file){
            data = stbi_load(file.getLocation().c_str(), &width, &height, &channels, 0);
            if (!data)
            {
                throw std::runtime_error("Unable to open image file: " + file.getLocation());
            }
        }

        /**
         * Get the raw image data
         * @return Pointer to image data. Is tied to lifetime of image object
         */
        unsigned char* getImageData() const {
            return data;
        }

        /**
         * Get pixel width of image
         */
        int getWidth() const {
            return width;
        }

        /**
         * Get pixel height of image
         */
        int getHeight() const {
            return height;
        }

        /**
         * Get the number of channels
         */
        int getChannelCount() const {
            return channels;
        }

        ~Image(){
            stbi_image_free(data);
        }

    };

} // Doge
