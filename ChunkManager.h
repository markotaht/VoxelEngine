#pragma once
#include "GameObject.h"
#include "Chunk.h"
#include "ShaderProgram.h"
#include "Material.h"

#include <vector>
class ChunkManager :
	public GameObject
{
public:
	ChunkManager(ShaderProgram* shaderProgram, Material* material);
	~ChunkManager();

	void render(glm::mat4x4 projectionMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 parentTransform);
private:

	//TODO keep track of the minimum corner to get relative coordinates inside the visible chunks
	std::vector<Chunk*> chunks;

	void init();

	int visibleRange = 15;
	Material* material;
	ShaderProgram* shaderProgram;
};

