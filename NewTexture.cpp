#include "Texture2D.h"
#include <SDL_surface.h>
#include <format>

namespace engine {
    namespace asset {
        void Texture2D::unload()
        {
            if (textureID != 0) {
                glDeleteTextures(1, &textureID);
                textureID = 0;
            }

            loadedTexture.reset();
        }

        Texture2D::~Texture2D() {
            unload();
        }

        size_t Texture2D::estimateMemoryUsage() const {
            if (!loadedTexture) return 0;

            return loadedTexture->pitch * loadedTexture->h;
        }

        size_t Texture2D::estimateGpuMemoryUsage() const {
            if (!loadedTexture) return 0;

            const SDL_PixelFormat* fmt = loadedTexture->format;
            int bytesPerPixel = fmt->BytesPerPixel;  // SDL handles padding/packing

            return loadedTexture->w * loadedTexture->h * bytesPerPixel;
        }
        std::string Texture2D::toString()
        {
            return std::format(
                "Texture2D{{ width: {}, height: {}, sourcePath: {} }}",
                getWidth(), getHeight(), sourcePath);
        }
    }
}
