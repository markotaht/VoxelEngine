#pragma once
#include <memory>
#include <string>
#include <typeindex>

#include "NewResource.h"
#include "IResourceLoader.h"
#include "IAnyLoader.h"
#include "TypeTag.h"

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
                using engine::core::TypeTag;
                return descriptorType == TypeTag<Desc>::index() && resourceType == TypeTag<Res>::index();
            }

            std::unique_ptr<resource::Resource> load(const void* descriptor, std::type_index type) const override {
                using engine::core::TypeTag;
                if (type != TypeTag<Res>::index()) return nullptr;

                std::unique_ptr<Res> result = loader->load(*static_cast<const Desc*>(descriptor));
                return std::unique_ptr<resource::Resource>(static_cast<resource::Resource*>(result.release()));
            }

        private:
            std::unique_ptr<IResourceLoader<Desc, Res>> loader;
        };
    }
}