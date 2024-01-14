/**
 * @file _define_image.h
 * @author 
 */

#ifndef __WSP_IMAGEUTIL_DEFINE_IMAGEUTIL_H__
#define __WSP_IMAGEUTIL_DEFINE_IMAGEUTIL_H__

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include <wsp/common/_define_commoncore.h>
#endif

#ifndef __WSP_COMMONCORE_COMMON_TYPES_H__
#include <wsp/common/common_types.h>
#endif

#ifndef __WSP_MATHCORE_MATH_TYPES_H__
#include <wsp/math/_math_types.h>
#endif

#include "_define_imagecore.h"

#ifndef __WSP_IMAGECORE_IMAGE_TYPES_H__
#include "_image_types.h"
#endif

#include <wsp/math/_define_mathutil.h>
#include <wsp/common/fn-io.h>


#define BEGIN_WSP_IMG_NAMESPACE BEGIN_WSP_CORE_NAMESPACE namespace img{
#define END_WSP_IMG_NAMESPACE } END_WSP_CORE_NAMESPACE


namespace wsp{ 
/**
 * @brief Collection of functions and classes for WspImage module
 */
namespace img{
    typedef uchar* ImgPtr8U;
    typedef ushort* ImgPtr16U;
    typedef u32* ImgPtr32U;

    /**
     * Convert YCbCr to RGB for SDTV.
     * @param r input CR
     * @param g input CG
     * @param b input CB
     * @param o_y output Y
     * @param o_cr output Cr
     * @param o_cb output Cb
     */
    template<typename InType, typename OutType>
    inline void ConvertYCbCrToRGB(
        InType y, InType cb, InType cr, 
        OutType &o_r, OutType &o_g, OutType &o_b
    ){
        InType cr128 = cr-128;
        InType cb128 = cb-128;
        //o_r = static_cast<OutType>(y16 + cr128/0.713);
        //o_b = static_cast<OutType>(y16 + cb128/0.564);
        //o_g = static_cast<OutType>((y16 - 0.299*cr128 - 0.114*cb128)/0.587);
        o_r = y + 1.4025 * (cr128);
        o_g = y - 0.344136 * (cb128) - 0.714136 * (cr128);
        o_b = y + 1.772 * (cb128);
    }

    /**
     * Convert YPbPr to RGB for HDTV.
     * @param r input PR
     * @param g input PG
     * @param b input PB
     * @param o_y output Y
     * @param o_pr output Pr
     * @param o_pb output Pb
     */
    template<typename InType, typename OutType>
    inline void ConvertYPbPrToRGB(
        InType y, InType pb, InType pr, 
        OutType &o_r, OutType &o_g, OutType &o_b
    ){
        InType pr128 = pr-128;
        InType pb128 = pb-128;
        o_r = static_cast<OutType>(y + 1.5748*pr128);
        o_g = static_cast<OutType>(y - 0.468*pr128 - 0.1873*pb128);
        o_b = static_cast<OutType>(y + 1.8556*pb128);
    }
    /**
     * Convert RGB to YCbCr for SDTV.
     * @param r input CR
     * @param g input CG
     * @param b input CB
     * @param o_y output Y
     * @param o_cr output Cr
     * @param o_cb output Cb
     */
    template<typename InType, typename OutType>
    inline void ConvertRGBToYCbCr(
        InType r, InType g, InType b, 
        OutType &o_y, OutType &o_cb, OutType &o_cr
    ){
        o_y  = static_cast<OutType>( 0.299*r + 0.587*g + 0.114*b + 16);
        o_cb = static_cast<OutType>(-0.169*r - 0.331*g + 0.500*b + 128); //= 0.564*(b-o_y);
        o_cr = static_cast<OutType>( 0.500*r - 0.419*g - 0.081*b + 128); //= 0.713*(r-o_y);
    }
    /**
     * Convert RGB to YCbCr for HDTV.
     * @param r input PR
     * @param g input PG
     * @param b input PB
     * @param o_y output Y
     * @param o_cr output Pr
     * @param o_cb output Pb
     */
    template<typename InType, typename OutType>
    inline void ConvertRGBToYPbPr(
        InType r, InType g, InType b, 
        OutType &o_y, OutType &o_pb, OutType &o_pr
    ){
        o_y  = static_cast<OutType>( 0.2126*r + 0.7152*g + 0.0722*b + 16);
        o_pb = static_cast<OutType>(-0.1146*r - 0.3854*g + 0.5000*b + 128); //= 0.5389*(b-o_y);
        o_pr = static_cast<OutType>( 0.5000*r - 0.4542*g - 0.0458*b + 128); //= 0.6350*(r-o_y);
    }
}}

#include "fn-image_log.h"

#endif