/**
 * @file gpl_tpl_fn-imgproc.inl
 * @author Jun-ichi Nishikata
 */

#ifndef _WSP_IMAGE_TEMPLATE_FUNCTION_IMG_PROC_GPL_INL_
#define _WSP_IMAGE_TEMPLATE_FUNCTION_IMG_PROC_GPL_INL_

#ifdef USE_QT
#include <QtGui/qimage.h>
#include <QtGui/qrgb.h>
#endif

#include "tpl_cl-image.h"
#include "gpl_tpl_fn-imgproc.h"

//! Resize -----------------------------------------------------
#ifdef USE_OPENCV
template<typename T> wsp::State wsp::img::cvResizeU8Image(wsp::Image<T> &io_img, u32 width, u32 height, int algorithm)
{
    int w=(int)io_img.width(), h=(int)io_img.height();
    int nChannels=(int)io_img.num_of_channels() * sizeof(T);
    int x, y;
    int len = (u32)(w*h);
    int i, i_cv;
    IplImage *src_img=NULL, *dst_mask=NULL, *dst_img=NULL, *src_mask=NULL;

    src_img = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, (int)nChannels);
    dst_img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, (int)nChannels);
    src_mask= cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
    dst_mask= cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);

    //printf("w:%d, h:%d, widthStep:%d\n", src_img->width, src_img->height, src_img->widthStep);

    wsp::img::cvConvertArrayToIplImage<uchar>(src_img, src_mask, (uchar*)io_img.data(), io_img.alpha_data(), w, h, nChannels);

    cvResize(src_img, dst_img, algorithm);
    cvResize(src_mask,dst_mask,algorithm);

    io_img.SetSize(width, height, io_img.num_of_channels());

    wsp::img::cvConvertIplImageToArray<uchar>((uchar*)io_img.data(), io_img.alpha_data(), dst_img, dst_mask);

    cvReleaseImage(&src_img);
    cvReleaseImage(&src_mask);
    cvReleaseImage(&dst_img);
    cvReleaseImage(&dst_mask);

    return WSP_STATE_SUCCESS;
}

template<typename T> wsp::State wsp::img::cvResizeImage(wsp::Image<T> &io_img, u32 width, u32 height, int algorithm)
{
    int w=(int)io_img.width(), h=(int)io_img.height();
    int nChannels=(int)io_img.num_of_channels();
    int x, y;
    int len = (u32)(w*h);
    int i, i_cv;
    IplImage *src_img=NULL, *dst_mask=NULL, *dst_img=NULL, *src_mask=NULL;

    int depthType;
    switch(sizeof(T)){
        case 1: depthType=IPL_DEPTH_8U; break;
        case 4: depthType=IPL_DEPTH_32F; break;
        case 8: depthType=IPL_DEPTH_64F; break;
        default: WSP_COMMON_ERROR_LOG("Data type of image is invalid\n"); return WSP_STATE_FAILURE;
    }

    src_img = cvCreateImage(cvSize(w, h), depthType, (int)nChannels);
    dst_img = cvCreateImage(cvSize(width, height), depthType, (int)nChannels);
    src_mask= cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
    dst_mask= cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);

    //printf("w:%d, h:%d, widthStep:%d\n", src_img->width, src_img->height, src_img->widthStep);

    //switch(sizeof(T)){
    //    case 1:
    //        wsp::img::cvConvertArrayToIplImage<T, uchar>(src_img, src_mask, io_img.data(), io_img.alpha_data(), w, h, nChannels);
    //        break;
    //    case 4:
    //        wsp::img::cvConvertArrayToIplImage<T, float>(src_img, src_mask, io_img.data(), io_img.alpha_data(), w, h, nChannels);
    //        break;
    //    case 8:
    //        wsp::img::cvConvertArrayToIplImage<T, double>(src_img, src_mask, io_img.data(), io_img.alpha_data(), w, h, nChannels);
    //        break;
    //    default: WSP_COMMON_ERROR_LOG("Data type of image is invalid\n"); return WSP_STATE_FAILURE;
    //}
    wsp::img::cvConvertArrayToIplImage<T>(src_img, src_mask, io_img.data(), io_img.alpha_data(), w, h, nChannels);


    cvResize(src_img, dst_img, algorithm);
    cvResize(src_mask,dst_mask,algorithm);

    io_img.SetSize(width, height, io_img.num_of_channels());

    wsp::img::cvConvertIplImageToArray<T>(io_img.data(), io_img.alpha_data(), dst_img, dst_mask);

    cvReleaseImage(&src_img);
    cvReleaseImage(&src_mask);
    cvReleaseImage(&dst_img);
    cvReleaseImage(&dst_mask);

    return WSP_STATE_SUCCESS;
}
template<typename T> wsp::State wsp::img::cvResizeImageNearestNeighbor(wsp::Image<T> &io_img, u32 newWidth, u32 newHeight){
    return wsp::img::cvResizeImage(io_img, newWidth, newHeight, CV_INTER_NN);
}
template<typename T> wsp::State wsp::img::cvResizeImageLinear(wsp::Image<T> &io_img, u32 newWidth, u32 newHeight){
    return wsp::img::cvResizeImage(io_img, newWidth, newHeight, CV_INTER_LINEAR);
}
template<typename T> wsp::State wsp::img::cvResizeImageArea(wsp::Image<T> &io_img, u32 newWidth, u32 newHeight){
    return wsp::img::cvResizeImage(io_img, newWidth, newHeight, CV_INTER_AREA);
}
template<typename T> wsp::State wsp::img::cvResizeImageBicubic(wsp::Image<T> &io_img, u32 newWidth, u32 newHeight){
    return wsp::img::cvResizeImage(io_img, newWidth, newHeight, CV_INTER_CUBIC);
}

#endif

//! Conversion -----------------------------------------------------
#ifdef USE_OPENCV
//template<typename T, typename U>
//wsp::State wsp::img::cvConvertArrayToIplImage(IplImage *o_img, IplImage *o_alpha, const T *in_img, const uchar *in_alpha, u32 width, u32 height, u32 nChannel)
//{
//    int w=(int)width, h=(int)height, spp=(int)nChannel;
//    if(o_img->width!=w || o_img->height!=h || (o_alpha!=NULL && (o_alpha->width!=w || o_alpha->height!=h))){
//        WSP_COMMON_ERROR_LOG("Image sizes of input and output images are different\n");
//        return WSP_STATE_FAILURE;
//    }
//
//    int x, y, c, i, i_cv;
//    for(y=0; y<h; y++){
//        for(x=0; x<w; x++){
//            i=y*w + x;
//            i_cv=y*o_img->widthStep + x*spp;
//            for(c=0; c<spp; c++){
//                o_img->imageData[i_cv+c] = (U)in_img[i*spp+c];
//            }
//            if(in_alpha!=NULL){
//                o_alpha->imageData[y*o_alpha->widthStep + x] = in_alpha[i];
//            }
//        }
//    }
//
//    return WSP_STATE_SUCCESS;
//}

template<typename _InType, typename _OutType>
void CopyIntoIplImage(
    const _InType *in_img,
    IplImage *o_img,
    s32 width, s32 height, s32 num_channels,
    bool invert_byte_order = false
)
{
    s32 x, y, i, i_cv, c;
    _OutType *data;
    if( invert_byte_order )
    {
        for(y=0; y<height; ++y)
        {
            for(x=0; x<width; ++x)
            {
                i = (y*width + x)*num_channels;
                i_cv = y*o_img->widthStep + x*sizeof(_OutType)*num_channels;
                data = reinterpret_cast<_OutType*>(&o_img->imageData[i_cv]);
                for(c=0; c<num_channels; c++)
                {
                    /* Invert byte order */
                    data[num_channels-c-1] = (_OutType)in_img[i+c];
                }
            }
        }
    }
    else
    {
        for(y=0; y<height; ++y)
        {
            for(x=0; x<width; ++x)
            {
                i = (y*width + x)*num_channels;
                i_cv = y*o_img->widthStep + x*sizeof(_OutType)*num_channels;
                data = reinterpret_cast<_OutType*>(&o_img->imageData[i_cv]);
                for(c=0; c<num_channels; c++)
                {
                    data[c] = (_OutType)in_img[i+c];
                }
            }
        }
    }
}
template<typename _InType, typename _OutType>
void CopyFromCvMat(
    const CvMat *in_img,
    _OutType *o_img,
    s32 width, s32 height, s32 num_channels,
    bool invert_byte_order=false
)
{
    s32 spp = CV_MAT_CN(in_img->type);
    assert(in_img->width == width);
    assert(in_img->height== height);
    assert(spp==num_channels);

    s32 x, y, i, i_cv, c;
    _InType *src_data;
    _OutType *data;
    if( invert_byte_order )
    {
        for(y=0; y<height; ++y)
        {
            for(x=0; x<width; ++x)
            {
                i = (y*width + x)*num_channels;
                i_cv = y*in_img->step + x*sizeof(_InType)*spp;
                src_data = (_InType*)(&in_img->data.ptr[i_cv]);
                data = &o_img[i];
            
                for(c=0; c<num_channels; ++c)
                {
                    /* Invert byte order */
                    data[num_channels-c-1] = (_OutType)src_data[c];
                }
            }
        }
    }
    else
    {
        for(y=0; y<height; ++y)
        {
            for(x=0; x<width; ++x)
            {
                i = (y*width + x)*num_channels;
                i_cv = y*in_img->step + x*sizeof(_InType)*spp;
                src_data = (_InType*)(&in_img->data.ptr[i_cv]);
                data = &o_img[i];
            
                for(c=0; c<num_channels; ++c)
                {
                    data[c] = (_OutType)src_data[c];
                }
            }
        }
    }
}
template<typename T>
wsp::State wsp::img::cvConvertArrayToIplImage(
    IplImage *o_img, 
    IplImage *o_alpha, 
    const T *in_img, 
    const uchar *in_alpha, 
    u32 width, u32 height, u32 nChannel,
    bool invert_byte_order
)
{
    int w=(int)width, h=(int)height, spp=(int)nChannel;
    if(o_img->width!=w || o_img->height!=h || (o_alpha!=NULL && (o_alpha->width!=w || o_alpha->height!=h))){
        WSP_COMMON_ERROR_LOG("Image sizes of input and output images are different\n");
        return WSP_STATE_FAILURE;
    }
    WSP_COMMON_DEBUG_LOG("%d == %d\n", o_img->nChannels, nChannel );
    assert( o_img->nChannels == nChannel );
    WSP_COMMON_DEBUG_LOG("%d == %d\n", o_img->width, width );
    assert( o_img->width == width );
    WSP_COMMON_DEBUG_LOG("%d == %d\n", o_img->height, height );
    assert( o_img->height== height );

    //printf("cvConvertArrayToIplImage: widthStep=%d\n", o_img->widthStep);

    switch( o_img->depth )
    {
    case IPL_DEPTH_8U:  CopyIntoIplImage<T, u8> ( in_img, o_img, w, h, spp, invert_byte_order ); break;
    case IPL_DEPTH_8S:  CopyIntoIplImage<T, s8> ( in_img, o_img, w, h, spp, invert_byte_order ); break;
    case IPL_DEPTH_16U: CopyIntoIplImage<T, u16>( in_img, o_img, w, h, spp, invert_byte_order ); break;
    case IPL_DEPTH_16S: CopyIntoIplImage<T, s16>( in_img, o_img, w, h, spp, invert_byte_order ); break;
    case IPL_DEPTH_32S: CopyIntoIplImage<T, s32>( in_img, o_img, w, h, spp, invert_byte_order ); break;
    case IPL_DEPTH_32F: CopyIntoIplImage<T, f32>( in_img, o_img, w, h, spp, invert_byte_order ); break;
    case IPL_DEPTH_64F: CopyIntoIplImage<T, f64>( in_img, o_img, w, h, spp, invert_byte_order ); break;
    default:
        WSP_COMMON_ERROR_LOG("Unsupported data type\n");
        return WSP_STATE_FAILURE;
    }
    if(in_alpha!=NULL && o_alpha!=NULL){
        CopyIntoIplImage<u8, u8>( in_alpha, o_alpha, w, h, 1 );
    }
    return WSP_STATE_SUCCESS;
}
template<typename T>
wsp::State wsp::img::cvConvertIplImageToArray(
    T *o_img, 
    uchar *o_alpha, 
    const IplImage *in_img, 
    const IplImage *in_alpha
)
{
    if(o_img==NULL){ WSP_COMMON_ERROR_LOG("Destination image is NULL\n"); return WSP_STATE_FAILURE; }

    u32 w=in_img->width, h=in_img->height, spp=in_img->nChannels;
    if((in_alpha!=NULL && (in_alpha->width!=(int)w || in_alpha->height!=(int)h))){
        WSP_COMMON_ERROR_LOG("Image and alpha map size are different\n"); return WSP_STATE_FAILURE;
    }

    u32 x, y, c, i, i_cv;
    T *data;
    //T *data = (T*)in_img->imageData;
    for(y=0; y<h; y++){
        for(x=0; x<w; x++){
            i = y*w + x;
            i_cv = y*in_img->widthStep + x*sizeof(T)*spp;
            //i_cv = (y*w + x) * spp;
            data = reinterpret_cast<T*>(&in_img->imageData[i_cv]);
            for(c=0; c<spp; c++){
                o_img[i*spp+c] = data[c];
            }
            if(in_alpha!=NULL){
                o_alpha[i] = in_alpha->imageData[y*in_alpha->widthStep + x];
            }
        }
    }

    return WSP_STATE_SUCCESS;
}

template<typename T> 
wsp::State wsp::img::cvConvertCvMatToArray(
    const CvMat *in_img, 
    const CvMat *in_alpha,
    T *o_img, 
    uchar *o_alpha,
    s32 width, s32 height, s32 num_channels,
    bool invert_byte_order
)
{
    if(o_img==NULL){ WSP_COMMON_ERROR_LOG("Destination image is NULL\n"); return WSP_STATE_INVALID_PARAMETER; }

    u32 w=in_img->cols, h=in_img->rows, spp=CV_MAT_CN(in_img->type);
    if((s32)w!=width || (s32)h!=height || (s32)spp!=num_channels){
        WSP_COMMON_ERROR_LOG("Input and output image sizes are different\n"); return WSP_STATE_INVALID_PARAMETER;
    }
    if((in_alpha!=NULL && (in_alpha->width!=(int)w || in_alpha->height!=(int)h ))){
        WSP_COMMON_ERROR_LOG("Image and alpha map size are different\n"); return WSP_STATE_INVALID_PARAMETER;
    }

    switch( CV_MAT_TYPE(in_img->type) )
    {
    case CV_8UC1: case CV_8UC2: case CV_8UC3: case CV_8UC4:
        CopyFromCvMat<u8, T>( in_img, o_img, w, h, spp, invert_byte_order ); 
        break;
    case CV_8SC1: case CV_8SC2: case CV_8SC3: case CV_8SC4: 
        CopyFromCvMat<s8, T>( in_img, o_img, w, h, spp, invert_byte_order ); 
        break;
    case CV_16UC1: case CV_16UC2: case CV_16UC3: case CV_16UC4: 
        CopyFromCvMat<u16, T>( in_img, o_img, w, h, spp, invert_byte_order ); 
        break;
    case CV_16SC1: case CV_16SC2: case CV_16SC3: case CV_16SC4: 
        CopyFromCvMat<s16, T>( in_img, o_img, w, h, spp, invert_byte_order ); 
        break;
    case CV_32SC1: case CV_32SC2: case CV_32SC3: case CV_32SC4: 
        CopyFromCvMat<s32, T>( in_img, o_img, w, h, spp, invert_byte_order ); 
        break;
    case CV_32FC1: case CV_32FC2: case CV_32FC3: case CV_32FC4: 
        CopyFromCvMat<f32, T>( in_img, o_img, w, h, spp, invert_byte_order ); 
        break;
    case CV_64FC1: case CV_64FC2: case CV_64FC3: case CV_64FC4: 
        CopyFromCvMat<f64, T>( in_img, o_img, w, h, spp, invert_byte_order ); 
        break;
    default:
        WSP_COMMON_ERROR_LOG("Unsupported data type: %d\n", in_img->type);
        return WSP_STATE_FAILURE;
    }
    if(in_alpha!=NULL && o_alpha!=NULL){
        CopyFromCvMat<u8, u8>( in_alpha, o_alpha, w, h, 1 );
    }

    return WSP_STATE_SUCCESS;
}


template<typename T> 
wsp::State wsp::img::cvConvertWspImageToIplImage(
    IplImage *o_img, 
    IplImage *o_alpha,
    const wsp::Image<T> &in_img,
    bool invert_byte_order
)
{
    return wsp::img::cvConvertArrayToIplImage<T>(o_img, o_alpha, in_img.data(), in_img.alpha_data(), in_img.width(), in_img.height(), in_img.num_of_channels(), invert_byte_order);
}
template<typename T> 
wsp::State wsp::img::cvConvertIplImageToWspImage(
    wsp::Image<T> &o_img, 
    const IplImage *in_img, 
    const IplImage *in_alpha
)
{
    return wsp::img::cvConvertIplImageToArray<T>(o_img.data(), o_img.alpha_data(), in_img, in_alpha);
}


#endif

#ifdef USE_QT
template<typename T>
wsp::State wsp::img::qtArrayToQImage(QImage &o_rgb, QImage *o_alpha, const T *in_img, const uchar *in_alpha, u32 width, u32 height, u32 nChannel)
{
    int w, h, len, x, y, i;


    w = (int)width;
    h = (int)height;
    len = w*h;
    int alpha;
    int value;

    switch(nChannel)
    {
    case 1:
        o_rgb = QImage( w, h, QImage::Format_Indexed8 );
        break;
    case 3:
        o_rgb = QImage( w, h, QImage::Format_RGB32 );
        break;
    case 4:
        o_rgb = QImage( w, h, QImage::Format_ARGB32 );
        break;
    default:
        return WSP_STATE_UNSUPPORTED_TYPE;
    }
    if( o_alpha!=NULL )
    {
        *o_alpha = QImage( w, h, QImage::Format_Indexed8 );
    }

    if(w!=o_rgb.width() || h!=o_rgb.height() || w==0 || h==0){
        WSP_COMMON_ERROR_LOG("Sizes of input and output image are different or 0: %d %d, %d %d\n", w, h, o_rgb.width(), o_rgb.height());
        return WSP_STATE_FAILURE;
    }
    if(o_alpha!=NULL && in_alpha!=NULL){
        if(w!=o_alpha->width() || h!=o_alpha->height()){
            WSP_COMMON_ERROR_LOG("Sizes of input and output image are different or 0.\n");
            return WSP_STATE_FAILURE;
        }
    }

    switch(nChannel)
    {
    case 1:
        for(y=0; y<h; y++){
            for(x=0; x<w; x++){
                i = y*w + x;
                value = (int)in_img[i];
                o_rgb.setPixel(x, y, qRgb(value, value, value));
                if(o_alpha!=NULL){
                    alpha=(int)in_alpha[i];
                    o_alpha->setPixel(x, y, qRgb(alpha, alpha, alpha));
                }
            }
        }
        break;
    case 3:
        for(y=0; y<h; y++){
            for(x=0; x<w; x++){
                i = y*w + x;
                o_rgb.setPixel(x, y, qRgb(in_img[i*3], in_img[i*3+1], in_img[i*3+2]));
                if(o_alpha!=NULL){
                    alpha=(int)in_alpha[i];
                    o_alpha->setPixel(x, y, qRgb(alpha, alpha, alpha));
                }
            }
        }
        break;
    default:
        WSP_COMMON_ERROR_LOG("Number of channel is invalid: %d\n", nChannel);
        return WSP_STATE_FAILURE;
    }

    return WSP_STATE_SUCCESS;
}

namespace{
    template<typename _InType, typename _OutType>
    wsp::State CopyQImageToArray( 
                _OutType *o_array, 
                uchar *o_alpha, 
                u32 width, u32 height, u32 num_of_channels,
                const QImage &in_img
    )
    {
        int byte_per_channels = sizeof(_InType);
        int wstep = in_img.bytesPerLine();
        int src_nc = wsp::img::qtGetNumOfChannels( in_img );
        u32 x, y, c, i, i_src;
        const _InType *data;
        int min_nc = num_of_channels;
        if( min_nc > src_nc )
        {
            min_nc = src_nc;
        }

        switch( in_img.format() )
        {
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied:
        case QImage::Format_ARGB4444_Premultiplied:
        case QImage::Format_ARGB6666_Premultiplied:
        case QImage::Format_ARGB8555_Premultiplied:
        case QImage::Format_ARGB8565_Premultiplied:
            {
                for(y=0; y<height; ++y)
                {
                    for(x=0; x<width; ++x)
                    {
                        i = y*width + x;
                        //i_src = y*wstep + x*sizeof(_InType)*src_nc;
                        QRgb argb = in_img.pixel( x, y );
                        data = reinterpret_cast<_InType*>( &argb );
                        for(c=0; c<min_nc; ++c)
                        {
                            o_array[i*num_of_channels+c] = static_cast<_OutType>(data[1 + c]);
                        }
                        if(o_alpha!=NULL)
                        {
                            o_alpha[i] = data[0];
                        }
                    }
                }
            }
            break;
        case QImage::Format_Mono:
        case QImage::Format_MonoLSB:
        case QImage::Format_Indexed8:
        case QImage::Format_RGB32:
        case QImage::Format_RGB16:
        case QImage::Format_RGB444:
        case QImage::Format_RGB555:
        case QImage::Format_RGB888:
            {
                for(y=0; y<height; ++y)
                {
                    for(x=0; x<width; ++x)
                    {
                        i = y*width + x;
                        //i_src = y*wstep + x*sizeof(_InType)*src_nc;
                        QRgb rgb = in_img.pixel( x, y );
                        data = reinterpret_cast<_InType*>( &rgb );
                        for(c=0; c<min_nc; ++c)
                        {
                            o_array[i*num_of_channels+c] = static_cast<_OutType>(data[c]);
                        }
                    }
                }
                if(o_alpha!=NULL)
                {
                    memset( o_alpha, 0xff, width * height );
                }
            }
            break;
        case QImage::Format_Invalid:
        default:
            return WSP_STATE_UNSUPPORTED_TYPE;
        }
        return WSP_STATE_SUCCESS;
    }
}

template<typename T>
wsp::State wsp::img::qtQImageToArray(
                        T *o_array, 
                        uchar *o_alpha, 
                        u32 width, u32 height, u32 num_of_channels,
                        const QImage &in_img
)
{
    u32 w = (u32)in_img.width();
    u32 h = (u32)in_img.height();

    if(w==0 || h==0 || w!=width || h!=height )
    {
        WSP_COMMON_ERROR_LOG("Sizes of input and output image are different or 0.\n");
        return WSP_STATE_INVALID_ARGUMENT;
    }

    WSP_COMMON_DEBUG_LOG("format=%s\n", wsp::img::qtGetFormatAsString(in_img.format()));

    switch( in_img.format() )
    {
    case QImage::Format_Indexed8:
        return CopyQImageToArray<u8, T>( o_array, o_alpha, width, height, num_of_channels, in_img );
    case QImage::Format_ARGB32:
        return CopyQImageToArray<u8, T>( o_array, o_alpha, width, height, num_of_channels, in_img );
    case QImage::Format_RGB32:
        return CopyQImageToArray<u8, T>( o_array, o_alpha, width, height, num_of_channels, in_img );
    case QImage::Format_ARGB32_Premultiplied:
    case QImage::Format_ARGB4444_Premultiplied:
    case QImage::Format_ARGB6666_Premultiplied:
    case QImage::Format_ARGB8555_Premultiplied:
    case QImage::Format_ARGB8565_Premultiplied:
    case QImage::Format_Invalid:
    case QImage::Format_Mono:
    case QImage::Format_MonoLSB:
    case QImage::Format_RGB16:
    case QImage::Format_RGB444:
    case QImage::Format_RGB555:
    case QImage::Format_RGB888:
    default:
        return WSP_STATE_UNSUPPORTED_TYPE;
    }

    return WSP_STATE_SUCCESS;
}
template<typename T> wsp::State wsp::img::qtWspImageToQImage(QImage &o_rgb, QImage *o_alpha, const wsp::Image<T> &in_img)
{
    return wsp::img::qtArrayToQImage(o_rgb, o_alpha, in_img.data(), in_img.alpha_data(), in_img.width(), in_img.height(), in_img.num_of_channels());
}
template<typename T> wsp::State wsp::img::qtQImageToWspImage(wsp::Image<T> &o_img, const QImage &in_img)
{
    int w  = in_img.width();
    int h  = in_img.height();
    int nc = wsp::img::qtGetNumOfChannelsExceptAlpha( in_img );
    o_img.SetSize( w, h, nc );
    return wsp::img::qtQImageToArray(o_img.data(), o_img.alpha_data(), w, h, nc, in_img);
}

#endif


// ----------------------------------------------------------------


#ifdef USE_OPENCV
template<typename T> wsp::State wsp::img::cvInpaint(T *io_img, const uchar *in_mask, int width, int height, int nChannels, int algType)
{
    if(io_img==NULL){
        WSP_COMMON_ERROR_LOG("Input image is NULL\n");
        return WSP_STATE_FAILURE;
    }

    int w=(int)width, h=(int)height;
    int x, y;
    int len = (u32)(w*h);
    int i, i_cv;
    IplImage *src_img=NULL, *mask_img=NULL, *dst_img;

    int depthType;
    switch(sizeof(T)){
        case 1: depthType=IPL_DEPTH_8U; break;
        case 4: depthType=IPL_DEPTH_32F; break;
        case 8: depthType=IPL_DEPTH_64F; break;
        default: WSP_COMMON_ERROR_LOG("Data type of image is invalid\n"); return WSP_STATE_FAILURE;
    }

    src_img = cvCreateImage(cvSize(w, h), depthType, (int)nChannels);
    dst_img = cvCreateImage(cvSize(w, h), depthType, (int)nChannels);
    mask_img= cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);

    //printf("w:%d, h:%d, ws:%d", src_img->width, src_img->height, src_img->widthStep);

    //WspFnImgIO::SaveImage24(io_img, NULL, width, height, "test_beforeInpaint.tif");

    wsp::img::cvConvertArrayToIplImage<T>(src_img, mask_img, io_img, in_mask, width, height, nChannels);

    cvInpaint(src_img, mask_img, dst_img, 10.0, algType);

    //cvNamedWindow ("src", CV_WINDOW_AUTOSIZE);
    //cvNamedWindow ("dst", CV_WINDOW_AUTOSIZE);
    //cvShowImage ("src", src_img);
    //cvShowImage ("dst", dst_img);
    //cvWaitKey (0);

    //cvDestroyWindow ("src");
    //cvDestroyWindow ("dst");

    wsp::img::cvConvertIplImageToArray<T>(io_img, NULL, dst_img, NULL);

    cvReleaseImage(&src_img);
    cvReleaseImage(&mask_img);
    cvReleaseImage(&dst_img);

    return WSP_STATE_SUCCESS;
}
template<typename T>
wsp::State wsp::img::cvInpaintAlpha(T *io_img, const uchar *in_alpha, int width, int height, int nChannels, int algType)
{
    u32 len=width*height;
    uchar *mask=new uchar[len];
    wsp::State state;
    for(u32 i=0; i<len; i++){
        mask[i]=255-in_alpha[i];
    }
    state=wsp::img::cvInpaint(io_img, mask, width, height, nChannels, algType);
    delete[] mask;
    return state;
}
template<typename T>
wsp::State wsp::img::cvInpaintState(T *io_img, const uchar *in_state, int width, int height, int nChannels,
                                            wsp::img::PixelState target_state, int algType)
{
    u32 len=width*height;
    uchar *mask=new uchar[len];
    wsp::State state;
    for(u32 i=0; i<len; i++){
        if(in_state[i]==target_state){
            mask[i] = 255; continue;
        }
        mask[i] = 0;
    }
    state=wsp::img::cvInpaint(io_img, mask, width, height, nChannels, algType);
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


template<typename T>
wsp::State wsp::img::cvGetSaliencyMap(
    const wsp::Image<T> &in_src,
    wsp::Image<T> &o_saliency_map
)
{
    char filename[MAX_LENGTH_OF_PATH];
    const int kNumOfSaliencyMapSrc = 9;
    wsp::Image<T> pyramid_color[kNumOfSaliencyMapSrc];
    wsp::Image<double> pyramid_intensity[kNumOfSaliencyMapSrc];
    wsp::Image<double> pyramid_chroma[kNumOfSaliencyMapSrc];
    wsp::Image<double> tmp_img;

    int src_w = in_src.width();
    int src_h = in_src.height();
    int nc = in_src.num_of_channels();

    /* Create pyramid image */
    int dst_w = src_w;
    int dst_h = src_h;
    int num_maps;
    for( num_maps=0; dst_w>2 && dst_h>2 && num_maps<kNumOfSaliencyMapSrc; ++num_maps, dst_w/=2, dst_h/=2 )
    {
        /* Scale down image */
        pyramid_color[num_maps].CopyFrom( in_src );
        wsp::img::cvResizeImageBicubic(
            pyramid_color[num_maps], dst_w, dst_h
        );

        /* Scale up image */
        wsp::img::cvResizeImageBicubic(
            pyramid_color[num_maps], src_w, src_h
        );

        /* Save source images */
        sprintf(filename, "src_saliencymap-%d.png", num_maps);
        pyramid_color[num_maps].Save(filename);
    }

    /* Get intensity maps */
    for( int i=0; i<num_maps; ++i )
    {
        /* Get intensity map */
        wsp::img::GetIntensityMap( pyramid_color[i], pyramid_intensity[i]);

        /* Save intensity maps */
        sprintf(filename, "intensity-%d.png", i);
        pyramid_intensity[i].Normalize(0, 255);
        pyramid_intensity[i].Save(filename);

        /* Get chroma map */
        wsp::img::GetChromaMap( pyramid_color[i], pyramid_chroma[i]);

        pyramid_chroma[i].Normalize(0, 255);

        tmp_img.CopyFrom( pyramid_chroma[i], 3 );
        sprintf(filename, "chroma_rgb-%d.png", i);
        tmp_img.Normalize(0, 255);
        tmp_img.Save(filename);

        /* Save red chroma map */
        tmp_img = pyramid_chroma[i];
        tmp_img.LeaveOneChannel(0);
        sprintf(filename, "chroma_r-%d.png", i);
        tmp_img.Normalize(0, 255);
        tmp_img.Save(filename);

        /* Save green chroma map */
        tmp_img = pyramid_chroma[i];
        tmp_img.LeaveOneChannel(1);
        sprintf(filename, "chroma_g-%d.png", i);
        tmp_img.Normalize(0, 255);
        tmp_img.Save(filename);

        /* Save blue chroma map */
        tmp_img = pyramid_chroma[i];
        tmp_img.LeaveOneChannel(2);
        sprintf(filename, "chroma_b-%d.png", i);
        tmp_img.Normalize(0, 255);
        tmp_img.Save(filename);

        /* Save yellow chroma map */
        tmp_img = pyramid_chroma[i];
        tmp_img.LeaveOneChannel(3);
        sprintf(filename, "chroma_y-%d.png", i);
        tmp_img.Normalize(0, 255);
        tmp_img.Save(filename);
    }

    {

    }

    return WSP_STATE_SUCCESS;
}

#endif


#endif
