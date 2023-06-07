#include "ChunkManager.h"

ChunkManager::ChunkManager(ShaderProgram* shaderProgram, Material* material) :material(material), shaderProgram{ shaderProgram }
{
	init();
}

ChunkManager::~ChunkManager()
{
	for (auto it : chunks) {
		delete it;
	}
}

void ChunkManager::render(glm::mat4x4 projectionMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 parentTransform)
{
	for (auto it : chunks) {
		it->render(projectionMatrix, viewMatrix, parentTransform);
	}
}

void ChunkManager::update(float deltaTime)
{
}

void ChunkManager::init()
{
	float offset = Chunk::CHUNK_SIZE * 2 * Block::BLOCK_SIZE;

	for (int x = 0; x < visibleRange; x++) {
		for (int z = 0; z < visibleRange; z++) {
			Chunk* chunk = new Chunk(shaderProgram, material);
			chunk->CreateMesh();
			chunk->position(glm::vec3(x * offset, 0, z * offset));
			chunks.push_back(chunk);
		}
	}
}
