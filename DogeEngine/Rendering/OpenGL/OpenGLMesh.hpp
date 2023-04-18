//
// Created by Philip on 4/1/2023.
//

#pragma once
#include "glad/glad.h"
#include <vector>
#include <stdexcept>
#include "glm.hpp"
#include "../API/VertexAttributes.hpp"
#include "../../Data/ResourceData.hpp"
#include "../../Data/ResourceManager.hpp"

namespace Doge {

    //todo separate mesh and opengl mesh. At least think it over. Image->texture.
    /**
     * A standard mesh
     */
    class OpenGLMesh : public ResourceData{
    private:
        std::vector<VertexAttributes> vertices;
        std::vector<unsigned int> indices;

        //todo dynamic geometry mode or separate class

        unsigned int vertex_buffer_id, vertex_array_id, element_buffer_id;

        /**
         * Load the mesh into video memory
         */
        void loadMesh(){
            //Create buffers
            glGenVertexArrays(1, &vertex_array_id);
            glGenBuffers(1, &vertex_buffer_id);
            glGenBuffers(1, &element_buffer_id);

            glBindVertexArray(vertex_array_id);

            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
            glBufferData(GL_ARRAY_BUFFER,   vertices.size()*sizeof(VertexAttributes), &vertices[0], GL_STATIC_DRAW );

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_id);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,  indices.size()*sizeof(unsigned int) , &indices[0], GL_STATIC_DRAW );


            //position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributes), (void*)0);
            glEnableVertexAttribArray(0);
            //normal attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributes), (void*)offsetof(VertexAttributes, normal));
            glEnableVertexAttribArray(1);
            //texture coord attribute
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAttributes), (void*)offsetof(VertexAttributes, tex_coords));
            glEnableVertexAttribArray(2);


            //Unbind
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


        }
    public:


        /**
         * Create a mesh from mesh data
         * @param vertices The vertex data
         * @param indices The index data
         */
        OpenGLMesh(const std::vector<VertexAttributes>&  vertices, const std::vector<unsigned int>& indices) : vertices(vertices), indices(indices){
            loadMesh();
        };

        /**
       * Autoload texture
       * @param manager Manager to request image from
       * @param ID Resource ID
       */
        OpenGLMesh(ResourceManager* manager, const std::string& ID) {
            throw std::runtime_error("Unable to find mesh: " + ID + ". Check that the requested resource ID is spelled correctly.");
        }
        //todo create autoload


        /**
         * Draw the mesh
         */
        void render(){
            glBindVertexArray(vertex_array_id);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        ~OpenGLMesh(){
            glDeleteVertexArrays(1, &vertex_array_id);
            glDeleteBuffers(1, &vertex_buffer_id);
            glDeleteBuffers(1, &element_buffer_id);
        }

    };

} // Doge
