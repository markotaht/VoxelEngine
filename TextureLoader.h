#pragma once
#include "IResourceLoader.h"
#include "FileDescriptor.h"
#include "Texture2D.h"

namespace engine {
	namespace loader {
		class TextureLoader : public IResourceLoader<descriptor::FileDescriptor, asset::Texture2D>
		{
		public:
			
			TextureLoader() = default;

			bool canLoad(const descriptor::FileDescriptor& desc) const;
			std::unique_ptr<asset::Texture2D> load(const descriptor::FileDescriptor& desc) const;

			bool operator==(const TextureLoader& other) const
			{
				return false;
			}
		};
	}
}

