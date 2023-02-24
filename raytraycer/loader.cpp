#include "loader.h"

bool Loader::LoadScene(const std::string filename, tinyobj::attrib_t & attrib,
                        std::vector<tinyobj::shape_t> & shapes,
                        std::vector<tinyobj::material_t> & materials) {
   // std::cout << "Loading scene " << filename << std::endl;
    std::string warn, err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(), "./scenes", true);
    std::cout << "Loading finished" << std::endl;
    if (!warn.empty()) {
        std::cout << "Warning: " << warn << std::endl;
        // return false;
    }
    if (!err.empty()) {
        std::cout << "Error: " << err << std::endl;
        return false;
    }

    if (!ret) {
        std::cout << "Failed to load/parse" << std::endl;
        return false;
    }

    //PrintLoadedShapeInfo(shapes);

    return true;
}

bool Loader::LoadCamera(const std::string filename, Camera& cam) {
    std::fstream camFile;
    std::string name;
    double a, b, c;
    camFile.open(filename, std::ios::in);
    if (camFile.is_open()) {
        //std::cout << "Reading file " << filename << std::endl;
        camFile >> name; // read fst line
        std::cout << "Camera associated with " << name << std::endl;
        camFile >> name >> a >> b >> c; // Read position
        cam.position = vec3(a, b, c);
        camFile >> name >> a >> b >> c; // up vector
        cam.up = vec3(a, b, c);
        camFile >> name >> a >> b >> c; // dir vector
        cam.dir = vec3(a, b, c);
        camFile >> name >> a; // FOV
        cam.fov = a;
    }
    else
        std::cout << "Could not read the file " << filename << std::endl;
    camFile.close();
    cam.InitCam();
    return true;
}

vec3 Loader::GetVertex(const tinyobj::attrib_t& attrib, size_t vert_idx) {
    double a = static_cast<const double>(attrib.vertices[3 * vert_idx + 0]);
    double b = static_cast<const double>(attrib.vertices[3 * vert_idx + 1]);
    double c = static_cast<const double>(attrib.vertices[3 * vert_idx + 2]);
    return vec3(a, b, c);
}

vec3 Loader::GetNormals(const tinyobj::attrib_t& attrib, size_t vert_idx) {
    double a = static_cast<const double>(attrib.normals[3 * vert_idx + 0]);
    double b = static_cast<const double>(attrib.normals[3 * vert_idx + 1]);
    double c = static_cast<const double>(attrib.normals[3 * vert_idx + 2]);
    return vec3(a, b, c);
}

bool Loader::ParseShapes(const std::vector<tinyobj::shape_t> & shapes, const std::vector<tinyobj::material_t>& mats, const tinyobj::attrib_t & attrib, std::vector<Triangle> & tri, std::vector<Light>& lights, std::vector<Node> & leafs) {
    if (shapes.empty())
        std::cout << "Nothing to parse" << std::endl;
    Triangle tmp;
    // Iterate through shapes
    for (size_t i = 0; i < shapes.size(); i++) {
        size_t index_offset = 0;
        // Faces iteration
        for (size_t f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++) {
            size_t fnum = shapes[i].mesh.num_face_vertices[f];
            // verticies of triangle, i have only triangles, so this have to suffice
            // save only coordinates of triangles
            tmp.a = GetVertex(attrib, shapes[i].mesh.indices[index_offset + 0].vertex_index);
            tmp.b = GetVertex(attrib, shapes[i].mesh.indices[index_offset + 1].vertex_index);
            tmp.c = GetVertex(attrib, shapes[i].mesh.indices[index_offset + 2].vertex_index);
            tmp.SetE1E2();
            // Normals
            if (attrib.normals.empty())
                tmp.CalculateNormal();
            else {
                tmp.na = GetNormals(attrib, shapes[i].mesh.indices[index_offset + 0].normal_index);
                tmp.nb = GetNormals(attrib, shapes[i].mesh.indices[index_offset + 1].normal_index);
                tmp.nc = GetNormals(attrib, shapes[i].mesh.indices[index_offset + 2].normal_index);
            }
            tmp.matIdx = shapes[i].mesh.material_ids[f];
            tri.push_back(tmp);
            // Light
            if (shapes[i].name.find("light") != std::string::npos)
                lights.push_back(Light(vec3(0.0), vec3(mats[tmp.matIdx].emission), &tri.back()));
            index_offset += fnum;
        }
    }
    for (size_t i = 0; i < tri.size(); ++i)
        leafs.push_back(Node(&tri[i]));
    std::cout << "Parsed " << tri.size() << " triangles" << std::endl;
    return true;
}

void Loader::PrintLoadedGenInfo(const tinyobj::attrib_t& attrib) const {
    std::cout << "# of vertices  : " << (attrib.vertices.size() / 3) << std::endl;
    std::cout << "# of normals   : " << (attrib.normals.size() / 3) << std::endl;
    std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2)
        << std::endl;

    
    for (size_t v = 0; v < attrib.vertices.size() / 3; v++) {
        printf("  v[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
            static_cast<const double>(attrib.vertices[3 * v + 0]),
            static_cast<const double>(attrib.vertices[3 * v + 1]),
            static_cast<const double>(attrib.vertices[3 * v + 2]));
    }

    for (size_t v = 0; v < attrib.normals.size() / 3; v++) {
        printf("  n[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
            static_cast<const double>(attrib.normals[3 * v + 0]),
            static_cast<const double>(attrib.normals[3 * v + 1]),
            static_cast<const double>(attrib.normals[3 * v + 2]));
    }

    for (size_t v = 0; v < attrib.texcoords.size() / 2; v++) {
        printf("  uv[%ld] = (%f, %f)\n", static_cast<long>(v),
            static_cast<const double>(attrib.texcoords[2 * v + 0]),
            static_cast<const double>(attrib.texcoords[2 * v + 1]));
    }      
}

void Loader::PrintLoadedShapeInfo(const std::vector<tinyobj::shape_t>& shapes) const {
    std::cout << "------------------------------------------------------" << std::endl;
    std::cout << "# of shapes    : " << shapes.size() << std::endl;
    int numOfTri = 0;
    // For each shape
    for (size_t i = 0; i < shapes.size(); i++) {
        printf("shape[%ld].name = %s\n", static_cast<long>(i),
            shapes[i].name.c_str());
        printf("Size of shape[%ld].mesh.indices: %lu\n", static_cast<long>(i),
            static_cast<unsigned long>(shapes[i].mesh.indices.size()));
        printf("Size of shape[%ld].lines.indices: %lu\n", static_cast<long>(i),
            static_cast<unsigned long>(shapes[i].lines.indices.size()));
        printf("Size of shape[%ld].points.indices: %lu\n", static_cast<long>(i),
            static_cast<unsigned long>(shapes[i].points.indices.size()));

        size_t index_offset = 0;

        printf("shape[%ld].num_faces: %lu\n", static_cast<long>(i),
            static_cast<unsigned long>(shapes[i].mesh.num_face_vertices.size()));

        // For each face
        for (size_t f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++) {
            size_t fnum = shapes[i].mesh.num_face_vertices[f];

            printf("  face[%ld].fnum = %ld\n", static_cast<long>(f),
                static_cast<unsigned long>(fnum));

            // For each vertex in the face
            for (size_t v = 0; v < fnum; v++) {
                tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + v];
                printf("    face[%ld].v[%ld].idx = %d/%d/%d\n", static_cast<long>(f),
                    static_cast<long>(v), idx.vertex_index, idx.normal_index,
                    idx.texcoord_index);
            }

            printf("  face[%ld].material_id = %d\n", static_cast<long>(f),
                shapes[i].mesh.material_ids[f]);
            printf("  face[%ld].smoothing_group_id = %d\n", static_cast<long>(f),
                shapes[i].mesh.smoothing_group_ids[f]);
            ++numOfTri;
            index_offset += fnum;
        }

        printf("shape[%ld].num_tags: %lu\n", static_cast<long>(i),
            static_cast<unsigned long>(shapes[i].mesh.tags.size()));
        for (size_t t = 0; t < shapes[i].mesh.tags.size(); t++) {
            printf("  tag[%ld] = %s ", static_cast<long>(t),
                shapes[i].mesh.tags[t].name.c_str());
            printf(" ints: [");
            for (size_t j = 0; j < shapes[i].mesh.tags[t].intValues.size(); ++j) {
                printf("%ld", static_cast<long>(shapes[i].mesh.tags[t].intValues[j]));
                if (j < (shapes[i].mesh.tags[t].intValues.size() - 1)) {
                    printf(", ");
                }
            }
            printf("]");

            printf(" floats: [");
            for (size_t j = 0; j < shapes[i].mesh.tags[t].floatValues.size(); ++j) {
                printf("%f", static_cast<const double>(
                    shapes[i].mesh.tags[t].floatValues[j]));
                if (j < (shapes[i].mesh.tags[t].floatValues.size() - 1)) {
                    printf(", ");
                }
            }
            printf("]");

            printf(" strings: [");
            for (size_t j = 0; j < shapes[i].mesh.tags[t].stringValues.size(); ++j) {
                printf("%s", shapes[i].mesh.tags[t].stringValues[j].c_str());
                if (j < (shapes[i].mesh.tags[t].stringValues.size() - 1)) {
                    printf(", ");
                }
            }
            printf("]");
            printf("\n");
        }
    }
    std::cout << "Scene has " << numOfTri << " triangles" << std::endl;
}

void Loader::PrintLoadedMatInfo(const std::vector<tinyobj::material_t>& materials) const {
    std::cout << "------------------------------------------------------" << std::endl;
    std::cout << "# of materials : " << materials.size() << std::endl;

    for (size_t i = 0; i < materials.size(); i++) {
        printf("material[%ld].name = %s\n", static_cast<long>(i),
            materials[i].name.c_str());
        printf("  material.Ka = (%f, %f ,%f)\n",
            static_cast<const double>(materials[i].ambient[0]),
            static_cast<const double>(materials[i].ambient[1]),
            static_cast<const double>(materials[i].ambient[2]));
        printf("  material.Kd = (%f, %f ,%f)\n",
            static_cast<const double>(materials[i].diffuse[0]),
            static_cast<const double>(materials[i].diffuse[1]),
            static_cast<const double>(materials[i].diffuse[2]));
        printf("  material.Ks = (%f, %f ,%f)\n",
            static_cast<const double>(materials[i].specular[0]),
            static_cast<const double>(materials[i].specular[1]),
            static_cast<const double>(materials[i].specular[2]));
        printf("  material.Tr = (%f, %f ,%f)\n",
            static_cast<const double>(materials[i].transmittance[0]),
            static_cast<const double>(materials[i].transmittance[1]),
            static_cast<const double>(materials[i].transmittance[2]));
        printf("  material.Ke = (%f, %f ,%f)\n",
            static_cast<const double>(materials[i].emission[0]),
            static_cast<const double>(materials[i].emission[1]),
            static_cast<const double>(materials[i].emission[2]));
        printf("  material.Ns = %f\n",
            static_cast<const double>(materials[i].shininess));
        printf("  material.Ni = %f\n", static_cast<const double>(materials[i].ior));
        printf("  material.dissolve = %f\n",
            static_cast<const double>(materials[i].dissolve));
        printf("  material.illum = %d\n", materials[i].illum);
        printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
        printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
        printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
        printf("  material.map_Ns = %s\n",
            materials[i].specular_highlight_texname.c_str());
        printf("  material.map_bump = %s\n", materials[i].bump_texname.c_str());
        printf("    bump_multiplier = %f\n", static_cast<const double>(materials[i].bump_texopt.bump_multiplier));
        printf("  material.map_d = %s\n", materials[i].alpha_texname.c_str());
        printf("  material.disp = %s\n", materials[i].displacement_texname.c_str());
        printf("  <<PBR>>\n");
        printf("  material.Pr     = %f\n", static_cast<const double>(materials[i].roughness));
        printf("  material.Pm     = %f\n", static_cast<const double>(materials[i].metallic));
        printf("  material.Ps     = %f\n", static_cast<const double>(materials[i].sheen));
        printf("  material.Pc     = %f\n", static_cast<const double>(materials[i].clearcoat_thickness));
        printf("  material.Pcr    = %f\n", static_cast<const double>(materials[i].clearcoat_thickness));
        printf("  material.aniso  = %f\n", static_cast<const double>(materials[i].anisotropy));
        printf("  material.anisor = %f\n", static_cast<const double>(materials[i].anisotropy_rotation));
        printf("  material.map_Ke = %s\n", materials[i].emissive_texname.c_str());
        printf("  material.map_Pr = %s\n", materials[i].roughness_texname.c_str());
        printf("  material.map_Pm = %s\n", materials[i].metallic_texname.c_str());
        printf("  material.map_Ps = %s\n", materials[i].sheen_texname.c_str());
        printf("  material.norm   = %s\n", materials[i].normal_texname.c_str());
        std::map<std::string, std::string>::const_iterator it(
            materials[i].unknown_parameter.begin());
        std::map<std::string, std::string>::const_iterator itEnd(
            materials[i].unknown_parameter.end());

        for (; it != itEnd; it++) {
            printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
        }
        printf("\n");
    }
}