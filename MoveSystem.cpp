#include "MoveSystem.h"
#include "TagPlayerControlled.h"
#include "TransformComponent.h"
#include "InputHandler.h"

#include <glm/glm.hpp>

void engine::system::MoveSystem::update(SystemContext& ctx)
{
	entity::Registry& ecs = ctx.ecs;

	InputHandler& inputHandler = InputHandler::getInstance();

	ecs.forEach<component::TransformComponent, component::TagPlayerControlled>([&](entity::Entity e, component::TransformComponent& transform, component::TagPlayerControlled& tag) {
		glm::vec3 dir = { inputHandler.getHorizontalAxis(), 0, inputHandler.getVerticalAxis() };
		if (glm::length(dir) > 0.0f) {
			dir = glm::normalize(dir);
			transform.position += (transform.rotation * dir) * 10.0f * ctx.dt;
		}
	});

}
