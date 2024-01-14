/**
 * @file _define_graphic.h
 * 
 * definition of graphic component
 */

#ifndef WSP_GRAPHIC_LOG_H_
#define WSP_GRAPHIC_LOG_H_

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include <wsp/common/_define_commoncore.h>
#endif

#define WSP_GRAPHIC_LOG_PREFIX "[wsp/graphics] "

#define WSP_GRAPHIC_ERROR_LOG(fmt, ...)    WSP_ERROR_LOG   (WSP_GRAPHIC_LOG_PREFIX, fmt, __VA_ARGS__)
#define WSP_GRAPHIC_WARNING_LOG(fmt, ...)  WSP_WARNING_LOG (WSP_GRAPHIC_LOG_PREFIX, fmt, __VA_ARGS__)
#define WSP_GRAPHIC_LOG(fmt, ...)          WSP_LOG         (WSP_GRAPHIC_LOG_PREFIX, fmt, __VA_ARGS__)
#define WSP_GRAPHIC_LOG_LF(fmt, ...)       WSP_LOG_LF      (WSP_GRAPHIC_LOG_PREFIX, fmt, __VA_ARGS__)
#define WSP_GRAPHIC_DEBUG_LOG(fmt, ...)    WSP_DEBUG_LOG   (WSP_GRAPHIC_LOG_PREFIX, fmt, __VA_ARGS__)


#endif