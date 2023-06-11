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

void StandardMaterial::init(Mesh* mesh, GLuint* vao)
{
	glGenVertexArrays(1, vao);
	glBindVertexArray(*vao);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBufferId());
	GLuint posId = shaderProgram->getAttribute("VertexPosition");
	glVertexAttribPointer(posId, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(posId);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->getNormalBufferId());
	GLuint normId = shaderProgram->getAttribute("VertexNormal");
	glVertexAttribPointer(normId, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(normId);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->getUVBufferId());
	GLuint uvId = shaderProgram->getAttribute("VertexUV");
	glVertexAttribPointer(uvId, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(uvId);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBufferId());

	glBindVertexArray(0);
}

void StandardMaterial::setTexture(Texture* texture)
{
	this->texture = texture;
}
