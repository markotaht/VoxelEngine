#pragma once
#include "TransformComponent.h"
#include "CameraComponent.h"

namespace engine::core {
	struct CameraData {
		component::TransformComponent* transform = nullptr;
		component::CameraComponent* camera = nullptr;
	};
}