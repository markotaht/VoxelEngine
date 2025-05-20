#include "Texture2DArray.h"

namespace engine::asset {
	void Texture2DArray::bind(GLuint slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);   // selects which texture unit
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureId);
	}
	void Texture2DArray::unload()
	{
		if (textureId != 0) {
			glDeleteTextures(1, &textureId);
			textureId = 0;
		}
		layers.clear();
	}

	size_t Texture2DArray::estimateMemoryUsage() const {
		if (layers.size() == 0) return 0;

		size_t total = 0;
		for (const auto& layer : layers) {
			total += layer.surface->pitch * layer.surface->h;
		}

		return total;
	}

	size_t Texture2DArray::estimateGpuMemoryUsage() const {
		if (layers.size() == 0) return 0;

		size_t total = 0;
		for (const auto& layer : layers) {
			const SDL_PixelFormat* fmt = layer.surface->format;
			int bytesPerPixel = fmt->BytesPerPixel;  // SDL handles padding/packing
			total += layer.surface->w * layer.surface->h * bytesPerPixel;
		}

		return total;
	}
}
