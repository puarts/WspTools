
#include "fn-math-gpl.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <wsp/math/_tpl_fn-math.h>
#include <wsp/math/_fn-math.h>

#include <opencv2/gpu/gpu.hpp>
#include <wsp/common/fn-debug.h>
#include <stdexcept>

// ------------------------------------------------------------------
// Fourier Transform
// ------------------------------------------------------------------
wsp::State wsp::opencv::FFT2D_CvGpu(cv::Mat &io_mat, bool dft1d, bool dft_inverse){
    int M, N;
    cv::gpu::GpuMat d_complex(io_mat);
    cv::Mat padded, complex;

    if( io_mat.empty() ){
        WSP_COMMON_ERROR_LOG("Input matrix is empty\n");
        return WSP_STATE_FAILURE;
    }

    //! Get DFT size (power of 2) and fix image as optimal size -------
    //M = cv::getOptimalDFTSize( io_mat.rows );
    //N = cv::getOptimalDFTSize( io_mat.cols );
    //cv::copyMakeBorder(io_mat, padded, 0, M-io_mat.rows, 0, N-io_mat.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
    // ------------------

    //! make complex data -------------------------------------
    //cv::Mat planes[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
    //cv::merge(planes, 2, complex);
    //d_complex = complex;
    // ------------------
    //io_mat.copyTo((cv::Mat)d_complex);

    if(dft_inverse==true)
    {
        cv::gpu::dft(d_complex, d_complex, io_mat.size(), cv::DFT_INVERSE);
        //cv::gpu::dft(d_complex, d_complex, padded.size(), cv::DFT_INVERSE);
    }
    else
    {
        cv::gpu::dft(d_complex, d_complex, io_mat.size());
        //cv::gpu::dft(d_complex, d_complex, padded.size(), cv::DFT_ROWS);
    }

    io_mat=d_complex;

    int cx = io_mat.cols/2;
    int cy = io_mat.rows/2;

    // rearrange the quadrants of Fourier image
    // so that the origin is at the image center
    //cv::Mat tmp;
    //cv::Mat q0(io_mat, cv::Rect(0, 0, cx, cy));
    //cv::Mat q1(io_mat, cv::Rect(cx, 0, cx, cy));
    //cv::Mat q2(io_mat, cv::Rect(0, cy, cx, cy));
    //cv::Mat q3(io_mat, cv::Rect(cx, cy, cx, cy));

    //q0.copyTo(tmp);
    //q3.copyTo(q0);
    //tmp.copyTo(q3);

    //q1.copyTo(tmp);
    //q2.copyTo(q1);
    //tmp.copyTo(q2);

    return WSP_STATE_SUCCESS;
}
wsp::State wsp::opencv::GetPOC_CvGpu_Double(double *o_poc, double *o_pocAv, const double *in_ref, const double *in_tpl, int width, int height, double low_pass_ratio, bool dft1d)
{
    if(o_poc==NULL && o_pocAv==NULL){
        WSP_COMMON_ERROR_LOG("Both of the destinations, o_poc and o_pocAv are NULL\n");
        return WSP_STATE_FAILURE;
    }
    float crossSpectrum;
    int i, j, x, y; //! iterators
    int w = (int)width;
    int h = (int)height;
    int length = w*h;
    int hw = w/2;
    int hh = h/2;
    //wsp::Image<double> imgRe;
    cv::Mat mat_tpl(w, h, CV_32FC2, cv::Scalar(0));
    cv::Mat mat_ref(w, h, CV_32FC2, cv::Scalar(0));
    cv::Mat mat_poc(w, h, CV_32FC2, cv::Scalar(0));
    cv::Mat ftMat_tpl(w, h, CV_32FC2, cv::Scalar(0));
    cv::Mat ftMat_ref(w, h, CV_32FC2, cv::Scalar(0));
    cv::Mat ftMat_cor(w, h, CV_32FC2, cv::Scalar(0));
    cv::Mat ftMat_corAv(w, 1, CV_32FC2, cv::Scalar(0));
    cv::gpu::GpuMat gpuMat(w, h, CV_32FC2, cv::Scalar(0));

    //imgRe.SetSize(w, h);
    //imgRe.Reset(0);

    //! apply hann window
    //for(i=0; i<length; i+=w){
    //    wsp::math::ApplyHannWindow_Complex(&img1[i], w);
    //    wsp::math::ApplyHannWindow_Complex(&img2[i], w);
    //}

    for(y=0; y<h; y++){
        for(x=0; x<w; x++){
            mat_tpl.at<float>(y, x*2) = (float)in_tpl[y*w + x];
            mat_ref.at<float>(y, x*2) = (float)in_ref[y*w + x];
            //mat_tpl.at<float>(y, x*2+1) = 0.0;
            //mat_ref.at<float>(y, x*2+1) = 0.0;
        }
    }

    gpuMat = mat_tpl;
    //mat_tpl.copyTo((cv::Mat)gpuMat);
    if(dft1d==false){
        cv::gpu::dft(gpuMat, gpuMat, cv::Size(w, h));
    }else{
        cv::gpu::dft(gpuMat, gpuMat, cv::Size(w, h), cv::DFT_ROWS);
    }
    //gpuMat.copyTo((cv::gpu::GpuMat)ftMat_tpl);
    ftMat_tpl = gpuMat;
    //mat_ref.copyTo((cv::Mat)gpuMat);
    gpuMat = mat_ref;
    if(dft1d==false){
        cv::gpu::dft(gpuMat, gpuMat, cv::Size(w, h));
    }else{
        cv::gpu::dft(gpuMat, gpuMat, cv::Size(w, h), cv::DFT_ROWS);
    }
    //gpuMat.copyTo((cv::gpu::GpuMat)ftMat_ref);
    ftMat_ref = gpuMat;


    //! obtain the cross power spectrum
    for(y=0; y<h; y++){
        for(x=0; x<w; x++){
            i = y*w + x;
            //! remove high freq(low pass filter) because it include many noise
            if(abs((int)x-hw) < (int)(hw*(1-low_pass_ratio))){
                ftMat_cor.at<float>(y, x*2)  = 0;
                ftMat_cor.at<float>(y, x*2+1) = 0;
                //imgRe[i] = 0;
                continue;
            }
            //! corr can be obtained by F(k1, k2)*conj(G(k1, k2))
            //!                            = (f0 - f1*i)*(g0 + g1*i)
            //!                            = (f0*g0 + f1*g1) + (f0*g1 - f1*g0)*i
            ftMat_cor.at<float>(y, x*2)  = (ftMat_tpl.at<float>(y, x*2) * ftMat_ref.at<float>(y, x*2)) + (ftMat_tpl.at<float>(y, x*2+1) * ftMat_ref.at<float>(y, x*2+1));
            ftMat_cor.at<float>(y, x*2+1)= (ftMat_tpl.at<float>(y, x*2) * ftMat_ref.at<float>(y, x*2+1)) - (ftMat_tpl.at<float>(y, x*2+1) * ftMat_ref.at<float>(y, x*2));

            //tmp = sqrt( pow( res[i][0], 2.0 ) + pow( res[i][1], 2.0 ) );
            crossSpectrum = sqrt(ftMat_cor.at<float>(y, x*2)*ftMat_cor.at<float>(y, x*2) + ftMat_cor.at<float>(y, x*2+1)*ftMat_cor.at<float>(y, x*2+1));
            //printf("crossSpec=%f\n", crossSpectrum);
            if(crossSpectrum!=0){
                ftMat_cor.at<float>(y, x*2)  /=crossSpectrum;
                ftMat_cor.at<float>(y, x*2+1)/=crossSpectrum;
            }

            //! debug
            //ftMat_cor.at<float>(y, x*2) = ftMat_ref.at<float>(y, x*2);
            //ftMat_cor.at<float>(y, x*2+1) = ftMat_ref.at<float>(y, x*2+1);
            //res[i][0] = img1[i][0];
            //res[i][1] = img1[i][1];
            //res[i][0] /= 256*N;
            //res[i][1] /= 256*N;
            //imgRe[i] = log(sqrt(img1[i][0]*img1[i][0] + img1[i][1]*img1[i][1]));
            //imgRe[i] = log(sqrt(res[i][0]*res[i][0] + res[i][1]*res[i][1]));
            //printf("%3d %3d: %f %f\n", x, y, ftMat_cor.at<float>(y, x*2), ftMat_cor.at<float>(y, x*2+1));
        }
    }
    //ftMat_cor = ftMat_tpl;
    ////imgRe.SaveAsTiff("fft_spectr_img1.tif");

    if(o_pocAv!=NULL){
        //! get average of correlation
        for(x=0; x<w; x++){
            ftMat_corAv.at<float>(0, x*2) = 0;
            ftMat_corAv.at<float>(0, x*2+1) = 0;
            for(y=0; y<h; y++){
                ftMat_corAv.at<float>(0, x*2) += ftMat_cor.at<float>(y, x*2);
                ftMat_corAv.at<float>(0, x*2+1) += ftMat_cor.at<float>(y, x*2+1);
            }
            ftMat_corAv.at<float>(0, x*2) /= w;
            ftMat_corAv.at<float>(0, x*2+1) /= w;
        }

        //ftMat_corAv.copyTo((cv::Mat)gpuMat);
        gpuMat = ftMat_corAv;
        if(dft1d==false){
            cv::gpu::dft(gpuMat, gpuMat, cv::Size(w, h));
        }else{
            cv::gpu::dft(gpuMat, gpuMat, cv::Size(w, h), cv::DFT_ROWS);
        }
        //gpuMat.copyTo((cv::gpu::GpuMat)mat_poc);
        mat_poc = gpuMat;

        for(x=0; x<w; x++){
            o_pocAv[x] = (double)mat_poc.at<float>(0, x*2)/(double)w;
        }
    }

    //! horizontal ifft
    if(o_poc!=NULL){
        //ftMat_cor.copyTo((cv::Mat)gpuMat);
        gpuMat = ftMat_cor;
        if(dft1d==false){
            cv::gpu::dft(gpuMat, gpuMat, cv::Size(w, h));
        }else{
            cv::gpu::dft(gpuMat, gpuMat, cv::Size(w, h), cv::DFT_ROWS);
        }
        //gpuMat.copyTo((cv::gpu::GpuMat)mat_poc);
        mat_poc = gpuMat;
        //wsp::math::FFT2D_CvGpu(mat, true);
        //wsp::math::SwapQuadrantHV_fftw(res, (u32)N, (u32)N);

        //! normalize and copy to result image
        for(y=0; y<h; y++){
            for(x=0; x<w; x++){
                i = y*w+x;
                //o_poc[i] = (double)mat.at<float>(y, x*2)/(double)(length);
                o_poc[i] = (double)mat_poc.at<float>(y, x*2)/(double)(length);
                //printf("%d %d: %f\n", x,y,mat.at<float>(y, x*2));
            }
        }
    }


    return WSP_STATE_SUCCESS;
}
