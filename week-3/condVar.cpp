#include <thread>
#include <condition_variable>
#include <mutex>
#include <random>

#include <Windows.h>

const int iterations = 3;
const int thCount = 10;

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
	std::mt19937 mt(threadIndex);

	for (int c = 0; c < iterations; c++) {

		// simulate work taking some time
		float f = 0;
		{
			const int count = mt() % 100000000 + 1000000;
			for (int r = 0; r < count; r++) {
				f += log10(f + r) * pow(f + r, 3.14) * sqrt(f + r);
			}
		}

		std::unique_lock<std::mutex> lock(mtx);
		if (!canProgress(threadIndex)) {
			event.wait(lock, [threadIndex]() {
				return canProgress(threadIndex);
			});
		}

		// report and log progress
		printf("Thread [%f] [%d] -> %d\n", f, threadIndex, progress[threadIndex]);
		progress[threadIndex]++;

		lock.unlock();
		// notify_one or notify_all
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
