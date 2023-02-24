#pragma once
#include <vector>

#include "ray.h"
#include "BoundingBox.h"
#include "triangle.h"

struct Node {
	BoundingBox bb;
	//    0 - x, 1 - y, 2 - z, 3 - leaf
	short axis;
	union {
		Node* l;
		Triangle* t;
	};

	Node(Triangle* tri) {
		axis = 3;
		t = tri;
		bb = BoundingBox(tri->a, tri->b, tri->c);
	}
	Node() : l(NULL) {};
};

struct InnerNode : Node {
	Node* r;

	InnerNode() : r(NULL) {};
};

class BVH
{
public:
	BVH() : root(NULL){};
	// Need to delete whole hierarchy
	~BVH() {
		if (root) {
			DeleteNode(root);
			delete root;
		}
	}
	Node* root;
	std::vector<Node> leafs;
	//---------------------
	// Build with sliding midpoint
	void Build(); // array, od do, atd...
	Node* RecursiveBuild(size_t start, size_t end, short axis);
	void SplitByPoint(size_t start, size_t end, short axis, double point, size_t & fstR);
	void FindAnySplit(size_t start, size_t end, short & axis, size_t& fstR);
	bool Traverse(Ray& r, Node * n);
	void DeleteNode(Node * n);
};

