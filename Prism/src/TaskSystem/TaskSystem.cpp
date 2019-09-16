#include "pch.h"
#include "TaskSystem.h"

namespace Prism {

	uint32_t g_NumThreads;
	std::vector<std::thread> g_Threads;

	bool g_Finished = false;
	std::deque<std::pair<Task, TaskLock*>> g_Tasks;
	std::mutex g_QueueMutex;
	std::condition_variable g_WaitCondition;

	// executed by each thread
	void processTasks(uint32_t index)
	{
		for (;;)
		{
			std::pair<Task, TaskLock*> task;
			{
				std::unique_lock<std::mutex> lock(g_QueueMutex);
				while (g_Tasks.empty() && !g_Finished)
					g_WaitCondition.wait(lock);

				// if no task here, finish was called
				if (g_Tasks.empty()) break;

				// get task
				task = std::move(g_Tasks.front());
				g_Tasks.pop_front();
			}

			// execute task
			{
				task.first(index);
				if (task.second)
					task.second->notify();
			}
		}
	}

	void TaskSystem::Init()
	{
		g_NumThreads = std::thread::hardware_concurrency();
		g_Threads = std::vector<std::thread>(g_NumThreads);
		for (uint32_t i = 0; i < g_NumThreads; ++i)
			g_Threads[i] = std::thread([=]() { processTasks(i); });
	}

	void TaskSystem::Submit(const Task& task, TaskLock* info)
	{
		std::unique_lock<std::mutex> lock(g_QueueMutex);
		g_Tasks.emplace_back(std::move(task), info);
		g_WaitCondition.notify_one();
	}

	void TaskSystem::Finish()
	{
		{
			std::unique_lock<std::mutex> lock(g_QueueMutex);
			g_Finished = true;
		}
		g_WaitCondition.notify_all();
		for (auto& thread : g_Threads)
			thread.join();
	}
}