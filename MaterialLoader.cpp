#include "MaterialLoader.h"
#include <memory>
#include <unordered_map>
#include <string>
#include <iostream>

#include "Types.h"
#include "ResourceManager.h"
#include "MaterialDescriptor.h"
#include "FileDescriptor.h"
#include "Texture2DArrayDescriptor.h"
#include "ITexture.h"
#include "Material.h"
#include "ResourceId.h"
#include "Texture2D.h"
#include "Texture2DArray.h"

#include "AutoLoaderRegistrar.h"
#include "ResourceManager.h"

namespace engine {
    namespace loader {
        static AutoLoaderRegistrar<engine::loader::MaterialLoader, IResourceLoader<descriptor::MaterialDescriptor, asset::Material>> regTex([](resource::ResourceManager& rm) {
            return std::make_unique<engine::loader::MaterialLoader>(rm);
            });

        bool MaterialLoader::canLoad(const descriptor::MaterialDescriptor& descriptor) const
        {
            return true;
        }
        std::unique_ptr<asset::Material> engine::loader::MaterialLoader::load(const descriptor::MaterialDescriptor& descriptor) const
        {
            core::ResourceId<asset::ShaderProgram> shaderProgramId = resourceManager.load<descriptor::ShaderDescriptor, asset::ShaderProgram>(descriptor.shaderDescriptor);
            resourceManager.addReference(shaderProgramId);
            std::unordered_map<std::string, core::ResourceId<asset::ITexture>> textures;
            for (const auto& entry : descriptor.textures) {
                const auto& id = entry.first;
                const auto& variant = entry.second;
                std::visit([this, &textures, &id](const auto& texDesc) {
                    using T = std::decay_t<decltype(texDesc)>;

                    if constexpr (std::is_same_v<T, descriptor::FileDescriptor>) {
                        auto texId = resourceManager.load<T, asset::Texture2D>(texDesc);
                        textures[id] = core::ResourceId<asset::ITexture>{ texId.value };
                        resourceManager.addReference(texId);
                    }
                    else if constexpr (std::is_same_v<T, descriptor::Texture2DArrayDescriptor>) {
                        auto texId = resourceManager.load<T, asset::Texture2DArray>(texDesc);
                        textures[id] = core::ResourceId<asset::ITexture>{ texId.value };
                        resourceManager.addReference(texId);
                    }
                    }, variant);
            }
            return std::make_unique<asset::Material>(shaderProgramId, descriptor.uniforms, textures);
        }
        bool MaterialLoader::uploadGPU(asset::Material& mat) const
        {
            if (mat.shaderProgramId) {
                if (resourceManager.isDirty(mat.shaderProgramId)) {
                    resourceManager.uploadIfDirty(mat.shaderProgramId);
                }
            }

            for (const auto& [name, texId] : mat.textures) {
                if (resourceManager.isDirty(texId)) {
                    resourceManager.uploadIfDirty(texId);
                }
            }

            return true;
        }
    }
}
