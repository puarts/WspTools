// @file fn-imgproc-gpl.cpp
// @author Junichi Nishikata
// E-mail: junichi.nishikata@gmail.com

#include <iostream>
#include <float.h>
#include <math.h>

//#include <wsp/common/tpl_cl-vector3.h>
//#include <wsp/common/WspRgb.h>
//#include <wsp/common/WspPoint.h>
#ifdef USE_FFTW
#include <wsp/fftw/fn-math-gpl.h>
#endif

#include <wsp/math/_fn-math.h>
extern "C"{
#include <wsp/math/_fn-math_core.h>
}

#include <wsp/image/gpl_tpl_fn-imgproc.h>
#include <wsp/image/tpl_fn-imgproc.hpp>
#include <wsp/image/fn-imgproc.h>
#include <wsp/image/_fn-imgproc.h>

#include "gpl_fn-imgproc.h"
#include "fn-image_proc.h"

using namespace std;

// Stereo -------------------------------------------------------

wsp::State wsp::img::StereoPOC_fftw(
    const wsp::Image<uchar> &in_main, 
    const wsp::Image<uchar> &in_sub, 
    wsp::Image<double> &o_depth, 
    const wsp::img::StereoPOCParams &params, 
    const wsp::Image<uchar> *in_mask_main, 
    const wsp::Image<uchar> *in_mask_sub
){
    wsp::State wstat;
    wstat = StereoPOC_main(o_depth, in_main, in_sub, params, in_mask_main);
    if(wstat!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("StereoPOC_main failed\n"); return wstat; }

    // handling cross checking
    if(params.cross_check==true){
        wsp::Image<double> depth_sub;
        printf("cross check\n");
        wsp::img::StereoPOCParams tmp_params(params);
        // swap min and max --------
        int tmp = params.dispx_min;
        tmp_params.dispx_min = -params.dispx_max;
        tmp_params.dispx_max = -tmp;
        tmp = params.dispy_min;
        tmp_params.dispy_min = -params.dispy_max;
        tmp_params.dispy_max = -tmp;
        // ---------------------

        wstat = wsp::img::StereoPOC_main(depth_sub, in_sub, in_main, tmp_params, in_mask_sub);
        if(wstat!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("StereoPOC_main failed\n"); return wstat; }

        // save temprary image(only x-axis) -----
        if(params.save_debug_data){
            wsp::Image<double> tmp_main, tmp_sub;
            tmp_main = o_depth;
            tmp_sub = depth_sub;
            tmp_main.Normalize(0, 255, 200);
            tmp_main.SetSize(tmp_main.width(), tmp_main.height(), 1);
            tmp_main.SetSize(tmp_main.width(), tmp_main.height(), 3);
            tmp_main.SaveImage("tmp_depth_main.tif", WSP_PIXEL_STATE_OCCLUDED);

            tmp_sub.Normalize(0, 255, 200);
            tmp_sub.SetSize(tmp_sub.width(), tmp_sub.height(), 1);
            tmp_sub.SetSize(tmp_sub.width(), tmp_sub.height(), 3);
            tmp_sub.SaveImage("tmp_depth_sub.tif", WSP_PIXEL_STATE_OCCLUDED);
        }
        // ---------------

        wsp::img::CrossCheck(o_depth, depth_sub, tmp_params.cross_check_fuzziness);
    }
    return wstat;
}


//! Any Color Matching
wsp::State wsp::img::StereoPOC_main(wsp::Image<double> &o_depth, 
                                    const wsp::Image<uchar> &in_main, const wsp::Image<uchar> &in_sub, 
                                    const wsp::img::StereoPOCParams &params, const wsp::Image<uchar> *in_mask)
{
    if(in_main.width()!=in_sub.width() || in_main.height()!=in_sub.height() || in_main.num_of_channels()!=in_sub.num_of_channels()){
        WSP_COMMON_ERROR_LOG("left and right image size must be the same: %d==%d, %d==%d, %d==%d\n", 
            in_main.width(), in_sub.width(), in_main.height(), in_sub.height(), in_main.num_of_channels(), in_sub.num_of_channels()); 
        return WSP_STATE_FAILURE;
    }
    int width = static_cast<int>(in_main.width()), height = static_cast<int>(in_main.height()), nc = static_cast<int>(in_main.num_of_channels());

    //double r_main, g_main, b_main, r_sub, g_sub, b_sub;
    char filename[MAX_LENGTH_OF_NAME];

    int blk_hw=(int)params.block_width/2, blk_hh=(int)params.block_height/2;
    int blk_w = 2*blk_hw, blk_h = 2*blk_hh + 1;
    int blk_len = blk_w * blk_h;

    wsp::Image<double> mainBlk, subBlk;
    mainBlk.SetSize(blk_w, blk_h);
    subBlk.SetSize(blk_w, blk_h);

    o_depth.SetSize((u32)width, (u32)height, 2);
    //o_depth.SetSize((u32)width, (u32)height, 1);
    o_depth.Reset(0);

    printf("Stereo Matching Based on POC ==================\n");
    printf("    width=%d, height=%d, numOfChannels=%d, \n", width, height, nc);
    printf("    block width=%d, block height=%d\n", params.block_width, params.block_height);
    printf("    search depth=%d, low pass ratio=%f\n", params.search_depth, params.low_pass_ratio);
    printf("    poc peak=%f, thre std dev=%f\n", params.thre_poc_peak, params.thre_std_deviation);

    //#ifdef USE_QT
    //QProgressDialog progress("POC Stereo Matching...", "Cancel", 0, height);
    //progress.setWindowModality(Qt::WindowModal);
    //#endif

    uchar thre_mask = 200;
    //#ifdef _OPENMP
    //#pragma omp parallel
    //#endif
    {
        double *d[2]; // temporary disparity for x and y
        double disp[2], value, sum_sub, sum_main;
        double mean_sd_main, mean_sd_sub;
        int m, interval, level;
        int x, y, i, j, k, c; // iterators
        int px, py, index_main, index_sub, index_blk;
        d[0] = new double[params.search_depth+1];
        d[1] = new double[params.search_depth+1];
        //#ifdef _OPENMP
        //#pragma omp for
        //#endif
        for(y=0; y<height; y++){
            for(x=0; x<width; x++){
                if(in_mask!=NULL){
                    if((*in_mask)[y*width+x]<thre_mask){
                        o_depth.SetAlpha(0, (u32)x, (u32)y);
                        o_depth.SetState(WSP_PIXEL_STATE_OCCLUDED, (u32)x, (u32)y);
                        continue;
                    }
                }
                interval=pow(2.0, (int)params.search_depth-1);

                // skip arround corner
                if(x-blk_hw*interval<0 || x+blk_hw*interval>=width
                    || y-blk_hh*interval<0 || y+blk_hh*interval>=height){
                    o_depth.SetAlpha(0, (u32)x, (u32)y);
                    o_depth.SetState(WSP_PIXEL_STATE_OCCLUDED, (u32)x, (u32)y);
                    continue;
                }

                // input 0 because first position between two images is the same choordinate
                d[0][0]=0.0;
                d[1][0]=0.0;

                // hieralchal loop
                for(level=0; level<(int)params.search_depth; interval/=2){
                    // block loop for copying block pixels
                    // evaluating RGB as sum of RGB
                    for(i=-blk_hh; i<=blk_hh; i+=1){
                        for(j=-blk_hw; j<=blk_hw-1; j+=1){
                            px = x + j*interval; // global x for block
                            py = y + i;          // global y for block
                            index_main= (py*width+px)*nc;                      // local index for main block image
                            index_sub = ((py-(int)floor(d[1][level]))*width+px-(int)floor(d[0][level]))*nc; // local index for sub block image

                            px = j+blk_hw; // Clamp minimum x to 0
                            py = i+blk_hh; // Clamp minimum y to 0
                            index_blk = py*blk_w + px; // calculate index within block

                            // getting sum of rgb for double poc source
                            sum_sub = 0.0;
                            sum_main = 0.0;
                            for(c=0; c<nc; c++){
                                sum_sub += static_cast<double>(in_sub[index_sub+c]);
                                sum_main+= static_cast<double>(in_main[index_main+c]);
                            }

                            // loop for getting data for pixels between the interval
                            for(k=-interval/2+1; k<=interval/2; k++){
                                for(c=0; c<nc; c++){
                                    sum_sub += static_cast<double>(in_sub[index_sub+k*nc+c]);
                                    sum_main+= static_cast<double>(in_main[index_main+k*nc+c]);
                                }
                            }

                            subBlk[index_blk]=sum_sub;
                            mainBlk[index_blk]=sum_main;
                            //printf("%f\t%f\n", sum_main, sum_sub);
                        }
                    }
                    level++;

                    //sprintf(filename, "mainblk_%d_%d.tif", x, y);
                    //mainBlk.SaveImage(filename);
                    //sprintf(filename, "subblk_%d_%d.tif", x, y);
                    //subBlk.SaveImage(filename);
                

                    // skip if blocks have no charactaristics
                    mean_sd_main=wsp::GetMeanStdDev(mainBlk.data(), (u32)blk_len);
                    mean_sd_sub=wsp::GetMeanStdDev(subBlk.data(), (u32)blk_len);
                    if(mean_sd_main<params.thre_std_deviation || mean_sd_sub<params.thre_std_deviation){
                        d[0][level] = DBL_MAX; break;
                    }

                    // compute disparity
                    //#ifdef _OPENMP
                    //#pragma omp critical
                    //#endif
                    {
                        if(params.use_2d_poc){
                            wsp::img::FindDisparity2DPOC(d[0][level], d[1][level], NULL, subBlk, mainBlk,
                                //params.dispx_min, params.dispx_max, 
                                params.thre_poc_peak, params.low_pass_ratio);
                        }else{
                            d[0][level] = wsp::img::FindDisparity1DPOC_fftw(subBlk.data(), mainBlk.data(), mainBlk.width(), mainBlk.height(), params.dispx_min, params.dispx_max, params.thre_poc_peak, params.low_pass_ratio);
                        }
                    }
                    //printf("%f\n", d[level]);

                    // if poc reliability is low than stop calculation
                    if(d[0][level]==DBL_MAX){ break; }

                    d[0][level] = level<params.search_depth? floor(d[0][level]) : d[0][level];
                    d[1][level] = level<params.search_depth? floor(d[1][level]) : d[1][level];
                }
                if(d[0][level]==DBL_MAX){
                    //o_depth.SetElem(0, (u32)x, (u32)y);
                    o_depth.SetAlpha(0, (u32)x, (u32)y);
                    o_depth.SetState(WSP_PIXEL_STATE_OCCLUDED, (u32)x, (u32)y);
                    continue;
                }
                disp[0]=0.0;
                disp[1]=0.0;
                for(i=1; i<=params.search_depth; i++){
                    disp[0] += d[0][i];
                    disp[1] += d[1][i];
                }
                o_depth.SetElem(disp[0], (u32)x, (u32)y, 0);
                o_depth.SetElem(disp[1], (u32)x, (u32)y, 1);
            }
            //#ifdef USE_QT
            //progress.setValue(y);
            //if(progress.wasCanceled()){ return WSP_STATE_CANCELED; }
            //#else
            printf("%d %% done..\r", y*100/height);
            //#endif
        }
        delete[] d[0];
        delete[] d[1];
    }
    //#ifdef USE_QT
    //progress.setValue(height);
    //#endif
    return WSP_STATE_SUCCESS;
}



double wsp::img::FindDisparity1DPOC_fftw(const double *in_reference, 
                                    const double *in_template, 
                                    int width, int height,
                                    double minDisp, double maxDisp, 
                                    double thre_pocPeak, 
                                    double low_pass_ratio)
{
    double delta_x;
    double *refData, *tplData, *pocData, *pocAvData;
    int i, j;
    int div;
    int w, length;
    wsp::State state;

    w = (int)width;
    //w = 1;
    // calculate power of two for fft width
    //while(w<(int)width){
    //    w = w<<1;
    //}

    length = w * (int)height;
    pocAvData = new double[w];
    pocData = new double[length];
    refData = new double[length];
    tplData = new double[length];

    wsp::CopyArray1dAsImage<double>(refData, (u32)w, height, in_reference, width, height);
    wsp::CopyArray1dAsImage<double>(tplData, (u32)w, height, in_template, width, height);



    //state=wsp::math::GetPhaseCorrelationFFT1DPerLine(pocData, pocAvData, refData, tplData, w, height, low_pass_ratio);
    state=wsp::fftw::GetPhaseCorrelationDFT1DPerLine(pocData, pocAvData, refData, tplData, w, height, low_pass_ratio);
    //printf("%s\r", state==WSP_STATE_SUCCESS? "success" : "failure");
    if(state!=WSP_STATE_SUCCESS){
        WSP_COMMON_ERROR_LOG("Getting POC failed\n");
        delete[] refData; delete[] tplData; delete[] pocData;
        return DBL_MAX;
    }

    if(wsp::GetMax(pocAvData, w)<thre_pocPeak){
        delete[] refData; delete[] tplData; delete[] pocData;
        return DBL_MAX; 
    }
    //printf("max: %f, min: %f\n", wsp::GetMax(pocAvData, w), wsp::GetMin(pocAvData, w));

    // get average
    //for(i=0; i<width; i++){
    //    pocAvData[i] = 0.0;
    //    for(j=0; j<height; j++){
    //        pocAvData[i] += pocData[j*width+i];
    //    }
    //    pocAvData[i] /= (double)width;
    //}

    // find the maximum value and its location
    //delta_x = wsp::img::GetPeakSubpixel(pocData, length, 1, low_pass_ratio);
    delta_x = wsp::img::GetPeakSubpixel(pocAvData, width, 1, low_pass_ratio);

    div = floor(delta_x/(double)w);
    delta_x = delta_x-(double)div*w;
    if(delta_x>(double)(w/2)){ delta_x-=w;}

    //printf("SUBPIXEL UNIT: Peak is at %f\n", delta_x);

    // freeing memories
    delete[] refData;
    delete[] tplData;
    delete[] pocData;

    //double threRatio_delta_x=0.1;
    //if(fabs(delta_x) > ((double)w/2)*threRatio_delta_x){ return DBL_MAX;}
    if(delta_x<minDisp || maxDisp<delta_x){ return DBL_MAX; }

    return delta_x;
}



wsp::State wsp::img::FindDisparity2DPOC(double &o_deltax, double &o_deltay, 
                                        wsp::Image<double> *o_poc,
                                        const wsp::Image<double> &in_ref, 
                                        const wsp::Image<double> &in_tpl, 
                                        //double minDisp, double maxDisp, 
                                        double thre_pocPeak, 
                                        double low_pass_ratio)
{
    double delta_x, deltaY;
    wsp::Image<double> tmp_poc;
    u32 width=in_ref.width(), height=in_ref.height();
    if(in_ref.IsTheSameSize(in_tpl)==false){
        WSP_COMMON_ERROR_LOG("The size of input images are different\n");
        return WSP_STATE_FAILURE;
    }
    if(in_ref.num_of_channels()!=1){ WSP_COMMON_ERROR_LOG("The num of channels must be 1\n"); return WSP_STATE_FAILURE; }

    int div;
    int length = (int)width*height;
    wsp::State state;

    //state=wsp::img::GetPhaseCorrelationFFT2D(pocData, refData, tplData, width, height, low_pass_ratio);
    state=wsp::img::GetPhaseCorrelationFFT2D(tmp_poc, in_ref, in_tpl, low_pass_ratio);



    if(state!=WSP_STATE_SUCCESS){
        WSP_COMMON_ERROR_LOG("Getting POC failed\n");
        //delete[] refData; delete[] tplData; delete[] pocData;
        return WSP_STATE_FAILURE;
    }
    //tmp_poc.Normalize(0, 255);
    //tmp_poc.SaveImage("poc_peak.png");
    tmp_poc.SetSize(width, height, 1);

    //if(wsp::GetMax(pocData, width*height)<thre_pocPeak){
    if(wsp::GetMax(tmp_poc.data(), width*height)<thre_pocPeak){
        o_deltax=DBL_MAX; o_deltay=DBL_MAX;
        return WSP_STATE_SUCCESS;
    }

    // find the maximum value and its location
    delta_x = wsp::img::GetPeakSubpixel(tmp_poc.data(), length, 1, low_pass_ratio);
    div = floor(delta_x/(double)width);
    delta_x = delta_x-(double)div*width;
    if(delta_x>(double)(width/2)){ delta_x-=width;}

    //wsp::math::Transpose<double>(tmp_poc.data(), width, height);
    tmp_poc.Transpose();
    deltaY = wsp::img::GetPeakSubpixel(tmp_poc.data(), length, 1, low_pass_ratio);
    div = floor(deltaY/(double)height);
    deltaY = deltaY-(double)div*height;
    if(deltaY>(double)(height/2)){ deltaY-=height;}
    //wsp::math::Transpose<double>(tmp_poc.data(), width, height);

    //printf("SUBPIXEL UNIT: Peak is at %f %f\n", delta_x, deltaY);

    // store data
    if(o_poc!=NULL){
        tmp_poc.Transpose();
        *o_poc=tmp_poc;
    }
    o_deltax = delta_x;
    o_deltay = deltaY;

    return WSP_STATE_SUCCESS;
}



wsp::State wsp::img::GetPhaseCorrelationFFT2D(
    wsp::Image<double> &o_poc, const wsp::Image<double> &in_ref, const wsp::Image<double> &in_tpl, 
    double low_pass_ratio
)
{
    int w=in_ref.width(), h=in_ref.height(), nc=in_ref.num_of_channels();
    if(w!=in_tpl.width() || h!=in_tpl.height() || nc!=in_tpl.num_of_channels()){
        WSP_COMMON_ERROR_LOG("sizes of input images are different\n"); return WSP_STATE_FAILURE;
    }
    wsp::Image<double> tmp_ref(in_ref), tmp_tpl(in_tpl);
    wsp::State state;
    int w_fft=wsp::math::GetNearestPowerOfTwo(in_ref.width());
    int h_fft=wsp::math::GetNearestPowerOfTwo(in_ref.height());
    tmp_ref.SetSize(w_fft, h_fft);
    tmp_tpl.SetSize(w_fft, h_fft);
    o_poc.SetSize(w_fft, h_fft);
    //printf("Original size: %d %d\n", w, h);
    //printf("FFT size: %d %d\n", w_fft, h_fft);
    state=wsp::fftw::GetPhaseCorrelationDFT2D(
        o_poc.data(), tmp_ref.data(), tmp_tpl.data(), 
        w_fft, h_fft, low_pass_ratio
    );
    if(state!=WSP_STATE_SUCCESS){
        WSP_COMMON_ERROR_LOG("GetPhaseCorrelationDFT2D failed\n"); return state;
    }

    o_poc.SetSize(w, h);
    
    return WSP_STATE_SUCCESS; 
}

