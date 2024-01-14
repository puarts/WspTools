/**
 * @file _fn-math.h
 * 
 */

#ifndef _WSP_MATHCORE_BASE_FUNCTION_MATH_H__
#define _WSP_MATHCORE_BASE_FUNCTION_MATH_H__

#include "_define_mathcore.h"
#include "_math_types.h"
#include "_define_mathutil.h"

#include <wsp/common/fn-debug.h>

namespace wsp{
    namespace math{



        //! \ingroup WspMath
        //! \brief solve quadratic function
        /*!
        |x1^2 x1 1||A| |y1|       |A| |x1^2 x1 1|^-1 |y1|
        |x2^2 x2 1||B|=|y2| => |B|=|x2^2 x2 1|    |y2|
        |x3^2 x3 1||C| |y3|       |C| |x3^2 x3 1|    |y3|

        To solve this quadratic function, need to solve inverse matrix
        Here is the result.
        denom = (x1 - x2)(x1 - x3)(x2 - x3)
        A = (x3 * (y2 - y1) + x2 * (y1 - y3) + x1 * (y3 - y2)) / denom
        B = (x3^2 * (y1 - y2) + x2^2 * (y3 - y1) + x1^2 * (y2 - y3)) / denom
        C = (x2 * x3 * (x2 - x3) * y1 + x3 * x1 * (x3 - x1) * y2 + x1 * x2 * (x1 - x2) * y3) / denom
        */
        inline void Fit2dParabolaFromThreePoints(
            double in_x1, double in_x2, double in_x3,
            double in_y1, double in_y2, double in_y3,
            double &o_a, double &o_b, double &o_c
            ){
            double denom = (in_x1 - in_x2)*(in_x1 - in_x3)*(in_x2 - in_x3);
            o_a = (in_x3 * (in_y2 - in_y1) + in_x2 * (in_y1 - in_y3) + in_x1 * (in_y3 - in_y2)) / denom;
            o_b = (in_x3*in_x3 * (in_y1 - in_y2) + in_x2*in_x2 * (in_y3 - in_y1) + in_x1*in_x1 * (in_y2 - in_y3)) / denom;
            o_c = (in_x2 * in_x3 * (in_x2 - in_x3) * in_y1 + in_x3 * in_x1 * (in_x3 - in_x1) * in_y2 + in_x1 * in_x2 * (in_x1 - in_x2) * in_y3) / denom;
        }

        inline void FindPeakOfQuadraticFunction(
            double &o_peak_x, double &o_peak_y,
            double coef2, double coef1, double coef0
            ){
            if (coef2 == 0){
                o_peak_x = 0;
                o_peak_y = coef0;
                return;
            }
            o_peak_x = -coef1 / (2 * coef2);
            o_peak_y = coef2*o_peak_x*o_peak_x + coef1*o_peak_x + coef0;
        }

        inline void FindQuadraticPeak(
            double in_x1, double in_x2, double in_x3,
            double in_y1, double in_y2, double in_y3,
            double &o_peak_x, double &o_peak_y
            ){
            double coefs[3] = { 0, 0, 0 };
            Fit2dParabolaFromThreePoints(in_x1, in_x2, in_x3, in_y1, in_y2, in_y3, coefs[0], coefs[1], coefs[2]);
            FindPeakOfQuadraticFunction(o_peak_x, o_peak_y, coefs[0], coefs[1], coefs[2]);
        }

        inline double GetQuadraticPeakDisparityFromNeighboringPixels(
            double in_y1, double in_y2, double in_y3,
            double &o_peak_x
            ){
            return (in_y1 - in_y3) / (2 * in_y1 - 4 * in_y2 + 2 * in_y3);
        }

        //! \ingroup WspMath
        //! \brief function for subpixel estimation of SAD matching
        /*!
        p1(in_x1, in_y1) and p2(in_x2, in_y2) make 1 line1,
        and line2 is the line go through p3(in_x3, in_y3). line1 and line2 make equiangular line.
        The crossing point of equiangular line is the peak.

        y1 - y2
        a1 = ---------   b1 = y1 - a1 * x1
        x1 - x2 ,

        a2 = -a1,   b2 = y3 - a2 * x3

        x = (b2-b1) / (a1-a2), y = a1*x + b1

        y/x distance ratio of p1-p2 is better to be greater than y/x distance ratio of p2-p3.
        */
        inline void FindPeakOfEquiangularLine(
            double in_x1, double in_x2, double in_x3,
            double in_y1, double in_y2, double in_y3,
            double &o_peak_x, double &o_peak_y
            ){
            if ((in_x1 - in_x2) == 0){
                o_peak_x = in_x2;
                o_peak_y = in_y2;
                return;
            }
            double a1, b1, a2, b2;
            a1 = (in_y1 - in_y2) / (in_x1 - in_x2);
            b1 = in_y1 - a1 * in_x1;
            a2 = -a1;
            b2 = in_y3 - a2 * in_x3;
            o_peak_x = (b2 - b1) / (a1 - a2);
            o_peak_y = a1*o_peak_x + b1;
        }
        inline double GetPeakOfEquiangularLineFromNeighboringPixels(
            double in_y1, double in_y2, double in_y3
            ){
            if (in_y3<in_y1){
                //return (in_y3-in_y1) / (2*(in_y2-in_y1));
                return (in_y3 - in_y1) / (2 * (in_y2 - in_y1));
            }
            return (in_y3 - in_y1) / (2 * (in_y2 - in_y3));
        }




        ////! Rotation Conversion -------------------------------------
        //WSP_DLL_EXPORT void EulerToMatrix(double *o_mat, const double in_eulerRot[3], wsp::math::RotationOrder order);
        //WSP_DLL_EXPORT void MatrixToEuler(double o_eulerRot[3], const double *in_mat, wsp::math::RotationOrder order);
        //WSP_DLL_EXPORT void InvertRotationMatrix(double *io_mat, wsp::math::RotationOrder order, wsp::math::AxisType axis=wsp::math::AXIS_XYZ);

        ////! Matrix Processing ---------------------------------------
        //WSP_DLL_EXPORT void MultiplyMatrix(double *o_mat, double *in_mat1, double *in_mat2);
        //WSP_DLL_EXPORT void LUDecomposition(double **L, double **U, double **mat, u32 row_size);
        ////void backSubstitution(WspMatrix& mat);

        ////! Other ---------------------------------------------------
        //WSP_DLL_EXPORT void GetLinearFunctionByLeastSquaresMethod(double &o_gradient, double &o_intercept, const double *in_posArrayX, const double *in_posArrayY, u32 length);

        //WSP_DLL_EXPORT void RungeKutta4th(double* dest, double st, double end, double step, double initValue, double(*dfunc)(double t, double y));
        //WSP_DLL_EXPORT void ImprovedEuler(double* dest, double st, double end, double step, double initValue, double(*dfunc)(double t, double y));
        //

        ////inline wsp::State GetPhaseCorrelationFFT1DPerLine(double *o_poc, double *o_pocAv, const double *in_ref, const double *in_tpl, int width, int height, double low_pass_ratio);

        ////double getMonteCurlo();
        ////double Boxmuller( double vd_stdev, double vd_meanValue, int sw=0);


        ////// pattern recognition 1
        ////int getGaussianRanddataSet(double *dataSet, double mean, double dev, int N);
        ////int bayesEstimation(double *dataSet, double mean_p, double stddev_p, int N);
        ////int bayesTest();

        //////! pattern recognition 2
        ////int getCircleRandSet(WspFunctionArray &dataSet, double holeRadius=0.1, double radius=0.3);
        ////int selectRandom(WspFunctionArray &destData, double* destVar, WspFunctionArray &srcData);
        ////int emAlgorithm(WspFunctionArray &dataSet,    WspFunctionArray &m_new, double *var_new, double *p_new,
        ////                WspFunctionArray &m_old, double *var_old, double *p_old, double &error);
        ////int emTest();

        //! Physical Equations --------------------------------
        inline double LennardJones_Potential(double distance, double epsilon = 1.0, double sigma = 1.0);
        inline double LennardJones_Force(double distance, double epsilon = 1.0, double sigma = 1.0);
        //inline double LennardJones_Position(double x0, double v, double a, double dt);

        //// ======================================
        ////          Fourier Transform
        //// ======================================
        //WSP_DLL_EXPORT void SwapQuadrantHV_Complex(complex64 *io_img, int width, int height);
        //WSP_DLL_EXPORT void SwapQuadrantHor_Complex(complex64 *io_img, int width, int height);
        //WSP_DLL_EXPORT void SwapQuadrantVer_Complex(complex64 *io_img, int width, int height);
        //WSP_DLL_EXPORT void Transpose_Complex(complex64 *io_img, int width, int height);

        //inline void ApplySpectralWeighting_Rectangular(complex64 *io_data, int length, double low_pass_ratio=0.75)
        //{
        //    int hlen = length/2;
        //    int thre_lpf = static_cast<int>(hlen*(1.0-low_pass_ratio));
        //    {
        //        int i;
        //        double *io_ptr = reinterpret_cast<double*>(io_data);
        //        double *end_ptr = io_ptr + length*2;
        //        for(i=0; io_ptr!=end_ptr; ++io_ptr, ++i){
        //            // remove high freq(low pass filter) because it include many noise
        //            if(abs(i-hlen) < thre_lpf){
        //                (*io_ptr) = 0.0;
        //                ++io_ptr;
        //                (*io_ptr) = 0.0;
        //            }
        //        }
        //    }
        //}
        //inline void ApplySpectralWeighting_Gaussian(complex64 *io_data, int length, double sigma=1.0)
        //{
        //    int hlen = length/2;
        //    double sigma_sq = sigma*sigma;
        //    double len_sq = length*length;
        //    double mult = -2*M_PI*sigma_sq/len_sq;
        //    {
        //        int i;
        //        double *io_ptr = reinterpret_cast<double*>(io_data);
        //        double *end_ptr = io_ptr + length*2;
        //        double weight;
        //        for(i=-hlen; io_ptr!=end_ptr; ++io_ptr, ++i){
        //            weight = exp(i*i*mult);
        //            (*io_ptr) *= weight;
        //            ++io_ptr;
        //            (*io_ptr) *= weight;
        //        }
        //    }
        //}

        ////! \ingroup WspMath
        ////! \brief 1D Discreat Fourier Transform
        //WSP_DLL_EXPORT wsp::State DFT1d(double *dest_r, double *dest_i, const double *src_r, const double *src_i, int sampleSize, bool inverse);

        //// ----------------------------
        ////! \ingroup WspMath
        ////! \brief 1D Fast Fourier Transform with separated data of complex conjugate into real array and imaginary array
        ///*!
        //    This computes an in-place complex-to-complex FFT 
        //    io_real and io_imag are the real and imaginary arrays of 2^m points.
        //    ifft = false gives forward transform
        //    ifft = true gives reverse transform 
        //    iter <= 0 gives calculation of 2^m from size
        //*/
        //WSP_DLL_EXPORT wsp::State FFT1d(double *io_real, double *io_imag, int size,int exponent=-1, bool ifft=false);
        ////! \ingroup WspMath
        ////! \brief Optimaized 1D Fast Fourier Transform with separated data of complex conjugate into real array and imaginary array
        //WSP_DLL_EXPORT wsp::State FFT1d_Opt(double *io_real, double *io_imag, int size, int exponent=-1, bool ifft=false);
        ////! \ingroup WspMath
        ////! \brief 1D Fast Fourier Transform with complex conjugate array, it is faster than separated complex data
        //WSP_DLL_EXPORT wsp::State FFT1d(complex64 *io_complex, int size, int exponent=-1, bool ifft=false);
        ////! \ingroup WspMath
        ////! \ingroup WspMath
        ////! \brief 2D Fast Fourier Transform
        //WSP_DLL_EXPORT wsp::State FFT2d(double *io_imag, double *io_real, int size_x, int size_y, int exponent_x=-1, int exponent_y=-1, bool ifft=false);

        ////! \ingroup WspMath
        ////! \brief 1D per line Fast Fourier Transform
        //WSP_DLL_EXPORT wsp::State FFT1dPerLine(double *io_real, double *io_imag, int width, int height, int exponent=-1, bool ifft=false);
        ////! \ingroup WspMath
        ////! \brief 1D per line Fast Fourier Transform
        //WSP_DLL_EXPORT wsp::State FFT1dPerLine(complex64 *io_complex, int width, int height, int exponent=-1, bool ifft=false);
        ////! \ingroup WspMath
        ////! \brief 2D per line Fast Fourier Transform
        //WSP_DLL_EXPORT wsp::State FFT2dPerLine(double *io_real, double *io_imag, int width, int height, int exponent_x=-1, int exponent_y=-1, bool ifft=false);
        ////! \ingroup WspMath
        ////! \brief 2D per line Fast Fourier Transform
        ///*!
        //    io_complex: input and output data array
        //    tmp_complex: if you give the array which has the same size of io_complex, function does'nt need to allocate memory
        //                 so it will be faster than the case that tmp_complex is NULL.
        //    do_last_transpose: if this is false, it will be faster but the result will be transposed.
        //*/
        //WSP_DLL_EXPORT wsp::State FFT2dPerLine(
        //    complex64 *io_complex, 
        //    int width, int height, 
        //    int exponent_x=-1, int exponent_y=-1, 
        //    bool ifft=false, 
        //    bool apply_hann_win=true,
        //    bool do_last_transpose=true,
        //    complex64 *tmp_complex=NULL
        //);

        ////! \ingroup WspMath
        ////! \brief Functio which can remove 2D high frequency spectrum
        //void RemoveCentricVerticalHorizontalArea(complex64 *io_data, int width, int height, double remove_ratio);
        ////! \ingroup WspMath
        ////! \brief Functio which can remove 1D high frequency spectrum
        //void RemoveCentricVerticalArea(complex64 *io_data, int width, int height, double remove_ratio);

        ////! \ingroup WspMath
        ////! \brief Functio which can get criss power spectrum 
        //void GetCrossPowerSpectrum(complex64 *io_ref, complex64 *io_tpl, complex64 *o_poc, int length);

        ////! \ingroup WspMath
        ////! \brief Function which can get Phase-Only Correlation by 1D Fast Fourier Transform 
        ///*!
        //    Get1dFftPoc is a function which can get Phase-Only Correlation by 1D Per line FFT.
        //    o_poc_real : output array for POC result
        //    o_poc_imag : buffer array for calculation of POC
        //*/
        //WSP_DLL_EXPORT wsp::State Get1dFftPoc(
        //    double *o_poc_real, double *o_poc_imag, 
        //    double *o_pocav_real, double *o_pocav_imag, 
        //    double *io_ref_real, double *io_ref_imag, 
        //    double *io_tpl_real, double *io_tpl_imag, 
        //    int width, int height, 
        //    double low_pass_ratio=1.0
        //);

        //WSP_DLL_EXPORT wsp::State Get1dFftPoc(
        //    complex64 *io_ref, 
        //    complex64 *io_tpl, 
        //    complex64 *o_poc, 
        //    complex64 *o_average_poc, 
        //    int width, 
        //    int height, 
        //    int exponent_x=-1,
        //    bool apply_hann_win=true,
        //    double low_pass_ratio=1.0
        //);

        ////! \ingroup WspMath
        ////! \brief Get 2D POC by using 2D FFT per line
        ///*!
        //    width and height must be power of two.
        //*/
        //WSP_DLL_EXPORT wsp::State Get2dPocWithFft(
        //    complex64 *io_ref,
        //    complex64 *io_tpl, 
        //    complex64 *o_poc, 
        //    int width, 
        //    int height,
        //    int exponent_x=-1,
        //    int exponent_y=-1,
        //    double low_pass_ratio=1.0, 
        //    bool apply_hann_win=true, 
        //    complex64 *tmp_data=NULL
        //);



    }
}

inline double wsp::math::LennardJones_Potential(double distance, double epsilon, double sigma){
    double sig_dist = sigma / distance;
    return 4.0*epsilon*(pow(sig_dist, 12) - pow(sig_dist, 6));
}


//! Differetial of Potential
inline double wsp::math::LennardJones_Force(double distance, double epsilon, double sigma)
{
    double sig_dist = sigma / distance;
    return 24.0*epsilon*(2.0*pow(sig_dist, 13.0) - pow(sig_dist, 7.0)) / sigma;
}
inline double LennardJones_Position(double x0, double v, double a, double dt)
{
    WSP_FATAL("not implemented");
    return 0.0;
}

//inline double wsp::math::LennardJones_Force(MyFnVector& p1, MyFnVector& p2) const{
//    MyFnVector v = p1-p2;
//    double dr = v.getNormSquare();
//    //double threshold = sigma + (pow(2.0, 1.0/6.0) * sigma - sigma)*0.0;
//    //if(dr < threshold){ dr = threshold; }
//    //if(dr < sigma){ dr = sigma; }
//    return 24.0*p1.epsilon*(2.0*pow(p1.sigma/dr,13.0) - pow(p1.sigma/dr,7.0))/p1.sigma;
//}



//! gaussian distribution ---------------------------------------------
//inline double wsp::math::GetGaussianWeight(double distance, double deviation){
//    return exp(-(distance*distance) / (2*deviation*deviation));
//}


//inline wsp::State wsp::math::GetPhaseCorrelationFFT1DPerLine(double *o_poc, double *o_pocAv, const double *in_ref, const double *in_tpl, int width, int height, double low_pass_ratio)
//{
//    //! TODO: implement the code for resizing reference and template image as size of power of 2
//    if(!wsp::math::IsPowerOfTwo<u32>(width)){
//        WSP_COMMON_ERROR_LOG("width must be power of two: width=%d\n", width);
//        return WSP_STATE_FAILURE;
//    }
//    return wsp::math::GetPhaseCorrelationDFT1DPerLine(o_poc, o_pocAv, in_ref, in_tpl, width, height, low_pass_ratio);
//}

#endif
