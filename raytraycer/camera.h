#pragma once
#include "vec3.h"
#include "ray.h"

struct Camera
{
	vec3 position;
	vec3 up;
	vec3 dir;
	double fov;
	int width, height;

	vec3 p00;
	vec3 qw;
	vec3 qh;

	vec3 b;
	//---------------------

	Camera() : position(vec3(0.0)), up(vec3(0.0,1.0,0.0)), dir(vec3(0.0)), fov(0.0), width(800), height(800) {}
	Camera(vec3 pos, vec3 up, vec3 dir, double fov, int w = 800, int h = 800) :
		position(pos), up(up), dir(dir), fov(fov), width(w), height(h)
	{
		InitCam();
	}

	Ray GenerateRay(int x, int y) const;
	void InitCam();
	void PrintCamInfo() const;
};

