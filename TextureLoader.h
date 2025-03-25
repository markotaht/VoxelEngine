#pragma once
#include "IResourceLoader.h"
#include "FileDescriptor.h"
#include "NewTexture.h"

namespace engine {
	namespace loader {
		class TextureLoader : public IResourceLoader<descriptor::FileDescriptor, asset::Texture>
		{
		public:
			
			TextureLoader() = default;

			bool canLoad(const descriptor::FileDescriptor& desc) const;
			std::unique_ptr<asset::Texture> load(const descriptor::FileDescriptor& desc) const;

			bool operator==(const TextureLoader& other) const
			{
				return false;
			}
		};
	}
}

