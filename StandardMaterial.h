#pragma once
#include "Material.h"
#include "Texture.h"

class StandardMaterial :
	public Material
{
public:
	StandardMaterial();
	StandardMaterial(Texture* texture, ShaderProgram* shaderProgram);
	void setTextures();
	void setUniforms();

	void setTexture(Texture* texture);
private:
	Texture* texture;
};

