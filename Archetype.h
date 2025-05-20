#pragma once
#include <cstddef>
#include <bitset>
#include <unordered_map>
#include <vector>
#include <memory>


#include "Entity.h"
#include "IComponentArray.h"
#include "ComponentArray.h"

namespace engine::entity {
	using ComponentTypeId = std::size_t;
	constexpr std::size_t MAX_COMPONENTS = 256;
	using ArchetypeKey = std::bitset<MAX_COMPONENTS>;


	inline ComponentTypeId nextComponentTypeID() {
		static ComponentTypeId id = 0;
		return id++;
	}

	template <typename T>
	ComponentTypeId getComponentTypeId() {
		static ComponentTypeId id = nextComponentTypeID();
		return id;
	}

	template<typename... Components>
	inline ArchetypeKey getArchetypeKey() {
		ArchetypeKey key;
		(key.set(getComponentTypeId<Components>()), ...);
		return key;
	}

	class Archetype {
	public:
		template<typename T>
		void addComponentData(const T& component) {
			auto& array = getOrCreateComponentArray<T>();
			array.add(component);
		}

		template<typename T>
		T* getComponentArray() {
			return reinterpret_cast<T*>(componentData[getComponentTypeId<T>()]->getRawData());
		}

		template<typename T>
		T* getComponent(size_t index) {
			return &reinterpret_cast<T*>(componentData[getComponentTypeId<T>()]->getRawData())[index];
		}

		inline void addEntity(Entity entity) {
			entities.push_back(entity);
		}

		inline Entity* getEntityArray() {
			return entities.data();
		}

		template<typename... Components>
		bool hasComponents() const {
			return (... && (componentData.contains(getComponentTypeId<Components>())));
		}

		inline std::size_t size() const {
			return entities.size();  // Or pick any component array length
		}

	private:
		std::unordered_map<ComponentTypeId, std::unique_ptr<IComponentArray>> componentData;
		std::vector<Entity> entities;

		template<typename T>
		ComponentArray<T>& getOrCreateComponentArray() {
			ComponentTypeId id = getComponentTypeId<T>();

			auto [it, inserted] = componentData.try_emplace(id, std::make_unique<ComponentArray<T>>());
			return *static_cast<ComponentArray<T>*>(it->second.get());
		}
	};
}