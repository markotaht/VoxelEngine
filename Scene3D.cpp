#include "Scene3D.h"

void engine::scene::Scene3D::render(resource::ResourceManager& resourceManager)
{
	for (auto& child : children) {
		child->render(transform.getMatrix(), camera->getViewMatrix(), camera->getProjectionMatrix(), resourceManager);
	}
}
