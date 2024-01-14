#pragma once

#define USE_QT4

#ifdef USE_QT4
    #ifndef QT_DEFINE
        #define QT_LARGEFILE_SUPPORT
        #define QT_THREAD_SUPPORT
        #define QT_CORE_LIB
        #define QT_GUI_LIB
        #define QT_OPENGL_LIB
        #define QT_DEFINE
    #endif
    #ifndef QT_LOADED
        #ifdef _DEBUG
            #define QT_NO_DEBUG 0
            //#pragma comment(lib, "qtmaind.lib") 
            //#pragma comment(lib, "QtCored4.lib") 
            //#pragma comment(lib, "QtGuid4.lib") 
            //#pragma comment(lib, "QtOpenGLd4.lib")

            #pragma comment(lib, "qtmain.lib") 
            #pragma comment(lib, "QtCore4.lib") 
            #pragma comment(lib, "QtGui4.lib") 
            #pragma comment(lib, "QtOpenGL4.lib")
            #pragma comment(lib, "QtXml4.lib")
        #else
            #define QT_NO_DEBUG 1
            #pragma comment(lib, "qtmain.lib") 
            #pragma comment(lib, "QtCore4.lib") 
            #pragma comment(lib, "QtGui4.lib") 
            #pragma comment(lib, "QtOpenGL4.lib")
            #pragma comment(lib, "QtXml4.lib")
        #endif
        #define QT_LOAD
    #endif
#endif