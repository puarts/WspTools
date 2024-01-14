#pragma once

#include <wsp/common/_define_commoncore.h>
#include <wsp/common/common_types.h>

namespace wsp{
    namespace fftw{
        // ==============================
        //         Correlation
        // ==============================
        WSP_DLL_EXPORT wsp::State GetPhaseCorrelationDFT1DPerLine_fftw(
            complex64 *o_poc,
            complex64 *o_pocAv,
            const complex64 *in_ref,
            const complex64 *in_tpl,
            int width, int height,
            double low_pass_ratio = 1.0
        );

        WSP_DLL_EXPORT wsp::State GetPhaseCorrelationDFT1DPerLine(double *o_poc, double *o_pocAv, const double *in_ref, const double *in_tpl, int width, int height, double low_pass_ratio = 1.0);

        WSP_DLL_EXPORT wsp::State GetPhaseCorrelationFFT1DPerLine3(double *o_poc3, double *o_pocAv3, double * const in_ref3, double * const in_tpl3, int width, int height, double low_pass_ratio = 1.0);
        WSP_DLL_EXPORT wsp::State GetPhaseCorrelationDFT2D(double *o_poc, const double *in_ref, const double *in_tpl, int width, int height, double low_pass_ratio = 1.0);
    }
}