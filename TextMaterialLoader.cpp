#include "TextMaterialLoader.h"
#include "ShaderDescriptor.h"
#include "NewShaderProgram.h"
#include "ResourceManager.h"
#include "TextMaterialDescriptor.h"
#include "Font.h"
#include <memory>

#include "AutoLoaderRegistrar.h"
#include "ResourceManager.h"

namespace engine::loader {

    static AutoLoaderRegistrar<engine::loader::TextMaterialLoader, IResourceLoader<descriptor::TextMaterialDescriptor, asset::TextMaterial>> regTex([](resource::ResourceManager& rm) {
        return std::make_unique<engine::loader::TextMaterialLoader>(rm);
        });

    bool TextMaterialLoader::canLoad(const descriptor::TextMaterialDescriptor& descriptor) const
    {
        return true;
    }

    std::unique_ptr<asset::TextMaterial> TextMaterialLoader::load(const descriptor::TextMaterialDescriptor& descriptor) const
    {
        core::ResourceId<asset::ShaderProgram> shaderProgramId = resourceManager.load<descriptor::ShaderDescriptor, asset::ShaderProgram>(descriptor.shaderDescriptor);
        core::ResourceId<asset::Font> fontId = resourceManager.load<asset::Font>(descriptor.fontDescriptor);
        return std::make_unique<asset::TextMaterial>(shaderProgramId, fontId);
    }
}
