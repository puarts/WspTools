/**
* @file tpl_fn-detection.hpp
* 
*/

#pragma once

#include <wsp/common/_ctypes.h>

namespace wsp{ namespace img{
    template<typename _Type>
    bool IsBlurredImage(
        const _Type* in_image,
        u32 width,
        u32 height,
        u32 num_channels);
}}

#include "tpl_fn-detection.inl"
