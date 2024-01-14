#pragma once

#ifndef OPENNI_LOADED
    #define OPENNI_LOADED
    #ifdef _WIN64
        #pragma comment(lib, "openNI64.lib")
        //#pragma comment(lib, "NiSampleModule64")
        //#pragma comment(lib, "NiSampleExtensionModule64")
    #else
        #pragma comment(lib, "openNI.lib")
        //#pragma comment(lib, "NiSampleModule")
        //#pragma comment(lib, "NiSampleExtensionModule")
    #endif
#endif
