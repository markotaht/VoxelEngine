#pragma once
#include "GameObject.h"
#include "Chunk.h"
#include "Block.h"
#include "ShaderProgram.h"
#include "Material.h"
#include "Camera.h"
#include "ResourceManager.h"

#include <glm/vec3.hpp>

#include <vector>
#include <future>
#include <chrono>
class ChunkManager :
	public GameObject
{
public:
	ChunkManager(Material* material, Camera* camera, ResourceManager* resourceManager);
	~ChunkManager();

	void render(glm::mat4x4 projectionMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 parentTransform);
	void update(float deltaTime);

private:

	//TODO keep track of the minimum corner to get relative coordinates inside the visible chunks
	std::vector<Chunk*> chunks;
	std::vector<std::future<Chunk*>> loadingChunks;

	glm::vec3 minCorner;

	glm::i32vec3 cameraChunk;

	void loadChunks();
	Chunk* createChunk(int x, int y, int z, glm::i32vec3 minChunk, float offset, ResourceManager* resourceManager);
	void handleLoadingChunks();

	float chunkSize;
	int visibleRange = 4;
	bool loading = false;
	Material* material;
	ResourceManager* resourceManager;
};

