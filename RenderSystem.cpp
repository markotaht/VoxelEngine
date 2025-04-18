#include "RenderSystem.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "MeshRendererComponent.h"
#include "Entity.h"
#include "Registry.h"

#include "NewMesh.h"
#include "NewMaterial.h"

namespace engine::system {
	void engine::system::RenderSystem::update(SystemContext& ctx)
	{
		entity::Entity activeCamera = entity::INVALID_ENTITY;
		component::TransformComponent* camTransform = nullptr;
		component::CameraComponent* cam = nullptr;

		entity::Registry& ecs = ctx.ecs;

		ecs.forEach<component::CameraComponent>([&](entity::Entity e, component::CameraComponent& c) {
			activeCamera = e;
			cam = &c;
			camTransform = ecs.tryGetComponent<component::TransformComponent>(e);
		});

		ecs.forEach<component::TransformComponent, component::MeshRendererComponent>([&](entity::Entity e, component::TransformComponent& transform, component::MeshRendererComponent& meshRenderer) {
			asset::Mesh* mesh = ctx.resMan->get<asset::Mesh>(meshRenderer.meshId);
			asset::Material* material = ctx.resMan->get<asset::Material>(meshRenderer.materialId);

			if (!mesh || !material) return;

			material->bind(transform.getMatrix(), camTransform->getInverseMatrix(), cam->getProjectionMatrix(), *ctx.resMan);
			mesh->bind();
			mesh->render();
		});
	}
}
