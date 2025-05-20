#pragma once
#include<string>
#include <memory>

#include "IGpuUploader.h"

namespace engine {
    namespace loader {
        template <typename Desc, typename Res>
        class IResourceLoader : public IGpuUploader<Res>{
        public:

            using DescriptorType = Desc;
            using ResourceType = Res;

            core::TypeId getDescriptorType() const { return core::typeId<Desc>(); };
            virtual ~IResourceLoader() = default;

            // Can this loader handle this path?
            virtual bool canLoad(const Desc& descriptor) const = 0;

            // Load and return the resource (owned by caller)
            virtual std::unique_ptr<Res> load(const Desc& Descriptor) const = 0;
        };
    }
}