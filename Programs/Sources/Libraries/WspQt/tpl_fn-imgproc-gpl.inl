#pragma once

#include "tpl_fn-imgproc-gpl.h"

//! Resize -----------------------------------------------------
#ifdef USE_OPENCV
template<typename T> wsp::State wsp::img::cvResizeU8Image(wsp::Image<T> &io_img, uint width, uint height, int algorithm)
{
    int w = (int)io_img.width(), h = (int)io_img.height();
    int nChannels = (int)io_img.num_of_channels() * sizeof(T);
    int x, y;
    int len = (uint)(w*h);
    int i, i_cv;
    IplImage *src_img = NULL, *dst_mask = NULL, *dst_img = NULL, *src_mask = NULL;

    src_img = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, (int)nChannels);
    dst_img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, (int)nChannels);
    src_mask = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
    dst_mask = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);

    //printf("w:%d, h:%d, widthStep:%d\n", src_img->width, src_img->height, src_img->widthStep);

    wsp::img::cvArray2IplImage<uchar>(src_img, src_mask, (uchar*)io_img.data(), io_img.alpha_data(), w, h, nChannels);

    cvResize(src_img, dst_img, algorithm);
    cvResize(src_mask, dst_mask, algorithm);

    io_img.SetSize(width, height, io_img.num_of_channels());

    wsp::img::cvIplImage2Array<uchar>((uchar*)io_img.data(), io_img.alpha_data(), dst_img, dst_mask);

    cvReleaseImage(&src_img);
    cvReleaseImage(&src_mask);
    cvReleaseImage(&dst_img);
    cvReleaseImage(&dst_mask);

    return WSP_STATE_SUCCESS;
}

template<typename T> wsp::State wsp::img::cvResizeImage(wsp::Image<T> &io_img, uint width, uint height, int algorithm)
{
    int w = (int)io_img.width(), h = (int)io_img.height();
    int nChannels = (int)io_img.num_of_channels();
    int x, y;
    int len = (uint)(w*h);
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
    //		wsp::img::cvArray2IplImage<T, uchar>(src_img, src_mask, io_img.data(), io_img.alpha_data(), w, h, nChannels);
    //		break;
    //	case 4:
    //		wsp::img::cvArray2IplImage<T, float>(src_img, src_mask, io_img.data(), io_img.alpha_data(), w, h, nChannels);
    //		break;
    //	case 8:
    //		wsp::img::cvArray2IplImage<T, double>(src_img, src_mask, io_img.data(), io_img.alpha_data(), w, h, nChannels);
    //		break;
    //	default: WSPERROR("Data type of image is invalid\n"); return WSP_STATE_FAILURE;
    //}
    wsp::img::cvArray2IplImage<T>(src_img, src_mask, io_img.data(), io_img.alpha_data(), w, h, nChannels);


    cvResize(src_img, dst_img, algorithm);
    cvResize(src_mask, dst_mask, algorithm);

    io_img.SetSize(width, height, io_img.num_of_channels());

    wsp::img::cvIplImage2Array<T>(io_img.data(), io_img.alpha_data(), dst_img, dst_mask);

    cvReleaseImage(&src_img);
    cvReleaseImage(&src_mask);
    cvReleaseImage(&dst_img);
    cvReleaseImage(&dst_mask);

    return WSP_STATE_SUCCESS;
}
template<typename T> wsp::State wsp::img::cvResizeImageNearestNeighbor(wsp::Image<T> &io_img, uint newWidth, uint newHeight){
    return wsp::img::cvResizeImage(io_img, newWidth, newHeight, CV_INTER_NN);
}
template<typename T> wsp::State wsp::img::cvResizeImageLinear(wsp::Image<T> &io_img, uint newWidth, uint newHeight){
    return wsp::img::cvResizeImage(io_img, newWidth, newHeight, CV_INTER_LINEAR);
}
template<typename T> wsp::State wsp::img::cvResizeImageArea(wsp::Image<T> &io_img, uint newWidth, uint newHeight){
    return wsp::img::cvResizeImage(io_img, newWidth, newHeight, CV_INTER_AREA);
}
template<typename T> wsp::State wsp::img::cvResizeImageBicubic(wsp::Image<T> &io_img, uint newWidth, uint newHeight){
    return wsp::img::cvResizeImage(io_img, newWidth, newHeight, CV_INTER_CUBIC);
}

#endif

//! Conversion -----------------------------------------------------
#ifdef USE_OPENCV
//template<typename T, typename U>
//wsp::State wsp::img::cvArray2IplImage(IplImage *o_img, IplImage *o_alpha, const T *in_img, const uchar *in_alpha, uint width, uint height, uint nChannel)
//{
//	int w=(int)width, h=(int)height, spp=(int)nChannel;
//	if(o_img->width!=w || o_img->height!=h || (o_alpha!=NULL && (o_alpha->width!=w || o_alpha->height!=h))){
//		WSPERROR("Image sizes of input and output images are different\n");
//		return WSP_STATE_FAILURE;
//	}
//
//	int x, y, c, i, i_cv;
//	for(y=0; y<h; y++){
//		for(x=0; x<w; x++){
//			i=y*w + x;
//			i_cv=y*o_img->widthStep + x*spp;
//			for(c=0; c<spp; c++){
//				o_img->imageData[i_cv+c] = (U)in_img[i*spp+c];
//			}
//			if(in_alpha!=NULL){
//				o_alpha->imageData[y*o_alpha->widthStep + x] = in_alpha[i];
//			}
//		}
//	}
//
//	return WSP_STATE_SUCCESS;
//}

template<typename T>
wsp::State wsp::img::cvArray2IplImage(IplImage *o_img, IplImage *o_alpha, const T *in_img, const uchar *in_alpha, uint width, uint height, uint nChannel)
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
wsp::State wsp::img::cvIplImage2Array(T *o_img, uchar *o_alpha, const IplImage *in_img, const IplImage *in_alpha)
{
    if (o_img == NULL){ WSPERROR("Destination image is NULL\n"); return WSP_STATE_FAILURE; }

    uint w = in_img->width, h = in_img->height, spp = in_img->nChannels;
    if ((in_alpha != NULL && (in_alpha->width != (int)w || in_alpha->height != (int)h))){
        WSPERROR("Image and alpha map size are different\n"); return WSP_STATE_FAILURE;
    }

    uint x, y, c, i, i_cv;
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

template<typename T> wsp::State wsp::img::cvWspImg2IplImage(IplImage *o_img, IplImage *o_alpha, const wsp::Image<T> &in_img)
{
    return wsp::img::cvArray2IplImage<T>(o_img, o_alpha, in_img.data(), in_img.alpha_data(), in_img.width(), in_img.height(), in_img.num_of_channels());
}
template<typename T> wsp::State wsp::img::cvIplImage2WspImg(wsp::Image<T> &o_img, const IplImage *in_img, const IplImage *in_alpha)
{
    return wsp::img::cvIplImage2Array<T>(o_img.data(), o_img.alpha_data(), in_img, in_alpha);
}


#endif

#ifdef USE_QT
template<typename T>
wsp::State wsp::img::qtArray2QImage(QImage &o_rgb, QImage *o_alpha, const T *in_img, const uchar *in_alpha, uint width, uint height, uint nChannel)
{
    uint pix_r, pix_g, pix_b;
    int w, h, len, x, y, i;

    w = (int)width;
    h = (int)height;
    len = w*h;
    int alpha;
    int value;

    if (w != o_rgb.width() || h != o_rgb.height() || w == 0 || h == 0){
        WSPERROR("Sizes of input and output image are different or 0: %d %d, %d %d\n", w, h, o_rgb.width(), o_rgb.height());
        return WSP_STATE_FAILURE;
    }
    if (o_alpha != NULL && in_alpha != NULL){
        if (w != o_alpha->width() || h != o_alpha->height()){
            WSPERROR("Sizes of input and output image are different or 0.\n");
            return WSP_STATE_FAILURE;
        }
    }

    switch (nChannel){
    case 1:
        for (y = 0; y<h; y++){
            for (x = 0; x<w; x++){
                i = y*w + x;
                value = (int)in_img[i];
                o_rgb.setPixel(x, y, qRgb(value, value, value));
                if (o_alpha != NULL){
                    alpha = (int)in_alpha[i];
                    o_alpha->setPixel(x, y, qRgb(alpha, alpha, alpha));
                }
            }
        }
        break;
    case 3:
        for (y = 0; y<h; y++){
            for (x = 0; x<w; x++){
                i = y*w + x;
                o_rgb.setPixel(x, y, qRgb(in_img[i * 3], in_img[i * 3 + 1], in_img[i * 3 + 2]));
                if (o_alpha != NULL){
                    alpha = (int)in_alpha[i];
                    o_alpha->setPixel(x, y, qRgb(alpha, alpha, alpha));
                }
            }
        }
        break;
    default:
        WSPERROR("Number of channel is invalid: %d\n", nChannel);
        return WSP_STATE_FAILURE;
    }

    return WSP_STATE_SUCCESS;
}
template<typename T>
wsp::State wsp::img::qtQImage2Array(T *o_img, uchar *o_alpha, const QImage &in_img, const QImage *in_alpha, uint nChannel)
{
    unsigned int pix_r, pix_g, pix_b;
    uint w, h, x, y;
    QColor pix;
    T value;
    uchar alpha;

    w = in_img.width();
    h = in_img.height();

    if (w == 0 || h == 0){
        WSPERROR("Sizes of input and output image are different or 0.\n");
        return WSP_STATE_FAILURE;
    }
    if (in_alpha != NULL && o_alpha != NULL){
        if (w != in_alpha->width() || h != in_alpha->height()){
            WSPERROR("Sizes of input and output image are different or 0.\n");
            return WSP_STATE_FAILURE;
        }
    }

    o_rgba.SetSize((uint)w, (uint)h);

    for (y = 0; y<h; y++){
        for (x = 0; x<w; x++){
            pix.setRgb(in_img.pixel(x, y));
            value = pix;
            o_rgba.SetElem(WspRgb(pix.red(), pix.green(), pix.blue()), (uint)x, (uint)y);
            if (in_alpha != NULL){
                alpha = (uchar)in_alpha->pixel(x, y).r;
                o_alpha[i] = alpha;
            }
        }
    }

    return WSP_STATE_SUCCESS;
}
template<typename T> wsp::State wsp::img::qtWspImg2QImage(QImage &o_rgb, QImage *o_alpha, const wsp::Image<T> &in_img)
{
    return wsp::img::qtArray2QImage(o_rgb, o_alpha, in_img.data(), in_img.alpha_data(), in_img.width(), in_img.height(), in_img.num_of_channels());
}
template<typename T> wsp::State wsp::img::qtQImage2WspImg(wsp::Image<T> &o_img, const QImage &in_img, const QImage *in_alpha, uint nChannels)
{
    return wsp::img::qtQImage2Array(o_img.data(), o_img.alpha_data(), in_img, in_alpha, nChannels);
}

#endif


// ----------------------------------------------------------------


#ifdef USE_OPENCV
template<typename T> wsp::State wsp::img::cvInpaint(T *io_img, const uchar *in_mask, int width, int height, int nChannels, int algType)
{
    if (io_img == NULL){
        WSPERROR("Input image is NULL\n");
        return WSP_STATE_FAILURE;
    }

    int w = (int)width, h = (int)height;
    int x, y;
    int len = (uint)(w*h);
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

    wsp::img::cvArray2IplImage<T>(src_img, mask_img, io_img, in_mask, width, height, nChannels);

    cvInpaint(src_img, mask_img, dst_img, 10.0, algType);

    //cvNamedWindow ("src", CV_WINDOW_AUTOSIZE);
    //cvNamedWindow ("dst", CV_WINDOW_AUTOSIZE);
    //cvShowImage ("src", src_img);
    //cvShowImage ("dst", dst_img);
    //cvWaitKey (0);

    //cvDestroyWindow ("src");
    //cvDestroyWindow ("dst");

    wsp::img::cvIplImage2Array<T>(io_img, NULL, dst_img, NULL);

    cvReleaseImage(&src_img);
    cvReleaseImage(&mask_img);
    cvReleaseImage(&dst_img);

    return WSP_STATE_SUCCESS;
}
template<typename T>
wsp::State wsp::img::cvInpaintAlpha(T *io_img, const uchar *in_alpha, int width, int height, int nChannels, int algType)
{
    uint len = width*height;
    uchar *mask = new uchar[len];
    wsp::State state;
    for (uint i = 0; i<len; i++){
        mask[i] = 255 - in_alpha[i];
    }
    state = wsp::img::cvInpaint(io_img, mask, width, height, nChannels, algType);
    delete[] mask;
    return state;
}
template<typename T>
wsp::State wsp::img::cvInpaintState(T *io_img, const uchar *in_state, int width, int height, int nChannels,
    wsp::img::PixelState target_state, int algType)
{
    uint len = width*height;
    uchar *mask = new uchar[len];
    wsp::State state;
    for (uint i = 0; i<len; i++){
        if (in_state[i] == target_state){
            mask[i] = 255; continue;
        }
        mask[i] = 0;
    }
    state = wsp::img::cvInpaint(io_img, mask, width, height, nChannels, algType);
    delete[] mask;
    return state;
}

template<typename T> inline wsp::State wsp::img::cvInpaintNavierStokes(T *io_img, const uchar *in_mask, int width, int height, int nChannels){
    return wsp::img::cvInpaint(io_img, in_mask, width, height, nChannels, CV_INPAINT_NS);
}
template<typename T> inline wsp::State wsp::img::cvInpaintAlexandruTelea(T *io_img, const uchar *in_mask, int width, int height, int nChannels){
    return wsp::img::cvInpaint(io_img, in_mask, width, height, nChannels, CV_INPAINT_TELEA);
}

template<typename T> inline wsp::State wsp::img::cvInpaintAlphaNavierStokes(T *io_img, const uchar *in_alpha, int width, int height, int nChannels){
    return wsp::img::cvInpaintAlpha(io_img, in_alpha, width, height, nChannels, CV_INPAINT_NS);
}
template<typename T> inline wsp::State wsp::img::cvInpaintAlphaAlexandruTelea(T *io_img, const uchar *in_alpha, int width, int height, int nChannels){
    return wsp::img::cvInpaintAlpha(io_img, in_alpha, width, height, nChannels, CV_INPAINT_TELEA);
}

template<typename T> inline wsp::State wsp::img::cvInpaintStateNavierStokes(T *io_img, const uchar *in_state, int width, int height, int nChannels, wsp::img::PixelState target_state){
    return wsp::img::cvInpaintState(io_img, in_state, width, height, nChannels, target_state, CV_INPAINT_NS);
}
template<typename T> inline wsp::State wsp::img::cvInpaintStateAlexandruTelea(T *io_img, const uchar *in_state, int width, int height, int nChannels, wsp::img::PixelState target_state){
    return wsp::img::cvInpaintState(io_img, in_state, width, height, nChannels, target_state, CV_INPAINT_TELEA);
}

#endif

// ==============================
//! Segmentation 
// ==============================
