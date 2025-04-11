#include "ChunkGenerator.h"
#include <memory>
#include <glm/gtc/noise.hpp>

#include "NewChunk.h"

#include <iostream>
template <typename T>
T clamp(T value, T min, T max) {
    return std::max(min, std::min(value, max));
}

namespace engine::world::voxel {
    constexpr float noiseScale = 0.01f;
    constexpr int maxTerrainHeight = Chunk::SizeY; // usually 64
    constexpr float heightScale = static_cast<float>(maxTerrainHeight);
    std::unique_ptr<Chunk> ChunkGenerator::generateChunk(glm::ivec3 chunkPosition)
    {
        std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>(chunkPosition);
        int maxHeight = 0;
        int layerOffset = Chunk::SizeX * Chunk::SizeY;
        for (int x = 0; x < Chunk::SizeX; x++) {
            for (int z = 0; z < Chunk::SizeZ; z++) {
                glm::vec2 perlinInput = glm::vec2(
                    (chunkPosition.x * Chunk::SizeX + x) * noiseScale,
                    (chunkPosition.z * Chunk::SizeZ + z) * noiseScale
                );
                float height = (glm::perlin(perlinInput) + 1.0f) / 2.0f;
                int blockAmount = static_cast<int>(height * heightScale);
                blockAmount = clamp(blockAmount, 0, Chunk::SizeY);
                maxHeight = std::max(blockAmount, maxHeight);
                for (int y = 0; y < blockAmount-1; y++) {
                    chunk->voxels[x + y * Chunk::SizeX + z * layerOffset] = { VoxelType::VoxelType_Default };
                }
                if (blockAmount > 0) {
                    int y = blockAmount - 1;
                    chunk->voxels[x + y * Chunk::SizeX + z * layerOffset] = { VoxelType::VoxelType_Grass };
                }
            }
        }
        chunk->maxHeight = clamp(maxHeight, 0, Chunk::SizeY);
        return chunk;
    }
}
