#include "KeyboardHandler.h"

void KeyboardHandler::handleKeyboardEvent() {
	keystates = SDL_GetKeyboardState(NULL);
}

bool KeyboardHandler::isPressed(SDL_Scancode keycode) {
	return keystates[keycode];
}

bool KeyboardHandler::isReleased(SDL_Scancode keycode) {
	return !keystates[keycode];
}

KeyboardHandler::~KeyboardHandler() {
	keystates = NULL;
}

KeyboardHandler* KeyboardHandler::instance = nullptr;