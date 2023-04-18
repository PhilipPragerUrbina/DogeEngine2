//
// Created by Philip on 4/17/2023.
//

#pragma once

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

#include "../Data/ResourceData.hpp"
#include "InFile.hpp"
#include "../Data/ResourceManager.hpp"
#include "../Rendering/OpenGL/OpenGLMesh.hpp"
#include "../Rendering/OpenGL/OpenGLTexture.hpp"

#include <iostream>
namespace Doge {

    class GLTFScene : public ResourceData{
    public:


        /**
         * Autoload GLTF scene
         * @param manager Manager to request from
         * @param id Resource ID to load
         * @throws runtime_error Problems reading file
         */
        GLTFScene(ResourceManager* manager, const std::string& id) {
            InFile file = manager->requestFile(id, {".glb", ".gltf"});

            tinygltf::Model gltf_model;
            tinygltf::TinyGLTF loader;

            std::string error;
            std::string warning;
            bool success;

            if(file.getExtension() == ".glb"){
                success = loader.LoadBinaryFromFile(&gltf_model, &error, &warning, file.getLocation());
            }else{ //.gltf
                success = loader.LoadASCIIFromFile(&gltf_model, &error, &warning, file.getLocation());
            }

            if (!warning.empty()) std::cout << "Warning: " << warning << "\n";
            if (!error.empty() || !success) throw std::runtime_error("Problems reading gltf file: " + file.getLocation() + " " + error);

            //Add image data
            //Can be requested later as a texture
            //Format for image ID is sceneid/texturename
            for(const auto& texture : gltf_model.textures){
                unsigned char* data = gltf_model.images[texture.source].image.data();
                int width = gltf_model.images[texture.source].width;
                int height = gltf_model.images[texture.source].height;
                int channels = gltf_model.images[texture.source].component;
                Image* image = new Image(width,height,channels,data);
                manager->addResource(id + "/" + texture.name, image);
            }
            //Add mesh data
            //Format for mesh id is sceneid/meshname/primitiveindex
            for(const auto& mesh : gltf_model.meshes) {
                int idx = 0;
                for(const auto& primitive : mesh.primitives){
                    std::vector<VertexAttributes> vertices;
                    std::vector<unsigned int > indices;

                    tinygltf::Accessor index_accessor = gltf_model.accessors[primitive.indices];
                    tinygltf::BufferView  index_buffer_view = gltf_model.bufferViews[index_accessor.bufferView];
                    tinygltf::Buffer  index_buffer = gltf_model.buffers[index_buffer_view.buffer];
                    if (index_accessor.ByteStride(index_buffer_view) == 2) {
                        const auto* index_data = reinterpret_cast<uint16_t*>(&index_buffer.data[index_buffer_view.byteOffset + index_accessor.byteOffset]);
                        for (int i = 0; i < index_accessor.count; ++i) {
                            indices.push_back((unsigned int)index_data[i]);
                        }
                    }else {
                        const auto* index_data = reinterpret_cast<uint32_t*>(&index_buffer.data[index_buffer_view.byteOffset + index_accessor.byteOffset]);
                        for (int i = 0; i < index_accessor.count; ++i) {
                            indices.push_back(index_data[i]);
                        }
                    }

                    //get tri positions
                    tinygltf::Accessor& accessor = gltf_model.accessors[primitive.attributes.at("POSITION")];
                    tinygltf::BufferView& buffer_view = gltf_model.bufferViews[accessor.bufferView];
                    tinygltf::Buffer& buffer = gltf_model.buffers[buffer_view.buffer];
                    const auto* positions = reinterpret_cast<const float*>(&buffer.data[buffer_view.byteOffset + accessor.byteOffset]);

                    //get tri normals
                    tinygltf::Accessor& accessor_1 = gltf_model.accessors[primitive.attributes.at("NORMAL")];
                    tinygltf::BufferView& buffer_view_1 = gltf_model.bufferViews[accessor_1.bufferView];
                    tinygltf::Buffer& buffer_1 = gltf_model.buffers[buffer_view_1.buffer];
                    const auto* normals = reinterpret_cast<const float*>(&buffer_1.data[buffer_view_1.byteOffset + accessor_1.byteOffset]);

                    //get tri texture coordinates
                    tinygltf::Accessor& accessor_2 = gltf_model.accessors[primitive.attributes.at("TEXCOORD_0")];
                    tinygltf::BufferView& buffer_view_2 = gltf_model.bufferViews[accessor_2.bufferView];
                    tinygltf::Buffer& buffer_2 = gltf_model.buffers[buffer_view_2.buffer];
                    const auto* texture_coordinates = reinterpret_cast<const float*>(&buffer_2.data[buffer_view_2.byteOffset + accessor_2.byteOffset]);

                    for (int i = 0; i < accessor.count ; ++i) {
                        VertexAttributes attributes;
                        attributes.position = glm::vec3(positions[i* 3 + 0], positions[i* 3 + 1],positions[i* 3 + 2]);
                        attributes.tex_coords = glm::vec2(texture_coordinates[i* 2 + 0], texture_coordinates[i* 2 + 1]);
                        attributes.normal = glm::vec3(normals[i* 3 + 0], normals[i* 3 + 1],normals[i* 3 + 2]);
                        vertices.push_back(attributes);
                    }
                    //todo tangents

                    OpenGLMesh* mesh_resource = new OpenGLMesh(vertices,indices);
                    manager->addResource(id + "/" + mesh.name + "/" + std::to_string(idx), mesh_resource);
                    idx++;
                }
            }

            //Add material data
            for(const auto& mesh : gltf_model.meshes) {


            }


        }

    };

} // Doge
