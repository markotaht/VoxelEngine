#pragma once
#include "GameObject.h"
#include "Material.h"
class Axis :
	public GameObject
{
public:
	Axis(Material* material);

	void render(glm::mat4x4 projectionMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 parentTransform);
private:
	void init();

	Material* material;

	GLfloat* axisVerts;
	GLfloat* axisColors;
	GLuint* axisIndices;
};

