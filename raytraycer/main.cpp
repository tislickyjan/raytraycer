#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

#include "vec3.h"
#include "camera.h"
#include "raytracer.h"
#include "conf.h"


int main(int argc, char const *argv[])
{
    if (argc != 3) {
        printf("Wrong number of arguments, please enter arguments as follows:\n");
        printf("raytracer.exe \"path/to/scene/file.obj\" \"output/file.ppm\"\n");
        return 1;
    }
    // test - rt class
    RayTracer rt;
    // Camera and scene loading and scene parsing
    printf("----------Load info----------\n");
    bool loaded = rt.Load(argv[1]);
    printf("-----------------------------\n");

    if (loaded) {
        printf("Everything is in order, starting RayTracing\n");
        rt.CreateImg();
        
        auto start = std::chrono::high_resolution_clock::now();
#ifdef BVH_TREE
        rt.BuildAccelStruct();
#endif
        
        printf("Starting image syntesis\n");
#ifdef MULTITHREAD
        rt.Syntetize(); // multi thread
#else
        rt.RayTracing(); // single thread
#endif
        printf("Image finished\n");

        auto end = std::chrono::high_resolution_clock::now();
        double dur = (double)std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        dur = dur / 1000.0;
        printf("---------Time----------------\n");
        printf("Computation time: %lf s\n", dur);
        printf("-----------------------------\n");
        rt.SaveImg(argv[2]);

    } else {
        printf("Loading was unsuccesful, could not load file %s\nOr it's affiliated files.\n", argv[1]);
        return 1;
    }
    
    return 0;
}