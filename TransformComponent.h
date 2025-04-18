#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace engine::component {
	struct TransformComponent {
        glm::vec3 position = glm::vec3(0.0f);
        glm::quat rotation = glm::quat(1.0f,0,0,0); // Euler angles in radians
        glm::vec3 scale = glm::vec3(1.0f);

        glm::mat4 getMatrix() const {
            glm::mat4 T = glm::translate(glm::mat4(1.0f), position);
            glm::mat4 R = glm::toMat4(rotation);
            glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);

            glm::mat4 modelMatrix = T * R * S;
            return modelMatrix;
        }

        glm::mat4 getInverseMatrix() const {
            return glm::inverse(getMatrix());
        }

        glm::vec3 up() const {
            return glm::rotate(rotation, { 0,1,0 });
        }

        glm::vec3 right() const {
            return glm::rotate(rotation, { 1,0,0 });
        }

        glm::vec3 forward() const {
            return glm::rotate(rotation, { 0,0,1 });
        }
	};
}