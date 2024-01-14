/**
* @file _define_commoncore.h
* 
* @brief Base definition of core module.
*/

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#define __WSP_COMMONCORE_DEFINE_COMMONCORE_H__

#if DEBUG==1
#ifndef _DEBUG
#define _DEBUG
#endif
#endif

/*------------------------------------------- 
OS type definition
--------------------------------------------*/
//#ifdef _WIN32
//    #define WIN32
//#endif
//#define UNIX

/*------------------------------------------- 
Memory allocation definition
--------------------------------------------*/
#ifdef _DEBUG
    #ifdef _WIN32
        #include <crtdbg.h>
        #define _CRTDBG_MAP_ALLOC
        #define WSP_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #else
        #define WSP_NEW new
    #endif
#else
    #define WSP_NEW new
#endif


#ifdef _WIN32
#pragma warning(disable:4996)
//#pragma warning(disable:4200 4507)

//#ifdef _UNICODE
//#if defined _M_IX86
//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
//#elif defined _M_IA64
//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
//#elif defined _M_X64
//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
//#else
//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
//#endif
//#endif
#endif

#ifdef _WINDLL
    #define WSP_DLL_EXPORT __declspec(dllexport)
    #define WSP_DLL_GLOBAL_VAR extern
#else
    #define WSP_DLL_EXPORT 
    #define WSP_DLL_GLOBAL_VAR __declspec(dllimport)
#endif

#ifdef USE_OPENMAYA
#ifndef NT_PLUGIN
#define NT_PLUGIN
#endif
#endif

//#ifndef POINTER_SIZE
//#  if defined(_WIN64)
//#   define POINTER_SIZE 8
//#  elif defined(_WIN32) || defined(Q_OS_WINCE) || defined(Q_OS_SYMBIAN)
//#   define POINTER_SIZE 4
//#  endif
//#endif

/** @brief bit per byte */
#define ONE_BYTE (8) 
#define MAX_LENGTH_OF_EXTENSION (16)
#define MAX_LENGTH_OF_NAME (128)
#define MAX_LENGTH_OF_PATH (512)
#define MAX_LENGTH_OF_DISCRIPTION (512)
/** @brief max number of buffer array size */
#define MAX_BUFFER (1024*10) 
#define MAX_STACK_DEPTH (300)
#define MAX_WHILE_LOOP (1000)
#define SIGNED_DATA_TYPE double
#define UNSIGNED_DATA_TYPE ushort

/** @brief number of spaces for per indent */
#define INDENT_SPACES (2) 


#define RESERVE_MEM (64)

//! column and precision for the list
#define PRINT_COLW (6)
#define PRINT_PREC (1)
#define PRINT_TYPE std::fixed
//#define PRINT_TYPE scientific



#ifndef NULL
#define NULL 0
#endif


#endif
