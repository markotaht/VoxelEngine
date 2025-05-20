#include "TextureLoader.h"

#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>

#include <memory>
#include <string>

#include <SDL_image.h>
#include <SDL_surface.h>

#include "Texture2D.h"
#include "StringUtils.h"

#include "AutoLoaderRegistrar.h"
#include "ResourceManager.h"

namespace engine {
    namespace loader {

        static AutoLoaderRegistrar<engine::loader::TextureLoader, IResourceLoader<descriptor::FileDescriptor, asset::Texture2D>> regTex([](resource::ResourceManager&) {
            return std::make_unique<engine::loader::TextureLoader>();
         });

        bool TextureLoader::canLoad(const descriptor::FileDescriptor& desc) const
        {
            return ends_with(desc.filePath, ".png") || ends_with(desc.filePath, ".jpg");
        }

        std::unique_ptr<asset::Texture2D> TextureLoader::load(const descriptor::FileDescriptor& desc) const
        {
            SDL_Surface* loadedSurface = IMG_Load(desc.filePath.c_str());
            if (loadedSurface == NULL) {
                printf("Unable to load image %s! SDL_image Error: %s\n", desc.filePath.c_str(), IMG_GetError());
                return nullptr;
            }
            return std::make_unique<asset::Texture2D>(asset::Texture2D::SurfacePtr(loadedSurface, SDL_FreeSurface), desc.filePath);
        }

        bool TextureLoader::uploadGPU(asset::Texture2D& tex) const {
            if (!tex.loadedTexture) {
                printf("No surface data to upload.\n");
                return false;
            }

            if (tex.textureID == 0) {
                glGenTextures(1, &tex.textureID);
            }
 
            glBindTexture(GL_TEXTURE_2D, tex.textureID);

            GLint internalFormat = GL_RGB8;
            GLenum format = GL_RGB;

            if (tex.loadedTexture->format->BytesPerPixel == 4) {
                internalFormat = GL_RGBA8;
                format = GL_RGBA;
            }

            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat,
                tex.loadedTexture->w, tex.loadedTexture->h, 0,
                format, GL_UNSIGNED_BYTE, tex.loadedTexture->pixels);

            if (glGetError() != GL_NO_ERROR) {
                printf("OpenGL error during texture upload for: %s\n", tex.sourcePath.c_str());
                glDeleteTextures(1, &tex.textureID);
                tex.textureID = 0;
                return false;
            }

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glBindTexture(GL_TEXTURE_2D, 0);
            return true;
        }
    }
}
