#pragma once
#include <memory>

#include "ISceneSubsystem.h"
#include "ICollidableTerrain.h"
#include "ITerrainBackend.h"
#include "SystemContext.h"

namespace engine::world {
	class TerrainSystem : public virtual scene::ISceneSubsystem, public ICollidableTerrain {
	public:
		void update(system::SystemContext& ctx) override {
			if (backend) backend->update(ctx.getCameraTransform()->position, ctx.dt);
		}

		void render(const glm::mat4& view, const glm::mat4& proj) override {
			if (backend) backend->render(view, proj);
		}

		bool isActive() const override {
			return true;
		}
		const char* getSubsystemName() const override { return "terrain"; }

		bool checkCollision(
			const component::ColliderComponent& movingCollider,
			const component::TransformComponent& movingTransform,
			std::vector<component::ColliderComponent>& outTerrainColliders,
			std::vector<component::TransformComponent>& outTerrainTransforms) const override {
			return backend && backend->checkCollision(movingCollider, movingTransform, outTerrainColliders, outTerrainTransforms);
		}

		void streamChunksAround(const glm::vec3 pos) {
			if (backend) backend->streamChunksAround(pos);
		}

		void streamChunksAroundThrottled(const glm::vec3 pos) {
			if (backend) {
				static glm::ivec2 lastChunkCoord = backend->worldToChunk(pos);
				glm::ivec2 current = backend->worldToChunk(pos);
				if (current != lastChunkCoord) {
					backend->streamChunksAround(pos);
					lastChunkCoord = current;
				}
			}
		}

		void setBackend(std::unique_ptr<ITerrainBackend> newBackend) {
			backend = std::move(newBackend);
		}

	private:
		std::unique_ptr<ITerrainBackend> backend;
	};
}