#pragma once

#ifndef USE_XERCES
    #define USE_XERCES
#endif

#ifdef USE_XERCES
    #ifndef XERCES_LOADED
        #define XERCES_LOADED
        #ifdef _DEBUG
            #pragma comment(lib, "xerces-c_3D.lib")
            #pragma comment(lib, "xerces-c_static_3D.lib")
        #else
            #pragma comment(lib, "xerces-c_3.lib")
            #pragma comment(lib, "xerces-c_static_3.lib")
        #endif
    #endif
#endif