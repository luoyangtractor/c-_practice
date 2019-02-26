#pragma once
#include <future>
#include <thread>
#include <mutex>
#include <queue>
#include <vector>
#include <atomic>
#include <algorithm>
#include <iostream>
#include <condition_variable>
#include <functional>

#define RUNNING 0
#define HOLDING 1

class ret_val
{
public:
	std::thread::id id;
};

class future_task
{
public:
	future_task();
	virtual ~future_task();
	ret_val do_sth();

private:
	std::future<ret_val> _return_value;
};

class future_thread_pool
{
public:
	future_thread_pool(int = 4, int = 20);
	virtual ~future_thread_pool();

	std::future<ret_val> insert_task(std::function<ret_val()>);

private:
	std::mutex _task_queue_mutex;
	std::mutex _condition_mutex;
	std::queue<std::packaged_task<ret_val()>> _task_queue;

	int _queue_size;
	int _thread_num;

	std::vector<std::thread*> _thread_array;
	std::atomic<int *> _threads_status;
	std::atomic<bool> _control_flag;
	std::atomic<bool> _empty_flag;

	std::condition_variable _condition;


	void work();

};

