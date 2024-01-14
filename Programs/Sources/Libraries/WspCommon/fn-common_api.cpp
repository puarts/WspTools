/**
 * @file _define_core.c
 * 
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "fn-common_api.h"
#include "_define_global.h"
#include "fn-string.h"
#include "fn-io.h"


void WSP_Init(int argc, char **argv)
{
#ifdef _DEBUG
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
#endif
    strcpy( WSP_EXE_FILE_PATH, argv[0] );
    WSP_GetDirectoryPathFromFilePath( WSP_EXE_DIR_PATH, WSP_EXE_FILE_PATH );
#ifdef _DEBUG
    #if _WIN64
        WSP_COMMON_DEBUG_LOG("x64 program");
    #else
        WSP_COMMON_DEBUG_LOG("win32 program");
    #endif
    WSP_COMMON_DEBUG_LOG("File Path:      %s", WSP_EXE_FILE_PATH);
    WSP_COMMON_DEBUG_LOG("Directory Path: %s", WSP_EXE_DIR_PATH);
#endif
}

