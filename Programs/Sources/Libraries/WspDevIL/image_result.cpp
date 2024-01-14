/**
 * @file image_result.c
 * 
 */

#include "image_result.h"

/* DevIL error: 0x00001200 - 0x000012ff */
WSP_DEFINE_C_RESULT_RANGE( WSP_ResultIlError, 0x00001200, 0x000012ff, WSP_RESULT_LEVEL_FATAL );
    WSP_DEFINE_C_RESULT( WSP_ResultIlNoError           , 0x00001201, WSP_RESULT_LEVEL_FATAL ); 
    WSP_DEFINE_C_RESULT( WSP_ResultIlInvalidEnum       , 0x00001202, WSP_RESULT_LEVEL_FATAL ); 
    WSP_DEFINE_C_RESULT( WSP_ResultIlOutOfMemory       , 0x00001203, WSP_RESULT_LEVEL_FATAL );
    WSP_DEFINE_C_RESULT( WSP_ResultIlFormatNotSupported, 0x00001204, WSP_RESULT_LEVEL_FATAL );
    WSP_DEFINE_C_RESULT( WSP_ResultIlInternalError     , 0x00001205, WSP_RESULT_LEVEL_FATAL ); 
    WSP_DEFINE_C_RESULT( WSP_ResultIlInvalidValue      , 0x00001206, WSP_RESULT_LEVEL_FATAL ); 
    WSP_DEFINE_C_RESULT( WSP_ResultIlIllegalOperation  , 0x00001207, WSP_RESULT_LEVEL_FATAL ); 
    WSP_DEFINE_C_RESULT( WSP_ResultIlIllegalFileValue  , 0x00001208, WSP_RESULT_LEVEL_FATAL );
    WSP_DEFINE_C_RESULT( WSP_ResultIlInvalidFileHeader , 0x00001209, WSP_RESULT_LEVEL_FATAL );
    WSP_DEFINE_C_RESULT( WSP_ResultIlInvalidParam      , 0x0000120a, WSP_RESULT_LEVEL_FATAL ); 
    WSP_DEFINE_C_RESULT( WSP_ResultIlCouldNotOpenFile  , 0x0000120b, WSP_RESULT_LEVEL_FATAL );
    WSP_DEFINE_C_RESULT( WSP_ResultIlInvalidExtension  , 0x0000120c, WSP_RESULT_LEVEL_FATAL ); 
    WSP_DEFINE_C_RESULT( WSP_ResultIlFileAlreadyExists , 0x0000120d, WSP_RESULT_LEVEL_FATAL );
    WSP_DEFINE_C_RESULT( WSP_ResultIlOutFormatSame     , 0x0000120e, WSP_RESULT_LEVEL_FATAL );
    WSP_DEFINE_C_RESULT( WSP_ResultIlStackOverflow     , 0x0000120f, WSP_RESULT_LEVEL_FATAL ); 
    WSP_DEFINE_C_RESULT( WSP_ResultIlStackUnderflow    , 0x00001210, WSP_RESULT_LEVEL_FATAL ); 
    WSP_DEFINE_C_RESULT( WSP_ResultIlInvalidConversion , 0x00001211, WSP_RESULT_LEVEL_FATAL ); 
    WSP_DEFINE_C_RESULT( WSP_ResultIlBadDimensions     , 0x00001212, WSP_RESULT_LEVEL_FATAL ); 
    WSP_DEFINE_C_RESULT( WSP_ResultIlFileReadError     , 0x00001213, WSP_RESULT_LEVEL_FATAL );
    WSP_DEFINE_C_RESULT( WSP_ResultIlUnknownError      , 0x000012ff, WSP_RESULT_LEVEL_FATAL );