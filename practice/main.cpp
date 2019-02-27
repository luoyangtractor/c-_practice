#include "thread_pool.h"
#include "future_thread_pool.h"

class task;
class thread_pool;

class future_task;
class future_thread_pool;


void test_thread_pool()
{
	thread_pool pool;
	try
	{
		for (int i = 0; i < 1000; i++)
		{
			task t;
			pool.insert_task(t);
		}
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
}


void test_future_thread_pool() 
{
	future_thread_pool pool;
	try
	{
		for (int i = 0; i < 1000; i++)
		{
			int tmp = 0;
			for (int j = 0; j < 3000; j++)
			{
				tmp++;
			}
			future_task t;
			std::function<ret_val()> func = std::bind(&future_task::do_sth, &t);
			std::future<ret_val> ret = pool.insert_task(func);
			//std::cout <<"after get "<< ret.get().id << std::endl << std::endl;
		}
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
}



int main()
{
	time_stamp start = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	
	try
	{
		for (int i = 0; i < 10000; i++)
		{
			int tmp = 0;
			for (int j = 0; j < 3; j++)
			{
				tmp++;
			}
			future_task t;
			t.do_sth();
			//std::cout <<"after get "<< ret.get().id << std::endl << std::endl;
		}
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
	
	time_stamp end = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	std::cout << end.time_since_epoch().count() - start.time_since_epoch().count() << "ms" << std::endl;
	int t;
	//std::cin >> t;

	future_thread_pool pool;
	
	try
	{
		for (int i = 0; i < 10000; i++)
		{
			int tmp = 0;
			for (int j = 0; j < 3; j++)
			{
				tmp++;
			}
			future_task t;
			std::function<ret_val()> func = std::bind(&future_task::do_sth, &t);
			std::future<ret_val> ret = pool.insert_task(func);
			//std::cout <<"after get "<< ret.get().id << std::endl << std::endl;
		}
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
	 end = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());

	//int t;
	std::cin >> t;
	return 0;
}