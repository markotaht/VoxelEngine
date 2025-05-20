#pragma once
#include <memory>
#include <string>

#include <SDL.h>

#include <gl/glew.h>

#include "ITexture.h"



namespace engine {
	namespace loader {
		class TextureLoader;
	}

	namespace asset {
		class Texture2D : public ITexture{

		public:
			using SurfacePtr = std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)>;

			Texture2D() = default;
			Texture2D(GLuint textureId, SurfacePtr loadedTexture, std::string sourcePath)
				:textureID{ textureId }, loadedTexture(std::move(loadedTexture)), sourcePath(sourcePath)
			{
			}
			Texture2D(SurfacePtr loadedTexture, std::string sourcePath):loadedTexture(std::move(loadedTexture)), textureID(0), sourcePath(sourcePath) {}


			void bind(GLuint slot) const override;

			~Texture2D();

			[[nodiscard]] inline int getWidth() const { return loadedTexture->w; }
			[[nodiscard]] inline int getHeight() const { return loadedTexture->h; }

			[[nodiscard]] inline void* getPixels() const { return loadedTexture->pixels; }
			[[nodiscard]] inline SDL_Surface* getSDLSurface() const { return loadedTexture.get(); }
			[[nodiscard]] GLuint getID() const override { return textureID; }

			void unload();
			size_t estimateMemoryUsage() const;
			size_t estimateGpuMemoryUsage() const;

			std::string toString();
			friend class loader::TextureLoader;

		private:
			GLuint textureID;
			SurfacePtr loadedTexture;
			std::string sourcePath;
		};
	}
}