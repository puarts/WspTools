/**
 * @file _tpl_fn-imglayout.hpp
 * 
 * @description Base template functions for image layout
 */

#ifndef _WSP_TEMPLATE_FN_IMG_LAYOUT_HPP_
#define _WSP_TEMPLATE_FN_IMG_LAYOUT_HPP_

#ifndef __WSP_COMMONCORE_RESULT_H__
#include <wsp/common/result.h>
#endif
#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#include <wsp/common/tpl_cl-vector.h>
#endif
#include "tpl_cl-image.h"

namespace wsp{ namespace img{

    template<typename _Type>
    void LayoutImagesAnyColumns(
        wsp::Image<_Type> &o_image,
        const wsp::Vector<wsp::Image<_Type>> &in_imagelist,
        u32 layout_unit_width, 
        u32 layout_unit_height,
        u32 num_columns
    );
}}

#include "tpl_fn-imglayout.inl"

#endif
