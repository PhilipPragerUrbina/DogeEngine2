//
// Created by Philip on 4/17/2023.
//

#pragma once
#include <unordered_map>

#include "Resource.hpp"
#include "../IO/Directory.hpp"
#include "ResourceData.hpp"

namespace Doge {

    /**
     * Manages lifetime of shared resources
     */
    class ResourceManager {
    private:
        std::unordered_map<std::string, ResourceData*> data;

        Directory file_system;
    public:
        /**
         * Create a manager of shared resources
         * Will load data off the filesystem into memory and transform it as needed
         * @param file_system Where to look for needed file data. (Resources directory)
         */
        ResourceManager(const Directory& file_system) : file_system(file_system){}


        /**
        * Get a file to access data from the filesystem.
        * @param id ID of file. The filename without the extension or directories.
        * @return A file from which data can be read.
        * @throws runtime_exception Unable to find file
        */
        InFile requestFile(const std::string& id) {
            std::optional<InFile> search = file_system.findFile(id);
            if(search) return search.value();
            throw std::runtime_error("Unable to find file: " + id);
        };


        /**
         * Add a resource directly
         * @details Useful for adding resources that were stored within other resources(eg: a texture within a GLTF scene)
         * @tparam T Type of resource
         * @param id Resource ID
         * @param resource Pointer to resource data. Lifetime will now be managed by this manager.
         * @throws runtime_exception Unable create resource.
         * @return Pointer to new resource.
         */
        template <class T> Resource<T> addResource(const std::string& id, T* resource){
            const std::string id_with_type = id + typeid(T).name(); //Make sure multiple things can have the same ID as long as they have different types

            if(data.find(id_with_type) == data.end()){ //Not found
                ResourceData* resource_data = dynamic_cast<ResourceData*>(resource);
                if(resource_data == nullptr){ throw std::runtime_error("Resource type must derive ResourceData."); }
                data[id_with_type] = resource_data;
                return Resource<T>(resource);
            } else {
                throw std::runtime_error("Resource already exists.");
            }
        }

        /**
       * Get a file to access data from the filesystem.
       * @param id ID of file. The filename without the extension or directories.
       * @param allowed_extensions File extensions(with '.'( that are allowed, in order of preference(best first).
       * @return A file from which data can be read.
       * @throws runtime_exception Unable to find file
       */
        InFile requestFile(const std::string& id, const std::vector<std::string>& allowed_extensions) {
            std::optional<InFile> search = file_system.findFile(id, allowed_extensions);
            if(search) return search.value();
            throw std::runtime_error("Unable to find file: " + id);
        };

        //todo add additional metadata such as loading options. Could be structs or even JSON load config files.
        /**
         * Get a resource of the requested type
         * @tparam T Type of resource
         * @param id ID of resource. Usually boils down to the filename(without extension) of the data that will be eventually found on the filesystem.
         * @details Multiple resources can have the same ID if of different types. ID's can be mutated as they are passed through a dependency tree.
         * @example Requesting a PBR material of ID "wood". Material: "wood" -> Texture: "wood_roughness" & Texture: "wood_albedo" -> Image: "wood_roughness" & Image: "wood_albedo" -> "wood_roughness.jpg" & "wood_albedo.png"
         * @return Pointer to resource data
         * @throws runtime_exception Unable to find or create resource
         */
        template <class T> Resource<T> requestResource(const std::string& id) {
            const std::string id_with_type = id + typeid(T).name(); //Make sure multiple things can have the same ID as long as they have different types

            if(data.find(id_with_type) == data.end()){ //Not found
                T* new_resource_data = new T(this, id); //Create data and any dependencies if needed

                ResourceData* resource_data = dynamic_cast<ResourceData*>(new_resource_data);
                if(resource_data == nullptr){ throw std::runtime_error("Resource type must derive ResourceData."); }
                data[id_with_type] = resource_data;
                return Resource<T>(new_resource_data);
            }
            //Found
            return Resource<T>((T*)data[id_with_type]);
        };

        ~ResourceManager(){
            //Clean up remaining resources
            for (const auto& pair : data) {
                delete pair.second;
            }
        }
    };

} // Doge
