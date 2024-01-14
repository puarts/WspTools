/**
 * @file math_result.h
 * 
 */

#ifndef __WSP_MATHUTIL_MATH_RESULT_H__
#define __WSP_MATHUTIL_MATH_RESULT_H__

#ifndef __WSP_COMMONCORE_RESULT_H__
#include <wsp/common/result.h>
#endif

#ifdef __cplusplus
namespace wsp{ namespace math{

    WSP_DEFINE_CPP_RESULT_RANGE( ResultInvalidUsage, 0x00002000, 0x000020ff, WSP_RESULT_LEVEL_USAGE );

    WSP_DEFINE_CPP_RESULT_RANGE( ResultStatusError, 0x00002100, 0x000021ff, WSP_RESULT_LEVEL_STATUS );
        WSP_DEFINE_CPP_RESULT( ResultParallel, 0x00002101, WSP_RESULT_LEVEL_STATUS );

    WSP_DEFINE_CPP_RESULT_RANGE( ResultFatalError, 0x00002200, 0x000022ff, WSP_RESULT_LEVEL_FATAL );
}}
#endif

#endif
