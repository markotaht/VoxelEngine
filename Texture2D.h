#pragma once
#include <memory>

#include <SDL.h>

#include <gl/glew.h>

#include "ITexture.h"

namespace engine {
	namespace asset {
		class Texture2D : public ITexture{

		public:
			using SurfacePtr = std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)>;

			Texture2D() = default;
			Texture2D(GLuint textureId, SurfacePtr loadedTexture)
				:textureID{ textureId }, loadedTexture(std::move(loadedTexture))
			{
			}


			void bind(GLuint slot) const override;

			~Texture2D();

			[[nodiscard]] inline int getWidth() const { return loadedTexture->w; }
			[[nodiscard]] inline int getHeight() const { return loadedTexture->h; }

			[[nodiscard]] inline void* getPixels() const { return loadedTexture->pixels; }
			[[nodiscard]] inline SDL_Surface* getSDLSurface() const { return loadedTexture.get(); }
			[[nodiscard]] GLuint getID() const override { return textureID; }

			void unload();

		private:
			GLuint textureID;
			SurfacePtr loadedTexture;
		};
	}
}