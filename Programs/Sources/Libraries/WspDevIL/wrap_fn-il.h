#pragma once

#include "image_result.h"
#include <wsp/common/_define_commoncore.h>

#ifdef __cplusplus
extern "C"{
#endif

WSP_DLL_EXPORT WSP_Result WSP_IlLoadImage(
    u8 **o_image,
    int *o_width, int *o_height, int *o_num_channels, 
    const char *filename
);

WSP_DLL_EXPORT WSP_Result WSP_IlLoadImageAsU8WithAlpha(
    u8 **o_image, u8 **o_alpha, 
    int *o_width, int *o_height, int *o_num_channels,
    const char *filename
);

WSP_DLL_EXPORT void WSP_IlFreeImage(u8 **io_image);

WSP_DLL_EXPORT WSP_Result WSP_IlSaveImage(
    const u8 *in_image, 
    int width, int height, int num_channels,
    const char *filename
);

#ifdef __cplusplus
}
#endif
