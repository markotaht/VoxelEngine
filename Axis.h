#pragma once
#include "GameObject.h"
#include "ShaderProgram.h"
class Axis :
	public GameObject
{
public:
	Axis(ShaderProgram* shaderProgram);

	void render(glm::mat4x4 projectionMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 parentTransform);
private:
	void init();

	ShaderProgram* shaderProgram;

	GLfloat* axisVerts;
	GLfloat* axisColors;
	GLuint* axisIndices;
};

