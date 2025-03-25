#pragma once
#include <memory>

#include <SDL.h>

#include <gl/glew.h>

#include "NewResource.h"

namespace engine {
	namespace asset {
		class Texture : public resource::Resource {

		public:
			using SurfacePtr = std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)>;

			Texture() = default;
			Texture(GLuint textureId, SurfacePtr loadedTexture)
				:textureID{ textureId }, loadedTexture(std::move(loadedTexture))
			{
			}
			~Texture();

			[[nodiscard]] inline int getWidth() const { return loadedTexture->w; }
			[[nodiscard]] inline int getHeight() const { return loadedTexture->h; }

			[[nodiscard]] inline void* getPixels() const { return loadedTexture->pixels; }
			[[nodiscard]] inline SDL_Surface* getSDLSurface() const { return loadedTexture.get(); }
			[[nodiscard]] GLuint getGLID() const { return textureID; }

			void unload();

		private:
			GLuint textureID;
			SurfacePtr loadedTexture;
		};
	}
}