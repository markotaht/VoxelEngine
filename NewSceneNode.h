#pragma once
#include "TransformComponent.h"
#include "NewResourceManager.h"
#include <vector>
#include <memory>

namespace engine::scene {

	class SceneNode {
	public:
		component::TransformComponent transform;

		virtual void render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, resource::ResourceManager& resourceManager) {}
		virtual void update(float deltaTime) {}
		virtual ~SceneNode() = default;

		void addChild(std::unique_ptr<SceneNode> child) {
			children.push_back(std::move(child));
		}

	protected:
		std::vector<std::unique_ptr<SceneNode>> children;
	};
}