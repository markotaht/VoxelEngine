#pragma once
#include "ShaderProgram.h"
class GenericShaderProgram : public ShaderProgram
{
public:
	GenericShaderProgram(const GLchar* vertexFile, const GLchar* fragmentFile);
	GenericShaderProgram() :fragmentFile{ nullptr }, vertexFile{nullptr} {};
	~GenericShaderProgram();
	bool loadProgram();

	bool loadFromFile(const char* path, const char* path2);
	bool loadFromFile(const char* path);
private:
	const GLchar* vertexFile;
	const GLchar* fragmentFile;

	void getShaderAttributes();
	void getShaderUniforms();
};

