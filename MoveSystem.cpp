#include "MoveSystem.h"
#include "TagPlayerControlled.h"
#include "TransformComponent.h"
#include "InputHandler.h"

#include "AutoSystemRegistrar.h"

#include <glm/glm.hpp>

namespace engine::system {

	static AutoSystemRegistrar<MoveSystem> _regMoveSys;

	void MoveSystem::update(SystemContext& ctx)
	{
		entity::Registry& ecs = ctx.getRegistry();

		InputHandler& inputHandler = InputHandler::getInstance();

		ecs.forEach<component::TransformComponent, component::TagPlayerControlled>([&](entity::Entity e, component::TransformComponent& transform, component::TagPlayerControlled& tag) {
			glm::vec3 dir = { inputHandler.getHorizontalAxis(), 0, inputHandler.getVerticalAxis() };
			float scale = 10.0f;
			if (inputHandler.isPressed(SDL_SCANCODE_LSHIFT)) {
				scale = 100.0f;
			}
			if (glm::length(dir) > 0.0f) {
				dir = glm::normalize(dir);
				transform.position += (transform.rotation * dir) * scale * ctx.dt;
			}
			});

	}
}
