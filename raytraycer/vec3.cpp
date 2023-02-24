#include "vec3.h"

double vec3::dot(const vec3& v)
{
    return (x * v.x + y * v.y + z * v.z);
}

vec3 vec3::cross(const vec3& v)
{
    return vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

std::ostream& operator<<(std::ostream& os, vec3 vec)
{
    os << vec.x << " " << vec.y << " " << vec.z;
    return os;
}

void vec3::clamp(double min, double max) {
    x = std::max(min, std::min(x, max));
    y = std::max(min, std::min(y, max));
    z = std::max(min, std::min(z, max));
}