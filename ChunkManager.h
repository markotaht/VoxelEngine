#pragma once
#include "GameObject.h"
#include "Chunk.h"
#include "Block.h"
#include "ShaderProgram.h"
#include "Material.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "NoiseGenerator.h"
#include "ChunkStorage.h"
#include "ChunkSerializer.h"

#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>

#include <vector>
#include <future>
#include <chrono>
#include <algorithm>
class ChunkManager :
	public GameObject
{
public:
	ChunkManager(Material* material, Camera* camera, ResourceManager* resourceManager, NoiseGenerator* noiseGenerator);
	~ChunkManager();

	void render(glm::mat4x4 projectionMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 parentTransform);
	void update(float deltaTime);

private:

	//TODO keep track of the minimum corner to get relative coordinates inside the visible chunks
	std::vector<Chunk*> chunks;
	std::vector<Chunk*> loadedChunks;
	std::vector<Chunk*> renderChunks;
	std::vector<std::future<Chunk*>> futures;

	glm::i32vec3 minCorner;

	glm::i32vec3 cameraChunk;

	void loadChunks();
	Chunk* createChunk(glm::i32vec3 chunkNumber, float offset);
	Chunk* loadChunk(ChunkData* data, glm::i32vec3 chunkNumber, float offset);
	void handleLoadingChunks();
	void setupChunks();

	float chunkSize;
	int visibleRange = 14;
	bool loading = false;
	Material* material;
	ResourceManager* resourceManager;
	NoiseGenerator* noiseGenerator;
	ChunkStorage chunkStorage;
};

