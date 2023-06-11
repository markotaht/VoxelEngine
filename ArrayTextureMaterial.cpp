#include "ArrayTextureMaterial.h"

ArrayTextureMaterial::ArrayTextureMaterial()
{
	this->texture = nullptr;
}

ArrayTextureMaterial::ArrayTextureMaterial(Texture* texture, ShaderProgram* shaderProgram) :texture{ texture }
{
	this->shaderProgram = shaderProgram;
}

void ArrayTextureMaterial::setTextures()
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture->getGLindx());
}

void ArrayTextureMaterial::setUniforms()
{
}

void ArrayTextureMaterial::init(Mesh* mesh, GLuint* vao)
{
	glGenVertexArrays(1, vao);
	glBindVertexArray(*vao);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBufferId());
	GLuint posId = shaderProgram->getAttribute("VertexPosition");
	glVertexAttribPointer(posId, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(posId);


	glBindBuffer(GL_ARRAY_BUFFER, mesh->getUVBufferId());
	GLuint uvId = shaderProgram->getAttribute("VertexUV");
	glVertexAttribPointer(uvId, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(uvId);
	

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBufferId());

	glBindVertexArray(0);
}

void ArrayTextureMaterial::setTexture(Texture* texture)
{
	this->texture = texture;
}
