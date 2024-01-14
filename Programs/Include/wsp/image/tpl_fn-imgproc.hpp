/**
 * @file tpl_fn-imgproc.hpp
 * 
 */

#ifndef WspImage_WspFnImgProc_tpl_HPP_
#define WspImage_WspFnImgProc_tpl_HPP_

#include <wsp/common/_tpl_cl-stackarray.h>
//#include <wsp/math/_tpl_fn-math.h>
#include <wsp/math/_define_mathutil.h>
#include <wsp/math/tpl_fn-basic_math.h>

#include "tpl_cl-image.h"
#include "tpl_cl-histogram.h"

#ifdef USE_QT
    #include <QtGui/QtGui>
#endif


//BEGIN_WSP_IMG_NAMESPACE
namespace wsp{ namespace img{
    struct SutcliffePolygonParam{
        u32 max_levels;
        u32 num_sides;
        u32 point_size;
        float strut_factor;
    };
#ifdef USE_QT
    template<typename T> 
    void DrawSutcliffePolygons(
        wsp::Image<T> &draw_image, 
        const struct wsp::img::SutcliffePolygonParam &param );
#endif
    /* ======================================
                     Masking
       ======================================*/
    template<typename T> struct CharacteristicMaskParam{
        double threshold_of_block_average;
        u32 block_size;
        double (*eval_func)(double, double);
        T mask_one_value;
        T mask_zero_value;
        CharacteristicMaskParam(){
            threshold_of_block_average = 30;
            block_size = 3;
            mask_one_value = static_cast<T>(1);
            mask_zero_value = static_cast<T>(0);
            eval_func = wsp::math::GetAbsDiff<double>;
        }
    };

    template<typename T, typename U> 
    wsp::State GetCharacteristicMask(
        T *o_mask, 
        const U *in_src, 
        const u32 *in_block_size_map, 
        u32 width, u32 height, u32 num_of_channels,
        double threshold_of_block_average, 
        T one_value, 
        T zero_value, 
        double (*eval_func)(double, double)=wsp::math::GetAbsDiff<double>
    );

    template<typename T, typename U> 
    inline wsp::State GetCharacteristicMask(
        T *o_mask, const U *in_src, u32 width, u32 height, u32 num_of_channels, wsp::img::CharacteristicMaskParam<T> &params);

    template<typename T, typename U> 
    inline wsp::State GetCharacteristicMask(
        wsp::Image<T> &o_mask, const wsp::Image<U> &in_src, const wsp::Image<u32> &in_block_size_map,
        double threshold_of_block_average, T one_value, T zero_value, double (*eval_func)(double, double)=wsp::math::GetAbsDiff<double>);

    template<typename T, typename U> 
    inline wsp::State GetCharacteristicMask(
        wsp::Image<T> &o_mask, const wsp::Image<U> &in_src, wsp::img::CharacteristicMaskParam<T> &params);


    /* ======================================
                  Pixel Operation
       ======================================*/
    template<typename T> 
    wsp::State ShiftPixelsByDepth(
        wsp::Image<T> &o_dest, 
        const wsp::Image<T> &in_src, 
        const wsp::Image<double> &in_depth
    );
    
    template<typename T> 
    wsp::State ShiftPixelsByDepth_SubpixelLevel(
        wsp::Image<T> &o_dest, 
        wsp::Image<double> *o_depth,
        const wsp::Image<T> &in_src,
        const wsp::Image<double> &in_depth, 
        T dest_min=(T)0, 
        T dest_max=(T)255
    );

    template<typename T> 
    wsp::State NormalizePixels(
        wsp::Image<T> &io_img, 
        T min, T max
    );
    
    template<typename T> 
    wsp::State ScaleUpByAlpha(
        wsp::Image<T> &io_img, 
        u32 alphaThreshold=30, 
        double mult=1.0
    );
    
    template<typename T> 
    inline void FillupSpecifiedStateWithAnyColor(
        T *io_data, 
        const uchar *in_state_data, 
        u32 width, 
        u32 height, 
        uchar red_state, 
        uchar fill_r=255, 
        uchar fill_g=0, 
        uchar fill_b=0
    );
    
    //template<typename T> void CutOff(T *io_im, u32 width, u32 height, u32 numOfChannels, T min, T max, T fill_min, T fill_max);
    template<typename T> 
    inline void Binalize(
        wsp::Image<T> &io_img, 
        T threshold=128, T min=0, T max=255
    );
    
    template<typename T> 
    inline void BinalizeAlpha(
        wsp::Image<T> &io_img, uchar threshold=128
    );
    
    template<typename T> 
    wsp::State RemoveHighVariancePixels(
        wsp::Image<T> &io_img, 
        const wsp::Image<T> &in_av_img, 
        double threshold_sd
    );

    template<typename _InType, typename _OutType>  
    inline wsp::State GetIntensityMap(
        const wsp::Image<_InType> &in_rgb, 
        wsp::Image<_OutType> &o_intensity
    );

    template<typename _InType, typename _OutType>  
    inline wsp::State GetChromaMap(
        const wsp::Image<_InType> &in_rgb, 
        wsp::Image<_OutType> &o_chroma_rgby,
        double plune_intensity_ratio = 0.1
    );

    /* ======================================
                  Segmentation
       ======================================*/
    //***********************************************************
    //* FUNCTION DECLARATION (wsp::img::SetAverageForSegment)
    //! \ingroup WspImage
    template<typename T> wsp::State SetAverageForSegment(wsp::Image<T> &io_img, const wsp::Image<int> &in_segm, uchar thre_alpha=255);
    
    //***********************************************************
    //* FUNCTION DECLARATION (wsp::img::SegmentationByHistogram)
    //! \ingroup WspImage
    //! \brief a function witch separate area by histogram valley
    /*!
        return number of segments
    */
    template<typename InType, typename OutType> 
    int SegmentationByHistogram(
        const InType *in_image, 
        OutType *o_segm, 
        int length, int num_channels, 
        int thre_pix_amount,
        InType histogram_min, InType histogram_max
    );

    template<typename InType, typename OutType> 
    int SegmentationByHistogram(
        const InType *in_image, 
        OutType *o_segm, 
        int length, int num_channels, 
        int thre_pix_amount
    ){
        if(num_channels!=1){
            WSP_COMMON_ERROR_LOG("Number of channels must be 1\n"); return 0;
        }
        InType min, max;
        int npix = length * num_channels;
           wsp::GetMinMax(in_image, min, max, npix);
        return wsp::img::SegmentationByHistogram( in_image, o_segm, length, num_channels, thre_pix_amount, min, max );
    }

    template<typename InType, typename OutType> 
    int SegmentationByHistogram(
        const InType *in_image, 
        OutType *o_segm, 
        int length, int num_channels
    ){
        if(num_channels!=1){
            WSP_COMMON_ERROR_LOG("Number of channels must be 1\n"); return 0;
        }
        InType min, max;
        int npix = length * num_channels;
           wsp::GetMinMax( in_image, min, max, npix );

        InType average = wsp::GetAverage( in_image, npix );
        int thre_pix_amount = (int)average/5;

        return wsp::img::SegmentationByHistogram( in_image, o_segm, length, num_channels, thre_pix_amount, min, max );
    }

    //***********************************************************
    //* FUNCTION DECLARATION (wsp::img::SegmentationByHistogramAverageBase)
    //! \ingroup WspImage
    template<typename InType, typename OutType> 
    inline int SegmentationByHistogramAverageBase(
        const InType *in_image, 
        OutType *o_segm, 
        int length, int num_channels, 
        double bg_estim_thre_pixel_amount_ratio
    ){
        if(num_channels!=1){ WSP_COMMON_ERROR_LOG("number of channels must be 1\n"); return WSP_STATE_INVALID_PARAMETER; }

        // Get average
        InType min, max;
        wsp::GetMinMax(in_image, min, max, length);
        int average_pix = length*num_channels/(max-min);
        return SegmentationByHistogram(
            in_image, o_segm, length, num_channels, 
            average_pix * bg_estim_thre_pixel_amount_ratio, 
            min, max
        );
    }

    //***********************************************************
    //* FUNCTION DECLARATION (wsp::img::DetermineThresholdOfSegmHistogram)
    //! \ingroup WspImage
    //! \brief determine appropriate threshold of SegmentationByHistogram
    /*!
        return threshold of pixel amount
    */
    template<typename T>
    int DetermineThresholdOfSegmHistogram(
        const wsp::Histogram<T> &in_histogram, 
        int min_num_segm, 
        int max_num_segm,
        double min_num_pix_ratio,
        double max_num_pix_ratio,
        int num_loop
    );


    /* ======================================
                   Interpolation
       ======================================*/


    template<typename T> 
    wsp::State FillUpTimes(
        wsp::Image<T> &io_img, 
        const wsp::Image<uchar> *in_mask=NULL, 
        wsp::img::PixelState target_state=WSP_PIXEL_STATE_OCCLUDED, 
        int init_blocksize=1, 
        int blocksize_step=1, 
        double variance=12.96);
    
    template<typename T> 
    wsp::State FillUpTimesWithinSegment(
        wsp::Image<T> &io_img, 
        const wsp::Image<int> &in_segm, 
        wsp::img::PixelState target_state=WSP_PIXEL_STATE_OCCLUDED, 
        int init_blocksize=1);

    template<typename T> 
    inline int FillUpGaussianAverageState(
        wsp::Image<T> &io_img, 
        const wsp::Image<uchar> *in_mask, 
        wsp::img::PixelState target_state, 
        int blocksize=3, 
        double variance=12.96, 
        bool use_alpha_weight=false);

    template<typename T> 
    inline int FillUpGaussianAverageAlpha(
        wsp::Image<T> &io_img, 
        const wsp::Image<uchar> *in_mask, 
        int blocksize=3, 
        double variance=12.96, 
        bool use_alpha_weight=false);


    //! threhold is continueous pixels to decide which pixel is inner occlusion 
    template<typename T> int FindInnerOcclusion(
        wsp::Image<T> &io_img,     
        int max_continuous_occlusions_hor, 
        int max_continuous_occlusions_ver
    );

    template<typename T> int FindInnerOcclusionByAlpha(
        wsp::Image<T> &io_img, 
        int max_continuous_alpha_hor, 
        int max_continuous_alpha_ver, 
        uchar thre_alpha=128
    );

    /* ======================================
                      Other
       ======================================*/

    template<typename T> 
    wsp::State FitIntensity_BinarySearch(
        wsp::Image<T> &io_img, 
        const wsp::Image<T> &in_comp_img, 
        double min_scale=0.1, 
        double max_scale=3.0, 
        double (*eval_func)(double, double)=wsp::math::GetSquaredDiff
    );
    //template<typename T> wsp::State FitIntensity_StepSearch(wsp::Image<T> &io_img, const wsp::Image<T> &in_comp_img, double min_scale=0.1, double max_scale=3.0, double search_step=0.05, double (*eval_func)(double, double)=wsp::math::GetSquaredDiff);

    template<typename T>
    wsp::State GetSaliencyMap(
        const wsp::Image<T> &in_src,
        wsp::Image<T> &o_saliency_map
    );
}}
//END_WSP_IMG_NAMESPACE



#include "tpl_fn-imgproc.inl"


#endif