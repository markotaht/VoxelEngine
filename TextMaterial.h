#pragma once
#include "Material.h"
class TextMaterial :
	public Material
{
public:
	TextMaterial(ShaderProgram* shaderProgram);
	void setTextures() {};
	void setUniforms() {};

	void init(Mesh* mesh, GLuint* vao);
};

