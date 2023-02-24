#pragma once
#include <limits>
#include "vec3.h"

struct BoundingBox
{
	vec3 pMin;
	vec3 pMax;
	vec3 centroid;

	BoundingBox() { pMin = vec3(std::numeric_limits<double>::max()); pMax = vec3(std::numeric_limits<double>::min()); centroid = vec3(0.0); };
	BoundingBox(const vec3 & a, const vec3 & b, const vec3 & c) { 
		double minX = std::min(std::min(a.x,b.x),c.x);
		double minY = std::min(std::min(a.y,b.y),c.y);
		double minZ = std::min(std::min(a.z,b.z),c.z);
		double maxX = std::max(std::max(a.x,b.x),c.x);
		double maxY = std::max(std::max(a.y,b.y),c.y);
		double maxZ = std::max(std::max(a.z,b.z),c.z);
		pMin = vec3(minX, minY, minZ);
		pMax = vec3(maxX, maxY, maxZ);
		centroid = (pMin + pMax) * 0.5;
	};
	// void Initialize(const Triangle t);
	void Include(BoundingBox other);
	void Include(vec3 other);
	short MaxExtent();
};

