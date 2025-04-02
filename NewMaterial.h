#pragma once
#include <unordered_map>
#include <string>

#include "Types.h"
#include "NewResource.h"
#include "NewShaderProgram.h"
#include "ResourceId.h"

namespace engine {
	namespace asset {
		using TexturesMap = std::unordered_map<std::string, core::ResourceId<asset::ITexture>>;

		class Material : public resource::Resource {
		public:
			Material(core::ResourceId<ShaderProgram> shaderProgramId, UniformMap uniforms, TexturesMap textures)
				:shaderProgramId(shaderProgramId)
				,uniforms(std::move(uniforms))
				,textures(std::move(textures)){}

			void bind(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, resource::ResourceManager& resourceManager);

			void unload(){}

		private:
			core::ResourceId<ShaderProgram> shaderProgramId;
			UniformMap uniforms;
			TexturesMap textures;

			void bindUniforms(ShaderProgram* shaderProgram);
			void bindTextures(ShaderProgram* shaderProgram, resource::ResourceManager& resourceManager);
		};
	}
}