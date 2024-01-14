/**
 * @file _fn-kinect_log.h
 * @author Jun-ichi Nishikata
 * @brief functions which provide access to I/O functionality.
 */

#ifndef __WSP_KINECT_FUNCTION_KINECT_LOG_H__
#define __WSP_KINECT_FUNCTION_KINECT_LOG_H__

#include <wsp/common/fn-io.h>

#ifdef _DEBUG
#define ENABLE_WSP_KINECT_LOG
#endif

#define WSP_KINECT_LOG_PREFIX "[WSP/KINECT] "

#ifdef ENABLE_WSP_KINECT_LOG
#define WSP_KINECT_ERROR_LOG(fmt, ...)    WSP_ERROR_LOG   (WSP_KINECT_LOG_PREFIX,fmt, __VA_ARGS__)
#define WSP_KINECT_WARNING_LOG(fmt, ...)  WSP_WARNING_LOG (WSP_KINECT_LOG_PREFIX,fmt, __VA_ARGS__)
#define WSP_KINECT_LOG(fmt, ...)          WSP_LOG         (WSP_KINECT_LOG_PREFIX,fmt, __VA_ARGS__)
#define WSP_KINECT_DEBUG_LOG(fmt, ...)    WSP_DEBUG_LOG   (WSP_KINECT_LOG_PREFIX,fmt, __VA_ARGS__)
#else
#define WSP_KINECT_ERROR_LOG(fmt, ...)    (0)
#define WSP_KINECT_WARNING_LOG(fmt, ...)  (0)
#define WSP_KINECT_LOG(fmt, ...)          (0)
#define WSP_KINECT_DEBUG_LOG(fmt, ...)    (0)
#endif

#endif
