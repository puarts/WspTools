#pragma once

#define USE_OPENFRAMEWORKS

#ifdef USE_OPENFRAMEWORKS
    #ifndef OPENFRAMEWORKS_LOADED
        #define OPENFRAMEWORKS_LOADED
        #ifdef _DEBUG
            #pragma comment(lib, "openframeworksLibDebug.lib")
        #else
            #pragma comment(lib, "openframeworksLib.lib")
        #endif
    #endif
#endif