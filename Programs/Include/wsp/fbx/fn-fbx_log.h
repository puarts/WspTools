/**
 * @file _define_graphic.h
 * 
 * definition of graphic component
 */

#ifndef WSP_FBX_LOG_H_
#define WSP_FBX_LOG_H_

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include <wsp/common/_define_commoncore.h>
#endif

#define WSP_FBX_LOG_PREFIX "[wsp/fbx] "

#define WSP_FBX_ERROR_LOG(fmt, ...)    WSP_ERROR_LOG   (WSP_FBX_LOG_PREFIX, fmt, __VA_ARGS__)
#define WSP_FBX_WARNING_LOG(fmt, ...)  WSP_WARNING_LOG (WSP_FBX_LOG_PREFIX, fmt, __VA_ARGS__)
#define WSP_FBX_LOG(fmt, ...)          WSP_LOG         (WSP_FBX_LOG_PREFIX, fmt, __VA_ARGS__)
#define WSP_FBX_LOG_LF(fmt, ...)       WSP_LOG_LF      (WSP_FBX_LOG_PREFIX, fmt, __VA_ARGS__)
#define WSP_FBX_DEBUG_LOG(fmt, ...)    WSP_DEBUG_LOG   (WSP_FBX_LOG_PREFIX, fmt, __VA_ARGS__)


#endif