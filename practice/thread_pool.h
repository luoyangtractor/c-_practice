#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <vector>

#define MAX_THREAD_NUM 20

#define RUNNING 0
#define BLOCKING 1
#define READY 2

#define TASK_FINISHED 0
#define TASK_UNFINISHED 0

class task
{
public:

	int task_id = 0;
	int finished = TASK_UNFINISHED;
	virtual void run();
private:
};


class thread_pool
{
public:
	thread_pool(int num = std::thread::hardware_concurrency());
	virtual ~thread_pool();

	bool insert_task(task);
private:
	void work(int);

	std::mutex m1;
	std::mutex m2;
	std::condition_variable cond_var;
	int thread_num;
	std::atomic<int*> thread_status;
	std::vector<std::thread *> pool;
	std::queue<task> task_queue;

	std::unique_lock<std::mutex> _condition_lock;
};

