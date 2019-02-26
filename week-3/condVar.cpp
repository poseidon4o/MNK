#include <thread>
#include <condition_variable>
#include <mutex>

#include <Windows.h>

const int iterations = 3;
const int thCount = 5;

int progress[thCount] = {0,};
std::mutex mtx;
std::condition_variable event;


bool canProgress(int threadIndex) {
	const int ourProgress = progress[threadIndex];

	for (int c = 0; c < thCount; c++) {
		if (threadIndex != c) {
			if (progress[c] < ourProgress) {
				return false;
			}
		}
	}

	return true;
}

void print(int threadIndex) {
	for (int c = 0; c < iterations; c++) {
		Sleep(rand() % 500);
		std::unique_lock<std::mutex> lock(mtx);
		if (canProgress(threadIndex)) {
			printf("Thread [%d] -> %d\n", threadIndex, c);
		} else {
			event.wait(lock, [threadIndex]() {
				return canProgress(threadIndex);
			});
		}

		lock.unlock();
		event.notify_one();
		
	}
}


int main() {
	std::thread workers[thCount];
	for (int c = 0; c < thCount; c++) {
		workers[c] = std::thread(print, c);
	}

	for (std::thread &th : workers) {
		th.join();
	}

	puts("done");
	getchar();
	return 0;
}
