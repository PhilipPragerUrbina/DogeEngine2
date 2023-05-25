//
// Created by Philip on 4/2/2023.
//

#pragma once
#include "glm.hpp"
#include <gtc/matrix_transform.hpp>
namespace Doge {

    class Camera {
    private:
        glm::mat4 projection;
        glm::mat4 transform; //view

    public:

        Camera(){
            transform = glm::lookAt(glm::vec3{0.0f,1.0f,0.0f}, {0.0f,0.0f,0.0f}, {0.0f,0.0f,1.0f});
            projection = glm::perspective(glm::radians(90.0f), 1.0f,0.1f,100.0f);
        }

        Camera(const glm::vec3& position, const glm::vec3& look_at, const glm::vec3& up = {0.0f,0.0f,1.0f}, float aspect_ratio = 1, float fov_degrees = 90) {
            transform = glm::lookAt(position, look_at, up);
            projection = glm::perspective(glm::radians(fov_degrees), aspect_ratio, 0.1f, 100000.0f);
        }

        //todo document and add helper methods. Very barebone right now.
        Camera(const glm::mat4 &projection, const glm::mat4 &transform) : projection(projection),
                                                                          transform(transform) {}


        const glm::mat4 &getProjection() const {
            return projection;
        }

        void setProjection(const glm::mat4 &new_projection) {
            projection = new_projection;
        }

        const glm::mat4 &getTransform() const {
            return transform;
        }

        void setTransform(const glm::mat4 &new_transform) {
           transform = new_transform;
        }

    };

} // Doge
