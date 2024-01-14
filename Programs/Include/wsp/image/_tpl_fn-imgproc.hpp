/**
 * @file _tpl_fn-imgproc.hpp
 * 
 * @description Base template functions for image processing
 */

#ifndef _WSP_TEMPLATE_BASE_FN_IMG_PROC_HPP_
#define _WSP_TEMPLATE_BASE_FN_IMG_PROC_HPP_

#ifndef __WSP_COMMONCORE_RESULT_H__
#include <wsp/common/result.h>
#endif
#ifndef __WSP_IMAGEUTIL_DEFINE_IMAGEUTIL_H__
#include "_define_image.h"
#endif

#include <wsp/math/tpl_fn-basic_math.h>

namespace wsp{ namespace img{

    template<typename _Type>
    void SwapQuadrantHorizontally(
        _Type *io_img, u32 width, u32 height, u32 num_of_channels);

    template<typename _Type>
    void SwapQuadrantVertically(
        _Type *io_img, u32 width, u32 height, u32 num_of_channels);

    template<typename _Type>
    void GenerateGrayGradientTexture(
        _Type *image_ptr, s32 width, s32 height, s32 num_channels, _Type max_value=255 );

    // ========================================================
    //                   Pixel Operation
    // ========================================================

    /**
     * @param io_bottom_img The data of base image
     * @param btm_img_w     Width of io_bottom_img
     * @param btm_img_h     Height of io_bottom_img
     * @param in_top_img    The data of image to place on io_bottom_img
     * @param top_img_w     Width of in_top_img
     * @param top_img_h     Height of in_top_img
     * @param center_x      X coordinate of center pos32 for in_top_img
     * @param center_y      Y coordinate of center pos32 for in_top_img
     * @param place_x       X coordinate to place in_top_img on io_bottom_img
     * @param place_y       Y coordinate to place in_top_img on io_bottom_img
     */
    template<typename _TopImgType, typename _BottomImgType>
    void PlaceImageOnImage(
        _BottomImgType *io_bottom_img,
        s32 btm_img_w, s32 btm_img_h,
        const _TopImgType *in_top_img,
        s32 top_img_w, s32 top_img_h, s32 num_channels,
        s32 center_x, s32 center_y,
        s32 place_x, s32 place_y,
        bool use_last_channel_as_alpha = false
    );

    /**
     * @brief Crop user domain from user domain map.
     * @param[out] o_left_x           croped image position x.
     * @param[out] o_top_y            croped image position y.
     * @param[out] o_height           croped image height.
     * @param[out] o_width            croped image width.
     * @param[in]  in_user_domain_map number of channels must be one.
     * @param[in]  use_alpha_mask     put alpha mask data on last channel of o_image.
     */
    template<typename _UserDomainMapType, typename _SrcColorType, typename _OutType>
    void ExtractUserDomain(
        _OutType *o_image,
        s32 *o_left_x, s32 *o_top_y, 
        s32 *o_width, s32 *o_height,
        const _UserDomainMapType *in_user_domain_map,
        const _SrcColorType *in_src_color_map,
        s32 width, s32 height,
        s32 num_channels
    );

    template<typename InType, typename OutType> 
    void FlipImageHorizontally(
        const InType *in_img, 
        OutType *o_img, 
        s32 width, s32 height, s32 num_channels
    );

    template<typename InType, typename OutType>
    void FlipImageVertically(
        const InType *in_img,
        OutType *o_img,
        s32 width, s32 height, s32 num_channels
        );

    template<typename InType, typename OutType> 
    void ShiftImageVer(
        const InType *in_img, 
        OutType *o_img, 
        s32 width, s32 height, s32 num_channels, 
        s32 shift_pixels
    );

    template<typename InType, typename OutType> 
    void ShiftImageHor(
        const InType *in_img, 
        OutType *o_img, 
        s32 width, s32 height, s32 num_channels, 
        s32 shift_pixels
    );

    template<typename T> 
    inline void ShiftImage(
        T *io_img, 
        s32 width, 
        s32 height, 
        s32 num_channels, 
        s32 shift_pixels_hor, 
        s32 shift_pixels_ver
    ){
        T *tmp_img = (T*)malloc(sizeof(T)*width*height*num_channels);
        ShiftImageHor(io_img, tmp_img, width, height, num_channels, shift_pixels_hor);
        ShiftImageVer(tmp_img, io_img, width, height, num_channels, shift_pixels_ver);
        free(tmp_img);
    }

    template<typename InType, typename OutType>
    void CropImage(
        const InType *in_img, 
        OutType *o_img, 
        s32 width, s32 height, s32 num_channels, 
        s32 crop_x, s32 crop_y, 
        s32 crop_width, s32 crop_height
    );

    /**
     * @param in_img Size of image is width * height * 3 * sizeof(T), and byte order must be RGB
     */
    template<typename T>
    void ChromaKeyRgb(
        const T *in_img,
        u8 *o_mask,
        s32 width, s32 height,
        s32 r_min, s32 r_max,
        s32 g_min, s32 g_max,
        s32 b_min, s32 b_max
    );

    // ========================================================
    //                   Channel Operation
    // ========================================================
    //! \ingroup WspImage
    //! \brief Copy a channel from one image to one image
    template<typename InType, typename OutType> 
    inline wsp::State CopyOneChannel(
        const InType *in_src, const uchar *in_alpha, OutType *o_dst, 
        s32 length, s32 src_num_channels, s32 dst_num_channels, 
        s32 target_src_channel_index, s32 target_dst_channel_index, bool mask_alpha=false, bool normalize=false
    );

    //! \ingroup WspImage
    //! \brief Copy RGB channels from one image to one image
    template<typename InType, typename OutType>
    inline wsp::State CopyRGBImage(
        const InType *in_src, const uchar *in_alpha, OutType *o_dst, s32 length, s32 src_num_channels, s32 dst_num_channels, 
        bool mask_alpha=false, bool normalize=false
    )
    {
        return wsp::img::CopyOneChannel(in_src, in_alpha, o_dst, length, src_num_channels, dst_num_channels, 0, 0, mask_alpha, normalize)
            && wsp::img::CopyOneChannel(in_src, in_alpha, o_dst, length, src_num_channels, dst_num_channels, 1, 1, mask_alpha, normalize)
            && wsp::img::CopyOneChannel(in_src, in_alpha, o_dst, length, src_num_channels, dst_num_channels, 2, 2, mask_alpha, normalize);
    }

    //! \ingroup WspImage
    //! \brief Copy every channels from one image to one image
    template<typename InType, typename OutType>
    inline wsp::State CopyAllChannels(
        const InType *in_src, 
        const uchar *in_alpha, 
        OutType *o_dst, 
        s32 length, 
        s32 src_num_channels, 
        s32 dst_num_channels,
        bool mask_alpha=false, 
        bool normalize=false
    )
    {
        for(s32 c=0; c<dst_num_channels; ++c){
            if(c>=src_num_channels){
                wsp::img::CopyOneChannel(in_src, in_alpha, o_dst, length, src_num_channels, dst_num_channels, src_num_channels-1, c, mask_alpha, normalize);
                continue;
            }
            wsp::img::CopyOneChannel(in_src, in_alpha, o_dst, length, src_num_channels, dst_num_channels, c, c, mask_alpha, normalize);
        }
        return WSP_STATE_SUCCESS;
    }
    //! \ingroup WspImage
    //! \brief Cut off value by a given threshold
    template<typename T> 
    wsp::State CutOff(T *io_im, uchar *io_alpha, s32 length, s32 num_channels, T thre_min, T thre_max=255);

    // =========================================================
    //                    Interpolation
    // =========================================================
    //! If target state is img::WSP_PIXEL_STATE_TRANSPARENT then fill up pixels by alpha less than 254
    //! else alpha must be 255 for all pixels
    template<typename T> s32 FillUpGaussianAverage(
            T *io_img, uchar *io_alpha, uchar *io_state, const uchar *in_mask, s32 width, s32 height, s32 nChannels,  
            wsp::img::PixelState targetState=WSP_PIXEL_STATE_OCCLUDED, 
            s32 blockSize=3, double variance=12.96, bool use_alpha_weight=false,
            uchar thre_transparent_alpha=255
    );
    template<typename T> s32 FillUpGaussianAverageHorizontalDirection(
            T *io_img, uchar *io_alpha, uchar *io_state, const uchar *in_mask, s32 width, s32 height, s32 num_channels,
            wsp::img::PixelState target_state=WSP_PIXEL_STATE_OCCLUDED,
            s32 block_size=3, double variance=12.96, bool use_alpha_weight=false
    );
    template<typename T> wsp::State FillUpTimes(
            T *io_img, uchar *io_alpha, uchar *io_state, const uchar *in_mask, s32 width, s32 height, s32 nChannel,  
            wsp::img::PixelState target_state=WSP_PIXEL_STATE_OCCLUDED, 
            s32 init_blocksize=1, s32 blocksize_step=1, 
            double variance=12.96, bool use_alpha_weight=false,
            bool is_directional=false,
            uchar thre_transparent_alpha=255
    );
    template<typename T> wsp::State FillUpZeroAlphaHorizontalLine(
            T *io_img, uchar *io_alpha,
            s32 width, s32 height, s32 num_channels,
            bool right_to_left=false,
            bool fill_alpha=true, bool fill_around_edge=false
    );
    /**
     * @param tmp_pattern_data temporary buffer for pattern array(memory size must be larger than length*num_channels)
     */
    template<typename T> void Inpas32VectorWithMirroredPatternAscending(
        T *io_vector, uchar *io_alpha,
        T *tmp_pattern_data, 
        s32 length, s32 num_channels, 
        uchar thre_alpha=128,
        bool fill_alpha=true
    );
    /**
     * @param tmp_pattern_data temporary buffer for pattern array(memory size must be larger than length*num_channels)
     */
    template<typename T> void Inpas32VectorWithMirroredPatternDescending(
        T *io_vector, uchar *io_alpha,
        T *tmp_pattern_data, 
        s32 length, s32 num_channels, 
        uchar thre_alpha=128,
        bool fill_alpha=true
    );
    template<typename T> void Inpas32WithHorizontalPatternBlend(
        T *io_img, uchar *io_alpha,
        s32 width, s32 height, s32 num_channels, 
        uchar thre_alpha=128,
        bool fill_alpha=true
    );
    template<typename T> wsp::State Inpas32WithHorizontalPatternAscending(
        T *io_img, uchar *io_alpha,
        s32 width, s32 height, s32 num_channels, 
        uchar thre_alpha=128,
        bool fill_alpha=true
    );
    template<typename T> wsp::State Inpas32WithHorizontalPatternDescending(
        T *io_img, uchar *io_alpha,
        s32 width, s32 height, s32 num_channels, 
        uchar thre_alpha=128,
        bool fill_alpha=true
    );

    // =========================================================
    //                       Transform
    // =========================================================
    struct AffineParam{
        f32 anchor_x, anchor_y;
        f32 translate_x, translate_y;
        f32 scale_x, scale_y;
        f32 rotate_angle;
        AffineParam()
            : anchor_x(0.0f),anchor_y(0.0f)
            , translate_x(0.0f), translate_y(0.0f)
            , scale_x(1.0f), scale_y(1.0f)
            , rotate_angle(0.0f)
        {}
    };
    template<typename _Type>
    void AffineTransform( 
        const _Type *in_src, 
        s32 src_width, s32 src_height, s32 src_num_channels, 
        _Type *o_dest,
        s32 width, s32 height, s32 num_channels, 
        const struct wsp::img::AffineParam &param
    );

    template<typename _InType, typename _OutType>
    void RotateImageClockwise(
        _OutType *o_img,
        const _InType *in_img,
        s32 width, s32 height, s32 num_channels
    );
    
    template<typename InType, typename OutType> void ScaleImageNearestNeighbor(
                                                                               const InType *in_src, s32 src_width, s32 src_height, s32 num_channels,
                                                                               OutType *o_dst,  s32 dst_width, s32 dst_height
                                                                               );
    
    template<typename InType, typename OutType> void ScaleImageBilinear(
                                                                        const InType *in_src, s32 src_width, s32 src_height, s32 num_channels,
                                                                        OutType *o_dst,  s32 dst_width, s32 dst_height
                                                                        );
    template<typename InType, typename OutType> void ScaleImageBilinear1(
                                                                         const InType *in_src, s32 src_width, s32 src_height, s32 num_channels,
                                                                         OutType *o_dst,  s32 dst_width, s32 dst_height
                                                                         );

    template<typename InType, typename OutType> 
    void ScaleImage(
        const InType *in_src, 
        s32 src_width, 
        s32 src_height, 
        s32 num_channels,
        OutType *o_dst, 
        s32 dst_width, s32 dst_height, 
        wsp::img::InterpolationAlgorithm alg
    ){
        switch(alg){
        case WSP_INTERPOLATION_NEAREST_NEIGHBOR:
            wsp::img::ScaleImageNearestNeighbor(
                in_src, 
                src_width, 
                src_height, 
                num_channels, 
                o_dst, 
                dst_width, 
                dst_height
            );
            break;
        case WSP_INTERPOLATION_BILINEAR:
            wsp::img::ScaleImageBilinear(
                in_src, 
                src_width, 
                src_height, 
                num_channels, 
                o_dst, 
                dst_width, 
                dst_height
            );
            break;
        default:
            WSP_COMMON_WARNING_LOG("Unrecognized s32erpolation algorithm\n");
            break;
        }
    }

    // =========================================================
    //                      Matching
    // =========================================================


    template<typename T>
    void GetDirectionalSumOfCorr(
        const T *in_main, 
        const T *in_sub, 
        double *o_corr_sum,
        s32 width, s32 height, s32 num_channels,
        s32 disp_x, s32 disp_y,
        double (*eval_func)(double, double),
        double default_corr=0.0
    );

    // =========================================================
    //                         Stereo 
    // =========================================================
    template<typename InType, typename OutType>
    wsp::State StereoStandardBM(
        const InType *in_main,
        const InType *in_sub, 
        OutType *o_disp, 
        s32 width, s32 height, s32 num_channels,
        const wsp::img::StereoBMParams &in_params
    );

    // =========================================================
    //                   Segmentation
    // =========================================================
    template<typename _SegmIntegerType, typename _RefType, typename _OutSegmType>
    wsp::State FindPrincipalSegments( 
        const _SegmIntegerType *in_segment_map, 
        const _RefType *in_ref_map, 
        _OutSegmType *o_principal_segm_map,
        s32 length, 
        f32 hit_percentage_thre,
        _SegmIntegerType segm_min_level,
        _SegmIntegerType segm_max_level
    );

    // =========================================================
    //                       Other
    // =========================================================

    //***********************************************************
    //* FUNCTION DECLARATION (wsp::img::GetCharacteristicMask)
    //! \ingroup WspImage
    template<typename T>
    void GetCharacteristicMask(
        uchar *o_state, 
        const T *in_src, 
        s32 width, s32 height, s32 nChannels, 
        s32 win_size,
        double thre_av, 
        double (*eval_func)(double, double),
        const double *in_winSizeMap=NULL
    );

    //! \ingroup WspImage
    //! \brief function for estimate background
    /*!
        in_segm must include index data like 0, 1, 2,.... 
        The max index must smaller than number of segmentation.
    */
    template<typename T>
    void EstimateBackgroundArea(
        const T *in_segm, 
        uchar *o_state, 
        s32 width, s32 height, s32 num_segm, 
        double bg_top_perct,
        double top_height_perct=0.5, 
        double far_depth_perct=0.5,
        bool lower_segm_is_near=true, 
        uchar bg_state=WSP_PIXEL_STATE_BACKGROUND
    );

    //***********************************************************
    //* FUNCTION DECLARATION (wsp::img::GetCharacteristicMaskByDeviation)
    //! \ingroup WspImage
    template<typename T>
    inline void GetCharacteristicMaskByDeviation(
        uchar *o_state, 
        const T *in_src, 
        s32 width, s32 height, s32 nChannels, 
        s32 win_size,
        double thre_av,
        const double *in_winSizeMap=NULL
    ){
        wsp::img::GetCharacteristicMask(o_state, in_src, width, height, nChannels, win_size, thre_av, wsp::math::GetAbsDiff<double>, in_winSizeMap);
    }
    //***********************************************************
    //* FUNCTION DECLARATION (wsp::img::GetCharacteristicMaskByVariance)
    //! \ingroup WspImage
    template<typename T>
    inline void GetCharacteristicMaskByVariance(
        uchar *o_state, 
        const T *in_src, 
        s32 width, s32 height, s32 nChannels, 
        s32 win_size,
        double thre_av,
        const double *in_winSizeMap=NULL
    ){
        wsp::img::GetCharacteristicMask(o_state, in_src, width, height, nChannels, win_size, thre_av, wsp::math::GetSquaredDiff<double>, in_winSizeMap);
    }

    /** @brief  Weight each pixel by gaussian distribution, the center pixel weight is 1.0 */

    template<typename T>
    void MultiplyGausssianDistributionWeight(
        T *io_img,
        s32 width, s32 height, s32 num_channels,
        s32 gauss_center_x, 
        s32 gauss_center_y,
        double gauss_sigma,
        double gauss_max
    );


}}


#include "_tpl_fn-imgproc.inl"

#endif