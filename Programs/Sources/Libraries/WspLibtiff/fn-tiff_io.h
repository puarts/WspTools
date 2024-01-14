#pragma once

#ifndef __WSP_IMAGEUTIL_DEFINE_IMAGEUTIL_H__
#include <wsp/common/_define_commoncore.h>
#endif

#include <wsp/common/common_types.h>

namespace wsp{ namespace libtiff{

    // Any Type of Image I/O ----------------------------------
    // Tiff I/O
    // pixel structure [R, G, B, A]
    WSP_DLL_EXPORT void LoadTiff32(u8 *O_image, u32 &o_width, u32 &o_height, const char *filename);

    WSP_DLL_EXPORT wsp::State SaveAsTiff8(const uchar *in_tiffImg, const uchar *in_alphaData, u32 width, u32 height, const char *filename);
    WSP_DLL_EXPORT wsp::State SaveAsTiff24(const uchar *in_tiffImg, const uchar *in_alphaData, u32 width, u32 height, const char *filename);
    WSP_DLL_EXPORT wsp::State SaveAsTiff32(const uchar *in_tiffImg, u32 width, u32 height, const char *filename);

    inline wsp::State SaveAsTiff8(const uchar *in_tiffImg, u32 width, u32 height, const char *filename);
    inline wsp::State SaveAsTiff24(const uchar *in_tiffImg, u32 width, u32 height, const char *filename);
}
}

// Tiff I/O --------------------------------------------------------------------

inline wsp::State wsp::libtiff::SaveAsTiff8(const uchar *in_img, u32 width, u32 height, const char *filename){
    return wsp::libtiff::SaveAsTiff8(in_img, NULL, width, height, filename);
}

inline wsp::State wsp::libtiff::SaveAsTiff24(const uchar *in_img, u32 width, u32 height, const char *filename){
    return wsp::libtiff::SaveAsTiff24(in_img, NULL, width, height, filename);
}


