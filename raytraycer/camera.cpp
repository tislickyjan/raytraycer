#include "camera.h"

Ray Camera::GenerateRay(int x, int y) const {
	vec3 pxy = p00 + qw * x - qh * y; // pixel position
	pxy = pxy.normalize(); // normalized
	//std::cout << "Ray shooted from " << position << " with direction " << pxy << std::endl;
	return Ray(position, pxy); // return ray from eye to scene
}

void Camera::InitCam() {
	double gw, gh;
	b = dir.cross(up);
	gw = 2*tan(fov / 2);
	gh = gw * (height / width);
	qw = b * (gw / (width - 1));
	qh = up * (gh / (height - 1));
	p00 = dir - b * (gw / 2) + up * (gh / 2);
	std::cout << "Camera initialised" << std::endl;
}

void Camera::PrintCamInfo() const {
	std::cout << "------------------------------" << std::endl;
	std::cout << "Printing information about the camera" << std::endl;
	std::cout << "Position: " << this->position << std::endl;
	std::cout << "Up vector: " << this->up << std::endl;
	std::cout << "Direction: " << this->dir << std::endl;
	std::cout << "FOV: " << this->fov << std::endl;
	std::cout << "Width: " << this->width << std::endl;
	std::cout << "Height: " << this->height << std::endl;
	std::cout << "------------------------------" << std::endl;
}