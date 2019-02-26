#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <mutex>
#include <atomic>

#include <Windows.h>

static LARGE_INTEGER freq;

/// Return timestamp with microsecond precision
static uint64_t timer_msec() {
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);

	return (1000000LL * t.QuadPart) / freq.QuadPart;
}

/// Utility class to make time measurements
struct Timer {
	uint64_t startTime = 0;

	void start() {
		startTime = timer_msec();
	}

	uint64_t elapsedMicro() const {
		return timer_msec() - startTime;
	}

	float elapsedMS() const {
		return elapsedMicro() / 1000.0;
	}
};

int getPerWorkers(int size, int workers) {
	return ((size - 1) / workers) + 1;
}

/// The actual function doing work
/// doing more work here will show the bigger difference between single and multi-threaded code
int doWork(int input) {
	return sqrt(input) * log10(input) * pow(input, 3.14);
	return input;
}


/// Non threaded code for base case
/// Returns result over ptr argument to make passing to std::thread easier
void sumSequential(const int *arr, int size, int64_t *result) {
	for (int c = 0; c < size; c++) {
		*result += doWork(arr[c]);
	}
}

/// Wrapper for convenience
int64_t callSequential(const int *arr, int size) {
	int64_t result = 0;
	sumSequential(arr, size, &result);
	return result;
}



/// Very naive implementation of threaded summing locking on each write to the result variable
void sumSequentialLock(const int *arr, int size, int64_t *result, std::mutex *mutex) {
	for (int c = 0; c < size; c++) {
		const int64_t thisItem = doWork(arr[c]);

		std::lock_guard<std::mutex> lock(*mutex);
		*result += thisItem;
	}
}

int64_t sumThreadsLock(const int *arr, int size, int threadCount) {
	std::vector<std::thread> workers(threadCount);
	int64_t result = 0;
	std::mutex mtx;

	const int perWorker = getPerWorkers(size, threadCount);
	for (int c = 0; c < workers.size(); c++) {
		const int *start = arr + c * perWorker;
		const int itemCount = std::min<int>(perWorker, arr + size - start);
		workers[c] = std::thread(sumSequentialLock, start, itemCount, &result, &mtx);
	}

	for (int c = 0; c < workers.size(); c++) {
		workers[c].join();
	}

	return result;
}



/// Run the threads and use atomic result to accumulate the sum
void sumSequentialAtomic(const int *arr, int size, std::atomic_int64_t *result) {
	for (int c = 0; c < size; c++) {
		*result += doWork(arr[c]);
	}
}

int64_t sumThreadsAtomic(const int *arr, int size, int threadCount) {
	std::vector<std::thread> workers(threadCount);
	std::atomic_int64_t result = 0;

	const int perWorker = getPerWorkers(size, threadCount);
	for (int c = 0; c < workers.size(); c++) {
		const int *start = arr + c * perWorker;
		const int itemCount = std::min<int>(perWorker, arr + size - start);
		workers[c] = std::thread(sumSequentialAtomic, start, itemCount, &result);
	}

	for (int c = 0; c < workers.size(); c++) {
		workers[c].join();
	}

	return result;
}



/// Good approach - each worker accumulates data in own variable and then main thread merges the result
int64_t sumThreads(const int *arr, int size, int threadCount) {
	std::vector<std::thread> workers(threadCount);
	std::vector<int64_t> results(threadCount, 0);

	const int perWorker = getPerWorkers(size, threadCount);
	for (int c = 0; c < workers.size(); c++) {
		const int *start = arr + c * perWorker;
		const int itemCount = std::min<int>(perWorker, arr + size - start);
		workers[c] = std::thread(sumSequential, start, itemCount, &results[c]);
	}

	int64_t result = 0;
	for (int c = 0; c < workers.size(); c++) {
		workers[c].join();
		result += results[c];
	}
	return result;
}



int64_t test(int testSize) {
	// do 5 passes and get average of all runs
	const int tries = 5;
	const int MB = 1024 * 1024;
	const int count = testSize * MB;
	std::vector<int> data(count);
	for (int c = 0; c < count; c++) {
		data[c] = c;
	}
	Timer t;

	int64_t total = 0;
	int64_t sequentialResult = 0;
	t.start();
	for (int c = 0; c < tries; c++) {
		sequentialResult = callSequential(&data[0], count);
		total += sequentialResult;
	}
	const float seqTime = t.elapsedMS() / tries;
	printf("sequential count [%d MB] for [%f ms] \n", testSize, seqTime);

	const int threadCount[] = {2, 4, 8, 16, 32, 64, 128};

	for (int thCount : threadCount) {
		typedef int64_t (*ThreadedFunction)(const int *, int, int);
		const int variants = 3;

		ThreadedFunction threadedFunctions[variants] = {sumThreads, sumThreadsAtomic, sumThreadsLock};
		const char *names[variants] = {"sumThreads", "sumThreadsAtomic", "sumThreadsLock"};

		for (int r = 0; r < variants; r++) {
			t.start();
			for (int c = 0; c < tries; c++) {
				const int64_t runResult = threadedFunctions[r](&data[0], count, thCount);
				if (sequentialResult != runResult) {
					printf("function [%s] produced wrong result\n", names[r]);
					exit(-1);
				}
				total += runResult;
			}
			const float parallelTime = t.elapsedMS() / tries;

			printf("parallel count [%d MB] for [%f ms] threads (%s) [%d]\n", testSize, parallelTime, names[r], thCount);
		}
		puts("--------------------------------------------------");
		
	}

	return total;
}

int main() {
	// init the frequency used to convert ticks to microseconds
	QueryPerformanceFrequency(&freq);

	int64_t sum = 0;

	// sample sizes in MB to test with
	const int testSizes[] = {1, 2, 5, 10, 100, 500};

	for (int size : testSizes) {
		sum += test(size);
		printf("\n\n");
	}

	puts("done, return to exit");
	getchar();
	return sum == 0;
}