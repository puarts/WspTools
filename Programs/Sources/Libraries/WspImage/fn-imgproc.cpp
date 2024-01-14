/**
 * @file fn-imgproc.cpp
 * 
 */

#include <iostream>
#include <float.h>
#include <math.h>

#include <wsp/common/tpl_cl-vector3.h>
#include <wsp/common/WspRgb.h>
#include <wsp/common/WspPoint.h>
#include "tpl_fn-imgproc.hpp"
#ifdef USE_FFTW
#include <wsp/fftw/fn-math-gpl.h>
#endif
//#include <wsp/common/_tpl_fn-sort.hpp"
//#include <wsp/common/_tpl_fn-array.hpp"
//#include "WspImage_Index.h>

#include "fn-imgproc.h"

using namespace std;



//! Generator ---------------------------------------------
wsp::State wsp::img::GenerateRandomGaussDistribution(double *o_img, u32 width, u32 height,
    double offsetMax,
    u32 repeatNum, 
    double scaleMax,
    double varianceMin, 
    double varianceMax)
{
    if(varianceMin>varianceMax){
        WSP_COMMON_ERROR_LOG("varianceMin must be smaller than variance max\n");
        return WSP_STATE_FAILURE;
    }
    double *offsetList_x, *offsetList_y, value;
    double *scaleList, *varList;
    double varRange;
    double xv, yv, xo, yo;
    u32 i, x, y;
    offsetList_x = WSP_NEW double[repeatNum];
    offsetList_y = WSP_NEW double[repeatNum];
    scaleList = WSP_NEW double[repeatNum];
    varList = WSP_NEW double[repeatNum];
    varRange = varianceMax - varianceMin;

    //! generate random parameters for gaussian distribution function
    for(i=0; i<repeatNum; i++){
        offsetList_x[i] = ((double)rand()/(double)(RAND_MAX+1) - 0.5) * offsetMax * 2;
        offsetList_y[i] = ((double)rand()/(double)(RAND_MAX+1) - 0.5) * offsetMax * 2;
        scaleList[i] = ((double)(rand()+1)/(double)(RAND_MAX+1) ) * scaleMax;
        varList[i] = ((double)(rand()+1)/(double)(RAND_MAX+1) ) * varRange + varianceMin;
        //printf("%f %f %f %f\n", offsetList_x[i], offsetList_y[i], scaleList[i], varList[i]);
    }

#ifdef USE_QT //! progress bar preparation
    QProgressDialog progress("Processing...", "Cancel", 0, (double)height);
#endif

    //! compute value at each pixels
    for(y=0; y<height; y++){
#ifdef USE_QT //! progress bar grows here
        progress.setValue((double)y);
        if(progress.wasCanceled()){ 
            progress.setValue((double)height);
            delete[] offsetList_x; delete[] offsetList_y;
            delete[] scaleList; delete[] varList;
            return WSP_STATE_CANCELED;
        }
#endif
        for(x=0; x<width; x++){
            xv = (double)((int)x-(int)width/2);
            yv = (double)((int)y-(int)height/2);
            value = 0;
            //printf("%f %f\n", xv, yv);
            for(i=0; i<repeatNum; i++){
                xo = xv + offsetList_x[i];
                yo = yv + offsetList_y[i];
                value += exp(-xo*xo/varList[i]) * exp(-yo*yo/varList[i]) * scaleList[i];
            }
            //printf("%f\n", value);
            o_img[y*width+x] = value;
        }
        printf("%d\r", y);
    }
#ifdef USE_QT // end progress bar
    progress.setValue((double)height);
#endif

    delete[] offsetList_x;
    delete[] offsetList_y;
    delete[] scaleList;
    delete[] varList;

    return WSP_STATE_SUCCESS;

}

wsp::State wsp::img::GetHorSum(wsp::Image<double> &o_sum, const wsp::Image<double> &in_img)
{
    u32 x, y;
    u32 width, height;
    double sum;
    width = (u32)in_img.width();
    height = (u32)in_img.height();

    o_sum.SetSize(width, height);

    //! Get horizontal sum
    for(y=0; y<height; y++){
        sum = 0;
        for(x=0; x<width; x++){
            sum += (double)in_img.GetElem(x, y);
            o_sum.SetElem(sum, x, y);
        }
    }
    //o_sum.SaveAsTiff("sumHor.tif");
    return WSP_STATE_SUCCESS;
}

//! Get sum of full sum of image
wsp::State wsp::img::GetFullSum(wsp::Image<double> &o_sum, const wsp::Image<double> &in_img)
{
    u32 x, y;
    u32 width, height;
    double sum;
    width = (u32)in_img.width();
    height = (u32)in_img.height();

    o_sum.SetSize(width, height);

    wsp::img::GetHorSum(o_sum, in_img);
    //! Get horizontal sum
    //for(y=0; y<height; y++){
    //    sum = 0;
    //    for(x=0; x<width; x++){
    //        sum += (double)in_img.GetElem(x, y);
    //        o_sum.SetElem(sum, x, y);
    //    }
    //}

    //! Get full sum
    for(x=0; x<width; x++){
        sum = 0;
        for(y=0; y<height; y++){
            sum += (double)o_sum.GetElem(x, y);
            o_sum.SetElem(sum, x, y);
        }
    }
    //o_sum.SaveAsTiff("sumFull.tif");

    return WSP_STATE_SUCCESS;
}





//! correct depth contour by color segmentation image because depth contour is not reliable
wsp::State wsp::img::RemoveUnreliableDepthBySegmentationMap(wsp::Image<double> &io_depth, const wsp::Image<int> &in_segmDepth, const wsp::Image<int> &in_segmCol)
{
    u32 i_seg, i, index, index_c, index_d;
    u32 nSegm, len=(int)in_segmCol.length();
    if(len!=(int)in_segmDepth.length() || len!=(int)io_depth.length()){
        WSP_COMMON_ERROR_LOG("length of arugment images must be the same\n"); return WSP_STATE_FAILURE;
    }

    wsp::Vector<int> segmList;
    wsp::Vector<int> segmCntIndex, segmCntNum; //! counter for find segm which has max area
    in_segmCol.GetIndexList(segmList);
    nSegm = segmList.length();
    for(i_seg=0; i_seg<nSegm; i_seg++){
        index_c = segmList[i_seg];

        //! init count list
        segmCntIndex.SetLength(0);
        segmCntNum.SetLength(0);

        printf("----------------\n");
        //! count num of pixels for each segment
        for(i=0; i<len; i++){
            if(in_segmCol[i]!=index_c){ continue; }

            index_d = in_segmDepth[i];
            index = segmCntIndex.GetIndex(index_d);
            if(index==-1){
                printf("index_d=%d\n", index_d);
                segmCntIndex.Append(index_d);
                segmCntNum.Append(0);
                segmCntNum[segmCntNum.length()-1]++;
                continue;
            }
            segmCntNum[index]++;
        }
        //! get segment index which has maximum area
        index = segmCntIndex[wsp::GetMaxIndex(segmCntNum.data(), segmCntNum.length())];

        //! print info
        for(i=0; i<segmCntNum.length(); i++){
            printf("index %d: %d\n", segmCntIndex[i], segmCntNum[i]);
        }
        printf("num of segm: %d, ", segmCntNum.length());
        printf("max index: %d\n", index);

        //! remove all segment excepting segment which has maximum area
        for(i=0; i<len; i++){
            if(in_segmCol[i]!=index_c){ continue; }

            index_d = in_segmDepth[i];
            if(index_d!=index){
                io_depth.SetElem(0, (u32)i);
                io_depth.SetAlpha(0, (u32)i);
                io_depth.SetState(WSP_PIXEL_STATE_OCCLUDED, (u32)i);
            }
        }
    }
    return WSP_STATE_SUCCESS;
}

