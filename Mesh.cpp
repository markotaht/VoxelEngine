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

Mesh::Mesh(GLfloat* vertices, GLfloat* colors, GLuint* indices, int vertexCount, int colorCount, int indicesCount)
{
	verticesCount = vertexCount;
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

	init();
}

Mesh::Mesh(GLfloat* vertices, GLfloat* normals, GLfloat* uvs, GLuint* indices, int vertexCount, int normalCount, int uvcount, int indicesCount)
{
	verticesCount = vertexCount;
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

	init();
}

Mesh::~Mesh()
{
	freeArrays();

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

inline void Mesh::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	if (normals != nullptr) {
		glBindBuffer(GL_ARRAY_BUFFER, nbo);
		glBufferData(GL_ARRAY_BUFFER, normalCount * sizeof(GLfloat), normals, GL_STATIC_DRAW);
	}

	if (colors != nullptr) {
		glBindBuffer(GL_ARRAY_BUFFER, cbo);
		glBufferData(GL_ARRAY_BUFFER, colorCount * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	}

	if (UVs != nullptr) {
		glBindBuffer(GL_ARRAY_BUFFER, uvbo);
		glBufferData(GL_ARRAY_BUFFER, uvCount * sizeof(GLfloat), UVs, GL_STATIC_DRAW);
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

void Mesh::init()
{
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &cbo);
	glGenBuffers(1, &ibo);
	glGenBuffers(1, &nbo);
	glGenBuffers(1, &uvbo);
	bind();
	freeArrays();
}

void Mesh::render(GLenum renderMode)
{
	glDrawElements(renderMode, indicesCount, GL_UNSIGNED_INT, 0);
}

Mesh* Mesh::cubePrimitive()
{

	return new Mesh(cubeVerts, cubeNormals, cubeUV, cubeIndices, vertCount, vertCount, UVCount, cubeIndicesCount);
}

Mesh* Mesh::planePrimitive()
{
	return new Mesh(planeVerts, planeNormals, planeUV, planeIndices, 12, 12, 8, 6);
}