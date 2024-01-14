/**
 * @file image_result_converter.h
 * @author Jun-ichi Nishikata
 */

#ifndef __WSP_IMAGECORE_IMAGE_RESULT_CONVERTER_H__
#define __WSP_IMAGECORE_IMAGE_RESULT_CONVERTER_H__

#include <wsp/common/_define_commoncore.h>
#include <wsp/common/result.h>

#include <IL/il.h>

#ifdef __cplusplus
extern "C"{
#endif

WSP_DLL_EXPORT WSP_Result WSP_MakeIlResult( ILenum value );

#ifdef __cplusplus
}
#endif

#endif
