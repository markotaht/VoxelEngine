#pragma once

#include "TextRenderer.h"
#include <vector>
#include <glm/vec3.hpp>

class DebugLog
{
public:
	static DebugLog* instance;
	static DebugLog* getInstance(TextRenderer* textRenderer) {
		if (instance == nullptr) {
			instance = new DebugLog(textRenderer);
		}
		return instance;
	}

	DebugLog(TextRenderer* textRenderer) :textRenderer{ textRenderer } {};
	~DebugLog();
	void render();

	inline void addMessage(std::string message) {
		messages.push_back(message);
	}

private:
	std::vector<std::string> messages;
	TextRenderer* textRenderer;
};
