/*
3. Реализовать функцию для барьерной 
синхронизации k потоков (условные переменные).
*/

#include <iostream>
#include <functional>
#include <mutex>
#include <thread>
#include <condition_variable>

class Barrier {
public:
	Barrier(int size, std::function<void()> callback)
	{
		this->size = size;
		current = 0;
		this->callback = callback;
	}

	void PushThread()
	{
		std::unique_lock<std::mutex> lock(mutex);
		current++;

		if (current == size)
		{
			callback();
			current = 0;
			cv.notify_all();
		}
		else
		{
			cv.wait(lock);
		}
	}

private:
	int size;
	int current;

	std::function<void()> callback;
	std::mutex mutex;
	std::condition_variable cv;
};

int main() {
	const int size = 3;
	int time[size] = { 4000, 2000, 7000 };

	Barrier barrier(size, [] {
		std::cout << "barrier passed!" << std::endl;
		});

	std::thread threadArr[size];
	for (int i = 0; i < size; i++) {
		threadArr[i] = std::thread([ms = time[i], &barrier]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(ms));
			std::cout << std::this_thread::get_id() << " passed time" << std::endl;
			barrier.PushThread();
		});
	}
	for (int i = 0; i < size; i++) {
		threadArr[i].join();
	}
}