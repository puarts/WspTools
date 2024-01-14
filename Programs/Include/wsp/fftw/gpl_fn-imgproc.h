#pragma once

#include <wsp/image/tpl_cl-image.h>

//class WspIndexImg;

namespace wsp{ namespace img{

    //! Phase-Only Correlation Matching -------------------------------
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

}}
