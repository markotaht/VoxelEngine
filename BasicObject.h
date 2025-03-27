#pragma once
#include "NewSceneNode.h"
#include "NewMeshRenderer.h"

namespace engine::scene {
	class BasicObject : public SceneNode {
	public:
		BasicObject(std::unique_ptr<render::MeshRenderer> renderer): renderer(std::move(renderer)){
			transform.position += glm::vec3(0, 0, -5);
		}

		void render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, resource::ResourceManager& resourceManager) {
			renderer->render(transform.getMatrix(), view, projection, resourceManager);
		}
	private:
		std::unique_ptr<render::MeshRenderer> renderer;
	};
}