#include <iostream>
#include <string>

std::string getFirstAndCount(std::string input, int &count) {

	std::string prev;
	// reserve so resize will not re-allocate memory
	prev.reserve(input.size());

	// odd number of digits means there is no previous element
	while (input.size() % 2 == 0) {
		
		for (int c = 0; c < input.size(); c += 2) {
			const int repeat = input[c] - '0';
			const char toRepeat = input[c + 1];
			prev.resize(prev.size() + repeat, toRepeat);
		}

		// re-use input
		std::swap(prev, input);
		prev.clear();
		++count;
	}

	return input;
}

int main() {
	std::string input;
	std::cin >> input;

	int count = 1; // counting is 1 based
	const std::string &first = getFirstAndCount(input, count);

	std::cout << first << ' ' << count;

	return 0;
}