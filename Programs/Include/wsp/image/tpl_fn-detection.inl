#pragma once

#include "tpl_fn-detection.hpp"
#include "tpl_cl-image.h"

template<typename _Type>
bool wsp::img::IsBlurredImage(
    const _Type* in_image,
    u32 width,
    u32 height,
    u32 num_channels)
{
    wsp::Image<_Type> phase_image;

    return false;
}
