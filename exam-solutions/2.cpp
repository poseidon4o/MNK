#include <iostream>
#include <cassert>
#include <algorithm>

const float EPS = 0.00001f;

float cubeRoot(float value) {
	// EPS is relative to input to handle large values
	const float relativeEPS = value * EPS;
	float left = EPS;
	float right = value;

	float error = relativeEPS * 2;
	float guess = 0.f;

	while (error >= relativeEPS) {
		// guess the middle of the interval
		guess = (left + right) / 2.f;

		const float cubed = guess * guess * guess;
		// this is actually the cube of the error
		error = fabs(cubed - value);

		// change interval
		if (cubed < value) {
			left = guess;
		} else {
			right = guess;
		}
	}

	return guess;
}

void testCubeRoot() {
	for (float c = 1.f; c < 100000.f; c += 0.1f) {
		const float cubed = c * c * c;
		const float custom = cubeRoot(cubed);
		const float error = fabs(c - custom);
		assert(error <= EPS * c);
	}
}

int main() {
	// testCubeRoot();
	float input = 0.f;
	std::cin >> input;
	std::cout << cubeRoot(input);

	return 0;
}
