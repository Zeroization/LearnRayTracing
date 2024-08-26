#pragma once

#include <atomic>
#include <thread>

class SpinLock
{
public:
	void acquire()
	{
		while (flag.test_and_set(std::memory_order_acquire))
		{
			std::this_thread::yield();
		}
	}

	void release()
	{
		flag.clear(std::memory_order_release);
	}

private:
	std::atomic_flag flag;
};

class MyLockGuard
{
public:
	MyLockGuard(SpinLock& spin_lock) : spin_lock(spin_lock) { spin_lock.acquire(); }
	~MyLockGuard() { spin_lock.release(); }

private:
	SpinLock& spin_lock;
};
