#pragma once

#include <functional>
#include <thread>
#include <mutex>
#include <deque>

namespace Prism {

	// input to function is thread index
	using Task = std::function<void(uint32_t threadIndex)>;

	struct TaskLock
	{
		TaskLock() : lock(mutex) {}
		TaskLock(const TaskLock&) = delete;
		TaskLock& operator=(const TaskLock&) = delete;

		// if the task is completed
		inline bool finished() { return m_Finished; }
		// wait for the task to complete
		inline void wait() { while (!m_Finished) cv.wait(lock); }
		// notify that the task is completed
		inline void notify() { m_Finished = true; cv.notify_all(); }
	private:
		bool m_Finished = false;
		std::mutex mutex;
		std::condition_variable cv;
		std::unique_lock<std::mutex> lock;
	};

	struct TaskSystem
	{
		static void Init();
		static void Submit(const Task& task, TaskLock* lock = nullptr);
		static void Finish();
	};
}