#pragma once
#include "ISystem.h"

namespace engine::system {
	class LookSystem : public ISystem
	{
	public:
		void update(SystemContext& ctx) override;
	};
}