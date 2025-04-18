#include "LookSystem.h"
#include "TagPlayerControlled.h"
#include "TransformComponent.h"
#include "PitchYawRotation.h"
#include "InputHandler.h"

#include <glm/glm.hpp>
void engine::system::LookSystem::update(SystemContext& ctx)
{
	entity::Registry& ecs = ctx.ecs;

	InputHandler& inputHandler = InputHandler::getInstance();

	ecs.forEach<component::TransformComponent, component::PitchYawRotationComponent, component::TagPlayerControlled>([&](
		entity::Entity e, 
		component::TransformComponent& transform,
		component::PitchYawRotationComponent& pitchYaw,
		component::TagPlayerControlled& tag) {

		glm::vec2 mouseDiff = inputHandler.getMouseMovement();
		pitchYaw.yaw += mouseDiff.x * inputHandler.mouseSensitivity;
		pitchYaw.pitch += mouseDiff.y * inputHandler.mouseSensitivity;

		pitchYaw.pitch = glm::clamp(pitchYaw.pitch, glm::radians(-89.0f), glm::radians(89.0f));

		glm::quat qPitch = glm::angleAxis(-pitchYaw.pitch, glm::vec3(1, 0, 0));
		glm::quat qYaw = glm::angleAxis(-pitchYaw.yaw, glm::vec3(0, 1, 0));
		glm::quat orientation = qYaw * qPitch;

		transform.rotation = orientation;
		});

}
