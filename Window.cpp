#include "Window.h"

bool Window::init() {
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		//Create window
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gContext = SDL_GL_CreateContext(window);
			if (gContext == NULL)
			{
				printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize GLEW
				glewExperimental = GL_TRUE;
				GLenum glewError = glewInit();
				if (glewError != GLEW_OK)
				{
					printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
				}

				//Use Vsync
				if (SDL_GL_SetSwapInterval(1) < 0)
				{
					printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
				}


				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL image could not be initilaized! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//Initialize OpenGL
			/*	if (!initGL())
				{
					printf("Unable to initialize OpenGL!\n");
					success = false;
				}*/


				/*
							renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
							if (renderer == NULL) {
								printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
								success = false;
							}
							else {
								int imgFlags = IMG_INIT_PNG;
								if (!(IMG_Init(imgFlags) & imgFlags)) {
									printf("SDL image could not be initilaized! SDL_image Error: %s\n", IMG_GetError());
									success = false;
								}*/
			}
		}
	}
	return success;
}

void Window::update() {
	SDL_GL_SwapWindow(window);
	//SDL_UpdateWindowSurface(window);
}

SDL_Renderer* Window::getRenderer() {
	return renderer;
}

Window::~Window() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;
}