#pragma once
#include "BoundingBox.h"

//struct BoundingBox;

struct Triangle {
	// verticies of triangle
	vec3 a, b, c;
	// normal indicies of a triangle
	vec3 na, nb, nc;
	// material index
	int matIdx;
	// u,v for intersection and normal calc
	vec3 e1, e2;
	// AABB
	//BoundingBox box;
	// triangle area
	double area;

	Triangle(vec3 ia, vec3 ib, vec3 ic, vec3 ina, vec3 inb, vec3 inc, int imatIdx) : a(ia), b(ib), c(ic), na(ina), nb(inb), nc(inc), matIdx(imatIdx) { 
		SetE1E2(); 
		//box = BoundingBox(a, b, c); 
	}
	Triangle(vec3 ia, vec3 ib, vec3 ic, int imatIdx) : a(ia), b(ib), c(ic), matIdx(imatIdx) {
		SetE1E2();
		//box = BoundingBox(a, b, c); 
	}
	Triangle() {};
	void CalculateNormal();
	void SetE1E2();
	vec3 CalculateNormalFromBary(double u, double v);
	void TriangleSurface();
};

