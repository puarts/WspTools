/**
 * @file _fn-imgproc.h
 * 
 * @brief Base Function Set for Image Processing
 */

#ifndef _WSP_IMAGE_BASE_FUNCTION_IMGPROC_H_
#define _WSP_IMAGE_BASE_FUNCTION_IMGPROC_H_

#ifndef __WSP_IMAGEUTIL_DEFINE_IMAGEUTIL_H__
#include "_define_image.h"
#endif

namespace wsp{ namespace img{
    // =====================================================
    //   Channel Operations (SPP means Samples Per Pixel)
    // =====================================================
    WSP_DLL_EXPORT wsp::State MergeChannels(
        uchar *o_rgbaImg, u32 dstSpp, 
        const uchar *in_rgbImg, u32 srcSpp, 
        const uchar *in_alphaData, 
        u32 width, u32 height);

    WSP_DLL_EXPORT wsp::State SeparateChannels(
        uchar *o_rgbImg, uchar *o_alphaData, 
        const uchar *in_imgRgba, 
        u32 width, u32 height);

    // =============================
    //         Stereo Vision
    // =============================

    /** @brief Find peak sub-pixel by estimation from pixel values. */
    WSP_DLL_EXPORT double GetPeakSubpixel(
        const double *in_pocData, int length, int d=1, double low_pass_ratio=1.0);

    /** @brief Find peak sub-pixel by estimation from pixel values. */
    WSP_DLL_EXPORT double GetPeakSubpixel(
        const complex64 *in_pocData, int length, int d, double low_pass_ratio);

    //! \ingroup WspImage
    //! \brief Function gives subpixel disparity by POC
    /*!
        val_max: max pixel value in the POC result
        max_index: index of val_max
        d: distance between max index and sampling indices
        val_md: sampled pixel value at pixel[max_index-d]
        val_pd: sampled pixel value at pixel[max_index+d]
        length: number of whole pixels
        low_pass_ratio: cutting off ratio of low pass filter

                            r(a-b) - r(a+b)
        delta = -----------------------------------------------
                                            V
                   r(a-b) - 2*cos( PI * b * - )*r(a) + r(a+b)
                                            N                    
        where a: position of a pixel which has maximum value
              b: num of pixels between a and sampling pixels
              (sampling pixels are r(a), r(a-b), r(a+b))
        
        define a=0, b=1 then

                            r(-1) - r(1)
        delta = -----------------------------------------------
                                       V
                   r(-1) - 2*cos( PI * - )*r(0) + r(1)
                                       N                    

    */
    inline double GetPeakSubpixel(
        double val_max, int max_index, int d, 
        double val_md, double val_pd, 
        int length, 
        double low_pass_ratio
    ){
        int half_len = length/2;
        int p_i = max_index-half_len;
        //printf("p=%d, p_i=%d, max=%f\n", p, p_i, max);
        //printf("r[p+d]=%f, r[p]=%f, r[p-d]=%f\n", r[p+d][0], r[p][0], r[p-d][0]);

        double cospid2r = 2*cos(low_pass_ratio*M_PI*d) * val_max;
        double u_pd = val_md + val_pd - cospid2r;
        double v_pd = p_i*cospid2r - (p_i-d)*val_md - (p_i+d)*val_pd;
        double delta = v_pd / u_pd;
        double disparity = delta - half_len + length;
        //printf("delta = %f\n", delta);
        //printf("disparity = %f\n", disparity);

        // check correction of equation
        //double left = (p_i-d+delta)*val_md+(p_i+d+delta)*val_pd;
        //double right = 2*(p_i+delta)*cos(low_pass_ratio*M_PI*d)*val_max;
        //printf("%f = %f\n", left, right);

        return disparity;
        //return delta;
    }

    //! \ingroup WspImage
    //! \brief return the distance for the given stereo disparity
    /*!
        disparity_pixels:        disparity in pixels
        focal_length_mm:        focal length in millimetres
        sensor_pixels_per_mm:    number of pixels per millimetre on the sensor
        baseline_mm:            distance between cameras in millimetres
    */
    inline double DisparityToDistance_Parallel(
            double disparity_pixels,
            double focal_length_mm,
            double sensor_pixels_per_mm,
            double baseline_mm
    ){
        double focal_length_pixels = focal_length_mm * sensor_pixels_per_mm;
        double distance_mm = focal_length_pixels * baseline_mm / disparity_pixels;
        return distance_mm;
    }


}}

#endif