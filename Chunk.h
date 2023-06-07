#pragma once
#include "MeshBuilder.h"
#include "Block.h"
#include "GameObject.h"
#include "ShaderProgram.h"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>

class Chunk: public GameObject
{
public:
	Chunk(ShaderProgram* shaderProgram, Material* material);
	~Chunk();

	void CreateMesh();

	static const int CHUNK_SIZE = 16;
	static const int CHUNK_SIZE_SQR = CHUNK_SIZE * CHUNK_SIZE;
private:
	std::vector<Block> blocks;
	ShaderProgram* shaderProgram;
	Material* material;

	inline Block getBlock(uint32_t x, uint32_t y, uint32_t z) {
		return blocks[x + y * CHUNK_SIZE + z * CHUNK_SIZE_SQR];
	};
};

