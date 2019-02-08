#include <iostream>
#include <utility>

int main() {

	int first, second;
	int readCount = 0;
	int current;

	while (std::cin >> current) {
		
		// first init the two numbers
		if (readCount == 0) {
			first = current;
		} else if (readCount == 1) {
			second = current;
			if (first < second) {
				std::swap(first, second);
			}
		} else {

			if (current > first) {
				// assign and shift first to second
				second = first;
				first = current;
			} else if (current > second) {
				second = current;
			}

		}
		++readCount;
	}

	// if the two numbers are equal or not initialized -> print error
	if (readCount < 2 || first == second) {
		std::cout << "No two distinct max numbers" << std::endl;
	} else {
		std::cout << "Max = " << first << " second = " << second << std::endl;
	}

	return 0;
}
