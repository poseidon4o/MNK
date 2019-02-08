#include <iostream>
#include <cstdint>

void printSubSets(int *arr, int size) {
	// all subsets of a set are 2^size
	const uint64_t total = 1ull << size;

	for (uint64_t counter = 0; counter < total; counter++) {
		// print braces so empty set is not just empty line
		std::cout << '{';
		for (int r = 0; r < size; r++) {
			// counting from 0 to the number of items
			// the values of counter will have all possible combinations of 1's and 0's with length size
			// for each value we print only the items of the set corresponding to the 1's in the counter
			if ((1ull << r) & counter) {
				std::cout << arr[r] << ',';
			}
		}
		std::cout << '}' << std::endl;
	}
}

int main() {
	const int setSize = 5;
	int numbers[setSize];
	for (int c = 0; c < setSize; c++) {
		numbers[c] = c;
	}

	printSubSets(numbers, setSize);

	return 0;
}