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
		surfaces.clear();
	}
}
