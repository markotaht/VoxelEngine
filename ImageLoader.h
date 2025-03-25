#pragma once
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#include <memory>
#include "Texture.h"
#include "Loader.h"
class ImageLoader : public Loader<Texture>
{
public:
	std::unique_ptr<Texture> loadResource(const char* path);
};

