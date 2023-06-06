#include "Transformation.h"
#include <iostream>

void Transformation::defaultPosition()
{
	positionMatrix = glm::mat4(1.0f);
}

void Transformation::defaultScale()
{
	scaleMatrix = glm::mat4(1.0f);
}

void Transformation::defaultRotation()
{
	rotationQuat = glm::quat(1.0, 0, 0, 0);
}
void Transformation::recalculateModelMatrix()
{
	modelMatrix = positionMatrix * glm::toMat4(rotationQuat) * scaleMatrix;
}
Transformation::Transformation()
{
	defaultPosition();
	defaultScale();
	defaultRotation();
	recalculateModelMatrix();
}

glm::vec3 Transformation::getPosition()
{
	glm::vec3 res;
	res[0] = positionMatrix[3][0];
	res[1] = positionMatrix[3][1];
	res[2] = positionMatrix[3][2];
	return res;
}

void Transformation::setPosition(glm::vec3 newPos)
{
	positionMatrix[3][0] = newPos[0];
	positionMatrix[3][1] = newPos[1];
	positionMatrix[3][2] = newPos[2];
	recalculateModelMatrix();
}

glm::vec3 Transformation::getScale()
{
	glm::vec3 res;
	res[0] = scaleMatrix[0][0];
	res[1] = scaleMatrix[1][1];
	res[2] = scaleMatrix[2][2];
	return res;
}

void Transformation::setScale(glm::vec3 newScale)
{
	scaleMatrix = glm::scale(newScale);
	recalculateModelMatrix();
}

glm::vec3 Transformation::getRotation()
{
	return glm::eulerAngles(rotationQuat) * 3.14159f / 180.f;
}

void Transformation::setRotation(GLfloat t, GLfloat x, GLfloat y, GLfloat z)
{
	rotationQuat = glm::quat(t, x, y, z);
	recalculateModelMatrix();
}

void Transformation::setRotation(GLfloat t, glm::vec3 axis)
{

	glm::vec3 norm = glm::normalize(axis);

	float w = glm::cos(t / 2);
	float v = glm::sin(t / 2);
	glm::vec3 qv = norm * v;

	glm::quat rot(w, qv);

	setRotation(rot);
}

void Transformation::setRotation(glm::quat rot) {
	rotationQuat = rot;
	recalculateModelMatrix();
}

void Transformation::rotate(GLfloat t, GLfloat x, GLfloat y, GLfloat z)
{
	rotationQuat = rotationQuat * glm::quat(t, x, y, z);
	recalculateModelMatrix();
}

void Transformation::rotate(GLfloat t, glm::vec3 axis)
{
	glm::vec3 norm = glm::normalize(axis);

	float w = glm::cos(t / 2);
	float v = glm::sin(t / 2);
	glm::vec3 qv = norm * v;

	glm::quat rot(w, qv);
	rotate(rot);
}

void Transformation::rotate(glm::quat rot) {
	rotationQuat = rot * rotationQuat;

	recalculateModelMatrix();
}

void Transformation::rotateGlobal(GLfloat t, glm::vec3 axis)
{
	glm::vec3 norm = glm::normalize(axis);

	float w = glm::cos(t / 2);
	float v = glm::sin(t / 2);
	glm::vec3 qv = norm * v;

	glm::quat rot(w, qv);
	rotateGlobal(rot);

}

void Transformation::rotateGlobal(glm::quat rot) {
	rotationQuat = rotationQuat * rot;

	recalculateModelMatrix();
}

void Transformation::translate(glm::vec3 shift)
{
	positionMatrix[3][0] += shift[0];
	positionMatrix[3][1] += shift[1];
	positionMatrix[3][2] += shift[2];

	recalculateModelMatrix();
}

glm::mat4x4 Transformation::M()
{
	return modelMatrix;
}
