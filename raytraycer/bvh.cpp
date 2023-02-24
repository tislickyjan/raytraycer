#include "bvh.h"

void BVH::Build() {
	printf("--------BVH INFO---------\n");
	printf("Starting BVH build\n");
	if (root) {
		DeleteNode(root);
		delete root;
	}
	root = RecursiveBuild((size_t)0, leafs.size(), 0);
	printf("Build complete\n");
	//printf("%li inner nodes\n", innerNodeNumber);
	printf("-------------------------\n");
}

Node* BVH::RecursiveBuild(size_t start, size_t end, short axis) {
	Node* node;
	// only one triangle, so make a leaf
	if (start == end - 1) {
		return &leafs[start];
	} // two triangles, make node and two leafs
	else if (start == end - 2) {
		node = new InnerNode();
		node->bb.Include(leafs[start].bb);
		node->bb.Include(leafs[start + 1].bb);
		node->axis = node->bb.MaxExtent();
		if (leafs[start].bb.centroid[node->axis] < leafs[start + 1].bb.centroid[node->axis]) {
			node->l = &leafs[start];
			((InnerNode*)node)->r = &leafs[start + 1];
		}
		else {
			node->l = &leafs[start+1];
			((InnerNode*)node)->r = &leafs[start];
		}
	}
	else { // more then two
		BoundingBox nbb = BoundingBox();
		BoundingBox cbb = BoundingBox();
		// Find divide point, center of whole bb, construct bb of current node
		for (size_t i = start; i < end; ++i) {
			nbb.Include(leafs[i].bb);
			cbb.Include(leafs[i].bb.centroid);
		}
		vec3 dividePoint = cbb.centroid;
		//axis = cbb.MaxExtent();
		size_t fstR = start;
		// main part from lecture, split on axis AXIS
		SplitByPoint(start, end, axis, dividePoint[axis], fstR);
		// all point on right or left, correction with all brute force i can do, sadly
		if (fstR == start || fstR == end)
			FindAnySplit(start, end, axis, fstR);
		// fstR is start for right part (fstR, end) and end for left part (start, fstR)
		node = new InnerNode();
		node->axis = axis;
		node->bb = nbb;
		node->l = RecursiveBuild(start, fstR, (axis+1)%3);
		((InnerNode*)node)->r = RecursiveBuild(fstR, end, (axis+1)%3);
	}
	return node;
}

void BVH::SplitByPoint(size_t start, size_t end, short axis, double point, size_t & fstR) {
	for (size_t i = start; i < end; ++i) {
		// triangle is on the left
		if (leafs[i].bb.centroid[axis] < point) {
			std::swap(leafs[i], leafs[fstR]);
			++fstR;
		}
	}
}

void BVH::FindAnySplit(size_t start, size_t end, short & axis, size_t& fstR) {
	vec3 dividePoint;
	for (short a = 0; a < 3; a++) {
		for (size_t i = start; i < end; i++) {
			dividePoint = leafs[i].bb.centroid;
			fstR = start;
			SplitByPoint(start, end, axis, dividePoint[axis], fstR);
			if (fstR != start && fstR != end)
				return;
		}
		axis = (axis + 1) % 3;
	}
	printf("We need a bigger gun!!!\n");
}

bool BVH::Traverse(Ray& r, Node * n) {
	double tmin = std::numeric_limits<double>::max();
	// intersection of ray with bbox of the node n
	if (r.IntersectBB(n->bb, tmin)) {
		// ray hitted something further then current node
		if (r.t > tmin) {
			// current node is a leaf
			if (n->axis == 3) {
				return r.IntersectTri(*n->t);
			}
			// current node is internal node
			else if (r.dir[n->axis] > 0) {
				Traverse(r, n->l);
				Traverse(r, ((InnerNode*)n)->r);
			} else {
				Traverse(r, ((InnerNode*)n)->r);
				Traverse(r, n->l);
			}
		}
	}
	return r.tri ? true : false;
}

void BVH::DeleteNode(Node * n) {
	if (n->l->axis != 3) {
		DeleteNode(n->l);
		delete n->l;
	}
	if (((InnerNode*)n)->r->axis != 3) {
		DeleteNode(((InnerNode*)n)->r);
		delete ((InnerNode*)n)->r;
	}
}
