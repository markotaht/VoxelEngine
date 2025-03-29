#pragma once
#include "Types.h"
#include "NewResource.h"
#include "ResourceId.h"

#include <vector>
#include <glm/glm.hpp>

namespace engine::asset {
	class TextMaterial : public resource::Resource {
	public:
		TextMaterial(core::ResourceId<ShaderProgram> shaderProgramId, core::ResourceId<Font> fontId): shaderProgramId(shaderProgramId), fontId(fontId){}

		void bind(glm::vec3 color, glm::mat4 projection, resource::ResourceManager& resourceManager);
		void unbind(resource::ResourceManager& resourceManager);

		void unload(){}

		core::ResourceId<Font> getFontId() const { return fontId; }
	private:
		core::ResourceId<ShaderProgram> shaderProgramId;
		core::ResourceId<Font> fontId;
	};
}