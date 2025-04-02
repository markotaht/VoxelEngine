#include "Texture2D.h"
#include <SDL_surface.h>

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
    }
}
