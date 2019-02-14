#include <iostream>
#include <algorithm>
#include <string>

static const int ALPHABET_SIZE = 255;

struct Trie
{
	struct Node
	{
		Node *children[ALPHABET_SIZE] = {nullptr};
		bool leaf = false;
	};

	Node root;
	int depth = 0;

	void free(Node *n) {
		for (int c = 0; c < ALPHABET_SIZE; c++) {
			if (n->children[c]) {
				free(n->children[c]);
				delete n->children[c];
			}
		}
	}

	static char index(char c) {
		return c;
	}

	void insert(const char *str, Node *& n, int currentDepth = 1) {
		if (!n) {
			n = new Node;
		}
		if (!*str) {
			depth = std::max(currentDepth, depth);
			n->leaf = true;
		} else {
			insert(str + 1, n->children[index(*str)]);
		}
	}

	void writeSorted(std::ostream &stream, std::string &stack, Node *n) {
		if (!n) {
			return;
		}
		if (n->leaf) {
			stream << stack << '\n';
		}
		for (int c = 0; c < ALPHABET_SIZE; c++) {
			if (n->children[c]) {
				stack.push_back(char(c));
				writeSorted(stream, stack, n->children[c]);
				stack.pop_back();
			}
		}
	}

public:
	Trie() {}
	Trie(const Trie &) = delete;
	Trie &operator=(const Trie &) = delete;

	~Trie() {
		free(&root);
	}

	void insert(const char *str) {
		if (*str) {
			insert(str + 1, root.children[index(*str)]);
		}
	}

	void writeSorted(std::ostream &stream) {
		std::string stack;
		stack.reserve(depth);
		writeSorted(stream, stack, &root);
	}
};


int main() {
	std::string inp;
	Trie t;

	while (std::cin >> inp) {
		t.insert(inp.c_str());
	}
	std::cout << "-----\n";
	t.writeSorted(std::cout);
	getchar();
	return 0;
}