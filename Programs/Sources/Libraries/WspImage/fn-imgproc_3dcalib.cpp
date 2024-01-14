/**
 * @file fn-imgproc_3dcalib.cpp
 * 
 * @brief Definition of image proccessing functions for Stereogram, 3D calibration.
 */

//#include <omp.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>    

#include <wsp/common/_tpl_fn-array.hpp>
#include <wsp/common/_cl-std_param.h>
#include <wsp/math/_fn-math.h>
#include <wsp/math/_fn-math_core.h>
#include <wsp/math/_tpl_fn-math.h>
#ifdef USE_FFTW
#include <wsp/fftw/fn-math-gpl.h>
#endif

#include <wsp/math/fn-basic_math.h>
#include <wsp/math/math_result_converter.h>
#include "_tpl_fn-imgproc.hpp"
#include "_fn-imgproc.h"
#include "tpl_fn-imgproc.hpp"
#include "tpl_fn-imgfilter.h"
//#include "gpl_tpl_fn-imgproc.h"
#include "fn-imgfilter.h"
//#include "gpl_fn-imgproc.h"


#include "fn-imgproc.h"

#define USE_OPTIMIZED_CODE


// =========================================================================================
// =========================================================================================
//**
//                       Block Correlation Stereo Matching
//**
// =========================================================================================
// =========================================================================================



/**
 * Find occlusion from correspondance between calculated left and right depth map. 
 */
wsp::State wsp::img::CrossCheck(wsp::Image<double> &io_depthMain, wsp::Image<double> &io_depthSub, double fuzziness, bool show_debug_info)
{
    int dispSize;
    int width, height, nc;
    wsp::Image<double> win_size_map, corr_map;

    width = (int)io_depthMain.width();
    height = (int)io_depthMain.height();
    nc = (int)io_depthMain.num_of_channels();

    if(show_debug_info){
        printf("Applying CrossCheck..\n");
    }

    if(width!=(int)io_depthSub.width() || height!=(int)io_depthSub.height() || nc!=io_depthSub.num_of_channels()){
        WSP_COMMON_ERROR_LOG("The size of main and sub image are different: main=(%d, %d, %d), sub=(%d, %d, %d)\n",
            width, height, nc, io_depthSub.width(), io_depthSub.height(), io_depthSub.num_of_channels()); 
        return WSP_STATE_FAILURE;
    }
    if(nc!=2){
        WSP_COMMON_ERROR_LOG("The number of channels must be 2(diparity of x, disparity of y)\n"); return WSP_STATE_FAILURE;
    }

    {
        int x, y; // iterators
        int dx, dy, dx_main, dx_sub, dy_main, dy_sub; // disparity
        int i_main, i_sub; // index
        // optimized code ----------------------------------------
        double *tmp_ptr;
        double *data_ptr = io_depthMain.data();
        double *end_ptr = data_ptr + io_depthMain.length()*nc;
        double *sub_data_ptr = io_depthSub.data();
        uchar *state_ptr = io_depthMain.state_data();

        #define USE_OPT_CROSSCHECK_CODE (1)
        #if USE_OPT_CROSSCHECK_CODE==1
        for(x=0, y=0; data_ptr!=end_ptr; data_ptr+=nc, sub_data_ptr+=nc, ++state_ptr, ++x)
        {
            if(x==width){ x=0; ++y; }
            if(*state_ptr==WSP_PIXEL_STATE_OCCLUDED){ continue; }
            dx_main = (int)*data_ptr;
            dy_main = (int)*(data_ptr+1);
            dx = x + dx_main;
            dy = y + dy_main;
            if(0<=dx && dx<width && 0<=dy && dy<height)
            {
                tmp_ptr = sub_data_ptr + (dx_main + dy_main*width) * nc;
                dx_sub = (int)*tmp_ptr;
                dy_sub = (int)*(tmp_ptr + 1);
                //printf("d_main: %d, d_sub: %d, %d\n", d_main, d_sub, d_main==d_sub);
                if(dx_main>=-dx_sub-fuzziness && dx_main<=-dx_sub+fuzziness 
                && dy_main>=-dy_sub-fuzziness && dy_main<=-dy_sub+fuzziness){
                    *state_ptr = WSP_PIXEL_STATE_NORMAL;
                    continue;
                }
            }
            *state_ptr = WSP_PIXEL_STATE_OCCLUDED;
        }
        #else
        // unoptimized code -------------------------------------------
        for(y=0; y<height; y++){
            for(x=0; x<width; x++){
                i_main = y*width+x;
                if(io_depthMain.GetState(i_main)==WSP_PIXEL_STATE_OCCLUDED){
                    continue;
                }
                dx_main = (int)io_depthMain[i_main*nc];
                dy_main = (int)io_depthMain[i_main*nc+1];
                dx = x + dx_main;
                dy = y + dy_main;
                if(0<=dx && dx<width && 0<=dy && dy<height)
                { 
                    i_sub = (dy*width+dx)*nc;
                    dx_sub = (int)io_depthSub[i_sub];
                    dy_sub = (int)io_depthSub[i_sub+1];
                    //printf("d_main: %d, d_sub: %d, %d\n", d_main, d_sub, d_main==d_sub);
                    if(dx_main>=-dx_sub-fuzziness && dx_main<=-dx_sub+fuzziness 
                        && dy_main>=-dy_sub-fuzziness && dy_main<=-dy_sub+fuzziness){
                        io_depthMain.SetState(WSP_PIXEL_STATE_NORMAL, i_main);
                        continue;
                    }
                }
                io_depthMain.SetState(WSP_PIXEL_STATE_OCCLUDED, i_main);
            }
        }
        #endif
    }
    

    return WSP_STATE_SUCCESS;
}

//***********************************************************
//* FUNCTION DECLARATION (wsp::img::GetAdaptiveWindowSize)
/**
 * @group WspImage
 */
template<typename T>
wsp::State GetAdaptiveWindowSize(
    wsp::Image<double> &o_winSize, 
    const wsp::Image<T> &in_img, 
    const wsp::Image<T> edgeImg,
    int win_min, int win_max, 
    double enough_edge_amount,
    bool saveDebugData
){
    int itr_x, itr_y, itr_w; // iterators
    int width, height;
    int w2_size, adaptive_size;
    double edge_sum, edge_av, edge_av_max;
    wsp::Image<double> edgeSum;
    wsp::State state;

    width = (int)in_img.width();
    height = (int)in_img.height();
    o_winSize.SetSize((u32)width, (u32)height);

    // Get edge image ---------------
    state=wsp::img::GetFullSum(edgeSum, edgeImg);
    if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Getting full sum failed\n"); return state; }
    // --------------- end Get edge image
    printf("enough edge amount = %f\n", enough_edge_amount);

    // main loop -------------------------------
    for(itr_y=0; itr_y<height; itr_y++){
        for(itr_x=0; itr_x<width; itr_x++){
            adaptive_size = win_min;
            edge_av_max = 0;
            for(itr_w=win_min; itr_w<=win_max; itr_w++)
            {
                w2_size = itr_w * 2 + 1;
                if( itr_x-itr_w-1<0 || itr_y-itr_w-1<0 || itr_x+itr_w>=width || itr_y+itr_w>=height){ break; }

                edge_sum = (edgeSum.GetElem(itr_x+itr_w, itr_y+itr_w)-edgeSum.GetElem(itr_x-itr_w-1, itr_y+itr_w))
                         - (edgeSum.GetElem(itr_x+itr_w, itr_y-itr_w-1)-edgeSum.GetElem(itr_x-itr_w-1, itr_y-itr_w-1));
                edge_av = edge_sum / static_cast<double>(w2_size * w2_size);
                //printf("edge_sum=%d,w2_size=%d,block_average=%d\n", edge_sum,w2_size,edge_av);

                // if current edge amount is enough to calculate correlation, than break.
                if(edge_sum >= enough_edge_amount){
                    adaptive_size = itr_w;
                    break;
                }
                // compare current value and max value
                if(edge_av >= edge_av_max){
                    edge_av_max = edge_av;
                    adaptive_size = itr_w;
                }
            }
            //cout<<adaptive_size<<endl;
            o_winSize.SetElem((double)adaptive_size, itr_x, itr_y);
        }
    }
    return WSP_STATE_SUCCESS;
}


template<typename T>
void GetSumOfZeroMeanNormalizedCrossCorr(
    const T *in_main, 
    const T *in_sub, 
    double *o_corr_sum,
    int width, int height, int num_channels,
    int disp_x, int disp_y,int blk_size
){
    int dx_step = disp_x*num_channels;
    int width_step = width*num_channels;
    int length = width * height;
    int num_elem = length * num_channels;

    double *main_norm_sum_map = (double*)malloc(num_elem*sizeof(double));
    double *sub_norm_sum_map = (double*)malloc(num_elem*sizeof(double));
    double *main_mean_sum_map = (double*)malloc(num_elem*sizeof(double));
    double *sub_mean_sum_map = (double*)malloc(num_elem*sizeof(double));
    double *main_mean_map = (double*)malloc(num_elem*sizeof(double));
    double *sub_mean_map = (double*)malloc(num_elem*sizeof(double));

    double *sum_main = (double*)malloc(num_channels*sizeof(double));
    double *sum_sub = (double*)malloc(num_channels*sizeof(double));
    double *mean_main = (double*)malloc(num_channels*sizeof(double));
    double *mean_sub = (double*)malloc(num_channels*sizeof(double));
    double *norm_main = (double*)malloc(num_channels*sizeof(double));
    double *norm_sub = (double*)malloc(num_channels*sizeof(double));
    int k = 1;
    int num_blk_pix = blk_size*blk_size;


    {
        // calculate summation of main and sub for average
        // unoptimized code ---------------------------------------------
        int x, y, i, c, ic;
        // get horizontal summation
        for(y=0; y<height; ++y){
            for(c=0; c<num_channels; ++c){
                sum_main[c] = 0.0;
                sum_sub[c] = 0.0;
            }
            for(x=0; x<width; ++x){
                i = (y*width+x)*num_channels;
                for(c=0; c<num_channels; ++c){
                    ic = i+c;
                    sum_main[c]+= in_main[ic];
                    sum_sub[c] += in_sub[ic];
                    main_mean_sum_map[ic]= sum_main[c]; 
                    sub_mean_sum_map[ic] = sum_sub[c];
                }
            }
        }
        // get vertical summation
        for(x=0; x<width; ++x){
            for(c=0; c<num_channels; ++c){
                sum_main[c] = main_norm_sum_map[x];
                sum_sub[c] = sub_norm_sum_map[x];
            }
            for(y=0; y<height; ++y){
                i = (y*width+x)*num_channels;
                for(c=0; c<num_channels; ++c){
                    ic = i+c;
                    sum_main[c]+= main_norm_sum_map[ic];
                    sum_sub[c] += sub_norm_sum_map[ic];
                    main_mean_sum_map[ic]= sum_main[c]; 
                    sub_mean_sum_map[ic] = sum_sub[c];
                }
            }
        }
    }

    // Calculate summation of norm of main and sub images
    {
        int wx1, wy1, wx2, wy2;
        int i_tl, i_tr, i_bl, i_br;
        // unoptimized code ---------------------------------------------
        int x, y, i, c, ic;
        // get horizontal summation
        for(y=0; y<height; ++y){
            for(c=0; c<num_channels; ++c){
                sum_main[c] = 0.0;
                sum_sub[c] = 0.0;
            }
            for(x=0; x<width; ++x){
                i = (y*width+x)*num_channels;
                wx1 = x+blk_size;
                wy1 = y+blk_size;
                wx2 = x-blk_size-1;
                wy2 = y-blk_size-1;
                if(wx1>=width || wy1>=height || wx2<0 || wy2<0){ 
                    for(c=0; c<num_channels; ++c){
                        ic = i+c;
                        main_norm_sum_map[ic] = 0; 
                        sub_norm_sum_map[ic] = 0;
                    }
                    continue; 
                }

                i_br = (wy1*width+wx1)*num_channels;
                i_bl = (wy1*width+wx2)*num_channels;
                i_tr = (wy2*width+wx1)*num_channels;
                i_tl = (wy2*width+wx2)*num_channels;
                for(c=0; c<num_channels; ++c){
                    ic = i+c;
                    mean_main[c]= (main_mean_sum_map[i_br+c]-main_mean_sum_map[i_bl+c])
                                 -(main_mean_sum_map[i_tr+c]-main_mean_sum_map[i_bl+c]);
                    mean_sub[c] = (sub_mean_sum_map[i_br+c] -sub_mean_sum_map[i_bl+c])
                                 -(sub_mean_sum_map[i_tr+c] -sub_mean_sum_map[i_tl+c]);
                    mean_main[c]/= (num_blk_pix - k);
                    mean_sub[c] /= (num_blk_pix - k);
                    main_mean_map[ic] = mean_main[c]; // keep calculated average on memory
                    sub_mean_map[ic] = mean_sub[c]; // keep calculated average on memory
                    sum_main[c]+= (in_main[ic]-mean_main[c])*(in_main[ic]-mean_main[c]);
                    sum_sub[c] += (in_sub[ic] -mean_sub[c]) *(in_sub[ic] -mean_sub[c]);
                    main_norm_sum_map[ic] = sum_main[c]; 
                    sub_norm_sum_map[ic] = sum_sub[c];
                }
            }
        }

        // get vertical summation
        for(x=0; x<width; ++x){
            i = x*num_channels;
            for(c=0; c<num_channels; ++c){
                sum_main[c] = main_norm_sum_map[i+c];
                sum_sub[c] = sub_norm_sum_map[i+c];
            }
            for(y=0; y<height; ++y){
                i = (y*width+x)*num_channels;
                for(c=0; c<num_channels; ++c){
                    ic = i+c;
                    sum_main[c] += main_norm_sum_map[ic];
                    sum_sub[c] += sub_norm_sum_map[ic];
                    main_norm_sum_map[ic] = sum_main[c]; 
                    sub_norm_sum_map[ic] = sum_sub[c];
                }
            }
        }
    }

    // calculate horizontal summation of correlation for ZNCC
    {
        int x, y, c, i, i_d, ic, ic_d;
        int wx1, wy1, wx2, wy2;
        int dx, dy;
        int i_tl, i_tr, i_bl, i_br;
        for(y=0; y<height; ++y){
            for(c=0; c<num_channels; ++c){
                sum_main[c] = 0.0;
            }
            for(x=0; x<width; ++x){
                dx = (int)x + disp_x;
                dy = (int)y + disp_y;
                i = (y*width + x) * num_channels;
                i_d = (dy*width + dx) * num_channels;
                wx1 = x+blk_size;
                wy1 = y+blk_size;
                wx2 = x-blk_size-1;
                wy2 = y-blk_size-1;

                if(wx1>=width || wy1>=height || wx2<0 || wy2<0
                    || dx<0 || dx>=(int)width || dy<0 || dy>=(int)height)
                {
                    for(c=0; c<num_channels; ++c){
                        sum_main[c] += wsp::math::kDefaultCorr;
                    }
                }else{
                    i_br = (wy1*width+wx1)*num_channels;
                    i_bl = (wy1*width+wx2)*num_channels;
                    i_tr = (wy2*width+wx1)*num_channels;
                    i_tl = (wy2*width+wx2)*num_channels;
                    for(c=0; c<num_channels; ++c){
                        ic = i+c;
                        ic_d = i_d+c;
                        norm_main[c] = (main_norm_sum_map[i_br+c]-main_norm_sum_map[i_bl+c])
                                    -(main_norm_sum_map[i_tr+c]-main_norm_sum_map[i_tl+c]);
                        norm_sub[c] = (sub_norm_sum_map[i_br+c] -sub_norm_sum_map[i_bl+c])
                                    -(sub_norm_sum_map[i_tr+c] -sub_norm_sum_map[i_tl+c]);

                        // multiply -1 because this must be decorrelation
                        sum_main[c] += - (in_main[ic]-main_mean_map[ic])*(in_sub[ic_d]-sub_mean_map[ic_d]) 
                                        /(sqrt(norm_main[c])*sqrt(norm_sub[c]));
                    }
                }

                // insert sum of all channels on output
                o_corr_sum[y*width+x] = 0.0;
                for(c=0; c<num_channels; ++c){
                    o_corr_sum[y*width+x] += sum_main[c];
                }
            }
        }
    }

    // Get full sum ---------------
    {
        double sum;
        int x, y;
        // optimized code --------------------
        double *corr_v_ptr, *end_v_ptr;
        double *corr_ptr = o_corr_sum;
        double *end_ptr = corr_ptr + width;
        for(; corr_ptr!=end_ptr; ++corr_ptr){
            sum=0.0;
            corr_v_ptr = corr_ptr;
            end_v_ptr = corr_ptr + length;
            for(; corr_v_ptr!=end_v_ptr; corr_v_ptr+=width){
                sum += *corr_v_ptr;
                *corr_v_ptr = sum;
            }
        }
        // unoptimized code ------------------
        //for(x=0; x<width; ++x){
        //    sum=0.0;
        //    for(y=0; y<height; ++y){
        //        int index = y*width+x;
        //        sum += tmp_corr_sum[index];
        //        tmp_corr_sum[index] = sum;
        //    }
        //}
    }

    free(sum_main);
    free(sum_sub);
    free(main_norm_sum_map);
    free(sub_norm_sum_map);
    free(main_mean_sum_map);
    free(sub_mean_sum_map);
    free(mean_main);
    free(mean_sub);
    free(norm_main);
    free(norm_sub);
    free(main_mean_map);
    free(sub_mean_map);
}


template<typename T>
void GetSumOfNormalizedCrossCorr(
    const T *in_main, 
    const T *in_sub, 
    double *o_corr_sum,
    int width, int height, int num_channels,
    int disp_x, int disp_y,int blk_size
){
    int dx_step = disp_x*num_channels;
    int width_step = width*num_channels;
    int length = width * height;
    int num_elem = length * num_channels;

    double *main_norm_map = (double*)malloc(num_elem*sizeof(double));
    double *sub_norm_map = (double*)malloc(num_elem*sizeof(double));

    double *sum_main = (double*)malloc(num_channels*sizeof(double));
    double *sum_sub = (double*)malloc(num_channels*sizeof(double));
    double *norm_main = (double*)malloc(num_channels*sizeof(double));
    double *norm_sub = (double*)malloc(num_channels*sizeof(double));

    T subst = 0;

    // Calculate summation of norm of main and sub images
    {
        // unoptimized code ---------------------------------------------
        int x, y, i, c, ic;
        // get horizontal summation
        for(y=0; y<height; ++y){
            for(c=0; c<num_channels; ++c){
                sum_main[c] = 0.0;
                sum_sub[c] = 0.0;
            }
            for(x=0; x<width; ++x){
                i = (y*width+x)*num_channels;
                for(c=0; c<num_channels; ++c){
                    ic = i+c;
                    sum_main[c] += (in_main[ic]-subst)*(in_main[ic]-subst);
                    sum_sub[c] += (in_sub[ic]-subst)*(in_sub[ic]-subst);
                    main_norm_map[ic] = sum_main[c]; 
                    sub_norm_map[ic] = sum_sub[c];
                }
            }
        }
        // get vertical summation
        for(x=0; x<width; ++x){
            i = x*num_channels;
            for(c=0; c<num_channels; ++c){
                sum_main[c] = main_norm_map[i+c];
                sum_sub[c] = sub_norm_map[i+c];
            }
            for(y=0; y<height; ++y){
                i = (y*width+x)*num_channels;
                for(c=0; c<num_channels; ++c){
                    ic = i+c;
                    sum_main[c] += main_norm_map[ic];
                    sum_sub[c] += sub_norm_map[ic];
                    main_norm_map[ic] = sum_main[c]; 
                    sub_norm_map[ic] = sum_sub[c];
                }
            }
        }
    }

    // calculate horizontal summation of correlation
    {
        int x, y, i, c;
        int wx1, wy1, wx2, wy2;
        int index, index_d;
        int dx, dy;
        int i_tl, i_tr, i_bl, i_br;
        for(y=0; y<height; ++y){
            for(c=0; c<num_channels; ++c){
                sum_main[c] = 0.0;
            }
            for(x=0; x<width; ++x){
                dx = (int)x + disp_x;
                dy = (int)y + disp_y;
                wx1 = x+blk_size;
                wy1 = y+blk_size;
                wx2 = x-blk_size-1;
                wy2 = y-blk_size-1;
                if(wx1>=width || wy1>=height || wx2<0 || wy2<0
                    || dx<0 || dx>=(int)width || dy<0 || dy>=(int)height)
                {
                    for(c=0; c<num_channels; ++c){
                        sum_main[c] += wsp::math::kDefaultCorr;
                    }
                }else{
                    i_br = (wy1*width+wx1)*num_channels;
                    i_bl = (wy1*width+wx2)*num_channels;
                    i_tr = (wy2*width+wx1)*num_channels;
                    i_tl = (wy2*width+wx2)*num_channels;

                    for(c=0; c<num_channels; ++c){
                        index = (y*width + x) * num_channels;
                        index_d = (dy*width + dx) * num_channels;
                        norm_main[c]= (main_norm_map[i_br+c]-main_norm_map[i_bl+c])
                                    -(main_norm_map[i_tr+c]-main_norm_map[i_tl+c]);
                        norm_sub[c] = (sub_norm_map[i_br+c] -sub_norm_map[i_bl+c])
                                    -(sub_norm_map[i_tr+c] -sub_norm_map[i_tl+c]);

                        // multiply -1 because of this must be decorrelation
                        sum_main[c] += - in_main[index+c]*in_sub[index_d+c] / (sqrt(norm_main[c])*sqrt(norm_sub[c]));
                    }
                }

                // insert sum of all channels on output
                o_corr_sum[y*width+x] = 0.0;
                for(c=0; c<num_channels; ++c){
                    o_corr_sum[y*width+x] += sum_main[c];
                }
            }
        }
    }
    // Get full sum ---------------
    {
        double sum;
        int x, y;
        // optimized code --------------------
        double *corr_v_ptr, *end_v_ptr;
        double *corr_ptr = o_corr_sum;
        double *end_ptr = corr_ptr + width;
        for(; corr_ptr!=end_ptr; ++corr_ptr){
            sum=0.0;
            corr_v_ptr = corr_ptr;
            end_v_ptr = corr_ptr + length;
            for(; corr_v_ptr!=end_v_ptr; corr_v_ptr+=width){
                sum += *corr_v_ptr;
                *corr_v_ptr = sum;
            }
        }
        // unoptimized code ------------------
        //for(x=0; x<width; ++x){
        //    sum=0.0;
        //    for(y=0; y<height; ++y){
        //        int index = y*width+x;
        //        sum += tmp_corr_sum[index];
        //        tmp_corr_sum[index] = sum;
        //    }
        //}
    }

    free(sum_main);
    free(sum_sub);
    free(main_norm_map);
    free(sub_norm_map);
    free(norm_main);
    free(norm_sub);
}


template<typename T>
void UpdateCorrelationMap(
    const T     *in_main, 
    const T     *in_sub, 
    uchar const *in_mask, 
    double      *io_depth, 
    uchar       *io_state,
    double      *io_corr, 
    int width, int height, int num_channels,
    int disp_x, int disp_y, 
    int win_size,
    double thre_corr, 
    double default_corr, 
    wsp::math::CorrelationFunction corr_func,
    double *tmp_corr_sum,
    const double *in_win_size_map
){
    double *corr_sum_map = tmp_corr_sum;
    int dx_step = disp_x*num_channels;
    int width_step = width*num_channels;
    int length = width * height;
    int num_elem = length * num_channels;

    if(tmp_corr_sum==NULL){
        corr_sum_map = (double*)malloc(sizeof(double)*length);
    }

    switch(corr_func){
    case WSP_CORRELATION_FUNCTION_SAD:
        wsp::img::GetDirectionalSumOfCorr(in_main, in_sub,  corr_sum_map, width, height, num_channels, disp_x, disp_y, wsp::math::GetAbsDiff, default_corr);
        break;
    case WSP_CORRELATION_FUNCTION_SSD:
        wsp::img::GetDirectionalSumOfCorr(in_main, in_sub,  corr_sum_map, width, height, num_channels, disp_x, disp_y, wsp::math::GetSquaredDiff, default_corr);
        break;
    case WSP_CORRELATION_FUNCTION_CC:
        wsp::img::GetDirectionalSumOfCorr(in_main, in_sub,  corr_sum_map, width, height, num_channels, disp_x, disp_y, wsp::math::GetCrossDecorr, default_corr);
        break;
    case WSP_CORRELATION_FUNCTION_NCC:
        GetSumOfNormalizedCrossCorr(in_main, in_sub, corr_sum_map, width, height, num_channels, disp_x, disp_y, win_size);
        break;
    case WSP_CORRELATION_FUNCTION_ZNCC:
        GetSumOfZeroMeanNormalizedCrossCorr(in_main, in_sub, corr_sum_map, width, height, num_channels, disp_x, disp_y, win_size);
        break;
    default:
        WSP_COMMON_ERROR_LOG("unrecognized correlation function: %d\n", corr_func);
        return;
    }


    // Update correlation map --------------
    int w_size = win_size;
    int w2_size = win_size*2 + 1;
    int init_offset = (w_size+1)*width + w_size+1;
    int end_w_step = (width - w2_size);
    {
        double sum;
        double corr_av_norm;
        int x, y, i;
        int wx1, wy1, wx2, wy2;
        double *corr_sum_ptr = corr_sum_map + init_offset;
        double *end_ptr      = corr_sum_map + (length-w_size*width+w_size+1);
        double *corr_ptr     = io_corr      + init_offset;
        const uchar *mask_ptr= in_mask      + init_offset;
        //uchar *state_ptr     = io_state     + init_offset;
        //double *dst_ptr      = io_depth     + init_offset*2;
        double *end_w_ptr;

        y=w_size+1;
        #define USE_OPT_UPDATECORRELATIONMAP_CODE (1)
        #if USE_OPT_UPDATECORRELATIONMAP_CODE==1
        for(i=init_offset; corr_sum_ptr<end_ptr; corr_ptr+=w2_size, corr_sum_ptr+=w2_size, mask_ptr+=w2_size, i+=w2_size, ++y){
            end_w_ptr = corr_sum_ptr + end_w_step;
            x=w_size+1;
            for(; corr_sum_ptr!=end_w_ptr; ++corr_ptr, ++corr_sum_ptr, ++mask_ptr, ++i, ++x){
                if(*mask_ptr==WSP_PIXEL_STATE_MASKED){ continue; }

                if(in_win_size_map!=NULL){
                    w_size =in_win_size_map[i];
                    w2_size = w_size*2 + 1;
                    wx1 = x+w_size;
                    wy1 = y+w_size;
                    wx2 = x-w_size-1;
                    wy2 = y-w_size-1;
                    if(wx1>=width || wy1>=height || wx2<0 || wy2<0){ continue; }
                }

                double *ptr_l_bottom= corr_sum_ptr + w_size*width - (w_size+1);
                double *ptr_l_top   = corr_sum_ptr - (w_size+1)*width - (w_size+1);
                double *ptr_r_bottom= ptr_l_bottom + w2_size;
                double *ptr_r_top   = ptr_l_top    + w2_size;

                //sum = (corr_sum_map[wy1*width+wx1]-corr_sum_map[wy1*width+wx2])
                //    - (corr_sum_map[wy2*width+wx1]-corr_sum_map[wy2*width+wx2]);
                sum = (*ptr_r_bottom - *ptr_l_bottom) - (*ptr_r_top - *ptr_l_top);

                // update correlation map if current decorrelation is lower
                if(sum < *corr_ptr)
                {
                    // normalize average of SSD
                    corr_av_norm = (sum/(double)((w_size*2+1)*(w_size*2+1))) / 65025.0; // (255*255 = 65025)
                    if(corr_av_norm > thre_corr){ continue; }

                    *corr_ptr = sum;
                    io_depth[i*2] = disp_x;
                    io_depth[i*2+1] = disp_y;
                    io_state[i] = WSP_PIXEL_STATE_NORMAL;
                }
                //++x;
            }
            //++y;
        }
        #else
        // unoptimized code ----------------------------------------------
        double value;
        int x, y;
        int dx, dy, wx1, wy1, wx2, wy2, subst;
        double *corr_sum_ptr = corr_sum_map;
        double *corr_ptr = io_corr;
        const uchar *mask_ptr = in_mask;
        uchar *state_ptr = io_state;
        double *dst_ptr = io_depth;
        double *end_ptr = dst_ptr + length*2;
        for(y=0, i=0; dst_ptr!=end_ptr; ++y){
            for(x=0; x<width; ++corr_ptr, ++corr_sum_ptr, dst_ptr+=2, ++state_ptr, ++mask_ptr, ++x, ++i){
                if(*mask_ptr==WSP_PIXEL_STATE_MASKED){ continue; }

                dx = x + disp_x;
                dy = y + disp_y;
                if(dx<0 || dx>=width || dy<0 || dy>=height){ continue; }

                w_size=win_size;
                //w_size =in_win_size_map[i];
                wx1 = x+w_size;
                wy1 = y+w_size;
                wx2 = x-w_size-1;
                wy2 = y-w_size-1;
                if(wx1>=width || wy1>=height || wx2<0 || wy2<0){ continue; }

                sum = (corr_sum_map[wy1*width+wx1]-corr_sum_map[wy1*width+wx2])
                    - (corr_sum_map[wy2*width+wx1]-corr_sum_map[wy2*width+wx2]);

                // update correlation map if current decorrelation is lower
                if(sum < *corr_ptr)
                {
                    // normalize average of SSD
                    corr_av_norm = (sum/(double)((w_size*2+1)*(w_size*2+1))) / 65025.0; // (255*255 = 65025)
                    if(corr_av_norm > thre_corr){ continue; }

                    *corr_ptr = sum;
                    *dst_ptr = disp_x;
                    *(dst_ptr+1) = disp_y;
                    *state_ptr = WSP_PIXEL_STATE_NORMAL;
                }
            }
        }
        #endif
    }
    if(tmp_corr_sum==NULL){
        free(corr_sum_map);
    }
}


//
//
//wsp::State wsp::img::UpdateCorrelationMap(
//    wsp::Image<double> &o_depth, 
//    wsp::Image<double> &io_corr, 
//    const wsp::Image<uchar> &in_main, 
//    const wsp::Image<uchar> &in_sub, 
//    const wsp::Image<double> &in_win_size_map, 
//    uchar const *in_mask, 
//    int disp_x, int disp_y, 
//    double thre_corr, 
//    double (*eval_func)(double, double)
//){
//    double sum, corr_av_norm, value;
//    int x, y, c; // iterators
//    int iy, i, index, index_d;
//    int width, height, nc, w_size, w2_size;
//    int dx, dy, wx1, wy1, wx2, wy2, subst;
//    wsp::Image<double> corrSumMap;
//
//    width = in_main.width();
//    height = in_main.height();
//    nc = in_main.num_of_channels();
//    int num_channels = nc;
//
//    if(    width!=in_sub.width()  || height!=in_sub.height()
//        || width!=io_corr.width() || height!=io_corr.height()
//        || width!=o_depth.width() || height!=o_depth.height()
//        || width!=in_win_size_map.width() || height!=in_win_size_map.height() )
//    { WSP_COMMON_ERROR_LOG("The size of main and sub image are different\n"); return WSP_STATE_FAILURE; }
//
//    corrSumMap.SetSize(width, height);
//
//    int dx_step = disp_x * num_channels;
//    int dy_step = disp_y * num_channels;
//    int width_step = width * num_channels;
//    int length = width * height;
//    int num_elem = length * num_channels;
//
//    // Get horizontal sum ---------
//    {
//        //double sum;
//        // optimized code -------------------------------------------------
//        const uchar *end_c_ptr, *end_w_ptr, *end_wd_ptr;
//        const uchar *ptr_main = in_main.data();
//        const uchar *end_ptr = ptr_main + num_elem;
//        const uchar *ptr_sub = in_sub.data() + dy_step*width + dx_step;
//        double *dst_ptr = corrSumMap.data();
//        for(y=0; ptr_main!=end_ptr; ++y){
//            sum = 0.0;
//            end_w_ptr = ptr_main + width_step;
//            for(x=0; ptr_main!=end_w_ptr; ++dst_ptr, ++x){
//                dx = (int)x + disp_x;
//                dy = (int)y + disp_y;
//                if(dx<0 || dx>=(int)width || dy<0 || dy>=(int)height){
//                    sum += wsp::math::kDefaultCorr;
//                    ptr_main+=num_channels;
//                    ptr_sub+=num_channels;
//                }else{
//                    end_c_ptr = ptr_main + num_channels;
//                    for(; ptr_main!=end_c_ptr; ++ptr_main, ++ptr_sub){
//                        sum += eval_func(static_cast<double>(*ptr_main), static_cast<double>(*ptr_sub));
//                    }
//                }
//                *dst_ptr =  sum;
//            }
//        }
//        // optimized code 2(not completed yet) -------------------------------------------
//        //if(disp_x<0){
//        //    y=0;
//        //    for(; ptr_main!=end_ptr; ++y){
//        //        x=0;
//        //        sum = 0.0;
//        //        end_wd_ptr = ptr_main - dx_step;
//        //        end_w_ptr = ptr_main + width_step;
//        //        for(; ptr_main<end_wd_ptr; ptr_main+=num_channels, ptr_sub+=num_channels, ++dst_ptr, ++x){
//        //            sum += wsp::math::kDefaultCorr;
//        //            *dst_ptr = sum;
//        //        }
//        //        for(; ptr_main!=end_w_ptr; ++dst_ptr, ++x){
//        //            end_c_ptr = ptr_main + num_channels;
//        //            for(; ptr_main!=end_c_ptr; ++ptr_main, ++ptr_sub){
//        //                sum += eval_func(static_cast<double>(*ptr_main), static_cast<double>(*ptr_sub));
//        //            }
//        //            *dst_ptr = sum/static_cast<double>(num_channels);
//        //        }
//        //    }
//        //}else if(disp_x>0){
//        //    for(; ptr_main!=end_ptr;){
//        //        sum = 0.0;
//        //        end_w_ptr = ptr_main + width_step;
//        //        end_wd_ptr = end_w_ptr - dx_step;
//        //        for(; ptr_main!=end_wd_ptr;++dst_ptr){
//        //            end_c_ptr = ptr_main + num_channels;
//        //            for(; ptr_main!=end_c_ptr; ++ptr_main, ++ptr_sub){
//        //                sum += eval_func(static_cast<double>(*ptr_main), static_cast<double>(*ptr_sub));
//        //            }
//        //            *dst_ptr = sum/static_cast<double>(num_channels);
//        //        }
//        //        for(; ptr_main!=end_w_ptr; ptr_main+=num_channels, ptr_sub+=num_channels, ++dst_ptr){
//        //            sum += wsp::math::kDefaultCorr;
//        //            *dst_ptr = sum;
//        //        }
//        //    }
//        //}
//        // unoptimized code ----------------------------------------------------
//        //for(y=0; y<height; y++){
//        //    sum = 0;
//        //    for(x=0; x<width; x++){
//        //        dx = (int)x + disp_x;
//        //        dy = (int)y + disp_y;
//        //        if(dx<0 || dx>=(int)width || dy<0 || dy>=(int)height){
//        //            sum += wsp::math::kDefaultCorr;
//        //        }else{
//        //            index = (y*width + x) * nc;
//        //            index_d = (dy*width + dx) * nc;
//        //            for(c=0; c<nc; c++){
//        //                //subst = (int)(in_main[index + c] - in_sub[index_d + c]);
//        //                //sum += (double)subst*subst;
//        //                sum += eval_func(static_cast<double>(in_main[index+c]), static_cast<double>(in_sub[index_d+c]));
//        //            
//        //            }
//        //            //subst = (int)(in_main.GetElem(x, y).r-in_sub.GetElem((u32)dx, (u32)dy).r);
//        //            //sum += (double)subst*subst;
//        //            //subst = (in_main.GetElem(x, y).g-in_sub.GetElem((u32)dx, (u32)dy).g);
//        //            //sum += (double)subst*subst;
//        //            //subst = (in_main.GetElem(x, y).b-in_sub.GetElem((u32)dx, (u32)dy).b);
//        //            //sum += (double)subst*subst;
//        //        }
//        //        corrSumMap.SetElem(sum/3.0, x, y);
//        //    }
//        //}
//    }
//
//    // Get full sum ---------------
//    for(x=0; x<width; x++){
//        sum=0;
//        for(y=0; y<height; y++){
//            sum += corrSumMap.GetElem(x, y);
//            corrSumMap.SetElem(sum, x, y);
//        }
//    }
//
//    // Update correlation map --------------
//    for(y=0; y<height; y++){
//        iy = y*width;
//        for(x=0; x<width; x++){
//            i = iy + x;
//
//            if(in_mask[i]==WSP_PIXEL_STATE_MASKED){
//                continue;
//            }
//
//            dx = (int)x + disp_x;
//            dy = (int)y + disp_y;
//            if(dx<0 || dx>=(int)width || dy<0 || dy>=(int)height){ continue; }
//
//            //w_size = (u32)in_win_size_map.GetElem(x, y);
//            w_size = (u32)in_win_size_map[i];
//            wx1 = (int)x+(int)w_size;
//            wy1 = (int)y+(int)w_size;
//            wx2 = (int)x-(int)w_size-1;
//            wy2 = (int)y-(int)w_size-1;
//            if(wx1>=(int)width || wy1>=(int)height || wx2<0 || wy2<0){ continue; }
//
//            sum = (double)(corrSumMap.GetElem((u32)wx1, (u32)wy1)-corrSumMap.GetElem((u32)wx2, (u32)wy1))
//                - (double)(corrSumMap.GetElem((u32)wx1, (u32)wy2)-corrSumMap.GetElem((u32)wx2, (u32)wy2));
//
//            //if(sum < io_corr.GetElem(x, y))
//            if(sum < io_corr[i])
//            {
//                // normalize average of SSD
//                corr_av_norm = (sum/(double)((w_size*2+1)*(w_size*2+1))) / 65025.0; // (255*255 = 65025)
//                if(corr_av_norm > thre_corr){ continue; }
//
//                io_corr.SetElem(sum, i);
//                //value = sqrt((double)(disp_x*disp_x+disp_y*disp_y));
//                //value = disp_x;
//                //o_depth.SetElem(value, i);
//                o_depth[i*o_depth.num_of_channels()  ] = disp_x;
//                o_depth[i*o_depth.num_of_channels()+1] = disp_y;
//                o_depth.SetState(WSP_PIXEL_STATE_NORMAL, i);
//            }
//        }
//    }
//    return WSP_STATE_SUCCESS;
//}


//
//
//void UpdateCorrelationMap_FunctionFitting2(
//    const uchar *in_main, 
//    const uchar *in_sub, 
//    uchar const *in_mask, 
//    double *io_depth, 
//    uchar *io_state,
//    double *io_corr, 
//    double **io_corr_min,
//    double **io_tmp_corr,
//    int width, int height, int num_channels,
//    int num_corr_img,
//    int disp_x, int disp_y, 
//    int win_size,
//    const wsp::img::StereoBMParams &params,
//    wsp::math::FittingFunction fit_func,
//    double *tmp_corr_sum,
//    const double *in_win_size_map
//){
//    double *corr_sum_map = tmp_corr_sum;
//    int dx_step = disp_x*num_channels;
//    int width_step = width*num_channels;
//    int length = width * height;
//    int num_elem = length * num_channels;
//
//    if(tmp_corr_sum==NULL){
//        corr_sum_map = (double*)malloc(sizeof(double)*length);
//    }
//
//
//    switch(params.eval_func){
//    case WSP_CORRELATION_FUNCTION_SAD:
//        GetDirectionalSumOfCorr(in_main, in_sub,  corr_sum_map, width, height, num_channels, disp_x, disp_y, wsp::math::GetAbsDiff);
//        break;
//    case WSP_CORRELATION_FUNCTION_SSD:
//        GetDirectionalSumOfCorr(in_main, in_sub,  corr_sum_map, width, height, num_channels, disp_x, disp_y, wsp::math::GetSquaredDiff);
//        break;
//    case WSP_CORRELATION_FUNCTION_NCC:
//        GetSumOfNormalizedCrossCorr(in_main, in_sub, corr_sum_map, width, height, num_channels, disp_x, disp_y, win_size);
//        break;
//    case WSP_CORRELATION_FUNCTION_ZNCC:
//        GetSumOfZeroMeanNormalizedCrossCorr(in_main, in_sub, corr_sum_map, width, height, num_channels, disp_x, disp_y, win_size);
//        break;
//    default:
//        WSP_COMMON_ERROR_LOG("unrecognized correlation function: %d\n", params.eval_func);
//        return;
//    }
//
//
//    // Update correlation map --------------
//    int w_size = win_size;
//    int w2_size = win_size*2 + 1;
//    int init_offset = (w_size+1)*width + w_size+1;
//    int end_w_step = (width - w2_size);
//
//    int corr_max_index = num_corr_img-1;
//    int corr_center_index = corr_max_index/2;
//
//    {
//        double sum;
//        double corr_av_norm;
//        int i;
//        double peak_x, peak_y, peak_corr;
//        double *corr_sum_ptr = corr_sum_map + init_offset;
//        double *end_ptr      = corr_sum_map + (length-w_size*width+w_size+1);
//        double *corr_ptr     = io_corr      + init_offset;
//
//        const uchar *mask_ptr= in_mask      + init_offset;
//        //uchar *state_ptr     = io_state     + init_offset;
//        //double *dst_ptr      = io_depth     + init_offset*2;
//        double *end_w_ptr;
//
//        double **corr_tmp_ptr = WSP_NEW double*[num_corr_img];
//        double **corr_min_ptr = WSP_NEW double*[num_corr_img];
//        for(int i=0; i<num_corr_img; ++i){
//            corr_tmp_ptr[i] = io_tmp_corr[i] + init_offset; 
//            corr_min_ptr[i] = io_corr_min[i] + init_offset;
//        }
//
//
//        const double *win_size_ptr = NULL;
//        if(in_win_size_map!=NULL){
//            win_size_ptr = in_win_size_map + init_offset;
//        }
//
//        //for(i=init_offset; corr_sum_ptr<end_ptr; corr_ptr+=w2_size, corr_sum_ptr+=w2_size, mask_ptr+=w2_size, i+=w2_size){
//        //    end_w_ptr = corr_sum_ptr + end_w_step;
//        //    //x=w_size+1;
//        //    for(; corr_sum_ptr!=end_w_ptr; 
//        //        ++corr_ptr, 
//        //        ++corr_tmp_ptr0, 
//        //        ++corr_tmp_ptr1, 
//        //        ++corr_tmp_ptr2, 
//        //        ++corr_min_ptr0, 
//        //        ++corr_min_ptr1, 
//        //        ++corr_min_ptr2, 
//        //        ++corr_sum_ptr, 
//        //        ++mask_ptr, 
//        //        ++i
//        //    ){
//        //        if(*mask_ptr==WSP_PIXEL_STATE_MASKED){ continue; }
//
//        //        if(in_win_size_map!=NULL){
//        //            w_size =*win_size_ptr;
//        //            ++win_size_ptr;
//        //        }
//        //        //wx1 = x+w_size;
//        //        //wy1 = y+w_size;
//        //        //wx2 = x-w_size-1;
//        //        //wy2 = y-w_size-1;
//        //        //if(wx1>=width || wy1>=height || wx2<0 || wy2<0){ continue; }
//        //        double *ptr_l_bottom= corr_sum_ptr + w_size*width - (w_size+1);
//        //        double *ptr_l_top   = corr_sum_ptr - (w_size+1)*width - (w_size+1);
//        //        double *ptr_r_bottom= ptr_l_bottom + w2_size;
//        //        double *ptr_r_top   = ptr_l_top    + w2_size;
//
//        //        //sum = (corr_sum_map[wy1*width+wx1]-corr_sum_map[wy1*width+wx2])
//        //        //    - (corr_sum_map[wy2*width+wx1]-corr_sum_map[wy2*width+wx2]);
//        //        sum = (*ptr_r_bottom - *ptr_l_bottom) - (*ptr_r_top - *ptr_l_top);
//
//        //        *corr_tmp_ptr0 = *corr_tmp_ptr1;
//        //        *corr_tmp_ptr1 = *corr_tmp_ptr2;
//        //        *corr_tmp_ptr2 = sum;
//
//        //        if(*corr_tmp_ptr0==DBL_MAX){ // skip first disparity among searching
//        //            continue;
//        //        }
//
//        //        if(*corr_tmp_ptr1 < *corr_ptr
//        //        && *corr_tmp_ptr1 < *corr_tmp_ptr0 
//        //        && *corr_tmp_ptr1 < *corr_tmp_ptr2
//        //        ){
//        //            // normalize average of SSD
//        //            corr_av_norm = (*corr_tmp_ptr1/(double)((w_size*2+1)*(w_size*2+1))) / 65025.0; // (255*255 = 65025)
//        //            if(corr_av_norm > thre_corr){ continue; }
//
//
//        //            // function fitting
//        //            if(fit_func==WSP_FITTING_FUNCTION_EQUIANGULAR_LINE){
//        //                if(*corr_tmp_ptr0>*corr_tmp_ptr2){
//        //                    wsp::math::FindPeakOfEquiangularLine(
//        //                        -1, 0, 1, 
//        //                        *corr_tmp_ptr0, *corr_tmp_ptr1, *corr_tmp_ptr2,
//        //                        peak_x, peak_corr
//        //                    );
//        //                }else{
//        //                    wsp::math::FindPeakOfEquiangularLine(
//        //                        1, 0, -1, 
//        //                        *corr_tmp_ptr2, *corr_tmp_ptr1, *corr_tmp_ptr0,
//        //                        peak_x, peak_corr
//        //                    );
//        //                }
//        //            }else if(fit_func==WSP_FITTING_FUNCTION_PARABOLA){
//        //                // parabola fitting
//        //                wsp::math::FindQuadraticPeak(
//        //                    -1, 0, 1, 
//        //                    *corr_tmp_ptr0, *corr_tmp_ptr1, *corr_tmp_ptr2,
//        //                    peak_x, peak_corr
//        //                );
//        //            }
//
//        //            *corr_ptr = peak_corr;
//        //            io_depth[i*2] = disp_x - 1 + peak_x;
//        //            io_depth[i*2+1] = disp_y;
//        //            io_state[i] = WSP_PIXEL_STATE_NORMAL;
//        //        }
//        //    }
//        //}
//        
//        if(fit_func==WSP_FITTING_FUNCTION_EQUIANGULAR_LINE){
//            for(i=init_offset; corr_sum_ptr<end_ptr; corr_ptr+=w2_size, corr_sum_ptr+=w2_size, mask_ptr+=w2_size, i+=w2_size){
//                end_w_ptr = corr_sum_ptr + end_w_step;
//                //x=w_size+1;
//                for(; corr_sum_ptr!=end_w_ptr; 
//                    ++corr_ptr, 
//                    ++corr_sum_ptr, 
//                    ++mask_ptr, 
//                    ++i
//                ){
//                    if(*mask_ptr==WSP_PIXEL_STATE_MASKED){ 
//                        for(int i=0; i<num_corr_img; ++i){ ++corr_tmp_ptr[i]; ++corr_min_ptr[i]; }
//                        continue; 
//                    }
//
//                    if(in_win_size_map!=NULL){
//                        w_size =*win_size_ptr;
//                        ++win_size_ptr;
//                    }
//                    //wx1 = x+w_size;
//                    //wy1 = y+w_size;
//                    //wx2 = x-w_size-1;
//                    //wy2 = y-w_size-1;
//                    //if(wx1>=width || wy1>=height || wx2<0 || wy2<0){ continue; }
//                    double *ptr_l_bottom= corr_sum_ptr + w_size*width - (w_size+1);
//                    double *ptr_l_top   = corr_sum_ptr - (w_size+1)*width - (w_size+1);
//                    double *ptr_r_bottom= ptr_l_bottom + w2_size;
//                    double *ptr_r_top   = ptr_l_top    + w2_size;
//
//                    //sum = (corr_sum_map[wy1*width+wx1]-corr_sum_map[wy1*width+wx2])
//                    //    - (corr_sum_map[wy2*width+wx1]-corr_sum_map[wy2*width+wx2]);
//                    sum = (*ptr_r_bottom - *ptr_l_bottom) - (*ptr_r_top - *ptr_l_top);
//
//                    for(int j=0; i<corr_max_index; ++j){ 
//                        *(corr_tmp_ptr[j])=*(corr_tmp_ptr[j+1]); 
//                    }
//                    *(corr_tmp_ptr[num_corr_img-1]) = sum;
//
//                    if(*(corr_tmp_ptr[corr_center_index]) >= *corr_ptr
//                    || *(corr_tmp_ptr[0])==DBL_MAX
//                    ){ 
//                        for(int i=0; i<num_corr_img; ++i){ ++corr_tmp_ptr[i]; ++corr_min_ptr[i]; }
//                        continue; 
//                    }
//                    // normalize average of SSD
//                    corr_av_norm = (*corr_tmp_ptr[corr_center_index]/(double)((w_size*2+1)*(w_size*2+1))) / 65025.0; // (255*255 = 65025)
//                    if(corr_av_norm > params.thre_corr){ 
//                        for(int i=0; i<num_corr_img; ++i){ ++corr_tmp_ptr[i]; ++corr_min_ptr[i]; }
//                        continue; 
//                    }
//
//                    // function fitting
//                    int sample_cnt = 0;
//                    double peak_x_av = 0.0;
//                    double peak_corr_av = 0.0;
//                    int rhs_index;
//                    for(int j=0; j<corr_center_index; ++j){
//                        rhs_index = corr_max_index-j;
//                        if(*corr_tmp_ptr[j] >= *corr_tmp_ptr[corr_center_index] 
//                        || *corr_tmp_ptr[rhs_index] >= *corr_tmp_ptr[corr_center_index]
//                        ){
//                            continue;
//                        }
//                        if(*corr_tmp_ptr[j]>*corr_tmp_ptr[rhs_index]){
//                            wsp::math::FindPeakOfEquiangularLine(
//                                -1, 0, 1, 
//                                *corr_tmp_ptr[j], *corr_tmp_ptr[corr_center_index], *corr_tmp_ptr[rhs_index],
//                                peak_x, peak_corr
//                            );
//                        }else{
//                            wsp::math::FindPeakOfEquiangularLine(
//                                1, 0, -1, 
//                                *corr_tmp_ptr[rhs_index], *corr_tmp_ptr[corr_center_index], *corr_tmp_ptr[j],
//                                peak_x, peak_corr
//                            );
//                        }
//                        peak_x_av += peak_x;
//                        peak_corr_av += peak_corr;
//                        ++sample_cnt;
//                    }
//                    if(sample_cnt==0){
//                        continue;
//                        for(int i=0; i<num_corr_img; ++i){ ++corr_tmp_ptr[i]; ++corr_min_ptr[i]; }
//                    }
//                    peak_x_av/=static_cast<double>(sample_cnt);
//                    peak_corr_av/=static_cast<double>(sample_cnt);
//
//
//
//                    *corr_ptr = peak_corr;
//                    io_depth[i*2] = disp_x - 1 + peak_x;
//                    io_depth[i*2+1] = disp_y;
//                    io_state[i] = WSP_PIXEL_STATE_NORMAL;
//
//                    for(int i=0; i<num_corr_img; ++i){ ++corr_tmp_ptr[i]; ++corr_min_ptr[i]; }
//                }
//            }
//        }else if(fit_func==WSP_FITTING_FUNCTION_PARABOLA){
//            for(i=init_offset; corr_sum_ptr<end_ptr; corr_ptr+=w2_size, corr_sum_ptr+=w2_size, mask_ptr+=w2_size, i+=w2_size){
//                end_w_ptr = corr_sum_ptr + end_w_step;
//                //x=w_size+1;
//                for(; corr_sum_ptr!=end_w_ptr; 
//                    ++corr_ptr, 
//                    ++corr_sum_ptr, 
//                    ++mask_ptr, 
//                    ++i
//                ){
//                    if(*mask_ptr==WSP_PIXEL_STATE_MASKED){ 
//                        for(int i=0; i<num_corr_img; ++i){ ++corr_tmp_ptr[i]; ++corr_min_ptr[i]; }
//                        continue; 
//                    }
//
//                    if(in_win_size_map!=NULL){
//                        w_size =*win_size_ptr;
//                        ++win_size_ptr;
//                    }
//                    //wx1 = x+w_size;
//                    //wy1 = y+w_size;
//                    //wx2 = x-w_size-1;
//                    //wy2 = y-w_size-1;
//                    //if(wx1>=width || wy1>=height || wx2<0 || wy2<0){ continue; }
//                    double *ptr_l_bottom= corr_sum_ptr + w_size*width - (w_size+1);
//                    double *ptr_l_top   = corr_sum_ptr - (w_size+1)*width - (w_size+1);
//                    double *ptr_r_bottom= ptr_l_bottom + w2_size;
//                    double *ptr_r_top   = ptr_l_top    + w2_size;
//
//                    //sum = (corr_sum_map[wy1*width+wx1]-corr_sum_map[wy1*width+wx2])
//                    //    - (corr_sum_map[wy2*width+wx1]-corr_sum_map[wy2*width+wx2]);
//                    sum = (*ptr_r_bottom - *ptr_l_bottom) - (*ptr_r_top - *ptr_l_top);
//
//                    for(int j=0; i<corr_max_index; ++j){ 
//                        *(corr_tmp_ptr[j])=*(corr_tmp_ptr[j+1]); 
//                    }
//                    *(corr_tmp_ptr[num_corr_img-1]) = sum;
//
//                    if(*(corr_tmp_ptr[corr_center_index]) >= *corr_ptr
//                    || *(corr_tmp_ptr[0])==DBL_MAX
//                    ){ 
//                        for(int i=0; i<num_corr_img; ++i){ ++corr_tmp_ptr[i]; ++corr_min_ptr[i]; }
//                        continue; 
//                    }
//                    // normalize average of SSD
//                    corr_av_norm = (*corr_tmp_ptr[corr_center_index]/(double)((w_size*2+1)*(w_size*2+1))) / 65025.0; // (255*255 = 65025)
//                    if(corr_av_norm > params.thre_corr){ 
//                        for(int i=0; i<num_corr_img; ++i){ ++corr_tmp_ptr[i]; ++corr_min_ptr[i]; }
//                        continue; 
//                    }
//
//                    // parabola fitting
//                    int sample_cnt = 0;
//                    double peak_x_av = 0.0;
//                    double peak_corr_av = 0.0;
//                    for(int j=0; j<corr_center_index; ++j){
//                        if(*corr_tmp_ptr[j] >= *corr_tmp_ptr[corr_center_index] 
//                        || *corr_tmp_ptr[corr_max_index-j] >= *corr_tmp_ptr[corr_center_index]
//                        ){
//                            continue;
//                        }
//                        wsp::math::FindQuadraticPeak(
//                            -1, 0, 1, 
//                            *corr_tmp_ptr[j], *corr_tmp_ptr[corr_center_index], *corr_tmp_ptr[corr_max_index-j],
//                            peak_x, peak_corr
//                        );
//                        peak_x_av += peak_x;
//                        peak_corr_av += peak_corr;
//                        ++sample_cnt;
//                    }
//                    if(sample_cnt==0){
//                        continue;
//                        for(int i=0; i<num_corr_img; ++i){ ++corr_tmp_ptr[i]; ++corr_min_ptr[i]; }
//                    }
//                    peak_x_av/=static_cast<double>(sample_cnt);
//                    peak_corr_av/=static_cast<double>(sample_cnt);
//
//                    *corr_ptr = peak_corr_av;
//                    io_depth[i*2] = disp_x - 1 + peak_x_av;
//                    io_depth[i*2+1] = disp_y;
//                    io_state[i] = WSP_PIXEL_STATE_NORMAL;
//
//                    for(int i=0; i<num_corr_img; ++i){ ++corr_tmp_ptr[i]; ++corr_min_ptr[i]; }
//                }
//            }
//        }
//        delete[] corr_tmp_ptr;
//        delete[] corr_min_ptr;
//    }
//
//    //// unoptimized code ----------------------------------------------
//    ////#ifdef _OPENMP
//    ////#pragma omp parallel
//    ////#endif
//    //{
//    //    double sum;
//    //    double corr_av_norm;
//    //    int i;
//    //    double peak_x, peak_y, peak_corr;
//    //    double corr0, corr1, corr2;
//    //    double value;
//    //    int x, y;
//    //    int dx, dy, wx1, wy1, wx2, wy2, subst;
//
//    //    i=0;
//    //    //#ifdef _OPENMP
//    //    //#pragma omp for
//    //    //#endif
//    //    for(y=0; y<height /*dst_ptr!=end_ptr*/; ++y){
//    //        for(x=0; x<width; ++x){
//    //            i = y*width + x;
//
//    //            if(in_mask[i]==WSP_PIXEL_STATE_MASKED){ continue; }
//
//    //            dx = x + disp_x;
//    //            dy = y + disp_y;
//    //            if(dx<0 || dx>=width || dy<0 || dy>=height){ continue; }
//
//    //            w_size=win_size;
//    //            //w_size =in_win_size_map[i];
//    //            wx1 = x+w_size;
//    //            wy1 = y+w_size;
//    //            wx2 = x-w_size-1;
//    //            wy2 = y-w_size-1;
//    //            if(wx1>=width || wy1>=height || wx2<0 || wy2<0){ continue; }
//
//    //            sum = (corr_sum_map[wy1*width+wx1]-corr_sum_map[wy1*width+wx2])
//    //                - (corr_sum_map[wy2*width+wx1]-corr_sum_map[wy2*width+wx2]);
//
//    //            io_tmp_corr[0][i] = io_tmp_corr[1][i];
//    //            io_tmp_corr[1][i] = io_tmp_corr[2][i];
//    //            io_tmp_corr[2][i] = sum;
//
//    //            corr0 = io_tmp_corr[0][i];
//    //            corr1 = io_tmp_corr[1][i];
//    //            corr2 = io_tmp_corr[2][i];
//
//    //            if(corr0==DBL_MAX){ // skip first disparity among searching
//    //                continue;
//    //            }
//
//    //            // update correlation map if current decorrelation is lower
//    //            if(corr1 < io_corr[i]
//    //            && corr1 < corr0 
//    //            && corr1 < corr2
//    //            ){
//    //                // normalize average of SSD
//    //                corr_av_norm = (corr1/(double)((w_size*2+1)*(w_size*2+1))) / 65025.0; // (255*255 = 65025)
//    //                if(corr_av_norm > thre_corr){ continue; }
//
//    //                wsp::math::FindQuadraticPeak(
//    //                    -1, 0, 1, 
//    //                    corr0, corr1, corr2,
//    //                    peak_x, peak_corr
//    //                );
//
//    //                io_corr[i] = peak_corr;
//    //                io_depth[i*2] = disp_x - 1 + peak_x;
//    //                io_depth[i*2+1] = disp_y;
//    //                io_state[i] = WSP_PIXEL_STATE_NORMAL;
//    //            }
//    //        }
//    //    }
//    //}
//    if(tmp_corr_sum==NULL){
//        free(corr_sum_map);
//    }
//}
//
//
//
//wsp::State StereoBMFunctionFitting2(
//        wsp::Image<double> &o_depth, 
//        const wsp::Image<uchar> &in_main_img, 
//        const wsp::Image<uchar> &in_sub_img, 
//        const wsp::img::StereoBMParams &params, 
//        const wsp::Image<double> *in_depth_ref
//){
//    int disp_size_x, disp_size_y, disp_size;
//    wsp::Image<double> win_size_map;
//    wsp::Image<double> *corr_tmp_map; // 1-2:save old corr for parabola, 0: save min corr
//    wsp::Image<double> corr_map, *corr_min_map;
//
//    int num_corr_img = params.subpixel_sample_range*2+1;
//    corr_tmp_map = WSP_NEW wsp::Image<double>[num_corr_img];
//    corr_min_map = WSP_NEW wsp::Image<double>[num_corr_img];
//
//    int width = in_main_img.width();
//    int height = in_main_img.height();
//    int length = width*height;
//    int num_channels = in_main_img.num_of_channels();
//    int num_elem = width*height*num_channels;
//
//    if(width!=in_sub_img.width() || height!=in_sub_img.height()){
//        WSP_COMMON_ERROR_LOG("The size of main and sub image are different: %d!=%d or %d!=%d\n", 
//                    width, in_sub_img.width(), height, in_sub_img.height()); 
//        return WSP_STATE_INVALID_PARAMETER;
//    }
//
//    // Initialize maps
//    o_depth.SetSize(width, height, 2);
//    for(int i=0; i<num_corr_img; ++i){
//        corr_tmp_map[i].SetSize(width, height);
//        corr_min_map[i].SetSize(width, height);
//        corr_tmp_map[i].Reset(DBL_MAX);
//        corr_min_map[i].Reset(DBL_MAX);
//    }
//    corr_map.SetSize(width, height);
//    corr_map.Reset(DBL_MAX);
//
//    // initialize data
//    memset(o_depth.data(), 0, sizeof(double)*length*2);
//    memset(o_depth.state_data(), WSP_PIXEL_STATE_OCCLUDED, length);
//    
//
//    disp_size_x = params.dispx_max - params.dispx_min+1;
//    disp_size_y = params.dispy_max - params.dispy_min+1;
//    disp_size = disp_size_x*disp_size_y;
//
//    if(params.show_debug_info){
//        printf("Stereo Block Matching:\n"
//               "    block size: %d %d \n"
//               "    disp range: x(%d, %d), y(%d, %d) \n"
//               "    thre_std_deviation: %f\n"
//               "    thre_corr: %f\n", 
//               "    scale_thre_enough_edge_amount: %f\n",
//                params.block_min, params.block_max,
//                params.dispx_min, params.dispx_max, 
//                params.dispy_min, params.dispy_max, 
//                params.thre_std_deviation, 
//                params.thre_corr,
//                params.scale_thre_enough_edge_amount
//        );
//    }
//
//    // Get adaptive window size for each pixels
//    if(params.block_min<params.block_max){
//        wsp::Image<double> edgeImg;
//        wsp::State state=wsp::img::LaplacianFilter(edgeImg, in_main_img);
//        if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Applying Laplacian Filter failed\n"); return state; }
//        edgeImg.Abs();
//        wsp::img::AverageFilter<double>(edgeImg);
//        if(params.save_debug_data){
//            edgeImg.SaveImage("edgeImage.tif");
//        }
//        double enough_edge_amount = wsp::GetMax(edgeImg.data(), edgeImg.width()*edgeImg.height()) * (width/4) * params.scale_thre_enough_edge_amount;
//
//        GetAdaptiveWindowSize(
//            win_size_map, in_main_img, edgeImg,
//            params.block_min, params.block_max, 
//            enough_edge_amount,
//            params.save_debug_data
//        );
//    }
//    if(params.save_debug_data){
//        win_size_map.SaveImage("win_size_map.tif");
//    }
//    //win_size_map.SaveImage("win_size_map.tif");
//
//    // mask the none-charactaristic area
//    //wsp::img::GetCharacteristicMaskByVariance(corr_map.state_data(), (uchar*)in_main_img.data(), win_size_map.data(),
//    //    width, height, in_main_img.num_of_channels(), params.thre_std_deviation);
//    if(params.thre_std_deviation!=0){
//        printf("Calculating characteristic mask...\r");
//        wsp::img::GetCharacteristicMaskByDeviation(
//            corr_map.state_data(),
//            in_main_img.data(), 
//            width, height, num_channels, 
//            params.block_min,
//            params.thre_std_deviation,
//            NULL //win_size_map.data()
//        );
//    }
//    if(params.save_debug_data){
//        wsp::Image<double> tmp_img = corr_map[0];
//        tmp_img.SaveState("stereo_mask.tif", WSP_PIXEL_STATE_MASKED);
//    }
//
//    double *win_map_ptr = NULL;
//    if(params.block_min<params.block_max){
//        win_map_ptr = win_size_map.data();
//    }
//
//    wsp::math::FittingFunction using_fit_func;
//    if(params.subpixel_fitting_function==WSP_FITTING_FUNCTION_UNDEFINED){
//        using_fit_func=wsp::math::DecideFittingFuncByCorrFunc(params.eval_func);
//    }
//
//    // +/- 1 because subpixel estimation needs at least 3 points for calculation of peak.
//    int dispx_max = params.dispx_max + 1;
//
//    // Search disparity range to find a pixel which has minimum correlation value
//    #ifdef USE_QT
//    QProgressDialog *progress=NULL;
//    if(params.show_progress){
//        progress=new QProgressDialog("Correlation Stereo Matching...", "Cancel", 0, disp_size);
//        progress->setWindowModality(Qt::WindowModal);
//    }
//    #endif
//
//    double **corr_tmp_data = WSP_NEW double*[num_corr_img];
//    double **corr_min_data = WSP_NEW double*[num_corr_img];
//    for(int i=0; i<num_corr_img; ++i){
//        corr_tmp_data[i] = corr_tmp_map[i].data();
//        corr_min_data[i] = corr_min_map[i].data();
//    }
//
//    //#ifdef _OPENMP
//    //#pragma omp parallel
//    //#endif
//    {
//        int dx, dy, d; // iterators
//        double *tmp_corr_sum = (double*)malloc(sizeof(double)*length);
//
//        //#ifdef _OPENMP
//        //#pragma omp for
//        //#endif
//        for(dx=params.dispx_min-1; dx<=dispx_max; ++dx){
//            for(dy=params.dispy_min; dy<=params.dispy_max; ++dy){
//                //printf("%d\n", omp_get_thread_num());
//                // display progress information ------------------------
//                if(params.show_progress){
//                    d = (dx-params.dispx_min)*disp_size_y + (dy-params.dispy_min);
//                    #ifdef USE_QT
//                    if(omp_get_thread_num()==0){
//                        progress->setValue(d*omp_get_max_threads());
//                        //if(progress->wasCanceled()){ return WSP_STATE_CANCELED; }
//                        qApp->processEvents();
//                    }
//                    #else
//                    printf("%3f %% done..\r", 100*d/static_cast<float>(disp_size));
//                    #endif
//                }
//                // ------------------------ end display progress information
//
//                // computing disparities
//                //printf("%d\n", dx);
//                //wsp::img::UpdateCorrelationMap(
//                //    o_depth, 
//                //    corr_map, 
//                //    in_main_img, 
//                //    in_sub_img, 
//                //    win_size_map, 
//                //    corr_map.state_data(), 
//                //    dx, dy, 
//                //    params.thre_corr, 
//                //    params.eval_func
//                //);
//                ::UpdateCorrelationMap_FunctionFitting2(
//                    in_main_img.data(), 
//                    in_sub_img.data(), 
//                    corr_map.state_data(), 
//                    o_depth.data(), 
//                    o_depth.state_data(),
//                    corr_map.data(), 
//                    corr_min_data,
//                    corr_tmp_data,
//                    width, height, num_channels,
//                    num_corr_img,
//                    dx, dy, 
//                    params.block_min,
//                    params,
//                    using_fit_func,
//                    tmp_corr_sum,
//                    win_map_ptr
//                );
//            }
//        }
//        free(tmp_corr_sum);
//    }
//
//    #ifdef USE_QT
//    if(progress->wasCanceled()){ delete progress; return WSP_STATE_CANCELED; }
//    delete progress;
//    #endif
//
//    //printf("\n");
//    // DEBUG ---------------------------
//    //double min, max;
//    //wsp::GetMinMax(o_depth.data(), min, max, o_depth.length()*o_depth.num_of_channels());
//    //printf("DEBUG: disparity min=%f, max=%f\n", min, max);
//    // -------------------------------
//    delete[] corr_tmp_map;
//    delete[] corr_min_map;
//
//
//    return WSP_STATE_SUCCESS;
//}






template<typename T>
void UpdateCorrelationMap_FunctionFitting(
    const T *in_main, 
    const T *in_sub, 
    uchar const *in_mask, 
    double *io_depth, 
    uchar *io_state,
    double *io_corr, 
    double *io_corr_min[3],
    double *io_tmp_corr[3],
    int width, int height, int num_channels,
    int disp_x, int disp_y, 
    int win_size,
    double thre_corr, 
    wsp::math::CorrelationFunction corr_func,
    wsp::math::FittingFunction fit_func,
    double *tmp_corr_sum,
    const double *in_win_size_map
){
    double *corr_sum_map = tmp_corr_sum;
    int dx_step = disp_x*num_channels;
    int width_step = width*num_channels;
    int length = width * height;
    int num_elem = length * num_channels;

    if(tmp_corr_sum==NULL){
        corr_sum_map = (double*)malloc(sizeof(double)*length);
    }

    switch(corr_func){
    case WSP_CORRELATION_FUNCTION_SAD:
        wsp::img::GetDirectionalSumOfCorr(in_main, in_sub,  corr_sum_map, width, height, num_channels, disp_x, disp_y, wsp::math::GetAbsDiff);
        break;
    case WSP_CORRELATION_FUNCTION_SSD:
        wsp::img::GetDirectionalSumOfCorr(in_main, in_sub,  corr_sum_map, width, height, num_channels, disp_x, disp_y, wsp::math::GetSquaredDiff);
        break;
    case WSP_CORRELATION_FUNCTION_CC:
        wsp::img::GetDirectionalSumOfCorr(in_main, in_sub,  corr_sum_map, width, height, num_channels, disp_x, disp_y, wsp::math::GetCrossDecorr);
        break;
    case WSP_CORRELATION_FUNCTION_NCC:
        GetSumOfNormalizedCrossCorr(in_main, in_sub, corr_sum_map, width, height, num_channels, disp_x, disp_y, win_size);
        break;
    case WSP_CORRELATION_FUNCTION_ZNCC:
        GetSumOfZeroMeanNormalizedCrossCorr(in_main, in_sub, corr_sum_map, width, height, num_channels, disp_x, disp_y, win_size);
        break;
    default:
        WSP_COMMON_ERROR_LOG("unrecognized correlation function: %d\n", corr_func);
        return;
    }


    // Update correlation map --------------
    int w_size = win_size;
    int w2_size = win_size*2 + 1;
    int init_offset = (w_size+1)*width + w_size+1;
    int end_w_step = (width - w2_size);

    #define USE_OPT_UPDATECORRELATIONMAP_FUNCTIONFITTING_CODE (1)
    #if     USE_OPT_UPDATECORRELATIONMAP_FUNCTIONFITTING_CODE==1
    {
        double sum;
        double corr_av_norm;
        int x, y, i;
        int wx1, wx2, wy1, wy2;
        double peak_x, peak_y, peak_corr;
        double *corr_sum_ptr = corr_sum_map + init_offset;
        double *end_ptr      = corr_sum_map + (length-w_size*width+w_size+1);
        double *corr_ptr     = io_corr      + init_offset;

        const uchar *mask_ptr= in_mask      + init_offset;
        //uchar *state_ptr     = io_state     + init_offset;
        //double *dst_ptr      = io_depth     + init_offset*2;
        double *end_w_ptr;

        double *corr_tmp_ptr0 = io_tmp_corr[0] + init_offset; 
        double *corr_tmp_ptr1 = io_tmp_corr[1] + init_offset; 
        double *corr_tmp_ptr2 = io_tmp_corr[2] + init_offset; 
        double *corr_min_ptr0 = io_corr_min[0] + init_offset;
        double *corr_min_ptr1 = io_corr_min[1] + init_offset;
        double *corr_min_ptr2 = io_corr_min[2] + init_offset;


        const double *win_size_ptr = NULL;
        if(in_win_size_map!=NULL){
            win_size_ptr = in_win_size_map + init_offset;
        }

        #define OPT_UPDATECORRELATIONMAP_FUNCTIONFITTING_CODE_ID (1)
        #if     OPT_UPDATECORRELATIONMAP_FUNCTIONFITTING_CODE_ID==0
        for(i=init_offset; corr_sum_ptr<end_ptr; corr_ptr+=w2_size, corr_sum_ptr+=w2_size, mask_ptr+=w2_size, i+=w2_size){
            end_w_ptr = corr_sum_ptr + end_w_step;
            //x=w_size+1;
            for(; corr_sum_ptr!=end_w_ptr; 
                ++corr_ptr, 
                ++corr_tmp_ptr0, 
                ++corr_tmp_ptr1, 
                ++corr_tmp_ptr2, 
                ++corr_min_ptr0, 
                ++corr_min_ptr1, 
                ++corr_min_ptr2, 
                ++corr_sum_ptr, 
                ++mask_ptr, 
                ++i
            ){
                if(*mask_ptr==WSP_PIXEL_STATE_MASKED){ continue; }

                if(in_win_size_map!=NULL){
                    w_size =*win_size_ptr;
                    ++win_size_ptr;
                }
                //wx1 = x+w_size;
                //wy1 = y+w_size;
                //wx2 = x-w_size-1;
                //wy2 = y-w_size-1;
                //if(wx1>=width || wy1>=height || wx2<0 || wy2<0){ continue; }
                double *ptr_l_bottom= corr_sum_ptr + w_size*width - (w_size+1);
                double *ptr_l_top   = corr_sum_ptr - (w_size+1)*width - (w_size+1);
                double *ptr_r_bottom= ptr_l_bottom + w2_size;
                double *ptr_r_top   = ptr_l_top    + w2_size;

                //sum = (corr_sum_map[wy1*width+wx1]-corr_sum_map[wy1*width+wx2])
                //    - (corr_sum_map[wy2*width+wx1]-corr_sum_map[wy2*width+wx2]);
                sum = (*ptr_r_bottom - *ptr_l_bottom) - (*ptr_r_top - *ptr_l_top);

                *corr_tmp_ptr0 = *corr_tmp_ptr1;
                *corr_tmp_ptr1 = *corr_tmp_ptr2;
                *corr_tmp_ptr2 = sum;

                if(*corr_tmp_ptr0==DBL_MAX){ // skip first disparity among searching
                    continue;
                }

                if(*corr_tmp_ptr1 < *corr_ptr
                && *corr_tmp_ptr1 < *corr_tmp_ptr0 
                && *corr_tmp_ptr1 < *corr_tmp_ptr2
                ){
                    // normalize average of SSD
                    corr_av_norm = (*corr_tmp_ptr1/(double)((w_size*2+1)*(w_size*2+1))) / 65025.0; // (255*255 = 65025)
                    if(corr_av_norm > thre_corr){ continue; }


                    // function fitting
                    if(fit_func==WSP_FITTING_FUNCTION_EQUIANGULAR_LINE){
                        if(*corr_tmp_ptr0>*corr_tmp_ptr2){
                            wsp::math::FindPeakOfEquiangularLine(
                                -1, 0, 1, 
                                *corr_tmp_ptr0, *corr_tmp_ptr1, *corr_tmp_ptr2,
                                peak_x, peak_corr
                            );
                        }else{
                            wsp::math::FindPeakOfEquiangularLine(
                                1, 0, -1, 
                                *corr_tmp_ptr2, *corr_tmp_ptr1, *corr_tmp_ptr0,
                                peak_x, peak_corr
                            );
                        }
                    }else if(fit_func==WSP_FITTING_FUNCTION_PARABOLA){
                        // parabola fitting
                        wsp::math::FindQuadraticPeak(
                            -1, 0, 1, 
                            *corr_tmp_ptr0, *corr_tmp_ptr1, *corr_tmp_ptr2,
                            peak_x, peak_corr
                        );
                    }

                    *corr_ptr = peak_corr;
                    io_depth[i*2] = disp_x - 1 + peak_x;
                    io_depth[i*2+1] = disp_y;
                    io_state[i] = WSP_PIXEL_STATE_NORMAL;
                }
            }
        }
        #else
        if(fit_func==WSP_FITTING_FUNCTION_EQUIANGULAR_LINE)
        {
            y = w_size+1;
            for(i=init_offset; corr_sum_ptr<end_ptr; corr_ptr+=w2_size, corr_sum_ptr+=w2_size, mask_ptr+=w2_size, i+=w2_size, ++y){
                end_w_ptr = corr_sum_ptr + end_w_step;
                x=w_size+1;
                for(; corr_sum_ptr!=end_w_ptr; 
                    ++corr_ptr, 
                    ++corr_tmp_ptr0, 
                    ++corr_tmp_ptr1, 
                    ++corr_tmp_ptr2, 
                    ++corr_min_ptr0, 
                    ++corr_min_ptr1, 
                    ++corr_min_ptr2, 
                    ++corr_sum_ptr, 
                    ++mask_ptr, 
                    ++i, 
                    ++x
                ){
                    if(*mask_ptr==WSP_PIXEL_STATE_MASKED){ continue; }

                    if(in_win_size_map!=NULL){
                        w_size =*win_size_ptr;
                        w2_size = w_size*2 + 1;
                        ++win_size_ptr;
                        wx1 = x+w_size;
                        wy1 = y+w_size;
                        wx2 = x-w_size-1;
                        wy2 = y-w_size-1;
                        if(wx1>=width || wy1>=height || wx2<0 || wy2<0){ continue; }
                    }
                    //wx1 = x+w_size;
                    //wy1 = y+w_size;
                    //wx2 = x-w_size-1;
                    //wy2 = y-w_size-1;
                    //if(wx1>=width || wy1>=height || wx2<0 || wy2<0){ continue; }
                    double *ptr_l_bottom= corr_sum_ptr + w_size*width - (w_size+1);
                    double *ptr_l_top   = corr_sum_ptr - (w_size+1)*width - (w_size+1);
                    double *ptr_r_bottom= ptr_l_bottom + w2_size;
                    double *ptr_r_top   = ptr_l_top    + w2_size;

                    //sum = (corr_sum_map[wy1*width+wx1]-corr_sum_map[wy1*width+wx2])
                    //    - (corr_sum_map[wy2*width+wx1]-corr_sum_map[wy2*width+wx2]);
                    sum = (*ptr_r_bottom - *ptr_l_bottom) - (*ptr_r_top - *ptr_l_top);

                    *corr_tmp_ptr0 = *corr_tmp_ptr1;
                    *corr_tmp_ptr1 = *corr_tmp_ptr2;
                    *corr_tmp_ptr2 = sum;

                    if((*corr_tmp_ptr1 >= *corr_ptr
                    || *corr_tmp_ptr1 >= *corr_tmp_ptr0 
                    || *corr_tmp_ptr1 >= *corr_tmp_ptr2)
                    || *corr_tmp_ptr0==DBL_MAX
                    ){ continue; }

                    // normalize average of SSD
                    corr_av_norm = (*corr_tmp_ptr1/(double)((w_size*2+1)*(w_size*2+1))) / 65025.0; // (255*255 = 65025)
                    if(corr_av_norm > thre_corr){ continue; }


                    // function fitting
                    if(*corr_tmp_ptr0>*corr_tmp_ptr2){
                        wsp::math::FindPeakOfEquiangularLine(
                            -1, 0, 1, 
                            *corr_tmp_ptr0, *corr_tmp_ptr1, *corr_tmp_ptr2,
                            peak_x, peak_corr
                        );
                    }else{
                        wsp::math::FindPeakOfEquiangularLine(
                            1, 0, -1, 
                            *corr_tmp_ptr2, *corr_tmp_ptr1, *corr_tmp_ptr0,
                            peak_x, peak_corr
                        );
                    }


                    *corr_ptr = peak_corr;
                    io_depth[i*2] = disp_x - 1 + peak_x;
                    io_depth[i*2+1] = disp_y;
                    io_state[i] = WSP_PIXEL_STATE_NORMAL;
                }
            }
        }
        else if(fit_func==WSP_FITTING_FUNCTION_PARABOLA)
        {
            y = w_size+1;
            for(i=init_offset; corr_sum_ptr<end_ptr; corr_ptr+=w2_size, corr_sum_ptr+=w2_size, mask_ptr+=w2_size, i+=w2_size, ++y){
                end_w_ptr = corr_sum_ptr + end_w_step;
                x=w_size+1;
                for(; corr_sum_ptr!=end_w_ptr; 
                    ++corr_ptr, 
                    ++corr_tmp_ptr0, 
                    ++corr_tmp_ptr1, 
                    ++corr_tmp_ptr2, 
                    ++corr_min_ptr0, 
                    ++corr_min_ptr1, 
                    ++corr_min_ptr2, 
                    ++corr_sum_ptr, 
                    ++mask_ptr, 
                    ++i,
                    ++x
                ){
                    if(*mask_ptr==WSP_PIXEL_STATE_MASKED){ continue; }

                    if(in_win_size_map!=NULL){
                        w_size =*win_size_ptr;
                        w2_size = w_size*2 + 1;
                        ++win_size_ptr;
                        wx1 = x+w_size;
                        wy1 = y+w_size;
                        wx2 = x-w_size-1;
                        wy2 = y-w_size-1;
                        if(wx1>=width || wy1>=height || wx2<0 || wy2<0){ continue; }

                    }
                    //wx1 = x+w_size;
                    //wy1 = y+w_size;
                    //wx2 = x-w_size-1;
                    //wy2 = y-w_size-1;
                    //if(wx1>=width || wy1>=height || wx2<0 || wy2<0){ continue; }
                    double *ptr_l_bottom= corr_sum_ptr + w_size*width - (w_size+1);
                    double *ptr_l_top   = corr_sum_ptr - (w_size+1)*width - (w_size+1);
                    double *ptr_r_bottom= ptr_l_bottom + w2_size;
                    double *ptr_r_top   = ptr_l_top    + w2_size;

                    //sum = (corr_sum_map[wy1*width+wx1]-corr_sum_map[wy1*width+wx2])
                    //    - (corr_sum_map[wy2*width+wx1]-corr_sum_map[wy2*width+wx2]);
                    sum = (*ptr_r_bottom - *ptr_l_bottom) - (*ptr_r_top - *ptr_l_top);

                    *corr_tmp_ptr0 = *corr_tmp_ptr1;
                    *corr_tmp_ptr1 = *corr_tmp_ptr2;
                    *corr_tmp_ptr2 = sum;

                    if((*corr_tmp_ptr1 >= *corr_ptr
                    || *corr_tmp_ptr1 >= *corr_tmp_ptr0 
                    || *corr_tmp_ptr1 >= *corr_tmp_ptr2)
                    || *corr_tmp_ptr0==DBL_MAX
                    ){ continue; }
                    // normalize average of SSD
                    corr_av_norm = (*corr_tmp_ptr1/(double)((w_size*2+1)*(w_size*2+1))) / 65025.0; // (255*255 = 65025)
                    if(corr_av_norm > thre_corr){ continue; }

                    // parabola fitting
                    wsp::math::FindQuadraticPeak(
                        -1, 0, 1, 
                        *corr_tmp_ptr0, *corr_tmp_ptr1, *corr_tmp_ptr2,
                        peak_x, peak_corr
                    );

                    *corr_ptr = peak_corr;
                    io_depth[i*2] = disp_x - 1 + peak_x;
                    io_depth[i*2+1] = disp_y;
                    io_state[i] = WSP_PIXEL_STATE_NORMAL;
                }
            }
        }
        #endif
    }
    #else
    // unoptimized code ----------------------------------------------
    //#ifdef _OPENMP
    //#pragma omp parallel
    //#endif
    {
        double sum;
        double corr_av_norm;
        int i;
        double peak_x, peak_y, peak_corr;
        double corr0, corr1, corr2;
        double value;
        int x, y;
        int dx, dy, wx1, wy1, wx2, wy2, subst;

        i=0;
        //#ifdef _OPENMP
        //#pragma omp for
        //#endif
        for(y=0; y<height /*dst_ptr!=end_ptr*/; ++y){
            for(x=0; x<width; ++x){
                i = y*width + x;

                if(in_mask[i]==WSP_PIXEL_STATE_MASKED){ continue; }

                dx = x + disp_x;
                dy = y + disp_y;
                if(dx<0 || dx>=width || dy<0 || dy>=height){ continue; }

                w_size=win_size;
                //w_size =in_win_size_map[i];
                wx1 = x+w_size;
                wy1 = y+w_size;
                wx2 = x-w_size-1;
                wy2 = y-w_size-1;
                if(wx1>=width || wy1>=height || wx2<0 || wy2<0){ continue; }

                sum = (corr_sum_map[wy1*width+wx1]-corr_sum_map[wy1*width+wx2])
                    - (corr_sum_map[wy2*width+wx1]-corr_sum_map[wy2*width+wx2]);

                io_tmp_corr[0][i] = io_tmp_corr[1][i];
                io_tmp_corr[1][i] = io_tmp_corr[2][i];
                io_tmp_corr[2][i] = sum;

                corr0 = io_tmp_corr[0][i];
                corr1 = io_tmp_corr[1][i];
                corr2 = io_tmp_corr[2][i];

                if(corr0==DBL_MAX){ // skip first disparity among searching
                    continue;
                }

                // update correlation map if current decorrelation is lower
                if(corr1 < io_corr[i]
                && corr1 < corr0 
                && corr1 < corr2
                ){
                    // normalize average of SSD
                    corr_av_norm = (corr1/(double)((w_size*2+1)*(w_size*2+1))) / 65025.0; // (255*255 = 65025)
                    if(corr_av_norm > thre_corr){ continue; }

                    wsp::math::FindQuadraticPeak(
                        -1, 0, 1, 
                        corr0, corr1, corr2,
                        peak_x, peak_corr
                    );

                    io_corr[i] = peak_corr;
                    io_depth[i*2] = disp_x - 1 + peak_x;
                    io_depth[i*2+1] = disp_y;
                    io_state[i] = WSP_PIXEL_STATE_NORMAL;
                }
            }
        }
    }
    #endif
    if(tmp_corr_sum==NULL){
        free(corr_sum_map);
    }
}



wsp::State wsp::img::StereoBMFunctionFitting(
        wsp::Image<double> &o_depth, 
        const wsp::Image<double> &in_main_img, 
        const wsp::Image<double> &in_sub_img, 
        const wsp::img::StereoBMParams &params, 
        const wsp::Image<double> *in_depth_ref,
        const wsp::Image<u8> *in_mask
){
    int disp_size_x, disp_size_y, disp_size;
    wsp::Image<double> win_size_map;
    wsp::Image<double> corr_tmp_map[3]; // 1-2:save old corr for parabola, 0: save min corr
    wsp::Image<double> corr_map, corr_min_map[3];

    int width = in_main_img.width();
    int height = in_main_img.height();
    int length = width*height;
    int num_channels = in_main_img.num_of_channels();
    int num_elem = width*height*num_channels;

    if(width!=in_sub_img.width() || height!=in_sub_img.height()){
        WSP_COMMON_ERROR_LOG("The size of main and sub image are different: %d!=%d or %d!=%d\n", 
                    width, in_sub_img.width(), height, in_sub_img.height()); 
        return WSP_STATE_INVALID_PARAMETER;
    }

    // Initialize maps
    o_depth.SetSize(width, height, 2);
    corr_tmp_map[0].SetSize(width, height);
    corr_tmp_map[1].SetSize(width, height);
    corr_tmp_map[2].SetSize(width, height);
    corr_min_map[0].SetSize(width, height);
    corr_min_map[1].SetSize(width, height);
    corr_min_map[2].SetSize(width, height);
    corr_map.SetSize(width, height);

    // initialize data
    memset(o_depth.data(), 0, sizeof(double)*length*2);
    memset(o_depth.state_data(), WSP_PIXEL_STATE_OCCLUDED, length);
    corr_tmp_map[0].Reset(DBL_MAX);
    corr_tmp_map[1].Reset(DBL_MAX);
    corr_tmp_map[2].Reset(DBL_MAX);
    corr_min_map[0].Reset(DBL_MAX);
    corr_min_map[1].Reset(DBL_MAX);
    corr_min_map[2].Reset(DBL_MAX);
    corr_map.Reset(DBL_MAX);
    

    disp_size_x = params.dispx_max - params.dispx_min+1;
    disp_size_y = params.dispy_max - params.dispy_min+1;
    disp_size = disp_size_x*disp_size_y;

    if(params.show_debug_info){
        printf("Stereo Block Matching:\n"
               "    block size: %d %d \n"
               "    disp range: x(%d, %d), y(%d, %d) \n"
               "    thre_std_deviation: %f\n"
               "    thre_corr: %f\n" 
               "    scale_thre_enough_edge_amount: %f\n",
                params.block_min, params.block_max,
                params.dispx_min, params.dispx_max, 
                params.dispy_min, params.dispy_max, 
                params.thre_std_deviation, 
                params.thre_corr,
                params.scale_thre_enough_edge_amount
        );
    }

    // Get adaptive window size for each pixels
    if(params.block_min<params.block_max){
        wsp::Image<double> edgeImg;
        wsp::State state=wsp::img::LaplacianFilter(edgeImg, in_main_img);
        if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Applying Laplacian Filter failed\n"); return state; }
        edgeImg.Abs();
        wsp::img::AverageFilter<double>(edgeImg);
        if(params.save_debug_data){
            edgeImg.SaveImage("edgeImage.tif");
        }
        double enough_edge_amount = wsp::GetMax(edgeImg.data(), edgeImg.width()*edgeImg.height()) * (width/4) * params.scale_thre_enough_edge_amount;

        GetAdaptiveWindowSize(
            win_size_map, in_main_img, edgeImg,
            params.block_min, params.block_max, 
            enough_edge_amount,
            params.save_debug_data
        );
    }
    if(params.save_debug_data){
        win_size_map.SaveImage("win_size_map.tif");
    }
    //win_size_map.SaveImage("win_size_map.tif");

    // mask the none-charactaristic area
    //wsp::img::GetCharacteristicMaskByVariance(corr_map.state_data(), (uchar*)in_main_img.data(), win_size_map.data(),
    //    width, height, in_main_img.num_of_channels(), params.thre_std_deviation);
    if(params.thre_std_deviation!=0)
    {
        printf("Calculating characteristic mask...\r");
        wsp::img::GetCharacteristicMaskByDeviation(
            corr_map.state_data(),
            in_main_img.data(), 
            width, height, num_channels, 
            params.block_min,
            params.thre_std_deviation,
            NULL //win_size_map.data()
        );
    }
    if(params.save_debug_data){
        wsp::Image<double> tmp_img = corr_map[0];
        tmp_img.SaveState("stereo_mask.tif", WSP_PIXEL_STATE_MASKED);
    }

    double *win_map_ptr = NULL;
    //if(params.block_min<params.block_max){
    //    win_map_ptr = win_size_map.data();
    //}

    wsp::math::FittingFunction using_fit_func;
    if(params.subpixel_fitting_function==WSP_FITTING_FUNCTION_UNDEFINED){
        using_fit_func=wsp::math::DecideFittingFuncByCorrFunc(params.eval_func);
    }

    // +/- 1 because subpixel estimation needs at least 3 points for calculation of peak.
    int dispx_max = params.dispx_max + 1;

    // Search disparity range to find a pixel which has minimum correlation value
    #ifdef USE_QT
    QProgressDialog *progress=NULL;
    if(params.show_progress){
        progress=new QProgressDialog("Correlation Stereo Matching...", "Cancel", 0, disp_size);
        progress->setWindowModality(Qt::WindowModal);
    }
    #endif
    //#ifdef _OPENMP
    //#pragma omp parallel
    //#endif
    {
        int dx, dy, d; // iterators
        double *tmp_corr_sum = (double*)malloc(sizeof(double)*length);

        //#ifdef _OPENMP
        //#pragma omp for
        //#endif
        for(dx=params.dispx_min-1; dx<=dispx_max; ++dx){
            for(dy=params.dispy_min; dy<=params.dispy_max; ++dy){
                //printf("%d\n", omp_get_thread_num());
                // display progress information ------------------------
                if(params.show_progress){
                    d = (dx-params.dispx_min)*disp_size_y + (dy-params.dispy_min);
                    #ifdef USE_QT
                    if(omp_get_thread_num()==0){
                        progress->setValue(d*omp_get_max_threads());
                        //if(progress->wasCanceled()){ return WSP_STATE_CANCELED; }
                        qApp->processEvents();
                    }
                    #else
                    printf("%3f %% done..\r", 100*d/static_cast<float>(disp_size));
                    #endif
                }
                // ------------------------ end display progress information

                // computing disparities
                //printf("%d\n", dx);
                //wsp::img::UpdateCorrelationMap(
                //    o_depth, 
                //    corr_map, 
                //    in_main_img, 
                //    in_sub_img, 
                //    win_size_map, 
                //    corr_map.state_data(), 
                //    dx, dy, 
                //    params.thre_corr, 
                //    params.eval_func
                //);
                double *corr_tmp_data[3] = {corr_tmp_map[0].data(), corr_tmp_map[1].data(), corr_tmp_map[2].data()};
                double *corr_min_data[3] = {corr_min_map[0].data(), corr_min_map[1].data(), corr_min_map[2].data()};
                ::UpdateCorrelationMap_FunctionFitting(
                    in_main_img.data(), 
                    in_sub_img.data(), 
                    corr_map.state_data(), 
                    o_depth.data(), 
                    o_depth.state_data(),
                    corr_map.data(), 
                    corr_min_data,
                    corr_tmp_data,
                    width, height, num_channels,
                    dx, dy, 
                    params.block_min,
                    params.thre_corr, 
                    params.eval_func,
                    using_fit_func,
                    tmp_corr_sum,
                    win_map_ptr
                );
            }
        }
        free(tmp_corr_sum);
    }

    #ifdef USE_QT
    delete progress;
    #endif

    //printf("\n");
    // DEBUG ---------------------------
    //double min, max;
    //wsp::GetMinMax(o_depth.data(), min, max, o_depth.length()*o_depth.num_of_channels());
    //printf("DEBUG: disparity min=%f, max=%f\n", min, max);
    // -------------------------------


    return WSP_STATE_SUCCESS;
}



wsp::State StereoBM_HighMemoryUsage(
    wsp::Image<double> &o_depth, 
    const wsp::Image<double> &in_main_img, 
    const wsp::Image<double> &in_sub_img, 
    const wsp::img::StereoBMParams &params, 
    const wsp::Image<double> *in_depth_ref
){
    int disp_size_x, disp_size_y, disp_size;
    wsp::Image<double> win_size_map, corr_map;

    int width = in_main_img.width();
    int height = in_main_img.height();
    int length = width*height;
    int num_channels = in_main_img.num_of_channels();
    int num_elem = width*height*num_channels;

    if(width!=in_sub_img.width() || height!=in_sub_img.height()){
        WSP_COMMON_ERROR_LOG("The size of main and sub image are different: %d!=%d or %d!=%d\n", 
                    width, in_sub_img.width(), height, in_sub_img.height()); 
        return WSP_STATE_FAILURE;
    }

    // Initialize maps
    o_depth.SetSize(width, height, 2);
    corr_map.SetSize(width, height);

    // initialize data
    memset(o_depth.data(), 0, sizeof(double)*length*2);
    memset(o_depth.state_data(), WSP_PIXEL_STATE_OCCLUDED, length);
    corr_map.Reset(DBL_MAX);

    disp_size_x = params.dispx_max - params.dispx_min+1;
    disp_size_y = params.dispy_max - params.dispy_min+1;
    disp_size = disp_size_x*disp_size_y;

    if(params.show_debug_info){
        printf("Stereo Block Matching:\n"
               "    block size: %d %d \n"
               "    disp range: x(%d, %d), y(%d, %d) \n"
               "    thre_std_deviation: %f\n"
               "    thre_corr: %f\n"
               "    scale_thre_enough_edge_amount: %f\n",
                params.block_min, params.block_max,
                params.dispx_min, params.dispx_max, 
                params.dispy_min, params.dispy_max, 
                params.thre_std_deviation, 
                params.thre_corr,
                params.scale_thre_enough_edge_amount
        );
    }
    // Get adaptive window size for each pixels
    if(params.block_min<params.block_max){
        wsp::Image<double> edgeImg;
        wsp::State state=wsp::img::LaplacianFilter(edgeImg, in_main_img);
        if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Applying Laplacian Filter failed\n"); return state; }
        edgeImg.Abs();
        wsp::img::AverageFilter<double>(edgeImg);
        if(params.save_debug_data){
            edgeImg.SaveImage("edgeImage.tif");
        }
        double enough_edge_amount = wsp::GetMax(edgeImg.data(), edgeImg.width()*edgeImg.height()) * (width/4) * params.scale_thre_enough_edge_amount;

        GetAdaptiveWindowSize(
            win_size_map, in_main_img, edgeImg,
            params.block_min, params.block_max, 
            enough_edge_amount,
            params.save_debug_data
        );
    }
    if(params.save_debug_data){
        win_size_map.SaveImage("win_size_map.tif");
    }
    //win_size_map.SaveImage("win_size_map.tif");

    // mask the none-charactaristic area
    //wsp::img::GetCharacteristicMaskByVariance(corr_map.state_data(), (uchar*)in_main_img.data(), win_size_map.data(),
    //    width, height, in_main_img.num_of_channels(), params.thre_std_deviation);
    if(params.thre_std_deviation!=0){
        printf("Calculating characteristic mask...\r");
        wsp::img::GetCharacteristicMaskByDeviation(
            corr_map.state_data(),
            in_main_img.data(), 
            width, height, num_channels, 
            params.block_min,
            params.thre_std_deviation,
            NULL //win_size_map.data()
        );
    }
    if(params.save_debug_data){
        wsp::Image<double> tmp_img = corr_map;
        tmp_img.SaveState("stereo_mask.tif", WSP_PIXEL_STATE_MASKED);
    }

    double *win_map_ptr = NULL;
    //if(params.block_min<params.block_max){
    //    win_map_ptr = win_size_map.data();
    //}

    // Search disparity range to find a pixel which has minimum correlation value
    #ifdef USE_QT
    QProgressDialog *progress=NULL;
    if(params.show_progress){
        progress=new QProgressDialog("Correlation Stereo Matching...", "Cancel", 0, disp_size);
        progress->setWindowModality(Qt::WindowModal);
    }
    #endif
    #ifdef _OPENMP
    #pragma omp parallel
    #endif
    {
        int dx, dy, d; // iterators
        double *tmp_corr_sum = (double*)malloc(sizeof(double)*length);
        #ifdef _OPENMP
        #pragma omp for
        #endif
        for(dx=params.dispx_min; dx<=params.dispx_max; ++dx){
            for(dy=params.dispy_min; dy<=params.dispy_max; ++dy){
                //printf("%d\n", omp_get_thread_num());
                // display progress information ------------------------
                if(params.show_progress){
                    d = (dx-params.dispx_min)*disp_size_y + (dy-params.dispy_min);
                    #ifdef USE_QT
                    if(omp_get_thread_num()==0){
                        progress->setValue(d*omp_get_max_threads());
                        //if(progress->wasCanceled()){ return WSP_STATE_CANCELED; }
                        qApp->processEvents();
                    }
                    #else
                    printf("%3f %% done..\r", 100*d/static_cast<float>(disp_size));
                    #endif
                }
                // ------------------------ end display progress information

                // computing disparities
                //printf("%d\n", dx);
                //wsp::img::UpdateCorrelationMap(
                //    o_depth, 
                //    corr_map, 
                //    in_main_img, 
                //    in_sub_img, 
                //    win_size_map, 
                //    corr_map.state_data(), 
                //    dx, dy, 
                //    params.thre_corr, 
                //    params.eval_func
                //);
                ::UpdateCorrelationMap(
                    in_main_img.data(), 
                    in_sub_img.data(), 
                    corr_map.state_data(), 
                    o_depth.data(), 
                    o_depth.state_data(),
                    corr_map.data(), 
                    width, height, num_channels,
                    dx, dy, 
                    params.block_min,
                    params.thre_corr, 
                    params.default_corr,
                    params.eval_func,
                    tmp_corr_sum,
                    win_map_ptr
                );
            }
        }
        free(tmp_corr_sum);
    }
    #ifdef USE_QT
    if(progress->wasCanceled()){ delete progress; return WSP_STATE_CANCELED; }
    delete progress;
    #endif

    //printf("\n");
    // DEBUG ---------------------------
    //double min, max;
    //wsp::GetMinMax(o_depth.data(), min, max, o_depth.length()*o_depth.num_of_channels());
    //printf("DEBUG: disparity min=%f, max=%f\n", min, max);
    // -------------------------------


    return WSP_STATE_SUCCESS;
}



wsp::State wsp::img::StereoFastBM_core(
    wsp::Image<double> &o_depth, 
    const wsp::Image<double> &in_main_img, 
    const wsp::Image<double> &in_sub_img, 
    const wsp::img::StereoBMParams &params, 
    const wsp::Image<double> *in_depth_ref
){
    int disp_size_x, disp_size_y, disp_size;
    wsp::Image<double> win_size_map, corr_map;

    int width = in_main_img.width();
    int height = in_main_img.height();
    int length = width*height;
    int num_channels = in_main_img.num_of_channels();
    int num_elem = width*height*num_channels;

    if(width!=in_sub_img.width() || height!=in_sub_img.height()){
        WSP_COMMON_ERROR_LOG("The size of main and sub image are different: %d!=%d or %d!=%d\n", 
                    width, in_sub_img.width(), height, in_sub_img.height()); 
        return WSP_STATE_FAILURE;
    }

    // Initialize maps
    o_depth.SetSize(width, height, 2);
    corr_map.SetSize(width, height);

    // initialize data
    memset(o_depth.data(), 0, sizeof(double)*length*2);
    memset(o_depth.state_data(), WSP_PIXEL_STATE_OCCLUDED, length);
    corr_map.Reset(DBL_MAX);

    disp_size_x = params.dispx_max - params.dispx_min+1;
    disp_size_y = params.dispy_max - params.dispy_min+1;
    disp_size = disp_size_x*disp_size_y;

    if(params.show_debug_info){
        printf("Stereo Block Matching:\n"
               "    block size: %d %d \n"
               "    disp range: x(%d, %d), y(%d, %d) \n"
               "    thre_std_deviation: %f\n"
               "    thre_corr: %f\n"
               "    scale_thre_enough_edge_amount: %f\n",
                params.block_min, params.block_max,
                params.dispx_min, params.dispx_max, 
                params.dispy_min, params.dispy_max, 
                params.thre_std_deviation, 
                params.thre_corr,
                params.scale_thre_enough_edge_amount
        );
    }
    // Get adaptive window size for each pixels
    if(params.block_min<params.block_max)
    {
        wsp::Image<double> edgeImg;
        wsp::State state=wsp::img::LaplacianFilter(edgeImg, in_main_img);
        if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Applying Laplacian Filter failed\n"); return state; }
        edgeImg.Abs();
        wsp::img::AverageFilter<double>(edgeImg);
        if(params.save_debug_data){
            edgeImg.SaveImage("edgeImage.tif");
        }
        double enough_edge_amount = wsp::GetMax(edgeImg.data(), edgeImg.width()*edgeImg.height()) * (width/4) * params.scale_thre_enough_edge_amount;

        GetAdaptiveWindowSize(
            win_size_map, in_main_img, edgeImg,
            params.block_min, params.block_max, 
            enough_edge_amount,
            params.save_debug_data
        );
    }
    if(params.save_debug_data){
        win_size_map.SaveImage("win_size_map.tif");
    }
    //win_size_map.SaveImage("win_size_map.tif");

    // mask the none-charactaristic area
    //wsp::img::GetCharacteristicMaskByVariance(corr_map.state_data(), (uchar*)in_main_img.data(), win_size_map.data(),
    //    width, height, in_main_img.num_of_channels(), params.thre_std_deviation);
    if(params.thre_std_deviation!=0){
        printf("Calculating characteristic mask...\r");
        wsp::img::GetCharacteristicMaskByDeviation(
            corr_map.state_data(),
            in_main_img.data(), 
            width, height, num_channels, 
            params.block_min,
            params.thre_std_deviation,
            NULL //win_size_map.data()
        );
    }
    if(params.save_debug_data){
        wsp::Image<double> tmp_img = corr_map;
        tmp_img.SaveState("stereo_mask.tif", WSP_PIXEL_STATE_MASKED);
    }

    double *win_map_ptr = NULL;
    //if(params.block_min<params.block_max){
    //    win_map_ptr = win_size_map.data();
    //}

    // Search disparity range to find a pixel which has minimum correlation value
    #ifdef USE_QT
    QProgressDialog *progress=NULL;
    if(params.show_progress){
        progress=new QProgressDialog("Correlation Stereo Matching...", "Cancel", 0, disp_size);
        progress->setWindowModality(Qt::WindowModal);
    }
    #endif
    #ifdef _OPENMP
    #pragma omp parallel
    #endif
    {
        int dx, dy, d; // iterators
        double *tmp_corr_sum = (double*)malloc(sizeof(double)*length);
        #ifdef _OPENMP
        #pragma omp for
        #endif
        for(dx=params.dispx_min; dx<=params.dispx_max; ++dx){
            for(dy=params.dispy_min; dy<=params.dispy_max; ++dy){
                //printf("%d\n", omp_get_thread_num());
                // display progress information ------------------------
                if(params.show_progress){
                    d = (dx-params.dispx_min)*disp_size_y + (dy-params.dispy_min);
                    #ifdef USE_QT
                    if(omp_get_thread_num()==0){
                        progress->setValue(d*omp_get_max_threads());
                        //if(progress->wasCanceled()){ return WSP_STATE_CANCELED; }
                        qApp->processEvents();
                    }
                    #else
                    printf("%3f %% done..\r", 100*d/static_cast<float>(disp_size));
                    #endif
                }
                // ------------------------ end display progress information

                // computing disparities
                //printf("%d\n", dx);
                //wsp::img::UpdateCorrelationMap(
                //    o_depth, 
                //    corr_map, 
                //    in_main_img, 
                //    in_sub_img, 
                //    win_size_map, 
                //    corr_map.state_data(), 
                //    dx, dy, 
                //    params.thre_corr, 
                //    params.eval_func
                //);
                ::UpdateCorrelationMap(
                    in_main_img.data(), 
                    in_sub_img.data(), 
                    corr_map.state_data(), 
                    o_depth.data(), 
                    o_depth.state_data(),
                    corr_map.data(), 
                    width, height, num_channels,
                    dx, dy, 
                    params.block_min,
                    params.thre_corr, 
                    params.default_corr,
                    params.eval_func,
                    tmp_corr_sum,
                    win_map_ptr
                );
            }
        }
        free(tmp_corr_sum);
    }
    #ifdef USE_QT
    delete progress;
    #endif

    //printf("\n");
    // DEBUG ---------------------------
    //double min, max;
    //wsp::GetMinMax(o_depth.data(), min, max, o_depth.length()*o_depth.num_of_channels());
    //printf("DEBUG: disparity min=%f, max=%f\n", min, max);
    // -------------------------------


    return WSP_STATE_SUCCESS;
}

//! this is a wrapper function for StereoAdaptiveBM_main, handling coarse to fine approach and cross checking
wsp::State wsp::img::StereoBM(
    wsp::Image<double> &o_depth, 
    const wsp::Image<uchar> &in_main_img, 
    const wsp::Image<uchar> &in_sub_img, 
    const wsp::img::StereoBMParams &in_params
){
    wsp::State wstat;
    wsp::Image<double> tmp_main, tmp_sub;
    wsp::Image<double> tmp_depth_main, tmp_depth_sub;
    wsp::img::StereoBMParams params(in_params);

    int sw, sh;
    int w = in_main_img.width();
    int h = in_main_img.height();
    int nc = in_main_img.num_of_channels();
    int nelem = w*h*nc;

    sw = w*params.prescaling_factor;
    sh = h*params.prescaling_factor;

    if(params.presubtract_average){
        tmp_main.SetSize(w, h, nc);
        tmp_sub.SetSize(w, h, nc);
        for(int c=0; c<nc; ++c){
            double average_main = in_main_img.GetAverage(c);
            double average_sub = in_sub_img.GetAverage(c);
            const uchar *src_main_ptr = in_main_img.data() + c;
            const uchar *src_sub_ptr = in_sub_img.data() + c;
            const uchar *end_ptr = src_main_ptr + nelem;
            double *dst_main_ptr = tmp_main.data() + c;
            double *dst_sub_ptr = tmp_sub.data() + c;
            for(; src_main_ptr!=end_ptr; 
                    src_main_ptr+=nc, src_sub_ptr+=nc,
                    dst_main_ptr+=nc, dst_sub_ptr+=nc
            ){
                *dst_main_ptr = static_cast<double>(*src_main_ptr) - average_main;
                *dst_sub_ptr = static_cast<double>(*src_sub_ptr) - average_sub;
            }
        }
        if(params.save_debug_data){
            tmp_main.SaveImage("debug_StereoBM_subtractImg_main.png");
            tmp_sub.SaveImage("debug_StereoBM_subtractImg_sub.png");
        }
    }else{
        tmp_main.CopyFrom(in_main_img);
        tmp_sub.CopyFrom(in_sub_img);
    }

    if(params.prescaling_factor!=1.0){
        wsp::Image<double> tmp_scale_main, tmp_scale_sub;
        tmp_scale_main.SetSize(sw, sh, nc);
        tmp_scale_sub.SetSize(sw, sh, nc);

        wsp::img::ScaleImage(tmp_main.data(), w, h, nc, tmp_scale_main.data(), sw, sh, WSP_INTERPOLATION_BILINEAR);
        wsp::img::ScaleImage(tmp_sub.data(), w, h, nc, tmp_scale_sub.data(), sw, sh, WSP_INTERPOLATION_BILINEAR);
        tmp_main.CopyFrom(tmp_scale_main);
        tmp_sub.CopyFrom(tmp_scale_sub);
        params.dispx_min *= params.prescaling_factor;
        params.dispx_max *= params.prescaling_factor;
        params.dispy_min *= params.prescaling_factor;
        params.dispy_max *= params.prescaling_factor;
    }



    int level;
    int div;
    // handling coarse to fine approach
    //for(level=params.coarse_to_fine_depth; level>0; level--){
    //    tmp_main = in_main_img;
    //    tmp_sub = in_sub_img;
    //    div = (u32)pow(2, static_cast<double>(level-1));
    //    wsp::img::cvResizeImageLinear(tmp_main, tmp_main.width()/div, tmp_main.height()/div);
    //    wsp::img::cvResizeImageLinear(tmp_sub, tmp_sub.width()/div, tmp_sub.height()/div);

    //    wstat = StereoFastBM_core(o_depth, in_main_img, in_sub_img, params, level==params.coarse_to_fine_depth? NULL: &tmp_depth);
    //    if(wstat!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("StereoBM failed\n"); return wstat; }

    //    tmp_depth = o_depth;
    //}
    if(params.subpixel_estimation){
        wstat = wsp::img::StereoBMFunctionFitting(tmp_depth_main, tmp_main, tmp_sub, params);
    }else{
        wstat = wsp::img::StereoFastBM_core(tmp_depth_main, tmp_main, tmp_sub, params);
    }
    if(wstat!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("StereoBM failed\n"); return wstat; }

    // handling cross checking
    wsp::Image<double> depth_sub;
    if(params.cross_check==true){
        //printf("Applying cross check...\r");
        wsp::img::StereoBMParams tmp_params(params);
        // swap min and max --------
        int tmp = params.dispx_min;
        tmp_params.dispx_min = -params.dispx_max;
        tmp_params.dispx_max = -tmp;
        tmp = params.dispy_min;
        tmp_params.dispy_min = -params.dispy_max;
        tmp_params.dispy_max = -tmp;
        // ---------------------
        if(params.subpixel_estimation){
            wstat = wsp::img::StereoBMFunctionFitting(tmp_depth_sub, tmp_sub, tmp_main, tmp_params);
        }else{
            wstat = wsp::img::StereoFastBM_core(tmp_depth_sub, tmp_sub, tmp_main, tmp_params, NULL);
        }
        if(wstat!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("StereoBM failed\n"); return wstat; }

        // save temprary image(only x-axis) -----
        if(params.save_debug_data){
            wsp::Image<double> save_disp_main, save_disp_sub;
            save_disp_main = tmp_depth_main;
            save_disp_sub = tmp_depth_sub;
            save_disp_main.Normalize(0, 255);
            save_disp_main.SetSize(save_disp_main.width(), save_disp_main.height(), 1);
            save_disp_main.SetSize(save_disp_main.width(), save_disp_main.height(), 3);
            save_disp_main.SaveImage("tmp_bm_disp_main.tif", WSP_PIXEL_STATE_OCCLUDED);

            save_disp_sub.Normalize(0, 255);
            save_disp_sub.SetSize(save_disp_sub.width(), save_disp_sub.height(), 1);
            save_disp_sub.SetSize(save_disp_sub.width(), save_disp_sub.height(), 3);
            save_disp_sub.SaveImage("tmp_bm_disp_sub.tif", WSP_PIXEL_STATE_OCCLUDED);
        }
        // ---------------
        wsp::img::CrossCheck(tmp_depth_main, tmp_depth_sub, params.cross_check_fuzziness);
        //o_depth.SaveImage("tmp_test.png");
    }

    if(params.prescaling_factor!=1.0){
        o_depth.SetSize(w, h, 2);
        wsp::img::ScaleImage(tmp_depth_main.data(), sw, sh, 2, o_depth.data(), w, h, WSP_INTERPOLATION_NEAREST_NEIGHBOR);
        wsp::img::ScaleImage(tmp_depth_main.state_data(), sw, sh, 1, o_depth.state_data(), w, h, WSP_INTERPOLATION_NEAREST_NEIGHBOR);
    }else{
        o_depth.CopyFrom(tmp_depth_main);
    }


    return WSP_STATE_SUCCESS;
}

// =========================================================================================
// =========================================================================================
//**
//                       Phase-Only Correlation Stereo Matching
//**
// =========================================================================================
// =========================================================================================

//
//wsp::State wsp::img::GetAdaptiveWindowSize_POC1DPerLine(wsp::Image<double> &o_winSize, const wsp::Image<WspRgb> &in_img, 
//                                                              int winH_min, int winH_max, int winWidth)
//{
//    int itr_x, itr_y, itr_w, itr_h, winH; // iterators
//    int width, height;
//    int winH_2, h2_size, adaptive_size;
//    int winW_2 = winWidth/2 - 1;
//    double edge_sum, edge_av, edge_av_max;
//    double enoughEdgeAmount;
//    wsp::Image<double> edgeImg, edgeSum;
//    wsp::State state;
//
//    width = (int)in_img.width();
//    height = (int)in_img.height();
//    o_winSize.SetSize((u32)width, (u32)height);
//
//    // Get edge image ---------------
//    state=wsp::img::LaplacianFilter(edgeImg, in_img);
//    if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Applying Laplacian Filter failed\n"); return state; }
//    edgeImg.Abs();
//    wsp::img::AverageFilter<double>(edgeImg);
//    //edgeImg.SaveAsTiff("edge.tif");
//    state=wsp::img::GetHorSum(edgeSum, edgeImg);
//    if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Getting hor sum failed\n"); return state; }
//    // --------------- end Get edge image
//
//    enoughEdgeAmount = wsp::GetMax(edgeImg.data(), edgeImg.width()*edgeImg.height()) * (width/100);
//    printf("enough edge amount = %f\n", enoughEdgeAmount);
//
//    // main loop -------------------------------
//    #ifdef USE_QT
//    QProgressDialog progress("Correlation Stereo Matching...", "Cancel", 0, height);
//    progress.setWindowModality(Qt::WindowModal);
//    #endif
//    for(itr_y=0; itr_y<height; itr_y++){
//        for(itr_x=0; itr_x<width; itr_x++){
//
//            // Ignore around edge of image
//            if(itr_x-winWidth/2<0 || itr_x+winWidth/2>=width){ continue; }
//
//            adaptive_size = winH_min;
//            edge_av_max = 0;
//            for(winH=winH_min; winH<=winH_max; winH++)
//            {
//                winH_2 = winH/2 - 1;
//                if( itr_y-winH_2<0 || itr_y+winH_2>=height){ break; }
//
//                for(itr_h=-winH; itr_h<=winH; itr_h++){
//                    edge_sum = (edgeSum.GetElem(itr_x-winW_2, itr_y+itr_h) - edgeSum.GetElem(itr_x+winW_2, itr_y+itr_h));
//                    if(edge_sum>enoughEdgeAmount){
//                        adaptive_size = winH;
//                        goto SET_WIN_SIZE;
//                    }
//                }
//            }
//            SET_WIN_SIZE:
//            //cout<<adaptive_size<<endl;
//            o_winSize.SetElem((double)adaptive_size, itr_x, itr_y);
//        }
//    }
//    #ifdef USE_QT
//    progress.setValue(height);
//    #endif
//    return WSP_STATE_SUCCESS;
//}









wsp::State wsp::img::StereoPOC(
    const wsp::Image<double> &in_main, 
    const wsp::Image<double> &in_sub, 
    wsp::Image<double> &o_depth, 
    const wsp::img::StereoPOCParams &params, 
    const wsp::Image<uchar> *in_mask_main, 
    const wsp::Image<uchar> *in_mask_sub
){
    int w = in_main.width(), h = in_main.height(), nc = in_main.num_of_channels();
    wsp::State state;
    o_depth.SetSize(w, h, 2);
    const uchar *mask_ptr = NULL;
    if(in_mask_main!=NULL){
        mask_ptr = in_mask_main->data();
    }
    switch(params.algorithm)
    {
    case WSP_STEREO_HIERARCHICAL_POC:
        state = wsp::img::StereoHierarchicalPOC(
            in_main.data(),
            in_sub.data(),
            mask_ptr,
            o_depth.data(), 
            o_depth.state_data(),
            w, h, nc, params
        );
        break;
    case WSP_STEREO_BM_BASED_FRONT_ONLY_POC:
    case WSP_STEREO_BM_BASED_POC:
        state = wsp::img::StereoBMBasedPOC(
            in_main.data(),
            in_sub.data(),
            mask_ptr,
            o_depth.data(), 
            o_depth.state_data(),
            w, h, nc, params
        );
        break;
    default:
        WSP_COMMON_ERROR_LOG("unknown algorithm\n");
        return WSP_STATE_INVALID_PARAMETER;
    }
    if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("StereoPOC_main failed\n"); return state; }

    // handling cross checking
    if(params.cross_check==true){
        wsp::Image<double> depth_sub(w, h, 2);
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

        //state = wsp::img::StereoPOC_main(depth_sub, in_sub, in_main, tmp_params, in_mask_sub);
        if(in_mask_main!=NULL){
            mask_ptr = in_mask_sub->data();
        }
        switch(params.algorithm){
        case WSP_STEREO_HIERARCHICAL_POC:
            state = wsp::img::StereoBMBasedPOC(
                in_sub.data(),
                in_main.data(),
                mask_ptr,
                depth_sub.data(), 
                depth_sub.state_data(),
                w, h, nc, params
            );
            break;
        case WSP_STEREO_BM_BASED_FRONT_ONLY_POC:
        case WSP_STEREO_BM_BASED_POC:
            state = wsp::img::StereoHierarchicalPOC(
                in_sub.data(),
                in_main.data(),
                mask_ptr,
                depth_sub.data(), 
                depth_sub.state_data(),
                w, h, nc, params
            );
            break;
        default:
            WSP_COMMON_ERROR_LOG("unknown algorithm\n");
            return WSP_STATE_INVALID_PARAMETER;
        }
        if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("StereoPOC_main failed\n"); return state; }

        // save temprary image(only x-axis) -----
        if(params.save_debug_data){
            wsp::Image<double> tmp_main, tmp_sub;
            tmp_main = o_depth;
            tmp_sub = depth_sub;
            tmp_main.Normalize(0, 255, 200);
            tmp_main.SetSize(tmp_main.width(), tmp_main.height(), 1);
            tmp_main.SetSize(tmp_main.width(), tmp_main.height(), 3);
            tmp_main.SaveImage("tmp_poc_disp_main.tif", WSP_PIXEL_STATE_OCCLUDED);

            tmp_sub.Normalize(0, 255, 200);
            tmp_sub.SetSize(tmp_sub.width(), tmp_sub.height(), 1);
            tmp_sub.SetSize(tmp_sub.width(), tmp_sub.height(), 3);
            tmp_sub.SaveImage("tmp_poc_disp_sub.tif", WSP_PIXEL_STATE_OCCLUDED);
        }
        // ---------------

        wsp::img::CrossCheck(o_depth, depth_sub, tmp_params.cross_check_fuzziness);
    }
    return state;
}




//! \ingroup WspImage
//! \brief Stereo macthing based on Phase-Only Correlation.
/*!
    in_main: main color image, the array size must be width*height*num_channels
    in_sub: sub color image, the array size must be width*height*num_channels
    in_mask: this provides skipping matching process, the array size must be width*height.
    o_depth: size of this array must be width*height*2.
    o_occlusion_map: array which saves occlusion info.
*/
//wsp::State wsp::img::StereoPOC_main(
//        const double *in_main,
//        const double *in_sub,
//        const uchar *in_mask,
//        double *o_depth, 
//        uchar *o_occlusion_map,
//        int width, 
//        int height, 
//        int num_channels,
//        const wsp::img::StereoPOCParams &params
//){
//    //char filename[MAX_LENGTH_OF_NAME];
//    int num_pix = width*height;
//    uchar thre_mask = 200;
//
//    int blk_hw = params.block_width/2;
//    int blk_hh = params.block_height/2;
//    int blk_w = 2*blk_hw;
//    int blk_h = 2*blk_hh + 1;
//    int blk_len = blk_w * blk_h;
//    int blk_exp_x = wsp::math::GetExponentOfPowerOfTwo(blk_w);
//    int blk_exp_y = wsp::math::GetExponentOfPowerOfTwo(blk_h);
//
//    memset(o_depth, 0, sizeof(double)*num_pix*2);
//
//    printf("Stereo Matching Based on POC ==================\n");
//    printf("    width=%d, height=%d, numOfChannels=%d, \n", width, height, num_channels);
//    printf("    block width=%d, block height=%d\n", params.block_width, params.block_height);
//    printf("    search depth=%d, low pass ratio=%f\n", params.search_depth, params.low_pass_ratio);
//    printf("    poc peak=%f, thre std dev=%f\n", params.thre_poc_peak, params.thre_std_deviation);
//
//    //#ifdef USE_QT
//    //QProgressDialog progress("POC Stereo Matching...", "Cancel", 0, height);
//    //progress.setWindowModality(Qt::WindowModal);
//    //#endif
//
//    #ifdef _OPENMP
//    #pragma omp parallel
//    #endif
//    {
//        int thread_show_progress = omp_get_num_threads()-2;
//        if(thread_show_progress<0){ thread_show_progress=0; }
//        //int show_progress_offset_y = thread_show_progress*height/omp_get_num_threads();
//        int show_progress_offset_y = height/omp_get_num_threads();
//        float show_progress_mult = omp_get_num_threads()*100.0/(float)height;
//
//        double *d[2]; // temporary disparity for x and y
//        double disp[2], value, sum_sub, sum_main;
//        double mean_sd_main, mean_sd_sub;
//        int m, interval, level;
//        int x, y, i, j, k, c; // iterators
//        int px, py, index_main, index_sub, index_blk;
//        d[0] = (double*)malloc(sizeof(double)*(params.search_depth+1));
//        d[1] = (double*)malloc(sizeof(double)*(params.search_depth+1));
//        complex64 *main_blk = (complex64*)malloc(sizeof(complex64)*blk_len);
//        complex64 *sub_blk = (complex64*)malloc(sizeof(complex64)*blk_len);
//        complex64 *tmp_poc = (complex64*)malloc(sizeof(complex64)*blk_len);
//        complex64 *tmp_data = (complex64*)malloc(sizeof(complex64)*blk_len);
//        double *tmp_average_poc = (double*)malloc(sizeof(double)*blk_w);
//
//
//        #ifdef _OPENMP
//        #pragma omp for
//        #endif
//        for(y=0; y<height; ++y)
//        {
//            uchar *occ_ptr = o_occlusion_map + y*width;
//            const uchar *mask_ptr = in_mask + y*width;
//            double *dst_ptr = o_depth + y*width*2;
//            for(x=0; x<width; ++x, ++occ_ptr, ++mask_ptr, dst_ptr+=2){
//                if(in_mask!=NULL){
//                    if(*mask_ptr<thre_mask){
//                        *occ_ptr = WSP_PIXEL_STATE_OCCLUDED;
//                        continue;
//                    }
//                }
//                interval=pow(2.0, (int)params.search_depth-1);
//
//                // skip arround corner
//                if(    x-blk_hw*interval<0 
//                    || x+blk_hw*interval>=width
//                    || y-blk_hh*interval<0 
//                    || y+blk_hh*interval>=height
//                ){
//                    *occ_ptr = WSP_PIXEL_STATE_OCCLUDED;
//                    continue;
//                }
//
//                // input 0 because first position between two images is the same choordinate
//                d[0][0]=0.0;
//                d[1][0]=0.0;
//
//                memset(main_blk, 0, sizeof(complex64)*blk_len);
//                memset(sub_blk, 0, sizeof(complex64)*blk_len);
//                // hieralchal loop
//                for(level=0; level<(int)params.search_depth; interval/=2){
//                    // block loop for copying block pixels
//                    // evaluating RGB as sum of RGB
//                    for(i=-blk_hh; i<=blk_hh; i+=1){
//                        for(j=-blk_hw; j<=blk_hw-1; j+=1){
//                            px = x + j*interval; // global x for block
//                            py = y + i; // global y for block
//                            // local index for main block image
//                            index_main= (py*width+px)*num_channels;    
//                            // local index for sub block image
//                            index_sub = ((py-(int)floor(d[1][level]))*width+px-(int)floor(d[0][level])) * num_channels; 
//
//                            px = j+blk_hw; // Clamp minimum x to 0
//                            py = i+blk_hh; // Clamp minimum y to 0
//                            index_blk = py*blk_w + px; // calculate index within block
//
//                            // getting sum of rgb for double poc source
//                            sum_sub = 0.0;
//                            sum_main = 0.0;
//                            for(c=0; c<num_channels; c++){
//                                sum_sub += in_sub[index_sub+c];
//                                sum_main+= in_main[index_main+c];
//                            }
//
//                            // loop for getting data for pixels between the interval
//                            for(k=-interval/2+1; k<=interval/2; k++){
//                                for(c=0; c<num_channels; c++){
//                                    sum_sub += in_sub[index_sub+k*num_channels+c];
//                                    sum_main+= in_main[index_main+k*num_channels+c];
//                                }
//                            }
//
//                            sub_blk[index_blk][0]=sum_sub;
//                            main_blk[index_blk][0]=sum_main;
//                            //printf("%f\t%f\n", sum_main, sum_sub);
//                        }
//                    }
//                    ++level;
//
//                    // compute disparity
//                    if(params.use_2d_poc){
//                        wsp::img::FindDisparity2DPOC(
//                            sub_blk, 
//                            main_blk, 
//                            blk_w, blk_h,
//                            d[0][level], d[1][level],
//                            params.thre_poc_peak, 
//                            params.low_pass_ratio,
//                            blk_exp_x, blk_exp_y,
//                            tmp_poc, 
//                            tmp_data
//                        );
//                        //wsp::img::FindDisparity2DPOC(d[0][level], d[1][level], NULL, subBlk, mainBlk,
//                        //    //params.dispx_min, params.dispx_max, 
//                        //    params.thre_poc_peak, params.low_pass_ratio);
//                    }else{
//                        d[0][level] = wsp::img::FindDisparity1DPOC(
//                            sub_blk, 
//                            main_blk, 
//                            blk_w, blk_h,
//                            params.thre_poc_peak, 
//                            params.low_pass_ratio,
//                            blk_exp_x,
//                            tmp_poc, 
//                            tmp_average_poc
//                        );
//                        //d[0][level] = wsp::img::FindDisparity1DPOC(subBlk.data(), mainBlk.data(), mainBlk.width(), mainBlk.height(), params.dispx_min, params.dispx_max, params.thre_poc_peak, params.low_pass_ratio);
//                    }
//                    //printf("%f\n", d[level]);
//
//                    // if poc reliability is low than stop calculation
//                    if(d[0][level]==DBL_MAX){ break; }
//
//                    d[0][level] = level<params.search_depth? floor(d[0][level]) : d[0][level];
//                    d[1][level] = level<params.search_depth? floor(d[1][level]) : d[1][level];
//                }
//                if(d[0][level]==DBL_MAX){
//                    *occ_ptr = WSP_PIXEL_STATE_OCCLUDED;
//                    continue;
//                }
//                disp[0]=0.0;
//                disp[1]=0.0;
//                for(i=1; i<=params.search_depth; i++){
//                    disp[0] += d[0][i];
//                    disp[1] += d[1][i];
//                }
//                *dst_ptr = disp[0];
//                *(dst_ptr+1) = disp[1];
//            }
//            //#ifdef USE_QT
//            //progress.setValue(y);
//            //if(progress.wasCanceled()){ return WSP_STATE_CANCELED; }
//            //#else
//            #ifdef _OPENMP
//            if(omp_get_thread_num()==thread_show_progress){
//                //double percentage = omp_get_num_threads()*y*100.0/(double)height;
//                //printf("%d*%d*100/%d = %f %% done..\r", omp_get_num_threads(), y, height, percentage);
//                printf("progress of thead %d: %f %% done..\r", thread_show_progress, show_progress_mult*(y-show_progress_offset_y));
//            }
//            //printf("progress of thead %d: %f %% done..\r", omp_get_thread_num(), show_progress_mult*(y-omp_get_thread_num()*show_progress_offset_y));
//            #endif
//            //#endif
//        }
//        free(d[0]);
//        free(d[1]);
//        free(main_blk);
//        free(sub_blk);
//        free(tmp_poc);
//        free(tmp_data);
//        free(tmp_average_poc);
//    }
//    //#ifdef USE_QT
//    //progress.setValue(height);
//    //#endif
//    return WSP_STATE_SUCCESS;
//}

wsp::State wsp::img::StereoPOC_core(
        const double *in_main,
        const double *in_sub,
        double *in_ref_depth,
        double *o_depth, 
        uchar *o_occlusion_map,
        int width, 
        int height, 
        int num_channels,
        const wsp::img::StereoPOCParams &params, 
        const uchar *in_mask,
        uchar masked_value
){
    int num_pix = width*height;
    uchar thre_mask = 200;

    int blk_hw = params.block_width/2;
    int blk_hh = params.block_height/2;
    int blk_w = 2*blk_hw;
    int blk_h = 2*blk_hh + 1;
    int blk_len = blk_w * blk_h;
    int blk_exp_x = wsp::math::GetExponentOfPowerOfTwo(blk_w);
    int blk_exp_y = wsp::math::GetExponentOfPowerOfTwo(blk_h);

    // initialize output depth
    memset(o_depth, 0, sizeof(double)*num_pix*2);

    #ifdef USE_QT
        #ifdef _OPENMP
        QProgressBar *progress=NULL;
        #else
        QProgressDialog *progress=NULL;
        #endif
        if(params.show_progress){
            #ifdef _OPENMP
            progress = WSP_NEW QProgressBar();
            progress->setMinimum(0);
            progress->setMaximum(height);
            progress->show();
            progress->setValue(0);
            #else
            progress = WSP_NEW QProgressDialog("Stereo Matching...", "Cancel", 0, height);
            progress->setWindowModality(Qt::WindowModal);
            #endif
        }
    #endif
    #ifdef _OPENMP
    #pragma omp parallel
    #endif
    {
        double disp_x, disp_y; // temporary disparity for x and y
        double value, sum_sub, sum_main;
        int x, y, i, j, k, c; // iterators
        int px, py, index_main, index_sub, index_blk;
        complex64 *main_blk = (complex64*)malloc(sizeof(complex64)*blk_len);
        complex64 *sub_blk = (complex64*)malloc(sizeof(complex64)*blk_len);
        complex64 *tmp_poc = (complex64*)malloc(sizeof(complex64)*blk_len);
        complex64 *tmp_data = (complex64*)malloc(sizeof(complex64)*blk_len);
        complex64 *tmp_average_poc = (complex64*)malloc(sizeof(complex64)*blk_w);

        #ifdef _OPENMP
            // prerequisite for showing progress process -------------------
            int thread_show_progress = omp_get_num_threads()-2;
            if(thread_show_progress<0){ thread_show_progress=0; }
            //int show_progress_offset_y = thread_show_progress*height/omp_get_num_threads();
            int show_progress_offset_y = height/omp_get_num_threads();
            float show_progress_mult = omp_get_num_threads()*100.0/(float)height;
            double range = height/omp_get_num_threads();
            double start_offset = thread_show_progress*range;
            double end_offset = (omp_get_num_threads()-thread_show_progress-1)*range;
            // ---------------------------------
            #pragma omp for
        #endif
        for(y=0; y<height; ++y)
        {
            uchar *occ_ptr = o_occlusion_map + y*width;
            const uchar *mask_ptr = in_mask + y*width;
            double *dst_ptr = o_depth + y*width*2;
            double *ref_depth_ptr = in_ref_depth + y*width*2;
            for(x=0; x<width; ++x, dst_ptr+=2, ref_depth_ptr+=2, ++occ_ptr, ++mask_ptr)
            {
                if(in_mask!=NULL){
                    if(*mask_ptr==masked_value){
                        *dst_ptr = *ref_depth_ptr;
                        *(dst_ptr+1) = *(ref_depth_ptr+1);
                        //*occ_ptr = WSP_PIXEL_STATE_MASKED;
                        *occ_ptr = WSP_PIXEL_STATE_OCCLUDED;
                        continue;
                    }
                }

                int index = y*width + x;
                int index_2 = index*2;
                int index_3 = index*3;
                disp_x = *ref_depth_ptr;
                disp_y = *(ref_depth_ptr+1);


                // skip arround corner
                if(    x-blk_hw<0 
                    || x+blk_hw>=width
                    || y-blk_hh<0 
                    || y+blk_hh>=height
                    || (x+disp_x)-blk_hw<0
                    || (x+disp_x)+blk_hw>=width
                    || (y+disp_y)-blk_hh<0 
                    || (y+disp_y)+blk_hh>=height
                ){
                    *occ_ptr = WSP_PIXEL_STATE_OCCLUDED;
                    continue;
                }
                
                memset(main_blk, 0, sizeof(complex64)*blk_len);
                memset(sub_blk, 0, sizeof(complex64)*blk_len);

                // block loop for copying block pixels
                // evaluating RGB as sum of RGB
                for(i=0; i<blk_h; ++i){
                    for(j=0; j<blk_w; ++j){
                        px = x+j-blk_hw; // global x for block
                        py = y+i-blk_hh; // global y for block
                        // current index for main block image
                        index_main= (py*width + px)*num_channels;
                        // current index for sub block image
                        index_sub = ((py+static_cast<int>(disp_y))*width + (px+static_cast<int>(disp_x)))*num_channels; 
                        // current index within block
                        index_blk = i*blk_w + j;

                        // getting sum of rgb for double poc source
                        sum_sub = 0.0;
                        sum_main = 0.0;
                        for(c=0; c<num_channels; ++c){
                            sum_sub += in_sub[index_sub+c];
                            sum_main+= in_main[index_main+c];
                        }

                        main_blk[index_blk][0] = sum_sub;
                        sub_blk[index_blk][0] = sum_main;
                    }
                }

                // DEBUG -------------------
                //if(x==width/2){
                //    wsp::Image<double> save_main((double*)main_blk, NULL, NULL, blk_w, blk_h, 2);
                //    wsp::Image<double> save_sub((double*)sub_blk, NULL, NULL, blk_w, blk_h, 2);
                //    sprintf(filename, "__poc_main_blk_x%d.png", x);
                //    save_main.SaveImage(filename);
                //    sprintf(filename, "__poc_sub_blk_x%d.png", x);
                //    save_sub.SaveImage(filename);
                //}
                // -------------------

                double tmp_dx=0.0, tmp_dy=0.0;

                // skip if blocks have no charactaristics
                double mean_sd_main = 0.0;
                double mean_sd_sub = 0.0;
                if(params.thre_std_deviation!=0.0){
                    // calculate standard deviation
                    double *mb_ptr = (double*)main_blk;
                    double *sb_ptr = (double*)sub_blk;
                    double *end_ptr = mb_ptr+blk_len*2;
                    double av_m=0.0, av_s=0.0;
                    for(; mb_ptr!=end_ptr; mb_ptr+=2, sb_ptr+=2){
                        av_m += *mb_ptr;  av_s += *sb_ptr;
                    }
                    av_m /= blk_len;  av_s/= blk_len;
                    mb_ptr = (double*)main_blk;
                    sb_ptr = (double*)sub_blk;
                    end_ptr = mb_ptr+blk_len*2;
                    for(; mb_ptr!=end_ptr; mb_ptr+=2, sb_ptr+=2){
                        mean_sd_main += fabs(*mb_ptr - av_m);
                        mean_sd_sub += fabs(*sb_ptr - av_s);
                    }
                    mean_sd_main /=blk_len;
                    mean_sd_sub /= blk_len;
                }

                // DEBUG ----------------
                //wsp::Image<double> save_main, save_sub;
                //save_main.CopyFrom((double*)main_blk, NULL, NULL, blk_w, blk_h, 2);
                //save_sub.CopyFrom((double*)sub_blk, NULL, NULL, blk_w, blk_h, 2);
                // -----------------------------
                if(mean_sd_main<params.thre_std_deviation || mean_sd_sub<params.thre_std_deviation){
                    tmp_dx = 0.0;
                    tmp_dy = 0.0;
                }else{
                    // compute disparity
                    if(params.use_2d_poc){
                        wsp::img::FindDisparity2DPOC(
                            main_blk, 
                            sub_blk, 
                            blk_w, blk_h,
                            tmp_dx, tmp_dy,
                            params.thre_poc_peak, 
                            params.low_pass_ratio,
                            blk_exp_x, blk_exp_y,
                            tmp_poc, 
                            tmp_data,
                            params.sample_pix_distance,
                            params.sample_range
                        );
                        //wsp::img::FindDisparity2DPOC(disp_x[level], disp_y[level], NULL, subBlk, mainBlk,
                        //    //params.dispx_min, params.dispx_max, 
                        //    params.thre_poc_peak, params.low_pass_ratio);
                    }else{
                        // Original FFT version ------------------------------
                        tmp_dx = wsp::img::FindDisparity1DPOC(
                            main_blk, 
                            sub_blk, 
                            blk_w, blk_h,
                            params.thre_poc_peak, 
                            params.low_pass_ratio,
                            blk_exp_x,
                            tmp_poc, 
                            tmp_average_poc,
                            params.sample_pix_distance,
                            params.sample_range
                        );
                        // FFTW version ---------------------------------------
                        //double *tmp_main = WSP_NEW double[blk_len];
                        //double *tmp_sub = WSP_NEW double[blk_len];
                        //for(i=0; i<blk_len; ++i){
                        //    tmp_main[i] = main_blk[i][0];
                        //    tmp_sub[i] = sub_blk[i][0];
                        //}
                        //tmp_dx = wsp::img::FindDisparity1DPOC_fftw(
                        //    tmp_sub, 
                        //    tmp_main, 
                        //    blk_w, blk_h, 
                        //    -1000, 
                        //    1000, 
                        //    params.thre_poc_peak, 
                        //    params.low_pass_ratio
                        //);
                        //delete[] tmp_main;
                        //delete[] tmp_sub;
                        // ------------------------------------
                    }
                }
                //printf("(%d, %d) = %f, %f\n", x, y, tmp_dx, tmp_dy);
                // DEBUG -------------------------------
                //if(tmp_dx==-1.0 && disp_x==0.0){
                //    save_main.Normalize(0, 255);
                //    save_main.SaveImage("tmp_blk_main.png");
                //    save_sub.Normalize(0, 255);
                //    save_sub.SaveImage("tmp_blk_sub.png");
                //    exit(0);
                //}
                // -------------------------------

                // if poc reliability is low than stop calculation
                if(tmp_dx==DBL_MAX){
                    *occ_ptr = WSP_PIXEL_STATE_OCCLUDED;
                    tmp_dx = 0.0;
                    tmp_dy = 0.0;
                    //continue;
                }
                *dst_ptr = disp_x + tmp_dx;
                *(dst_ptr+1) = disp_y + tmp_dy;
                //if(*dst_ptr>blk_w){
                //    printf("(%d, %d) = (%f, %f)\n", x, y, *dst_ptr, *(dst_ptr+1));
                //}
            }
            if(params.show_progress){
                #ifdef _OPENMP
                    if(omp_get_thread_num()==thread_show_progress){
                        double percentage = (y-start_offset)/(double)(height-start_offset-end_offset);
                        //printf("y=%d, show_thread=%d, num_threads=%d, diff=%f ", y, thread_show_progress, omp_get_num_threads(),diff);
                        printf("POC stereo matching progress %f %% done..\r", percentage*100.0f);
                        //printf("progress of thead %d: %f %% done..\r", thread_show_progress, show_progress_mult*(y-show_progress_offset_y));
                        #ifdef USE_QT
                        progress->setValue((int)(percentage * height));
                        qApp->processEvents();
                        #endif
                    }
                    //printf("progress of thead %d: %f %% done..\r", omp_get_thread_num(), show_progress_mult*(y-omp_get_thread_num()*show_progress_offset_y));
                #else
                    printf("POC stereo matching progress %f %% done..\r", y*100.0/(float)height);
                    #ifdef USE_QT
                    progress->setValue(y);
                    if(progress->wasCanceled()){ delete progress; return WSP_STATE_CANCELED; }
                    #endif
                #endif
                //#endif
            }
        }
        free(main_blk);
        free(sub_blk);
        free(tmp_poc);
        free(tmp_data);
        free(tmp_average_poc);
    }
    #ifdef USE_QT
    delete progress;
    #endif

    return WSP_STATE_SUCCESS;
}

wsp::State wsp::img::StereoHierarchicalPOC(
        const double *in_main,
        const double *in_sub,
        const uchar *in_mask,
        double *o_depth, 
        uchar *o_occlusion_map,
        int width, 
        int height, 
        int num_channels,
        const wsp::img::StereoPOCParams &params
){
    char filename[MAX_LENGTH_OF_NAME];
    int num_pix = width*height;
    uchar thre_mask = 200;

    int blk_hw = params.block_width/2;
    int blk_hh = params.block_height/2;
    int blk_w = 2*blk_hw;
    int blk_h = 2*blk_hh + 1;
    int blk_len = blk_w * blk_h;
    int blk_exp_x = wsp::math::GetExponentOfPowerOfTwo(blk_w);
    int blk_exp_y = wsp::math::GetExponentOfPowerOfTwo(blk_h);

    if(params.show_debug_info)
    {
        printf(
            "===========================================================\n"
            "        Hierarchical Stereo Matching Based on POC \n"
            "===========================================================\n"
            "    width=%d, height=%d, numOfChannels=%d, \n"
            "    block width=%d, block height=%d\n"
            "    search depth=%d, low pass ratio=%f\n"
            "    poc peak=%f, thre std dev=%f\n"
            "    save debug data=%s\n"
            , width, height, num_channels 
            , params.block_width, params.block_height 
            , params.search_depth, params.low_pass_ratio 
            , params.thre_poc_peak, params.thre_std_deviation
            , params.save_debug_data?"true":"false"
        );
    }

    //#ifdef USE_QT
    //QProgressDialog progress("POC Stereo Matching...", "Cancel", 0, height);
    //progress.setWindowModality(Qt::WindowModal);
    //#endif

    wsp::Image<double> tmp_main, tmp_sub;
    wsp::Image<double> tmp_ref_depth;
    wsp::Image<double> *tmp_depth = WSP_NEW wsp::Image<double>[params.search_depth];
    for(int level=params.search_depth-1; level!=-1; --level)
    {
        if(params.show_debug_info){
            printf(
                "-----------------------------------\n"
                "Computing Hierarchical Stereo Mathicng: level %d\n"
                , level
            );
        }
        int sw = width>>level;
        int sh = height>>level;
        int slen = sw*sh;
        tmp_main.SetSize(sw, sh, num_channels);
        tmp_sub.SetSize(sw, sh, num_channels);
        wsp::img::ScaleImageNearestNeighbor(in_main, width, height, num_channels, tmp_main.data(), sw, sh);
        wsp::img::ScaleImageNearestNeighbor(in_sub,  width, height, num_channels, tmp_sub.data(), sw, sh);

        // DEBUG ---------------------------
        wsp::Image<double> save_ref(tmp_main);
        sprintf(filename, "poc_main_level%d.png", level);
        save_ref.SaveImage(filename);
        wsp::Image<double> save_tpl(tmp_sub);
        sprintf(filename, "poc_sub_level%d.png", level);
        save_tpl.SaveImage(filename);
        // ------------------

        tmp_ref_depth.SetSize(sw, sh, 2);
        if(level==params.search_depth-1){
            // When it is first computation, initialize ref by zero.
            tmp_ref_depth.Reset(0.0);
        }else{
            //wsp::img::ScaleImageNearestNeighbor(
            //        tmp_depth[level+1].data(),
            //        sw>>1, sh>>1, 2,
            //        tmp_ref_depth.data(),
            //        sw, sh
            //);
            wsp::img::ScaleImageBilinear(
                    tmp_depth[level+1].data(),
                    sw>>1, sh>>1, 2,
                    tmp_ref_depth.data(),
                    sw, sh
            );
            // round disparity off and multiply scaling factor for next computation
            double *dst_ptr = tmp_ref_depth.data();
            double *end_ptr = dst_ptr + slen*2;
            for(; dst_ptr!=end_ptr; ++dst_ptr){
                *dst_ptr = wsp::math::Round(*dst_ptr*2.0);
            }
            // DEBUG ---------------------------
            wsp::Image<double> save_depth(tmp_ref_depth);
            sprintf(filename, "poc_depth_level%d_scaled.png", level);
            save_depth.Normalize(0, 255);
            save_depth.SaveImage(filename);
            // ------------------
        }
        tmp_depth[level].SetSize(sw, sh, 2);
        tmp_depth[level].Reset(0.0);
        // main part
        wsp::img::StereoPOC_core(
            tmp_main.data(), 
            tmp_sub.data(), 
            tmp_ref_depth.data(), 
            tmp_depth[level].data(), 
            tmp_depth[level].state_data(),
            sw, sh, num_channels, params
        );
        if(level==0){
            double *src_ptr = tmp_depth[level].data();
            double *dst_ptr = o_depth;
            double *end_ptr = dst_ptr + slen*2;
            for(; dst_ptr!=end_ptr; ++dst_ptr, ++src_ptr){
                *dst_ptr = *src_ptr;
            }
        }

        // DEBUG ---------------------------
        wsp::Image<double> save_depth(tmp_depth[level]);
        sprintf(filename, "poc_depth_level%d_computed.png", level);
        save_depth.SetNumOfChannels(1);
        save_depth.Normalize(0, 255);
        save_depth.SaveImage(filename);
        // ------------------
    }



    return WSP_STATE_SUCCESS;
}



wsp::State wsp::img::StereoBMBasedPOC(
    const double *in_main,
    const double *in_sub,
    const uchar *in_mask,
    double *o_depth, 
    uchar *o_occlusion_map,
    int width, 
    int height, 
    int num_channels,
    const wsp::img::StereoPOCParams &params
)
{
    char filename[MAX_LENGTH_OF_NAME];
    int num_pix = width*height;
    uchar thre_mask = 200;

    int length = width*height;
    int blk_hw = params.block_width/2;
    int blk_hh = params.block_height/2;
    int blk_w = 2*blk_hw;
    int blk_h = 2*blk_hh + 1;
    int blk_len = blk_w * blk_h;
    int blk_exp_x = wsp::math::GetExponentOfPowerOfTwo(blk_w);
    int blk_exp_y = wsp::math::GetExponentOfPowerOfTwo(blk_h);

    if(params.show_debug_info)
    {
        printf(
            "===========================================================\n"
            "      Stereo Matching Based on Hybrid of BM and POC\n"
            "===========================================================\n"
            "    width=%d, height=%d, numOfChannels=%d, \n"
            "    block width=%d, block height=%d\n"
            "    search depth=%d, low pass ratio=%f\n"
            "    poc peak=%f, thre std dev=%f\n"
            "    use_mask=%s, save debug data=%s\n"
            , width, height, num_channels
            , params.block_width, params.block_height
            , params.search_depth, params.low_pass_ratio
            , params.thre_poc_peak, params.thre_std_deviation
            , in_mask==NULL?"false":"true", params.save_debug_data?"true":"false"
        );
    }

    //#ifdef USE_QT
    //QProgressDialog progress("POC Stereo Matching...", "Cancel", 0, height);
    //progress.setWindowModality(Qt::WindowModal);
    //#endif

    wsp::Image<double> tmp_ref_depth;
    wsp::Image<uchar> tmp_main;
    wsp::Image<uchar> tmp_sub;
    tmp_main.CopyFrom(in_main, NULL, NULL, width, height, num_channels);
    tmp_sub.CopyFrom(in_sub, NULL, NULL, width, height, num_channels);

    wsp::img::StereoBMParams bm_params = params.bm_params;
    //bm_params.block_min=params.bm_params.block_min;
    //bm_params.block_max=params.bm_params.block_max;
    //bm_params.cross_check=params.bm_params.cross_check;
    //bm_params.cross_check_fuzziness=params.bm_params.cross_check_fuzziness;
    //bm_params.dispx_max=params.bm_params.dispx_max;
    //bm_params.dispx_min=params.bm_params.dispx_min;
    //bm_params.thre_corr=params.bm_params.thre_corr;
    //bm_params.thre_std_deviation = params.bm_params.thre_std_deviation;
    bm_params.show_debug_info = false;
    bm_params.subpixel_estimation = false;
    //bm_params.eval_func = params.bm_params.eval_func;
    

    wsp::img::StereoBM(
        tmp_ref_depth,
        tmp_main,
        tmp_sub,
        bm_params
    );

    if(params.save_debug_data){
        wsp::Image<double> save_image(tmp_ref_depth);
        save_image.Normalize(0, 255);
        save_image.SetNumOfChannels(3);
        save_image.SaveImage("debug_StereoBMBasedPOC_disp_bm.png");
        save_image.SaveState("debug_StereoBMBasedPOC_occlusions.png", WSP_PIXEL_STATE_OCCLUDED);
    }

    uchar *front_mask = NULL;
    if(params.algorithm==WSP_STEREO_BM_BASED_FRONT_ONLY_POC)
    {
        front_mask = (uchar*)malloc(length);
        memset(front_mask, 0, length);

        wsp::Image<int> segm(width, height, 1);
        wsp::Image<double> tmp_bm_disp;
        tmp_bm_disp.CopyFrom(tmp_ref_depth, 1);
        //tmp_disp.SaveImage("tmp_ref_disp.png");
        int segm_num;
        //segm_num = wsp::img::SegmentationByHistogram<double, int>(
        //    tmp_bm_disp.data(), 
        //    segm.data(),
        //    num_pix, 1,
        //    0, 
        //    1, 
        //    true, 5
        //);
        //segm_num = wsp::img::SegmentationByHistogramAverageBase(
        //    tmp_bm_disp.data(),
        //    segm.data(),
        //    num_pix, 1,
        //    params.bg_estim_thre_pixel_amount_ratio,
        //    1
        //);
        segm_num = wsp::img::SegmentationByHistogram(
            tmp_bm_disp.data(), 
            segm.data(), 
            num_pix, 
            1, params.bg_estim_thre_pixel_amount_ratio*num_pix
        );
        if(params.show_debug_info){
            printf("Number of segments %d\n", segm_num);
        }
        if(segm_num>params.bg_estim_num_bg_segm)
        {
            // DEBUG ---------------------------------
            wsp::Image<uchar> debug_img(width, height, 1);
            debug_img.Reset(0);
            uchar *debug_ptr = debug_img.data();
            // --------------------------

            // create POC mask by segmentation map ---------
            int *segm_ptr = segm.data();
            uchar *src_ptr = tmp_ref_depth.state_data();
            uchar *end_ptr = src_ptr + length;
            uchar *mask_ptr = front_mask;
            if(params.nearer_is_brighter){
                int thre_id = params.bg_estim_num_bg_segm;
                for(; src_ptr!=end_ptr; ++src_ptr, ++segm_ptr
                    ,++debug_ptr, ++mask_ptr
                ){
                    if(*segm_ptr<thre_id){
                        *src_ptr = WSP_PIXEL_STATE_OCCLUDED;
                        *mask_ptr = 0xff;
                        // DEBUG --------------
                        *debug_ptr = 255;
                        // -----------
                    }
                }
            }else{
                for(; src_ptr!=end_ptr; ++src_ptr, ++segm_ptr
                    ,++debug_ptr, ++mask_ptr
                ){
                    int thre_id = segm_num - params.bg_estim_num_bg_segm;
                    if(*segm_ptr>thre_id){
                        *src_ptr = WSP_PIXEL_STATE_OCCLUDED;
                        *mask_ptr = 0xff;
                        // DEBUG --------------
                        *debug_ptr = 255;
                        // -----------
                    }
                }
            }
            // -----------------------

            // DEBUG -----------------------------
            if(params.save_debug_data){
                segm.Normalize(0, 255);
                segm.SetNumOfChannels(3);
                segm.SaveImage("debug_StereoBMBasedPOC_segm.png");
                tmp_bm_disp.SetNumOfChannels(3);
                tmp_bm_disp.Save("debug_StereoBMBasedPOC_disp_bm.raw");
                debug_img.SaveImage("debug_StereoBMBasedPOC_poc_mask.png");
            }
            // ------------------------

            //wsp::Image<double> segm(tmp_ref_depth.data(),NULL, NULL, width, height, 1);
            //double thre = (params.dispx_max+params.dispx_min)/2.0;
            //wsp::img::Binalize<double>(segm, thre, params.dispx_min, params.dispx_max);
            //double *segm_ptr = segm.data();
            //uchar *src_ptr = tmp_ref_depth.state_data();
            //uchar *end_ptr = src_ptr + width*height;
            //for(; src_ptr!=end_ptr; ++src_ptr, ++segm_ptr){
            //    if(*segm_ptr<thre){
            //        *src_ptr = WSP_PIXEL_STATE_OCCLUDED;
            //    }
            //}
        }        
    }
    //double *src_ptr = tmp_ref_depth.data();
    //double *end_ptr = src_ptr + width*height*2;
    //for(; src_ptr!=end_ptr; ++src_ptr){
    //    *src_ptr = -*src_ptr;
    //}


    // create POC mask by segmentation map ---------
    if(in_mask!=NULL){
        const uchar *mask_ptr = in_mask;
        uchar *src_ptr = tmp_ref_depth.state_data();
        uchar *end_ptr = src_ptr + length;
        for(; src_ptr!=end_ptr; ++src_ptr, ++mask_ptr){
            if(*mask_ptr==0){
                *src_ptr = WSP_PIXEL_STATE_OCCLUDED;
            }
        }
    }
    // -----------------------

    wsp::img::StereoPOC_core(
        in_main, 
        in_sub, 
        tmp_ref_depth.data(), 
        o_depth, 
        o_occlusion_map,
        width, height, num_channels, params,
        tmp_ref_depth.state_data(),
        WSP_PIXEL_STATE_OCCLUDED
    );

    // DEBUG ---------------------------
    if(params.save_debug_data){
        wsp::Image<double> save_depth(o_depth, NULL, NULL, width, height, 2);
        save_depth.Normalize(0, 255);
        save_depth.SetNumOfChannels(3);
        save_depth.SaveImage("debug_StereoBMBasedPOC_disp_poc.png");
    }

    //int len = width*height;
    //for(int i=0; i<len; ++i){
    //    printf("bm:%f %f, poc:%f %f\n", tmp_ref_depth[i*2], tmp_ref_depth[i*2+1]
    //            , o_depth[i*2], o_depth[i*2+1]);
    //}
    // ------------------

    if(front_mask!=NULL){
        uchar *mask_ptr = front_mask;
        uchar *end_ptr = mask_ptr + length;
        uchar *occ_ptr = o_occlusion_map;
        for(; mask_ptr!=end_ptr; ++mask_ptr, ++occ_ptr){
            if(*mask_ptr==0xff){
                *occ_ptr = WSP_PIXEL_STATE_NORMAL;
            }
        }
        free(front_mask);
    }


    return WSP_STATE_SUCCESS;
}


//wsp::State wsp::img::StereoBMBasedFrontOnlyPOC(
//    const double *in_main,
//    const double *in_sub,
//    const uchar *in_mask,
//    double *o_depth, 
//    uchar *o_occlusion_map,
//    int width, 
//    int height, 
//    int num_channels,
//    const wsp::img::StereoPOCParams &params
//){
//    char filename[MAX_LENGTH_OF_NAME];
//    int num_pix = width*height;
//    uchar thre_mask = 200;
//
//    int blk_hw = params.block_width/2;
//    int blk_hh = params.block_height/2;
//    int blk_w = 2*blk_hw;
//    int blk_h = 2*blk_hh + 1;
//    int blk_len = blk_w * blk_h;
//    int blk_exp_x = wsp::math::GetExponentOfPowerOfTwo(blk_w);
//    int blk_exp_y = wsp::math::GetExponentOfPowerOfTwo(blk_h);
//
//    printf("============================================================\n"
//           "  Stereo Matching Based on Hybrid of BM and Front-Only POC\n"
//           "============================================================\n");
//    printf("    width=%d, height=%d, numOfChannels=%d, \n", width, height, num_channels);
//    printf("    block width=%d, block height=%d\n", params.block_width, params.block_height);
//    printf("    search depth=%d, low pass ratio=%f\n", params.search_depth, params.low_pass_ratio);
//    printf("    poc peak=%f, thre std dev=%f\n", params.thre_poc_peak, params.thre_std_deviation);
//
//    //#ifdef USE_QT
//    //QProgressDialog progress("POC Stereo Matching...", "Cancel", 0, height);
//    //progress.setWindowModality(Qt::WindowModal);
//    //#endif
//
//    wsp::Image<double> tmp_ref_depth;
//    wsp::Image<uchar> tmp_main;
//    wsp::Image<uchar> tmp_sub;
//    tmp_main.CopyFrom(in_main, NULL, NULL, width, height, num_channels);
//    tmp_sub.CopyFrom(in_sub, NULL, NULL, width, height, num_channels);
//
//    wsp::img::StereoBMParams bm_params;
//    bm_params.block_min=params.bm_params.block_min;
//    bm_params.block_max=params.bm_params.block_max;
//    bm_params.cross_check=params.bm_params.cross_check;
//    bm_params.cross_check_fuzziness=0;
//    bm_params.dispx_max=params.bm_params.dispx_max;
//    bm_params.dispx_min=params.bm_params.dispx_min;
//    bm_params.thre_corr=0.2;
//    bm_params.thre_std_deviation = params.thre_std_deviation;
//
//    wsp::img::StereoBM(
//        tmp_ref_depth,
//        tmp_main,
//        tmp_sub,
//        bm_params
//    );
//
//    wsp::Image<uchar> segm(width, height, 1);
//    wsp::img::SegmentationByHistogram(
//        tmp_ref_depth.data(), 
//        segm.data(),
//        num_pix, 1, num_pix*0.002, 5
//    );
//    uchar *segm_ptr = segm.data();
//    uchar *src_ptr = tmp_ref_depth.state_data();
//    uchar *end_ptr = src_ptr + width*height;
//    for(; src_ptr!=end_ptr; ++src_ptr, ++segm_ptr){
//        if(*segm_ptr<128){
//            *src_ptr = WSP_PIXEL_STATE_OCCLUDED;
//        }
//    }
//
//    wsp::img::StereoPOC_core(
//        in_main, 
//        in_sub, 
//        tmp_ref_depth.data(), 
//        o_depth, 
//        o_occlusion_map,
//        width, height, num_channels, params,
//        tmp_ref_depth.state_data(),
//        WSP_PIXEL_STATE_OCCLUDED
//    );
//
//    // DEBUG ---------------------------
//    //wsp::Image<double> save_depth(o_depth, NULL, NULL, width, height, 2);
//    //save_depth.SetNumOfChannels(1);
//    //sprintf(filename, "debug_bm-based-poc_poc-depth.png");
//    //save_depth.Normalize(0, 255);
//    //save_depth.SaveImage(filename);
//
//    //int len = width*height;
//    //for(int i=0; i<len; ++i){
//    //    printf("bm:%f %f, poc:%f %f\n", tmp_ref_depth[i*2], tmp_ref_depth[i*2+1]
//    //            , o_depth[i*2], o_depth[i*2+1]);
//    //}
//    // ------------------
//
//
//
//    return WSP_STATE_SUCCESS;
//}



double wsp::img::FindDisparity1DPOC(
        complex64 *io_ref, 
        complex64 *io_tpl, 
        int width, int height, 
        double thre_poc_peak, 
        double low_pass_ratio,
        int exponent_x,
        complex64 *tmp_poc,
        complex64 *tmp_average_poc,
        int sample_pix_distance,
        int sample_range
){
    double delta_x;
    WSP_MathState state;

    int length = width * height;
    complex64 *poc_data=tmp_poc;
    //double *poc_average_data;
    complex64 *poc_average_data = tmp_average_poc;
    if(tmp_poc==NULL){
        poc_data = (complex64*)malloc(sizeof(complex64)*length);
    }
    if(tmp_average_poc==NULL){
        poc_average_data = (complex64*)malloc(sizeof(double)*width);
    }


    state = WSP_Get1dFftPoc_Complex(
        io_ref, 
        io_tpl, 
        poc_data, 
        poc_average_data, 
        width, height, exponent_x, true, low_pass_ratio
    );
    //state = wsp::math::GetPhaseCorrelationDFT1DPerLine_fftw(poc_data, poc_average_data, io_ref, io_tpl, width, height, low_pass_ratio);
    if(state!=WSP_STATE_SUCCESS){
        WSP_COMMON_ERROR_LOG("Getting POC failed\n");
        delta_x = DBL_MAX; goto EXIT; 
    }

    // check average ---------------------------
    //complex64 *av_ptr = poc_average_data;
    //complex64 *end_ptr = av_ptr + width;
    //complex64 *poc_ptr = poc_data;
    //for(; av_ptr!=end_ptr; ++av_ptr, ++poc_ptr){
    //    complex64 *poc_v_ptr = poc_ptr;
    //    complex64 *end_poc_v = poc_ptr+height*width;
    //    double sum = 0.0;
    //    for(; poc_v_ptr!=end_poc_v; poc_v_ptr+=width){
    //        sum += (*poc_v_ptr)[0];
    //    }
    //    sum/=(float)height;
    //    //printf("%f==%f\n", *av_ptr, sum);
    //    (*av_ptr)[0] = sum;
    //}
    // --------------------------------

    //if(wsp::GetMax((double*)poc_average_data, width)<thre_poc_peak){
    //    //WSP_COMMON_WARNING_LOG("POC reached threshold\n");
    //    delta_x = DBL_MAX; goto EXIT; 
    //}


    // get average of peaks calculated by any sampling point sets
    delta_x = 0.0;
    for(int i=0; i<sample_range; ++i){
        delta_x += wsp::img::GetPeakSubpixel(poc_average_data, width, sample_pix_distance+i, low_pass_ratio);
    }
    delta_x /= static_cast<double>(sample_range);


    ////int div = floor(delta_x/(double)width);
    ////delta_x = delta_x-(double)div*width;
    //double floating_point = 1.0 - (delta_x-floor(delta_x));
    ////printf("%f %f\n", delta_x, floating_point);
    //delta_x = floor(delta_x) + floating_point;
    ////printf("%f\n", delta_x);
    if(delta_x>(double)(width/2)){ delta_x-=width; }
    if(fabs(delta_x)>(double)(width/2)){
        //WSP_COMMON_WARNING_LOG("Computed disparity is wrong: %f\n", delta_x);
        delta_x=DBL_MAX; goto EXIT;
    }
    //delta_x *=-1;

EXIT:
    // freeing memories
    if(tmp_poc==NULL){ free(poc_data); }
    if(tmp_average_poc==NULL){ free(poc_average_data); }
    return delta_x;
}

wsp::Result wsp::img::FindDisparity2DPOC(
        complex64 *io_ref, 
        complex64 *io_tpl, 
        int width, int height,
        double &o_deltax, double &o_deltay, 
        //double minDisp, double maxDisp, 
        double thre_poc_peak, 
        double low_pass_ratio,
        int exponent_x, int exponent_y,
        complex64 *io_tmp_poc, 
        complex64 *io_tmp_data,
        int sample_pix_distance,
        int sample_range
){
    double delta_x, delta_y;
    complex64 *poc_data = io_tmp_poc;
    complex64 *tmp_data = io_tmp_data;

    int div;
    int length = (int)width*height;
    WSP_MathState state;

    if(io_tmp_poc==NULL){
        poc_data = (complex64*)malloc(sizeof(complex64)*length);
    }
    if(io_tmp_data==NULL){
        tmp_data = (complex64*)malloc(sizeof(complex64)*length);
    }

    state = WSP_Get2dPocWithFft_Complex(io_ref, io_tpl, poc_data, width, height, exponent_x, exponent_y, low_pass_ratio, true, tmp_data); 
    if(state!=WSP_STATE_SUCCESS){
        WSP_COMMON_ERROR_LOG("Getting POC failed\n");
        goto EXIT;
    }

    if(wsp::GetMax((double*)poc_data, length*2)<thre_poc_peak){
        o_deltax=DBL_MAX; o_deltay=DBL_MAX;
        goto EXIT;
    }

    // get average of x-peaks calculated by any sample sets
    delta_x = 0.0;
    for(int i=0; i<sample_range; ++i){
        delta_x += wsp::img::GetPeakSubpixel(poc_data, length, sample_pix_distance, low_pass_ratio);
    }
    delta_x /= static_cast<double>(sample_range);



    // find the maximum value and its location
    
    div = floor(delta_x/(double)width);
    delta_x = delta_x-(double)div*width;
    if(delta_x>(double)(width/2)){ delta_x-=width;}

    WSP_Transpose_Complex(poc_data, width, height);
    
    // get average of y-peaks calculated by any sample sets
    delta_y = 0.0;
    for(int i=0; i<sample_range; ++i){
        delta_y += wsp::img::GetPeakSubpixel(poc_data, length, sample_pix_distance, low_pass_ratio);
    }
    delta_y /= static_cast<double>(sample_range);

    div = floor(delta_y/(double)height);
    delta_y = delta_y-(double)div*height;
    if(delta_y>(double)(height/2)){ delta_y-=height;}

    //printf("SUBPIXEL UNIT: Peak is at %f %f\n", delta_x, deltaY);

    o_deltax = delta_x;
    o_deltay = delta_y;

EXIT:
    if(io_tmp_poc==NULL){ free(poc_data); }
    if(io_tmp_data==NULL){ free(tmp_data); }
    return WSP_MakeMathResult( state );
}