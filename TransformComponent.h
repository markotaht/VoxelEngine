#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace engine::component {
	struct TransformComponent {
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 rotation = glm::vec3(0.0f); // Euler angles in radians
        glm::vec3 scale = glm::vec3(1.0f);

        glm::mat4 getMatrix() const {
            glm::mat4 mat = glm::mat4(1.0f);
            mat = glm::translate(mat, position);
            mat = glm::rotate(mat, rotation.x, glm::vec3(1, 0, 0));
            mat = glm::rotate(mat, rotation.y, glm::vec3(0, 1, 0));
            mat = glm::rotate(mat, rotation.z, glm::vec3(0, 0, 1));
            mat = glm::scale(mat, scale);
            return mat;
        }

        glm::mat4 getInverseMatrix() const {
            return glm::inverse(getMatrix());
        }
	};
}