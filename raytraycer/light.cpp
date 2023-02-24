#include "light.h"

std::vector<Light> Light::SampleAreaLight() {
	double u, v;
	std::vector<Light> sampledLights;
	for (int i = 0; i < numOfSamples; ++i) {
		u = (double)rand() / RAND_MAX;
		v = (double)rand() / RAND_MAX;
		if (u + v > 1.0) {
			u = 1 - u;
			v = 1 - v;
		}
		vec3 pl = tri->a + tri->e1 * u + tri->e2 * v;
		sampledLights.push_back(Light(pl + tri->na * eps, light, tri));
	}
	return sampledLights;
}

double Light::CalculateWeight(vec3 dl) {
	if (tri) {
		double num = tri->area * tri->na.dot(-dl.normalize());
		double denum = numOfSamples * (dl.length() * dl.length());
		//printf("%lf from %lf / %lf\n", std::(num / denum), num, denum);
		return std::abs(num/denum);
	}
	return 1.f;
}
