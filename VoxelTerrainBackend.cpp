#include "VoxelTerrainBackend.h"

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <memory>
#include <unordered_set>
#include <algorithm>

#include <iostream>
#include <glm/gtx/string_cast.hpp>

namespace engine::world::voxel {
	void VoxelTerrainBackend::update(const glm::vec3& cameraWorldPosition, float dt)
	{
		/*glm::ivec2 cameraChunk = getChunkXZ(cameraWorldPosition);
		std::unordered_set<glm::ivec2, std::hash<glm::ivec2>> desiredChunks;

		// Mark all chunks we want loaded
		for (int dx = -loadRadius; dx <= loadRadius; ++dx) {
			for (int dz = -loadRadius; dz <= loadRadius; ++dz) {
				glm::ivec2 chunkCoord = cameraChunk + glm::ivec2(dx, dz);
				desiredChunks.insert(chunkCoord);
			}
		}*/
		
	}

	bool VoxelTerrainBackend::checkCollision(const component::ColliderComponent& bounds, const component::TransformComponent& transform, std::vector<component::ColliderComponent>& collidersOut, std::vector<component::TransformComponent>& transformsOut) const
	{
		glm::vec3 rawObjMin = transform.position - bounds.size;
		glm::vec3 rawObjMax = transform.position + bounds.size;

		glm::vec3 min = glm::min(rawObjMin, rawObjMax);
		glm::vec3 max = glm::max(rawObjMin, rawObjMax);

		glm::ivec2 c1 = worldToChunk(min);
		glm::ivec2 c2 = worldToChunk(max);

		std::unordered_set<glm::ivec2, std::hash<glm::ivec2>> toBeChecked;

		for (int x = c1.x; x <= c2.x; ++x) {
			for (int z = c1.y; z <= c2.y; ++z) {
				toBeChecked.emplace(x, z);
			}
		}

		for (auto& chunkPos : toBeChecked) {
			auto it = loadedChunks.find(chunkPos);
			if (it == loadedChunks.end()) continue;

			Chunk* chunk = it->second.get();

			glm::vec3 chunkWorldMin = chunk->getWorldCoordinates();
			glm::vec3 chunkWorldMax = chunkWorldMin + glm::vec3(Chunk::SizeX, Chunk::SizeY, Chunk::SizeZ) * VOXEL_SIZE;

			glm::vec3 clampedMin = glm::clamp(min, chunkWorldMin, chunkWorldMax);
			glm::vec3 clampedMax = glm::clamp(max, chunkWorldMin, chunkWorldMax);


			glm::ivec3 rawMin = chunk->worldToVoxel(clampedMin);
			glm::ivec3 rawMax = chunk->worldToVoxel(clampedMax);

			glm::ivec3 voxelMin = glm::min(rawMin, rawMax);
			glm::ivec3 voxelMax = glm::max(rawMin, rawMax);


			for (int x = voxelMin.x; x <= voxelMax.x; ++x) {
				for (int y = voxelMin.y; y <= voxelMax.y; ++y) {
					for (int z = voxelMin.z; z <= voxelMax.z; ++z) {
						glm::ivec3 voxelPos = { x, y, z };
						if (!chunk->inBounds(voxelPos)) continue;

						Voxel voxel = chunk->getVoxel(voxelPos);
						if (voxel.type != VoxelType::VoxelType_Air) {
							glm::vec3 voxelPosition = chunk->voxelToWorld(voxelPos);
							component::TransformComponent voxelTransform;
							voxelTransform.position = voxelPosition;

							component::ColliderComponent voxelCollider;
							voxelCollider.type = component::ColliderType::AABB;
							voxelCollider.size = glm::vec3(VOXEL_SIZE * 0.5f);

							transformsOut.push_back(voxelTransform);
							collidersOut.push_back(voxelCollider);
						}
					}
				}
			}
		}
		
		return transformsOut.size() > 0;
	}

	void VoxelTerrainBackend::render(const glm::mat4& view, const glm::mat4& projection) const
	{
		for (auto it = loadedChunks.begin(); it != loadedChunks.end(); ++it) {
			chunkRenderer->render(it->second.get(), view, projection);
		}
	}

	void VoxelTerrainBackend::streamChunksAround(glm::vec3 pos)
	{
		glm::ivec2 cameraChunk = worldToChunk(pos);
		for (int dx = -loadRadius; dx <= loadRadius; ++dx) {
			for (int dz = -loadRadius; dz <= loadRadius; ++dz) {
				glm::ivec2 chunkCoord = cameraChunk + glm::ivec2(dx, dz);
				if (!isLoaded(chunkCoord)) requestChunk(chunkCoord);
			}
		}

		removeFarChunks(cameraChunk);
	}

	ChunkStatus VoxelTerrainBackend::getChunkStatus(const glm::ivec2 chunkPos)
	{
		auto it = chunkStatus.find(chunkPos);
		if (it == chunkStatus.end()) return ChunkStatus::Unloaded;
		return it->second;
	}

	bool VoxelTerrainBackend::isLoaded(const glm::ivec2 chunkPos)
	{
		return getChunkStatus(chunkPos) == ChunkStatus::Loaded;
	}

	void VoxelTerrainBackend::requestChunk(const glm::ivec2 chunkPos)
	{
		if (chunkStatus[chunkPos] != ChunkStatus::Unloaded) return;
		chunkStatus[chunkPos] = ChunkStatus::Queued;

		threadPool.enqueue([this, chunkPos]() {
			glm::ivec3 chunkPos3D = { chunkPos.x, 0, chunkPos.y };
			auto chunk = chunkGenerator.generateChunk(chunkPos3D);
			chunkMesher.generateChunkMesh(*(chunk.get()));

			mainThreadDispatcher.enqueueMainThread([this, chunkPos, chunk = std::move(chunk)]() mutable {
				chunk->mesh->upload();
				loadedChunks[chunkPos] = std::move(chunk);
				chunkStatus[chunkPos] = ChunkStatus::Loaded;
				});
		});
	}

	void VoxelTerrainBackend::removeFarChunks(const glm::ivec2 chunkPos)
	{
		for (auto it = loadedChunks.begin(); it != loadedChunks.end(); ) {
			const glm::ivec2& coord = it->first;

			int chebyshev = glm::max(abs(coord.x - chunkPos.x), abs(coord.y - chunkPos.y));
			if (chebyshev > loadRadius) {
				chunkStatus[coord] = ChunkStatus::Unloaded;
			//	unloadChunk(coord); // free memory, remove from render, etc.
				it = loadedChunks.erase(it);
			}
			else {
				++it;
			}
		}
	}
}
