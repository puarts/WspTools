/**
 * @file fn-mgproc.h
 * 
 * Definition of functions for image processing.
 */

#ifndef WspImage_WspFnImgProc_H_
#define WspImage_WspFnImgProc_H_

#ifndef __WSP_COMMONCORE_RESULT_H__
#include <wsp/common/result.h>
#endif
#include <wsp/math/_define_mathutil.h>
#include "tpl_cl-image.h"


namespace wsp{ namespace img{


    // ==============================
    //           Generator 
    // ==============================
    WSP_DLL_EXPORT wsp::State GenerateRandomGaussDistribution(
        double *o_img, u32 width, u32 height,
        double offsetMax, u32 repeatNum,
        double scaleMax=1.0, double varianceMin= 50, double varianceMax=255
    );

    // ==============================
    //        Fourie Transform
    // ==============================
    //WSP_DLL_EXPORT int DFT1d(double *o_real, double *o_imag, const double *in_real, const double *in_imag, int dataSize, bool inverse);
    //WSP_DLL_EXPORT int DFT2d(wsp::Image<double> &o_real, wsp::Image<double> &o_imag, const wsp::Image<double> &in_real, const wsp::Image<double> &in_imag, bool inverse);
    //WSP_DLL_EXPORT int FFT1d(double *ar, double *ai, int n, int iter, int flag);
    //WSP_DLL_EXPORT int FFT2d(double *array_real, double *array_imag, int n, int nmax, int flag);
    //WSP_DLL_EXPORT int FFT2d(double **im_real, double **im_imag, int n, int nmax, int flag);


    // ==============================
    //        Segmentation 
    // ==============================
    WSP_DLL_EXPORT wsp::State Segmentation(uchar *o_segm, uchar *in_img, u32 width, u32 height, u32 nChannels, int threshold);
    WSP_DLL_EXPORT wsp::State RemoveUnreliableDepthBySegmentationMap(wsp::Image<double> &io_depth, const wsp::Image<int> &in_segmDepth, const wsp::Image<int> &in_segmCol);


    // ===================================================================
    // ===================================================================
    //**
    //                        Stereo Matching 
    //**
    // ===================================================================
    // ===================================================================
    //***********************************************************
    //* FUNCTION DECLARATION (wsp::img::StereoMatching)
    //! \ingroup WspImage
    //! \brief main function which provides access to every stereo matching functionality.
    WSP_DLL_EXPORT wsp::State StereoMatching(
        const wsp::Image<double> &in_main, 
        const wsp::Image<double> &in_sub, 
        wsp::Image<double> &o_depth, 
        wsp::img::StereoAlgorithm alg,
        const void *stereo_params, 
        const wsp::Image<uchar> *in_mask_main=NULL, 
        const wsp::Image<uchar> *in_mask_sub=NULL
    );
    //***********************************************************
    //* FUNCTION DECLARATION (wsp::img::CrossCheck)
    //! \ingroup WspImage
    WSP_DLL_EXPORT wsp::State CrossCheck(wsp::Image<double> &io_depthMain, wsp::Image<double> &io_depthSub, double fuzziness=0.0, bool show_debug_info=false);

    // Block Matching ---------------------------------------------------
    //! Variable Window Matching
    WSP_DLL_EXPORT wsp::State GetHorSum(wsp::Image<double> &o_sum, const wsp::Image<double> &in_img);
    WSP_DLL_EXPORT wsp::State GetFullSum(wsp::Image<double> &o_sum, const wsp::Image<double> &in_img);

    WSP_DLL_EXPORT wsp::State StereoBMFunctionFitting(
        wsp::Image<double> &o_depth, 
        const wsp::Image<double> &in_mainImg, 
        const wsp::Image<double> &in_subImg, 
        const wsp::img::StereoBMParams &params, 
        const wsp::Image<double> *in_depth_ref=NULL,
        const wsp::Image<u8> *in_mask=NULL
    );

    //***********************************************************
    //* FUNCTION DECLARATION (wsp::img::StereoFastBM_core)
    //! \ingroup WspImage
    /*! 
        @param params.thre_corr Threshold of SSD(0.0 to 0.1, lower SSD is higher correlation)
    */

    WSP_DLL_EXPORT wsp::State StereoFastBM_core(
        wsp::Image<double> &o_depth, 
        const wsp::Image<double> &in_mainImg, 
        const wsp::Image<double> &in_subImg, 
        const wsp::img::StereoBMParams &params, 
        const wsp::Image<double> *in_depth_ref=NULL
    );




    //***********************************************************
    //* FUNCTION DECLARATION (wsp::img::StereoBM)
    //! \ingroup WspImage
    WSP_DLL_EXPORT wsp::State StereoBM(wsp::Image<double> &o_depth, const wsp::Image<uchar> &in_mainImg, const wsp::Image<uchar> &in_subImg, const wsp::img::StereoBMParams &params);
    //WSP_DLL_EXPORT wsp::State StereoBM(wsp::Image<double> &,wsp::Image<unsigned char> const &,              wsp::Image<unsigned char> const &,struct wsp::img::ParametersStereoBM_st const &);
    //***********************************************************
    //* FUNCTION DECLARATION (wsp::img::StereoBM_SSD)
    //! \ingroup WspImage
    inline wsp::State StereoBM_SSD(wsp::Image<double> &o_depth, const wsp::Image<uchar> &in_mainImg, const wsp::Image<uchar> &in_subImg, const wsp::img::StereoBMParams &in_params){
        wsp::img::StereoBMParams params(in_params);
        params.eval_func=WSP_CORRELATION_FUNCTION_SSD; //wsp::math::GetAbsDiff; 
        return StereoBM(o_depth, in_mainImg, in_subImg, params);
    }
    //***********************************************************
    //* FUNCTION DECLARATION (wsp::img::StereoBM_SAD)
    //! \ingroup WspImage
    inline wsp::State StereoBM_SAD(wsp::Image<double> &o_depth, const wsp::Image<uchar> &in_mainImg, const wsp::Image<uchar> &in_subImg, const wsp::img::StereoBMParams &in_params){
        wsp::img::StereoBMParams params(in_params);
        params.eval_func=WSP_CORRELATION_FUNCTION_SAD; //wsp::math::GetSquaredDiff; 
        return StereoBM(o_depth, in_mainImg, in_subImg, params);
    }
    //WSP_DLL_EXPORT wsp::State UpdateCorrelationMap(wsp::Image<double> &o_depth, wsp::Image<double> &io_corr, const wsp::Image<uchar> &in_main, const wsp::Image<uchar> &in_sub, 
    //                                        const wsp::Image<double> &winSizeMap, uchar const *in_mask, 
    //                                        int disp_x, int disp_y, double thre_corr=0.1, double (*eval_func)(double, double)=wsp::math::GetSquaredDiff<double>);

    // Phase-Only Correlation Matching -------------------------------
    //WSP_DLL_EXPORT wsp::State GetAdaptiveWindowSize_POC1DPerLine(wsp::Image<double> &o_winSize, const wsp::Image<WspRgb> &in_img, int winH_min=11, int winH_max=64, int winWidth=32);

    //***********************************************************
    //* FUNCTION DECLARATION (wsp::img::StereoBM_SAD)
    //! \ingroup WspImage
    //WSP_DLL_EXPORT wsp::State StereoSimpleBM_NCC();

    //***********************************************************
    //* FUNCTION DECLARATION (wsp::img::StereoPOC)
    //! \ingroup WspImage
    //! \brief wrapper function of StereoPOC_main which is handling cross checking
    WSP_DLL_EXPORT wsp::State StereoPOC(
        const wsp::Image<double> &in_main, 
        const wsp::Image<double> &in_sub, 
        wsp::Image<double> &o_depth, 
        const wsp::img::StereoPOCParams &params, 
        const wsp::Image<uchar> *in_mask_main=NULL, 
        const wsp::Image<uchar> *in_mask_sub=NULL
    );



    //***********************************************************
    //* FUNCTION DECLARATION (wsp::img::StereoPOC_core)
    //! \ingroup WspImage
    //! \brief Stereo macthing based on Phase-Only Correlation.
    /*!
        in_mask: this can be NULL, or array which has width*height length.
        o_depth: size of this array must be width*height*2.
    */
    WSP_DLL_EXPORT wsp::State StereoPOC_core(
        const double *in_main,
        const double *in_sub,
        double *in_ref_depth,
        double *o_depth, 
        uchar *o_occlusion_map,
        int width, 
        int height, 
        int num_channels,
        const wsp::img::StereoPOCParams &params,
        const uchar *in_mask=NULL,
        uchar masked_value=0
    );

    //***********************************************************
    //* FUNCTION DECLARATION (wsp::img::StereoHierarchicalPOC)
    //! \ingroup WspImage
    WSP_DLL_EXPORT wsp::State StereoHierarchicalPOC(
        const double *in_main,
        const double *in_sub,
        const uchar *in_mask,
        double *o_depth, 
        uchar *o_occlusion_map,
        int width, 
        int height, 
        int num_channels,
        const wsp::img::StereoPOCParams &params
    );
    //***********************************************************
    //* FUNCTION DECLARATION (wsp::img::StereoBMBasedPOC)
    //! \ingroup WspImage
    WSP_DLL_EXPORT wsp::State StereoBMBasedPOC(
        const double *in_main,
        const double *in_sub,
        const uchar *in_mask,
        double *o_depth, 
        uchar *o_occlusion_map,
        int width, 
        int height, 
        int num_channels,
        const wsp::img::StereoPOCParams &params
    );

    //***********************************************************
    //* FUNCTION DECLARATION (wsp::img::StereoBMBasedSegmentPOC)
    //! \ingroup WspImage
    //WSP_DLL_EXPORT wsp::State StereoBMBasedFrontOnlyPOC(
    //    const double *in_main,
    //    const double *in_sub,
    //    const uchar *in_mask,
    //    double *o_depth, 
    //    uchar *o_occlusion_map,
    //    int width, 
    //    int height, 
    //    int num_channels,
    //    const wsp::img::StereoPOCParams &params
    //);

    //***********************************************************
    //* FUNCTION DECLARATION (wsp::img::FindDisparity1DPOC)
    //! \ingroup WspImage
    //! \brief Function which can find disparity with 2D POC
    /*!
        width and height must be power of two
    */
    WSP_DLL_EXPORT double FindDisparity1DPOC(
            complex64 *io_reference, 
            complex64 *io_template, 
            int width, int height, 
            double thre_poc_peak=0.1, 
            double low_pass_ratio=1.0,
            int exponent_x=-1,
            complex64 *tmp_poc=NULL,
            complex64 *tmp_avearge_poc=NULL,
            int sample_pix_distance=1,
            int sample_range=4
    );
    //***********************************************************
    //* FUNCTION DECLARATION (wsp::img::FindDisparity2DPOC)
    //! \ingroup WspImage
    //! \brief Function which can find disparity with 2D POC
    /*!
        width and height must be power of two
    */
    WSP_DLL_EXPORT wsp::Result FindDisparity2DPOC(
            complex64 *io_reference, 
            complex64 *io_template, 
            int width, int height,
            double &o_deltax, double &o_deltay, 
            double thre_pocPeak=0.1, 
            double low_pass_ratio=1.0,
            int exponent_x=-1, 
            int exponent_y=-1,
            complex64 *tmp_poc=NULL, 
            complex64 *tmp_data=NULL,
            int sample_pix_distance=1,
            int sample_range=4
    );


}}




#endif