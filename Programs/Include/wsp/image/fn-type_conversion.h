/**
 * @file fn-type_conversion.h
 * @author 
 */

#ifndef __WSP_IMAGECORE_FUNCTION_TYPE_CONVERSION_H__
#define __WSP_IMAGECORE_FUNCTION_TYPE_CONVERSION_H__

#include "_image_types.h"

#ifdef __cplusplus
extern "C"{
#endif

WSP_DLL_EXPORT WSP_ImageFormat WSP_GetImageFormat( const char *filename );
WSP_DLL_EXPORT WSP_ImageFormat WSP_GetImageFormatID(const char *extension);

#ifdef __cplusplus
}
#endif


#endif
