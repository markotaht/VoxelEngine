#pragma once
#include <cstdint>

namespace engine::core {
	class FrameClock {
	public:
		void nextFrame() {
			++currentFrame;
		}

		uint64_t getFrame() const{
			return currentFrame;
		}
	private:
		uint64_t currentFrame = 0;
	};
}