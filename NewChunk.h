#pragma once
#include <cstdint>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "NewMesh.h"

namespace engine::world::voxel {
	enum class VoxelType : uint8_t {
		VoxelType_Air = 0,
		VoxelType_Default = 1,
		VoxelType_Grass = 2,
		VoxelTypeCount
	};

	static const float VOXEL_SIZE = 0.25f;
	static const float VOXEL_WIDTH = 2 * VOXEL_SIZE;

	struct Voxel {
		VoxelType type = VoxelType::VoxelType_Air;
	};

	struct Chunk {
		static constexpr int SizeX = 16, SizeY = 64, SizeZ = 16;
		Voxel voxels[SizeX * SizeY * SizeZ];
			
		bool dirty = true;
		std::unique_ptr<asset::Mesh> mesh = nullptr;
		glm::ivec3 chunkPosition = { 0,0,0 };
		int maxHeight = 0;

		Chunk(const glm::ivec3& pos): chunkPosition(pos){}

		inline glm::vec3 getWorldCoordinates() const {
			return glm::vec3(chunkPosition) * glm::vec3(Chunk::SizeX, Chunk::SizeY, Chunk::SizeZ);
		}

		inline glm::mat4 getModelMatrix() const {
			return glm::translate(glm::mat4(1.0f), getWorldCoordinates());
		}

		inline Voxel getVoxel(glm::ivec3 pos) const {
			return voxels[pos.x + pos.y * SizeX + pos.z * SizeY * SizeZ];
		};

		inline Voxel getVoxel(int x, int y, int z) const {
			return voxels[x + y * SizeX + z * SizeY * SizeZ];
		};

		inline bool inBounds(glm::ivec3 pos) const {
			return pos.x >= 0 && pos.x < SizeX &&
				pos.y >= 0 && pos.y < SizeY &&
				pos.z >= 0 && pos.z < SizeZ;
		}
	};
}