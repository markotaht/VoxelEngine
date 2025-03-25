#include "NewMeshRenderer.h"

#include "NewResourceManager.h"
#include "NewMesh.h"
#include "NewMaterial.h"

#include <iostream>

namespace engine {
	namespace render {
		void MeshRenderer::render(resource::ResourceManager& resourceManager)
		{
			asset::Material* material = resourceManager.get<asset::Material>(materialId);
			asset::Mesh* mesh = resourceManager.get<asset::Mesh>(meshId);

			if (!material || !mesh) return;

			material->bind(resourceManager);
			mesh->bind();
			mesh->render();
		}
	}
}
