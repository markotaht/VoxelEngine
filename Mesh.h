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
	Mesh(GLfloat* vertices, GLfloat* colors, GLuint* indices, int vertexCount, int colorCount, int indicesCount);
	Mesh(GLfloat* vertices, GLfloat* normals, GLfloat* uvs, GLuint* indices, int vertexCount, int normalCount, int uvcount, int indicesCount);
	~Mesh();

	bool loadFromFile(const char* path);
	bool loadFromFile(const char* path, const char* path2);

	void init();
	void render(GLenum renderMode);

	static Mesh* cubePrimitive();
	static Mesh* planePrimitive();

	inline bool hasNormals() { return normalCount > 0; }
	inline bool hasUVs() { return uvCount > 0; }
	inline bool hasColors() { return colorCount > 0; }

	GLuint getVertexBufferId() { return vbo; }
	GLuint getColorBufferId() { return cbo; }
	GLuint getNormalBufferId() { return nbo; }
	GLuint getUVBufferId() { return uvbo; }
	GLuint getIndexBufferId() { return ibo; }
private:

	void bind();
	void freeArrays();
	GLuint vbo;
	GLuint cbo;
	GLuint ibo;
	GLuint nbo;
	GLuint uvbo;

	GLfloat* vertices;
	int verticesCount;
	GLfloat* colors;
	int colorCount;
	GLfloat* normals;
	int normalCount;
	GLuint* indices;
	int indicesCount;

	GLfloat* UVs;
	int uvCount;
};

