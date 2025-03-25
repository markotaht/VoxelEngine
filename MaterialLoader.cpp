#include "MaterialLoader.h"
#include <memory>
#include <unordered_map>
#include <string>
#include <iostream>

#include "Types.h"
#include "NewResourceManager.h"
#include "MaterialDescriptor.h"
#include "FileDescriptor.h"
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
            std::cout << shaderProgramId;
            std::unordered_map<std::string, core::ResourceId<asset::Texture>> textures;
            for (auto& [id, textureDescriptor] : descriptor.textures) {
                textures[id] = resourceManager.load<asset::Texture>(textureDescriptor);
            }
            return std::make_unique<asset::Material>(shaderProgramId, descriptor.uniforms, textures);
        }
    }
}
