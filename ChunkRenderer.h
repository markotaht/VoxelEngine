#pragma once
#include <glm/glm.hpp>

#include "NewChunk.h"
#include "ResourceManager.h"
#include "Material.h"
#include "ResourceId.h"

namespace engine::world::voxel {
	class ChunkRenderer {
	public:
		ChunkRenderer(resource::ResourceManager& resMan, core::ResourceId<asset::Material> materialId): resMan(resMan), materialId(materialId){}
		void render(Chunk* chunk, const glm::mat4& view, const glm::mat4& projection);
	private:
		resource::ResourceManager& resMan;
		core::ResourceId<asset::Material> materialId;
	};
}