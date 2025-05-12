#include "ShaderLoader.h"
#include "NewShaderProgram.h"
#include "StringUtils.h"

#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#include <vector>

#include <fstream>

#include "AutoLoaderRegistrar.h"
#include "ResourceManager.h"

namespace engine {
	namespace loader {
		static AutoLoaderRegistrar<engine::loader::ShaderLoader, IResourceLoader<descriptor::ShaderDescriptor, asset::ShaderProgram>> regTex([](resource::ResourceManager&) {
			return std::make_unique<engine::loader::ShaderLoader>();
			});

		bool ShaderLoader::canLoad(const descriptor::ShaderDescriptor& descriptor) const
		{
			//TODO make it broader or remove
			return ends_with(descriptor.vertexPath, ".vs") && ends_with(descriptor.fragmentPath, ".frag");
		}

		std::unique_ptr<asset::ShaderProgram> ShaderLoader::load(const descriptor::ShaderDescriptor& descriptor) const
		{
			std::string vertSource = loadFile(descriptor.vertexPath);
			std::string fragSource = loadFile(descriptor.fragmentPath);

			GLuint vertShader = compileShader(vertSource.c_str(), GL_VERTEX_SHADER);
			GLuint fragShader = compileShader(fragSource.c_str(), GL_FRAGMENT_SHADER);

			if (vertShader == 0 || fragShader == 0) {
				glDeleteShader(vertShader);
				glDeleteShader(fragShader);
				return nullptr;
			}

			GLuint programId = glCreateProgram();
			glAttachShader(programId, vertShader);
			glAttachShader(programId, fragShader);
			glLinkProgram(programId);

			GLint programSuccess = GL_TRUE;
			glGetProgramiv(programId, GL_LINK_STATUS, &programSuccess);
			if (programSuccess != GL_TRUE) {
				printf("Error linking program: %d vertexFile: %s  fragmentFile: %s!\n", programId, descriptor.vertexPath.c_str(), descriptor.fragmentPath.c_str());
				printProgramLog(programId);
				glDeleteShader(vertShader);
				glDeleteShader(fragShader);
				glDeleteProgram(programId);
				programId = 0;
				return nullptr;
			}

			glDeleteShader(vertShader);
			glDeleteShader(fragShader);

			std::unordered_map<std::string, GLint> attributesMap = reflectAttributes(programId);
			std::unordered_map<std::string, GLint> uniformsMap = reflectUniforms(programId);

			std::unique_ptr<asset::ShaderProgram> programPrt = std::make_unique<asset::ShaderProgram>(programId, std::move(attributesMap), std::move(uniformsMap));
			return programPrt;
		}

		std::string ShaderLoader::loadFile(const std::string& path) const
		{
			std::ifstream sourcefile(path);
			if (!sourcefile) {
				printf("Failed to open shader file: %s\n", path.c_str());
			}
			return std::string((std::istreambuf_iterator<char>(sourcefile)), std::istreambuf_iterator<char>());
		}

		GLuint ShaderLoader::compileShader(const char* source, GLenum shaderType) const
		{
			GLuint shaderID = glCreateShader(shaderType);
			glShaderSource(shaderID, 1, &source, nullptr);
			glCompileShader(shaderID);

			GLint shaderCompiled = GL_FALSE;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
			if (shaderCompiled != GL_TRUE) {
				printf("Unable to compile shader %d!\n\nSource:\n%s\n", shaderID, source);
				printShaderLog(shaderID);
				glDeleteShader(shaderID);
				shaderID = 0;
			}

			return shaderID;
		}

		std::unordered_map<std::string, GLint> ShaderLoader::reflectAttributes(GLuint programId) const
		{
			GLint attributeCount = 0;
			GLint attributeMaxLength = 0;
			glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &attributeCount);
			glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attributeMaxLength);

			std::unordered_map<std::string, GLint> attributesMap;

			if (attributeCount > 0) {
				for (int i = 0; i < attributeCount; i++) {
					std::vector<GLchar> name(attributeMaxLength);
					GLsizei length;
					GLint size;
					GLenum type;
					glGetActiveAttrib(programId, i, attributeMaxLength, &length, &size, &type, name.data());
					GLint attribute = glGetAttribLocation(programId, name.data());
					if (attribute == -1) {
						printf("%s is not a valid user-defined glsl program attribute!\n", name.data());
					}
					attributesMap[name.data()] = attribute;
				}
			}
			return attributesMap;
		}

		std::unordered_map<std::string, GLint> ShaderLoader::reflectUniforms(GLuint programId) const
		{
			GLint uniformCount = 0;
			GLint uniformMaxLength = 0;
			glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &uniformCount);
			glGetProgramiv(programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformMaxLength);

			std::unordered_map<std::string, GLint> uniformsMap;

			if (uniformCount > 0) {
				for (int i = 0; i < uniformCount; i++) {
					std::vector<GLchar> name(uniformMaxLength);
					GLsizei length;
					GLint size;
					GLenum type;
					glGetActiveUniform(programId, i, uniformMaxLength, &length, &size, &type, name.data());
					GLint uniform = glGetUniformLocation(programId, name.data());
					if (uniform == -1) {
						printf("%s is not a valid glsl program uniform!\n", name.data());
					}

					uniformsMap[name.data()] = uniform;
				}
			}
			return uniformsMap;
		}

		void ShaderLoader::printShaderLog(GLuint shader) const
		{
			if (glIsShader(shader))
			{
				//Shader log length
				int infoLogLength = 0;
				int maxLength = infoLogLength;

				//Get info string length
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<char> infoLog(maxLength);

				//Get info log
				glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog.data());
				if (infoLogLength > 0)
				{
					//Print Log
					printf("%s\n", infoLog.data());
				}
			}
			else
			{
				printf("Name %d is not a shader\n", shader);
			}
		}

		void ShaderLoader::printProgramLog(GLuint program) const
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
				std::vector<char> infoLog(maxLength);

				//Get info log
				glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog.data());
				if (infoLogLength > 0)
				{
					//Print Log
					printf("%s\n", infoLog.data());
				}
			}
			else
			{
				printf("Name %d is not a program\n", program);
			}
		}
	}
}
