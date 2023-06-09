//
// Created by Philip on 4/3/2023.
//

#pragma once

#include "stb_image.h"
#include <string>
#include <stdexcept>
#include "InFile.hpp"
#include "../Data/ResourceData.hpp"
#include "../Data/ResourceManager.hpp"
namespace Doge {

    /**
     * Load an image into memory from the filesystem
     */
    class Image : public ResourceData{
    private:
        int width, height, channels;
        unsigned char* data;
        bool stb = true;
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
         * Create image from raw data
         */
        Image(int width, int height, int channels, unsigned char *data) : width(width), height(height),
                                                                          channels(channels), data(data) , stb(false){}




        /**
         * Autoload image
         * @param manager Manager to request from
         * @param id Resource ID to load
         */
        Image(ResourceManager* manager, const std::string& id) : Image(manager->requestFile(id, {".hdr", ".png", ".jpg", ".jpeg", ".tga", ".bmp"})){}


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

        ~Image() override{
            if(stb){
                stbi_image_free(data);//todo separate out loaders from data
            }else{
                delete[] data;
            }
        }

    };

} // Doge
