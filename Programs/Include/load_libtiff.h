#pragma once
#ifndef USE_LIBTIFF
    #define USE_LIBTIFF
#endif

#ifdef USE_LIBTIFF
    #include <tiffio.h>
    #ifndef LIBTIFF_LOADED
        #pragma comment(lib, "libtiff.lib") 
        #define LIBTIFF_LOADED
    #endif
#endif