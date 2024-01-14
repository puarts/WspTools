#pragma once

#define USE_DevIL

#ifdef USE_DevIL
    #include <IL/il.h>
    //#include <IL/ilu.h>
    //#include <IL/ilut.h>

    #ifndef DevIL_LOADED
        #define DevIL_LOADED
        #pragma comment(lib, "DevIL.lib")
        //#pragma comment(lib, "ILU.lib")
        //#pragma comment(lib, "ILUT.lib")
    #endif
#endif