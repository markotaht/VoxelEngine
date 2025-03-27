#pragma once
#include "ISystem.h"

namespace engine::system {
	class RenderSystem : public ISystem
	{
	public:
		void update(SystemContext& ctx) override;
	};
}

