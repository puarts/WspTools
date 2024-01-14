/**
 * @file _fn-io.h
 * 
 * @brief functions which provide access to I/O functionality.
 */

#ifndef __WSP_COMMONCORE_FUNCTION_IO_H__
#define __WSP_COMMONCORE_FUNCTION_IO_H__

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include "_define_commoncore.h"
#endif
#ifndef __WSP_COMMONCORE_CTYPE_H__
#include "_ctypes.h"
#endif

#include "result.h"

#include <stdio.h>

#ifdef __cplusplus
extern "C"{
#endif

    WSP_DLL_EXPORT void WSP_DebugPrintf(const char *fmt, ...);
    WSP_DLL_EXPORT void WSP_Error      (const char *fmt, ...);
    WSP_DLL_EXPORT void WSP_Warning    (const char *fmt, ...);
    WSP_DLL_EXPORT void WSP_Printf     (const char *fmt, ...);
    WSP_DLL_EXPORT bool WSP_Exists(const char *path);
    WSP_DLL_EXPORT bool WSP_ExistsFile(const char *path);
    WSP_DLL_EXPORT bool WSP_ExistsDirectory(const char *path);

#ifdef __cplusplus
}
#endif


//#define WSP_ERROR_LOG(prefix, fmt, ...)    WSP_Error      (prefix "Error: " fmt "(%s, line:%d, %s)\n", __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)
//#define WSP_WARNING_LOG(prefix, fmt, ...)  WSP_Warning    (prefix "Warning: " fmt "(%s, line:%d, %s)\n", __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)
//#define WSP_LOG(prefix, fmt, ...)          WSP_Printf     (prefix fmt "(%s, line:%d, %s)\n", __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)
//#ifdef _DEBUG
//#define WSP_DEBUG_LOG(prefix, fmt, ...)    WSP_DebugPrintf(prefix fmt "(%s, line:%d, %s)\n", __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)
//#else
//#define WSP_DEBUG_LOG(prefix, fmt, ...) (void*)prefix
//#endif

#define WSP_ERROR_LOG(prefix, ...) printf(prefix "Error: ");printf(__VA_ARGS__);printf("(%s, line:%d, %s)\n",  __FILE__, __LINE__, __FUNCTION__)

#define WSP_WARNING_LOG(prefix, ...) printf(prefix "Warning: ");printf(__VA_ARGS__);printf("(%s, line:%d, %s)\n", __FILE__, __LINE__, __FUNCTION__)

#define WSP_LOG_LF(prefix, ...) printf(prefix);printf(__VA_ARGS__);printf("\n");/*printf("(%s, line:%d, %s)\n", __FILE__, __LINE__, __FUNCTION__)*/

#define WSP_LOG(prefix, ...) printf(prefix);printf(__VA_ARGS__);

#ifdef _DEBUG
#define WSP_DEBUG_LOG(prefix, ...) printf(prefix);printf(__VA_ARGS__);printf("(%s, line:%d, %s)\n", __FILE__, __LINE__, __FUNCTION__)
#else
#define WSP_DEBUG_LOG(prefix, ...) (void*)prefix
#endif


#define WSP_COMMON_LOG_PREFIX "[WSP/COMMON] "

#define WSP_COMMON_ERROR_LOG(...)    WSP_ERROR_LOG   (WSP_COMMON_LOG_PREFIX, __VA_ARGS__)
#define WSP_COMMON_WARNING_LOG(...)  WSP_WARNING_LOG (WSP_COMMON_LOG_PREFIX, __VA_ARGS__)
#define WSP_COMMON_LOG(...)          WSP_LOG         (WSP_COMMON_LOG_PREFIX, __VA_ARGS__)
#define WSP_COMMON_DEBUG_LOG(...)    WSP_DEBUG_LOG   (WSP_COMMON_LOG_PREFIX, __VA_ARGS__)



#define WSP_CHECK_ERROR_MSG(wstat, retFailureValue, ...) \
    if(wstat != WSP_STATE_SUCCESS){                       \
        WSP_COMMON_ERROR_LOG(__VA_ARGS__);                  \
        return retFailureValue;                       \
    }

#endif
