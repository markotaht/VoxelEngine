#include "StandardMaterial.h"

StandardMaterial::StandardMaterial()
{
	this->texture = nullptr;
}

StandardMaterial::StandardMaterial(Texture* texture, ShaderProgram* shaderProgram):texture{texture}
{
	this->shaderProgram = shaderProgram;
}

void StandardMaterial::setTextures()
{
	glBindTexture(GL_TEXTURE_2D, texture->getGLindx());
}

void StandardMaterial::setUniforms()
{
}

void StandardMaterial::setTexture(Texture* texture)
{
	this->texture = texture;
}
