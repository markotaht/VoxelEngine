#pragma once
#include <gl/glew.h>
#include <unordered_map>
#include <memory>

#include "IResourceLoader.h"
#include "ShaderDescriptor.h"
#include "ShaderProgram.h"

namespace engine {
	namespace loader {
		class ShaderLoader : public IResourceLoader<descriptor::ShaderDescriptor, asset::ShaderProgram>
		{
		public:
			bool canLoad(const descriptor::ShaderDescriptor& descriptor) const;
			std::unique_ptr<asset::ShaderProgram> load(const descriptor::ShaderDescriptor& descriptor) const;
			bool uploadGPU(asset::ShaderProgram& shaderProgram) const;
		private:
			std::string loadFile(const std::string& path) const;
			GLuint compileShader(const char* source, GLenum shaderType) const;

			std::unordered_map<std::string, GLint> reflectAttributes(GLuint programId) const;
			std::unordered_map<std::string, GLint> reflectUniforms(GLuint programId) const;


			void printShaderLog(GLuint shader) const;
			void printProgramLog(GLuint program) const;
		};
	}
}

