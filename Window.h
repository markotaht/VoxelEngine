#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>

#include <vector>
#include <memory>

class Window
{
public:

	using WindowPtr = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;

	Window() = default;

	bool init();
	void update();
	~Window();

	SDL_GLContext getThreadContext(int i);
	inline SDL_Window* getWindow() const { return window.get(); }

	static const int SCREEN_WIDTH = 640;
	static const int SCREEN_HEIGHT = 480;
private:
	WindowPtr window = { nullptr, SDL_DestroyWindow };
	SDL_GLContext gContext;

	std::vector<SDL_GLContext> threadContexts;
};
