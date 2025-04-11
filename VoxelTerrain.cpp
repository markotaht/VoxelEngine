#include "VoxelTerrain.h"

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <memory>
#include <unordered_set>

#include <iostream>

namespace engine::world::voxel {
	void VoxelTerrain::update(const glm::vec3& cameraWorldPosition)
	{
		glm::ivec2 cameraChunk = getChunkXZ(cameraWorldPosition);
		std::unordered_set<glm::ivec2, std::hash<glm::ivec2>> desiredChunks;

		// Mark all chunks we want loaded
		for (int dx = -loadRadius; dx <= loadRadius; ++dx) {
			for (int dz = -loadRadius; dz <= loadRadius; ++dz) {
				glm::ivec2 chunkCoord = cameraChunk + glm::ivec2(dx, dz);
				desiredChunks.insert(chunkCoord);
			}
		}
		
		for (auto it = loadedChunks.begin(); it != loadedChunks.end(); ++it) {
			if (desiredChunks.find(it->first) == desiredChunks.end()) {
				toBeRemoved.push_back(it->first);
			}
		}

		for (const auto& coord : desiredChunks) {
			if (loadedChunks.find(coord) == loadedChunks.end()) {
				glm::ivec3 chunkPos = { coord.x, 0, coord.y }; // or actual Y slice
				auto chunk = chunkGenerator.generateChunk(chunkPos);
				chunkMesher.generateChunkMesh(*(chunk.get()));
				loadedChunks[coord] = std::move(chunk);
			}
		}
		
	}
	void VoxelTerrain::render(const glm::mat4& view, const glm::mat4& projection) const
	{
		for (auto it = loadedChunks.begin(); it != loadedChunks.end(); ++it) {
			chunkRenderer->render(it->second.get(), view, projection);
		}
	}
	void VoxelTerrain::finalize()
	{
		for (auto it = toBeRemoved.begin(); it != toBeRemoved.end(); ++it) {
			loadedChunks.erase(*it);
		}
		toBeRemoved.clear();
	}
}
