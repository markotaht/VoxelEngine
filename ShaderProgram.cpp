#include "ShaderProgram.h"

ShaderProgram::ShaderProgram()
{
	programId = 0;
}

ShaderProgram::~ShaderProgram()
{
	freeProgram();
}

bool ShaderProgram::bind()
{
	for (GLint attribute : attributes) {
		glEnableVertexAttribArray(attribute);
	}
	glUseProgram(programId);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		printf("Error binding shader! %s\n", gluErrorString(error));
		printProgramLog(programId);
		return false;
	}
	return true;
}

void ShaderProgram::unbind()
{
	for (GLint attribute : attributes) {
		glDisableVertexAttribArray(attribute);
	}
	glUseProgram(NULL);
}

GLuint ShaderProgram::loadShaderFromFile(const GLchar* path, GLenum shaderType)
{
	GLuint shaderID = 0;
	std::string shaderString;
	std::ifstream sourcefile(path);

	if (sourcefile) {
		shaderString.assign((std::istreambuf_iterator<char>(sourcefile)), std::istreambuf_iterator<char>());
		const GLchar* shaderCode = shaderString.c_str();
		shaderID = glCreateShader(shaderType);
		glShaderSource(shaderID, 1, (const GLchar**)&shaderCode, NULL);
		glCompileShader(shaderID);

		GLint shaderCompiled = GL_FALSE;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
		if (shaderCompiled != GL_TRUE) {
			printf("Unable to compile shader %d!\n\nSource:\n%s\n", shaderID, shaderCode);
			printShaderLog(shaderID);
			glDeleteShader(shaderID);
			shaderID = 0;
		}
	}
	else
	{
		printf("Unable to open file %s\n", path);
	}

	return shaderID;
}

void ShaderProgram::freeProgram()
{
	glDeleteProgram(programId);
}

GLuint ShaderProgram::getProgramId()
{
	return programId;
}

GLuint ShaderProgram::addAttribute(const GLchar* name)
{
	GLint attribute = glGetAttribLocation(programId, name);
	if (attribute == -1) {
		printf("%s is not a valid glsl program attribute!\n", name);
	}
	else {
		attributeMap[name] = attribute;
		attributes.push_back(attribute);
	}
	return attribute;
}

GLuint ShaderProgram::getAttribute(const GLchar* name)
{
	std::map<const GLchar*, GLuint>::iterator it = attributeMap.find(name);
	if (it != attributeMap.end()) {
		return it->second;
	}
	return -1;
}

GLuint ShaderProgram::addUniform(const GLchar* name)
{
	GLint uniform = glGetUniformLocation(programId, name);
	if (uniform == -1) {
		printf("%s is not a valid glsl program uniform!\n", name);
	}
	else {
		uniformMap[name] = uniform;
		uniforms.push_back(uniform);
	}
	return uniform;
}

GLuint ShaderProgram::getUniform(const GLchar* name)
{
	std::map<const GLchar*, GLuint>::iterator it = uniformMap.find(name);
	if (it != uniformMap.end()) {
		return it->second;
	}
	return -1;
}

void ShaderProgram::printShaderLog(GLuint shader)
{
	//Make sure name is shader
	if (glIsShader(shader))
	{
		//Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a shader\n", shader);
	}
}

void ShaderProgram::printProgramLog(GLuint program)
{
	//Make sure name is shader
	if (glIsProgram(program))
	{
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a program\n", program);
	}
}
