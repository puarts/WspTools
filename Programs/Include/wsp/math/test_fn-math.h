
#include <wsp/common/fn-util.h>
#include "_fn-math.h"
#include "_tpl_fn-math.h"
#include <wsp/image/tpl_cl-image.h>
#include "_tpl_fn-matrix.h>
#include "fn-math-gpl.h"
#include <wsp/image/fn-imgproc.h>
#include <wsp/image/_fn-imgproc.h>


void Test1dPoc();
void Test1dPerLineFft();
void Test2dPerLineFft();
void TestFourier();
void TestDft();
void TestSwap();
void TestIFFT();
void Test2dPoc();
void TestFractalMandelbrot();

void TestFnMath(){
    //TestFourier();
    //Test1dPoc();
    //Test1dPerLineFft();
    //Test2dPerLineFft();
    //Test2dPoc();
    //TestSwap();
    //TestDft();
    //TestIFFT();
    TestFractalMandelbrot();
}

void TestFractalMandelbrot(){
    int k=1;
    float i,j,r,x,y=-16;
    while(y++<15){
        for(x=0; x<84; ++x){
            putchar(" .:-;!/>)|&IH%*#"[k&15]);
            for(i=k=r=0; j=r*r-i*i-2+x/25, i=2*r*i+y/10, j*j+i*i<11 && k++<111; r=j);{
            }
        }
        puts("");
    }
}

void Test2dPoc(){
    //int o_deltax, o_deltay;
    //double thre_pocPeak = 0.00;
    //double low_pass_ratio = 0.75;
    //wsp::Image<double> ref, tpl, poc;
    //ref.LoadImageFile("X:/__Images_Test/Human_Syuno/shuno_l_512.tif", 1);
    //tpl.LoadImageFile("X:/__Images_Test/Human_Syuno/shuno_l_512_shift_x138-y117.tif", 1);
    //double deltaX, deltaY;
    //wsp::Image<double> tmp_poc;
    //int width=ref.width(), height=ref.height();
    //if(ref.IsTheSameSize(tpl)==false){
    //    WSP_ERROR_LOG("The size of input images are different\n");
    //    return;
    //}
    //if(ref.num_of_channels()!=1){ WSP_ERROR_LOG("The num of channels must be 1\n"); return; }

    //int div;
    //int length = (int)width*height;
    //wsp::State state;

    ////state=wsp::img::GetPhaseCorrelationFFT2D(pocData, refData, tplData, width, height, low_pass_ratio);
    //state=wsp::math::GetPhaseCorrelationDFT2D(tmp_poc, ref, tpl, low_pass_ratio);
    //if(state!=WSP_STATE_SUCCESS){
    //    WSP_ERROR_LOG("Getting POC failed\n");
    //    //delete[] refData; delete[] tplData; delete[] pocData;
    //    return;
    //}
    ////tmp_poc.Normalize(0, 255);
    ////tmp_poc.SaveImage("poc_peak.png");
    //tmp_poc.SetSize(width, height, 1);

    ////if(wsp::GetMax(pocData, width*height)<thre_pocPeak){
    //if(wsp::GetMax(tmp_poc.data(), width*height)<thre_pocPeak){
    //    o_deltax=DBL_MAX; o_deltay=DBL_MAX;
    //    return;
    //}

    ////! find the maximum value and its location
    //deltaX = wsp::img::GetPeakSubpixel(tmp_poc.data(), length, 1, low_pass_ratio);
    //div = floor(deltaX/(double)width);
    //deltaX = deltaX-(double)div*width;
    //if(deltaX>(double)(width/2)){ deltaX-=width;}

    ////wsp::math::Transpose<double>(tmp_poc.data(), width, height);
    //tmp_poc.Transpose();
    //deltaY = wsp::img::GetPeakSubpixel(tmp_poc.data(), length, 1, low_pass_ratio);
    //div = floor(deltaY/(double)height);
    //deltaY = deltaY-(double)div*height;
    //if(deltaY>(double)(height/2)){ deltaY-=height;}
    ////wsp::math::Transpose<double>(tmp_poc.data(), width, height);

    ////printf("SUBPIXEL UNIT: Peak is at %f %f\n", deltaX, deltaY);

    ////! store data
    //tmp_poc.Transpose();
    //poc=tmp_poc;
    //o_deltax = deltaX;
    //o_deltay = deltaY;
    //poc.SaveImage("test_poc_fft.bmp");

    ////wsp::Image<double> ref, tpl, poc;
    ////ref.LoadImageFile("X:/__Images_Test/Human_Syuno/shuno_l_512.tif");
    ////tpl.LoadImageFile("X:/__Images_Test/Human_Syuno/shuno_l_512_shift_x138-y117.tif");
    //////ref.LoadImageFile("X:/__Images_Test/Human_Syuno/shuno_l_512.tif", 1);
    //////tpl.LoadImageFile("X:/__Images_Test/Human_Syuno/shuno_l_512_shift_x138-y117.tif", 1);
    ////int w = ref.width(), h = ref.height();
    ////ref.SetSize(w, h, 2);
    ////tpl.SetSize(w, h, 2);
    ////poc.SetSize(w, h, 2);
    ////printf("w=%d, h=%d\n", w, h);
    ////wsp::math::Get2dFftPoc(poc.data(), NULL, ref.data(), tpl.data(), w, h, 1.0);
}



void TestIFFT(){
    wsp::Image<double> img, img_i;
    img.LoadImageFile("X:/__Images_Test/Human_Syuno/shuno_l_512.tif", 1);
    int w = img.width(), h = img.height();
    img.SetSize(w, h, 2);
    //img_i.SetSize(w, h, 1);
    //wsp::math::FFT1dPerLine(img.data(), img_i.data(), w, h, -1, false);
    wsp::math::FFT1dPerLine((complex64*)img.data(), w, h, -1, false);
    wsp::math::FFT1dPerLine((complex64*)img.data(), w, h, -1, true);
    //for(int i=0; i<w*h; ++i){
    //    img[i*2] = exp(exp(exp(img[i*2])));
    //    img[i*2+1] = exp(exp(exp(img[i*2+1])));
    //}
    img.SaveImage("test_ifft.png");
}

void TestDft(){
    int size;
    int m = 23;
    size = pow(2.0, m);
    //size = 32;
    double *complex = WSP_NEW double[size*2];
    double *real = WSP_NEW double[size];
    double *imag = WSP_NEW double[size];
    double *src_r = WSP_NEW double[size];
    double *src_i = WSP_NEW double[size];
    double *dst_r = WSP_NEW double[size];
    double *dst_i = WSP_NEW double[size];
    for(int i=0; i<size; ++i){
        real[i] = sin(i*0.1) + cos(i*0.3);
        imag[i] = 0;
        src_r[i] = sin(i*0.1) + cos(i*0.3);
        src_i[i] = 0;
        complex[i*2] = sin(i*0.1) + cos(i*0.3);
        complex[i*2+1] = 0;
    }
    //ShowComplexArray(real, imag, size);
    float sum_time=0;
    int num_test=30;
    for(int i=0; i<num_test; ++i){
        wsp::StartTimer();
        wsp::math::FFT1d_Opt(real, imag, size, false); // 3.99 sec
        sum_time+=wsp::StopTimer();
    }
    printf("separated data type: %f sec\n", sum_time/(float)num_test);

    sum_time=0;
    for(int i=0; i<num_test; ++i){
        wsp::StartTimer();
        wsp::math::FFT1d((complex64*)complex, size, false); // 
        sum_time+=wsp::StopTimer();
    }
    printf("merged data type: %f sec\n", sum_time/(float)num_test);

    //wsp::math::FFT1d_Opt(real, imag, size, m, false);
    //wsp::StartTimer();
    //wsp::math::DFT1d(dst_r, dst_i, src_r, src_i, size, false);
    //wsp::StopTimer();
    //ShowComplexArray(real, imag, size);
    //wsp::math::FFT1d(real, imag, size, true);
    //wsp::math::FFT1d_Opt(real, imag, size, m, true);
    //ShowComplexArray(real, imag, size);

    //for(int i=size-256; i<size; ++i){
    //    if(i<0){ continue; }
    //    printf("FFT(%f, %f)  FFTComp(%f, %f)  DFT(%f, %f)\n", real[i], imag[i], complex[i*2], complex[i*2+1], dst_r[i], dst_i[i]);
    //    //printf("%f\t%f\n", real[i], imag[i]);
    //}
    delete[] real;
    delete[] imag;
    delete[] src_r;
    delete[] src_i;
    delete[] dst_r;
    delete[] dst_i;
}

void TestSwap(){
    wsp::Image<uchar> img, dst;
    img.LoadImageFile("X:/__Images_Test/Human_Syuno/shuno_l_512.tif", 1);
    int w = img.width(), h = img.height();
    dst.SetSize(w, h, 1);
    //wsp::math::SwapQuadrantHor<uchar>(img.data(), dst.data(), w, h);
    wsp::math::SwapQuadrantVer<uchar>(img.data(), dst.data(), w, h);
    dst.SaveImage("test_swap.png");
}

void Test1dPerLineFft(){
    wsp::Image<double> img, img_i;
    img.LoadImageFile("X:/__Images_Test/Human_Syuno/shuno_l_512.tif", 1);
    int w = img.width(), h = img.height();
    img.SetSize(w, h, 2);
    //img_i.SetSize(w, h, 1);
    //wsp::math::FFT1dPerLine(img.data(), img_i.data(), w, h, -1, false);
    wsp::math::FFT1dPerLine((complex64*)img.data(), w, h, -1, false);
    for(int i=0; i<w*h; ++i){
        img[i*2] = exp(exp(exp(img[i*2])));
        img[i*2+1] = exp(exp(exp(img[i*2+1])));
    }
    img.SaveImage("test_fft1dperline.png");
}

void Test2dPerLineFft(){
    wsp::Image<double> img, tmp_img;
    img.LoadImageFile("X:/__Images_Test/Human_Syuno/shuno_l_512.tif", 1);
    int w = img.width(), h = img.height();
    img.SetSize(w, h, 2);
    tmp_img.SetSize(w, h, 2);
    //img_i.SetSize(w, h, 1);
    //wsp::math::FFT1dPerLine(img.data(), img_i.data(), w, h, -1, false);
    printf("w=%d, h=%d\n", w, h);
    wsp::math::FFT2dPerLine((complex64*)img.data(), w, h, -1, -1, false, (complex64*)tmp_img.data());
    wsp::math::FFT2dPerLine((complex64*)img.data(), w, h, -1, -1, true, (complex64*)tmp_img.data());
    //for(int i=0; i<w*h; ++i)
    //    for(int i_exp=0; i_exp<5; ++i_exp){
    //        img[i*2] = exp(img[i*2]);
    //        img[i*2+1] = exp(img[i*2+1]);
    //    }
    //}
    //for(int i=0; i<w*h; ++i){
    //    for(int j=0; j<2; ++j){
    //        img[i*2] = exp(img[i*2]);
    //        img[i*2+1] = exp(img[i*2+1]);
    //    }
    //}
    //img.Normalize(0, 255);
    img.SaveImage("test_fft2dperline.png");
}

void Test1dPoc(){
    wsp::Image<double> ref, tpl, poc;
    ref.LoadImageFile("X:/__Images_StereoSet/tsucuba_left_512.png");
    tpl.LoadImageFile("X:/__Images_StereoSet/tsucuba_right_512.png");
    //ref.LoadImageFile("X:/__Images_Test/Human_Syuno/shuno_l_512.tif", 1);
    //tpl.LoadImageFile("X:/__Images_Test/Human_Syuno/shuno_l_512_shift_x138-y117.tif", 1);
    int w = ref.width(), h = ref.height();
    ref.SetSize(w, h, 2);
    tpl.SetSize(w, h, 2);
    poc.SetSize(w, h, 2);
    wsp::math::Get1dFftPoc((complex64*)ref.data(), (complex64*)tpl.data(), (complex64*)poc.data(), NULL, w, h, 1.0);


    //wsp::Image<double> ref_real, ref_imag;
    //wsp::Image<double> tpl_real, tpl_imag;
    //wsp::Image<double> o_real, o_imag;

    //ref_real.LoadImageFile("X:/__Images_Test/Human_Syuno/shuno_l_512.tif", 1);
    //int w = ref_real.width(), h = ref_real.height();
    //ref_imag.SetSize(w, h, 1);
    //ref_imag.Reset(0);

    //tpl_real.LoadImageFile("X:/__Images_Test/Human_Syuno/shuno_l_512_shift_x138-y117.tif", 1);
    //tpl_imag.SetSize(w, h, 1);
    //tpl_imag.Reset(0);

    //o_real.SetSize(w, h, 1);
    //o_imag.SetSize(w, h, 1);

    //ref_real.SaveImage("X:/__Images_Test/Human_Syuno/poc_test_ref.png");
    //tpl_real.SaveImage("X:/__Images_Test/Human_Syuno/poc_test_tpl.png");

    //wsp::math::Get1dFftPoc(o_real.data(), o_imag.data(), NULL, NULL, ref_real.data(), ref_imag.data(),
    //                        tpl_real.data(), tpl_imag.data(), w, h, 1.0);

    //for(int i=0; i<o_real.length(); ++i){
    //    o_real[i] = exp(exp(exp(o_real[i])));
    //}
    //o_real.SaveImage("poc_test.png");
}


void ShowComplexArray(double *real, double *imag, int size){
    printf("real ------------\n");
    for(int i=0; i<size; ++i){
        printf("%f\n", real[i]);
    }
    printf("imag ------------\n");
    for(int i=0; i<size; ++i){
        printf("%f\n", imag[i]);
    }
}
void TestFourier(){
    int size;
    int m = 23;
    size = pow(2.0, m);
    //size = 32;
    double *real = new double[size];
    double *imag = new double[size];
    for(int i=0; i<size; ++i){
        real[i] = sin(i*0.1) + cos(i*0.3);
        imag[i] = 0;
    }
    //ShowComplexArray(real, imag, size);
    wsp::StartTimer();
    wsp::math::FFT1d(real, imag, size, false); // 3.99 sec
    //wsp::math::FFT1d_Opt(real, imag, size, m, false);
    //ShowComplexArray(real, imag, size);
    wsp::math::FFT1d(real, imag, size, true);
    //wsp::math::FFT1d_Opt(real, imag, size, m, true);
    //ShowComplexArray(real, imag, size);
    wsp::StopTimer();

    for(int i=size-256; i<size; ++i){
        if(i<0){ continue; }
        printf("%f\t%f\n", real[i], imag[i]);
    }
    delete[] real;
    delete[] imag;
}