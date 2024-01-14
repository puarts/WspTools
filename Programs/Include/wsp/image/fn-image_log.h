/**
 * @file _define_graphic.h
 * 
 * definition of graphic component
 */

#ifndef WSP_IMAGE_LOG_H_
#define WSP_IMAGE_LOG_H_

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include <wsp/common/_define_commoncore.h>
#endif

#define WSP_IMAGE_LOG_PREFIX "[WSP/IMG] "

#define WSP_IMAGE_ERROR_LOG(...)    WSP_ERROR_LOG   (WSP_IMAGE_LOG_PREFIX, __VA_ARGS__)
#define WSP_IMAGE_WARNING_LOG(...)  WSP_WARNING_LOG (WSP_IMAGE_LOG_PREFIX, __VA_ARGS__)
#define WSP_IMAGE_LOG(...)          WSP_LOG         (WSP_IMAGE_LOG_PREFIX, __VA_ARGS__)
#define WSP_IMAGE_DEBUG_LOG(...)    WSP_DEBUG_LOG   (WSP_IMAGE_LOG_PREFIX, __VA_ARGS__)


#endif