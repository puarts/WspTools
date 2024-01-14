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

#endif
