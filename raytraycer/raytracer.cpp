#include "raytracer.h"

void RayTracer::SetMinD(double& h, double& d, double& mD) {
    if (h > 0.f && h < std::numeric_limits<float>::max()) {
        d = std::min(h, d);
        mD = std::max(mD, d);
        //cout << "For coordinates: " << i << ", " << j << " is l:" << l << endl;
    }
}

void RayTracer::RayTracingD() {
    double maxD = 0; // probably wrong, but how to make it better ??
    // go through pixels
    for (int i = 0; i < view.width; ++i) {
        for (int j = 0; j < view.height; ++j) {
            double d = std::numeric_limits<float>::max(); // inf for closest distance
            Ray ray = view.GenerateRay(i, j);
            // intersect objects
            for (const Sphere& sp : spheres) {
                double hit = (float)sp.intersect(ray);
                SetMinD(hit, d, maxD);
            }
            for (Triangle& t : triangles) {
                ray.IntersectTri(t);
                SetMinD(ray.t, d, maxD);
            }
            // save color
            //float color = 255.f - (d / maxD) * 255;
            finalImg[3 * (j * view.width + i) + 0] = (float)d; // R
            finalImg[3 * (j * view.width + i) + 1] = (float)d; // G
            finalImg[3 * (j * view.width + i) + 2] = (float)d; // B
        }
    }
    maxD *= 1.35f;
    std::cout << "RT finished, with depth " << maxD << std::endl;
    // Post process data
    for (int i = 0; i < view.width * view.width * 3; ++i) {
        float color = (float)(1 - (clamp((double)finalImg[i],0.0,maxD) / maxD)) * 255.f;
        finalImg[i] = color;
    }
}

void RayTracer::RayTracing() {
    // go through pixels
    for (int i = 0; i < view.width; ++i) {
        for (int j = 0; j < view.height; ++j) {;
            Ray ray = view.GenerateRay(i, j);
            vec3 color = TraceRay(ray, 8) * 255.f;
            finalImg[3 * (j * view.width + i) + 0] = (float)color.x;
            finalImg[3 * (j * view.width + i) + 1] = (float)color.y;
            finalImg[3 * (j * view.width + i) + 2] = (float)color.z;
        }
        if (i % 80 == 0 && i != 0)
            printf("#");
    }
    printf("\n");
}

void RayTracer::RayTracingTH(unsigned start, unsigned end) {
    // go through pixels
    for (unsigned x = start; x < end; x ++) {
        int i = x % (view.width), j = x / (view.width);
        Ray ray = view.GenerateRay(i, j);
        vec3 color = TraceRay(ray, 8) * 255.f;
        finalImg[3*(j * view.width + i) + 0] = (float)color.x;
        finalImg[3*(j * view.width + i) + 1] = (float)color.y;
        finalImg[3*(j * view.width + i) + 2] = (float)color.z;
    }
}

bool RayTracer::Syntetize()
{
    unsigned numOfTh = std::thread::hardware_concurrency() - 1;
    //unsigned numOfTh = 1;
    std::vector<std::thread> workers;
    if (numOfTh <= 0)
        numOfTh = 1;
    if (finalImg == NULL) {
        std::cout << "Image is not created, please call CreateImg() method" << std::endl;
        return false;
    }

    unsigned threadStep = imgSize / numOfTh, threadMod = imgSize % numOfTh;
    for (unsigned i = 0; i < numOfTh; i++) {
        unsigned start = i*threadStep, end = (i+1)*threadStep;
        if (i == numOfTh - 1 && threadMod != 0)
            end += threadMod;
        workers.push_back(std::thread(&RayTracer::RayTracingTH, this, start, end));
    }

    for (std::thread& t : workers)
        t.join();
    return true;
}

vec3 RayTracer::TraceRay(Ray & r, int depth) {
    // intersect objects
    vec3 color = vec3(0.0), reflectionColor = vec3(0.0), refractionColor = vec3(0.0);
    double ratio = 1.0;
    if (FindClosesInt(r)) {
        for (Light& al : lights) { // through all area light
            std::vector<Light> samples = al.SampleAreaLight();
            for (Light & pl : samples) { // throuhg all samples
                color += EvaluateLight(r, mats[r.tri->matIdx], pl); // diffuse color
            }
        }
        if (depth) { // if depth > 0 recurse on reflection and refraction
            vec3 reflCoef = vec3(mats[r.tri->matIdx].specular);
            if (reflCoef.x > 0.0 && reflCoef.y > 0.0 && reflCoef.z > 0.0) {
                Ray reflectedRay = r.ReflectRay();
                reflectionColor = TraceRay(reflectedRay, depth - 1) * reflCoef; // reflected part
            }
            vec3 transmittance = vec3(mats[r.tri->matIdx].transmittance);
            if (transmittance.x > 0.0 && transmittance.y > 0.0 && transmittance.z > 0.0) {
                Ray refractedRay = r.RefractRay(mats[r.tri->matIdx].ior, ratio);
                if (refractedRay.origin.x != std::numeric_limits<double>::min())
                    refractionColor = TraceRay(refractedRay, depth - 1) * transmittance; // refracted part
            }
        }

        color = color + reflectionColor * ratio + refractionColor * (1 - ratio);
    }
    else {
        return color;
    }
    color.clamp(0.0, 1.0);
    return color;
}

bool RayTracer::FindClosesInt(Ray& r) {
#ifndef BVH_TREE
    for (Triangle& t : triangles)
        r.IntersectTri(t);
#else
    tree.Traverse(r, tree.root);
#endif
    return r.tri ? true : false;
}

void RayTracer::SaveImg(std::string path) {
    //image saving
    std::cout << "Saving image as " << path << std::endl;
    std::ofstream output(path);
    output << "P3\n" << view.width << " " << view.height << "\n" << 255 << std::endl;
    for (int i = 0; i < view.width * view.height * 3; ++i)
        output << (int)finalImg[i] << " ";
    output.close();
}

void RayTracer::CreateImg() {
    if (finalImg == NULL)
        delete[] finalImg;
    imgSize = view.width * view.height ;
    finalImg = new float[imgSize*3];
}

void RayTracer::AddSphere(float radius, const vec3& pos) {
    spheres.push_back(Sphere(radius, pos));
}

void RayTracer::SetCamera(const Camera& cam) {
    view = cam;
}

bool RayTracer::Load(const std::string path) {
    std::vector<tinyobj::shape_t> shapes;
    tinyobj::attrib_t attribs;
    std::string camPath = path;
    camPath.replace(camPath.end() - 3, camPath.end(), "txt");

    std::cout << "Loading scene " << path << std::endl;
    if (!l.LoadScene(path, attribs, shapes, mats)) {
        std::cout << "Loading of " << path << " failed, aborting rendering" << std::endl;
        return false;
    }

    std::cout << "Loading camera information " << camPath << std::endl;
    if (!l.LoadCamera(camPath, view)) {
        std::cout << "Loading of " << camPath << " failed, aborting rendering" << std::endl;
        return false;
    }

    std::cout << "Parsing shapes to triangles" << std::endl;
    l.ParseShapes(shapes, mats, attribs, triangles, lights, tree.leafs);
    printf("Size of leafs is %d\n",(int)tree.leafs.size());
    //l.PrintLoadedMatInfo(mats);
    //CreateImg();
    return true;
}

vec3 RayTracer::EvaluateLight(Ray& r, const tinyobj::material_t& mat, Light& light) {
    if (mat.emission[0] >= .5f)
        return vec3(mat.emission);
    vec3 hit = r.origin + r.dir * r.t; // Hit coordinates
    vec3 dl = light.pos - hit; // tolight vector
    vec3 n = r.tri->CalculateNormalFromBary(r.u, r.v); // interpolated normal
    vec3 color = vec3(0.0);
    double weight = light.CalculateWeight(dl);

    Ray shadow = Ray(hit + dl.normalize() * eps, dl.normalize());
    // occluded
    if (!FindClosesInt(shadow) || shadow.tri != light.tri || std::abs(dl.length() - shadow.t >= eps))
        return color;

    dl = dl.normalize();
    vec3 rd = vec3(mat.diffuse); // Diffuse part
    vec3 rs = vec3(mat.specular); // Specular part
    
    vec3 rl = (n * 2 * n.dot(-dl) + dl).normalize(); // Reflected ray
    
    vec3 diffP = rd * std::max(n.dot(dl), 0.0); // Diffuse color
    vec3 specP = rs * std::pow(std::max((rl).dot(-r.dir),0.0), mat.shininess); // Specular color
    
    color = (light.light * weight) * (diffP + specP);
    //color.clamp(0.0, 1.0);
    return color;
}

void RayTracer::BuildAccelStruct() {
    tree.Build();
}
