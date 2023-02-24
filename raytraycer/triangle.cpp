#include "triangle.h"

vec3 Triangle::CalculateNormalFromBary(double u, double v) {
	return (na * (1 - u - v) + nb * u + nc * v).normalize();
}

//https://math.stackexchange.com/questions/516219/finding-out-the-area-of-a-triangle-if-the-coordinates-of-the-three-vertices-are
//https://www.quora.com/How-can-I-find-the-area-of-a-triangle-in-3D-coordinate-geometry
void Triangle::TriangleSurface() {
	area = (e1.cross(e2)).length() * 0.5;
}

void Triangle::SetE1E2() {
	e1 = b - a;
	e2 = c - a;
	TriangleSurface();
}

void Triangle::CalculateNormal() {
	na = nb = nc = (e1.cross(e2)).normalize();
}