#pragma once

#include <cmath>

#include "vec3.h"
#include "ray.h"


struct Sphere {
    Sphere(float rad_, vec3 p_):
        rad(rad_), p(p_) {}

    double intersect(const Ray &r) const { // returns distance, 0 if nohit
        vec3 op = p - r.origin; // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
        double t, eps=1e-4, b=op.dot(r.dir), det=b*b-op.dot(op)+rad*rad;
        if (det<0) return 0; else det=sqrt(det);
        return (t=b-det)>eps ? t : ((t=b+det)>eps ? t : 0);
    }

    float rad;  // radius
    vec3 p;     // position
}; 