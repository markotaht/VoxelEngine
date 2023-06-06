#include "InputHandler.h"
#include <SDL.h>
glm::vec2 InputHandler::getMousePosition()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	return glm::vec2(x - 320.f, y - 240.f);
}
