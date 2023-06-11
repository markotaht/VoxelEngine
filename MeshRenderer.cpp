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
	material->init(mesh, &vao);
}

void MeshRenderer::render()
{
	glBindVertexArray(vao);
	mesh->render(renderMode);
	glBindVertexArray(0);
}
