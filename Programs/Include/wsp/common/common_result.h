/**
 * @file common_result.h
 * 
 */

#ifndef __WSP_COMMONCORE_COMMON_RESULT_H__
#define __WSP_COMMONCORE_COMMON_RESULT_H__

#ifndef __WSP_COMMONCORE_RESULT_H__
#include "result.h"
#endif

/* ======================================
     declare C result
   ======================================*/
#ifdef __cplusplus
extern "C"{
#endif
    /* -------------------------------
            Result declarations
       -------------------------------*/
    WSP_DECLARE_C_RESULT( WSP_ResultInvalidUsage );
        WSP_DECLARE_C_RESULT( WSP_ResultInvalidArgument );
        WSP_DECLARE_C_RESULT( WSP_ResultInvalidParameter );
        WSP_DECLARE_C_RESULT( WSP_ResultUnsupported );
        WSP_DECLARE_C_RESULT( WSP_ResultNullData );
        WSP_DECLARE_C_RESULT( WSP_ResultTooLargeSize );
        WSP_DECLARE_C_RESULT( WSP_ResultInvalidSize );

    WSP_DECLARE_C_RESULT( WSP_ResultStatusError );
        WSP_DECLARE_C_RESULT( WSP_ResultCanceled );
        WSP_DECLARE_C_RESULT( WSP_ResultEndOfFile );
        WSP_DECLARE_C_RESULT( WSP_ResultUnknownParameter );
        WSP_DECLARE_C_RESULT( WSP_ResultNotFound );
        WSP_DECLARE_C_RESULT( WSP_ResultResourceLeak );
        WSP_DECLARE_C_RESULT( WSP_ResultInsufficientMemory );

    WSP_DECLARE_C_RESULT( WSP_ResultFatalError );
        WSP_DECLARE_C_RESULT( WSP_ResultNotImplemented );
        WSP_DECLARE_C_RESULT( WSP_ResultUnknownError );

#ifdef __cplusplus
}
#endif


/* ======================================
     define C++ wrapper for C result
   ======================================*/
#ifdef __cplusplus
namespace wsp{
    WSP_CPP_RESULT_FROM_C_RESULT( ResultInvalidUsage, WSP_ResultInvalidUsage );
        WSP_CPP_RESULT_FROM_C_RESULT( ResultInvalidArgument, WSP_ResultInvalidArgument );
        WSP_CPP_RESULT_FROM_C_RESULT( ResultInvalidParameter, WSP_ResultInvalidParameter );
        WSP_CPP_RESULT_FROM_C_RESULT( ResultUnsupported, WSP_ResultUnsupported );
        WSP_CPP_RESULT_FROM_C_RESULT( ResultNullData, WSP_ResultNullData );
        WSP_CPP_RESULT_FROM_C_RESULT( ResultTooLargeSize, WSP_ResultTooLargeSize );
        WSP_CPP_RESULT_FROM_C_RESULT( ResultInvalidSize, WSP_ResultInvalidSize );
        

    WSP_CPP_RESULT_FROM_C_RESULT( ResultStatusError, WSP_ResultStatusError );
        WSP_CPP_RESULT_FROM_C_RESULT( ResultCanceled, WSP_ResultCanceled );
        WSP_CPP_RESULT_FROM_C_RESULT( ResultEndOfFile, WSP_ResultEndOfFile );
        WSP_CPP_RESULT_FROM_C_RESULT( ResultNotFound, WSP_ResultNotFound );
        WSP_CPP_RESULT_FROM_C_RESULT( ResultResourceLeak, WSP_ResultResourceLeak );
        WSP_CPP_RESULT_FROM_C_RESULT( ResultInsufficientMemory, WSP_ResultInsufficientMemory );
        WSP_CPP_RESULT_FROM_C_RESULT( ResultUnknownParameter, WSP_ResultUnknownParameter );

    WSP_CPP_RESULT_FROM_C_RESULT( ResultFatal, WSP_ResultFatalError );
        WSP_CPP_RESULT_FROM_C_RESULT( ResultNotImplemented, WSP_ResultNotImplemented );
        WSP_CPP_RESULT_FROM_C_RESULT( ResultUnknownError, WSP_ResultUnknownError );
}
#endif



#endif
