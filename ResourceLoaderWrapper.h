#pragma once
#include <memory>
#include <string>
#include <typeindex>

#include "Resource.h"
#include "IResourceLoader.h"
#include "IAnyLoader.h"
#include "TypeUtils.h"

namespace engine {
    namespace loader {
        template<typename Desc, typename Res>
        class ResourceLoaderWrapper : public IAnyloader {
        public:
            ResourceLoaderWrapper(std::unique_ptr<IResourceLoader<Desc, Res>> loader): loader(std::move(loader)) {}

            bool canLoad(const void* descriptor) const override {
                return loader->canLoad(*static_cast<const Desc*>(descriptor));
            }

            bool canLoad(std::type_index descriptorType, std::type_index resourceType) const override {
                const auto descType = engine::util::getTypeIndex<Desc>();
                const auto resType = engine::util::getTypeIndex<Res>();
                return descriptorType == descType && resourceType == resType;
            }

            std::unique_ptr<resource::Resource> load(const void* descriptor, std::type_index type) const override {
                if (type != engine::util::getTypeIndex<Res>()) return nullptr;

                std::unique_ptr<Res> result = loader->load(*static_cast<const Desc*>(descriptor));
                return std::unique_ptr<resource::Resource>(static_cast<resource::Resource*>(result.release()));
            }

            bool uploadGPU(resource::Resource& res) const override {
                Res* actual = dynamic_cast<Res*>(&res);
                if (!actual) return false;
                return loader->uploadGPU(*actual);
            }

        private:
            std::unique_ptr<IResourceLoader<Desc, Res>> loader;
        };
    }
}