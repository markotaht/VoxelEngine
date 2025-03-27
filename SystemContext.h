#pragma once
#include "Registry.h"
#include "NewResourceManager.h"

namespace engine::system {
	struct SystemContext {
		entity::Registry& ecs;
		resource::ResourceManager* resMan;
		float dt;
	};
}