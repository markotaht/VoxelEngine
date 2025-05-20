#include "RenderSystem.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "MeshRendererComponent.h"
#include "Entity.h"
#include "Registry.h"

#include "Mesh.h"
#include "Material.h"
#include "AutoSystemRegistrar.h"

namespace engine::system {

	static AutoSystemRegistrar<RenderSystem> _regRenderSys;

	void engine::system::RenderSystem::update(SystemContext& ctx)
	{
		component::TransformComponent* camTransform = ctx.getCameraTransform();
		component::CameraComponent* cam = ctx.getCameraComponent();

		if (!cam || !camTransform) return;

		entity::Registry& ecs = ctx.getRegistry();

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
