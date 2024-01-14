/**
 * @file _fn-gui_io.h
 * @author Jun-ichi Nishikata
 * @brief functions which provide access to I/O functionality.
 */

#ifndef __WSP_GUI_FUNCTION_GUI_IO_H__
#define __WSP_GUI_FUNCTION_GUI_IO_H__

#include <wsp/common/fn-io.h>

#ifdef _DEBUG
#define ENABLE_WSP_GUI_LOG
#endif

#define WSP_GUI_LOG_PREFIX "[WSP/GUI] "

#ifdef ENABLE_WSP_GUI_LOG
#define WSP_GUI_ERROR_LOG(fmt, ...)    WSP_ERROR_LOG   (WSP_GUI_LOG_PREFIX,fmt, __VA_ARGS__)
#define WSP_GUI_WARNING_LOG(fmt, ...)  WSP_WARNING_LOG (WSP_GUI_LOG_PREFIX,fmt, __VA_ARGS__)
#define WSP_GUI_LOG(fmt, ...)          WSP_LOG         (WSP_GUI_LOG_PREFIX,fmt, __VA_ARGS__)
#define WSP_GUI_DEBUG_LOG(fmt, ...)    WSP_DEBUG_LOG   (WSP_GUI_LOG_PREFIX,fmt, __VA_ARGS__)
#else
#define WSP_GUI_ERROR_LOG(fmt, ...)    (0)
#define WSP_GUI_WARNING_LOG(fmt, ...)  (0)
#define WSP_GUI_LOG(fmt, ...)          (0)
#define WSP_GUI_DEBUG_LOG(fmt, ...)    (0)
#endif

#endif
