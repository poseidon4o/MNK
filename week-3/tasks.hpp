#include <thread>
#include <queue>
#include <vector>
#include <atomic>
#include <cassert>
#include <condition_variable>


struct Task {
	virtual void run() = 0;
	virtual ~Task() = default;
};



struct TaskRunner {

	TaskRunner() = default;
	TaskRunner(const TaskRunner &) = delete;
	TaskRunner & operator=(const TaskRunner &) = delete;

	void start(int count) {
		assert(count > 0);
		isRunning = true;
		for (int c = 0; c < count; c++) {
			threads.emplace_back(&TaskRunner::threadBase, this);
		}
	}

	void addTask(Task *t) {
		assert(t && "Null-ptr task");
		{
			std::lock_guard<std::mutex> lock(taskMtx);
			tasks.push(t);
		}

		taskEvent.notify_one();
	}

	void stop() {
		{
			std::lock_guard<std::mutex> lock(taskMtx);
			if (!isRunning) {
				return;
			}
			isRunning = false;
		}

		taskEvent.notify_all();

		for (std::thread &th : threads) {
			assert(th.joinable() && "Already stopped");
			th.join();
		}

		threads.clear();
	}

	void waitDone() {
		if (tasks.empty()) {
			return;
		}

		std::unique_lock<std::mutex> lock(taskMtx);
		if (tasks.empty()) {
			return;
		}

		doneEvent.wait(lock, [this]() {
			return tasks.empty();
		});
	}


	~TaskRunner() {
		for (std::thread &th : threads) {
			assert(!th.joinable() && "Call stop before ~TaskRunner");
		}
	}
	
	
private:
	void threadBase() {
		while (true) {

			if (!isRunning) {
				// no need for double check pattern
				break;
			}

			Task *current = nullptr;
			if (!tasks.empty()) {
				std::lock_guard<std::mutex> lock(taskMtx);
				if (tasks.empty()) {
					// other thread stole the task
					continue;
				}
				current = tasks.front();
				tasks.pop();
			} else {
				std::unique_lock<std::mutex> lock(taskMtx);

				if (!isRunning) {
					break;
				}

				if (tasks.empty()) {
					taskEvent.wait(lock, [this]() {
						return !isRunning || !tasks.empty();
					});
				}

				if (!isRunning) {
					break;
				}

				current = tasks.front();
				tasks.pop();
			}

			assert(current && "Null-ptr task");

			current->run();
			doneEvent.notify_all();
		}
	}


	std::condition_variable doneEvent;

	bool isRunning;
	std::condition_variable taskEvent;

	std::mutex taskMtx;
	std::queue<Task*> tasks;
	
	std::vector<std::thread> threads;
};