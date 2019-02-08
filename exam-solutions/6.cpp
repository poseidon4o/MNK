#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>

const int NUM_COMPONENTS = 3;

void toGrayscale(int *image, int width, int height) {
	const int size = width * height * NUM_COMPONENTS;
	for (int c = 0; c < size; c += 3) {
		const int intensity = image[c] + image[c + 1] + image[c + 2];
		image[c] = image[c + 1] = image[c + 2] = intensity;
	}
}

int main() {
	int width = 600;
	int height = 400;
	const int size = width * height * NUM_COMPONENTS;
	int *image = new int[size];

	srand(time(nullptr));

	for (int c = 0; c < size; c++) {
		image[c] = rand();
	}

	toGrayscale(image, width, height);

	for (int c = 0; c < size; c += 3) {
		assert(image[c] == image[c + 1] && image[c + 1] == image[c + 2] && "Failed to convert to grayscale");
	}

	delete[] image;

	return 0;
}