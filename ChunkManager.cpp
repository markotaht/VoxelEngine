#include "ChunkManager.h"

ChunkManager::ChunkManager(ShaderProgram* shaderProgram, Material* material, Camera* camera) :material(material), shaderProgram{ shaderProgram }
{
	chunkSize = Chunk::CHUNK_SIZE * Block::BLOCK_SIZE * 2;
	cameraChunk = glm::i32vec3(0, 0, 0);
	this->setMainCamera(camera);
	loadChunks();
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
	glm::vec3 cameraPosition = mainCamera->transform.getPosition();
	glm::i32vec3 newCameraChunk = cameraPosition / chunkSize;

	if (newCameraChunk != cameraChunk) {
		cameraChunk = newCameraChunk;
		loadChunks();
	}

	std::cout << loading << std::endl;
	std::cout << chunks.size() << std::endl;
	if (loading) {
		handleLoadingChunks();
	}
}

void ChunkManager::loadChunks()
{
	loadingChunks.clear();
	glm::i32vec3 minChunk(cameraChunk.x - visibleRange, 0, cameraChunk.z - visibleRange);
	float offset = Chunk::CHUNK_SIZE * 2 * Block::BLOCK_SIZE;

	for (int x = 0; x < visibleRange*2; x++) {
		for (int z = 0; z < visibleRange*2; z++) {
			loadingChunks.emplace_back(std::async(std::launch::async, [&, x, z, minChunk, offset]() {return createChunk(x, 0, z, minChunk, offset); }));
		}
	}
	loading = true;
}

Chunk* ChunkManager::createChunk(int x, int y, int z, glm::i32vec3 minChunk, float offset)
{
	Chunk* chunk = new Chunk(shaderProgram, material);
	chunk->CreateMesh();
	chunk->position(glm::vec3((x + minChunk.x) * offset, 0, (z + minChunk.z) * offset));
	return chunk;
}

void ChunkManager::handleLoadingChunks()
{
	bool allDone = true;
	for (auto& it : loadingChunks) {
		if (it.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
			allDone = false;
		}
	}

	if (allDone) {
		for (auto it : chunks) {
			delete it;
		}
		chunks.clear();

		for (auto& it : loadingChunks) {
			Chunk* chunk = it.get();
			chunk->FinalizeMesh();
			chunks.push_back(chunk);
		}
		loadingChunks.clear();
		loading = false;
	}
}
