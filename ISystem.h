#pragma once
#include "Registry.h"
#include "SystemContext.h"

namespace engine::system {
	class ISystem {
	public:
		virtual void update(SystemContext& ctx) = 0;
	};
}