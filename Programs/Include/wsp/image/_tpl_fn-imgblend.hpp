/**
 * @file _tpl_fn-imgblend.hpp
 * 
 * @description Base template functions for image blending
 */

#ifndef _WSP_TEMPLATE_BASE_FN_IMG_BLEND_HPP_
#define _WSP_TEMPLATE_BASE_FN_IMG_BLEND_HPP_

#ifndef __WSP_COMMONCORE_RESULT_H__
#include <wsp/common/result.h>
#endif
#ifndef __WSP_IMAGEUTIL_DEFINE_IMAGEUTIL_H__
#include "_define_image.h"
#endif

/** @brief image layer mode */
typedef enum{
    WSP_IMAGE_BLEND_MODE_NORMAL=0,
    WSP_IMAGE_BLEND_MODE_DARKEN,
    WSP_IMAGE_BLEND_MODE_MULTIPLY,
    WSP_IMAGE_BLEND_MODE_BURN,
    WSP_IMAGE_BLEND_MODE_LIGHTEN,
    WSP_IMAGE_BLEND_MODE_ADD,
    WSP_IMAGE_BLEND_MODE_SCREEN,
    WSP_IMAGE_BLEND_MODE_DODGE,
    WSP_IMAGE_BLEND_MODE_OVERLAY,
    WSP_IMAGE_BLEND_MODE_SOFTLIGHT,
    WSP_IMAGE_BLEND_MODE_HARDLIGHT,
    WSP_IMAGE_BLEND_MODE_DIFFERENCE,
    WSP_IMAGE_BLEND_MODE_EXCLUSION,
    WSP_IMAGE_BLEND_MODE_SUBTRACT,
    WSP_IMAGE_BLEND_MODE_DIVIDE,
    WSP_IMAGE_BLEND_MODE_UNKNOWN
} WSP_ImageBlendMode;

inline const char* WSP_GetImageBlendModeAsString( WSP_ImageBlendMode mode )
{
    switch( mode )
    {
    case WSP_IMAGE_BLEND_MODE_NORMAL:    return "WSP_IMAGE_BLEND_MODE_NORMAL";
    case WSP_IMAGE_BLEND_MODE_DARKEN:    return "WSP_IMAGE_BLEND_MODE_DARKEN";
    case WSP_IMAGE_BLEND_MODE_MULTIPLY:  return "WSP_IMAGE_BLEND_MODE_MULTIPLY";
    case WSP_IMAGE_BLEND_MODE_BURN:      return "WSP_IMAGE_BLEND_MODE_BURN";
    case WSP_IMAGE_BLEND_MODE_LIGHTEN:   return "WSP_IMAGE_BLEND_MODE_LIGHTEN";
    case WSP_IMAGE_BLEND_MODE_ADD:       return "WSP_IMAGE_BLEND_MODE_ADD";
    case WSP_IMAGE_BLEND_MODE_SCREEN:    return "WSP_IMAGE_BLEND_MODE_SCREEN";
    case WSP_IMAGE_BLEND_MODE_DODGE:     return "WSP_IMAGE_BLEND_MODE_DODGE";
    case WSP_IMAGE_BLEND_MODE_OVERLAY:   return "WSP_IMAGE_BLEND_MODE_OVERLAY";
    case WSP_IMAGE_BLEND_MODE_SOFTLIGHT: return "WSP_IMAGE_BLEND_MODE_SOFTLIGHT";
    case WSP_IMAGE_BLEND_MODE_HARDLIGHT: return "WSP_IMAGE_BLEND_MODE_HARDLIGHT";
    case WSP_IMAGE_BLEND_MODE_DIFFERENCE:return "WSP_IMAGE_BLEND_MODE_DIFFERENCE";
    case WSP_IMAGE_BLEND_MODE_EXCLUSION: return "WSP_IMAGE_BLEND_MODE_EXCLUSION";
    case WSP_IMAGE_BLEND_MODE_SUBTRACT:  return "WSP_IMAGE_BLEND_MODE_SUBTRACT";
    case WSP_IMAGE_BLEND_MODE_DIVIDE:    return "WSP_IMAGE_BLEND_MODE_DIVIDE";
    }
    return "Unknown";
}



namespace wsp{ namespace img{

    typedef WSP_ImageBlendMode ImageBlendMode;


    template<typename _Type> 
    wsp::Result BlendImage(
        _Type       *io_base_img, 
        const _Type *in_overlay_img,
        const u8    *in_alpha_data, 
        u32 width_step, u32 height, u32 num_channels,
        WSP_ImageBlendMode mode
    );

    template<typename _Type> 
    void BlendNormal(
        _Type       *io_base_img, 
        const _Type *in_overlay_img,
        const u8    *in_alpha_data, 
        u32 width_step, u32 height, u32 num_channels
    );

    template<typename _Type> 
    void BlendDarken(
        _Type       *io_base_img, 
        const _Type *in_overlay_img,
        const u8    *in_alpha_data, 
        u32 width_step, u32 height, u32 num_channels
    );

    template<typename _Type> 
    void BlendMultiply(
        _Type       *io_base_img, 
        const _Type *in_overlay_img,
        const u8    *in_alpha_data, 
        u32 width_step, u32 height, u32 num_channels
    );

    template<typename _Type> 
    void BlendBurn(
        _Type       *io_base_img, 
        const _Type *in_overlay_img,
        const u8    *in_alpha_data, 
        u32 width_step, u32 height, u32 num_channels
    );

    template<typename _Type> 
    void BlendLighten(
        _Type       *io_base_img, 
        const _Type *in_overlay_img,
        const u8    *in_alpha_data, 
        u32 width_step, u32 height, u32 num_channels
    );

    template<typename _Type> 
    void BlendAdd(
        _Type       *io_base_img, 
        const _Type *in_overlay_img,
        const u8    *in_alpha_data, 
        u32 width_step, u32 height, u32 num_channels
    );

    template<typename _Type> 
    void BlendScreen(
        _Type       *io_base_img, 
        const _Type *in_overlay_img,
        const u8    *in_alpha_data, 
        u32 width_step, u32 height, u32 num_channels
    );

    template<typename _Type> 
    void BlendDodge(
        _Type       *io_base_img, 
        const _Type *in_overlay_img,
        const u8    *in_alpha_data, 
        u32 width_step, u32 height, u32 num_channels
    );

    template<typename _Type> 
    void BlendOverlay(
        _Type       *io_base_img, 
        const _Type *in_overlay_img,
        const u8    *in_alpha_data, 
        u32 width_step, u32 height, u32 num_channels
    );

    template<typename _Type> 
    void BlendSoftLight(
        _Type       *io_base_img, 
        const _Type *in_overlay_img,
        const u8    *in_alpha_data, 
        u32 width_step, u32 height, u32 num_channels
    );

    template<typename _Type> 
    void BlendHardLight(
        _Type       *io_base_img, 
        const _Type *in_overlay_img,
        const u8    *in_alpha_data, 
        u32 width_step, u32 height, u32 num_channels
    );

    template<typename _Type> 
    void BlendSubtract(
        _Type       *io_base_img, 
        const _Type *in_overlay_img,
        const u8    *in_alpha_data, 
        u32 width_step, u32 height, u32 num_channels
    );

    template<typename _Type> 
    void BlendDifference(
        _Type       *io_base_img, 
        const _Type *in_overlay_img,
        const u8    *in_alpha_data, 
        u32 width_step, u32 height, u32 num_channels
    );

    template<typename _Type> 
    void BlendDivide(
        _Type       *io_base_img, 
        const _Type *in_overlay_img,
        const u8    *in_alpha_data, 
        u32 width_step, u32 height, u32 num_channels
    );

    template<typename _Type> 
    void BlendExclusion(
        _Type       *io_base_img, 
        const _Type *in_overlay_img,
        const u8    *in_alpha_data, 
        u32 width_step, u32 height, u32 num_channels
    );

}}

#include "_tpl_fn-imgblend.inl"

#endif
