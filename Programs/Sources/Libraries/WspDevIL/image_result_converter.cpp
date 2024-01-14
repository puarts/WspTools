/**
 * @file image_result_converter.c
 * 
 */

#include "image_result.h"
#include "image_result_converter.h"

WSP_Result WSP_MakeIlResult( ILenum value )
{
    switch( value )
    {
    case IL_NO_ERROR             : return WSP_ResultIlNoError           ();
    case IL_INVALID_ENUM         : return WSP_ResultIlInvalidEnum       ();
    case IL_OUT_OF_MEMORY        : return WSP_ResultIlOutOfMemory       ();
    case IL_FORMAT_NOT_SUPPORTED : return WSP_ResultIlFormatNotSupported();
    case IL_INTERNAL_ERROR       : return WSP_ResultIlInternalError     ();
    case IL_INVALID_VALUE        : return WSP_ResultIlInvalidValue      ();
    case IL_ILLEGAL_OPERATION    : return WSP_ResultIlIllegalOperation  ();
    case IL_ILLEGAL_FILE_VALUE   : return WSP_ResultIlIllegalFileValue  ();
    case IL_INVALID_FILE_HEADER  : return WSP_ResultIlInvalidFileHeader ();
    case IL_INVALID_PARAM        : return WSP_ResultIlInvalidParam      ();
    case IL_COULD_NOT_OPEN_FILE  : return WSP_ResultIlCouldNotOpenFile  ();
    case IL_INVALID_EXTENSION    : return WSP_ResultIlInvalidExtension  ();
    case IL_FILE_ALREADY_EXISTS  : return WSP_ResultIlFileAlreadyExists ();
    case IL_OUT_FORMAT_SAME      : return WSP_ResultIlOutFormatSame     ();
    case IL_STACK_OVERFLOW       : return WSP_ResultIlStackOverflow     ();
    case IL_STACK_UNDERFLOW      : return WSP_ResultIlStackUnderflow    ();
    case IL_INVALID_CONVERSION   : return WSP_ResultIlInvalidConversion ();
    case IL_BAD_DIMENSIONS       : return WSP_ResultIlBadDimensions     ();
    case IL_FILE_READ_ERROR      : return WSP_ResultIlFileReadError     ();
    default:                       return WSP_ResultIlUnknownError();
    }
}
