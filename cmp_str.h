#pragma once
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#include <map>
struct cmp_str
{
	bool operator()(GLchar const* a, GLchar const* b) const
	{
		return std::strcmp(a, b) < 0;
	}
};