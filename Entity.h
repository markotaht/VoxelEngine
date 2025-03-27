#pragma once
#include <cstdint>
#include <limits>

namespace engine::entity {
	constexpr uint32_t INVALID_ENTITY = std::numeric_limits<uint32_t>::max();

	using Entity = uint32_t;
}