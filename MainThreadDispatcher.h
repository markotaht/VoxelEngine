#pragma once
#include <queue>
#include <mutex>
#include <functional>

namespace engine::core {
	class MainThreadDispatcher {
	public:
		template<typename Func>
		void enqueueMainThread(Func&& job) {
			std::lock_guard<std::mutex> lock(mainThreadQueueMutex);
			mainThreadQueue.emplace(std::make_unique<JobImpl<Func>>(std::forward<Func>(job)));
		}

		void runMainThreadTasks();
	private:
		struct IJob {
			virtual void run() = 0;
			virtual ~IJob() = default;
		};

		template<typename Func>
		struct JobImpl : IJob {
			Func fn;
			JobImpl(Func&& f) : fn(std::move(f)) {}
			void run() override { fn(); }
		};
		std::mutex mainThreadQueueMutex;
		std::queue<std::unique_ptr<IJob>> mainThreadQueue;
	};
}