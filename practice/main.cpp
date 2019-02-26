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
	test_future_thread_pool();

	int t;
	std::cin >> t;
	return 0;
}