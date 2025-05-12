#pragma once
#include <SDL.h>

namespace engine::core {
	class FrameTimer {
	public:
		explicit FrameTimer(double fixedStep = 1.0 / 60.0)
			: fixedTimeStep(fixedStep),
			lastCounter(SDL_GetPerformanceCounter()),
			lastTick(SDL_GetTicks()),
			smoothedFps(60.0f),
			accumulator(0.0),
			deltaTime(0.0)
		{}

		void update();

		bool shouldRunFixedUpdate();

		double getDeltaTime() const { return deltaTime; }
		float getSmoothedFPS() const { return smoothedFps; }
		double getFixedTimeStep() const { return fixedTimeStep; }
	private:
		const double fixedTimeStep;
		Uint64 lastCounter;
		Uint32 lastTick;
		double deltaTime;
		float smoothedFps;
		double accumulator;
	};
}