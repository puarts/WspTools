/**
 * @file fn-multimedia_log.h
 * @author Jun-ichi Nishikata
 */

#ifndef WSP_MULTIMEDIA_FN_MULTIMEDIA_LOG_H_
#define WSP_MULTIMEDIA_FN_MULTIMEDIA_LOG_H_

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include <wsp/common/_define_commoncore.h>
#endif

#define WSP_MULTIMEDIA_LOG_PREFIX "[WSP/MULTIMEDIA] "

#define WSP_MULTIMEDIA_ERROR_LOG(fmt, ...)    WSP_ERROR_LOG   (WSP_MULTIMEDIA_LOG_PREFIX, fmt, __VA_ARGS__)
#define WSP_MULTIMEDIA_WARNING_LOG(fmt, ...)  WSP_WARNING_LOG (WSP_MULTIMEDIA_LOG_PREFIX, fmt, __VA_ARGS__)
#define WSP_MULTIMEDIA_LOG(fmt, ...)          WSP_LOG         (WSP_MULTIMEDIA_LOG_PREFIX, fmt, __VA_ARGS__)
#define WSP_MULTIMEDIA_DEBUG_LOG(fmt, ...)    WSP_DEBUG_LOG   (WSP_MULTIMEDIA_LOG_PREFIX, fmt, __VA_ARGS__)


#endif