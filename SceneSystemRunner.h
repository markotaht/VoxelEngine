#pragma once
#include "SystemRegistry.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "CameraData.h"

#include <vector>

namespace engine::system {
	class SceneSystemRunner {
	public:
		SceneSystemRunner(SystemRegistry& registry, resource::ResourceManager& resourceManager, scene::Scene& globalScene)
			:systems(registry), resourceManager(resourceManager), globalScene(globalScene){}

		void fixedUpdateAll(double fixedDeltaTime, std::vector<scene::Scene*>& loadedScenes, core::CameraData& camData);
		void updateAll(double deltaTime, std::vector<scene::Scene*>& loadedScenes, core::CameraData& camData);

	private:
		SystemRegistry& systems;
		resource::ResourceManager& resourceManager;
		scene::Scene& globalScene;
	};
}