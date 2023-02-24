#include "BoundingBox.h"

void BoundingBox::Include(BoundingBox other){
	pMin = vec3(std::min(pMin.x, other.pMin.x),
				std::min(pMin.y, other.pMin.y),
				std::min(pMin.z, other.pMin.z));
	pMax = vec3(std::max(pMax.x, other.pMax.x),
				std::max(pMax.y, other.pMax.y),
				std::max(pMax.z, other.pMax.z));
	centroid = (pMin + pMax) * 0.5;
}

void BoundingBox::Include(vec3 other) {
	pMin = vec3(std::min(pMin.x, other.x),
				std::min(pMin.y, other.y),
				std::min(pMin.z, other.z));
	pMax = vec3(std::max(pMax.x, other.x),
				std::max(pMax.y, other.y),
				std::max(pMax.z, other.z));
	centroid = (pMin + pMax) * 0.5;
}

short BoundingBox::MaxExtent() {
	vec3 d = pMax - pMin;
	if (d.x > d.y && d.x > d.z)
		return 0;
	else if (d.y > d.z)
		return 1;
	else
		return 2;
}
