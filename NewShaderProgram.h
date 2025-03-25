#pragma once
#include <gl/glew.h>
#include <vector>
#include <string>
#include <unordered_map>

#include "NewResource.h"
namespace engine {
	namespace asset {
		class ShaderProgram : public resource::Resource
		{
		public:
			ShaderProgram(GLuint programId,
				std::unordered_map<std::string, GLint> attributeMap,
				std::unordered_map<std::string, GLint> uniformMap)
				: programId(programId),
				attributeMap(std::move(attributeMap)),
				uniformMap(std::move(uniformMap)) {}
			~ShaderProgram() {
				if (programId) glDeleteProgram(programId);
			}

			bool bind() const;
			void unbind() const;
			void unload() {};

			GLint getAttribute(const GLchar* name) const;
			GLint getUniform(const GLchar* name) const;

			[[nodiscard]] GLuint getProgramId() const { return programId; }
		private:

			GLuint programId;
			const std::unordered_map<std::string, GLint> attributeMap;
			const std::unordered_map<std::string, GLint> uniformMap;
		};
	}
}