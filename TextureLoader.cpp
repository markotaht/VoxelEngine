#include "TextureLoader.h"

#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>

#include <memory>
#include <string>

#include <SDL_image.h>
#include <SDL_surface.h>

#include "NewTexture.h"
#include "StringUtils.h"

namespace engine {
    namespace loader {

        bool TextureLoader::canLoad(const descriptor::FileDescriptor& desc) const
        {
            return ends_with(desc.filePath, ".png") || ends_with(desc.filePath, ".jpg");
        }

        std::unique_ptr<asset::Texture> TextureLoader::load(const descriptor::FileDescriptor& desc) const
        {
            SDL_Surface* loadedSurface = IMG_Load(desc.filePath.c_str());
            if (loadedSurface == NULL) {
                printf("Unable to load image %s! SDL_image Error: %s\n", desc.filePath.c_str(), IMG_GetError());
                return nullptr;
            }

            GLuint GLtexture = 0;
            glGenTextures(1, &GLtexture);
            glBindTexture(GL_TEXTURE_2D, GLtexture);

            GLint internalFormat = GL_RGB8;
            GLenum format = GL_RGB;

            if (loadedSurface->format->BytesPerPixel == 4) {
                internalFormat = GL_RGBA8;
                format = GL_RGBA;
            }

            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat,
                loadedSurface->w, loadedSurface->h, 0,
                format, GL_UNSIGNED_BYTE, loadedSurface->pixels);

            if (glGetError() != GL_NO_ERROR) {
                printf("OpenGL error during texture upload for: %s\n", desc.filePath.c_str());
                glDeleteTextures(1, &GLtexture);
                SDL_FreeSurface(loadedSurface);
                return nullptr;
            }

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            asset::Texture::SurfacePtr surface(loadedSurface, SDL_FreeSurface);
            glBindTexture(GL_TEXTURE_2D, 0);
            return std::make_unique<asset::Texture>(GLtexture, std::move(surface));
        }
    }
}
