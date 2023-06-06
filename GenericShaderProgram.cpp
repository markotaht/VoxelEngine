#include "GenericShaderProgram.h"

GenericShaderProgram::GenericShaderProgram(const GLchar* vertexFile, const GLchar* fragmentFile)
{
	this->fragmentFile = fragmentFile;
	this->vertexFile = vertexFile;

	loadProgram();
}

GenericShaderProgram::~GenericShaderProgram()
{
}

bool GenericShaderProgram::loadProgram()
{
	programId = glCreateProgram();
	GLuint vertexShader = loadShaderFromFile(vertexFile, GL_VERTEX_SHADER);
	if (vertexShader == 0) {
		glDeleteProgram(programId);
		return false;
	}
	glAttachShader(programId, vertexShader);

	GLuint fragmentShader = loadShaderFromFile(fragmentFile, GL_FRAGMENT_SHADER);
	if (fragmentShader == 0) {
		glDeleteShader(vertexShader);
		glDeleteProgram(programId);
		return false;
	}
	glAttachShader(programId, fragmentShader);

	glLinkProgram(programId);

	GLint programSuccess = GL_TRUE;
	glGetProgramiv(programId, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE) {
		printf("Error linking program %d!\n", programId);
		printProgramLog(programId);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(programId);
		programId = 0;
		return false;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	getShaderAttributes();
	getShaderUniforms();
	return true;
}

bool GenericShaderProgram::loadFromFile(const char* path, const char* path2)
{
	this->fragmentFile = path2;
	this->vertexFile = path;

	return loadProgram();
}

bool GenericShaderProgram::loadFromFile(const char* path)
{
	return false;
}

void GenericShaderProgram::getShaderAttributes()
{
	GLint attributeCount = 0;
	GLint attributeMaxLength = 0;
	glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &attributeCount);
	glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attributeMaxLength);

	if (attributeCount > 0) {
		for (int i = 0; i < attributeCount; i++) {
			GLchar* name = new GLchar[attributeMaxLength];
			GLsizei length;
			GLint size;
			GLenum type;
			glGetActiveAttrib(programId, i, attributeMaxLength, &length, &size, &type, name);
			addAttribute(name);
		}
	}
}

void GenericShaderProgram::getShaderUniforms()
{
	GLint uniformCount = 0;
	GLint uniformMaxLength = 0;
	glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &uniformCount);
	glGetProgramiv(programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformMaxLength);

	if (uniformCount > 0) {
		for (int i = 0; i < uniformCount; i++) {
			GLchar* name = new GLchar[uniformMaxLength];
			GLsizei length;
			GLint size;
			GLenum type;
			glGetActiveUniform(programId, i, uniformMaxLength, &length, &size, &type, name);

			addUniform(name);
		}
	}
}
