#pragma once
#include <fstream>
#include <vector>
#include <thread>

#include "tiny_obj_loader.h"
#include "camera.h"
#include "sphere.h"
#include "ray.h"
#include "triangle.h"
#include "vec3.h"
#include "loader.h"
#include "light.h"
#include "bvh.h"
#include "conf.h"

class RayTracer
{
private:
	// General stuff
	float* finalImg;
	long imgSize;
	Loader l;
	// Scene stuff
	Camera view;
	// Holds all materials
	std::vector<tinyobj::material_t> mats;
	// Holds extracted triangles from tinyobj format
	std::vector<Triangle> triangles;
	// Defines spheres
	std::vector<Sphere> spheres;
	// Lights stuff
	std::vector<Light> lights;
	// BVH tree
	BVH tree;

public:
	//RayTracer() {};
	~RayTracer() { if (finalImg) delete[] finalImg; }
	// Creates final img
	void CreateImg();
	// Syntetize img
	void RayTracingD();
	// Syntetize img with colors
	void RayTracing();
	// Multithread RT
	void RayTracingTH(unsigned start, unsigned end);
	bool Syntetize();
	// Syntetize img with colors and lights
	vec3 TraceRay(Ray & r, int depth);
	// loads scene and other stuff, it just need path without extension
	bool Load(const std::string path);
	// Saves created image in path
	void SaveImg(std::string path);
	// Sets minimum distance
	void SetMinD(double & h, double & d, double & mD);
	// Adds sphere with given coordinates and radius
	void AddSphere(float radius, const vec3 & position);
	void SetCamera(const Camera & cam);
	// Find closes intersection, can be replaced with ds
	bool FindClosesInt(Ray& r);
	// Find closes intersection with ds
	//void FindClosesIntBVH(Ray& r);
	// Evaluate light
	vec3 EvaluateLight(Ray & r, const tinyobj::material_t & mat, Light & light);
	// Build BVH tree
	void BuildAccelStruct();
	// Clamp function
	template <typename T>
	inline T clamp(T val, T low, T high)
	{
		return std::max(std::min(val, high), low);
	}
};

