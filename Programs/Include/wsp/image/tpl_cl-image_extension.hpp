
#ifndef _WSP_IMAGE_TEMPLATE_CLASS_IMAGE_EXTENSION_HPP_
#define _WSP_IMAGE_TEMPLATE_CLASS_IMAGE_EXTENSION_HPP_

#include "tpl_cl-image.h"

namespace wsp{ namespace img{
    class ImageExtension
    {
    public:
        template<typename _Type>
        static void ApplyFourierTransform(
            wsp::Image<_Type> &io_real_image,
            wsp::Image<_Type> &o_imaginary_image);

        template<typename _Type>
        static void ApplyInverseFourierTransform(
            wsp::Image<_Type> &io_real_image, 
            const wsp::Image<_Type> &in_imaginary_image);

        template<typename _Type>
        static void FftShift(wsp::Image<_Type> &io_image);

        template<typename _Type>
        static bool IsBlurredImage(wsp::Image<_Type> &this_image);
    };
}}

#include "tpl_cl-image_extension.inl"

#endif
