#pragma once
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#include "GeometricPrimitives.h"
#include <iostream>

#include "Resource.h"
class Mesh : public Resource
{

public:
	Mesh();
	Mesh(GLfloat* vertices, GLfloat* normals, GLfloat* uvs, GLuint* indices, int vertexCount, int normalCount, int uvcount, int indicesCount);
	~Mesh();

	bool loadFromFile(const char* path);
	bool loadFromFile(const char* path, const char* path2);

	void init();
	void render();

	static Mesh* cubePrimitive();
	static Mesh* planePrimitive();

	GLuint getVertexBufferId() { return vbo; }
	GLuint getNormalBufferId() { return nbo; }
	GLuint getUVBufferId() { return uvbo; }
	GLuint getIndexBufferId() { return ibo; }
private:

	void bind();
	void freeArrays();
	GLuint vbo;
	GLuint ibo;
	GLuint nbo;
	GLuint uvbo;

	GLfloat* vertices;
	int verticesCount;
	GLfloat* normals;
	int normalCount;
	GLuint* indices;
	int indicesCount;

	GLfloat* UVs;
	int uvCount;
};

