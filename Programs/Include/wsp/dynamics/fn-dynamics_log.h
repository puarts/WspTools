/**
 * @file _fn-dynamics_log.h
 * 
 * @brief log macros.
 */

#ifndef __WSP_DYNAMICS_FUNCTION_DYNAMICS_LOG_H__
#define __WSP_DYNAMICS_FUNCTION_DYNAMICS_LOG_H__

#include <wsp/common/fn-io.h>

#ifdef _DEBUG
#define ENABLE_WSP_DYNAMICS_LOG
#endif

#define WSP_DYNAMICS_LOG_PREFIX "[WSP/DYNAMICS] "

#ifdef ENABLE_WSP_DYNAMICS_LOG
#define WSP_DYNAMICS_ERROR_LOG(fmt, ...)    WSP_ERROR_LOG   (WSP_DYNAMICS_LOG_PREFIX,fmt, __VA_ARGS__)
#define WSP_DYNAMICS_WARNING_LOG(fmt, ...)  WSP_WARNING_LOG (WSP_DYNAMICS_LOG_PREFIX,fmt, __VA_ARGS__)
#define WSP_DYNAMICS_LOG(fmt, ...)          WSP_LOG         (WSP_DYNAMICS_LOG_PREFIX,fmt, __VA_ARGS__)
#define WSP_DYNAMICS_DEBUG_LOG(fmt, ...)    WSP_DEBUG_LOG   (WSP_DYNAMICS_LOG_PREFIX,fmt, __VA_ARGS__)
#else
#define WSP_DYNAMICS_ERROR_LOG(fmt, ...)    (0)
#define WSP_DYNAMICS_WARNING_LOG(fmt, ...)  (0)
#define WSP_DYNAMICS_LOG(fmt, ...)          (0)
#define WSP_DYNAMICS_DEBUG_LOG(fmt, ...)    (0)
#endif

#endif
