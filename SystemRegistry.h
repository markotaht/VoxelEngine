#pragma once
#include <cstddef>
#include <unordered_map>
#include <memory>
#include "ISystem.h"
#include "SystemContext.h"

namespace engine::system {
	using SystemTypeId = std::size_t;

	inline SystemTypeId nextSystemTypeId(){
		static SystemTypeId id = 0;
		return id++;
	}

	template <typename T>
	SystemTypeId getSystemTypeId() {
		static SystemTypeId id = nextSystemTypeId();
		return id;
	}

	class SystemRegistry {
	public:
		static SystemRegistry& instance() {
			static SystemRegistry registry;
			return registry;
		}

		template<typename T, typename... Args>
		T& registerSystem(Args&... args) {
			SystemTypeId systemId = getSystemTypeId<T>();
			auto system = std::make_unique<T>(args...);
			T* ptr = system.get();
			frameUpdateSystems[systemId] = std::move(system);
			return *ptr;
		}

		template<typename T, typename... Args>
		T& registerFixedUpdateSystem(Args&... args) {
			SystemTypeId systemId = getSystemTypeId<T>();
			auto system = std::make_unique<T>(args...);
			T* ptr = system.get();
			fixedUpdateSystems[systemId] = std::move(system);
			return *ptr;
		}

		template<typename T>
		T* getSystem() {
			SystemTypeId systemId = getSystemTypeId<T>();
			auto it = frameUpdateSystems.find(systemId);
			if (it != frameUpdateSystems.end()) {
				return static_cast<T*>(it->second.get());
			}
			else {
				auto it = fixedUpdateSystems.find(systemId);
				if (it != fixedUpdateSystems.end()) {
					return static_cast<T*>(it->second.get());
				}
				else {
					return nullptr;
				}
			}
		}

		inline void updateAll(SystemContext& ctx){
			for (auto& [_, system] : frameUpdateSystems) {
				system->update(ctx);
			}
		}

		inline void fixedUpdateAll(SystemContext& ctx) {
			for (auto& [_, system] : fixedUpdateSystems) {
				system->update(ctx);
			}
		}


	private:
		std::unordered_map<SystemTypeId, std::unique_ptr<ISystem>> frameUpdateSystems;
		std::unordered_map<SystemTypeId, std::unique_ptr<ISystem>> fixedUpdateSystems;

	};
}