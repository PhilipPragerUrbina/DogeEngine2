//
// Created by Philip on 4/2/2023.
//

#pragma once

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "../Rendering/API/VertexAttributes.hpp"
#include <stdexcept>
#include "InFile.hpp"

namespace Doge {

    /**
     * Load OBJ files into memory
     */
    class OBJMesh {
    private:

        std::vector<VertexAttributes> attributes;
        std::vector<unsigned int> indices;

    public:

        /**
         * Load an obj file into memory
         * @param input_file File to load
         * @param invert_normals Some models have normals that are flipped
         * @throws runtime_error Problem opening file
         */
        OBJMesh(const InFile& input_file , bool  invert_normals = false) {

            tinyobj::ObjReaderConfig reader_config;
            reader_config.mtl_search_path = "./";
            tinyobj::ObjReader reader;

            if (!reader.ParseFromFile(input_file.getLocation(), reader_config)) {
                throw std::runtime_error("Issue reading obj file: " + reader.Error());
            }

            auto &vertices = reader.GetAttrib();
            auto &shapes = reader.GetShapes();

            int index = 0;

            for (const auto & shape : shapes) {

                //Loop over faces
                size_t index_offset = 0;
                for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {

                    size_t fv = shape.mesh.num_face_vertices[f];
                    //Loop over vertices in the face.
                    for (size_t v = 0; v < fv; v++) {

                        indices.push_back(index);
                        index++;

                        VertexAttributes attribute{};

                        tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                        tinyobj::real_t vx = vertices.vertices[3 * size_t(idx.vertex_index) + 0];
                        tinyobj::real_t vy = vertices.vertices[3 * size_t(idx.vertex_index) + 1];
                        tinyobj::real_t vz = vertices.vertices[3 * size_t(idx.vertex_index) + 2];
                        attribute.position = glm::vec3 {vx,vy,vz};

                        //Check if normal data is available
                        if (idx.normal_index >= 0) {
                            tinyobj::real_t nx = vertices.normals[3 * size_t(idx.normal_index) + 0];
                            tinyobj::real_t ny = vertices.normals[3 * size_t(idx.normal_index) + 1];
                            tinyobj::real_t nz = vertices.normals[3 * size_t(idx.normal_index) + 2];
                            attribute.normal = glm::vec3 {nx,ny,nz};
                        }else{
                            attribute.normal = glm::vec3 {0,0,0};
                            if(invert_normals){
                                attribute.normal = - attribute.normal;
                            }
                        }

                        //Check if texture coordinate data is available
                        if (idx.texcoord_index >= 0) {
                            tinyobj::real_t tx = vertices.texcoords[2 * size_t(idx.texcoord_index) + 0];
                            tinyobj::real_t ty = vertices.texcoords[2 * size_t(idx.texcoord_index) + 1];
                            attribute.tex_coords = {tx,ty};

                        }else{
                            attribute.tex_coords = {0.5,0.5};
                        }
                        attributes.push_back(attribute);
                    }
                    index_offset += fv;
                }
            }
        }

        /**
         * Get loaded vertex attributes
         */
        std::vector<VertexAttributes> getAttributes() const {
            return attributes;
        }

        /**
         * Get loaded indices to go with attributes
         */
        std::vector<unsigned int> getIndices() const {
            return indices;
        }
    };

} // Doge
