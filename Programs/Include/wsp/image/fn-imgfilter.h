// @file WspFnImgFilter.h
// 

#ifndef WspImage_WspFnImgFilter_H_
#define WspImage_WspFnImgFilter_H_

#include "tpl_cl-image.h"

namespace wsp{
namespace img{
    // ==============================
    // Smoothing Filters
    // ==============================
    void GaussianFilterSimpleRecursive(
        u32 *o_dst, 
        u32 *in_src, 
        u32 width, 
        u32 height, 
        float a
    );
    //WSP_DLL_EXPORT void MedianRgb(wsp::Image<WspRgb> &io_img, u32 block_size);

    // ==============================
    // Edge Detection
    // ==============================
    //WSP_DLL_EXPORT wsp::State LaplacianFilter(wsp::Image<double> &o_edge, const wsp::Image<WspRgb> &in_img);
    WSP_DLL_EXPORT wsp::State LaplacianFilter(wsp::Image<double> &o_edge, const wsp::Image<uchar> &in_img);
    WSP_DLL_EXPORT wsp::State SobelFilter(double *o_edge, const uchar *in_img, u32 width, u32 height, u32 num_of_channels);
    inline wsp::State SobelFilter(wsp::Image<double> &o_edge, const wsp::Image<uchar> &in_img);
}
}

//! Edge Detection ---------------------------------------------------
inline wsp::State wsp::img::SobelFilter(wsp::Image<double> &o_edge, const wsp::Image<uchar> &in_img){
    if(in_img.num_of_channels()!=3){ WSP_COMMON_ERROR_LOG("Num of channels must be 3\n"); return WSP_STATE_FAILURE; }
    o_edge.SetSize(in_img.width(), in_img.height(), 1);
    return wsp::img::SobelFilter(o_edge.data(), in_img.data(), in_img.width(), in_img.height(), in_img.num_of_channels());
}



#endif