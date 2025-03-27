#pragma once
#include "NewSceneNode.h"

namespace engine::scene {
	class Camera : public SceneNode {
	public:
		void update(float deltaTime) {}

        float fov = 60.0f;
        float nearPlane = 0.1f;
        float farPlane = 1000.0f;
        float aspectRatio = 640.0f / 480.0f;

        glm::mat4 getViewMatrix() const {
            return glm::inverse(transform.getMatrix());
        }

        glm::mat4 getProjectionMatrix() const {
            return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
        }
	};
}