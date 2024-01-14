// @file fn-imgio-gpl.h
// @author Junichi Nishikata
// WspImage format input and output by using GPL library

#ifndef _WSP_FN_IMG_IO_GPL_H_
#define _WSP_FN_IMG_IO_GPL_H_

#ifndef __WSP_IMAGEUTIL_DEFINE_IMAGEUTIL_H__
#include "_define_image.h"
#endif

namespace wsp{ namespace img{
    static wsp::DebugMode dbgmode = WSP_DEBUGMODE_OFF;

    // Any Type of Image I/O ----------------------------------
    // Tiff I/O
    // pixel structure [R, G, B, A]
    #ifdef USE_LIBTIFF
    WSP_DLL_EXPORT wsp::img::ImgPtr8U LoadTiff32(u32 &o_width, u32 &o_height, const char *filename);
    WSP_DLL_EXPORT wsp::State SaveAsTiff32(const uchar *in_tiffImg, u32 width, u32 height, const char *filename);
    inline wsp::State SaveAsTiff8(const uchar *in_tiffImg, u32 width, u32 height, const char *filename);
    inline wsp::State SaveAsTiff8(const uchar *in_tiffImg, const uchar *in_alphaData, u32 width, u32 height, const char *filename);
    inline wsp::State SaveAsTiff24(const uchar *in_tiffImg, u32 width, u32 height, const char *filename);
    inline wsp::State SaveAsTiff24(const uchar *in_tiffImg, const uchar *in_alphaData, u32 width, u32 height, const char *filename);
    #endif

}}


// Any Type of Image I/O -------------------------------------------------------



// Tiff I/O --------------------------------------------------------------------
//inline void wsp::img::SaveAsTiff(const uchar *ary, u32 width, u32 height, const char *filename){
//    wsp::Image<uchar> img(ary, width, height);
//    SaveAsTiff(img, filename);
//}
#ifdef USE_LIBTIFF
inline wsp::State wsp::img::SaveAsTiff8(const uchar *in_img, u32 width, u32 height, const char *filename){
    return wsp::img::SaveAsTiff8(in_img, NULL, width, height, filename);
}
inline wsp::State wsp::img::SaveAsTiff8(const uchar *in_img, const uchar *in_alphaData, u32 width, u32 height, const char *filename){
    u32 dataLen = height*width;
    wsp::State st;
    uchar *tmp_imgRgba = new uchar[dataLen * 4];
    MergeChannels(tmp_imgRgba, 4, in_img, 1, in_alphaData, width, height);    //! Convert RGB to RGBA
    st = wsp::img::SaveAsTiff32(tmp_imgRgba, width, height, filename);
    if(st==WSP_STATE_FAILURE){ delete[] tmp_imgRgba; return WSP_STATE_FAILURE; }
    delete[] tmp_imgRgba;
    return WSP_STATE_SUCCESS;
}
inline wsp::State wsp::img::SaveAsTiff24(const uchar *in_img, u32 width, u32 height, const char *filename){
    return wsp::img::SaveAsTiff24(in_img, NULL, width, height, filename);
}
inline wsp::State wsp::img::SaveAsTiff24(const uchar *in_img, const uchar *in_alphaData, u32 width, u32 height, const char *filename){
    u32 dataLen = height*width;
    wsp::State st;
    uchar *tmp_imgRgba = new uchar[dataLen * 4];
    MergeChannels(tmp_imgRgba, 4, in_img, 3, in_alphaData, width, height);    //! Convert RGB to RGBA
    st = wsp::img::SaveAsTiff32(tmp_imgRgba, width, height, filename);
    if(st==WSP_STATE_FAILURE){ delete[] tmp_imgRgba; return WSP_STATE_FAILURE; }
    delete[] tmp_imgRgba;
    return WSP_STATE_SUCCESS;
}
#endif

#endif