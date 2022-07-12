#include <iostream>
#include <chrono>

#include "debug/log.hpp"
#include "game/time_manager.hpp"

#include "multithread/threadpool.hpp"

ThreadPool::ThreadPool(const unsigned int i_nThread)
{
	this->nThread = i_nThread;

	running.test_and_set();

	multithread = i_nThread > 0;

	for (unsigned int i = 0; i < i_nThread; ++i)
	{
		threads.emplace_back(std::jthread(std::bind(&ThreadPool::PoolRoutine, this, i)));
		workers.emplace_back(false);
	}
}

ThreadPool::~ThreadPool()
{
	running.clear();
	parallelQueueCV.notify_all();
	for (auto& thread : threads)
		thread.join();
}

void ThreadPool::SetMultithread(const bool i_param)
{
	if (parallelTasks.empty())
		multithread = i_param;
}

const bool ThreadPool::GetMultithreadParam() const
{
	return multithread;
}

unsigned int ThreadPool::GetThreadsNumber() const
{
	return nThread;
}

bool ThreadPool::Clear()
{
	ThreadPool& tp = Instance();

	if (!tp.parallelTasks.empty())
		return false;
	else
	{
		for (bool worker : tp.workers)
		{
			if (worker)
				return false;
		}
	}

	return true;
}

void ThreadPool::AddTask(const Task& i_fct, const bool i_parallel)
{
	ThreadPool& tp = Instance();

	if (i_parallel)
	{
		std::lock_guard<std::mutex> guard(tp.parallelQueueMX);

		if (tp.parallelTasks.empty())
			tp.parallelQueueCV.notify_one();

		tp.parallelTasks.emplace_back(Task(i_fct));
	}
	else
	{
		std::lock_guard<std::mutex> guard(tp.mainThreadQueueMX);
		tp.mainThreadTasks.emplace_back(Task(i_fct));
		return;
	}

	if (!tp.multithread)
		tp.Work();
}

void ThreadPool::RemoveFirstTask()
{
	parallelTasks.pop_front();
}

void ThreadPool::Work(const int i_id)
{
	Task t;

	if (multithread)
	{
		if (parallelTasks.empty())
		{
			std::unique_lock<std::mutex> lock(parallelQueueMX);
			parallelQueueCV.wait(lock, [&]() {
				return running.test() ? !parallelTasks.empty() : true;
				});

			return;
		}
		else
		{
			std::lock_guard<std::mutex> guard(parallelQueueMX);

			if (parallelTasks.empty())
				return;

			t = parallelTasks.front();
			workers[i_id] = true;
			RemoveFirstTask();
		}
	}
	else
	{
		t = parallelTasks.front();
		RemoveFirstTask();
	}

	t(/*userData*/);
	if (multithread)
		workers[i_id] = false;

	auto now = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::steady_clock::now().time_since_epoch()).count());
	lastTaskTime.store(now);
}

void ThreadPool::PoolRoutine(const int i_id)
{
	while (running.test())
	{
		if (!multithread)
			std::this_thread::sleep_for(std::chrono::milliseconds(10));

		Work(i_id);
	}
}

void ThreadPool::PollMainThreadTasks()
{
	ThreadPool& tp = Instance();

	std::lock_guard<std::mutex> guard(tp.mainThreadQueueMX);

	for (Task& task : tp.mainThreadTasks)
		task();

	tp.mainThreadTasks.clear();
}

void ThreadPool::FinishTasks()
{
	while (!Clear())
	{
		// do all main thread tasks while other threads are working
		PollMainThreadTasks();
	}

	// finish remaining main thread tasks
	PollMainThreadTasks();
}

void ThreadPool::Delay(float i_time)
{
	float start = Game::Time::GetTime();
	while (Game::Time::GetTime() - start < i_time)
	{}
}

void ThreadPool::PrintThreadId(const int i_id)
{
	Log::Info("thread #" + std::to_string(i_id));
}

void ThreadPool::SetStartTime()
{
	startTime = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::steady_clock::now().time_since_epoch()).count());
}

float ThreadPool::GetLoadingTime()
{
	endTime = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::steady_clock::now().time_since_epoch()).count());

	return (endTime - startTime) / 1000.f;
}

float ThreadPool::PrintLastWorkingTime()
{
	if (lastTaskTime == 0.f)
		return 0.f;

	return (lastTaskTime - startTime) / 1000.f;
}

float ThreadPool::PrintLastWorkingTimeOnce()
{
	if (lastTaskTime == 0.f)
		return 0.f;

	float val = (lastTaskTime - startTime) / 1000.f;

	Log::Info("LOADING TIME : " + std::to_string(val) + "s");

	lastTaskTime = 0.f;

	return val;
}