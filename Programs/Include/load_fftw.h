#pragma once

#define USE_FFTW3

#ifdef USE_FFTW3
    #include <fftw3.h>

    #ifndef FFTW_LOADED
        #define FFTW_LOADED
        #pragma comment(lib, "libfftw3-3.lib")
        #pragma comment(lib, "libfftw3f-3.lib")
        #pragma comment(lib, "libfftw3l-3.lib")
    #endif
#endif