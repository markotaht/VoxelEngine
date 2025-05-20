#include "Texture2DArrayLoader.h"
#include "Texture2DArrayDescriptor.h"
#include "Texture2DArray.h"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <SDL.h>
#include <string>
#include <SDL_image.h>
#include <SDL_surface.h>

#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>

#include "AutoLoaderRegistrar.h"
#include "ResourceManager.h"

SDL_Surface* flipSurfaceVertically(SDL_Surface* surface) {
    SDL_Surface* flipped = SDL_CreateRGBSurfaceWithFormat(
        0,
        surface->w,
        surface->h,
        surface->format->BitsPerPixel,
        surface->format->format
    );

    if (!flipped) return nullptr;

    int pitch = surface->pitch;
    uint8_t* srcPixels = static_cast<uint8_t*>(surface->pixels);
    uint8_t* dstPixels = static_cast<uint8_t*>(flipped->pixels);

    for (int y = 0; y < surface->h; ++y) {
        memcpy(
            dstPixels + y * pitch,
            srcPixels + (surface->h - 1 - y) * pitch,
            pitch
        );
    }

    return flipped;
}

namespace engine::loader {

    static AutoLoaderRegistrar<engine::loader::Texture2DArrayLoader, IResourceLoader<descriptor::Texture2DArrayDescriptor, asset::Texture2DArray>> regTex([](resource::ResourceManager&) {
        return std::make_unique<engine::loader::Texture2DArrayLoader>();
        });

    bool Texture2DArrayLoader::canLoad(const descriptor::Texture2DArrayDescriptor& desc) const
    {
        return true;
    }
    std::unique_ptr<asset::Texture2DArray> Texture2DArrayLoader::load(const descriptor::Texture2DArrayDescriptor& desc) const
    {
        std::vector<asset::TextureLayer> textureLayers = std::move(loadImages(desc));
        if (textureLayers.empty()) {
            std::cerr << "Failed to load any textures for Texture2DArray\n";
            return nullptr;
        }

        return std::make_unique<asset::Texture2DArray>(std::move(textureLayers), desc.width, desc.height);
    }

    bool Texture2DArrayLoader::uploadGPU(asset::Texture2DArray& tex) const
    {
        if (tex.textureId == 0) {
            glGenTextures(1, &tex.textureId);
        }
        glBindTexture(GL_TEXTURE_2D_ARRAY, tex.textureId);

        int width = tex.width;
        int height = tex.height;
        int layers = tex.layers.size();

        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, layers, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        for (size_t i = 0; i < tex.layers.size(); ++i) {
            auto& surfaceLayer = tex.layers[i];
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
                0,             // mip level
                0, 0, i,       // x, y, z offset
                width, height, 1,  // width, height, depth (1 layer)
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                surfaceLayer.surface->pixels);
        }

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        return true;
    }

    std::vector<asset::TextureLayer> Texture2DArrayLoader::loadImages(const descriptor::Texture2DArrayDescriptor& desc) const
    {
        std::vector<asset::TextureLayer> layers;
        uint32_t index = 0;
        for (auto& [id, fileDescriptor] : desc.textures) {
            SDL_Surface* loadedSurface = IMG_Load(fileDescriptor.filePath.c_str());
            if (loadedSurface == NULL) {
                printf("Unable to load image %s! SDL_image Error: %s\n", fileDescriptor.filePath.c_str(), IMG_GetError());
                return {};
            }
            SDL_Surface* converted = flipSurfaceVertically(SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_ABGR8888, 0));
            SDL_FreeSurface(loadedSurface);

            if (!converted) {
                std::cerr << "Failed to convert surface format.\n";
                return {};
            }

            layers.emplace_back(asset::TextureLayer{asset::SurfacePtr(converted, SDL_FreeSurface), id, fileDescriptor.filePath});
        }
        return layers;
    }
}
