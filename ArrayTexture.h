#pragma once
#include "Texture.h"
#include "ResourceManager.h"

#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>

#include <vector>
#include <utility>
#include <string>

class ArrayTexture
{
public:
	ArrayTexture(ResourceManager* resourceManager);

	void init();
	void addTexture(std::string name, std::string path);

	inline GLuint getTexture() { return texture; }
private:
	GLuint texture;
	GLuint currentLayer;
	ResourceManager* resourceManager;

	std::vector<std::pair<std::string, std::string>> textures;
};

