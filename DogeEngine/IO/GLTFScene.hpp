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
#include "../ECS/ECSManager.hpp"
#include "../Rendering/Shaders/Materials/GLTFMaterial.hpp"
#include "../ECS/Components/TransformComponent.hpp"
#include "../ECS/Components/HierarchyComponent.hpp"
#include "../ECS/Components/RenderComponent.hpp"
#include <gtx/quaternion.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
namespace Doge {

    class GLTFScene : public ResourceData{
    public:

        tinygltf::Model gltf_model;
        std::string ID;

        /**
         * Autoload GLTF scene
         * @param manager Manager to request from
         * @param id Resource ID to load
         * @throws runtime_error Problems reading file
         */
        GLTFScene(ResourceManager* manager, const std::string& id) {
            InFile file = manager->requestFile(id, {".glb", ".gltf"});

           ID = id;
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
            //Format for image ID is sceneid/textureidx
            int tex_idx = 0;
            for(const auto& texture : gltf_model.textures){
                unsigned char* data = gltf_model.images[texture.source].image.data(); //todo ownershup here is wierd
                int width = gltf_model.images[texture.source].width;
                int height = gltf_model.images[texture.source].height;
                int channels = gltf_model.images[texture.source].component;
                Image* image = new Image(width,height,channels,data);
                manager->addResource(id + "/" + std::to_string(tex_idx), image);
                tex_idx++;
            }

            int mat_id = 0;
            for(const auto& material : gltf_model.materials){
                if(material.pbrMetallicRoughness.baseColorTexture.index < 0){
                    glm::vec3 color = {material.pbrMetallicRoughness.baseColorFactor[0],
                                       material.pbrMetallicRoughness.baseColorFactor[1],
                                       material.pbrMetallicRoughness.baseColorFactor[2]};
                    manager->addResource(ID + "/" +  std::to_string(mat_id), (Material*)new SimpleShadedMaterial( manager, color));
                }else{
                    Resource<OpenGLTexture> color = manager->requestResource<OpenGLTexture>(ID + "/" +  std::to_string(material.pbrMetallicRoughness.baseColorTexture.index));
                    manager->addResource(ID + "/" +  std::to_string(mat_id), (Material*)new GLTFMaterial( manager, color));
                }
                mat_id++;
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

        }

        /**
           * Generate the scene as components
           */
        void create(ECSManager* manager, ResourceManager* rc, Entity parent){ //todo combine shader manager into resource manager
            //todo parent option

            for (int node : gltf_model.scenes[gltf_model.defaultScene].nodes) {
                Entity child = recurse(manager,rc, gltf_model.nodes[node], parent);
                HierarchyComponent* parent_h = manager->getComponent<HierarchyComponent>(parent);
              parent_h->addChild( child );
            }

        }

        Entity recurse(ECSManager* manager, ResourceManager* rc,const tinygltf::Node& current_node,const Entity& parent){
            Entity current_entity = manager->createEntity();
            if(!current_node.matrix.empty()) {
                manager->addComponent(current_entity, TransformComponent(glm::make_mat4(current_node.matrix.data())));;
            }else{
                glm::mat4 TRS(1.0f);
                if(!current_node.translation.empty()){
                    TRS = glm::translate(TRS, {current_node.translation[0], current_node.translation[1],current_node.translation[2]});
                }
                if(!current_node.rotation.empty()){
                    TRS = TRS * glm::toMat4(glm::quat(current_node.rotation[0], current_node.rotation[1],current_node.rotation[2],current_node.rotation[3] ));
                }
                if(!current_node.scale.empty()){
                    TRS = glm::scale(TRS, {current_node.scale[0], current_node.scale[1],current_node.scale[2]});
                }


                manager->addComponent(current_entity,  TransformComponent(TRS));;
            }


            HierarchyComponent   hierarchy_component;
            hierarchy_component = HierarchyComponent(parent);




            for (int child : current_node.children) {
               hierarchy_component.addChild(recurse(manager,rc, gltf_model.nodes[child], current_entity));
            }
            generateMesh(manager,rc,&hierarchy_component,current_node,current_entity);
            manager->addComponent(current_entity, hierarchy_component);
            return current_entity;
        }

        void generateMesh( ECSManager* manager, ResourceManager* rc, HierarchyComponent* parent_hierarchy, const tinygltf::Node& node, Entity parent){
            if(node.mesh < 0){
                return;
            }
            for (int i = 0; i < gltf_model.meshes[node.mesh].primitives.size(); ++i) {
                Resource<OpenGLMesh> mesh = rc->requestResource<OpenGLMesh>(ID + "/" + gltf_model.meshes[node.mesh].name + "/" + std::to_string(i));




                Entity child = manager->createEntity();


                //todo proper sub resource support. No mor '/'
                    manager->addComponent(child, RenderComponent(mesh, rc->requestResource<Material>(ID + "/" + std::to_string(gltf_model.meshes[node.mesh].primitives[i].material))));



                parent_hierarchy->addChild(child);
                manager->addComponent(child, HierarchyComponent(parent)); //todo auto set parent on child add
                manager->addComponent(child, TransformComponent(glm::mat4(1.0f)));


            }


        }

    };

} // Doge
