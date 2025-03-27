#pragma once

namespace engine::component {
	class IComponentStorage {
	public:
		virtual ~IComponentStorage() = default;
		virtual size_t size() const = 0;
	};
}