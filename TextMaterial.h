#pragma once
#include "Types.h"
#include "Resource.h"
#include "ResourceId.h"

#include <vector>
#include <glm/glm.hpp>
#include "ResourceReflection.h"

namespace engine {
	namespace loader {
		class TextMaterialLoader;
	}
	namespace asset {
		class TextMaterial : public resource::Resource {
		public:
			TextMaterial(core::ResourceId<ShaderProgram> shaderProgramId, core::ResourceId<Font> fontId) : shaderProgramId(shaderProgramId), fontId(fontId) {}

			void bind(glm::vec3 color, glm::mat4 projection, resource::ResourceManager& resourceManager);
			void unbind(resource::ResourceManager& resourceManager);

			void unload() {}

			core::ResourceId<Font> getFontId() const { return fontId; }

			std::string toString() { return ""; }

			REFLECT_RESOURCE_IDS(shaderProgramId, fontId)
			friend class loader::TextMaterialLoader;
		private:
			core::ResourceId<ShaderProgram> shaderProgramId;
			core::ResourceId<Font> fontId;
		};
	}
}