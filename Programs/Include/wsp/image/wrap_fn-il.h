#pragma once

#include "image_result.h"
#include <wsp/common/_define_commoncore.h>
#include <IL/il.h>

#ifdef __cplusplus
extern "C"{
#endif


WSP_DLL_EXPORT char* WSP_IlGetErrorMessage(ILenum errCode);

WSP_DLL_EXPORT WSP_Result WSP_IlLoadImageAsU8(
    u8 **o_rgba, 
    u32 *o_width, u32 *o_height, u32 *o_nChannel, 
    const char *filename
);
WSP_DLL_EXPORT WSP_Result WSP_IlLoadImageAsU8WithAlpha(
    u8 **o_rgb, u8 **o_alpha, 
    u32 *o_width, u32 *o_height, u32 *o_nChannel, 
    const char *filename
);
WSP_DLL_EXPORT WSP_Result WSP_IlSaveRGBAImage(
    const u8 *in_rgba, 
    u32 width, u32 height, 
    const char *filename
);
WSP_DLL_EXPORT WSP_Result WSP_IlSaveLuminanceImage(
    const u8 *in_luminance, 
    u32 width, u32 height, 
    const char *filename
);

#ifdef __cplusplus
}
#endif
