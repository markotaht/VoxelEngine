#pragma once
#include "MeshBuilder.h"
#include "Block.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "NoiseGenerator.h"
#include "ChunkSerializer.h"

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <array>

class Chunk: public GameObject
{
public:
	Chunk();
	Chunk(Material* material, glm::ivec3 chunkPos);
	~Chunk();

	void InitializeChunk(NoiseGenerator* noiseGenerator);
	void CreateMesh(ResourceManager* resourceManager);
	void FinalizeMesh();

	inline glm::ivec3 getChunkNumber() { return chunkNumber; }

	static const int CHUNK_WIDTH = 16;
	static const int CHUNK_HEIGHT = 64;
	static const int CHUNK_SIZE_SQR = CHUNK_WIDTH * CHUNK_HEIGHT;
private:
	friend ChunkData* chunkSerializer::serialize(Chunk* chunk);
	friend Chunk* chunkSerializer::deserialize(ChunkData* chunk, Material* material);

	std::array<BlockType, CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT> blocks;
	std::vector<glm::ivec3> blocksToRender;
	Material* material;
	MeshBuilder* builder;
	Mesh* mesh;
	glm::ivec3 chunkNumber;
	int maxHeight;

	inline BlockType getBlock(uint32_t x, uint32_t y, uint32_t z) {
		return blocks[x + y * CHUNK_WIDTH + z * CHUNK_SIZE_SQR];
	};

	inline BlockType getBlock(glm::uvec3 pos) {
		return blocks[pos.x + pos.y * CHUNK_WIDTH + pos.z * CHUNK_SIZE_SQR];
	};

	void calculateBlocksToRender();
};

