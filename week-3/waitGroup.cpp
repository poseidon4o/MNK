#include <mutex>
#include <vector>
#include <random>


/// Class representing thread safe counter with the ability to wait for all tasks to be done
/// Implemented using mutex + cond var
class WaitGroup {
public:
	/// Initialize with number of tasks
	WaitGroup(int tasks)
		: m_remaining(tasks) {}

	WaitGroup(const WaitGroup &) = delete;
	WaitGroup & operator=(const WaitGroup &) = delete;

	/// Mark one task as done (substract 1 from counter)
	void done() {
		bool notify = false;
		{
			std::lock_guard<std::mutex> lock(m_mtx);
			--m_remaining;
			notify = m_remaining <= 0;
		}
		if (notify) {
			m_condVar.notify_all();
		}
	}

	/// Get number of tasks remaining at call time, could be less when function returns
	int remaining() const {
		return m_remaining;
	}

	/// Block until all tasks are done
	void wait() {
		std::unique_lock<std::mutex> lock(m_mtx);
		if (m_remaining > 0) {
			m_condVar.wait(lock, [this]() { return this->m_remaining == 0; });
		}
	}


private:
	int                     m_remaining; ///< number of remaining tasks
	std::mutex              m_mtx;       ///< lock protecting m_remaining
	std::condition_variable m_condVar;   ///< cond var to wait on m_remaining
};

const int tasks = 20;
int results[tasks] = {};


int doWork(int threadIndex) {
	std::mt19937 mt(threadIndex);

	// simulate work taking some time
	float f = 0;
	{
		const int count = mt() % 100000000 + 1000000;
		for (int r = 0; r < count; r++) {
			f += log10(f + r) * pow(f + r, 3.14) * sqrt(f + r);
		}
	}

	return f == 10;
}


void calculateResult1(int threadIndex, WaitGroup *wg) {
	results[threadIndex] += doWork(threadIndex);
	printf("Thread [%d] step 1\n", threadIndex);

	// mark as one done
	wg->done();

	// wait for all
	wg->wait();

	// do step 2
	results[threadIndex] += doWork(threadIndex);
	printf("Thread [%d] step 2\n", threadIndex);
}

int main() {
	std::thread workers[tasks];

	WaitGroup wg(tasks);
	
	for (int c = 0; c < tasks; c++) {
		workers[c] = std::thread(calculateResult1, c, &wg);
	}

	for (int c = 0; c < tasks; c++) {
		workers[c].join();
	}
	

	puts("done.");
	getchar();
	return 0;
}