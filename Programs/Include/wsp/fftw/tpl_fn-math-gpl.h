#pragma once


#ifndef M_PI
    #define _USE_MATH_DEFINES
#endif
#include <math.h>

#include <wsp/math/_define_mathutil.h>
#include <fftw3.h>

namespace wsp{ namespace fftw{
    // Correlation ----------------------------------------------
    template<typename T> 
    wsp::State GetPhaseCorrelation1d(
        double *o_poc, const T *in_ref, const T *in_tpl, 
        u32 length, 
        double low_pass_ratio=1.0
    );
    template<typename T> 
    wsp::State GetPhaseCorrelation2d(
        double *o_poc, const T *in_ref, const T *in_tpl, 
        u32 width, u32 height, 
        double low_pass_ratio=1.0
    );
}}

//! get phase correlation from two images and save result to the third image
template<typename T>
wsp::State wsp::fftw::GetPhaseCorrelation1d(double *o_poc, const T *in_reference, const T *in_template, u32 length, double low_pass_ratio)
{
    double crossSpectrum;
    u32 i; //! iterators
    u32 x, y;
    int M = (length-1)/2;
        
    //! allocate FFTW input and output arrays
    fftw_complex *img1 = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * length);
    fftw_complex *img2 = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * length);
    fftw_complex *res  = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * length);    
    
    //! setup FFTW plans
    fftw_plan fft_img1 = fftw_plan_dft_1d(length, img1, img1, FFTW_FORWARD,  FFTW_ESTIMATE);
    fftw_plan fft_img2 = fftw_plan_dft_1d(length, img2, img2, FFTW_FORWARD,  FFTW_ESTIMATE);
    fftw_plan ifft_res = fftw_plan_dft_1d(length, res,  res,  FFTW_BACKWARD, FFTW_ESTIMATE);
    
    //! load images' data to FFTW input
    for(i=0; i<length; i++){
        img1[i][0] = (double)in_reference[i];
        img2[i][0] = (double)in_template[i];
        img1[i][1] = 0.0;
        img2[i][1] = 0.0;
    }
    
    //! obtain the FFT of img1 and img2
    fftw_execute(fft_img1);
    fftw_execute(fft_img2);

    //! obtain the cross power spectrum
    for(i=0; i<length; i++){
        //! remove high freq(low pass filter) because it include many noise
        if(abs((int)i-M) > (int)(M*low_pass_ratio)){
            res[i][0] = 0;
            res[i][1] = 0;
            continue;
        }
        //! corr can be represented as F(k1, k2)*conj(G(k1, k2))
        //! = (f0 - f1*i)*(g0 + g1*i) = (f0*g0 + f1*g1) + (f0*g1 - f1*g0)*i
        res[i][0] = (img1[i][0] * img2[i][0]) + (img1[i][1] * img2[i][1]);
        res[i][1] = (img1[i][0] * img2[i][1]) - (img1[i][1] * img2[i][0]);

        //tmp = sqrt( pow( res[i][0], 2.0 ) + pow( res[i][1], 2.0 ) );
        crossSpectrum = sqrt(res[i][0]*res[i][0] + res[i][1]*res[i][1]);

        res[i][0] /= crossSpectrum;
        res[i][1] /= crossSpectrum;
    }
    //for(y=0; y<height; y++){
    //    for(x=0; x<width; x++){
    //        i = y*width + x;
    //        //! remove high freq because it include many noise
    //        if(x<width*0.2 || x>width*0.8){
    //            res[i][0] = 0;
    //            res[i][1] = 0;
    //            continue;
    //        }


    //        res[i][0] = (img2[i][0] * img1[i][0]) - (img2[i][1] * (-img1[i][1]));
    //        res[i][1] = (img2[i][0] * (-img1[i][1])) + (img2[i][1] * img1[i][0]);

    //        //tmp = sqrt( pow( res[i][0], 2.0 ) + pow( res[i][1], 2.0 ) );
    //        crossSpectrum = sqrt(res[i][0]*res[i][0] + res[i][1]*res[i][1]);

    //        res[i][0] /= crossSpectrum;
    //        res[i][1] /= crossSpectrum;
    //    }
    //}

    //! obtain the phase correlation array
    fftw_execute(ifft_res);

    //! normalize and copy to result image
    for(i=0; i<length; i++) {
        o_poc[i] = res[i][0]/(double)length;
    }

    //! deallocate FFTW arrays and plans
    fftw_destroy_plan(fft_img1);
    fftw_destroy_plan(fft_img2);
    fftw_destroy_plan(ifft_res);
    fftw_free(img1);
    fftw_free(img2);
    fftw_free(res);
    return WSP_STATE_SUCCESS;
}
//template<typename T>
//wsp::State wsp::fftw::GetPhaseCorrelation2d(double *o_poc, const T *in_reference, const T *in_template,
//                                                u32 width, u32 height, double low_pass_ratio)
//{
//    double crossSpectrum;
//    int i; //! iterators
//    u32 x, y;
//    int w = (int)width;
//    int h = (int)height;
//    int length = w*h;
//    int M = (w-1)/2;
//        
//    //! allocate FFTW input and output arrays
//    fftw_complex *img1 = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * length);
//    fftw_complex *img2 = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * length);
//    fftw_complex *res  = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * length);    
//    
//    //! setup FFTW plans
//    fftw_plan fft_img1 = fftw_plan_dft_1d(w, img1, img1, FFTW_FORWARD,  FFTW_ESTIMATE);
//    fftw_plan fft_img2 = fftw_plan_dft_1d(w, img2, img2, FFTW_FORWARD,  FFTW_ESTIMATE);
//    fftw_plan ifft_res = fftw_plan_dft_1d(w, res,  res,  FFTW_BACKWARD, FFTW_ESTIMATE);
//    
//    //! load images' data to FFTW input
//    for(i=0; i<length; i++){
//        img1[i][0] = (double)in_reference[i];
//        img2[i][0] = (double)in_template[i];
//        img1[i][1] = 0.0;
//        img2[i][1] = 0.0;
//    }
//    
//    //! obtain the FFT of img1 and img2
//    fftw_execute(fft_img1);
//    fftw_execute(fft_img2);
//
//    //! obtain the cross power spectrum
//    for(i=0; i<length; i++){
//        //! remove high freq(low pass filter) because it include many noise
//        //if(abs((int)i-M) > (int)(M*low_pass_ratio)){
//        //    res[i][0] = 0;
//        //    res[i][1] = 0;
//        //    continue;
//        //}
//        //! corr can be represented as F(k1, k2)*conj(G(k1, k2))
//        //!                            = (f0 - f1*i)*(g0 + g1*i)
//        //!                            = (f0*g0 + f1*g1) + (f0*g1 - f1*g0)*i
//        //res[i][0] = (img1[i][0] * img2[i][0]) + (img1[i][1] * img2[i][1]);
//        //res[i][1] = (img1[i][0] * img2[i][1]) - (img1[i][1] * img2[i][0]);
//
//        ////tmp = sqrt( pow( res[i][0], 2.0 ) + pow( res[i][1], 2.0 ) );
//        //crossSpectrum = sqrt(res[i][0]*res[i][0] + res[i][1]*res[i][1]);
//
//        //res[i][0] /= crossSpectrum;
//        //res[i][1] /= crossSpectrum;
//        res[i][0] = img1[i][0];
//        res[i][1] = img1[i][1];
//    }
//
//
//    //! obtain the phase correlation array
//    fftw_execute(ifft_res);
//
//    //! normalize and copy to result image
//    for(i=0; i<length; i++) {
//        o_poc[i] = res[i][0]/(double)length;
//    }
//
//    //! deallocate FFTW arrays and plans
//    fftw_destroy_plan(fft_img1);
//    fftw_destroy_plan(fft_img2);
//    fftw_destroy_plan(ifft_res);
//    fftw_free(img1);
//    fftw_free(img2);
//    fftw_free(res);
//    return WSP_STATE_SUCCESS;
//}

template<typename T>
wsp::State wsp::fftw::GetPhaseCorrelation2d(double *o_poc, const T *in_reference, const T *in_template,
                                                u32 width, u32 height, double low_pass_ratio)
{
    double crossSpectrum;
    int i; //! iterators
    u32 x, y;
    int w = (int)width;
    int h = (int)height;
    int length = w*h;
    int M = (w-1)/2;
        
    //! allocate FFTW input and output arrays
    fftw_complex *img1 = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * length);
    fftw_complex *img2 = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * length);
    fftw_complex *res  = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * length);    
    
    //! setup FFTW plans
    fftw_plan fft_img1 = fftw_plan_dft_2d(w, h, img1, img1, FFTW_FORWARD,  FFTW_ESTIMATE);
    fftw_plan fft_img2 = fftw_plan_dft_2d(w, h, img2, img2, FFTW_FORWARD,  FFTW_ESTIMATE);
    fftw_plan ifft_res = fftw_plan_dft_2d(w, h, res,  res,  FFTW_BACKWARD, FFTW_ESTIMATE);
    
    //! load images' data to FFTW input
    for(i=0; i<length; i++){
        img1[i][0] = (double)in_reference[i];
        img2[i][0] = (double)in_template[i];
        img1[i][1] = 0.0;
        img2[i][1] = 0.0;
    }
    
    //! obtain the FFT of img1 and img2
    fftw_execute(fft_img1);
    fftw_execute(fft_img2);

    //! obtain the cross power spectrum
    for(i=0; i<length; i++){
        //! remove high freq(low pass filter) because it include many noise
        //if(abs((int)i-M) > (int)(M*low_pass_ratio)){
        //    res[i][0] = 0;
        //    res[i][1] = 0;
        //    continue;
        //}
        //! corr can be represented as F(k1, k2)*conj(G(k1, k2))
        //!                            = (f0 - f1*i)*(g0 + g1*i)
        //!                            = (f0*g0 + f1*g1) + (f0*g1 - f1*g0)*i
        //res[i][0] = (img1[i][0] * img2[i][0]) + (img1[i][1] * img2[i][1]);
        //res[i][1] = (img1[i][0] * img2[i][1]) - (img1[i][1] * img2[i][0]);

        ////tmp = sqrt( pow( res[i][0], 2.0 ) + pow( res[i][1], 2.0 ) );
        //crossSpectrum = sqrt(res[i][0]*res[i][0] + res[i][1]*res[i][1]);

        //res[i][0] /= crossSpectrum;
        //res[i][1] /= crossSpectrum;
        res[i][0] = img1[i][0];
        res[i][1] = img1[i][1];
    }


    //! obtain the phase correlation array
    fftw_execute(ifft_res);

    //! normalize and copy to result image
    for(i=0; i<length; i++) {
        o_poc[i] = res[i][0]/(double)length;
    }

    //! deallocate FFTW arrays and plans
    fftw_destroy_plan(fft_img1);
    fftw_destroy_plan(fft_img2);
    fftw_destroy_plan(ifft_res);
    fftw_free(img1);
    fftw_free(img2);
    fftw_free(res);
    return WSP_STATE_SUCCESS;
}
