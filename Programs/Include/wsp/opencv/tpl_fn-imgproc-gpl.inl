#pragma once

#include "tpl_fn-imgproc-gpl.h"

//! Resize -----------------------------------------------------
template<typename T> wsp::State wsp::opencv::cvResizeU8Image(wsp::Image<T> &io_img, u32 width, u32 height, int algorithm)
{
    int w = (int)io_img.width(), h = (int)io_img.height();
    int nChannels = (int)io_img.num_of_channels() * sizeof(T);
    int x, y;
    int len = (u32)(w*h);
    int i, i_cv;
    IplImage *src_img = NULL, *dst_mask = NULL, *dst_img = NULL, *src_mask = NULL;

    src_img = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, (int)nChannels);
    dst_img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, (int)nChannels);
    src_mask = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
    dst_mask = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);

    //printf("w:%d, h:%d, widthStep:%d\n", src_img->width, src_img->height, src_img->widthStep);

    wsp::opencv::cvArray2IplImage<uchar>(src_img, src_mask, (uchar*)io_img.data(), io_img.alpha_data(), w, h, nChannels);

    cvResize(src_img, dst_img, algorithm);
    cvResize(src_mask, dst_mask, algorithm);

    io_img.SetSize(width, height, io_img.num_of_channels());

    wsp::opencv::cvIplImage2Array<uchar>((uchar*)io_img.data(), io_img.alpha_data(), dst_img, dst_mask);

    cvReleaseImage(&src_img);
    cvReleaseImage(&src_mask);
    cvReleaseImage(&dst_img);
    cvReleaseImage(&dst_mask);

    return WSP_STATE_SUCCESS;
}

template<typename T> wsp::State wsp::opencv::cvResizeImage(wsp::Image<T> &io_img, u32 width, u32 height, int algorithm)
{
    int w = (int)io_img.width(), h = (int)io_img.height();
    int nChannels = (int)io_img.num_of_channels();
    int x, y;
    int len = (u32)(w*h);
    int i, i_cv;
    IplImage *src_img = NULL, *dst_mask = NULL, *dst_img = NULL, *src_mask = NULL;

    int depthType;
    switch (sizeof(T)){
    case 1: depthType = IPL_DEPTH_8U; break;
    case 4: depthType = IPL_DEPTH_32F; break;
    case 8: depthType = IPL_DEPTH_64F; break;
    default: WSPERROR("Data type of image is invalid\n"); return WSP_STATE_FAILURE;
    }

    src_img = cvCreateImage(cvSize(w, h), depthType, (int)nChannels);
    dst_img = cvCreateImage(cvSize(width, height), depthType, (int)nChannels);
    src_mask = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
    dst_mask = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);

    //printf("w:%d, h:%d, widthStep:%d\n", src_img->width, src_img->height, src_img->widthStep);

    //switch(sizeof(T)){
    //	case 1:
    //		wsp::opencv::cvArray2IplImage<T, uchar>(src_img, src_mask, io_img.data(), io_img.alpha_data(), w, h, nChannels);
    //		break;
    //	case 4:
    //		wsp::opencv::cvArray2IplImage<T, float>(src_img, src_mask, io_img.data(), io_img.alpha_data(), w, h, nChannels);
    //		break;
    //	case 8:
    //		wsp::opencv::cvArray2IplImage<T, double>(src_img, src_mask, io_img.data(), io_img.alpha_data(), w, h, nChannels);
    //		break;
    //	default: WSPERROR("Data type of image is invalid\n"); return WSP_STATE_FAILURE;
    //}
    wsp::opencv::cvArray2IplImage<T>(src_img, src_mask, io_img.data(), io_img.alpha_data(), w, h, nChannels);


    cvResize(src_img, dst_img, algorithm);
    cvResize(src_mask, dst_mask, algorithm);

    io_img.SetSize(width, height, io_img.num_of_channels());

    wsp::opencv::cvIplImage2Array<T>(io_img.data(), io_img.alpha_data(), dst_img, dst_mask);

    cvReleaseImage(&src_img);
    cvReleaseImage(&src_mask);
    cvReleaseImage(&dst_img);
    cvReleaseImage(&dst_mask);

    return WSP_STATE_SUCCESS;
}

template<typename T> wsp::State wsp::opencv::cvResizeImageNearestNeighbor(wsp::Image<T> &io_img, u32 newWidth, u32 newHeight){
    return wsp::opencv::cvResizeImage(io_img, newWidth, newHeight, CV_INTER_NN);
}

template<typename T> wsp::State wsp::opencv::cvResizeImageLinear(wsp::Image<T> &io_img, u32 newWidth, u32 newHeight){
    return wsp::opencv::cvResizeImage(io_img, newWidth, newHeight, CV_INTER_LINEAR);
}

template<typename T> wsp::State wsp::opencv::cvResizeImageArea(wsp::Image<T> &io_img, u32 newWidth, u32 newHeight){
    return wsp::opencv::cvResizeImage(io_img, newWidth, newHeight, CV_INTER_AREA);
}

template<typename T> wsp::State wsp::opencv::cvResizeImageBicubic(wsp::Image<T> &io_img, u32 newWidth, u32 newHeight){
    return wsp::opencv::cvResizeImage(io_img, newWidth, newHeight, CV_INTER_CUBIC);
}

//! Conversion -----------------------------------------------------
template<typename T>
wsp::State wsp::opencv::cvArray2IplImage(IplImage *o_img, IplImage *o_alpha, const T *in_img, const uchar *in_alpha, u32 width, u32 height, u32 nChannel)
{
    int w = (int)width, h = (int)height, spp = (int)nChannel;
    if (o_img->width != w || o_img->height != h || (o_alpha != NULL && (o_alpha->width != w || o_alpha->height != h))){
        WSPERROR("Image sizes of input and output images are different\n");
        return WSP_STATE_FAILURE;
    }

    //printf("cvArray2IplImage: widthStep=%d\n", o_img->widthStep);

    int x, y, c, i, i_cv;
    T *data;
    //data = (T*)o_img->imageData;
    for (y = 0; y<h; y++){
        for (x = 0; x<w; x++){
            i = y*w + x;
            i_cv = y*o_img->widthStep + x*sizeof(T)*spp;
            //i_cv=(y*w + x)*spp;
            data = reinterpret_cast<T*>(&o_img->imageData[i_cv]);
            for (c = 0; c<spp; c++){
                data[c] = in_img[i*spp + c];
            }
            if (in_alpha != NULL){
                o_alpha->imageData[y*o_alpha->widthStep + x] = in_alpha[i];
            }
        }
    }

    return WSP_STATE_SUCCESS;
}

template<typename T>
wsp::State wsp::opencv::cvIplImage2Array(T *o_img, uchar *o_alpha, const IplImage *in_img, const IplImage *in_alpha)
{
    if (o_img == NULL){ WSPERROR("Destination image is NULL\n"); return WSP_STATE_FAILURE; }

    u32 w = in_img->width, h = in_img->height, spp = in_img->nChannels;
    if ((in_alpha != NULL && (in_alpha->width != (int)w || in_alpha->height != (int)h))){
        WSPERROR("Image and alpha map size are different\n"); return WSP_STATE_FAILURE;
    }

    u32 x, y, c, i, i_cv;
    T *data;
    //T *data = (T*)in_img->imageData;
    for (y = 0; y<h; y++){
        for (x = 0; x<w; x++){
            i = y*w + x;
            i_cv = y*in_img->widthStep + x*sizeof(T)*spp;
            //i_cv = (y*w + x) * spp;
            data = reinterpret_cast<T*>(&in_img->imageData[i_cv]);
            for (c = 0; c<spp; c++){
                o_img[i*spp + c] = data[c];
            }
            if (in_alpha != NULL){
                o_alpha[i] = in_alpha->imageData[y*in_alpha->widthStep + x];
            }
        }
    }

    return WSP_STATE_SUCCESS;
}

template<typename T> wsp::State wsp::opencv::cvWspImg2IplImage(IplImage *o_img, IplImage *o_alpha, const wsp::Image<T> &in_img)
{
    return wsp::opencv::cvArray2IplImage<T>(o_img, o_alpha, in_img.data(), in_img.alpha_data(), in_img.width(), in_img.height(), in_img.num_of_channels());
}

template<typename T> wsp::State wsp::opencv::cvIplImage2WspImg(wsp::Image<T> &o_img, const IplImage *in_img, const IplImage *in_alpha)
{
    return wsp::opencv::cvIplImage2Array<T>(o_img.data(), o_img.alpha_data(), in_img, in_alpha);
}

// ----------------------------------------------------------------

template<typename T> wsp::State wsp::opencv::cvInpaint(T *io_img, const uchar *in_mask, int width, int height, int nChannels, int algType)
{
    if (io_img == NULL){
        WSPERROR("Input image is NULL\n");
        return WSP_STATE_FAILURE;
    }

    int w = (int)width, h = (int)height;
    int x, y;
    int len = (u32)(w*h);
    int i, i_cv;
    IplImage *src_img = NULL, *mask_img = NULL, *dst_img;

    int depthType;
    switch (sizeof(T)){
    case 1: depthType = IPL_DEPTH_8U; break;
    case 4: depthType = IPL_DEPTH_32F; break;
    case 8: depthType = IPL_DEPTH_64F; break;
    default: WSPERROR("Data type of image is invalid\n"); return WSP_STATE_FAILURE;
    }

    src_img = cvCreateImage(cvSize(w, h), depthType, (int)nChannels);
    dst_img = cvCreateImage(cvSize(w, h), depthType, (int)nChannels);
    mask_img = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);

    //printf("w:%d, h:%d, ws:%d", src_img->width, src_img->height, src_img->widthStep);

    //WspFnImgIO::SaveImage24(io_img, NULL, width, height, "test_beforeInpaint.tif");

    wsp::opencv::cvArray2IplImage<T>(src_img, mask_img, io_img, in_mask, width, height, nChannels);

    cvInpaint(src_img, mask_img, dst_img, 10.0, algType);

    //cvNamedWindow ("src", CV_WINDOW_AUTOSIZE);
    //cvNamedWindow ("dst", CV_WINDOW_AUTOSIZE);
    //cvShowImage ("src", src_img);
    //cvShowImage ("dst", dst_img);
    //cvWaitKey (0);

    //cvDestroyWindow ("src");
    //cvDestroyWindow ("dst");

    wsp::opencv::cvIplImage2Array<T>(io_img, NULL, dst_img, NULL);

    cvReleaseImage(&src_img);
    cvReleaseImage(&mask_img);
    cvReleaseImage(&dst_img);

    return WSP_STATE_SUCCESS;
}

template<typename T>
wsp::State wsp::opencv::cvInpaintAlpha(T *io_img, const uchar *in_alpha, int width, int height, int nChannels, int algType)
{
    u32 len = width*height;
    uchar *mask = new uchar[len];
    wsp::State state;
    for (u32 i = 0; i<len; i++){
        mask[i] = 255 - in_alpha[i];
    }
    state = wsp::opencv::cvInpaint(io_img, mask, width, height, nChannels, algType);
    delete[] mask;
    return state;
}

template<typename T>
wsp::State wsp::opencv::cvInpaintState(T *io_img, const uchar *in_state, int width, int height, int nChannels,
    wsp::img::PixelState target_state, int algType)
{
    u32 len = width*height;
    uchar *mask = new uchar[len];
    wsp::State state;
    for (u32 i = 0; i<len; i++){
        if (in_state[i] == target_state){
            mask[i] = 255; continue;
        }
        mask[i] = 0;
    }
    state = wsp::opencv::cvInpaint(io_img, mask, width, height, nChannels, algType);
    delete[] mask;
    return state;
}

template<typename T> inline wsp::State wsp::opencv::cvInpaintNavierStokes(T *io_img, const uchar *in_mask, int width, int height, int nChannels){
    return wsp::opencv::cvInpaint(io_img, in_mask, width, height, nChannels, CV_INPAINT_NS);
}

template<typename T> inline wsp::State wsp::opencv::cvInpaintAlexandruTelea(T *io_img, const uchar *in_mask, int width, int height, int nChannels){
    return wsp::opencv::cvInpaint(io_img, in_mask, width, height, nChannels, CV_INPAINT_TELEA);
}

template<typename T> inline wsp::State wsp::opencv::cvInpaintAlphaNavierStokes(T *io_img, const uchar *in_alpha, int width, int height, int nChannels){
    return wsp::opencv::cvInpaintAlpha(io_img, in_alpha, width, height, nChannels, CV_INPAINT_NS);
}

template<typename T> inline wsp::State wsp::opencv::cvInpaintAlphaAlexandruTelea(T *io_img, const uchar *in_alpha, int width, int height, int nChannels){
    return wsp::opencv::cvInpaintAlpha(io_img, in_alpha, width, height, nChannels, CV_INPAINT_TELEA);
}

template<typename T> inline wsp::State wsp::opencv::cvInpaintStateNavierStokes(T *io_img, const uchar *in_state, int width, int height, int nChannels, wsp::img::PixelState target_state){
    return wsp::opencv::cvInpaintState(io_img, in_state, width, height, nChannels, target_state, CV_INPAINT_NS);
}

template<typename T> inline wsp::State wsp::opencv::cvInpaintStateAlexandruTelea(T *io_img, const uchar *in_state, int width, int height, int nChannels, wsp::img::PixelState target_state){
    return wsp::opencv::cvInpaintState(io_img, in_state, width, height, nChannels, target_state, CV_INPAINT_TELEA);
}

