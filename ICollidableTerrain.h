#pragma once
#include "ISceneSubsystem.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"

#include <vector>

namespace engine::world {
	class ICollidableTerrain : public virtual scene::ISceneSubsystem {
	public:
		virtual bool checkCollision(
			const component::ColliderComponent& movingCollider,
			const component::TransformComponent& movingTransform,
			std::vector<component::ColliderComponent>& outTerrainColliders,
			std::vector<component::TransformComponent>& outTerrainTransforms) const = 0;
	};
}