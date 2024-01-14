/**
 * @file common_result_converter.h
 * 
 */

#ifndef WSP_COMMONCORE_COMMON_RESULT_CONVERTER_H_
#define WSP_COMMONCORE_COMMON_RESULT_CONVERTER_H_

#include "_define_mathcore.h"

#ifndef __WSP_COMMONCORE_RESULT_H__
#include <wsp/common/result.h>
#endif

#ifndef __WSP_COMMONCORE_COMMON_RESULT_H__
#include <wsp/common/common_result.h>
#endif

#include "_math_types.h"
#include "math_result.hpp"


inline WSP_Result WSP_MakeMathResult( WSP_MathState value )
{
    switch( value )
    {
    case WSP_MATH_STATE_SUCCESS          : return wsp::ResultSuccess();
    case WSP_MATH_STATE_FAILURE          : return wsp::ResultUnknownError();
    case WSP_MATH_STATE_INVALID_PARAMETER: return wsp::ResultInvalidParameter();
    case WSP_MATH_STATE_PARALLEL         : return wsp::math::ResultParallel();
    default                              : return wsp::ResultUnknownError();
    }
}


#endif
