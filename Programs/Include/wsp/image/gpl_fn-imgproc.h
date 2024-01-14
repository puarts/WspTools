/**
 * @file fn-imgproc-gpl.h
 * @author Junichi Nishikata
 */


#ifndef WspImage_WspFnImgProcGpl_H_
#define WspImage_WspFnImgProcGpl_H_

#include "tpl_cl-image.h"

//class WspIndexImg;

namespace wsp{ namespace img{
    //static wsp::DebugMode dbgmode=WSP_DEBUGMODE_OFF;

    /* =================================
        Filter
       =================================*/

    // ==============================
    //! Resize
    // ==============================
    #ifdef USE_OPENCV
    WSP_DLL_EXPORT wsp::State cvResizeImage(wsp::Image<double> &io_img, u32 newWidth, u32 newHeight, int interpolation=CV_INTER_LINEAR);
    //WSP_DLL_EXPORT wsp::State cvResizeImage(wsp::Image<WspRgb> &io_img, u32 newWidth, u32 newHeight, int interpolation=CV_INTER_LINEAR);

    inline wsp::State cvResizeImage_NearestNeighbor(wsp::Image<double> &io_img, u32 newWidth, u32 newHeight);
    inline wsp::State cvResizeImage_BiLinear(wsp::Image<double> &io_img, u32 newWidth, u32 newHeight);
    inline wsp::State cvResizeImage_Area(wsp::Image<double> &io_img, u32 newWidth, u32 newHeight);
    inline wsp::State cvResizeImage_BiCubic(wsp::Image<double> &io_img, u32 newWidth, u32 newHeight);
    #endif

    // ==============================
    //! Type Conversion
    // ==============================

    #ifdef USE_QT
    //WSP_DLL_EXPORT wsp::State qtQImage2RgbImg(wsp::Image<WspRgb> &o_rgba, const QImage &in_rgba, const QImage *in_alpha);
    //WSP_DLL_EXPORT wsp::State qtRgbImgToQImage(QImage &o_rgb, QImage *o_alpha, const wsp::Image<WspRgb> &in_rgba);
    #endif

    // ==============================
    //! Generator 
    // ==============================

    // ==============================
    //! Pixel Operation
    // ==============================

    // ==============================
    //! Occlusion Processing 
    // ==============================

    // ==============================
    //! Interpolation 
    // ==============================
    #ifdef USE_OPENCV
    //WSP_DLL_EXPORT wsp::State cvInpaint(wsp::Image<WspRgb> &io_img, int flags);
    //WSP_DLL_EXPORT wsp::State cvInpaint(uchar *io_rgb, u32 width, u32 height, int flags);
    //inline wsp::State cvInpaintNavierStokes(wsp::Image<WspRgb> &io_img);
    //inline wsp::State cvInpaintAlexandruTelea(wsp::Image<WspRgb> &io_img);
    #ifdef USE_QT
    //inline wsp::State cvInpaintDialog(wsp::Image<WspRgb> &io_img);
    #endif
    #endif
    



    // ==============================
    //! Fourie Transform
    // ==============================



    // ==============================
    //! Correlation
    // ==============================

    //! ======================================
    //!            Stereo Matching 
    //! ======================================

    //! Block Matching -----------------------------------

    //! Phase-Only Correlation Matching -------------------------------
    #ifdef USE_FFTW
    //! Any Channels 1D per line Hierarchical
    WSP_DLL_EXPORT wsp::State StereoPOC_main(
        wsp::Image<double> &o_depth, 
        const wsp::Image<uchar> &in_main, 
        const wsp::Image<uchar> &in_sub, 
        const wsp::img::StereoPOCParams &params, 
        const wsp::Image<uchar> *in_mask=NULL
    );

    WSP_DLL_EXPORT wsp::State StereoPOC_fftw(
        const wsp::Image<uchar> &in_main, 
        const wsp::Image<uchar> &in_sub, 
        wsp::Image<double> &o_depth, 
        const wsp::img::StereoPOCParams &params, 
        const wsp::Image<uchar> *in_mask_main=NULL, 
        const wsp::Image<uchar> *in_mask_sub=NULL
    );

    //! RGB 1D per line Hierarchical
    //WSP_DLL_EXPORT wsp::State StereoMatchingPOC_1DPerLineRgb_Hierarchical(wsp::Image<double> &o_depth, const wsp::Image<WspRgb> &in_main, const wsp::Image<WspRgb> &in_sub, double minDisp, double maxDisp, u32 blockWidth=32, u32 blockHeight=11,
    //                    u32 searchDepth=2, double thre_meanStdDev=30.0, double thre_pocPeak=0.1, double low_pass_ratio=0.75);
    WSP_DLL_EXPORT double FindDisparity1DPOC_fftw(
        const double *in_reference, const double *in_template, 
        int width, int height, 
        double minDisp, double maxDisp, 
        double thre_pocPeak=0.1, double low_pass_ratio=1.0
    );

    //! \ingroup WspImage
    //! \brief Function which can find disparity with 2D POC
    WSP_DLL_EXPORT wsp::State FindDisparity2DPOC(
        double &o_deltax, double &o_deltay, 
        wsp::Image<double> *o_poc, 
        const wsp::Image<double> &in_reference, 
        const wsp::Image<double> &in_template, 
        //double minDisp, double maxDisp, 
        double thre_pocPeak=0.1, double low_pass_ratio=1.0
    );
    // ==============================
    //         Correlation
    // ==============================
    WSP_DLL_EXPORT wsp::State GetPhaseCorrelationFFT2D(wsp::Image<double> &o_poc, const wsp::Image<double> &in_ref, const wsp::Image<double> &in_tpl, double low_pass_ratio=1.0);
    WSP_DLL_EXPORT wsp::State GetPhaseCorrelationFFT2DPerLine(wsp::Image<double> &o_poc, const wsp::Image<double> &in_ref, const wsp::Image<double> &in_tpl, double low_pass_ratio=1.0);

    //wsp::State GetPhaseCorrelationFFT1DPerLine3(double *o_poc[3], double *o_pocAv[3], double * const in_ref[3], double * const in_tpl[3], int width, int height, double low_pass_ratio=1.0);
    //wsp::State GetPhaseCorrelationFFT1DPerLine3(double *o_poc[3], double *o_pocAv[3], const double *in_ref[3], const double *in_tpl[3], int width, int height, double low_pass_ratio=1.0);
    WSP_DLL_EXPORT wsp::State GetPhaseCorrelationDFT2DPerLine(double *o_poc, const double *in_ref, const double *in_tpl, int width, int height, double low_pass_ratio = 1.0);

    //! these are wrapper functions for GetPhaseCorrelationDFT functions, resizing input images as size of power of two
    inline wsp::State GetPhaseCorrelationFFT1DPerLine(double *o_poc, double *o_pocAv, const double *in_ref, const double *in_tpl, int width, int height, double low_pass_ratio = 1.0);
#endif

    #ifdef USE_OPENCV
    WSP_DLL_EXPORT void cvGpuStereoMatching(wsp::Image<double> &o_depth, const char *filenameL, const char *filenameR, int maxdisp, int winsize, int method=0);
    //WSP_DLL_EXPORT void StereoMatchingPOC_OpenCV(IplImage *in_main, IplImage *in_sub, IplImage *o_depth);
    //WSP_DLL_EXPORT void StereoMatchingPOC_OpenCV(IplImage *o_depth, const IplImage *in_main, const IplImage *in_sub);
    //WSP_DLL_EXPORT WspPoint2 FindDisparityPOC_OpenCV(const IplImage *in_reference, const IplImage* in_template);
    #endif

}}
#endif