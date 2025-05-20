#pragma once
#include "TypeId.h"

namespace engine::loader {
	class IGpuUploaderBase {
	public:
		virtual bool uploadGPU(void* resource) = 0;
		virtual core::TypeId getResourceType() const = 0;
		virtual ~IGpuUploaderBase() = default;
	};
}