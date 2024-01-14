/**
* @file _define_global.h
* 
* @brief Definition of global variables.
*/

#ifndef __WSP_CORE_DEFINE_GLOBAL_H__
#define __WSP_CORE_DEFINE_GLOBAL_H__

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include "_define_commoncore.h"
#endif

#ifdef __cplusplus
extern "C"{
#endif
    __declspec(dllimport) char WSP_EXE_FILE_PATH[MAX_LENGTH_OF_PATH];
    __declspec(dllimport) char WSP_EXE_DIR_PATH[MAX_LENGTH_OF_PATH];

#ifdef __cplusplus
}
#endif

#endif