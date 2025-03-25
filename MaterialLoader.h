#pragma once
#include "IResourceLoader.h"
#include "ResourceLoaderWrapper.h"
#include "Types.h"
#include "NewMaterial.h"
#include "MaterialDescriptor.h"

template class engine::loader::ResourceLoaderWrapper<engine::descriptor::MaterialDescriptor, engine::asset::Material>;

namespace engine {
    namespace loader {
        class MaterialLoader :
            public IResourceLoader<descriptor::MaterialDescriptor, asset::Material>
        {
        public:
            MaterialLoader(resource::ResourceManager& resourceManager) : resourceManager(resourceManager){}

            bool canLoad(const descriptor::MaterialDescriptor& descriptor) const;
            std::unique_ptr<asset::Material> load(const descriptor::MaterialDescriptor& Descriptor) const;

        private:
            resource::ResourceManager& resourceManager;
        };
    }
}

