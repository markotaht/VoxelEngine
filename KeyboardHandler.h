#pragma once
#include <SDL.h>
class KeyboardHandler {
public:

	static KeyboardHandler* instance;
	static KeyboardHandler* getInstance() {
		if (instance == nullptr) {
			instance = new KeyboardHandler();
		}
		return instance;
	}

	void handleKeyboardEvent();
	bool isPressed(SDL_Scancode keycode);
	bool isReleased(SDL_Scancode keycode);

	~KeyboardHandler();
private:
	const Uint8* keystates;
};