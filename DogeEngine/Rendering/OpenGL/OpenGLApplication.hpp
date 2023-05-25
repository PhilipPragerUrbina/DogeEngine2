//
// Created by Philip on 4/1/2023.
//

#pragma once
#include <glad/glad.h>
#include <glm.hpp>
#include "../UI/WindowApplication.hpp"
#include "../API/DrawRequest.hpp"
#include "../../../Deprecated/OpenGLShaderManager.hpp"
#include <unordered_set>

namespace Doge {

    auto shader_hash = [](const Resource<OpenGLShaderProgram>& a) { return a->getProgramID(); }; //For making set unique though the resource pointer

    /**
     * Manages everything related to opengl
     */
    class OpenGLApplication {
    private:
        // version 4.6
        WindowApplication<OPENGL_CORE,4,6> window;

        std::vector<DrawRequest> render_queue;


        std::unordered_set<Resource<OpenGLShaderProgram>, decltype(shader_hash)> shaders{10,shader_hash}; //Unique list of shader programs


        RenderData active_render_data;

        glm::vec3 background_color;

        /**
         * Render a frame
         */
        void render(){
            for(Resource<OpenGLShaderProgram> shader : shaders ){
                shader->use();
                for (const DrawRequest& draw_call : render_queue) {
                    if(shader->isSame(draw_call.getMaterial()->getShaderProgram())){
                        active_render_data.object_transform = draw_call.getTransform();
                        draw_call.getMaterial()->setUniforms(active_render_data);
                        draw_call.getMesh()->render();
                    }
                }
            }
            //todo deferred rendering path: https://learnopengl.com/Advanced-Lighting/Deferred-Shading
            render_queue.clear();
        }
    public:

        /**
         * Start the application
         * @param start_width Start window size
         * @param start_height Start window size
         * @param application_name Name of application and window
         * @throw runtime_error
         */
        explicit OpenGLApplication(const std::string& application_name,unsigned int start_width = 500, unsigned int start_height = 500) : window((int)start_width,(int)start_height, application_name){
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                throw std::runtime_error ("Error initializing glad.");
            }

            glEnable(GL_DEPTH_TEST);

            glViewport(0,0,(int)start_width,(int)start_height);

        }

        /**
         * Update the application
         * @return True if it should keep running
         */
        bool run(){


            glClearColor(background_color.x,background_color.y, background_color.z, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            render();

            window.update();

            if(window.checkResized()){
                glViewport(0,0, window.getWidth(),window.getHeight());
            }
            return window.open();
        }

        /**
         * Set the currently active camera for rendering
         * @param camera New active camera
         */
        void setActiveCamera(const Camera& camera){
            active_render_data.camera = camera;
        }


        /**
         * Request to draw a mesh and material
         * @param mesh Mesh to draw
         * @param material Material to use.
         * @warning Material must have been created using this application's shader manager using getShaderManager()
         * @param transform The transform of the object to draw
         */
        void draw(OpenGLMesh* mesh, Material* material, const glm::mat4& transform){
            shaders.insert(material->getShaderProgram()); //Build unique list of shaders
            render_queue.emplace_back(mesh,material,transform);
        }


        /**
         * Set the background color
         * @param new_color Color to render behind objects in RGB
         */
        void setBackGroundColor(const glm::vec3& new_color){
            background_color = new_color;
        }



    };

} // Doge
