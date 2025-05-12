#pragma once
#include "Scene.h"
#include "Registry.h"
#include "ResourceManager.h"

#include "CameraComponent.h"
#include "TransformComponent.h"

namespace engine::system {
	struct SystemContext {
		scene::Scene& scene;
		scene::Scene* globalScene;
		resource::ResourceManager* resMan;
		float dt;

		inline entity::Registry& getRegistry() {
			return scene.getRegistry();
		}

		template<typename T>
		T* getSubsystem() {
			for (auto& sys : scene.getSubsystems()) {
				if (auto* casted = dynamic_cast<T*>(sys.get())) {
					return casted;
				}
			}
			return nullptr;
		}

		entity::Entity getActiveCamera() const;
		component::CameraComponent* getCameraComponent() const;
		component::TransformComponent* getCameraTransform() const;
	};
}