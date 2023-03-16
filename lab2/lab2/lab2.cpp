#include <iostream>
#include <condition_variable>
#include <queue>

template<class T>
class Queue
{
public:
	void Push(T value) 
	{
		std::lock_guard<std::mutex> lq(mutex);
		queue.push(std::move(value));
		
		cv.notify_one();
	}

	T Pop() 
	{
		std::unique_lock<std::mutex> ul(mutex);
		
		cv.wait(ul, [&]() {return !queue.empty(); });

		T value = std::move(queue.front());
		queue.pop();

		return value;
	}

private:
	std::queue<T> queue;
	std::mutex mutex;
	std::condition_variable cv;
};

int main() 
{
	Queue<int> queueFrom;
	Queue<std::pair<std::thread::id, int>> queueTo;

	for (int i = 0; i < 10; i++)
	{
		queueFrom.Push(i);
	}

	std::vector<std::thread> threads;
	for (int i = 0; i < 2; i++)
	{
		threads.push_back(std::thread([&queueFrom, &queueTo]()
			{
				while (true)
				{
					queueTo.Push(
						{
							std::this_thread::get_id(),
							queueFrom.Pop()
						});
				}
			}));
	}

	int i = 0;
	for (;;i++) {
		auto pair = queueTo.Pop();
		std::cout << "Thread ID: " << pair.first << " " << " #"
			      << i << " element: " << pair.second << std::endl;
	}

	return 0;
}