#pragma once
#include <vector>
#include <fstream>
#include <iostream>

#include "vec3.h"
#include "tiny_obj_loader.h"
#include "camera.h"
#include "triangle.h"
#include "light.h"
#include "bvh.h"

//based on tinyobjloader, loads and parse objects from tinyobjloader
struct Loader
{
	bool LoadScene(const std::string filename, tinyobj::attrib_t& attrib,
		 std::vector<tinyobj::shape_t>& shapes,
		 std::vector<tinyobj::material_t>& materials);
	bool LoadCamera(const std::string filename, Camera & cam);
	bool ParseShapes(const std::vector<tinyobj::shape_t> & shapes, const std::vector<tinyobj::material_t> & mats, const tinyobj::attrib_t & attrib, std::vector<Triangle> & tri, std::vector<Light>& lights, std::vector<Node> & leafs);
	vec3 GetVertex(const tinyobj::attrib_t& attrib, size_t vert_idx);
	vec3 GetNormals(const tinyobj::attrib_t& attrib, size_t norm_idx);
	void PrintLoadedGenInfo(const tinyobj::attrib_t& attrib) const;
	void PrintLoadedShapeInfo(const std::vector<tinyobj::shape_t>& shapes) const;
	void PrintLoadedMatInfo(const std::vector<tinyobj::material_t>& materials) const;
};

