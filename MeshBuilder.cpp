#include "MeshBuilder.h"

MeshBuilder::MeshBuilder()
{
}

MeshBuilder::MeshBuilder(int vertexCount, int indexCount)
{
	vertices.reserve(vertexCount * 3);
	normals.reserve(vertexCount * 3);
	uvs.reserve(vertexCount * 2);

	indices.reserve(indexCount);
}

MeshBuilder::~MeshBuilder()
{
	vertices.clear();
	vertices.shrink_to_fit();
	normals.clear();
	normals.shrink_to_fit();
	uvs.clear();
	uvs.shrink_to_fit();
	indices.clear();
	indices.shrink_to_fit();
}
/*
GLuint MeshBuilder::addVertex(glm::vec3 vertex, glm::vec3 normal, glm::vec2 uv)
{
	vertices.push_back(vertex);
	normals.push_back(normal);
	uvs.push_back(uv);
	return vertexIndex++;
}*/

GLuint MeshBuilder::addVertex(glm::vec3 vertex, glm::vec3 normal, glm::vec3 uv)
{
	vertices.push_back(vertex);
	normals.push_back(normal);
	uvs.push_back(uv);
	return vertexIndex++;
}

void MeshBuilder::addTriangle(GLuint v1, GLuint v2, GLuint v3)
{
	indices.push_back(v1);
	indices.push_back(v2);
	indices.push_back(v3);
}

Mesh* MeshBuilder::buildMesh()
{
	return new Mesh(vertices.data(), normals.data(), uvs.data(), indices.data(), vertices.size(), normals.size(), uvs.size(), indices.size());
}
