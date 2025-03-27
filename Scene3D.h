#pragma once
#include <memory>

#include "SceneNode.h"
#include "NewCamera.h"
#include "NewResourceManager.h"

namespace engine::scene {
	class Scene3D : public SceneNode {
	public:
		void render(resource::ResourceManager& resourceManager);
		void addCamera(std::unique_ptr<Camera> camera) {
			this->camera = std::move(camera);
		}

	private:
		std::unique_ptr<Camera> camera;
	};
}