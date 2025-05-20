#pragma once
#include "IGpuUploaderBase.h"
#include "TypeId.h"

namespace engine::loader {
	
	template<typename Res>
	class IGpuUploader : public IGpuUploaderBase {
	public:
		bool uploadGPU(void* resource) {
			return uploadGPU(*static_cast<Res*>(resource));
		}

		core::TypeId getResourceType() const { return core::typeId<Res>(); };

		virtual bool uploadGPU(Res& resource) const = 0;
	};
}