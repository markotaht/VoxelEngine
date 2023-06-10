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
}

GLuint MeshBuilder::addVertex(glm::vec3 vertex, glm::vec3 normal, glm::vec2 uv)
{
	vertices.push_back(vertex[0]);
	vertices.push_back(vertex[1]);
	vertices.push_back(vertex[2] );
	normals.push_back(normal[0]);
	normals.push_back(normal[1]);
	normals.push_back(normal[2]);
	uvs.push_back(uv[0]);
	uvs.push_back(uv[1]);
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
