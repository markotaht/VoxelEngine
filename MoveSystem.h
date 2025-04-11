#pragma once
#include "ISystem.h"

namespace engine::system {
	class MoveSystem : public ISystem
	{
	public:
		void update(SystemContext& ctx) override;
	};
}