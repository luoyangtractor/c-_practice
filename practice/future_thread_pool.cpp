#include "future_thread_pool.h"



future_thread_pool::future_thread_pool(int thread_max_num , int queue_max_size, long long max_wait_time): \
			_thread_num(std::thread::hardware_concurrency()>0 ? std::min(thread_max_num, (int)std::thread::hardware_concurrency()): thread_max_num),\
			_queue_size(queue_max_size),\
			_control_flag(false),\
			_empty_flag(true),\
			_max_wait_time(max_wait_time)
{
	_threads_status = new int[_thread_num];
	for (int i = 0; i < _thread_num; i++)
	{
		std::thread* work_thread = new std::thread(&future_thread_pool::work, this);
		//work_thread->detach();
		_thread_array.push_back(work_thread);
		_threads_status[i] = HOLDING;
	}
}

void future_thread_pool::work()
{
	std::unique_lock<std::mutex> _condition_lock(_condition_mutex);
	while (!_control_flag)
	{
		while (_empty_flag)
		{
			_condition.wait(_condition_lock);
		}		
		_task_queue_mutex.lock();
		if (_task_queue.size() > 0)
		{
			auto _task = std::move(_task_queue.front().first);
			time_stamp _insert_time = _task_queue.front().second;
			_task_queue.pop();
			if (_task_queue.size() <= 0)_empty_flag = true;
			_task_queue_mutex.unlock();

			// process task
			time_stamp _tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
			if (_tp.time_since_epoch().count() - _insert_time.time_since_epoch().count() <= _max_wait_time)
			{
				//std::cout<< "wait_time: "<< _tp.time_since_epoch().count() - _insert_time.time_since_epoch().count() << std::endl;
				_task();
			}
			else
			{
				std::cout << "task timeout" << std::endl;
			}
		}
		else
		{
			_empty_flag = true;
			_task_queue_mutex.unlock();

		}
	}
}

std::future<ret_val> future_thread_pool::insert_task(std::function<ret_val()> func)
{
	std::packaged_task<ret_val()> p_task(func);
	std::future<ret_val> ret = p_task.get_future();
	_task_queue_mutex.lock();
	std::cout << "insert a tast  task_queue size: " << _task_queue.size()<< std::endl;
	if (_task_queue.size() < _queue_size)
	{
		time_stamp tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		_task_queue.push(std::move(std::make_pair(std::move(p_task), tp)));
	}
	else
	{

		while (_task_queue.size() >= _queue_size)
		{
			_task_queue.pop();
		}
		time_stamp tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		_task_queue.push(std::move(std::make_pair(std::move(p_task), tp)));
	}
	_task_queue_mutex.unlock();
	_empty_flag = false;
	_condition.notify_one();
	return ret;
}

future_thread_pool::~future_thread_pool()
{
	_control_flag = true;
	if (_threads_status != nullptr)
	{
		delete _threads_status;
		_threads_status = nullptr;
	}
	for (int i = 0; i < _thread_array.size(); i++)
	{
		if (_thread_array[i] != nullptr)
		{
			_thread_array[i]->join();
			delete _thread_array[i];
			_thread_array[i] = nullptr;
		}
	}
}

future_task::future_task()
{

}


future_task::~future_task()
{

}


ret_val future_task::do_sth()
{
	int tmp = 0;
	//test of performance
	for (int i = 0; i < 1; i++)
	{
		tmp++;
	}
	//std::cout <<"do in thread: "<< std::this_thread::get_id()<<std::endl;
	ret_val ret;
	ret.id = std::this_thread::get_id();
	return ret;
}
