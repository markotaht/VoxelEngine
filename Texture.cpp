#include "Texture.h"

Texture::Texture(GLuint index, int width, int heigh) :GLtexture(index),
width(width), height(height)
{
}

Texture::Texture() {
	width = 0;
	height = 0;
}

Texture::~Texture() {
	free();
}

bool Texture::loadFromFile(const char* path) {
	free();

	SDL_Surface* loadedSurface = IMG_Load(path);
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
		SDL_FreeSurface(loadedSurface);
	}
	return true;
}

bool Texture::loadFromFile(const char* path, const char* path2)
{
	return false;
}

void Texture::free()
{
	glDeleteTextures(1, &GLtexture);
}

int Texture::getWidth()
{
	return width;
}

int Texture::getHeight()
{
	return height;
}