#pragma once
#include <memory>
#include <glm/glm.hpp>

#include "NewChunk.h"

namespace engine::world::voxel {
	class ChunkGenerator {
	public:
		std::unique_ptr<Chunk> generateChunk(glm::ivec3 chunkPosition);
	};
}