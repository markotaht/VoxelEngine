#pragma once
#include "IResourceLoader.h"
#include "ResourceLoaderWrapper.h"
#include "Types.h"
#include "NewTextMaterial.h"
#include "TextMaterialDescriptor.h"

template class engine::loader::ResourceLoaderWrapper<engine::descriptor::TextMaterialDescriptor, engine::asset::TextMaterial>;

namespace engine {
    namespace loader {
        class TextMaterialLoader :
            public IResourceLoader<descriptor::TextMaterialDescriptor, asset::TextMaterial>
        {
        public:
            TextMaterialLoader(resource::ResourceManager& resourceManager) : resourceManager(resourceManager) {}

            bool canLoad(const descriptor::TextMaterialDescriptor& descriptor) const;
            std::unique_ptr<asset::TextMaterial> load(const descriptor::TextMaterialDescriptor& descriptor) const;

        private:
            resource::ResourceManager& resourceManager;
        };
    }
}