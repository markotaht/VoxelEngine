#include "Chunk.h"

Chunk::Chunk(ShaderProgram* shaderProgram, Material* material) : blocks(CHUNK_SIZE* CHUNK_SIZE* CHUNK_SIZE) {
	this->shaderProgram = shaderProgram;
	this->material = material;
}

Chunk::~Chunk() {
	GameObject::~GameObject();
}

void Chunk::CreateMesh() {
	int triangleCount = 0;
	bool debug = false;
	MeshBuilder builder;
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {

				/*if (z > 0 && blocks[x][y][z - 1].IsActive() && z < CHUNK_SIZE - 1 && blocks[x][y][z + 1].IsActive() &&
					y > 0 && blocks[x][y - 1][z].IsActive() && y < CHUNK_SIZE - 1 && blocks[x][y + 1][z].IsActive() &&
					x > 0 && blocks[x - 1][y][z].IsActive() && x < CHUNK_SIZE - 1 && blocks[x + 1][y][z].IsActive()) {
					continue;
				}*/

				glm::vec3 p1(x - Block::BLOCK_SIZE, y - Block::BLOCK_SIZE, z + Block::BLOCK_SIZE);
				glm::vec3 p2(x + Block::BLOCK_SIZE, y - Block::BLOCK_SIZE, z + Block::BLOCK_SIZE);
				glm::vec3 p3(x + Block::BLOCK_SIZE, y + Block::BLOCK_SIZE, z + Block::BLOCK_SIZE);
				glm::vec3 p4(x - Block::BLOCK_SIZE, y + Block::BLOCK_SIZE, z + Block::BLOCK_SIZE);
				glm::vec3 p5(x + Block::BLOCK_SIZE, y - Block::BLOCK_SIZE, z - Block::BLOCK_SIZE);
				glm::vec3 p6(x - Block::BLOCK_SIZE, y - Block::BLOCK_SIZE, z - Block::BLOCK_SIZE);
				glm::vec3 p7(x - Block::BLOCK_SIZE, y + Block::BLOCK_SIZE, z - Block::BLOCK_SIZE);
				glm::vec3 p8(x + Block::BLOCK_SIZE, y + Block::BLOCK_SIZE, z - Block::BLOCK_SIZE);

				glm::vec3 n;
				GLuint v1;
				GLuint v2;
				GLuint v3;
				GLuint v4;

				if (debug || z == CHUNK_SIZE - 1 || !getBlock(x, y, z + 1).IsActive()) {
					n = glm::vec3(0, 0, 1);

					v1 = builder.addVertex(p1, n, glm::vec2(0, 1));
					v2 = builder.addVertex(p2, n, glm::vec2(1, 1));
					v3 = builder.addVertex(p3, n, glm::vec2(1, 0));
					v4 = builder.addVertex(p4, n, glm::vec2(0, 0));

					builder.addTriangle(v1, v2, v3);
					builder.addTriangle(v1, v3, v4);
					triangleCount += 2;
				}

				if (debug || z == 0 || !getBlock(x, y, z - 1).IsActive()) {
					n = glm::vec3(0, 0, -1);

					v1 = builder.addVertex(p5, n, glm::vec2(0, 1));
					v2 = builder.addVertex(p6, n, glm::vec2(1, 1));
					v3 = builder.addVertex(p7, n, glm::vec2(1, 0));
					v4 = builder.addVertex(p8, n, glm::vec2(0, 0));

					builder.addTriangle(v1, v2, v3);
					builder.addTriangle(v1, v3, v4);
					triangleCount += 2;
				}

				if (debug || x == CHUNK_SIZE - 1 || !getBlock(x + 1, y, z).IsActive()) {
					n = glm::vec3(1, 0, 0);

					v1 = builder.addVertex(p2, n, glm::vec2(0, 1));
					v2 = builder.addVertex(p5, n, glm::vec2(1, 1));
					v3 = builder.addVertex(p8, n, glm::vec2(1, 0));
					v4 = builder.addVertex(p3, n, glm::vec2(0, 0));

					builder.addTriangle(v1, v2, v3);
					builder.addTriangle(v1, v3, v4);
					triangleCount += 2;
				}

				if (debug || x == 0 || !getBlock(x - 1, y, z).IsActive()) {
					n = glm::vec3(-1, 0, 0);

					v1 = builder.addVertex(p6, n, glm::vec2(0, 1));
					v2 = builder.addVertex(p1, n, glm::vec2(1, 1));
					v3 = builder.addVertex(p4, n, glm::vec2(1, 0));
					v4 = builder.addVertex(p7, n, glm::vec2(0, 0));

					builder.addTriangle(v1, v2, v3);
					builder.addTriangle(v1, v3, v4);
					triangleCount += 2;
				}

				if (debug || y == CHUNK_SIZE - 1 || !getBlock(x, y + 1, z).IsActive()) {
					n = glm::vec3(0, 1, 0);

					v1 = builder.addVertex(p4, n, glm::vec2(0, 1));
					v2 = builder.addVertex(p3, n, glm::vec2(1, 1));
					v3 = builder.addVertex(p8, n, glm::vec2(1, 0));
					v4 = builder.addVertex(p7, n, glm::vec2(0, 0));

					builder.addTriangle(v1, v2, v3);
					builder.addTriangle(v1, v3, v4);
					triangleCount += 2;
				}

				if (debug || y == 0 || !getBlock(x, y - 1, z).IsActive()) {
					n = glm::vec3(0, -1, 0);

					v1 = builder.addVertex(p6, n, glm::vec2(0, 1));
					v2 = builder.addVertex(p5, n, glm::vec2(1, 1));
					v3 = builder.addVertex(p2, n, glm::vec2(1, 0));
					v4 = builder.addVertex(p1, n, glm::vec2(0, 0));

					builder.addTriangle(v1, v2, v3);
					builder.addTriangle(v1, v3, v4);
					triangleCount += 2;
				}
			}
		}
	}
	std::cout << triangleCount << std::endl;
	Mesh* mesh = builder.buildMesh();
	meshRenderer = new MeshRenderer(mesh, shaderProgram, material);
}