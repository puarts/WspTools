/**
 * @file image_result.h
 * @author Jun-ichi Nishikata
 */

#pragma once

#ifndef __WSP_COMMONCORE_RESULT_H__
#include <wsp/common/result.h>
#endif

#ifdef __cplusplus
extern "C"{
#endif
    /* DevIL results */
    WSP_DECLARE_C_RESULT( WSP_ResultIlError              );
    WSP_DECLARE_C_RESULT( WSP_ResultIlNoError            ); 
    WSP_DECLARE_C_RESULT( WSP_ResultIlInvalidEnum        ); 
    WSP_DECLARE_C_RESULT( WSP_ResultIlOutOfMemory        );
    WSP_DECLARE_C_RESULT( WSP_ResultIlFormatNotSupported );
    WSP_DECLARE_C_RESULT( WSP_ResultIlInternalError      ); 
    WSP_DECLARE_C_RESULT(WSP_ResultIlInvalidValue);
    WSP_DECLARE_C_RESULT(WSP_ResultIlIllegalOperation);
    WSP_DECLARE_C_RESULT(WSP_ResultIlIllegalFileValue);
    WSP_DECLARE_C_RESULT(WSP_ResultIlInvalidFileHeader);
    WSP_DECLARE_C_RESULT(WSP_ResultIlInvalidParam);
    WSP_DECLARE_C_RESULT(WSP_ResultIlCouldNotOpenFile);
    WSP_DECLARE_C_RESULT(WSP_ResultIlInvalidExtension);
    WSP_DECLARE_C_RESULT(WSP_ResultIlFileAlreadyExists);
    WSP_DECLARE_C_RESULT(WSP_ResultIlOutFormatSame);
    WSP_DECLARE_C_RESULT(WSP_ResultIlStackOverflow);
    WSP_DECLARE_C_RESULT(WSP_ResultIlStackUnderflow);
    WSP_DECLARE_C_RESULT(WSP_ResultIlInvalidConversion);
    WSP_DECLARE_C_RESULT(WSP_ResultIlBadDimensions);
    WSP_DECLARE_C_RESULT(WSP_ResultIlFileReadError);
    WSP_DECLARE_C_RESULT(WSP_ResultIlUnknownError);
#ifdef __cplusplus
}
#endif

/*
*   Result range for image module: 0x00001000 - 0x00001fff
*/
#ifdef __cplusplus
namespace wsp{ namespace img{

    WSP_DEFINE_CPP_RESULT_RANGE( ResultInvalidUsage, 0x00001000, 0x000010ff, WSP_RESULT_LEVEL_USAGE );
    WSP_DEFINE_CPP_RESULT_RANGE( ResultIsExternalData, 0x00001001, 0x00001001, WSP_RESULT_LEVEL_USAGE );
    WSP_DEFINE_CPP_RESULT_RANGE( ResultDifferentNumOfChannels, 0x00001002, 0x00001002, WSP_RESULT_LEVEL_USAGE );

    WSP_DEFINE_CPP_RESULT_RANGE(ResultFatalError, 0x00001100, 0x000011ff, WSP_RESULT_LEVEL_FATAL);

    /* DevIL error: 0x00001200 - 0x000012ff */
    WSP_CPP_RESULT_FROM_C_RESULT(ResultIlError, WSP_ResultIlError);
    WSP_CPP_RESULT_FROM_C_RESULT(ResultIlNoError, WSP_ResultIlNoError);
    WSP_CPP_RESULT_FROM_C_RESULT(ResultIlInvalidEnum, WSP_ResultIlInvalidEnum);
    WSP_CPP_RESULT_FROM_C_RESULT(ResultIlOutOfMemory, WSP_ResultIlOutOfMemory);
    WSP_CPP_RESULT_FROM_C_RESULT(ResultIlFormatNotSupported, WSP_ResultIlFormatNotSupported);
    WSP_CPP_RESULT_FROM_C_RESULT(ResultIlInternalError, WSP_ResultIlInternalError);
    WSP_CPP_RESULT_FROM_C_RESULT(ResultIlInvalidValue, WSP_ResultIlInvalidValue);
    WSP_CPP_RESULT_FROM_C_RESULT(ResultIlIllegalOperation, WSP_ResultIlIllegalOperation);
    WSP_CPP_RESULT_FROM_C_RESULT(ResultIlIllegalFileValue, WSP_ResultIlIllegalFileValue);
    WSP_CPP_RESULT_FROM_C_RESULT(ResultIlInvalidFileHeader, WSP_ResultIlInvalidFileHeader);
    WSP_CPP_RESULT_FROM_C_RESULT(ResultIlInvalidParam, WSP_ResultIlInvalidParam);
    WSP_CPP_RESULT_FROM_C_RESULT(ResultIlCouldNotOpenFile, WSP_ResultIlCouldNotOpenFile);
    WSP_CPP_RESULT_FROM_C_RESULT(ResultIlInvalidExtension, WSP_ResultIlInvalidExtension);
    WSP_CPP_RESULT_FROM_C_RESULT(ResultIlFileAlreadyExists, WSP_ResultIlFileAlreadyExists);
    WSP_CPP_RESULT_FROM_C_RESULT(ResultIlOutFormatSame, WSP_ResultIlOutFormatSame);
    WSP_CPP_RESULT_FROM_C_RESULT(ResultIlStackOverflow, WSP_ResultIlStackOverflow);
    WSP_CPP_RESULT_FROM_C_RESULT(ResultIlStackUnderflow, WSP_ResultIlStackUnderflow);
    WSP_CPP_RESULT_FROM_C_RESULT(ResultIlInvalidConversion, WSP_ResultIlInvalidConversion);
    WSP_CPP_RESULT_FROM_C_RESULT(ResultIlBadDimensions, WSP_ResultIlBadDimensions);
    WSP_CPP_RESULT_FROM_C_RESULT(ResultIlFileReadError, WSP_ResultIlFileReadError);

}}
#endif
