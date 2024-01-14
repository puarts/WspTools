#include "fn-image_proc.h"
#include "fn-math-gpl.h"

#include <wsp/common/fn-debug.h>
#include <wsp/math/_tpl_fn-math.h>
#include <wsp/math/_fn-math_core.h>

#include <fftw3.h>

#include <wsp/fftw/fn-math-gpl.h>
#include "detail-fft.h"


wsp::State wsp::fftw::GetPhaseCorrelationDFT1DPerLine_fftw(
    complex64 *o_poc,
    complex64 *o_pocAv,
    const complex64 *in_ref,
    const complex64 *in_tpl,
    int width,
    int height,
    double low_pass_ratio
){
    if (o_poc == NULL && o_pocAv == NULL){
        WSP_COMMON_ERROR_LOG("Both of the destinations, o_poc and o_pocAv are NULL\n");
        return WSP_STATE_FAILURE;
    }
    double crossSpectrum;
    int i, j, x, y; //! iterators
    int w = (int)width;
    int h = (int)height;
    int length = w*h;
    int hw = w / 2;
    int hh = h / 2;
    //wsp::Image<double> imgRe;
    fftw_plan ifft_res;



    // allocate FFTW input and output arrays
    //fftw_complex *img1 = (fftw_complex*)fftw_malloc( sizeof( fftw_complex ) * length);
    //fftw_complex *img2 = (fftw_complex*)fftw_malloc( sizeof( fftw_complex ) * length);
    //fftw_complex *res  = (fftw_complex*)fftw_malloc( sizeof( fftw_complex ) * length);
    //fftw_complex *res_av = (fftw_complex*)fftw_malloc( sizeof( fftw_complex ) * w);

    fftw_complex *img1 = (fftw_complex*)in_ref; //fftw_malloc( sizeof( fftw_complex ) * length);
    fftw_complex *img2 = (fftw_complex*)in_tpl; //fftw_malloc( sizeof( fftw_complex ) * length);
    fftw_complex *res = (fftw_complex*)o_poc; //fftw_malloc( sizeof( fftw_complex ) * length);
    fftw_complex *res_av = (fftw_complex*)o_pocAv; //fftw_malloc( sizeof( fftw_complex ) * w);

    // load images' data to FFTW input
    //for(i=0; i<length; i++){
    //    img1[i][0] = in_ref[i];
    //    img2[i][0] = in_tpl[i];
    //    img1[i][1] = 0.0;
    //    img2[i][1] = 0.0;
    //}

    // apply hann window
    for (i = 0; i<length; i += w){
        WSP_ApplyHannWindow_Complex(&img1[i], w);
        WSP_ApplyHannWindow_Complex(&img2[i], w);
    }

    //imgRe.CopyFrom((double*)img1, NULL, NULL, w, h, 2);
    //imgRe.SaveImage("test-1dpoc-ref_hann.png");
    //imgRe.CopyFrom((double*)img2, NULL, NULL, w, h, 2);
    //imgRe.SaveImage("test-1dpoc-tpl_hann.png");



    // swapping quadrant horizontal
    //wsp::math::SwapQuadrantHor_fftw(img1, (u32)N, (u32)N);
    //wsp::math::SwapQuadrantHor_fftw(img2, (u32)N, (u32)N);

    wsp::fftw::detail::FFT1DPerLine_fftw(img1, w, h);
    wsp::fftw::detail::FFT1DPerLine_fftw(img2, w, h);

    // swapping quadrant 1 and 3, 2 and 4
    //wsp::math::SwapQuadrantHor_fftw(img1, (u32)w, (u32)h);
    //wsp::math::SwapQuadrantHor_fftw(img2, (u32)w, (u32)h);

    //imgRe.CopyFrom((double*)img1, NULL, NULL, w, h, 2);
    //imgRe.SaveImage("test-1dpoc-ref_fft.png");
    //imgRe.CopyFrom((double*)img2, NULL, NULL, w, h, 2);
    //imgRe.SaveImage("test-1dpoc-tpl_fft.png");

    // obtain the cross power spectrum
    for (y = 0; y<h; y++){
        for (x = 0; x<w; x++){
            i = y*w + x;
            //! remove high freq(low pass filter) because it include many noise
            if (abs((int)x - hw) < (int)(hw*(1 - low_pass_ratio))){
                res[i][0] = 0;
                res[i][1] = 0;
                //imgRe[i] = 0;
                continue;
            }
            //! corr can be obtained by F(k1, k2)*conj(G(k1, k2))
            //!                            = (f0 - f1*i)*(g0 + g1*i)
            //!                            = (f0*g0 + f1*g1) + (f0*g1 - f1*g0)*i
            res[i][0] = (img1[i][0] * img2[i][0]) + (img1[i][1] * img2[i][1]);
            res[i][1] = (img1[i][0] * img2[i][1]) - (img1[i][1] * img2[i][0]);

            //tmp = sqrt( pow( res[i][0], 2.0 ) + pow( res[i][1], 2.0 ) );
            crossSpectrum = sqrt(res[i][0] * res[i][0] + res[i][1] * res[i][1]);

            if (crossSpectrum != 0){
                res[i][0] /= crossSpectrum;
                res[i][1] /= crossSpectrum;
            }

            // debug
            //res[i][0] = img1[i][0];
            //res[i][1] = img1[i][1];
            //res[i][0] /= 256*N;
            //res[i][1] /= 256*N;
            //imgRe[i] = log(sqrt(img1[i][0]*img1[i][0] + img1[i][1]*img1[i][1]));
            //imgRe[i] = log(sqrt(res[i][0]*res[i][0] + res[i][1]*res[i][1]));
        }
    }
    //wsp::math::GetCrossPowerSpectrum((complex64*)img1, (complex64*)img2, (complex64*)res, length);
    //if(low_pass_ratio<1.0){
    //    wsp::math::RemoveCentricVerticalArea(o_poc, width, height, low_pass_ratio);
    //    int x, y, i;
    //    int hw = width/2;
    //    for(y=0; y<height; y++){
    //        for(x=0; x<width; x++){
    //            i = y*width + x;
    //            // remove high freq(low pass filter) because it include many noise
    //            if(abs(x-hw) < (int)(hw*(1.0-low_pass_ratio))){
    //                o_poc[i][0] = 0;
    //                o_poc[i][1] = 0;
    //            }
    //        }
    //    }
    //}    
    //imgRe.CopyFrom((double*)res, NULL, NULL, w, h, 2);
    //imgRe.SaveImage("test-1dpoc-cross_sectrum.png");

    if (o_pocAv != NULL){
        // get average of correlation
        for (x = 0; x<w; x++){
            res_av[x][0] = 0;
            res_av[x][1] = 0;
            for (y = 0; y<h; y++){
                res_av[x][0] += res[y*w + x][0];
                res_av[x][1] += res[y*w + x][1];
                //res_av[x][0] *= res[y*w+x][0];
                //res_av[x][1] *= res[y*w+x][1];
            }
            res_av[x][0] /= w;
            res_av[x][1] /= w;
        }

        //wsp::math::SwapQuadrantHor_fftw(res, (u32)w, (u32)h);

        ifft_res = fftw_plan_dft_1d(w, &res_av[0], &res_av[0], FFTW_BACKWARD, FFTW_ESTIMATE);
        fftw_execute(ifft_res);
        fftw_destroy_plan(ifft_res);

        for (x = 0; x<w; x++){
            o_pocAv[x][0] = res_av[x][0] / (double)w;
        }
    }

    // horizontal ifft
    if (o_poc != NULL){
        wsp::fftw::detail::FFT1DPerLine_fftw(res, w, h, FFTW_BACKWARD);
        //wsp::math::SwapQuadrantHV_fftw(res, (u32)N, (u32)N);

        //! normalize and copy to result image
        for (i = 0; i<length; i++){
            o_poc[i][0] = res[i][0] / (double)(length);
        }
    }

    // deallocate FFTW arrays and plans
    //fftw_free(img1);
    //fftw_free(img2);
    //fftw_free(res);
    //fftw_free(res_av);
    return WSP_STATE_SUCCESS;
}

wsp::State wsp::fftw::GetPhaseCorrelationDFT1DPerLine(
    double *o_poc,
    double *o_pocAv,
    const double *in_ref,
    const double *in_tpl,
    int width,
    int height,
    double low_pass_ratio
){
    if (o_poc == NULL && o_pocAv == NULL){
        WSP_COMMON_ERROR_LOG("Both of the destinations, o_poc and o_pocAv are NULL\n");
        return WSP_STATE_FAILURE;
    }
    double crossSpectrum;
    int i, j, x, y; //! iterators
    int w = (int)width;
    int h = (int)height;
    int length = w*h;
    int hw = w / 2;
    int hh = h / 2;
    //wsp::Image<double> imgRe;
    fftw_plan ifft_res;



    // allocate FFTW input and output arrays
    fftw_complex *img1 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)* length);
    fftw_complex *img2 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)* length);
    fftw_complex *res = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)* length);
    fftw_complex *res_av = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)* w);

    // load images' data to FFTW input
    for (i = 0; i<length; i++){
        img1[i][0] = in_ref[i];
        img2[i][0] = in_tpl[i];
        img1[i][1] = 0.0;
        img2[i][1] = 0.0;
    }

    // apply hann window
    for (i = 0; i<length; i += w){
        WSP_ApplyHannWindow_Complex(&img1[i], w);
        WSP_ApplyHannWindow_Complex(&img2[i], w);
    }

    //imgRe.CopyFrom((double*)img1, NULL, NULL, w, h, 2);
    //imgRe.SaveImage("test-1dpoc-ref_hann.png");
    //imgRe.CopyFrom((double*)img2, NULL, NULL, w, h, 2);
    //imgRe.SaveImage("test-1dpoc-tpl_hann.png");



    // swapping quadrant horizontal
    //wsp::math::SwapQuadrantHor_fftw(img1, (u32)N, (u32)N);
    //wsp::math::SwapQuadrantHor_fftw(img2, (u32)N, (u32)N);

    wsp::fftw::detail::FFT1DPerLine_fftw(img1, (u32)w, (u32)h);
    wsp::fftw::detail::FFT1DPerLine_fftw(img2, (u32)w, (u32)h);

    // swapping quadrant 1 and 3, 2 and 4
    //wsp::math::SwapQuadrantHor_fftw(img1, (u32)w, (u32)h);
    //wsp::math::SwapQuadrantHor_fftw(img2, (u32)w, (u32)h);

    //imgRe.CopyFrom((double*)img1, NULL, NULL, w, h, 2);
    //imgRe.SaveImage("test-1dpoc-ref_fft.png");
    //imgRe.CopyFrom((double*)img2, NULL, NULL, w, h, 2);
    //imgRe.SaveImage("test-1dpoc-tpl_fft.png");

    // obtain the cross power spectrum
    for (y = 0; y<h; y++){
        for (x = 0; x<w; x++){
            i = y*w + x;
            //! remove high freq(low pass filter) because it include many noise
            if (abs((int)x - hw) < (int)(hw*(1 - low_pass_ratio))){
                res[i][0] = 0;
                res[i][1] = 0;
                //imgRe[i] = 0;
                continue;
            }
            //! corr can be obtained by F(k1, k2)*conj(G(k1, k2))
            //!                            = (f0 - f1*i)*(g0 + g1*i)
            //!                            = (f0*g0 + f1*g1) + (f0*g1 - f1*g0)*i
            res[i][0] = (img1[i][0] * img2[i][0]) + (img1[i][1] * img2[i][1]);
            res[i][1] = (img1[i][0] * img2[i][1]) - (img1[i][1] * img2[i][0]);

            //tmp = sqrt( pow( res[i][0], 2.0 ) + pow( res[i][1], 2.0 ) );
            crossSpectrum = sqrt(res[i][0] * res[i][0] + res[i][1] * res[i][1]);

            if (crossSpectrum != 0){
                res[i][0] /= crossSpectrum;
                res[i][1] /= crossSpectrum;
            }

            // debug
            //res[i][0] = img1[i][0];
            //res[i][1] = img1[i][1];
            //res[i][0] /= 256*N;
            //res[i][1] /= 256*N;
            //imgRe[i] = log(sqrt(img1[i][0]*img1[i][0] + img1[i][1]*img1[i][1]));
            //imgRe[i] = log(sqrt(res[i][0]*res[i][0] + res[i][1]*res[i][1]));
        }
    }
    //imgRe.CopyFrom((double*)res, NULL, NULL, w, h, 2);
    //imgRe.SaveImage("test-1dpoc-cross_sectrum.png");

    if (o_pocAv != NULL){
        //! get average of correlation
        for (x = 0; x<w; x++){
            res_av[x][0] = 0;
            res_av[x][1] = 0;
            for (y = 0; y<h; y++){
                res_av[x][0] += res[y*w + x][0];
                res_av[x][1] += res[y*w + x][1];
                //res_av[x][0] *= res[y*w+x][0];
                //res_av[x][1] *= res[y*w+x][1];
            }
            res_av[x][0] /= w;
            res_av[x][1] /= w;
        }

        //wsp::math::SwapQuadrantHor_fftw(res, (u32)w, (u32)h);

        ifft_res = fftw_plan_dft_1d(w, &res_av[0], &res_av[0], FFTW_BACKWARD, FFTW_ESTIMATE);
        fftw_execute(ifft_res);
        fftw_destroy_plan(ifft_res);

        for (x = 0; x<w; x++){
            o_pocAv[x] = res_av[x][0] / (double)w;
        }
    }

    //! horizontal ifft
    if (o_poc != NULL){
        wsp::fftw::detail::FFT1DPerLine_fftw(res, (u32)w, (u32)h, FFTW_BACKWARD);
        //wsp::math::SwapQuadrantHV_fftw(res, (u32)N, (u32)N);

        //! normalize and copy to result image
        for (i = 0; i<length; i++){
            o_poc[i] = res[i][0] / (double)(length);
        }
    }

    //! deallocate FFTW arrays and plans
    fftw_free(img1);
    fftw_free(img2);
    fftw_free(res);
    fftw_free(res_av);
    return WSP_STATE_SUCCESS;
}


wsp::State wsp::fftw::GetPhaseCorrelationFFT1DPerLine3(double *o_poc3, double *o_pocAv3, double * const in_ref3, double * const in_tpl3, int width, int height, double low_pass_ratio)
{
    if (o_poc3 == NULL && o_pocAv3 == NULL){
        WSP_COMMON_ERROR_LOG("Both of the destinations, o_poc and o_pocAv are NULL\n");
        return WSP_STATE_FAILURE;
    }
    if (!wsp::math::IsPowerOfTwo(width)){
        WSP_COMMON_ERROR_LOG("width must be power of two: width=%d\n", width);
        return WSP_STATE_FAILURE;
    }
    double crossSpectrum;
    int i, j, itr_rgb, x, y; //! iterators
    int w = (int)width;
    int h = (int)height;
    int length = w*h;
    int hw = w / 2;
    int hh = h / 2;
    //wsp::Image<double> imgRe;
    fftw_plan ifft_res;
    fftw_complex *img1[3], *img2[3], *res[3], *res_av[3];
    fftw_complex rgbArray1[4], rgbArray2[4]; //! the reason why rgb has 4 element is to apply FFT

    //imgRe.SetSize(w, h);
    //imgRe.Reset(0);

    //! allocate FFTW input and output arrays
    for (itr_rgb = 0; itr_rgb<3; itr_rgb++){
        img1[itr_rgb] = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)* length);
        img2[itr_rgb] = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)* length);
        res[itr_rgb] = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)* length);
        res_av[itr_rgb] = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)* w);
        //! load images' data to FFTW input
        for (i = 0; i<length; i++){
            img1[itr_rgb][i][0] = in_ref3[i * 3 + itr_rgb];
            img2[itr_rgb][i][0] = in_tpl3[i * 3 + itr_rgb];
            img1[itr_rgb][i][1] = 0.0;
            img2[itr_rgb][i][1] = 0.0;
        }
        //! apply hann window
        for (i = 0; i<length; i += w){
            WSP_ApplyHannWindow_Complex(&img1[itr_rgb][i], w);
            WSP_ApplyHannWindow_Complex(&img2[itr_rgb][i], w);
        }
        //! swapping quadrant horizontal
        //wsp::math::SwapQuadrantHor_fftw(img1, (u32)N, (u32)N);
        //wsp::math::SwapQuadrantHor_fftw(img2, (u32)N, (u32)N);
        wsp::fftw::detail::FFT1DPerLine_fftw(img1[itr_rgb], (u32)w, (u32)h);
        wsp::fftw::detail::FFT1DPerLine_fftw(img2[itr_rgb], (u32)w, (u32)h);

        //! swapping quadrant 1 and 3, 2 and 4
        //wsp::math::SwapQuadrantHor_fftw(img1, (u32)w, (u32)h);
        //wsp::math::SwapQuadrantHor_fftw(img2, (u32)w, (u32)h);
    }

    //! Apply FFT on rgb channels
    for (i = 0; i<length; i++){
        for (itr_rgb = 0; itr_rgb<3; itr_rgb++){
            rgbArray1[itr_rgb][0] = img1[itr_rgb][i][0];
            rgbArray1[itr_rgb][1] = img1[itr_rgb][i][1];
            rgbArray2[itr_rgb][0] = img2[itr_rgb][i][0];
            rgbArray2[itr_rgb][1] = img2[itr_rgb][i][1];
        }
        //rgbArray[3][0]=0.0;
        //rgbArray[3][1]=0.0;
        rgbArray1[3][0] = rgbArray1[0][0];
        rgbArray1[3][1] = rgbArray1[0][1];
        rgbArray2[3][0] = rgbArray2[0][0];
        rgbArray2[3][1] = rgbArray2[0][1];
        wsp::fftw::detail::FFT1DPerLine_fftw(rgbArray1, 4, 1);
        wsp::fftw::detail::FFT1DPerLine_fftw(rgbArray2, 4, 1);
        for (itr_rgb = 0; itr_rgb<3; itr_rgb++){
            img1[itr_rgb][i][0] = rgbArray1[itr_rgb][0];
            img1[itr_rgb][i][1] = rgbArray1[itr_rgb][1];
        }
    }


    //! obtain the cross power spectrum
    for (y = 0; y<h; y++){
        for (x = 0; x<w; x++){
            i = y*w + x;
            //! remove high freq(low pass filter) because it include many noise
            if (abs((int)x - hw) < (int)(hw*(1 - low_pass_ratio))){
                for (itr_rgb = 0; itr_rgb<3; itr_rgb++){
                    res[itr_rgb][i][0] = 0;
                    res[itr_rgb][i][1] = 0;
                    //imgRe[i] = 0;
                }
                continue;
            }
            //! corr can be obtained by F(k1, k2)*conj(G(k1, k2))
            //!                            = (f0 - f1*i)*(g0 + g1*i)
            //!                            = (f0*g0 + f1*g1) + (f0*g1 - f1*g0)*i

            for (itr_rgb = 0; itr_rgb<3; itr_rgb++){
                res[itr_rgb][i][0] = (img1[itr_rgb][i][0] * img2[itr_rgb][i][0]) + (img1[itr_rgb][i][1] * img2[itr_rgb][i][1]);
                res[itr_rgb][i][1] = (img1[itr_rgb][i][0] * img2[itr_rgb][i][1]) - (img1[itr_rgb][i][1] * img2[itr_rgb][i][0]);
                //tmp = sqrt( pow( res[i][0], 2.0 ) + pow( res[i][1], 2.0 ) );
                crossSpectrum = sqrt(res[itr_rgb][i][0] * res[itr_rgb][i][0] + res[itr_rgb][i][1] * res[itr_rgb][i][1]);

                if (crossSpectrum != 0){
                    res[itr_rgb][i][0] /= crossSpectrum;
                    res[itr_rgb][i][1] /= crossSpectrum;
                }
            }

            //! debug
            //res[i][0] = img1[i][0];
            //res[i][1] = img1[i][1];
            //res[i][0] /= 256*N;
            //res[i][1] /= 256*N;
            //imgRe[i] = log(sqrt(img1[i][0]*img1[i][0] + img1[i][1]*img1[i][1]));
            //imgRe[i] = log(sqrt(res[i][0]*res[i][0] + res[i][1]*res[i][1]));
        }
    }
    //imgRe.SaveAsTiff("fft_spectr_img1.tif");

    if (o_pocAv3 != NULL){
        //! get average of correlation
        for (itr_rgb = 0; itr_rgb<3; itr_rgb++){
            for (x = 0; x<w; x++){
                res_av[itr_rgb][x][0] = 0;
                res_av[itr_rgb][x][1] = 0;
                for (y = 0; y<h; y++){
                    res_av[itr_rgb][x][0] += res[itr_rgb][y*w + x][0];
                    res_av[itr_rgb][x][1] += res[itr_rgb][y*w + x][1];
                    //res_av[x][0] *= res[y*w+x][0];
                    //res_av[x][1] *= res[y*w+x][1];

                }
                res_av[itr_rgb][x][0] /= w;
                res_av[itr_rgb][x][1] /= w;
            }
            ifft_res = fftw_plan_dft_1d(w, &res_av[itr_rgb][0], &res_av[itr_rgb][0], FFTW_BACKWARD, FFTW_ESTIMATE);
            fftw_execute(ifft_res);
            fftw_destroy_plan(ifft_res);
            for (x = 0; x<w; x++){
                o_pocAv3[x * 3 + itr_rgb] = res_av[itr_rgb][x][0] / (double)w;
            }
        }
    }

    //! horizontal ifft
    if (o_poc3 != NULL){
        for (itr_rgb = 0; itr_rgb<3; itr_rgb++){
            wsp::fftw::detail::FFT1DPerLine_fftw(res[itr_rgb], (u32)w, (u32)h, FFTW_BACKWARD);
            //wsp::math::SwapQuadrantHV_fftw(res, (u32)N, (u32)N);

            //! normalize and copy to result image
            for (i = 0; i<length; i++){
                o_poc3[i * 3 + itr_rgb] = res[itr_rgb][i][0] / (double)(length);
            }
        }
    }

    //! deallocate FFTW arrays and plans
    for (itr_rgb = 0; itr_rgb<3; itr_rgb++){
        fftw_free(img1[itr_rgb]);
        fftw_free(img2[itr_rgb]);
        fftw_free(res[itr_rgb]);
        fftw_free(res_av[itr_rgb]);
    }
    return WSP_STATE_SUCCESS;
}


wsp::State wsp::fftw::GetPhaseCorrelationDFT2D(
    double *o_poc, 
    const double *in_ref, 
    const double *in_tpl, 
    int width, 
    int height, 
    double low_pass_ratio
)
{
    if (!wsp::math::IsPowerOfTwo(width)){
        WSP_COMMON_ERROR_LOG("width must be power of two: width=%d\n", width);
        return WSP_STATE_FAILURE;
    }
    double crossSpectrum;
    int i, j, x, y; //! iterators
    int w = (int)width;
    int h = (int)height;
    int length = w*h;
    int hw = w / 2;
    int hh = h / 2;
    //wsp::Image<double> imgRe;
    fftw_plan fft_img1, fft_img2, ifft_res;

    //imgRe.SetSize(w, h);
    //imgRe.Reset(0);

    //! allocate FFTW input and output arrays
    fftw_complex *img1 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)* length);
    fftw_complex *img2 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)* length);
    fftw_complex *res = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)* length);

    //! load images' data to FFTW input
    for (i = 0; i<length; i++){
        img1[i][0] = in_ref[i];
        img2[i][0] = in_tpl[i];
        img1[i][1] = 0.0;
        img2[i][1] = 0.0;
    }

    //! apply hann window
    for (i = 0; i<length; i += w){
        WSP_ApplyHannWindow_Complex(&img1[i], w);
        WSP_ApplyHannWindow_Complex(&img2[i], w);
    }

    //! swapping quadrant horizontal
    //wsp::math::SwapQuadrantHor_fftw(img1, (u32)N, (u32)N);
    //wsp::math::SwapQuadrantHor_fftw(img2, (u32)N, (u32)N);

    //! horizontal fft
    for (i = 0; i<length; i += w){
        //! setup FFTW plans
        fft_img1 = fftw_plan_dft_1d(w, &img1[i], &img1[i], FFTW_FORWARD, FFTW_ESTIMATE);
        fft_img2 = fftw_plan_dft_1d(w, &img2[i], &img2[i], FFTW_FORWARD, FFTW_ESTIMATE);

        //! obtain the FFT of img1 and img2
        fftw_execute(fft_img1);
        fftw_execute(fft_img2);

        //! free memories
        fftw_destroy_plan(fft_img1);
        fftw_destroy_plan(fft_img2);
    }

    //! transpose images
    WSP_Transpose_Complex(img1, (u32)w, (u32)h);
    WSP_Transpose_Complex(img2, (u32)w, (u32)h);

    //! apply hann window
    for (i = 0; i<length; i += h){
        WSP_ApplyHannWindow_Complex(&img1[i], h);
        WSP_ApplyHannWindow_Complex(&img2[i], h);
    }

    //! swap quadrant vertical
    //wsp::math::SwapQuadrantHor_fftw(img1, (u32)N, (u32)N);
    //wsp::math::SwapQuadrantHor_fftw(img2, (u32)N, (u32)N);

    //! vertical fft
    for (i = 0; i<length; i += h){
        //! setup FFTW plans
        fft_img1 = fftw_plan_dft_1d(h, &img1[i], &img1[i], FFTW_FORWARD, FFTW_ESTIMATE);
        fft_img2 = fftw_plan_dft_1d(h, &img2[i], &img2[i], FFTW_FORWARD, FFTW_ESTIMATE);

        //! obtain the FFT of img1 and img2
        fftw_execute(fft_img1);
        fftw_execute(fft_img2);

        //! free memories
        fftw_destroy_plan(fft_img1);
        fftw_destroy_plan(fft_img2);
    }

    WSP_Transpose_Complex(img1, (u32)h, (u32)w);
    WSP_Transpose_Complex(img2, (u32)h, (u32)w);
    //! swapping quadrant 1 and 3, 2 and 4
    //wsp::math::SwapQuadrantHV_fftw(img1, (u32)w, (u32)h);
    //wsp::math::SwapQuadrantHV_fftw(img2, (u32)w, (u32)h);


    //! obtain the cross power spectrum
    for (y = 0; y<h; y++){
        for (x = 0; x<w; x++){
            i = y*w + x;
            //! remove high freq(low pass filter) because it include many noise
            if (abs((int)x - hw) >(int)(hw*low_pass_ratio)
                || abs((int)y - hh) >(int)(hh*low_pass_ratio)){
                res[i][0] = 0;
                res[i][1] = 0;
                continue;
            }
            //! corr can be obtained by F(k1, k2)*conj(G(k1, k2))
            //!                            = (f0 - f1*i)*(g0 + g1*i)
            //!                            = (f0*g0 + f1*g1) + (f0*g1 - f1*g0)*i
            res[i][0] = (img1[i][0] * img2[i][0]) + (img1[i][1] * img2[i][1]);
            res[i][1] = (img1[i][0] * img2[i][1]) - (img1[i][1] * img2[i][0]);

            //tmp = sqrt( pow( res[i][0], 2.0 ) + pow( res[i][1], 2.0 ) );
            crossSpectrum = sqrt(res[i][0] * res[i][0] + res[i][1] * res[i][1]);

            res[i][0] /= crossSpectrum;
            res[i][1] /= crossSpectrum;

            //! debug
            //res[i][0] = img1[i][0];
            //res[i][1] = img1[i][1];
            //res[i][0] /= 256*N;
            //res[i][1] /= 256*N;
            //imgRe[i] = log(sqrt(img1[i][0]*img1[i][0] + img1[i][1]*img1[i][1]));
            //imgRe[i] = log(sqrt(img2[i][0]*img2[i][0] + img2[i][1]*img2[i][1]));
        }
    }
    //imgRe.SaveImage("fft_spectr_img2.tif");

    //wsp::math::SwapQuadrantHV_fftw(res, (u32)w, (u32)h);
    WSP_Transpose_Complex(res, (u32)w, (u32)h);

    //! vertical ifft
    for (i = 0; i<length; i += h){
        ifft_res = fftw_plan_dft_1d(h, &res[i], &res[i], FFTW_BACKWARD, FFTW_ESTIMATE);
        fftw_execute(ifft_res);
        fftw_destroy_plan(ifft_res);
    }

    //! transpose images
    WSP_Transpose_Complex(res, (u32)h, (u32)w);

    //! horizontal ifft
    for (i = 0; i<length; i += w){
        ifft_res = fftw_plan_dft_1d(w, &res[i], &res[i], FFTW_BACKWARD, FFTW_ESTIMATE);
        fftw_execute(ifft_res);
        fftw_destroy_plan(ifft_res);
    }

    //wsp::math::SwapQuadrantHV_fftw(res, (u32)N, (u32)N);

    //! normalize and copy to result image
    for (i = 0; i<length; i++){
        o_poc[i] = res[i][0] / (double)(length);
    }

    //! deallocate FFTW arrays and plans
    fftw_free(img1);
    fftw_free(img2);
    fftw_free(res);
    return WSP_STATE_SUCCESS;
}

