#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>

#include <vector>

class Window
{
public:
	bool init();
	void update();
	SDL_Renderer* getRenderer();
	~Window();

	SDL_GLContext getThreadContext(int i);
	inline SDL_Window* getWindow() { return window; }

	static const int SCREEN_WIDTH = 640;
	static const int SCREEN_HEIGHT = 480;
private:
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_GLContext gContext;

	std::vector<SDL_GLContext> threadContexts;
};

#pragma once
