#pragma once
#include <glm/glm.hpp>

namespace engine::world {
    class ITerrain {
    public:
        virtual ~ITerrain() = default;

        // Query
        virtual float getHeightAt(float x, float z) const = 0;
        virtual glm::vec3 getNormalAt(float x, float z) const = 0;
        virtual bool isSolidAt(const glm::vec3& worldPos) const = 0;

        // Modification
        virtual void setMaterialAt(const glm::vec3& worldPos, int materialID) = 0;
        virtual void removeAt(const glm::vec3& worldPos) = 0;

        // Raycast
       // virtual bool raycast(const Ray& ray, RaycastHit& hitOut) const = 0;

        // Chunking (optional)
        virtual void update(float deltaTime) = 0;
        virtual void render() const = 0;
    };
}