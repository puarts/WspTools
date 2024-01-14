
#ifndef _WSP_IMAGE_TEMPLATE_CLASS_IMAGE_EXTENSION_INL_
#define _WSP_IMAGE_TEMPLATE_CLASS_IMAGE_EXTENSION_INL_

#include "tpl_cl-image_extension.hpp"

#include "_tpl_fn-imgproc.hpp"
#include <wsp/math/_tpl_fn-fourier_transform.hpp>
#ifdef USE_FFTW
#include <wsp/fftw/fn-math-gpl.h>
#endif


//#define USE_OPTIMIZED_ImageExtension_ApplyFourierTransform
template<typename _Type>
void wsp::img::ImageExtension::ApplyFourierTransform(
    wsp::Image<_Type> &this_image, wsp::Image<_Type> &o_imaginary_image)
{
    wsp::Image<_Type> real_image(this_image);

    real_image.SetSizeToPowerOfTwo();

    s32 old_width = this_image.width();
    s32 old_height = this_image.height();
    s32 resized_width = real_image.width();
    s32 resized_height = real_image.height();
    s32 num_channels = this_image.num_of_channels();
    s32 num_pixels = resized_width * resized_height;

    o_imaginary_image.SetSize(resized_width, resized_height, num_channels);

    wsp::Image<_Type>* real_images = WSP_NEW wsp::Image<_Type>[num_channels];
    wsp::Image<_Type>* imag_images = WSP_NEW wsp::Image<_Type>[num_channels];

    try
    {
        // Apply FFT for each channels
        for (s32 i = 0; i < num_channels; ++i)
        {
            wsp::Image<_Type>* channel_real_image = &real_images[i];
            wsp::Image<_Type>* channel_imag_image = &imag_images[i];

            *channel_real_image = real_image;
            channel_real_image->LeaveOneChannel(i);
            channel_imag_image->SetSize(resized_width, resized_height, 1, true);
            channel_imag_image->Reset(0);


            //char buffer[128];
            //sprintf(buffer, "real_image_%d.png", i);
            //channel_real_image->Save(buffer);
            //sprintf(buffer, "imag_image_%d.png", i);
            //channel_imag_image->Save(buffer);


            wsp::math::ApplyFft2d<_Type>(
                channel_real_image->data(),
                channel_imag_image->data(),
                resized_width, resized_height,
                -1, -1
                , wsp::math::ApplyHannWindow<_Type>
                /*, wsp::math::ApplyHammingWindow<_Type>*/
                );

            //wsp::math::FftwApplyFft2d(
            //    channel_real_image->data(),
            //    channel_imag_image->data(),
            //    resized_width, resized_height);

            //sprintf(buffer, "real_image_after_%d.png", i);
            //channel_real_image->Save(buffer);
            //sprintf(buffer, "imag_image_after_%d.png", i);
            //channel_imag_image->Save(buffer);
        }

        this_image.SetSize(resized_width, resized_height, num_channels);

        // store fft image into each channels of source image
        {
            s32 num_pixels = resized_width * resized_height;
            for (s32 i = 0; i < num_channels; ++i)
            {
                _Type* real_ptr = this_image.data() + i;
                _Type* imag_ptr = o_imaginary_image.data() + i;
                _Type* src_real_ptr = real_images[i].data();
                _Type* src_imag_ptr = imag_images[i].data();
                _Type* end_ptr = src_real_ptr + num_pixels;
                for (; src_real_ptr != end_ptr;
                    real_ptr += num_channels, imag_ptr += num_channels,
                    ++src_real_ptr, ++src_imag_ptr)
                {
                    *real_ptr = *src_real_ptr;
                    *imag_ptr = *src_imag_ptr;
                }
            }
        }
    }
    catch (...)
    {
        delete[] real_images;
        delete[] imag_images;
        throw;
    }

    delete[] real_images;
    delete[] imag_images;
}

template<typename _Type>
void wsp::img::ImageExtension::ApplyInverseFourierTransform(
    wsp::Image<_Type> &io_real_image,
    const wsp::Image<_Type> &in_imaginary_image)
{
    throw WSP_NEW std::exception("not implemented");
}

template<typename _Type>
static void wsp::img::ImageExtension::FftShift(wsp::Image<_Type> &io_image)
{
    wsp::Image<_Type> tmp_image(io_image);

    wsp::img::SwapQuadrantHorizontally(
        tmp_image.data(), io_image.width(), io_image.height(), io_image.num_of_channels());

    wsp::img::SwapQuadrantVertically(
        tmp_image.data(), io_image.width(), io_image.height(), io_image.num_of_channels());

    io_image = tmp_image;
}

#endif
