#include "thread_pool.h"

class task;
class thread_pool;

int main()
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
		std::cout << e.what()<<std::endl;
	}
	int t;
	std::cin >> t;
	return 0;
}