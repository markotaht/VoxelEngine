#include "ImageLoader.h"

Texture* ImageLoader::loadResource(const char* path)
{
	GLuint GLtexture = 0;
	SDL_Surface* loadedSurface = IMG_Load(path);
	Texture* texture = nullptr;
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
	}
	else {
		glGenTextures(1, &GLtexture);
		glBindTexture(GL_TEXTURE_2D, GLtexture);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, loadedSurface->w, loadedSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, loadedSurface->pixels);
		if (glGetError() != GL_NO_ERROR) {
			printf("Error");
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		texture = new Texture(GLtexture, loadedSurface->w, loadedSurface->h);
		SDL_FreeSurface(loadedSurface);
	}

	return texture;
}
