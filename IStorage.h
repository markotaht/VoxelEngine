#pragma once
#include "ResourceId.h"
#include "OutStream.h"

namespace engine::resource {
	class IStorage {
	public:
		virtual ~IStorage() = default;
		virtual std::vector<core::ResourceIdBase> unloadUnused() = 0;
		virtual void unloadAll() = 0;
		virtual void reloadAll() = 0;
		virtual void uploadAllDirty() = 0;
		virtual size_t estimateMemoryUsage() = 0;
		virtual size_t estimateGPUMemoryUsage() = 0;
		virtual void markDirty(core::ResourceIdBase id) = 0;
		virtual bool isDirty(core::ResourceIdBase id) = 0;
		virtual void clearDirty(core::ResourceIdBase id) = 0;
		virtual void addReference(core::ResourceIdBase id) = 0;
		virtual void removeReference(core::ResourceIdBase id) = 0;
		virtual void tick(uint64_t frameIndex) = 0;
		virtual void debugPrint(core::OutStream& out) const = 0;
		virtual void setLastSeenFrame(core::ResourceIdBase id, uint64_t frameIndex) = 0;
	};
}