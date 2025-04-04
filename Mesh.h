#pragma once
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#include "GeometricPrimitives.h"
#include <iostream>

#include <glm/vec3.hpp>
#include "Resource.h"
class Mesh : public Resource
{

public:
	Mesh();
	Mesh(glm::vec3* vertices, glm::vec3* colors, GLuint* indices, int vertexCount, int colorCount, int indicesCount);
	Mesh(glm::vec3* vertices, glm::vec3* normals, glm::vec3* uvs, GLuint* indices, int vertexCount, int normalCount, int uvcount, int indicesCount);
	~Mesh();

	bool loadFromFile(const char* path);
	bool loadFromFile(const char* path, const char* path2);

	void initBuffers();
	void render(GLenum renderMode);

	//static Mesh* cubePrimitive();
	//static Mesh* planePrimitive();

	inline bool hasNormals() { return normalCount > 0; }
	inline bool hasUVs() { return uvCount > 0; }
	inline bool hasColors() { return colorCount > 0; }

	GLuint getVertexBufferId() { return vbo; }
	GLuint getColorBufferId() { return cbo; }
	GLuint getNormalBufferId() { return nbo; }
	GLuint getUVBufferId() { return uvbo; }
	GLuint getIndexBufferId() { return ibo; }
private:

	void bind(glm::vec3* vertices, glm::vec3* normals, glm::vec3* uvs, glm::vec3* colors, GLuint* indices, int vertexCount, int normalCount, int uvcount, int colorCount, int indicesCount);
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

