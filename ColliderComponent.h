#pragma once
#include <glm/glm.hpp>

namespace engine::component {
    enum class ColliderType { AABB, Sphere };

    struct ColliderComponent {
        ColliderType type;
        glm::vec3 size; // For AABB: half-extents
        float radius;   // For sphere
    };
}