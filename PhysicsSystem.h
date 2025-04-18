#pragma once
#include "ISystem.h"

#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"

namespace engine::system {
    using namespace component;

    struct CollisionInfo {
        glm::vec3 normal;
        float penetration;
        bool collided;
    };

    class PhysicsSystem : public ISystem
    {
    public:
        void update(SystemContext& ctx) override;
    private:
        CollisionInfo checkCollision(const TransformComponent& a, const ColliderComponent& ca,
            const TransformComponent& b, const ColliderComponent& cb);

        glm::vec3 closestPointToAABB(const engine::component::TransformComponent& aabb, const engine::component::ColliderComponent& caabb, const engine::component::TransformComponent& sphere);

        void resolveCollision(TransformComponent& a, RigidBodyComponent& rbA, TransformComponent& b, RigidBodyComponent& rbB, const CollisionInfo& collision);

        CollisionInfo PhysicsSystem::checkSphereToAABB(
            const TransformComponent& sphere, const ColliderComponent& csphere,
            const TransformComponent& aabb, const ColliderComponent& caabb);

    };
}

