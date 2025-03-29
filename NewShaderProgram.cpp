#include "NewShaderProgram.h"
#include <string>
#include <gl/glew.h>

namespace engine {
	namespace asset {
		constexpr GLint InvalidLocation = static_cast<GLint>(-1);

		bool ShaderProgram::bind() const
		{
			glUseProgram(programId);

			for (const auto& [name, location] : attributeMap) {
				glEnableVertexAttribArray(location);
			}
		
			GLenum error = glGetError();
			if (error != GL_NO_ERROR) {
				printf("Error binding shader! %s\n", glewGetErrorString(error));
				//printProgramLog(programId);
				return false;
			}
			return true;
		}

		void ShaderProgram::unbind() const
		{

			for (const auto& [name, location] : attributeMap) {
				glDisableVertexAttribArray(location);
			}

			glUseProgram(0);
		}

		GLint ShaderProgram::getAttribute(const GLchar* name) const
		{
			auto it = attributeMap.find(name);
			if (it != attributeMap.end()) {
				return it->second;
			}
			return InvalidLocation;
		}

		GLint ShaderProgram::getUniform(const GLchar* name) const
		{
			auto it = uniformMap.find(name);
			if (it != uniformMap.end()) {
				return it->second;
			}
			return InvalidLocation;
		}
	}
}
