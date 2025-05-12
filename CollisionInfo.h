#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>

#include <ostream>

struct CollisionInfo {
    glm::vec3 normal;
    float penetration;
    bool collided;
};

inline std::ostream& operator<<(std::ostream& os, const CollisionInfo& collisionInfo) {
    os << "Collision Normal: " << collisionInfo.normal << ", penetration: " << collisionInfo.penetration << ", collided: " << collisionInfo.collided << "\n";
    return os;
}
