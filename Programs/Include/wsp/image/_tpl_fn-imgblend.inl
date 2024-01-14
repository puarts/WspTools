/**
 * @file _tpl_fn-imgblend.inl
 * 
 */

#ifndef _WSP_TEMPLATE_BASE_FN_IMG_BLEND_INL_
#define _WSP_TEMPLATE_BASE_FN_IMG_BLEND_INL_

#include "_tpl_fn-imgblend.hpp"

namespace {
    typedef float BlendCalcType;

    inline BlendCalcType BlendValue(
        BlendCalcType base_value, BlendCalcType blend_value
    )
    {
        return blend_value;
    }

    inline BlendCalcType DarkenValue(
        BlendCalcType base_value, BlendCalcType blend_value
    )
    {
        return base_value < blend_value? base_value : blend_value;
    }

    inline BlendCalcType MultiplyValue(
        BlendCalcType base_value, BlendCalcType blend_value
    )
    {
        BlendCalcType tmp_value = blend_value/255.0f;
        if( tmp_value > 1.0f)
        {
            tmp_value = 1.0f;
        }
        return base_value * tmp_value;
    }

    inline BlendCalcType BurnValue(
        BlendCalcType base_value, BlendCalcType blend_value
    )
    {
        BlendCalcType tmp_value;
        if (blend_value == 0) tmp_value = 0;
        else tmp_value = 255 - ((255 - base_value) * 255 / blend_value);
 
        return (tmp_value < 0)? 0 : tmp_value;
    }

    inline BlendCalcType LightenValue(
        BlendCalcType base_value, BlendCalcType blend_value
    )
    {
        return base_value > blend_value? base_value : blend_value;
    }

    inline BlendCalcType AddValue(
        BlendCalcType base_value, BlendCalcType blend_value
    )
    {
        BlendCalcType tmp_value = base_value + blend_value;
        if( tmp_value > 255.0 )
        {
            tmp_value = 255.0;
        }
        return tmp_value;
    }

    inline BlendCalcType ScreenValue(
        BlendCalcType base_value, BlendCalcType blend_value
    )
    {
        return 255 - ((255 - base_value) * (255 - blend_value)) / 255;
    }

    inline BlendCalcType DodgeValue(
        BlendCalcType base_value, BlendCalcType blend_value
    )
    {
        BlendCalcType tmp_value;
        if (blend_value == 255){ tmp_value = 255; }
        else{ tmp_value = base_value * 255 / (255 - blend_value); }
 
        return (tmp_value > 255)? 255 : tmp_value;
    }

    inline BlendCalcType OverlayValue(
        BlendCalcType base_value, BlendCalcType blend_value
    )
    {
        BlendCalcType tmp_value;
        if (base_value < 128){ tmp_value = base_value * blend_value * 2 / 255; }
        else{ tmp_value = 2 * (base_value + blend_value - base_value * blend_value / 255) - 255; }
 
        return (tmp_value > 255)? 255 : tmp_value;
    }

    inline BlendCalcType SoftLightValue(
        BlendCalcType base_value, BlendCalcType blend_value
    )
    {
        return blend_value < 128?
            (pow((base_value / 255), ((255 - blend_value)) / 128)) * 255
            : pow((base_value / 255), (128 / blend_value)) * 255;
    }

    inline BlendCalcType HardLightValue(
        BlendCalcType base_value, BlendCalcType blend_value
    )
    {
        BlendCalcType tmp_value;
        if (blend_value < 128){ tmp_value = base_value * blend_value * 2 / 255; }
        else{ tmp_value = 2 * (base_value + blend_value - base_value * blend_value / 255) - 255; }
 
        return (tmp_value > 255)? 255: tmp_value;
    }

    inline BlendCalcType SubtractValue(
        BlendCalcType base_value, BlendCalcType blend_value
    )
    {
        BlendCalcType tmp_value = base_value - blend_value;
        return tmp_value < 0? 0 : tmp_value;
    }

    inline BlendCalcType DifferenceValue(
        BlendCalcType base_value, BlendCalcType blend_value
    )
    {
        return fabs(base_value - blend_value);
    }

    inline BlendCalcType DivideValue(
        BlendCalcType base_value, BlendCalcType blend_value
    )
    {
        if( blend_value==0 ){ return 255; }
        BlendCalcType tmp_value = base_value * 255 / blend_value;
        return tmp_value>255? 255: tmp_value;
    }

    inline BlendCalcType ExclusionValue(
        BlendCalcType base_value, BlendCalcType blend_value
    )
    {
        return base_value + blend_value - 2 * base_value * blend_value / 255;
    }

    template<typename _Type>
    void BlendMain(
        _Type       *io_base_img, 
        const _Type *in_overlay_img,
        const u8    *in_alpha_data, 
        u32 width_step, u32 height, u32 num_channels, 
        BlendCalcType (*BlendFunc) ( BlendCalcType, BlendCalcType )
    )
    {
        float transmittance;
        const _Type *blend_ptr = in_overlay_img;
        const u8 *alpha_ptr  = in_alpha_data;
        _Type *dst_ptr     = io_base_img;
        _Type *dst_end_ptr = dst_ptr + width_step * height;
        _Type *c_ep;
        for( ; dst_ptr!=dst_end_ptr; ++alpha_ptr ) 
        {
            transmittance = (*alpha_ptr)/255.0f;
            for( c_ep = dst_ptr + num_channels; dst_ptr!=c_ep; ++dst_ptr, ++blend_ptr) 
            {
                *dst_ptr = static_cast<_Type>(*dst_ptr * (1.0f-transmittance) +
                    (_Type)BlendFunc( (BlendCalcType)*dst_ptr, (BlendCalcType)*blend_ptr ) * transmittance);
            }
        }
    }
}

template<typename _Type> 
wsp::Result wsp::img::BlendImage(
    _Type       *io_base_img, 
    const _Type *in_overlay_img,
    const u8    *in_alpha_data, 
    u32 width_step, u32 height, u32 num_channels,
    WSP_ImageBlendMode mode
)
{
    switch( mode )
    {
    case WSP_IMAGE_BLEND_MODE_NORMAL:     wsp::img::BlendNormal    ( io_base_img, in_overlay_img, in_alpha_data, width_step, height, num_channels ); break;
    case WSP_IMAGE_BLEND_MODE_DARKEN:     wsp::img::BlendDarken    ( io_base_img, in_overlay_img, in_alpha_data, width_step, height, num_channels ); break;
    case WSP_IMAGE_BLEND_MODE_MULTIPLY:   wsp::img::BlendMultiply  ( io_base_img, in_overlay_img, in_alpha_data, width_step, height, num_channels ); break;
    case WSP_IMAGE_BLEND_MODE_BURN:       wsp::img::BlendBurn      ( io_base_img, in_overlay_img, in_alpha_data, width_step, height, num_channels ); break;
    case WSP_IMAGE_BLEND_MODE_LIGHTEN:    wsp::img::BlendLighten   ( io_base_img, in_overlay_img, in_alpha_data, width_step, height, num_channels ); break;
    case WSP_IMAGE_BLEND_MODE_ADD:        wsp::img::BlendAdd       ( io_base_img, in_overlay_img, in_alpha_data, width_step, height, num_channels ); break;
    case WSP_IMAGE_BLEND_MODE_SCREEN:     wsp::img::BlendScreen    ( io_base_img, in_overlay_img, in_alpha_data, width_step, height, num_channels ); break;
    case WSP_IMAGE_BLEND_MODE_DODGE:      wsp::img::BlendDodge     ( io_base_img, in_overlay_img, in_alpha_data, width_step, height, num_channels ); break;
    case WSP_IMAGE_BLEND_MODE_OVERLAY:    wsp::img::BlendOverlay   ( io_base_img, in_overlay_img, in_alpha_data, width_step, height, num_channels ); break;
    case WSP_IMAGE_BLEND_MODE_SOFTLIGHT:  wsp::img::BlendSoftLight ( io_base_img, in_overlay_img, in_alpha_data, width_step, height, num_channels ); break;
    case WSP_IMAGE_BLEND_MODE_HARDLIGHT:  wsp::img::BlendHardLight ( io_base_img, in_overlay_img, in_alpha_data, width_step, height, num_channels ); break;
    case WSP_IMAGE_BLEND_MODE_SUBTRACT:   wsp::img::BlendSubtract  ( io_base_img, in_overlay_img, in_alpha_data, width_step, height, num_channels ); break;
    case WSP_IMAGE_BLEND_MODE_DIFFERENCE: wsp::img::BlendDifference( io_base_img, in_overlay_img, in_alpha_data, width_step, height, num_channels ); break;
    case WSP_IMAGE_BLEND_MODE_DIVIDE:     wsp::img::BlendDivide    ( io_base_img, in_overlay_img, in_alpha_data, width_step, height, num_channels ); break;
    case WSP_IMAGE_BLEND_MODE_EXCLUSION:  wsp::img::BlendExclusion ( io_base_img, in_overlay_img, in_alpha_data, width_step, height, num_channels ); break;
    default: return wsp::ResultInvalidArgument();
    }
    return wsp::ResultSuccess();
}

template<typename _Type> 
void wsp::img::BlendNormal(
    _Type       *io_base_img, 
    const _Type *in_overlay_img,
    const u8    *in_alpha_data, 
    u32 width_step, u32 height, u32 num_channels
)
{
    BlendMain( 
        io_base_img, in_overlay_img, in_alpha_data, 
        width_step, height, num_channels,
        BlendValue
    );
}

template<typename _Type> 
void wsp::img::BlendDarken(
    _Type       *io_base_img, 
    const _Type *in_overlay_img,
    const u8    *in_alpha_data, 
    u32 width_step, u32 height, u32 num_channels
)
{
    BlendMain( 
        io_base_img, in_overlay_img, in_alpha_data, 
        width_step, height, num_channels,
        DarkenValue
    );
}


template<typename _Type> 
void wsp::img::BlendMultiply(
    _Type       *io_base_img, 
    const _Type *in_overlay_img,
    const u8    *in_alpha_data, 
    u32 width_step, u32 height, u32 num_channels
)
{
    BlendMain( 
        io_base_img, in_overlay_img, in_alpha_data, 
        width_step, height, num_channels,
        MultiplyValue
    );
}

template<typename _Type> 
void wsp::img::BlendBurn(
    _Type       *io_base_img, 
    const _Type *in_overlay_img,
    const u8    *in_alpha_data, 
    u32 width_step, u32 height, u32 num_channels
)
{
    BlendMain( 
        io_base_img, in_overlay_img, in_alpha_data, 
        width_step, height, num_channels,
        BurnValue
    );
}

template<typename _Type> 
void wsp::img::BlendLighten(
    _Type       *io_base_img, 
    const _Type *in_overlay_img,
    const u8    *in_alpha_data, 
    u32 width_step, u32 height, u32 num_channels
)
{
    BlendMain( 
        io_base_img, in_overlay_img, in_alpha_data, 
        width_step, height, num_channels,
        LightenValue
    );
}


template<typename _Type> 
void wsp::img::BlendAdd(
    _Type       *io_base_img, 
    const _Type *in_overlay_img,
    const u8    *in_alpha_data, 
    u32 width_step, u32 height, u32 num_channels
)
{
    BlendMain( 
        io_base_img, in_overlay_img, in_alpha_data, 
        width_step, height, num_channels,
        AddValue
    );
}


template<typename _Type> 
void wsp::img::BlendScreen(
    _Type       *io_base_img, 
    const _Type *in_overlay_img,
    const u8    *in_alpha_data, 
    u32 width_step, u32 height, u32 num_channels
)
{
    BlendMain( 
        io_base_img, in_overlay_img, in_alpha_data, 
        width_step, height, num_channels,
        ScreenValue
    );
}


template<typename _Type> 
void wsp::img::BlendDodge(
    _Type       *io_base_img, 
    const _Type *in_overlay_img,
    const u8    *in_alpha_data, 
    u32 width_step, u32 height, u32 num_channels
)
{
    BlendMain( 
        io_base_img, in_overlay_img, in_alpha_data, 
        width_step, height, num_channels,
        DodgeValue
    );
}

template<typename _Type> 
void wsp::img::BlendOverlay(
    _Type       *io_base_img, 
    const _Type *in_overlay_img,
    const u8    *in_alpha_data, 
    u32 width_step, u32 height, u32 num_channels
)
{
    BlendMain( 
        io_base_img, in_overlay_img, in_alpha_data, 
        width_step, height, num_channels,
        OverlayValue
    );
}

template<typename _Type> 
void wsp::img::BlendSoftLight(
    _Type       *io_base_img, 
    const _Type *in_overlay_img,
    const u8    *in_alpha_data, 
    u32 width_step, u32 height, u32 num_channels
)
{
    BlendMain( 
        io_base_img, in_overlay_img, in_alpha_data, 
        width_step, height, num_channels,
        SoftLightValue
    );
}

template<typename _Type> 
void wsp::img::BlendHardLight(
    _Type       *io_base_img, 
    const _Type *in_overlay_img,
    const u8    *in_alpha_data, 
    u32 width_step, u32 height, u32 num_channels
)
{
    BlendMain( 
        io_base_img, in_overlay_img, in_alpha_data, 
        width_step, height, num_channels,
        HardLightValue
    );
}

template<typename _Type> 
void wsp::img::BlendSubtract(
    _Type       *io_base_img, 
    const _Type *in_overlay_img,
    const u8    *in_alpha_data, 
    u32 width_step, u32 height, u32 num_channels
)
{
    BlendMain( 
        io_base_img, in_overlay_img, in_alpha_data, 
        width_step, height, num_channels,
        SubtractValue
    );
}

template<typename _Type> 
void wsp::img::BlendDifference(
    _Type       *io_base_img, 
    const _Type *in_overlay_img,
    const u8    *in_alpha_data, 
    u32 width_step, u32 height, u32 num_channels
)
{
    BlendMain( 
        io_base_img, in_overlay_img, in_alpha_data, 
        width_step, height, num_channels,
        DifferenceValue
    );
}

template<typename _Type> 
void wsp::img::BlendDivide(
    _Type       *io_base_img, 
    const _Type *in_overlay_img,
    const u8    *in_alpha_data, 
    u32 width_step, u32 height, u32 num_channels
)
{
    BlendMain( 
        io_base_img, in_overlay_img, in_alpha_data, 
        width_step, height, num_channels,
        DivideValue
    );
}

template<typename _Type> 
void wsp::img::BlendExclusion(
    _Type       *io_base_img, 
    const _Type *in_overlay_img,
    const u8    *in_alpha_data, 
    u32 width_step, u32 height, u32 num_channels
)
{
    BlendMain( 
        io_base_img, in_overlay_img, in_alpha_data, 
        width_step, height, num_channels,
        ExclusionValue
    );
}

#endif
