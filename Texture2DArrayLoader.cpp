#include "Texture2DArrayLoader.h"
#include "Texture2DArrayDescriptor.h"
#include "Texture2DArray.h"

#include <iostream>
#include <vector>
#include <memory>
#include <SDL.h>
#include <string>
#include <SDL_image.h>
#include <SDL_surface.h>

#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>

namespace engine::loader {
    bool Texture2DArrayLoader::canLoad(const descriptor::Texture2DArrayDescriptor& desc) const
    {
        return true;
    }
    std::unique_ptr<asset::Texture2DArray> Texture2DArrayLoader::load(const descriptor::Texture2DArrayDescriptor& desc) const
    {
        std::vector < std::pair<std::string, asset::Texture2DArray::SurfacePtr>> textureMap = std::move(loadImages(desc));
        if (textureMap.empty()) {
            std::cerr << "Failed to load any textures for Texture2DArray\n";
            return nullptr;
        }

        GLuint textureArray;
        glGenTextures(1, &textureArray);
        glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);

        int width = desc.width;
        int height = desc.height;
        int layers = textureMap.size();

        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, layers, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        int index = 0;
        for (auto& [id, surfacePtr] : textureMap) {
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
                0,             // mip level
                0, 0, index,       // x, y, z offset
                width, height, 1,  // width, height, depth (1 layer)
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                surfacePtr->pixels);
        }

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

        return std::make_unique<asset::Texture2DArray>(textureArray, std::move(textureMap));
    }

    std::vector < std::pair<std::string, asset::Texture2DArray::SurfacePtr>> Texture2DArrayLoader::loadImages(const descriptor::Texture2DArrayDescriptor& desc) const
    {
        std::vector < std::pair<std::string, asset::Texture2DArray::SurfacePtr>> textureVector;
        for (auto& [id, fileDescriptor] : desc.textures) {
            SDL_Surface* loadedSurface = IMG_Load(fileDescriptor.filePath.c_str());
            if (loadedSurface == NULL) {
                printf("Unable to load image %s! SDL_image Error: %s\n", fileDescriptor.filePath.c_str(), IMG_GetError());
                return {};
            }
            SDL_Surface* converted = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_ABGR8888, 0);
            SDL_FreeSurface(loadedSurface);

            if (!converted) {
                std::cerr << "Failed to convert surface format.\n";
                return {};
            }

            textureVector.emplace_back(id, std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)>(converted, SDL_FreeSurface));
        }
        return textureVector;
    }
}
