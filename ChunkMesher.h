#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "NewChunk.h"


namespace engine::world::voxel {
	class ChunkMesher {
	public:
		void generateChunkMesh(Chunk& chunk);
	private:
		std::vector<glm::ivec3> findVoxelsToRender(const Chunk* chunk);
	};
}