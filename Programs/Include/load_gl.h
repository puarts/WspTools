#pragma once

#ifndef USE_GL
#define USE_GL
#endif

#ifdef USE_GL
    #ifndef GL_LOADED
        #if defined _WIN64
            #pragma comment(lib, "opengl32_x64.lib") 
            #pragma comment(lib, "freeglut64.lib") 
            //#pragma comment(lib, "glut32_x64.lib") 
        //#elif defined _WIN32
        #else
            #pragma comment(lib, "opengl32.lib") 
            #pragma comment(lib, "freeglut.lib") 
            //#pragma comment(lib, "glut32.lib") 
        #endif
        //#pragma comment(lib, "glu32.lib") 
        //#pragma comment(lib, "glew32.lib") 
        #pragma comment(lib, "glew32s.lib") 
        #define GL_LOADED
    #endif
#endif
