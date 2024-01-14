
#include "detail-fft.h"

#include <wsp/common/fn-io.h>

wsp::State wsp::fftw::detail::FFT1DPerLine_fftw(fftw_complex *img, int width, int height, int sign)
{
    if (width == 0 || height == 0){
        WSP_COMMON_ERROR_LOG("width or height must not be zero\n");
        return WSP_STATE_FAILURE;
    }
    int i, length, w, h;
    fftw_plan fft_img;

    w = (int)width;
    h = (int)height;
    length = w*h;

    //! horizontal fft
    for (i = 0; i<length; i += w){
        //! setup FFTW plans
        fft_img = fftw_plan_dft_1d(w, &img[i], &img[i], sign, FFTW_ESTIMATE);

        //! obtain the FFT of img1 and img2
        fftw_execute(fft_img);

        //! free memories
        fftw_destroy_plan(fft_img);
    }
    return WSP_STATE_SUCCESS;
}

