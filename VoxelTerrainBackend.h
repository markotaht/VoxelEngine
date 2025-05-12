#pragma once
#include "ITerrainCollision.h"
#include "NewChunk.h"
#include "ChunkGenerator.h"
#include "ChunkMesher.h"
#include "ChunkRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

#include "ITerrainBackend.h"

#include "ThreadPool.h"
#include "MainThreadDispatcher.h"

namespace engine::world::voxel {
	class VoxelTerrainBackend : public ITerrainBackend, public ITerrainCollision {
	public:
		VoxelTerrainBackend(resource::ResourceManager& resMan, core::ResourceId<asset::Material> materialId,
			core::ThreadPool& threadPool, core::MainThreadDispatcher& mainThreadDispatcher)
			:threadPool(threadPool), mainThreadDispatcher(mainThreadDispatcher) {
			chunkRenderer = std::move(std::make_unique<ChunkRenderer>(resMan, materialId));
		}

		glm::ivec2 worldToChunk(glm::vec3 pos) const override {
			glm::vec3 chunkRaw = pos / Chunk::chunkSize;
			return glm::floor(glm::vec2(chunkRaw.x, chunkRaw.z));
		}

		void update(const glm::vec3& cameraWorldPosition, float dt) override;

		bool checkCollision(const component::ColliderComponent& bounds, const component::TransformComponent& transform, std::vector<component::ColliderComponent>& collidersOut, std::vector<component::TransformComponent>& transformsOut) const override;

		void render(const glm::mat4& view, const glm::mat4& projection) const;

		void streamChunksAround(const glm::vec3 pos) override;

		ChunkStatus getChunkStatus(const glm::ivec2 chunkPos);
		bool isLoaded(const glm::ivec2 chunkPos);

	private:
		const int loadRadius = 4;
		ChunkGenerator chunkGenerator;
		ChunkMesher chunkMesher;

		core::ThreadPool& threadPool;
		core::MainThreadDispatcher& mainThreadDispatcher;
		
		std::unique_ptr<ChunkRenderer> chunkRenderer;

		std::unordered_map<glm::ivec2, ChunkStatus, std::hash<glm::ivec2>> chunkStatus;

		std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, std::hash<glm::ivec2>> loadedChunks;

		void requestChunk(const glm::ivec2 chunkPos);
		void removeFarChunks(const glm::ivec2 chunkPos);
	};
}