#include "thread_pool.h"

#include <exception>

void task::run()
{
	long long test =  0;
	std::cout << std::this_thread::get_id() << std::endl;
	for (int i = 0; i < 10000; i++)
	{
		for (int j = 0; j < 10000; j++)
		{
			test += j;
		}
		test -= i;
	}
	
}

thread_pool::thread_pool(int num)
{
	if (num <= 0 || num > MAX_THREAD_NUM)
	{
		std::cout << "invalid thread num"<<std::endl;
		throw "invalid thread num";
	}
	thread_num = num;
	thread_status = new int[thread_num];
	for (int i = 0; i < thread_num; i++)
	{
		thread_status[i] = READY;
		std::thread* new_thread = new std::thread(&thread_pool::work, this, i);
		

		pool.push_back(new_thread);
	}
	std::cout << thread_num << " thread pool succeded" << std::endl;
}

void thread_pool::work(int thread_num)
{
	//!!!!!!!!!!!!!!!!!!!!!!!! wrong
	//std::unique_lock <std::mutex> lock(m1);
	while (1)
	{
		m2.lock();
		if(!task_queue.empty())
		{
			auto task = task_queue.front();
			task_queue.pop();
			m2.unlock();

			thread_status[thread_num] = RUNNING;
			task.run();
			task.finished = TASK_FINISHED;
			thread_status[thread_num] = READY;
		}
		else
		{
			m2.unlock();
			thread_status[thread_num] = BLOCKING;
			//cond_var.wait(lock);
			thread_status[thread_num] = READY;
		}
	}
}


bool thread_pool::insert_task(task t)
{
	std::unique_lock <std::mutex> lck(m1);
	m2.lock();
	//set t.task_id
	t.finished = TASK_UNFINISHED;
	task_queue.push(t);
	m2.unlock();
	cond_var.notify_all();
	return true;
}


thread_pool::~thread_pool()
{
	delete[] thread_status;
	for (int i = 0; i < thread_num; i++)
	{
		delete pool[i];
		pool[i]->join();
	}
}
