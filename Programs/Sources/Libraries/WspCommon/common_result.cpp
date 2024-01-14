/**
 * @file common_result.c
 * 
 */

#ifndef __WSP_COMMONCORE_COMMON_RESULT_H__
#include "common_result.h"
#endif



/* -------------------------------
        Result definitions
   -------------------------------*/
/*
 *   Result range for common module: 0x00000000 - 0x00000fff
 */


WSP_DEFINE_C_RESULT_RANGE( WSP_ResultInvalidUsage, 0x00000200, 0x000002ff, WSP_RESULT_LEVEL_USAGE );
    WSP_DEFINE_C_RESULT( WSP_ResultInvalidArgument,    0x00000201, WSP_RESULT_LEVEL_USAGE );
    WSP_DEFINE_C_RESULT( WSP_ResultInvalidParameter,   0x00000202, WSP_RESULT_LEVEL_USAGE );
    WSP_DEFINE_C_RESULT( WSP_ResultUnknownParameter,   0x00000203, WSP_RESULT_LEVEL_USAGE );
    WSP_DEFINE_C_RESULT( WSP_ResultUnsupported,        0x00000204, WSP_RESULT_LEVEL_USAGE );
    WSP_DEFINE_C_RESULT( WSP_ResultNullData,           0x00000205, WSP_RESULT_LEVEL_USAGE );
    WSP_DEFINE_C_RESULT( WSP_ResultTooLargeSize,       0x00000206, WSP_RESULT_LEVEL_USAGE );
    WSP_DEFINE_C_RESULT( WSP_ResultInvalidSize,        0x00000207, WSP_RESULT_LEVEL_USAGE );

WSP_DEFINE_C_RESULT_RANGE( WSP_ResultStatusError,   0x00000100, 0x000001ff, WSP_RESULT_LEVEL_STATUS );
    WSP_DEFINE_C_RESULT( WSP_ResultCanceled,           0x00000101, WSP_RESULT_LEVEL_STATUS );
    WSP_DEFINE_C_RESULT( WSP_ResultEndOfFile,          0x00000102, WSP_RESULT_LEVEL_STATUS );
    WSP_DEFINE_C_RESULT( WSP_ResultNotFound,           0x00000103, WSP_RESULT_LEVEL_STATUS );
    WSP_DEFINE_C_RESULT( WSP_ResultResourceLeak,       0x00000104, WSP_RESULT_LEVEL_STATUS );
    WSP_DEFINE_C_RESULT( WSP_ResultInsufficientMemory, 0x00000105, WSP_RESULT_LEVEL_STATUS );

WSP_DEFINE_C_RESULT_RANGE( WSP_ResultFatalError, 0x00000300, 0x000003ff, WSP_RESULT_LEVEL_FATAL );
    WSP_DEFINE_C_RESULT( WSP_ResultNotImplemented, 0x00000301, WSP_RESULT_LEVEL_FATAL );
    WSP_DEFINE_C_RESULT( WSP_ResultUnknownError,   0x000003ff, WSP_RESULT_LEVEL_FATAL );

