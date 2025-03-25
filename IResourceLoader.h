#pragma once
#include<string>
#include <memory>

namespace engine {
    namespace loader {
        template <typename Desc, typename Res>
        class IResourceLoader {
        public:

            using DescriptorType = Desc;
            using ResourceType = Res;
            virtual ~IResourceLoader() = default;

            // Can this loader handle this path?
            virtual bool canLoad(const Desc& descriptor) const = 0;

            // Load and return the resource (owned by caller)
            virtual std::unique_ptr<Res> load(const Desc& Descriptor) const = 0;
        };
    }
}