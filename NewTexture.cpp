#include "NewTexture.h"
#include <SDL_surface.h>

namespace engine {
    namespace asset {
        void Texture::unload()
        {
            if (textureID != 0) {
                glDeleteTextures(1, &textureID);
                textureID = 0;
            }

            loadedTexture.reset();
        }

        Texture::~Texture() {
            unload();
        }
    }
}
