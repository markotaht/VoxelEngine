#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Mesh* mesh, ShaderProgram* shaderProgram) :mesh(mesh), shaderProgram(shaderProgram), material(nullptr)
{
	init();
}

MeshRenderer::MeshRenderer(Mesh* mesh, ShaderProgram* shaderProgram, Material* material) : mesh(mesh), shaderProgram(shaderProgram), material(material)
{
	init();
}

MeshRenderer::~MeshRenderer()
{
	glDeleteVertexArrays(1, &vao);
}

void MeshRenderer::init()
{

	if (material != nullptr) {
		shaderProgram = material->getShaderProgram();
	}
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

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

void MeshRenderer::render()
{
	glBindVertexArray(vao);
	mesh->render();
	glBindVertexArray(0);
}
