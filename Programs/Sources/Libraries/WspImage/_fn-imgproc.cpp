// @file _fn-imgproc.cpp
// 

#define _USE_MATH_DEFINES
#include <math.h>

#include "_fn-imgproc.h"

//! Channel Operation -------------------------------------------------------------------------
wsp::State wsp::img::MergeChannels(uchar *o_dst, u32 dst_spp, const uchar *in_data, u32 src_spp, const uchar *in_alpha, u32 width, u32 height)
{
    if(dst_spp==src_spp){ 
        WSP_COMMON_ERROR_LOG("spp of source and destination is the same\n"); return WSP_STATE_INVALID_PARAMETER; 
    }
    //! if source image is not RGB or Gray image and destination's spp is not 2 or 4, than return
    if( dst_spp < src_spp){ WSP_COMMON_ERROR_LOG("Samples Per Pixel are wrong\n"); return WSP_STATE_FAILURE; }

    //! Creating alpha channel
    //tmp_alpha = new uchar[dataLen + RESERVE_MEM];
    //if(tmp_alpha==NULL){ WSP_COMMON_ERROR_LOG("Memory allocation failed\n"); return WSP_STATE_INSUFFICIENT_MEMORY; }
    //if(in_alpha==NULL){ memset(tmp_alpha, 255, dataLen); }
    //else{ memcpy(tmp_alpha, in_alpha, dataLen); }

    //for(u32 y=0; y<height; y++){
    //    yIndex = y*width;
    //    for(u32 x=0; x<width; x++){
    //        imgIndex = yIndex + x;
    //        srcIndex = imgIndex * src_spp;
    //        dstIndex = imgIndex * dst_spp;
    //        for(u32 i=0; i<dst_spp-1; i++){
    //            if(i >= src_spp){
    //                o_dst[dstIndex+i] = in_data[srcIndex+src_spp-1];
    //                continue;
    //            }
    //            o_dst[dstIndex+i] = in_data[srcIndex+i];
    //        }
    //        o_dst[dstIndex+dst_spp-1] = tmp_alpha[imgIndex];
    //    }
    //}
    u32 i, c;
    u32 len = width*height;
    for(i=0; i<len; i++){
        u32 src_index = i*src_spp;
        u32 dst_index = i*dst_spp;
        for(c=0; c<dst_spp-1; c++){
            if(i >= src_spp){
                o_dst[dst_index+c] = in_data[src_index+src_spp-1];
                continue;
            }
            o_dst[dst_index+c] = in_data[src_index+c];
        }
        if(in_alpha==NULL){ continue; }
        o_dst[dst_index+dst_spp-1] = in_alpha[i];
    }
    //delete[] tmp_alpha;
    return WSP_STATE_SUCCESS;
}

wsp::State wsp::img::SeparateChannels(uchar *o_rgbImg, uchar *o_alpha, const uchar *in_imgRgba, u32 width, u32 height)
{
    for(u32 i=0; i<width*height; i++){
        u32 dstIndex = i*3;
        u32 srcIndex = i*4;
        o_rgbImg[dstIndex] = in_imgRgba[srcIndex];
        o_rgbImg[dstIndex+1] = in_imgRgba[srcIndex+1];
        o_rgbImg[dstIndex+2] = in_imgRgba[srcIndex+2];
        o_alpha[i] = in_imgRgba[srcIndex+3];
    }
    return WSP_STATE_SUCCESS;
}

// =======================================================
// =======================================================
//                 Stereo Matching
// =======================================================
// =======================================================


//! Find a peak for a phase correlation result, d is distance of sampling pixels from a peak pixel
double wsp::img::GetPeakSubpixel(const double *in_poc_data, int length, int d, double low_pass_ratio)
{
    // find max index ---------------
    int p = 0;
    const double *ptr=in_poc_data;
    const double *end_ptr=ptr+length;
    double max = *ptr;
    ++ptr;
    for(int i=1; ptr!=end_ptr; ++ptr, ++i){
        if(max<*ptr){
            max=*ptr;
            p=i;
        }
    }
    // -----------------------
    //int p_i = p-M;
    //printf("p=%d, p_i=%d\n", p, p_i);
    //printf("r[p+d]=%f, r[p]=%f, r[p-d]=%f\n", r[p+d], r[p], r[p-d]);
    if(p-d<0){
        return wsp::img::GetPeakSubpixel(max, p, d, in_poc_data[length+(p-d)], in_poc_data[p+d], length, low_pass_ratio);
    }else if(p+d>=length){
        return wsp::img::GetPeakSubpixel(max, p, d, in_poc_data[p-d], in_poc_data[(p+d)-length], length, low_pass_ratio);
    }
    return wsp::img::GetPeakSubpixel(max, p, d, in_poc_data[p-d], in_poc_data[p+d], length, low_pass_ratio);

    //double cospid2r = 2*cos(low_pass_ratio*M_PI*d)*r[p];
    //double u_pd = r[p-d] + r[p+d] - cospid2r;
    //double v_pd = p_i*cospid2r - (p_i-d)*r[p-d] - (p_i+d)*r[p+d];
    //double delta = v_pd / u_pd;
    //double disparity = delta - M + N;
    ////printf("delta = %f\n", delta);
    ////printf("disparity = %f\n", disparity);

    //// check correction of equation
    //double left = (p_i-d+delta)*r[p-d]+(p_i+d+delta)*r[p+d];
    //double right = 2*(p_i+delta)*cos(low_pass_ratio*M_PI*d)*r[p];
    ////printf("%f = %f\n", left, right);

    //return disparity;
    ////return delta;
}

//! Find a peak for a phase correlation result, d is distance of sampling pixels from a peak pixel
double wsp::img::GetPeakSubpixel(const complex64 *in_poc_data, int length, int d, double low_pass_ratio)
{
    int p = 0;
    // find max index ---------------
    complex64 *ptr=const_cast<complex64*>(in_poc_data);
    complex64 *end_ptr=ptr+length;
    double max = *ptr[0];
    ++ptr;
    for(int i=1; ptr!=end_ptr; ++ptr, ++i){
        if(max<*ptr[0]){
            max=*ptr[0];
            p=i;
        }
    }
    // -----------------------
    if(p-d<0){
        return wsp::img::GetPeakSubpixel(max, p, d, in_poc_data[length+(p-d)][0], in_poc_data[p+d][0], length, low_pass_ratio);
    }else if(p+d>=length){
        return wsp::img::GetPeakSubpixel(max, p, d, in_poc_data[p-d][0], in_poc_data[(p+d)-length][0], length, low_pass_ratio);
    }
    return wsp::img::GetPeakSubpixel(max, p, d, in_poc_data[p-d][0], in_poc_data[p+d][0], length, low_pass_ratio);
}

