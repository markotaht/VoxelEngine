#pragma once

#include <SDL.h>
#include <glm/vec2.hpp>
class InputHandler
{
public:
	static InputHandler& getInstance() {
		static InputHandler instance;
		return instance;
	}

	float mouseSensitivity = 0.02f;

	glm::vec2 getMousePosition();
	glm::vec2 getMouseMovement() { return mouseMovement; }
	int getHorizontalAxis();
	int getVerticalAxis();

	bool isPressed(SDL_Scancode keycode);
	bool isReleased(SDL_Scancode keycode);

	void update();
	void handleEvent(SDL_Event& e);
private:
	InputHandler() {
		prevMousePosition = getMousePosition();
	};
	~InputHandler() = default;

	InputHandler(const InputHandler&) = delete;
	InputHandler& operator=(const InputHandler&) = delete;

	const Uint8* keystates;
	glm::vec2 prevMousePosition;
	glm::vec2 mouseMovement {0, 0};
};

