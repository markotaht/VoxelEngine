#include "Chunk.h"

Chunk::Chunk(Material* material) : blocks(CHUNK_SIZE* CHUNK_SIZE* CHUNK_SIZE) {
	this->material = material;
}

Chunk::~Chunk() {
}

void Chunk::InitializeChunk()
{
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int z = 0; z < CHUNK_SIZE; z++) {
			blocks[x + CHUNK_SIZE_SQR - CHUNK_SIZE + z * CHUNK_SIZE_SQR] = Block(BlockType::BlockType_Grass);
		}
	}
}

void Chunk::CreateMesh(ResourceManager* resourceManager) {
	int triangleCount = 0;
	bool debug = false;
	builder = MeshBuilder(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 4 * 6 * 3, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 12);

	for (int x = 0; x < CHUNK_SIZE; x++) {
		float xNeg = (2 * x - 1) * Block::BLOCK_SIZE + Block::BLOCK_SIZE;
		float xPos = (2 * x + 1) * Block::BLOCK_SIZE + Block::BLOCK_SIZE;
		for (int y = 0; y < CHUNK_SIZE; y++) {
			float yNeg = (2 * y - 1) * Block::BLOCK_SIZE + Block::BLOCK_SIZE;
			float yPos = (2 * y + 1) * Block::BLOCK_SIZE + Block::BLOCK_SIZE;
			for (int z = 0; z < CHUNK_SIZE; z++) {
				float zNeg = (2 * z - 1) * Block::BLOCK_SIZE + Block::BLOCK_SIZE;
				float zPos = (2 * z + 1) * Block::BLOCK_SIZE + Block::BLOCK_SIZE;

				Block block = blocks[x + y * CHUNK_SIZE + z * CHUNK_SIZE_SQR];
				BlockTextureData textureData = *BlockTextures[block.getBlockType()];
				/*if (z > 0 && blocks[x][y][z - 1].IsActive() && z < CHUNK_SIZE - 1 && blocks[x][y][z + 1].IsActive() &&
					y > 0 && blocks[x][y - 1][z].IsActive() && y < CHUNK_SIZE - 1 && blocks[x][y + 1][z].IsActive() &&
					x > 0 && blocks[x - 1][y][z].IsActive() && x < CHUNK_SIZE - 1 && blocks[x + 1][y][z].IsActive()) {
					continue;
				}*/


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

				if (debug || z == CHUNK_SIZE - 1 || !getBlock(x, y, z + 1).IsActive()) {
					Texture* tex = resourceManager->getResource<Texture>(textureData.front);
					n = glm::vec3(0, 0, 1);

					v1 = builder.addVertex(p1, n, glm::vec2(0, 1), tex->getLayer());
					v2 = builder.addVertex(p2, n, glm::vec2(1, 1), tex->getLayer());
					v3 = builder.addVertex(p3, n, glm::vec2(1, 0), tex->getLayer());
					v4 = builder.addVertex(p4, n, glm::vec2(0, 0), tex->getLayer());

					builder.addTriangle(v1, v2, v3);
					builder.addTriangle(v1, v3, v4);
					triangleCount += 2;
				}

				if (debug || z == 0 || !getBlock(x, y, z - 1).IsActive()) {
					Texture* tex = resourceManager->getResource<Texture>(textureData.back);
					n = glm::vec3(0, 0, -1);

					v1 = builder.addVertex(p5, n, glm::vec2(0, 1), tex->getLayer());
					v2 = builder.addVertex(p6, n, glm::vec2(1, 1), tex->getLayer());
					v3 = builder.addVertex(p7, n, glm::vec2(1, 0), tex->getLayer());
					v4 = builder.addVertex(p8, n, glm::vec2(0, 0), tex->getLayer());

					builder.addTriangle(v1, v2, v3);
					builder.addTriangle(v1, v3, v4);
					triangleCount += 2;
				}

				if (debug || x == CHUNK_SIZE - 1 || !getBlock(x + 1, y, z).IsActive()) {
					Texture* tex = resourceManager->getResource<Texture>(textureData.left);
					n = glm::vec3(1, 0, 0);

					v1 = builder.addVertex(p2, n, glm::vec2(0, 1), tex->getLayer());
					v2 = builder.addVertex(p5, n, glm::vec2(1, 1), tex->getLayer());
					v3 = builder.addVertex(p8, n, glm::vec2(1, 0), tex->getLayer());
					v4 = builder.addVertex(p3, n, glm::vec2(0, 0), tex->getLayer());

					builder.addTriangle(v1, v2, v3);
					builder.addTriangle(v1, v3, v4);
					triangleCount += 2;
				}

				if (debug || x == 0 || !getBlock(x - 1, y, z).IsActive()) {
					Texture* tex = resourceManager->getResource<Texture>(textureData.right);
					n = glm::vec3(-1, 0, 0);

					v1 = builder.addVertex(p6, n, glm::vec2(0, 1), tex->getLayer());
					v2 = builder.addVertex(p1, n, glm::vec2(1, 1), tex->getLayer());
					v3 = builder.addVertex(p4, n, glm::vec2(1, 0), tex->getLayer());
					v4 = builder.addVertex(p7, n, glm::vec2(0, 0), tex->getLayer());

					builder.addTriangle(v1, v2, v3);
					builder.addTriangle(v1, v3, v4);
					triangleCount += 2;
				}

				if (debug || y == CHUNK_SIZE - 1 || !getBlock(x, y + 1, z).IsActive()) {
					Texture* tex = resourceManager->getResource<Texture>(textureData.top);
					n = glm::vec3(0, 1, 0);

					v1 = builder.addVertex(p4, n, glm::vec2(0, 1), tex->getLayer());
					v2 = builder.addVertex(p3, n, glm::vec2(1, 1), tex->getLayer());
					v3 = builder.addVertex(p8, n, glm::vec2(1, 0), tex->getLayer());
					v4 = builder.addVertex(p7, n, glm::vec2(0, 0), tex->getLayer());

					builder.addTriangle(v1, v2, v3);
					builder.addTriangle(v1, v3, v4);
					triangleCount += 2;
				}

				if (debug || y == 0 || !getBlock(x, y - 1, z).IsActive()) {
					Texture* tex = resourceManager->getResource<Texture>(textureData.bottom);
					n = glm::vec3(0, -1, 0);

					v1 = builder.addVertex(p6, n, glm::vec2(0, 1), tex->getLayer());
					v2 = builder.addVertex(p5, n, glm::vec2(1, 1), tex->getLayer());
					v3 = builder.addVertex(p2, n, glm::vec2(1, 0), tex->getLayer());
					v4 = builder.addVertex(p1, n, glm::vec2(0, 0), tex->getLayer());

					builder.addTriangle(v1, v2, v3);
					builder.addTriangle(v1, v3, v4);
					triangleCount += 2;
				}
			}
		}
	}
}

void Chunk::FinalizeMesh()
{
	Mesh* mesh = builder.buildMesh();
	meshRenderer = new MeshRenderer(mesh, material);
}
