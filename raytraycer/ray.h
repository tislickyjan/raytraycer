#pragma once
#include <limits>
#include "vec3.h"
#include "triangle.h"
#include "BoundingBox.h"

struct Ray {
	vec3 origin;
	vec3 dir;
	double t, u ,v;
	Triangle* tri;
	bool inside;

	Ray() :tri(NULL) { origin = vec3(0.0); dir = vec3(0.0); t = std::numeric_limits<double>::max(); inside = false; }
	Ray(vec3 o, vec3 d) : origin(o), dir(d), tri(NULL), inside(false) { t = std::numeric_limits<double>::max(); }
	Ray(vec3 o, vec3 d, bool in) : origin(o), dir(d), tri(NULL), inside(in) { t = std::numeric_limits<double>::max(); }
	Ray ReflectRay();
	Ray RefractRay(double IOR, double & r);
	bool IntersectTri(Triangle & t);
	bool IntersectBB(const BoundingBox & bb, double & tmin);
};