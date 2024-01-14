#pragma once


#include <wsp/image/tpl_cl-image.h>

namespace wsp{ namespace img{
    //static wsp::DebugMode dbgmode=WSP_DEBUGMODE_OFF;

    /* =================================
        Filter
       =================================*/

    // ==============================
    //! Resize
    // ==============================
    WSP_DLL_EXPORT wsp::State cvResizeImage(wsp::Image<double> &io_img, u32 newWidth, u32 newHeight, int interpolation=CV_INTER_LINEAR);
    //WSP_DLL_EXPORT wsp::State cvResizeImage(wsp::Image<WspRgb> &io_img, u32 newWidth, u32 newHeight, int interpolation=CV_INTER_LINEAR);

    inline wsp::State cvResizeImage_NearestNeighbor(wsp::Image<double> &io_img, u32 newWidth, u32 newHeight);
    inline wsp::State cvResizeImage_BiLinear(wsp::Image<double> &io_img, u32 newWidth, u32 newHeight);
    inline wsp::State cvResizeImage_Area(wsp::Image<double> &io_img, u32 newWidth, u32 newHeight);
    inline wsp::State cvResizeImage_BiCubic(wsp::Image<double> &io_img, u32 newWidth, u32 newHeight);


    // ==============================
    //! Interpolation 
    // ==============================
    //WSP_DLL_EXPORT wsp::State cvInpaint(wsp::Image<WspRgb> &io_img, int flags);
    //WSP_DLL_EXPORT wsp::State cvInpaint(uchar *io_rgb, u32 width, u32 height, int flags);
    //inline wsp::State cvInpaintNavierStokes(wsp::Image<WspRgb> &io_img);
    //inline wsp::State cvInpaintAlexandruTelea(wsp::Image<WspRgb> &io_img);
    


    WSP_DLL_EXPORT void cvGpuStereoMatching(wsp::Image<double> &o_depth, const char *filenameL, const char *filenameR, int maxdisp, int winsize, int method=0);
    //WSP_DLL_EXPORT void StereoMatchingPOC_OpenCV(IplImage *in_main, IplImage *in_sub, IplImage *o_depth);
    //WSP_DLL_EXPORT void StereoMatchingPOC_OpenCV(IplImage *o_depth, const IplImage *in_main, const IplImage *in_sub);
    //WSP_DLL_EXPORT WspPoint2 FindDisparityPOC_OpenCV(const IplImage *in_reference, const IplImage* in_template);

}}
