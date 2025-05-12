#pragma once
#include "Registry.h"
#include "ISceneSubsystem.h"

#include <vector>
#include <memory>

namespace engine::scene {
	class Scene {
	public:

		entity::Entity createEntity() {
			return entityRegistry.create();
		}

		template<typename... Components>
		void addEntity(entity::Entity entity, Components&&... components) {
			entityRegistry.addEntity<Components...>(entity, std::forward<Components>(components)...);
		}

		template<typename... Components, typename Func>
		void forEach(Func func) {
			entityRegistry.forEach<Components...>(std::forward<Func>(func));
		}

		template<typename T>
		T& getComponent(entity::Entity e) {
			return entityRegistry.template getComponent<T>(e);
		}

		template<typename T>
		T* tryGetComponent(entity::Entity e) {
			return entityRegistry.template tryGetComponent<T>(e);
		}

		inline entity::Registry& getRegistry() {
			return entityRegistry;
		}

		template<typename T, typename... Args>
		T* addSubsystem(Args&&... args) {
			auto sys = std::make_unique<T>(std::forward<Args>(args)...);
			T* rawPtr = sys.get();
			subsystems.emplace_back(std::move(sys));
			return rawPtr;
		}

		inline const std::vector<std::unique_ptr<ISceneSubsystem>>& getSubsystems() const {
			return subsystems;
		}
	private:
		entity::Registry entityRegistry;
		std::vector<std::unique_ptr<ISceneSubsystem>> subsystems;
	};
}