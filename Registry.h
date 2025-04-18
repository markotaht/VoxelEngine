#pragma once
#include "Entity.h"
#include "Archetype.h"

#include <cassert>
#include <memory>
#include <unordered_map>

namespace engine::entity {
    struct EntityLocation {
        Archetype* archetype;
        size_t index;
    };


	class Registry {
    public:
        Entity nextEntity = 1;

        Entity create() {
            return nextEntity++;
        }

        template<typename... Components>
        void addEntity(Entity entity, Components&&... components) {
            ArchetypeKey key = getArchetypeKey<std::decay_t<Components>...>();
            Archetype& archetype = getOrCreateArchetype(key);

            std::size_t index = archetype.size();
            archetype.addEntity(entity);
            (archetype.addComponentData(std::forward<Components>(components)), ...);

            entityToArchetype.try_emplace(entity, EntityLocation{ &archetype, index });
        }

        template<typename... Components, typename Func>
        void forEach(Func func){
            for (auto& [_, archetype] : archetypes) {
                if (archetype->hasComponents<Components...>()) {
                    Entity* entityArray = archetype->getEntityArray();

                    std::tuple<Components*...> componentArrays = { archetype->getComponentArray<Components>()... };

                    for (size_t i = 0; i < archetype->size(); i++) {
                        applyToEntity(func, i, entityArray[i], componentArrays, std::index_sequence_for<Components...>{});
                    }
                }
            }
        }

        template<typename T>
        T& getComponent(Entity e) {
            EntityLocation& loc = entityToArchetype[e];  // Safe because you control input
            assert(loc.archetype && "Entity has no valid archetype!");
            assert(loc.archetype->hasComponents<T>() && "Entity is missing required component!");
            return loc.archetype->getComponentArray<T>()[loc.index];
        }

        template<typename T>
        T* tryGetComponent(Entity e) {
            EntityLocation& loc = entityToArchetype[e];  // Safe because you control input
            assert(loc.archetype && "Entity has no valid archetype!");
            assert(loc.archetype->hasComponents<T>() && "Entity is missing required component!");
            return &loc.archetype->getComponentArray<T>()[loc.index];
        }

    private:
        std::unordered_map<ArchetypeKey, std::unique_ptr<Archetype>> archetypes;
        std::unordered_map<Entity, EntityLocation> entityToArchetype;

        inline Archetype& getOrCreateArchetype(ArchetypeKey key) {
            auto [it, inserted] = archetypes.try_emplace(key, std::make_unique<Archetype>());
            return *it->second.get();
        }

        template<typename Func, typename... Components, std::size_t... Indices>
        void applyToEntity(Func& func, std::size_t i, Entity entity, std::tuple<Components*...>& arrays, std::index_sequence<Indices...>) {
            func(entity, (std::get<Indices>(arrays)[i])...);
        }
	};
}