#include "Camera.h"
#include "KeyboardHandler.h"

#include <stdio.h>
Camera::Camera()
{
	forward[2] = 1;
	setPerspective(1);
	transform.setPosition(glm::vec3(0, 0, 0));
	viewMatrix = glm::mat4();
	lookAt(transform.getPosition(), transform.getPosition() + forward);
	//viewMatrix = new Matrix4x4(transform.M().inverse());

}

glm::mat4x4 Camera::lookAt(glm::vec3 loc, glm::vec3 to)
{
	glm::vec3 up(0, 1, 0);
	glm::vec3 forward = glm::normalize(loc - to);
	glm::vec3 right = glm::normalize(glm::cross(up,forward));
	up = glm::cross(forward, right);

	glm::mat4 orientationMatrix(
		glm::vec4(right, 0.f),
		glm::vec4(up, 0.f),
		glm::vec4(forward, 0.f),
		glm::vec4(0.f, 0.f, 0.f, 1.f));

	glm::mat4 positionMatrix(1.0f);
	positionMatrix[3] = glm::vec4(loc, 1.f);

	viewMatrix = positionMatrix * orientationMatrix;
	return viewMatrix;
}

void Camera::setOrtographic()
{
	projectionMatrix = glm::mat4x4();
	projectionMatrix[0][0] = 1;
	projectionMatrix[1][1] = 1;
	projectionMatrix[3][3] = 1;
}

void Camera::setPerspective(GLfloat near)
{
	projectionMatrix = glm::perspective(
		glm::radians(60.f),
		640.0f / 480.0f,
		0.1f,
		1000.0f
	);
}

void Camera::update(float deltaTime)
{	
	//printf("Camera Update");
	count += deltaTime;
	glm::vec2 newPos = handler.getMousePosition();
	glm::vec2 dif = newPos - oldPos;

	oldPos = newPos;

	GLfloat rotY = glm::radians(dif[0]);
	GLfloat rotX = glm::radians(dif[1]);

	transform.rotateGlobal(-rotX, glm::vec3(1.f, 0.f, 0.f));
	transform.rotate(-rotY, glm::vec3(0.f, 1.f, 0.f));
	viewMatrix = glm::inverse(transform.M());

	if (KeyboardHandler::getInstance()->isPressed(SDL_SCANCODE_A)) {
		transform.translate(transform.getRotationQuaternion() * glm::vec3(-1.f, 0.f, 0.f) * deltaTime);
	}

	if (KeyboardHandler::getInstance()->isPressed(SDL_SCANCODE_D)) {
		transform.translate(transform.getRotationQuaternion() *  glm::vec3(1.f, 0.f, 0.f) * deltaTime);
	}

	if (KeyboardHandler::getInstance()->isPressed(SDL_SCANCODE_W)) {
		transform.translate(transform.getRotationQuaternion() * glm::vec3(0.f, 0.f, -1.f) * deltaTime);
	}

	if (KeyboardHandler::getInstance()->isPressed(SDL_SCANCODE_S)) {
		transform.translate(transform.getRotationQuaternion() * glm::vec3(0.f, 0.f, 1.f) * deltaTime);
	}

	//std::cout << transform.getRotation()[0] << transform.getRotation()[1] << transform.getRotation()[2] << std::endl;
}

void Camera::render(glm::mat4x4 projectionMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 parentTransform)
{
	//SceneNode::draw(projectionMatrix, viewMatrix, transform.M()*parentTransform);
}
