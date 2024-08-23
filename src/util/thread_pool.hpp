#pragma once

#include "spin_lock.hpp"

#include <vector>
#include <queue>
#include <thread>
#include <functional>

// 一个线程执行一个Task
class Task
{
public:
	virtual void run() = 0;
};

// 并行For循环的任务
class ParallelForTask : public Task
{
public:
	ParallelForTask(size_t x, size_t y, const std::function<void(size_t, size_t)>& lambda)
		: x(x), y(y), lambda(lambda) {}

	void run() override
	{
		lambda(x, y);
	}

private:
	size_t x, y;
	std::function<void(size_t, size_t)> lambda;
};

class ThreadPool
{
public:
	// 每个线程开始前执行的默认函数
	static void WorkerThread(ThreadPool* master) 
	{
		while (master->alive == 1)
		{
			Task* task = master->getTask();
			if (task != nullptr)
			{
				task->run();
			} 
			else 
			{
				// 当前线程“放弃”执行，让操作系统调度另一线程继续执行
				std::this_thread::yield();
			}
		}
	}

	ThreadPool(size_t thread_count = 0)
	{
		alive = 1;

		// 如果为0, 就赋值成CPU线程数
		if (thread_count == 0)
		{
			thread_count = std::thread::hardware_concurrency();
		}
		for (size_t i = 0; i < thread_count; ++i)
		{
			threads.push_back(std::thread(ThreadPool::WorkerThread, this));
		}
	}

	~ThreadPool()
	{
		wait();
		alive = 0;
		for (auto& thread : threads)
		{
			thread.join();
		}
		threads.clear();	
	}

	// 让Main线程等待所有子线程执行完毕
	void wait() const
	{
		while (!tasks.empty())
		{
			std::this_thread::yield();
		}
	}

	// 并行For循环
	void parallelFor(size_t width, size_t height, const std::function<void(size_t, size_t)>& lambda)
	{
		MyLockGuard guard(spin_lock);
		for (size_t x = 0; x < width; ++x)
		{
			for (size_t y = 0; y < height; ++y)
			{
				tasks.push(new ParallelForTask(x, y, lambda));
			}
		}
	}

	// 为线程池添加任务
	void addTask(Task* task)
	{
		// 保证同时只有一个线程添加任务
		MyLockGuard guard(spin_lock);
		tasks.push(task);
	}

	// 为线程池获取任务
	Task* getTask()
	{
		// 保证同时只有一个线程获取任务
		MyLockGuard guard(spin_lock);
		if (tasks.empty())
		{
			return nullptr;
		}
		Task* task = tasks.front();
		tasks.pop();
		return task;
	}

private:
	std::atomic<int> alive;						// 线程池是否还存在
	std::vector<std::thread> threads;
	std::queue<Task*> tasks;
	SpinLock spin_lock;
};
