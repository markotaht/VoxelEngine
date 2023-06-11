#pragma once
#include "Material.h"
#include "Texture.h"

class ArrayTextureMaterial:	public Material
{
public:
	ArrayTextureMaterial();
	ArrayTextureMaterial(Texture* texture, ShaderProgram* shaderProgram);
	void setTextures();
	void setUniforms();

	void init(Mesh* mesh, GLuint* vao);

	void setTexture(Texture* texture);
private:
	Texture* texture;
};

