#include "BasicShaderProgram2D.h"
bool BasicShaderProgram2D::loadProgram()
{
	programId = glCreateProgram();
	GLuint vertexShader = loadShaderFromFile("basic.vs", GL_VERTEX_SHADER);
	if (vertexShader == 0) {
		glDeleteProgram(programId);
		return false;
	}
	glAttachShader(programId, vertexShader);

	GLuint fragmentShader = loadShaderFromFile("basic.fs", GL_FRAGMENT_SHADER);
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

	return true;
}