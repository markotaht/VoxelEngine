#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "ColliderComponent.h"
#include "TransformComponent.h"

namespace engine::world {
	enum ChunkStatus {
		Unloaded, Queued, Loading, Loaded
	};

	class ITerrainBackend {
	public:
		virtual ~ITerrainBackend() = default;

		virtual void render(const glm::mat4& view, const glm::mat4& projection) const = 0;
		virtual void update(const glm::vec3& cameraWorldPosition, float dt) = 0;
		virtual bool checkCollision(
			const component::ColliderComponent& bounds,
			const component::TransformComponent& transform, 
			std::vector<component::ColliderComponent>& collidersOut, 
			std::vector<component::TransformComponent>& transformsOut) const = 0;
		virtual void streamChunksAround(const glm::vec3 pos) = 0;
		virtual glm::ivec2 worldToChunk(glm::vec3 pos) const = 0;
	};
}