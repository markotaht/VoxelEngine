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

	void free();

	int getWidth();
	int getHeight();

	GLuint getGLindx() { return GLtexture; }

private:
	int width;
	int height;

	GLuint GLtexture;
};

