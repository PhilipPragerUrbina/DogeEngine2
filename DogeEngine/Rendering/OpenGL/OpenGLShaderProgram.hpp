//
// Created by Philip on 4/1/2023.
//

#pragma once
#include <glad/glad.h>
#include <vector>
#include <fstream>
#include <glm.hpp>
#include "../../IO/InFile.hpp"

namespace Doge {

    /**
     * Contains vertex and fragment shaders in a shader program
     */
    class OpenGLShaderProgram {
    private:
        unsigned int program_id;

        //for comparison
        std::string vertex_location;
        std::string  fragment_location;

    public:

        /**
         * Create a shader program from shader binaries
         * @warning Expects shaders to be SPIR-V .spv files
         * @param vertex_file Vertex shader spv file
         * @param fragment_file Fragment shader spv file
         * @param entry_point The name of the main method in both shaders
         * @throws runtime_error
         */
        OpenGLShaderProgram(const InFile& vertex_file, const InFile& fragment_file, const std::string& entry_point = "main") : fragment_location(fragment_file.getLocation()), vertex_location(vertex_file.getLocation()){
            //Read from files
            std::vector<char> vertex_bytes = vertex_file.readFile();
            std::vector<char> fragment_bytes = fragment_file.readFile();

            unsigned int vertex_id, fragment_id;

            //Create vertex shader
            vertex_id = glCreateShader(GL_VERTEX_SHADER);
            glShaderBinary(1, &vertex_id, GL_SHADER_BINARY_FORMAT_SPIR_V, vertex_bytes.data(), (int)vertex_bytes.size());
            glSpecializeShader(vertex_id, entry_point.c_str(), 0, nullptr, nullptr); //todo set constants here
            //Validate
            int compiled = 0;
            char info_log[512];
            glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &compiled);
            if(!compiled) {
                glGetShaderInfoLog(vertex_id, 512, nullptr, info_log);
                throw std::runtime_error("Vertex shader failed compilation. " + std::string(info_log));
            }

            //Create fragment shader
            fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderBinary(1, &fragment_id, GL_SHADER_BINARY_FORMAT_SPIR_V, fragment_bytes.data(), (int)fragment_bytes.size());
            glSpecializeShader(fragment_id, entry_point.c_str(), 0, nullptr, nullptr); //todo set constants here
            //Validate
            glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &compiled);
            if(!compiled) {
                glGetShaderInfoLog(fragment_id, 512, nullptr, info_log);
                throw std::runtime_error("Fragment shader failed compilation. " + std::string(info_log));
            }

            //Create shader program
            program_id = glCreateProgram();
            glAttachShader(program_id,vertex_id);
            glAttachShader(program_id,fragment_id);
            glLinkProgram(program_id);
            //Validate
            glGetProgramiv(program_id, GL_LINK_STATUS, &compiled);
            if(!compiled)
            {
                glGetProgramInfoLog(program_id, 512, NULL, info_log);
                throw std::runtime_error("Shader program failed compilation. " + std::string(info_log));
            }
            //Clean up shaders as they are no longer needed once linked
            glDeleteShader(vertex_id);
            glDeleteShader(fragment_id);
            //todo Create a shader class that goes within the program class to allow for less redundancy and greater flexibility(more similar to vulkan, constants,deferred binding, etc)
        }


        /**
         * Check if the files are the same as the files that created this shader program
         * @param other_vertex_location Vertex binary location
         * @param other_fragment_location Fragment binary location
         * @return True if same
         */
        bool isSame(const std::string& other_vertex_location, const std::string& other_fragment_location) const {
            return other_fragment_location == fragment_location && other_vertex_location == vertex_location;
        }

        /**
         * Set the program as the currently active shader program
         */
        void use() const{
            glUseProgram(program_id);
        }


        /**
         * Bind a UBO(Uniform Buffer Object) to this shader
         * @param name Name of UBO in shader
         * @param binding_point Location of UBO
         */
        void bindUBO(const std::string& name , unsigned int binding_point) const {
            unsigned int index= glGetUniformBlockIndex(program_id, name.c_str());
            glUniformBlockBinding(program_id, index, binding_point);
        }

        /**
         * Set a texture in the shader
         * @param texture_name Name of texture sampler uniform
         * @param texture_id The texture to bind
         * @param texture_count Starts at 0, and should be incremented for every additional texture that is set. For example, albedo:0, roughness:1, normal:2,etc.
         */
        void setTexture(const std::string& texture_name, unsigned int texture_id, int texture_unit) const {
            glActiveTexture(GL_TEXTURE0 + texture_unit);
            glBindTexture(GL_TEXTURE_2D, texture_id);
            setUniform(texture_name, texture_unit);
        }

        //Shader uniform methods. Set shader uniforms of name to a value

        void setUniform(const std::string& name, bool value) const{
            glUniform1i(glGetUniformLocation(program_id, name.c_str()), (int)value);
        }

        void setUniform(const std::string& name, int value) const{
            glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
        }

        void setUniform(const std::string& name, float value) const{
            glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
        }

        void setUniform(const std::string& name, const glm::vec2& value) const{
            glUniform2fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
        }

        void setUniform(const std::string& name, const glm::vec3& value) const{
            glUniform3fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
        }

        void setUniform(const std::string& name, const glm::vec4& value) const{
            glUniform4fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
        }

        void setUniform(const std::string &name, const glm::mat2& value) const
        {
            glUniformMatrix2fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, &value[0][0]);
        }

        void setUniform(const std::string &name, const glm::mat3& value) const
        {
            glUniformMatrix3fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, &value[0][0]);
        }

        void setUniform(const std::string &name, const glm::mat4& value) const
        {
            glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, &value[0][0]);
        }

        ~OpenGLShaderProgram(){
            glDeleteProgram(program_id); //Remove program once no longer needed
        }
    };

} // Doge
