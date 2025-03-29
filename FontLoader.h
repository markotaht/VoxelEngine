#pragma once
#include "IResourceLoader.h"
#include "FileDescriptor.h"
#include "Font.h"

#include <memory>

namespace engine::loader {
    class FontLoader :
        public IResourceLoader<descriptor::FileDescriptor, asset::Font>
    {
    public:

        bool canLoad(const descriptor::FileDescriptor& descriptor) const;
        std::unique_ptr<asset::Font> load(const descriptor::FileDescriptor& descriptor) const;

    };
}

