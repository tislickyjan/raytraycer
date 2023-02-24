#pragma once
#include <cmath>
#include <iostream>
#include <fstream>

const double epsilon = 1e-9; // Small value
const float eps = .0002f;

struct vec3
{
    vec3() : x(0), y(0), z(0) {}
    vec3(double v) : x(v), y(v), z(v) {}
    vec3(double x0, double y0, double z0 = 0) : x(x0), y(y0), z(z0) {}
    vec3(const float* vals) { x = vals[0]; y = vals[1]; z = vals[2]; };
    vec3 operator*(double a) const { return vec3(x * a, y * a, z * a); }
    vec3 operator*(const vec3 r) const { return vec3(x * r.x, y * r.y, z * r.z); }
    vec3 operator/(const double r) const { return fabs(r) > epsilon ? vec3(x / r, y / r, z / r) : vec3(0, 0, 0); }
    vec3 operator+(const vec3& v) const { return vec3(x + v.x, y + v.y, z + v.z); }
    vec3 operator-(const vec3& v) const { return vec3(x - v.x, y - v.y, z - v.z); }
    vec3 operator-() const { return vec3(-x, -y, -z); }
    void operator+=(const vec3& v) { x += v.x, y += v.y, z += v.z; }
    void operator-=(const vec3& v) { x -= v.x, y -= v.y, z -= v.z; }
    void operator*=(double a) { x *= a, y *= a, z *= a; }
    void operator*=(const vec3& v) { x *= v.x, y *= v.y, z *= v.z; }
    bool operator==(const vec3& v) { return (x == v.x && y == v.y && z == v.z); }
    template<typename T> // yes this is not fully optimal
    double operator[](T idx) const { return idx == 0 ? x : (idx == 1 ? y : z); }
    double length() const { return sqrt(x * x + y * y + z * z); }
    double average() { return (x + y + z) / 3; }
    vec3 normalize() const { return (*this) / length(); }

    double x, y, z;

    void clamp(double min, double max);

    double dot(const vec3& v);

    vec3 cross(const vec3& v);

    friend std::ostream& operator<<(std::ostream& os, vec3 vec);
};
