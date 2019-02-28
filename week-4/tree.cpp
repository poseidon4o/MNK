
#include <cassert>
#include <utility>
#include <tuple>


/// Recursive implementation of non-balanced binary search tree
/// Does not allow duplicate values and the value type must provide operator<
/// @tparam T - the type of the contained elements, must provide operator<, copy ctor, default ctor, operator=
template <typename T>
class BSTree {
	struct Node {
		Node * left = nullptr;
		Node * right = nullptr;
		T data;
	};

	Node * root;
	int count;

	/// Find a Node in the tree for the given value, node can be not yet created
	/// @param n - ref to Node pointer, root of subtree to search
	/// @param value - the value to search for
	/// @return - reference to the Node, could be nullptr
	Node *& find(Node *& n, const T & value) {
		// if the value is not in the tree - return where would it be if it was
		// if we find the value - return the node
		if (!n || n->data == value) {
			return n;
		}

		// recurse into chosen subtree
		if (value < n->value) {
			return find(n->left, value);
		} else {
			return find(n->right, value);
		}
	}

	/// Find the Node containing the min value in a given subtree, return the reference to the pointer to the min Node
	/// Can return nullptr, if argument is nullptr
	/// @param n - reference to the the subtree to search in
	/// @return - reference to the pointer pointing to the minNode, can be equal to @n
	Node *& getMin(Node *& n) {
		// if n does not have left child, then n is ref to pointer pointing to 
		// Node that is the last left child (min element) in this subtree
		if (!n->left) {
			return n;
		}
		return getMin(n->left);
	}

	/// Copy a source tree into the destination
	/// @param dest - reference to the destination node
	/// @param source - the source tree
	void copy(Node *& dest, const Node * source) {
		if (!source) {
			dest = nullptr;
			return;
		}

		dest = new Node();
		dest->data = source->data;

		copy(dest->left, source->left);
		copy(dest->right, source->right);
	}

	/// De-allocate all nodes in the given subtree
	/// @param n - the root of the subtree, can be nullptr
	void clear(Node *& n) {
		if (!n) {
			return;
		}
		clear(n->left);
		clear(n->right);
		delete n;
		n = nullptr;
	}

public:
	BSTree() : root(nullptr), count(0)
	{}

	~BSTree() {
		clear(root);
	}

	BSTree(const BSTree & other)
		: root(nullptr), count(0)
	{
		copy(root, other.root);
	}

	BSTree & operator=(const BSTree &other) {
		if (this != &other) {
			clear(root);
			count = other.count;
			copy(root, other.root);
		}
		return *this;
	}

	BSTree(BSTree && other)
		: root(nullptr), count(0) {
		swap(other);
	}

	BSTree & operator=(BSTree && other) {
		swap(other);
		return *this;
	}

	/// Swap with another BSTree
	/// @param other - the target of the swap
	void swap(BSTree & other) {
		std::swap(root, other.root);
		std::swap(count, other.count);
	}

	/// Get number of elements in the tree
	/// @return - the number of elements in the tree
	int size() const {
		return count;
	}

	/// Check if size is 0
	/// @return - true if no elements, false otherwise
	bool empty() const {
		return size() == 0;
	}

	/// Try to insert the value if not yet inserted
	/// @param value - the value to insert
	/// @return - true if inserted, false otherwise
	bool insert(const T & value) {
		Node *& n = find(root, value);
		if (!n) {
			++count;
			n = new Node();
			n->data = value;
			return true;
		}
		return false;
	}

	/// Check if the given value is in the tree
	/// @param value - the value to search for
	/// @return - true if the value is in the tree, false otherwise
	bool find(const T & value) {
		return find(root, value) != nullptr;
	}

	/// Try to remove the given value
	/// @param value - the value to remove
	/// @return - true if the value was found and removed, false otherwise
	bool remove(const T & value) {
		Node *& n = find(root, value);
		if (!n) {
			return false;
		}

		bool nullN = false;
		// fine to copy as we want to tell delete where is the memory we want to free
		Node * toDelete = n;
		if (!n->left && !n->right) {
			// n needs to be set to nullptr after this since it has no children
			nullN = true;
		} else if (!n->left) {
			n = n->right;
		} else if (!n->right) {
			n = n->left;
		} else {
			// both n->left and n->right are not nullptr
			Node *& minNode = getMin(n->right);
			// lets assume T is expensive to copy - so we swap n with the minNode and not their values
			Node * minNodePtr = minNode;
			assert(minNode->left == nullptr && "Minimum node must not have left child");
			// pull right subtree one level up
			minNode = minNode->right;
			// assign original children of toDelete to the minNode
			minNodePtr->left = n->left;
			minNodePtr->right = n->right;
			// assign the ref
			n = minNode;
			nullN = false;
		}

		delete toDelete;
		n = nullN ? nullptr : n;

		return true;
	}
};
