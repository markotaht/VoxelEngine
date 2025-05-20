#pragma once
#include <gl/glew.h>
#include <vector>
#include <string>
#include <unordered_map>

#include "Resource.h"
namespace engine {
	namespace loader {
		class ShaderLoader;
	}

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

			ShaderProgram(std::string vs, std::string fs,
				std::string vPath = {}, std::string fPath = {})
				: programId(0),
				vertexSource(std::move(vs)), fragmentSource(std::move(fs)),
				vertexPath(std::move(vPath)), fragmentPath(std::move(fPath)) {}


			~ShaderProgram() {
				if (programId) glDeleteProgram(programId);
			}

			bool bind() const;
			void unbind() const;
			void unload() {};

			GLint getAttribute(const GLchar* name) const;
			GLint getUniform(const GLchar* name) const;

			[[nodiscard]] GLuint getProgramId() const { return programId; }

			std::string toString() { return ""; }

			friend class loader::ShaderLoader;
		private:
			std::string vertexSource;
			std::string fragmentSource;

			std::string vertexPath;
			std::string fragmentPath;

			GLuint programId;
			std::unordered_map<std::string, GLint> attributeMap;
			std::unordered_map<std::string, GLint> uniformMap;
		};
	}
}