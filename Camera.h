#pragma once
#include "SceneNode.h"
#include "InputHandler.h"
#include <iostream>
#include <math.h>
#include <gl/glew.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera : public SceneNode
{
	glm::mat4x4 viewMatrix;
	glm::mat4x4 projectionMatrix;

	glm::vec3 forward;
	glm::vec2 oldPos;

	InputHandler handler;
	GLfloat count = 0;
public:
	Camera();

	glm::vec3 getFroward() {
		return transform.getRotationQuaternion() * forward;
	}

	glm::mat4x4 lookAt(glm::vec3 loc, glm::vec3 to);
	glm::mat4x4 getViewMatrix() { return viewMatrix; }

	void setOrtographic();
	void setPerspective(GLfloat near = 1.0f);
	glm::mat4x4 getProjectionMatrix() { return projectionMatrix; }

	glm::mat4x4 getProjectionViewMatrix() { return projectionMatrix * viewMatrix; }

	void update(float deltaTime);
	void render(glm::mat4x4 projectionMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 parentTransform);
};

