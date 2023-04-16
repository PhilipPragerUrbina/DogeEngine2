//
// Created by Philip on 4/1/2023.
//

#pragma once
#include <vector>
#include "OpenGLShaderProgram.hpp"
namespace Doge {

    /**
     * Manages the lifetime of shader programs
     * Makes sure material instances share the same underlying shader program.
     */
    class OpenGLShaderManager {
    private:
        std::vector<std::shared_ptr<OpenGLShaderProgram>> programs;
    public:

        /**
         * Link a material to this shader manager by getting a shader program from it
         * @param vertex Path to fragment binary that material uses
         * @param fragment Path to vertex binary that material uses
         * Should be called by material instance on creation.
         * @return The shader program for the material to use
         */
        std::shared_ptr<OpenGLShaderProgram> getShaderProgram(const std::string& vertex, const std::string& fragment){
            //Check if shader program already exists
            //Should only load a shader program if it has not been created yet.
            for (std::shared_ptr<OpenGLShaderProgram> program : programs) {
                if(program->isSame(vertex,fragment)) return program;
            }
            //Create new program
            std::shared_ptr<OpenGLShaderProgram> new_program = std::make_shared<OpenGLShaderProgram>(InFile(vertex),InFile(fragment));
            programs.push_back(new_program);
            return new_program;
        }

        /**
         * Get the shader programs that this manager manages for rendering
         */
        std::vector<std::shared_ptr<OpenGLShaderProgram>> getPrograms() const {
            return programs;
        }


        //todo reference counting for removeUnused()

    };

} // Doge
