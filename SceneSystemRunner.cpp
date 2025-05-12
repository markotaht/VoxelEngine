#include "SceneSystemRunner.h"
#include "SystemContext.h"

namespace engine::system {
	void SceneSystemRunner::fixedUpdateAll(double fixedDeltaTime, std::vector<scene::Scene*>& loadedScenes, core::CameraData& camData)
	{
		SystemContext globalCtx = { globalScene, &globalScene, &resourceManager, static_cast<float>(fixedDeltaTime) };
		systems.fixedUpdateAll(globalCtx);

		for (auto* localScene : loadedScenes) {
			SystemContext localCtx = { *localScene, &globalScene, &resourceManager, static_cast<float>(fixedDeltaTime) };
			systems.fixedUpdateAll(localCtx);
		}
	}
	void SceneSystemRunner::updateAll(double deltaTime, std::vector<scene::Scene*>& loadedScenes, core::CameraData& camData)
	{
		SystemContext globalCtx = { globalScene, &globalScene, &resourceManager, static_cast<float>(deltaTime) };
		systems.updateAll(globalCtx);
		for (const auto& subsystem : globalScene.getSubsystems()) {
			if (subsystem && subsystem->isActive()) {
				subsystem->update(globalCtx);
				subsystem->render(camData.transform->getInverseMatrix(), camData.camera->getProjectionMatrix());
			}
		}

		for (auto* localScene : loadedScenes) {
			SystemContext localCtx = { *localScene, &globalScene, &resourceManager, static_cast<float>(deltaTime) };
			systems.updateAll(localCtx);
			for (const auto& subsystem : localScene->getSubsystems()) {
				if (subsystem && subsystem->isActive()) {
					subsystem->update(localCtx); 
					subsystem->render(camData.transform->getInverseMatrix(), camData.camera->getProjectionMatrix());
				}
			}
		}
	}
}
