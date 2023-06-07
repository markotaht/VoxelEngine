#include "DebugLog.h"


DebugLog* DebugLog::instance = nullptr;
DebugLog::~DebugLog()
{
	delete textRenderer;
}

void DebugLog::render()
{
	for (int i = 0; i < messages.size(); i++) {
		textRenderer->render(messages[i], 0, 24 * i, 0.5f, glm::vec3(1.f, 0.f, 0.f));
	}
	messages.clear();
}
