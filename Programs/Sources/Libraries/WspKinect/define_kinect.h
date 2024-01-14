#pragma once

#if defined(_WIN32) // Microsoft Visual Studio
    #ifndef RC_INVOKED
        #ifdef _MSC_VER // MingW32 (added gameover/m gingold 10/07/2011)
            #if _MSC_VER >= 1300 && _MSC_VER <= 1600// Before MSVC7 (2003)
            #define USE_XN
            #endif
        #endif
    #endif
#endif