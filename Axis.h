#pragma once
#include "GameObject.h"
#include "Material.h"

#include <glm/vec3.hpp>
class Axis :
	public GameObject
{
public:
	Axis(Material* material);

	void render(glm::mat4x4 projectionMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 parentTransform);
private:
	void init();

	Material* material;

	glm::vec3* axisVerts;
	glm::vec3* axisColors;
	GLuint* axisIndices;
};

