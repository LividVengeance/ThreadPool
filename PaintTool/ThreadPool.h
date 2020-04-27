#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <vector>
#include <thread>
#include <atomic>
#include <functional>
#include <condition_variable>

#include "WorkQueue.h"
#include "Task.h"

class ThreadPool
{
public:
	~ThreadPool();

	// Singleton Methods
	static ThreadPool& GetInstance();
	static void DestroyInstance();

	void Initialize();
	void Submit(CTask _iworkItem);
	void DoWork();
	void Start();
	void Stop();
	std::atomic_int& getItemsProcessed();

private:
	// Make the threadpool a singleton
	ThreadPool();
	ThreadPool(unsigned int _size);

	//The ThreadPool is non-copyable.
	ThreadPool(const ThreadPool& _kr) = delete;
	ThreadPool& operator= (const ThreadPool& _kr) = delete;

protected:
	static ThreadPool* s_pThreadPool;

private:
	//An atomic boolean variable to stop all threads in the threadpool.
	std::atomic_bool m_bStop{ false };

	//A WorkQueue of tasks which are functors
	CWorkQueue<CTask >* m_pWorkQueue;

	//Create a pool of worker threads
	std::vector<std::thread> m_workerThreads;

	//A variable to hold the number of threads we want in the pool
	unsigned int m_iNumberOfThreads;

	//An atomic variable to keep track of how many items have been processed.
	std::atomic_int m_aiItemsProcessed{ 0 };

};
#endif
