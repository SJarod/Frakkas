#pragma once

#include <thread>
#include <atomic>
#include <deque>
#include <mutex>
#include <condition_variable>

#include "utils/singleton.hpp"
#include "multithread/task.hpp"

class ThreadPool : public Singleton<ThreadPool>
{
	friend class Singleton<ThreadPool>;

private:
	std::vector<std::jthread>	threads;
	//which thread is working?
	std::vector<bool>			workers;

	//number of threads
	unsigned int		nThread = 0;
	bool				multithread = false;

	float				startTime = 0.f;
	float				endTime = 0.f;
	std::atomic<float>	lastTaskTime;

	// shared data
	std::deque<Task>	parallelTasks;
	std::deque<Task>	mainThreadTasks;

	std::mutex				parallelQueueMX;
	std::condition_variable parallelQueueCV;
	std::mutex				mainThreadQueueMX;

	//is the routine running?
	std::atomic_flag running = ATOMIC_FLAG_INIT;

	/**
	 * Create the thread pool.
	 * Specify the number of threads, the number of threads is by default the maximum that the machine can get.
	 *
	 * @param nThread
	 */
	ThreadPool(const unsigned int i_nThread = std::thread::hardware_concurrency());

public:
	/**
	 * End the thread pool.
	 */
	~ThreadPool();

	/**
	 * Set the pool to function or not.
	 * true : multithread
	 * false : monothread
	 * @param i_param
	 */
	void SetMultithread(const bool i_param);

	/**
	 * Is the pool functionning in multithread?
	 */
	const bool GetMultithreadParam() const;

	/**
	 * Get the number of threads.
	 */
	unsigned int GetThreadsNumber() const;

	/**
	 * Is the thread pool working?
	 */
	static bool Clear();

	/**
	 * Add a task to the task queue.
	 * @param i_fct : the task
	 * @param i_parallel : should the task be done in a parallel thread?
	 */
	static void AddTask(const Task& i_fct, const bool i_parallel = true);

	/**
	 * Remove the first task from the task queue.
	 */
	void RemoveFirstTask();

	/**
	 * Choose a task then do it.
	 */
	void Work(const int i_id = -1);

	/**
	 * Thread routine.
	 * Main loop function
	 */
	void PoolRoutine(const int i_id);

	/**
	 * Do the tasks contained in the main thread queue.
	 */
	static void PollMainThreadTasks();

	/**
	 * Call this function in the main thread to pause it and finish every remaining tasks.
	 */
	static void FinishTasks();

	/**
	 * Pause this thread for a certain time.
	 * @param i_time in seconds
	 */
	static void Delay(float i_time);

	/**
	 * Print the thread id.
	 */
	void PrintThreadId(const int i_id);

	/**
	 * Get the time to store as the thread pool's starting time.
	 */
	void SetStartTime();

	/**
	 * Return the time between now and the start time.
	 * Equivalent to a loading time.
	 * The user chooses when to show the loading time, do not forget to set the start time.
	 */
	float GetLoadingTime();

	/**
	 * Time when the pool has finished doing every tasks.
	 */
	float PrintLastWorkingTime();

	/**
	 * Print the time when the last task was finished.
	 */
	float PrintLastWorkingTimeOnce();
};