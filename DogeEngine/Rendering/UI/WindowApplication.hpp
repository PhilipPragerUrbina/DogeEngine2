//
// Created by Philip on 2/28/2023.
//

#pragma once
#include <glfw3.h>
#include <stdexcept>

namespace Doge {

    /**
     * Which graphics API GLFW should set up for
     */
    enum GraphicsAPI {
        OPENGL_CORE
    };

    /**
     * A GLFW window wrapper. For single window glfw applications.
     * @tparam API Currently only supports OpenGL core
     * @tparam MAJOR_VERSION Major graphics API version number
     * @tparam MINOR_VERSION Minor graphics API version number
     */
    template <const GraphicsAPI API, const int MAJOR_VERSION, const int MINOR_VERSION> class WindowApplication {
    private:
        GLFWwindow* glfw_window;
        bool is_resized = false;
    public:
        /**
         * Create a window and init GLFW.
         * @param start_width Dimensions
         * @param start_height Dimensions
         * @param sample_num Number of glfw samples for anti-aliasing
         * @throws runtime_error
         */
        WindowApplication(int start_width, int start_height, const std::string& name, const int sample_num = 1){
            if(glfwInit() != GLFW_TRUE){
                throw std::runtime_error("Error initializing glfw");
            }

            glfwWindowHint(GLFW_SAMPLES, sample_num);

            //Set Opengl version
            if(API == OPENGL_CORE){
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR_VERSION);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR_VERSION);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            }

            glfw_window = glfwCreateWindow( start_width, start_height , name.c_str() , nullptr , nullptr);
            if(glfw_window == nullptr){
                throw std::runtime_error("Error creating glfw window.");
            }

            glfwMakeContextCurrent(glfw_window);


            glfwSetWindowUserPointer(glfw_window, this); //Allow callbacks to reference this class
            glfwSetFramebufferSizeCallback(glfw_window, framebufferResizeCallback);
        }

        //todo lambda event system

        /**
         * Marks the framebuffer as resized
         */
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
            auto app = reinterpret_cast<WindowApplication*>(glfwGetWindowUserPointer(window));
            app->is_resized = true;
        }

        /**
         * True if window has been resized.
         * If it has been resized this check will mark it as not resized anymore
         */
        bool checkResized(){
            if(is_resized){
                is_resized = false;
                return true;
            }
            return false;
        }

        /**
         * Get the GLFW window
         */
        GLFWwindow* getWindow() const {
            return glfw_window;
        }

        /**
         * Get current width of window
         */
        int getWidth() const {
            int width;
            glfwGetWindowSize(glfw_window,&width, nullptr);
            return width;
        }

        /**
         * Get current height of window
         */
        int getHeight() const{
            int height;
            glfwGetWindowSize(glfw_window,nullptr, &height);
            return height;
        }

        /**
         * True if window should not close
         */
        bool open() const {
            return !glfwWindowShouldClose(glfw_window);
        }

        /**
         * Update the window by polling events
         */
        void update(){
            glfwSwapBuffers(glfw_window);
            glfwPollEvents();
        }

        ~WindowApplication(){
            glfwDestroyWindow(glfw_window);
            glfwTerminate();
        }

    };

} // Doge
