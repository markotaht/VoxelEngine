#pragma once
#include "ResourceManager.h"
#include "IResourceLoader.h"

#include <functional>
#include <memory>

namespace engine::loader {
    template<typename LoaderType, typename InterfaceType>
    struct AutoLoaderRegistrar {
        AutoLoaderRegistrar(std::function<std::unique_ptr<InterfaceType>(resource::ResourceManager&)> factory) {
            resource::ResourceManager& resMan = resource::ResourceManager::instance();
            resMan.registerLoader(factory(resMan));
        }
    };
}