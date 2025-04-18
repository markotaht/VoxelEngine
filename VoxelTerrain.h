#pragma once
#include "ITerrain.h"
#include "NewChunk.h"
#include "ChunkGenerator.h"
#include "ChunkMesher.h"
#include "ChunkRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

namespace engine::world::voxel {
	class VoxelTerrain : public ITerrain {
	public:
		VoxelTerrain(resource::ResourceManager& resMan, core::ResourceId<asset::Material> materialId) {
			chunkRenderer = std::move(std::make_unique<ChunkRenderer>(resMan, materialId));
		}


		glm::ivec2 getChunkXZ(glm::vec3 pos) const {
			return glm::floor(glm::vec2(pos.x, pos.z) / float(Chunk::SizeX));
		}

		void update(const glm::vec3& cameraWorldPosition);


		float getHeightAt(float x, float z) const { return 0; };
		glm::vec3 getNormalAt(float x, float z) const { return { 0,0,0 }; };
		bool isSolidAt(const glm::vec3 & worldPos) const {
			return false;
		};

		// Modification
		void setMaterialAt(const glm::vec3& worldPos, int materialID) {};
		void removeAt(const glm::vec3& worldPos) {};

		// Raycast
	   // virtual bool raycast(const Ray& ray, RaycastHit& hitOut) const = 0;

		// Chunking (optional)
		void update(float deltaTime) {};
		void render(const glm::mat4& view, const glm::mat4& projection) const;
		void render() const {};
		void finalize();

	private:
		const int loadRadius = 4;
		ChunkGenerator chunkGenerator;
		ChunkMesher chunkMesher;
		std::unique_ptr<ChunkRenderer> chunkRenderer;

		std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, std::hash<glm::ivec2>> loadedChunks;
		std::vector<glm::ivec2> toBeRemoved;
	};
}