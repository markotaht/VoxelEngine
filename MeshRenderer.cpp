#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material): mesh(mesh), material{material}
{
	if(material == nullptr){
		throw std::invalid_argument("Material cannot be NULL");
	}
	init();
}

MeshRenderer::~MeshRenderer()
{
	delete mesh;
	glDeleteVertexArrays(1, &vao);
}

void MeshRenderer::init()
{
	ShaderProgram* shaderProgram = material->getShaderProgram();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBufferId());
	GLuint posId = shaderProgram->getAttribute("VertexPosition");
	glVertexAttribPointer(posId, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(posId);

	if (mesh->hasNormals()) {
		glBindBuffer(GL_ARRAY_BUFFER, mesh->getNormalBufferId());
		GLuint normId = shaderProgram->getAttribute("VertexNormal");
		glVertexAttribPointer(normId, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
		glEnableVertexAttribArray(normId);
	}

	if (mesh->hasColors()) {
		glBindBuffer(GL_ARRAY_BUFFER, mesh->getColorBufferId());
		GLuint colorId = shaderProgram->getAttribute("VertexColor");
		glVertexAttribPointer(colorId, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
		glEnableVertexAttribArray(colorId);
	}

	if (mesh->hasUVs()) {
		glBindBuffer(GL_ARRAY_BUFFER, mesh->getUVBufferId());
		GLuint uvId = shaderProgram->getAttribute("VertexUV");
		glVertexAttribPointer(uvId, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
		glEnableVertexAttribArray(uvId);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBufferId());

	glBindVertexArray(0);
}

void MeshRenderer::render()
{
	glBindVertexArray(vao);
	mesh->render(renderMode);
	glBindVertexArray(0);
}
