#include "Mesh.h"

Mesh::Mesh()
{
	vertices = NULL;
	indices = NULL;
	normals = NULL;
	UVs = NULL;
	indicesCount = 0;
	verticesCount = 0;
	normalCount = 0;
	uvCount = 0;
}

Mesh::Mesh(glm::vec3* vertices, glm::vec3* colors, GLuint* indices, int vertexCount, int colorCount, int indicesCount)
{
	this->indicesCount = indicesCount;
	initBuffers();
	bind(vertices, nullptr, nullptr, colors, indices, vertexCount, 0, 0, colorCount, indicesCount);
	/*verticesCount = vertexCount;
	this->vertices = new GLfloat[verticesCount];
	std::copy(vertices, vertices + verticesCount, this->vertices);

	this->colorCount = colorCount;
	this->colors = new GLfloat[colorCount];
	std::copy(colors, colors + colorCount, this->colors);

	this->indicesCount = indicesCount;
	this->indices = new GLuint[indicesCount];
	std::copy(indices, indices + indicesCount, this->indices);

	normals = nullptr;
	UVs = nullptr;

	init();*/
}

Mesh::Mesh(glm::vec3* vertices, glm::vec3* normals, glm::vec3* uvs, GLuint* indices, int vertexCount, int normalCount, int uvcount, int indicesCount)
{
	this->indicesCount = indicesCount;
	initBuffers();
	bind(vertices, normals, uvs, nullptr, indices, vertexCount, normalCount, uvcount, 0, indicesCount);
	/*verticesCount = vertexCount;
	this->vertices = new GLfloat[verticesCount];
	std::copy(vertices, vertices + verticesCount, this->vertices);

	this->normalCount = normalCount;
	this->normals = new GLfloat[normalCount];
	std::copy(normals, normals + normalCount, this->normals);

	this->indicesCount = indicesCount;
	this->indices = new GLuint[indicesCount];
	std::copy(indices, indices + indicesCount, this->indices);

	uvCount = uvcount;
	UVs = new GLfloat[uvCount];
	std::copy(uvs, uvs + uvCount, UVs);

	init();*/
}

Mesh::~Mesh()
{
	//freeArrays();

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &nbo);
	glDeleteBuffers(1, &uvbo);
}

bool Mesh::loadFromFile(const char* path)
{
	return false;
}

bool Mesh::loadFromFile(const char* path, const char* path2)
{
	return false;
}

inline void Mesh::bind(glm::vec3* vertices, glm::vec3* normals, glm::vec3* uvs, glm::vec3* colors, GLuint* indices, int vertexCount, int normalCount, int uvcount, int colorCount, int indicesCount)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);

	if (normals != nullptr) {
		glBindBuffer(GL_ARRAY_BUFFER, nbo);
		glBufferData(GL_ARRAY_BUFFER, normalCount * sizeof(glm::vec3), normals, GL_STATIC_DRAW);
	}

	if (colors != nullptr) {
		glBindBuffer(GL_ARRAY_BUFFER, cbo);
		glBufferData(GL_ARRAY_BUFFER, colorCount * sizeof(glm::vec3), colors, GL_STATIC_DRAW);
	}

	if (uvs != nullptr) {
		glBindBuffer(GL_ARRAY_BUFFER, uvbo);
		glBufferData(GL_ARRAY_BUFFER, uvcount * sizeof(glm::vec3), uvs, GL_STATIC_DRAW);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(GLuint), indices, GL_STATIC_DRAW);
}

void Mesh::freeArrays()
{
	if (vertices != NULL) {
		delete[] vertices;
		vertices = NULL;
	}
	if (colors != NULL) {
		delete[] colors;
		colors = NULL;
	}
	if (indices != NULL) {
		delete[] indices;
		indices = NULL;
	}
	if (normals != NULL) {
		delete[] normals;
		normals = NULL;
	}
	if (UVs != NULL) {
		delete[] UVs;
		UVs = NULL;
	}
}

void Mesh::initBuffers()
{
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &cbo);
	glGenBuffers(1, &ibo);
	glGenBuffers(1, &nbo);
	glGenBuffers(1, &uvbo);
	//bind();
	//freeArrays();
}

void Mesh::render(GLenum renderMode)
{
	glDrawElements(renderMode, indicesCount, GL_UNSIGNED_INT, 0);
}
/*
Mesh* Mesh::cubePrimitive()
{

	return new Mesh(cubeVerts, cubeNormals, cubeUV, cubeIndices, vertCount, vertCount, UVCount, cubeIndicesCount);
}

Mesh* Mesh::planePrimitive()
{
	return new Mesh(planeVerts, planeNormals, planeUV, planeIndices, 12, 12, 8, 6);
}*/