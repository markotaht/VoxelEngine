#include "ChunkRenderer.h"
#include "NewMaterial.h"

#include <iostream>

namespace engine::world::voxel {
	void ChunkRenderer::render(Chunk* chunk, const glm::mat4& view, const glm::mat4& projection)
	{
		asset::Material* material = resMan.get<asset::Material>(materialId);

		if (!chunk->mesh || !material) return;

		material->bind(chunk->getModelMatrix(), view, projection, resMan);
		chunk->mesh->bind();
		chunk->mesh->render();
	}
}
