#include "InputHandler.h"
#include <SDL.h>

#include <iostream>
glm::vec2 InputHandler::getMousePosition()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	return glm::vec2(x - 320.f, y - 240.f);
}

int InputHandler::getHorizontalAxis()
{
	if (isPressed(SDL_SCANCODE_A)) {
		return -1;
	}
	else if (isPressed(SDL_SCANCODE_D)) {
		return 1;
	}
	else {
		return 0;
	}
}

int InputHandler::getVerticalAxis()
{
	if (isPressed(SDL_SCANCODE_W)) {
		return -1;
	}
	else if (isPressed(SDL_SCANCODE_S)) {
		return 1;
	}
	else {
		return 0;
	}
}

bool InputHandler::isPressed(SDL_Scancode keycode) {
	return keystates[keycode];
}

bool InputHandler::isReleased(SDL_Scancode keycode) {
	return !keystates[keycode];
}

void InputHandler::handleEvent(SDL_Event& e)
{
	if (e.type == SDL_MOUSEMOTION && (e.motion.xrel != 0 || e.motion.yrel != 0)) {
		mouseMovement = { e.motion.xrel, e.motion.yrel };
	}
}

void InputHandler::update()
{
	keystates = SDL_GetKeyboardState(nullptr);
	mouseMovement = { 0,0 };
}
