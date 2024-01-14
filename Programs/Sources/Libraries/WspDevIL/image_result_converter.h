/**
 * @file image_result_converter.h
 * 
 */

#pragma once

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

