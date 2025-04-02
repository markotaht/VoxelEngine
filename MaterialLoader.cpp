#include "MaterialLoader.h"
#include <memory>
#include <unordered_map>
#include <string>
#include <iostream>

#include "Types.h"
#include "NewResourceManager.h"
#include "MaterialDescriptor.h"
#include "FileDescriptor.h"
#include "Texture2DArrayDescriptor.h"
#include "ITexture.h"
#include "NewMaterial.h"
#include "ResourceId.h"

namespace engine {
    namespace loader {
        bool MaterialLoader::canLoad(const descriptor::MaterialDescriptor& descriptor) const
        {
            return true;
        }
        std::unique_ptr<asset::Material> engine::loader::MaterialLoader::load(const descriptor::MaterialDescriptor& descriptor) const
        {
            core::ResourceId<asset::ShaderProgram> shaderProgramId = resourceManager.load<descriptor::ShaderDescriptor, asset::ShaderProgram>(descriptor.shaderDescriptor);
            std::unordered_map<std::string, core::ResourceId<asset::ITexture>> textures;
            for (const auto& entry : descriptor.textures) {
                const auto& id = entry.first;
                const auto& variant = entry.second;
                std::visit([this, &textures, &id](const auto& texDesc) {
                    using T = std::decay_t<decltype(texDesc)>;

                    if constexpr (std::is_same_v<T, descriptor::FileDescriptor>) {
                        auto texId = resourceManager.load<T, asset::Texture2D>(texDesc);
                        textures[id] = core::ResourceId<asset::ITexture>{ texId.value };
                    }
                    else if constexpr (std::is_same_v<T, descriptor::Texture2DArrayDescriptor>) {
                        auto texId = resourceManager.load<T, asset::Texture2DArray>(texDesc);
                        textures[id] = core::ResourceId<asset::ITexture>{ texId.value };
                    }
                    }, variant);
            }
            return std::make_unique<asset::Material>(shaderProgramId, descriptor.uniforms, textures);
        }
    }
}
