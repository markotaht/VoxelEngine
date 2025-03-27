#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace engine::component {
    struct CameraComponent {
        float fov = 60.0f;
        float nearPlane = 0.1f;
        float farPlane = 1000.0f;
        float aspectRatio = 640.0f / 480.0f;

        glm::mat4 getProjectionMatrix() const {
            return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
        }
    };
}