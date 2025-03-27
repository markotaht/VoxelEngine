#pragma once
#include "Types.h"
#include "ResourceId.h"

namespace engine {
	namespace render {
		class MeshRenderer {
		public:
			MeshRenderer(core::ResourceId<asset::Mesh> meshId, core::ResourceId<asset::Material> materialId) : meshId(meshId), materialId(materialId){}

			void render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, resource::ResourceManager& resourceManager);
		private:
			core::ResourceId<asset::Mesh> meshId;
			core::ResourceId<asset::Material> materialId;
		};
	}
}