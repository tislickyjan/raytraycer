#pragma once
#include <vector>
#include "vec3.h"
#include "triangle.h"

struct Light
{
	vec3 pos;
	vec3 light;
	Triangle * tri;

	const int numOfSamples = 32; // or 32

	Light(vec3 p, vec3 l) : pos(p), light(l), tri(NULL) {}
	Light(vec3 p, vec3 l, Triangle* t) : pos(p), light(l), tri(t) {}
	std::vector<Light> SampleAreaLight();
	double CalculateWeight(vec3 dl);
};
