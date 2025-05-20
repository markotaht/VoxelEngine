#pragma once
#include "ITexture.h"

#include <SDL.h>
#include <gl/glew.h>
#include <memory>
#include <vector>

namespace engine{
	namespace loader {
		class Texture2DArrayLoader;
	}

	namespace asset {
		using SurfacePtr = std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)>;
		struct TextureLayer {
			SurfacePtr surface;
			std::string name;
			std::string filePath;
		};

		class Texture2DArray : public ITexture {
		public:

			Texture2DArray(GLuint textureId, std::vector<asset::TextureLayer>&& layers, int width, int height)
				:textureId(textureId), layers(std::move(layers)), width(width), height(height) {}

			Texture2DArray(std::vector<asset::TextureLayer>&& layers, int width, int height)
				:layers(std::move(layers)), width(width), height(height), textureId(0) {}

			void bind(GLuint slot) const override;

			GLuint getID() const override { return textureId; }

			void unload();

			~Texture2DArray() {
				unload();
			}

			int getWidth() const { return width; }
			int getHeight() const { return height; }

			size_t estimateMemoryUsage() const;
			size_t estimateGpuMemoryUsage() const;
			std::string toString() { return ""; }

			friend class loader::Texture2DArrayLoader;

		private:
			GLuint textureId;
			int width;
			int height;
			std::vector<asset::TextureLayer> layers;
		};
	}
}