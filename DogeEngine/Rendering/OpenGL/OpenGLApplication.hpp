//
// Created by Philip on 4/1/2023.
//

#pragma once
#include <glad/glad.h>
#include <glm.hpp>
#include "../UI/WindowApplication.hpp"
#include "../API/DrawRequest.hpp"


namespace Doge {

    /**
     * Manages everything related to opengl
     */
    class OpenGLApplication {
    private:
        // version 4.6
        WindowApplication<OPENGL_CORE,4,6> window;

        std::vector<DrawRequest> render_queue;

        OpenGLShaderManager manager;

        RenderData active_render_data;

        glm::vec3 background_color;

        /**
         * Render a frame
         */
        void render(){
            for(std::shared_ptr<OpenGLShaderProgram> shader : manager.getPrograms() ){
                shader->use();
                for (const DrawRequest& draw_call : render_queue) {
                    if(shader->isSame(draw_call.getMaterial()->getVertexLocation(), draw_call.getMaterial()->getFragmentLocation())){
                        active_render_data.object_transform = draw_call.getTransform();
                        draw_call.getMaterial()->setUniforms(active_render_data);
                        draw_call.getMesh()->render();
                        //todo not reset uniforms if same material
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
            render_queue.emplace_back(mesh,material,transform);
        }

        /**
         * Get teh shader manager in order to create materials
         */
        OpenGLShaderManager* getShaderManager(){
            return &manager;
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
