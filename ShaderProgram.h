#pragma once
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>

#include "cmp_str.h"
#include "Resource.h"

class ShaderProgram : public Resource
{
public:

	ShaderProgram();
	virtual ~ShaderProgram();

	bool bind();
	void unbind();

	GLuint loadShaderFromFile(const GLchar* path, GLenum shaderType);
	virtual bool loadProgram() = 0;
	virtual void freeProgram();

	virtual bool loadFromFile(const char* path, const char* path2) = 0;

	GLuint getProgramId();

	GLuint addAttribute(const GLchar* name);
	GLuint getAttribute(const GLchar* name);

	GLuint addUniform(const GLchar* name);
	GLuint getUniform(const GLchar* name);

protected:
	GLuint programId = 0;
	std::vector<GLint> attributes;
	std::map<const GLchar*, GLuint, cmp_str> attributeMap;

	std::vector<GLint> uniforms;
	std::map<const GLchar*, GLuint, cmp_str> uniformMap;

	void printShaderLog(GLuint shader);
	void printProgramLog(GLuint program);
};
