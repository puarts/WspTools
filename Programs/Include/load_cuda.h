#pragma once

#define USE_CUDA

#ifdef USE_CUDA
    #ifndef CUDA_LOADED
        #define CUDA_LOADED
        #pragma comment(lib, "cudart.lib")
        #pragma comment(lib, "cufft.lib")
        #pragma comment(lib, "freeglut.lib")
        #if defined WIN64 || defined X64
            #pragma comment(lib, "cutil64.lib")
            #pragma comment(lib, "glew64.lib")
        #else
            #pragma comment(lib, "cutil32.lib")
            #pragma comment(lib, "glew32.lib")
        #endif
    #endif
#endif