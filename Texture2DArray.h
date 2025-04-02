#pragma once
#include "ITexture.h"

#include <SDL.h>
#include <gl/glew.h>
#include <memory>
#include <vector>

namespace engine::asset {
	class Texture2DArray : public ITexture {
	public:
		using SurfacePtr = std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)>;

		Texture2DArray(GLuint textureId, std::vector<std::pair<std::string, SurfacePtr>>&& surfaces): textureId(textureId), surfaces(std::move(surfaces)){}

		void bind(GLuint slot) const override;

		GLuint getID() const override { return textureId; }

		void unload();

		~Texture2DArray() {
			unload();
		}

	private:
		GLuint textureId;
		std::vector<std::pair<std::string, SurfacePtr>> surfaces;
	};
}