/**
 * @file common_result_converter.h
 * 
 */

#ifndef WSP_COMMONCORE_COMMON_RESULT_CONVERTER_H_
#define WSP_COMMONCORE_COMMON_RESULT_CONVERTER_H_

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include "_define_commoncore.h"
#endif
#ifndef __WSP_COMMONCORE_RESULT_H__
#include "result.h"
#endif
#ifndef __WSP_COMMONCORE_COMMON_RESULT_H__
#include "common_result.h"
#endif


inline WSP_Result WSP_MakeCommonResult( int value )
{
    switch( value )
    {
    case WSP_STATE_FAILURE:             return WSP_ResultUnknownError();
    case WSP_STATE_SUCCESS:             return WSP_ResultSuccess();
    case WSP_STATE_CANCELED:            return WSP_ResultCanceled();
    case WSP_STATE_INSUFFICIENT_MEMORY: return WSP_ResultInsufficientMemory();
    case WSP_STATE_INVALID_ARGUMENT:    return WSP_ResultInvalidArgument();
    case WSP_STATE_INVALID_PARAMETER:   return WSP_ResultInvalidParameter();
    case WSP_STATE_NOT_FOUND:           return WSP_ResultNotFound();
    case WSP_STATE_NOT_IMPLEMENTED:     return WSP_ResultNotImplemented();
    case WSP_STATE_UNSUPPORTED_TYPE:    return WSP_ResultUnsupported();
    case WSP_STATE_UNKNOWN_PARAMETER:   return WSP_ResultUnknownParameter();
    case WSP_STATE_END_OF_FILE:         return WSP_ResultEndOfFile();
    case WSP_STATE_NULL_DATA:           return WSP_ResultNullData();
    default:                            return WSP_ResultUnknownError();
    }
}


#endif
