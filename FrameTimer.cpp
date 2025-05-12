#include "FrameTimer.h"
#include <glm/glm.hpp>

namespace engine::core {
	void FrameTimer::update()
	{
		Uint64 nowCounter = SDL_GetPerformanceCounter();
		Uint32 nowTick = SDL_GetTicks();

		double frameSeconds = (nowCounter - lastCounter) / static_cast<double>(SDL_GetPerformanceFrequency());
		lastCounter = nowCounter;

		float msDelta = static_cast<float>(nowTick - lastTick);
		lastTick = nowTick;

		if (msDelta <= 0.01f) msDelta = 0.01f;
		float instantaneousFps = 1000.f / msDelta;
		smoothedFps = glm::mix(smoothedFps, instantaneousFps, 0.1f);

		deltaTime = frameSeconds;
		accumulator += deltaTime;
	}
	bool FrameTimer::shouldRunFixedUpdate()
	{
		if (accumulator >= fixedTimeStep) {
			accumulator -= fixedTimeStep;
			return true;
		}
		return false;
	}
}
