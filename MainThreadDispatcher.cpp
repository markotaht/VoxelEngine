#include "MainThreadDispatcher.h"

namespace engine::core {
	void MainThreadDispatcher::runMainThreadTasks() {
		std::queue<std::unique_ptr<IJob>> tasks;

		{
			std::lock_guard<std::mutex> lock(mainThreadQueueMutex);
			std::swap(tasks, mainThreadQueue);
		}

		while (!tasks.empty()) {
			tasks.front()->run();
			tasks.pop();
		}
	}
}
