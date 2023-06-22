#include "Chunk.h"

#include <glm/gtc/noise.hpp>
Chunk::Chunk()
{
}
Chunk::Chunk(Material* material, glm::ivec3 chunkNumber) : chunkNumber{ chunkNumber } {
	blocks.fill(BlockType::BlockType_Air);
	this->material = material;
}

Chunk::~Chunk() {
}

void Chunk::InitializeChunk(NoiseGenerator* noiseGenerator)
{
	for (int x = 0; x < CHUNK_WIDTH; x++) {
		for (int z = 0; z < CHUNK_WIDTH; z++) {
			float height = glm::perlin(glm::vec2((chunkNumber.x * CHUNK_WIDTH + x) * BLOCK_WIDTH, (chunkNumber.z * CHUNK_WIDTH + z) * BLOCK_WIDTH)) + 1;
			int blockAmount = round((height / 2.f) / BLOCK_WIDTH);
			maxHeight = std::max(maxHeight, std::min(blockAmount, CHUNK_HEIGHT));
			for (int i = 0; i < CHUNK_HEIGHT; i++) {
				if (i == blockAmount - 1) {
					blocks[x + i * CHUNK_WIDTH + z * CHUNK_SIZE_SQR] = BlockType::BlockType_Grass;
				}
				else if(i < blockAmount - 1){
					blocks[x + i * CHUNK_WIDTH + z * CHUNK_SIZE_SQR] = BlockType::BlockType_Default;
				}
			}
		}
	}
	calculateBlocksToRender();
}

void Chunk::CreateMesh(ResourceManager* resourceManager) {
	int triangleCount = 0;
	bool debug = false;
	builder = new MeshBuilder();

	BlockTextureData textureData = blockTextures[static_cast<size_t>(BlockType::BlockType_Default)];
	BlockType prevBlock = BlockType::BlockType_Default;
	for (auto &it : blocksToRender) {
		int x = it.x;
		int y = it.y;
		int z = it.z;
		float yNeg = BLOCK_WIDTH * y;
		float yPos = yNeg + BLOCK_WIDTH;
		float xNeg = BLOCK_WIDTH * x;
		float xPos = xNeg + BLOCK_WIDTH;
		float zNeg = BLOCK_WIDTH * z;
		float zPos = zNeg + BLOCK_WIDTH;
		BlockType block = getBlock(x, y, z);
				if (block != prevBlock) {
					prevBlock = block;
					textureData = blockTextures[static_cast<size_t>(block)];
				}


				glm::vec3 p1(xNeg, yNeg, zPos);
				glm::vec3 p2(xPos, yNeg, zPos);
				glm::vec3 p3(xPos, yPos, zPos);
				glm::vec3 p4(xNeg, yPos, zPos);
				glm::vec3 p5(xPos, yNeg, zNeg);
				glm::vec3 p6(xNeg, yNeg, zNeg);
				glm::vec3 p7(xNeg, yPos, zNeg);
				glm::vec3 p8(xPos, yPos, zNeg);

				glm::vec3 n;
				GLuint v1;
				GLuint v2;
				GLuint v3;
				GLuint v4;

				if (debug || z == CHUNK_WIDTH - 1 || getBlock(x, y, z + 1) == BlockType::BlockType_Air) {
					Texture* tex = resourceManager->getResource<Texture>(textureData.front);
					n = glm::vec3(0, 0, 1);

					v1 = builder->addVertex(p1, n, glm::vec3(0, 1, tex->getLayer()));
					v2 = builder->addVertex(p2, n, glm::vec3(1, 1, tex->getLayer()));
					v3 = builder->addVertex(p3, n, glm::vec3(1, 0, tex->getLayer()));
					v4 = builder->addVertex(p4, n, glm::vec3(0, 0, tex->getLayer()));

					builder->addTriangle(v1, v2, v3);
					builder->addTriangle(v1, v3, v4);
					triangleCount += 2;
				}

				if (debug || z == 0 || getBlock(x, y, z - 1) == BlockType::BlockType_Air) {
					Texture* tex = resourceManager->getResource<Texture>(textureData.back);
					n = glm::vec3(0, 0, -1);

					v1 = builder->addVertex(p5, n, glm::vec3(0, 1, tex->getLayer()));
					v2 = builder->addVertex(p6, n, glm::vec3(1, 1, tex->getLayer()));
					v3 = builder->addVertex(p7, n, glm::vec3(1, 0, tex->getLayer()));
					v4 = builder->addVertex(p8, n, glm::vec3(0, 0, tex->getLayer()));

					builder->addTriangle(v1, v2, v3);
					builder->addTriangle(v1, v3, v4);
					triangleCount += 2;
				}

				if (debug || x == CHUNK_WIDTH - 1 || getBlock(x + 1, y, z) == BlockType::BlockType_Air) {
					Texture* tex = resourceManager->getResource<Texture>(textureData.left);
					n = glm::vec3(1, 0, 0);

					v1 = builder->addVertex(p2, n, glm::vec3(0, 1, tex->getLayer()));
					v2 = builder->addVertex(p5, n, glm::vec3(1, 1, tex->getLayer()));
					v3 = builder->addVertex(p8, n, glm::vec3(1, 0, tex->getLayer()));
					v4 = builder->addVertex(p3, n, glm::vec3(0, 0, tex->getLayer()));

					builder->addTriangle(v1, v2, v3);
					builder->addTriangle(v1, v3, v4);
					triangleCount += 2;
				}

				if (debug || x == 0 || getBlock(x - 1, y, z) == BlockType::BlockType_Air) {
					Texture* tex = resourceManager->getResource<Texture>(textureData.right);
					n = glm::vec3(-1, 0, 0);

					v1 = builder->addVertex(p6, n, glm::vec3(0, 1, tex->getLayer()));
					v2 = builder->addVertex(p1, n, glm::vec3(1, 1, tex->getLayer()));
					v3 = builder->addVertex(p4, n, glm::vec3(1, 0, tex->getLayer()));
					v4 = builder->addVertex(p7, n, glm::vec3(0, 0, tex->getLayer()));

					builder->addTriangle(v1, v2, v3);
					builder->addTriangle(v1, v3, v4);
					triangleCount += 2;
				}

				if (debug || y == CHUNK_HEIGHT - 1 || getBlock(x, y + 1, z) == BlockType::BlockType_Air) {
					Texture* tex = resourceManager->getResource<Texture>(textureData.top);
					n = glm::vec3(0, 1, 0);

					v1 = builder->addVertex(p4, n, glm::vec3(0, 1, tex->getLayer()));
					v2 = builder->addVertex(p3, n, glm::vec3(1, 1, tex->getLayer()));
					v3 = builder->addVertex(p8, n, glm::vec3(1, 0, tex->getLayer()));
					v4 = builder->addVertex(p7, n, glm::vec3(0, 0, tex->getLayer()));

					builder->addTriangle(v1, v2, v3);
					builder->addTriangle(v1, v3, v4);
					triangleCount += 2;
				}

				if (debug || y == 0 || getBlock(x, y - 1, z) == BlockType::BlockType_Air) {
					Texture* tex = resourceManager->getResource<Texture>(textureData.bottom);
					n = glm::vec3(0, -1, 0);

					v1 = builder->addVertex(p6, n, glm::vec3(0, 1, tex->getLayer()));
					v2 = builder->addVertex(p5, n, glm::vec3(1, 1, tex->getLayer()));
					v3 = builder->addVertex(p2, n, glm::vec3(1, 0, tex->getLayer()));
					v4 = builder->addVertex(p1, n, glm::vec3(0, 0, tex->getLayer()));

					builder->addTriangle(v1, v2, v3);
					builder->addTriangle(v1, v3, v4);
					triangleCount += 2;
				}
			}
			blocksToRender.clear();
			blocksToRender.shrink_to_fit();
}

void Chunk::FinalizeMesh()
{
	mesh = builder->buildMesh();
	delete builder;
	builder = NULL;
	meshRenderer = new MeshRenderer(mesh, material);
}

void Chunk::calculateBlocksToRender()
{

	for (int y = 0; y < maxHeight; y++) {
		for (int x = 0; x < CHUNK_WIDTH; x++) {
			for (int z = 0; z < CHUNK_WIDTH; z++) {
				BlockType block = getBlock(x, y, z);
				if (block == BlockType::BlockType_Air) continue;
				if (z > 0 && getBlock(x, y, z - 1) != BlockType::BlockType_Air && z < CHUNK_WIDTH - 1 && getBlock(x, y, z + 1) != BlockType::BlockType_Air &&
					y > 0 && getBlock(x, y - 1, z) != BlockType::BlockType_Air && y < CHUNK_HEIGHT - 1 && getBlock(x, y + 1, z) != BlockType::BlockType_Air &&
					x > 0 && getBlock(x - 1, y, z) != BlockType::BlockType_Air && x < CHUNK_WIDTH - 1 && getBlock(x + 1, y, z) != BlockType::BlockType_Air) {
					continue;
				}
				blocksToRender.push_back(glm::ivec3(x, y, z));
			}
		}
	}
}
