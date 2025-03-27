#pragma once
#include <vector>

#include "IComponentStorage.h"
namespace engine::component {

    template<typename Component>
    class ComponentStorage: public IComponentStorage {
    public:
        std::vector<Component> components;
        std::vector<engine::entity::Entity> entities;

        void add(engine::entity::Entity e, const Component& c) {
            entities.push_back(e);
            components.push_back(c);
        }

        Component* get(engine::entity::Entity e) {
            for (size_t i = 0; i < entities.size(); ++i) {
                if (entities[i] == e) return &components[i];
            }
            return nullptr;
        }

        bool has(engine::entity::Entity e) const {
            return std::find(entities.begin(), entities.end(), e) != entities.end();
        }

        void remove(engine::entity::Entity e) {
            for (size_t i = 0; i < entities.size(); ++i) {
                if (entities[i] == e) {
                    entities.erase(entities.begin() + i);
                    components.erase(components.begin() + i);
                    return;
                }
            }
        }

        size_t size() const { return components.size(); }
        const std::vector<engine::entity::Entity>& getEntities() const { return entities; }
        std::vector<Component>& getComponents() { return components; }
    };
}