#pragma once
#include "IResourceLoader.h"
#include "Texture2DArrayDescriptor.h"
#include "Texture2DArray.h"

#include <memory>
#include <SDL.h>
#include <string>
#include <unordered_map>

namespace engine {
	namespace loader {
		class Texture2DArrayLoader : public IResourceLoader<descriptor::Texture2DArrayDescriptor, asset::Texture2DArray>
		{
		public:

			Texture2DArrayLoader() = default;

			bool canLoad(const descriptor::Texture2DArrayDescriptor& desc) const;
			std::unique_ptr<asset::Texture2DArray> load(const descriptor::Texture2DArrayDescriptor& desc) const;
			bool uploadGPU(asset::Texture2DArray& tex) const;


			bool operator==(const Texture2DArrayLoader& other) const
			{
				return false;
			}
		private:
			std::vector<asset::TextureLayer> loadImages(const descriptor::Texture2DArrayDescriptor& desc) const;
		};
	}
}