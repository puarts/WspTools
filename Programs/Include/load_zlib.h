#pragma once
#define USE_ZLIB

#ifdef USE_ZLIB
    #include <zlib.h>
    #ifndef ZLIB_LOADED
        #pragma comment(lib, "zdll.lib") 
        #define ZLIB_LOADED
    #endif
#endif