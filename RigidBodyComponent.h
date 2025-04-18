#pragma once
#include <glm/glm.hpp>

namespace engine::component {
    struct RigidBodyComponent {
        glm::vec3 velocity = glm::vec3(0.0f);
        glm::vec3 acceleration = glm::vec3(0.0f);
        float mass = 1.0f;
        bool isStatic = false;
    };
}