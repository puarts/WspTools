#pragma once

#include <wsp/common/common_types.h>

#include <fftw3.h>

namespace wsp{
    namespace fftw{
        namespace detail {
            wsp::State FFT1DPerLine_fftw(fftw_complex *img, int width, int height, int sign = FFTW_FORWARD);
        }
    }
}
