#include "NewMeshRenderer.h"

#include "ResourceManager.h"
#include "NewMesh.h"
#include "NewMaterial.h"

#include <iostream>

namespace engine {
	namespace render {
		void MeshRenderer::render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, resource::ResourceManager& resourceManager)
		{
			asset::Material* material = resourceManager.get<asset::Material>(materialId);
			asset::Mesh* mesh = resourceManager.get<asset::Mesh>(meshId);

			if (!material || !mesh) return;

			material->bind(model, view, projection, resourceManager);
			mesh->bind();
			mesh->render();
		}
	}
}
