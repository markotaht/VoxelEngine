#pragma once
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "CollisionInfo.h"

#include <vector>

namespace engine::world {
	class ITerrainCollision {
	public:
		virtual ~ITerrainCollision() = default;
		virtual bool checkCollision(const component::ColliderComponent& bounds, const component::TransformComponent& transform,
			std::vector<component::ColliderComponent>& collidersOut, std::vector<component::TransformComponent>& transformsOut ) const = 0;
	};
}