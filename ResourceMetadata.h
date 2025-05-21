#pragma once
#include <typeindex>
#include <vector>
#include <format>
#include <string>

#include "ResourceId.h"
#include "TypeId.h"

namespace engine::resource {
	template<typename T>
	struct ResourceMetadata {
		core::TypeId descriptorType;
		bool dirty = true;
		size_t refCount = 0;
		uint64_t lastUsedFrame = 0;
		size_t estimatesMemoryUsage = 0;
		size_t estimatedGpuMemoryUsage = 0;
		std::vector<core::ResourceIdBase> hardDependencies;
		// sizeBytes, tags, lastUsedFrame, etc.;

		ResourceMetadata(
			core::TypeId descriptorType,
			bool dirty = true,
			size_t refCount = 0,
			uint64_t lastUsedFrame = 0,
			size_t estimatesCpuMemoryUsage = 0,
			size_t estimatedGpuMemoryUsage = 0)
			: descriptorType(descriptorType),
			dirty(dirty), refCount(refCount), lastUsedFrame(lastUsedFrame)
			, estimatesMemoryUsage(estimatesCpuMemoryUsage), estimatedGpuMemoryUsage(estimatedGpuMemoryUsage) {
		}

		std::string formatMetadata() const{
			return std::format(
				"ResourceMetadata<{}>{{ descriptorType: {}, dirty: {}, refCount: {}, lastUsedFrame: {}, "
				"estimatesMemoryUsage: {}, estimatedGpuMemoryUsage: {} }}",
				core::TypeName<T>(), descriptorType, dirty, refCount,
				lastUsedFrame, estimatesMemoryUsage, estimatedGpuMemoryUsage);
		}
	};
}