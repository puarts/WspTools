/**
 * @file _fn-test_io.h
 * @author Jun-ichi Nishikata
 * @brief functions which provide access to I/O functionality.
 */

#ifndef __WSP_TESTCORE_FUNCTION_IO_H__
#define __WSP_TESTCORE_FUNCTION_IO_H__

#include <wsp/common/fn-io.h>

#define WSP_TEST_LOG_PREFIX "[WSP/TEST] "

#define WSP_TEST_ERROR_LOG(fmt, ...)    WSP_ERROR_LOG   (WSP_TEST_LOG_PREFIX,fmt, __VA_ARGS__)
#define WSP_TEST_WARNING_LOG(fmt, ...)  WSP_WARNING_LOG (WSP_TEST_LOG_PREFIX,fmt, __VA_ARGS__)
#define WSP_TEST_LOG(fmt, ...)          WSP_LOG         (WSP_TEST_LOG_PREFIX,fmt, __VA_ARGS__)
#define WSP_TEST_DEBUG_LOG(fmt, ...)    WSP_DEBUG_LOG   (WSP_TEST_LOG_PREFIX,fmt, __VA_ARGS__)

#endif
