#include "ray.h"

Ray Ray::ReflectRay()
{
	vec3 n = tri->CalculateNormalFromBary(u, v);
	vec3 refl = (n * 2 * n.dot(-dir) + dir).normalize();
	vec3 position = origin + dir * t;
	return Ray(position + refl * eps, refl);
}

// fresnel effect
// https://blog.demofox.org/2017/01/09/raytracing-reflection-refraction-fresnel-total-internal-reflection-and-beers-law/
// https://www.broxzier.com/raytracer/
Ray Ray::RefractRay(double IOR, double & r){
	vec3 triNorm = tri->CalculateNormalFromBary(u, v);
	vec3 refrDir = vec3(0.0);
	vec3 position = origin + dir * t;
	double dot = (-dir).dot(triNorm);
	double gamma = 0.0;
	double sqrPart = 0.0;
	bool in = false;
	double r0 = 1 / (1 + IOR);

	if (dot > 0.0){ // going inside of an object
		gamma = 1/IOR;
		in = true;
		r0 *= (IOR - 1);
	}
	else { // going outside of an object
		gamma = IOR;
		triNorm = -triNorm; // i have blunt angle and i need to switch normal
		dot = (-dir).dot(triNorm); // recalculate dot product
		in = false;
		r0 *= (1 - IOR);
	}

	r0 *= r0;
	r = r0 + (1-r0)*std::pow((1-dot),5);
	
	sqrPart = 1-(gamma*gamma * (1-dot*dot));
	
	if (sqrPart > 0.f)
		refrDir = triNorm * (gamma * dot - std::sqrt(sqrPart)) + dir * gamma;
	else
		return Ray(vec3(std::numeric_limits<double>::min()),vec3(0.0));
	refrDir = refrDir.normalize();

	return Ray(position + refrDir * eps, refrDir, in);
}

bool Ray::IntersectTri(Triangle & tr) {
	vec3 p = dir.cross(tr.e2); // is ray paralell with triangle
	double d = tr.e1.dot(p);

	// no backface culling, if inside true - not cull, else cull backface
	if (inside){
		if (std::abs(d) < std::numeric_limits<double>::epsilon())
			return false;
	}
	else {
		if (d < std::numeric_limits<double>::epsilon())
			return false;
	}

	double invD = 1 / d;

	vec3 q = origin - tr.a;
	double u = q.dot(p) * invD;
	if (u < 0.0 || u > 1.0)
		return false;

	vec3 r = q.cross(tr.e1);
	double v = r.dot(dir) * invD;
	if (v < 0.0 || u + v > 1.0)
		return false;

	double t = tr.e2.dot(r) * invD;
	// Something was hit, i only want the closest triangle
	if (this->t > t && t > 0) {
		this->t = t;
		this->u = u;
		this->v = v;
		this->tri = &tr;
	}
	else
		return false;

	return true;
}

// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
bool Ray::IntersectBB(const BoundingBox& bb, double& tmin) {
	double tmax, t1, t2, tymin, tymax, tzmin, tzmax;
	t1 = (bb.pMin.x - origin.x) / dir.x;
	t2 = (bb.pMax.x - origin.x) / dir.x;
	tmin = std::min(t1, t2);
	tmax = std::max(t1, t2);
	t1 = (bb.pMin.y - origin.y) / dir.y;
	t2 = (bb.pMax.y - origin.y) / dir.y;
	tymin = std::min(t1, t2);
	tymax = std::max(t1, t2);

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	t1 = (bb.pMin.z - origin.z) / dir.z;
	t2 = (bb.pMax.z - origin.z) / dir.z;
	tzmin = std::min(t1, t2);
	tzmax = std::max(t1, t2);

	if ((tmin > tzmax) || (tzmin > tzmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	return true;
}