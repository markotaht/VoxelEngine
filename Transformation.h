#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H
#pragma once

#include <gl/glew.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

class Transformation
{
	glm::mat4x4 modelMatrix;
	glm::mat4x4 positionMatrix;
	glm::mat4x4 scaleMatrix;
	glm::quat rotationQuat;

	void defaultPosition();
	void defaultScale();
	void defaultRotation();

	void recalculateModelMatrix();

public:
	Transformation();

	glm::vec3 getPosition();
	void setPosition(glm::vec3 newPos);

	glm::vec3 getScale();
	void setScale(glm::vec3 newScale);

	glm::vec3 getRotation();
	void setRotation(GLfloat t, GLfloat x, GLfloat y, GLfloat z);
	void setRotation(GLfloat t, glm::vec3 axis);
	void setRotation(glm::quat rot);

	void rotate(GLfloat t, GLfloat x, GLfloat y, GLfloat z);
	void rotate(GLfloat t, glm::vec3 axis);
	void rotate(glm::quat rot);

	void rotateGlobal(GLfloat t, glm::vec3 axis);
	void rotateGlobal(glm::quat rot);


	glm::quat getRotationQuaternion() { return rotationQuat; }

	void translate(glm::vec3 shift);

	glm::mat4x4 M();
};

#endif
