#include "ChunkManager.h"

ChunkManager::ChunkManager(Material* material, Camera* camera, ResourceManager* resourceManager, NoiseGenerator* noiseGenerator) :material{ material }, resourceManager{resourceManager},
noiseGenerator{noiseGenerator}
{
	chunks.reserve(visibleRange* visibleRange * 4);
	chunkSize = Chunk::CHUNK_WIDTH * BLOCK_WIDTH;
	cameraChunk = glm::i32vec3(0, 0, 0);
	this->setMainCamera(camera);
	loadChunks();
}

ChunkManager::~ChunkManager()
{
	for (auto it : chunks) {
		delete it;
	}
	for (auto it : loadedChunks) {
		delete it;
	}
	for (auto it : renderChunks) {
		delete it;
	}
}

void ChunkManager::render(glm::mat4x4 projectionMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 parentTransform)
{
	for (auto it : renderChunks) {
		it->render(projectionMatrix, viewMatrix, parentTransform);
	}
}

void ChunkManager::update(float deltaTime)
{
	glm::vec3 cameraPosition = mainCamera->transform.getPosition();
	cameraPosition.y = 0;
	glm::i32vec3 newCameraChunk = cameraPosition / chunkSize;

	if (newCameraChunk != cameraChunk && !loading) {
		cameraChunk = newCameraChunk;
		loadChunks();
	}

	if (loading) {
		handleLoadingChunks();
	}

	if (!loadedChunks.empty() && !loading) {
		setupChunks();
	}
}

void ChunkManager::loadChunks()
{
	minCorner = glm::i32vec3(cameraChunk.x - visibleRange, 0, cameraChunk.z - visibleRange);
	float offset = Chunk::CHUNK_WIDTH * BLOCK_WIDTH;
	for (int x = 0; x < visibleRange * 2; x++) {
		for (int z = 0; z < visibleRange * 2; z++) {
			glm::ivec3 chunkNumber = glm::ivec3(x, 0, z) + minCorner;
			if (chunkStorage.hasChunk(chunkNumber)) {
				ChunkData* data = chunkStorage.getChunk(chunkNumber);
				if (data->loaded) {
					auto is_chunk = [chunkNumber](Chunk* chunk) {return chunk->getChunkNumber() == chunkNumber; };
					loadedChunks.push_back(*std::find_if(renderChunks.begin(), renderChunks.end(), is_chunk));
				}
				else {
					futures.push_back(std::async(std::launch::async, [&, data, chunkNumber, offset] {
						return loadChunk(data, chunkNumber, offset);
						}));
				}
			}
			else {
				futures.push_back(std::async(std::launch::async, [&, chunkNumber, offset] {
					return createChunk(chunkNumber, offset);
					}));
			}
		}
	}
	loading = true;
}

Chunk* ChunkManager::createChunk( glm::i32vec3 chunkNumber, float offset)
{
	Chunk* chunk = new Chunk(material, chunkNumber);
	chunk->InitializeChunk(noiseGenerator);
	chunk->CreateMesh(resourceManager);
	chunk->position(glm::vec3(chunkNumber) * offset);
	return chunk;
}

Chunk* ChunkManager::loadChunk(ChunkData* data, glm::i32vec3 chunkNumber, float offset) {
	Chunk* chunk = chunkSerializer::deserialize(data, material);
	data->loaded = true;
	chunk->CreateMesh(resourceManager);
	chunk->position(glm::vec3(chunkNumber) * offset);
	return chunk;
}

void ChunkManager::handleLoadingChunks()
{
	bool allDone = true;
	for (auto& it : futures) {
		if (it.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
			allDone = false;
		}
	}

	if (allDone) {
		for (auto it : chunks) {
			delete it;
		}
		chunks.clear();

		for (auto& it : futures) {
			Chunk* chunk = it.get();
			chunkStorage.addChunk(chunkSerializer::serialize(chunk), chunk->getChunkNumber());
			chunk->FinalizeMesh();
			loadedChunks.push_back(chunk);
		}
		futures.clear();
		loading = false;
	}
}

void ChunkManager::setupChunks()
{
	for (auto it : renderChunks) {
		glm::i32vec3 chunkNumber = it->getChunkNumber();
		auto is_chunk = [chunkNumber](Chunk* chunk) {return chunk->getChunkNumber() == chunkNumber; };
		if (std::find_if(loadedChunks.begin(), loadedChunks.end(), is_chunk) == loadedChunks.end()) {
			delete it;
			chunkStorage.getChunk(chunkNumber)->loaded = false;
		}
	}

	renderChunks.clear();
	renderChunks.insert(renderChunks.begin(), loadedChunks.begin(), loadedChunks.end());
	loadedChunks.clear();
}
