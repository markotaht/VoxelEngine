#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>

#include "Resource.h"

class Texture : public Resource
{
public:
	Texture(GLuint index, int width, int heigh);
	Texture();
	~Texture();

	bool loadFromFile(const char* path);
	bool loadFromFile(const char* path, const char* path2);
	bool loadToGlArray(const char* path, int i, GLuint texture);

	void setArrayTextureData(GLuint layer, GLuint texture) {
		this->layer = layer;
		this->GLtexture = texture;
	}

	void free();
	void freeSurface();

	inline int getWidth() {
		return width;
	};
	inline int getHeight() {
		return height;
	};
	inline SDL_Surface* getPixels() {
		return loadedTexture;
	}

	GLuint getGLindx() { return GLtexture; }
	GLuint getLayer() { return layer; }

private:
	int width;
	int height;
	GLuint layer;

	GLuint GLtexture;
	SDL_Surface* loadedTexture;
};

