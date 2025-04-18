#pragma once

namespace engine::entity {
	class IComponentArray {
	public:
		virtual void addRaw(const void* data) = 0;
		virtual void* getRawData() = 0;
		virtual ~IComponentArray() = default;
	};
}