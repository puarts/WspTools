/**
 * @file _fn-math.h
 * 
 */

#ifndef _WSP_MATHCORE_BASE_FUNCTION_MATH_CORE_H__
#define _WSP_MATHCORE_BASE_FUNCTION_MATH_CORE_H__

#include "_define_mathcore.h"
#include "_math_types.h"

#ifdef __cplusplus
extern "C"{
#endif

WSP_DLL_EXPORT s32 WSP_GetClosestPowerOfTwo( s32 n );

//! Rotation Conversion -------------------------------------
WSP_DLL_EXPORT void WSP_EulerToMatrix( double *o_mat, const double in_eulerRot[3], WSP_RotationOrder order );
WSP_DLL_EXPORT void WSP_MatrixToEuler( double o_eulerRot[3], const double *in_mat, WSP_RotationOrder order );
WSP_DLL_EXPORT void WSP_InvertRotationMatrix( double *io_mat, WSP_RotationOrder order, WSP_AxisType axis );

//! Matrix Processing ---------------------------------------
WSP_DLL_EXPORT void WSP_MultiplyMatrix( double *o_mat, double *in_mat1, double *in_mat2 );
//WSP_DLL_EXPORT void WSP_LUDecomposition(double **L, double **U, double **mat, u32 row_size);
////void WSP_backSubstitution(WspMatrix& mat);

//! Other ---------------------------------------------------
WSP_DLL_EXPORT void WSP_GetLinearFunctionByLeastSquaresMethod(
                                            double *o_gradient, 
                                            double *o_intercept, 
                                            const double *in_posArrayX, 
                                            const double *in_posArrayY, 
                                            u32 length
                                        );
//
//WSP_DLL_EXPORT void WSP_RungeKutta4th(double* dest, double st, double end, double step, double initValue, double(*dfunc)(double t, double y));
//WSP_DLL_EXPORT void WSP_ImprovedEuler(double* dest, double st, double end, double step, double initValue, double(*dfunc)(double t, double y));
//    
//
////inline WSP_MathState GetPhaseCorrelationFFT1DPerLine(double *o_poc, double *o_pocAv, const double *in_ref, const double *in_tpl, int width, int height, double low_pass_ratio);
//
////double getMonteCurlo();
////double Boxmuller( double vd_stdev, double vd_meanValue, int sw=0);
//
//
////// pattern recognition 1
////int getGaussianRanddataSet(double *dataSet, double mean, double dev, int N);
////int bayesEstimation(double *dataSet, double mean_p, double stddev_p, int N);
////int bayesTest();
//
//////! pattern recognition 2
////int getCircleRandSet(WspFunctionArray &dataSet, double holeRadius=0.1, double radius=0.3);
////int selectRandom(WspFunctionArray &destData, double* destVar, WspFunctionArray &srcData);
////int emAlgorithm(WspFunctionArray &dataSet,    WspFunctionArray &m_new, double *var_new, double *p_new,
////                WspFunctionArray &m_old, double *var_old, double *p_old, double &error);
////int emTest();
//

// ======================================
//          Fourier Transform
// ======================================
/* MOVED */WSP_DLL_EXPORT void WSP_ApplyHannWindow_Complex(complex64 *io_data, int length);
WSP_DLL_EXPORT void WSP_ApplyHannWindowHor(complex64 *io_data, int width, int height);
/* MOVED */WSP_DLL_EXPORT void WSP_SwapQuadrantHV_Complex(complex64 *io_img, int width, int height);
/* MOVED */WSP_DLL_EXPORT void WSP_SwapQuadrantHor_Complex(complex64 *io_img, int width, int height);
/* MOVED */WSP_DLL_EXPORT void WSP_SwapQuadrantVer_Complex(complex64 *io_img, int width, int height);
WSP_DLL_EXPORT void WSP_Transpose_Complex(complex64 *io_img, int width, int height);

/**
 * @param low_pass_ratio @default 0.75
 */
WSP_DLL_EXPORT void WSP_ApplySpectralWeighting_Rectangular(
    complex64 *io_data, int length, double low_pass_ratio);

/**
 * @param sigma @default 1.0
 */
WSP_DLL_EXPORT void WSP_ApplySpectralWeighting_Gaussian(
    complex64 *io_data, int length, double sigma);

//! \ingroup WspMath
//! \brief 1D Discreat Fourier Transform
/* MOVED */WSP_DLL_EXPORT WSP_MathState WSP_ApplyDft1dToDoubleArray(
    double *dest_r, double *dest_i, 
    const double *src_r, const double *src_i, int sampleSize, bool inverse);

// ----------------------------
//! \ingroup WspMath
//! \brief 1D Fast Fourier Transform with separated data of complex conjugate into real array and imaginary array
/*!
    This computes an in-place complex-to-complex FFT 
    io_real and io_imag are the real and imaginary arrays of 2^m points.
    ifft = false gives forward transform
    ifft = true gives reverse transform 
    iter <= 0 gives calculation of 2^m from size
*/
/* MOVED */WSP_DLL_EXPORT WSP_MathState WSP_ApplyFft1dToDoubleArray(
    double *io_real, double *io_imag, int size, int exponent, bool ifft);

//! \ingroup WspMath
//! \brief Optimaized 1D Fast Fourier Transform with separated data of complex conjugate into real array and imaginary array
/**
 * @param exponent @default -1
 * @param ifft     default is false
 */
/* MOVED */WSP_DLL_EXPORT WSP_MathState WSP_ApplyFft1dToDoubleArray_Opt(
    double *io_real, double *io_imag, int size, int exponent, bool ifft);

//! \ingroup WspMath
//! \brief 1D Fast Fourier Transform with complex conjugate array, it is faster than separated complex data
/* MOVED */WSP_DLL_EXPORT WSP_MathState WSP_ApplyFft1dToComplexArray(
    complex64 *io_complex, int size, int exponent, bool ifft);

//! \ingroup WspMath
//! \ingroup WspMath
//! \brief 2D Fast Fourier Transform
/**
 * @param exponent_x @default -1
 * @param exponent_y @default -1
 */
/* MOVED */WSP_DLL_EXPORT WSP_MathState WSP_ApplyFft2dToDoubleArray(
    double *io_imag, double *io_real, 
    int size_x, int size_y, int exponent_x, int exponent_y, bool ifft);

//! \ingroup WspMath
//! \brief 1D per line Fast Fourier Transform
WSP_DLL_EXPORT WSP_MathState WSP_ApplyFft1dPerLineToDoubleArray(
    double *io_real, double *io_imag, int width, int height, int exponent, bool ifft);

//! \ingroup WspMath
//! \brief 1D per line Fast Fourier Transform
WSP_DLL_EXPORT WSP_MathState WSP_ApplyFft1dPerLineToComplexArray(
    complex64 *io_complex, int width, int height, int exponent, bool ifft);

//! \ingroup WspMath
//! \brief 2D per line Fast Fourier Transform
/* MOVED */WSP_DLL_EXPORT WSP_MathState WSP_ApplyFft2dPerLineToDoubleArray(
    double *io_real, double *io_imag, 
    int width, int height, int exponent_x, int exponent_y, bool ifft);

//! \ingroup WspMath
//! \brief 2D per line Fast Fourier Transform
/*!
    @param exponent_x @default -1
    @param exponent_y @default -1
    @param apply_han_win Apply HAN window to the source bytes before apply FFT @default true
    @param do_last_transpose @default false
    @param tmp_complex temporary complex buffer for speeding up @default NULL
    io_complex: input and output data array
    tmp_complex: if you give the array which has the same size of io_complex, function does'nt need to allocate memory
                    so it will be faster than the case that tmp_complex is NULL.
    do_last_transpose: if this is false, it will be faster but the result will be transposed.
*/
/* MOVED */WSP_DLL_EXPORT WSP_MathState WSP_ApplyFft2dPerLineToComplexArray(
    complex64 *io_complex, 
    int width, int height, 
    int exponent_x, int exponent_y, 
    bool ifft, 
    bool apply_hann_win,
    bool do_last_transpose,
    complex64 *tmp_complex
);

//! \ingroup WspMath
//! \brief Functio which can remove 2D high frequency spectrum
WSP_DLL_EXPORT void WSP_RemoveCentricVerticalHorizontalArea(
    complex64 *io_data, int width, int height, double remove_ratio);

//! \ingroup WspMath
//! \brief Functio which can remove 1D high frequency spectrum
WSP_DLL_EXPORT void WSP_RemoveCentricVerticalArea(
    complex64 *io_data, int width, int height, double remove_ratio);

//! \ingroup WspMath
//! \brief Functio which can get criss power spectrum 
WSP_DLL_EXPORT void WSP_GetCrossPowerSpectrum(
    complex64 *io_ref, complex64 *io_tpl, complex64 *o_poc, int length);

//! \ingroup WspMath
//! \brief Function which can get Phase-Only Correlation by 1D Fast Fourier Transform 
/*!
    @brief Get1dFftPoc is a function which can get Phase-Only Correlation by 1D Per line FFT.
    @param o_poc_real     output array for POC result
    @param o_poc_imag     buffer array for calculation of POC
    @param low_pass_ratio @default 1.0
*/
WSP_DLL_EXPORT WSP_MathState WSP_Get1dFftPoc_Double(
    double *o_poc_real, double *o_poc_imag, 
    double *o_pocav_real, double *o_pocav_imag, 
    double *io_ref_real, double *io_ref_imag, 
    double *io_tpl_real, double *io_tpl_imag, 
    int width, int height, 
    double low_pass_ratio
);

/**
 * @param exponent_x @default -1
 * @param apply_hann_win @default true
 * @param low_pass_ratio @default 1.0
 */
WSP_DLL_EXPORT WSP_MathState WSP_Get1dFftPoc_Complex(
    complex64 *io_ref, 
    complex64 *io_tpl, 
    complex64 *o_poc, 
    complex64 *o_average_poc, 
    int width, 
    int height, 
    int exponent_x,
    bool apply_hann_win,
    double low_pass_ratio
);

//! \ingroup WspMath
//! \brief Get 2D POC by using 2D FFT per line
/**
 * @brief width and height must be power of two.
 * @param exponent_x @default -1
 * @param exponent_y @default -1
 * @param apply_hann_win @default true
 * @param low_pass_ratio @default 1.0
 * @param tmp_data @default NULL
 */
WSP_DLL_EXPORT WSP_MathState WSP_Get2dPocWithFft_Complex(
    complex64 *io_ref,
    complex64 *io_tpl, 
    complex64 *o_poc, 
    int width, 
    int height,
    int exponent_x,
    int exponent_y,
    double low_pass_ratio, 
    bool apply_hann_win, 
    complex64 *tmp_data
);



WSP_DLL_EXPORT double WSP_GetLennardJonesPotential(double distance, double epsilon, double sigma);

//! Differetial of Potential
WSP_DLL_EXPORT double WSP_GetLennardJonesForce(double distance, double epsilon, double sigma);
//inline double WSP_LennardJones_Position(double x0, double v, double a, double dt)
//{
//}

#ifdef __cplusplus
}
#endif

#endif
