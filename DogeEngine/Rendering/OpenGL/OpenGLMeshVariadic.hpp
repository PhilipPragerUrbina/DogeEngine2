//
// Created by Philip on 4/1/2023.
//

#pragma once
#include "glad/glad.h"
#include <vector>
#include <stdexcept>
namespace Doge {

    /**
     * Unfinished experiment, to see if it is possible to create user defined vertex attribute layouts
     * @tparam Vertex_attributes 
     */
    template<typename... Vertex_attributes> class OpenGLMeshVariadic {
    private:
        std::vector<std::tuple<Vertex_attributes...>> vertices;
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
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(std::tuple<Vertex_attributes...>), vertices.data(), GL_STATIC_DRAW );

            glBindBuffer(GL_ARRAY_BUFFER, element_buffer_id);
            glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW );

 
            //todo swtich to array of  GL type enums, and check compatibility with shaders
            std::tuple<Vertex_attributes...> example;
            std::apply([](auto&&... args) {
                int i = 0;
                for(const auto arg : {args...}) {
                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
                    glEnableVertexAttribArray(i);
                    i++;
                }
                }, example);
           // glVertexAttribPointer(0)

            //Unbind
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);


        }
    public:

        /**
         * Create a mesh from mesh data
         * @param vertices The vertex data
         * @param indices The index data of same length
         * @throws runtime_error
         */
        OpenGLMeshVariadic(const std::vector<std::tuple<Vertex_attributes...>>&  vertices, const std::vector<unsigned int>& indices) : vertices(vertices), indices(indices){
            if(vertices.size() != indices.size()) { throw std::runtime_error("Index data is not same length as vertex data.");}
            loadMesh();
        };

        /**
         * Create a mesh from vertex data.
         * Will auto generate indices
         * @param vertices The vertex data
         */
        OpenGLMeshVariadic(const std::vector<std::tuple<Vertex_attributes...>>&  vertices) : vertices(vertices) {
            for (int i = 0; i < vertices.size(); ++i) {
                indices.push_back(i);
            }
            loadMesh();
        }



    };

} // Doge
