#include "TextMaterial.h"

TextMaterial::TextMaterial(ShaderProgram* shaderProgram)
{
	this->shaderProgram = shaderProgram;
}

void TextMaterial::init(Mesh* mesh, GLuint* vao)
{
	glGenVertexArrays(1, vao);
	glBindVertexArray(*vao);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBufferId());
	GLuint posId = shaderProgram->getAttribute("VertexPosition");
	glVertexAttribPointer(posId, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(posId);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->getColorBufferId());
	GLuint colorId = shaderProgram->getAttribute("VertexColor");
	glVertexAttribPointer(colorId, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(colorId);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBufferId());

	glBindVertexArray(0);
}
