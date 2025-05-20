#pragma once
#include <cstdint>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"

namespace engine::world::voxel {
	enum class VoxelType : uint8_t {
		VoxelType_Air = 0,
		VoxelType_Default = 1,
		VoxelType_Grass = 2,
		VoxelTypeCount
	};

	static const float VOXEL_SIZE = 2.0f;

	struct Voxel {
		VoxelType type = VoxelType::VoxelType_Air;
	};

	struct Chunk {
		static constexpr int SizeX = 16, SizeY = 64, SizeZ = 16;
		static inline const glm::vec3 chunkSize { Chunk::SizeX* VOXEL_SIZE,
			Chunk::SizeY* VOXEL_SIZE,
			Chunk::SizeZ* VOXEL_SIZE };

		Voxel voxels[SizeX * SizeY * SizeZ];
			
		bool dirty = true;
		std::unique_ptr<asset::Mesh> mesh = nullptr;
		glm::ivec3 chunkPosition = { 0,0,0 };
		int maxHeight = 0;

		Chunk(const glm::ivec3& pos): chunkPosition(pos){}

		inline glm::vec3 getWorldCoordinates() const {
			return glm::vec3(chunkPosition) * chunkSize;
		}

		inline glm::mat4 getModelMatrix() const {
			return glm::translate(glm::mat4(1.0f), getWorldCoordinates());
		}

		inline Voxel getVoxel(glm::ivec3 pos) const {
			if (pos.x < 0 || pos.y < 0 || pos.z < 0 ||
				pos.x >= SizeX || pos.y >= SizeY || pos.z >= SizeZ) {
				return Voxel{}; // or throw, or return default air voxel
			}
			return voxels[pos.x + pos.y * SizeX + pos.z * SizeY * SizeZ];
		};

		inline Voxel getVoxel(glm::vec3 pos) const {
			return getVoxel(worldToVoxel(pos));
		};

		inline Voxel getVoxel(int x, int y, int z) const {
			return voxels[x + y * SizeX + z * SizeY * SizeZ];
		};

		inline glm::vec3 voxelToWorld(glm::ivec3 voxelPos) const{
			return getWorldCoordinates() + (glm::vec3(voxelPos) + 0.5f) * VOXEL_SIZE;
		}

		inline glm::ivec3 worldToVoxel(glm::vec3 pos) const{
			return glm::floor((pos - getWorldCoordinates()) / VOXEL_SIZE);
		}

		inline glm::vec3 worldToVoxelWorld(glm::vec3 pos) const {
			return voxelToWorld(worldToVoxel(pos)) + glm::vec3(VOXEL_SIZE * 0.5f);
		}

		inline bool inBounds(glm::ivec3 pos) const {
			return pos.x >= 0 && pos.x < SizeX &&
				pos.y >= 0 && pos.y < SizeY &&
				pos.z >= 0 && pos.z < SizeZ;
		}
	};
}