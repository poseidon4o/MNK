#include <utility>
#include <stack>
#include <cassert>

const char WALL = '#';
const char EMPTY = ' ';
const char END = 'e';

char maze[10][6] {
	"     ",
	" ### ",
	"  #  ",
	" ## #",
	" ##  ",
	"  ## ",
	"# #  ",
	"  # #",
	" ## e",
	"  #  ",
};

const int HEIGHT = sizeof(maze) / sizeof(maze[0]);
const int WIDTH = sizeof(maze[0]) / sizeof(maze[0][0]);

bool findPath(int y, int x) {
	if (y < 0 || y >= HEIGHT || x < 0 || x > WIDTH) {
		return false;
	}

	if (maze[y][x] == WALL) {
		return false;
	}

	if (maze[y][x] == END) {
		return true;
	}

	maze[y][x] = WALL;

	// operator || will short circuit and stop on first true
	const bool checkNext =
		findPath(y, x + 1) ||
		findPath(y + 1, x) ||
		findPath(y, x - 1) ||
		findPath(y - 1, x);

	maze[y][x] = EMPTY;

	return checkNext;
}

// same as above but without recursion
bool findPathStack(int y, int x) {
	struct State {
		int y, x; // current coordinates
		bool isWall = false; // set to true for fake states used to un-mark cells
	};

	std::stack<State> stack;
	// push start
	stack.push({y, x});

	while (!stack.empty()) {
		const State current = stack.top();
		stack.pop();

		if (current.isWall) {
			maze[current.y][current.x] = EMPTY;
			continue;
		}

		if (current.y < 0 || current.y >= HEIGHT || current.x < 0 || current.x >= WIDTH) {
			continue;
		}

		if (maze[current.y][current.x] == WALL) {
			continue;
		}


		if (maze[current.y][current.x] == END) {
			// this return here will return from 1 function and will thus clear the stack in one go
			// in the recursive function it will clear each level one by one
			return true;
		}

		// mark as wall
		maze[current.y][current.x] = WALL;
		// push un-mark before all next states so it will be un-marked after all next states are visited
		stack.push({current.y, current.x, true});

		stack.push({current.y - 1, current.x});
		stack.push({current.y, current.x - 1});
		stack.push({current.y + 1, current.x});
		stack.push({current.y, current.x + 1});
	}

	// END was never reached
	return false;
}


int main() {
	for (int c = 0; c < 5; c++) {
		for (int r = 0; r < 5; r++) {
			const bool rec = findPath(c, r);
			const bool stack = findPathStack(c, r);

			assert(rec == stack);
		}
	}

	puts("done");
	getchar();
	return 0;
}