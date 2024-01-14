/**
 * @file gpl_tpl_fn-imgproc.h
 * @@author Jun-ichi Nishikata
 */

#ifndef _WSP_IMAGE_TEMPLATE_FUNCTION_IMG_PROC_GPL_H_
#define _WSP_IMAGE_TEMPLATE_FUNCTION_IMG_PROC_GPL_H_

#ifndef __WSP_IMAGEUTIL_DEFINE_IMAGEUTIL_H__
#include "_define_image.h"
#endif
#include "_tpl_fn-imgproc.hpp"
#include "tpl_cl-image.h"

#ifdef USE_QT
    #include <QtGui/QtGui>
#endif

#ifdef USE_OPENCV
#include <opencv2/core/core.hpp>
#endif

namespace wsp{ namespace img{
    // ==============================
    //  Resize
    // ==============================
    #ifdef USE_OPENCV
    // 0 - CV_INTER_NEAREST 最近傍補間
    // 1 - CV_INTER_LINEAR バイリニア補間（デフォルト）
    // 2 - CV_INTER_AREA ピクセル領域の関係を利用したリサンプリング．画像を大幅に縮小する場合は，モアレを避けることができる良い手法です．しかし，画像を拡大する場合は， INTER_NEAREST メソッドと同様になります
    // 3 - CV_INTER_CUBIC 4x4 の近傍領域を利用するバイキュービック補間
    // 4 - CV_INTER_LANCZOS4 8x8 の近傍領域を利用する Lanczos法の補間
    template<typename T> 
    wsp::State cvResizeImage(
        wsp::Image<T> &io_img, 
        u32 newWidth, 
        u32 newHeight, 
        int algorithm=1
    );
    
    template<typename T> 
    wsp::State cvResizeU8Image(
        wsp::Image<T> &io_img, 
        u32 newWidth, 
        u32 newHeight, 
        int algorithm=1
    );
    
    template<typename T> 
    wsp::State cvResizeImageNearestNeighbor(
        wsp::Image<T> &io_img, 
        u32 newWidth, 
        u32 newHeight
    );
    
    template<typename T> 
    wsp::State cvResizeImageLinear(
        wsp::Image<T> &io_img, 
        u32 newWidth, 
        u32 newHeight
    );
    
    template<typename T> 
    wsp::State cvResizeImageArea(
        wsp::Image<T> &io_img, 
        u32 newWidth, 
        u32 newHeight
    );
    
    template<typename T> 
    wsp::State cvResizeImageBicubic(
        wsp::Image<T> &io_img, 
        u32 newWidth, 
        u32 newHeight
    );
    // ==============================
    //  Type Conversion
    // ==============================
    template<typename T, typename U> 
    wsp::State cvConvertArrayToIplImage(
        IplImage *o_img, 
        IplImage *o_alpha, 
        const T *in_img, 
        const uchar *in_alpha, 
        u32 width, 
        u32 height, 
        u32 nChannel
    );
    
    template<typename T> 
    wsp::State cvConvertArrayToIplImage(
        IplImage *o_img, 
        IplImage *o_alpha, 
        const T *in_img, 
        const uchar *in_alpha, 
        u32 width, 
        u32 height, 
        u32 nChannel,
        bool invert_byte_order=false
    );
    
    template<typename T> 
    wsp::State cvConvertIplImageToArray(
        T *o_img, 
        uchar *o_alpha, 
        const IplImage *in_img, 
        const IplImage *in_alpha
    );

    template<typename T> 
    wsp::State cvConvertCvMatToArray(
        const CvMat *in_img, 
        const CvMat *in_alpha,
        T *o_img, 
        uchar *o_alpha,
        s32 width, s32 height, s32 num_channels,
        bool invert_byte_order=false
    );


    template<typename T> 
    wsp::State cvConvertWspImageToIplImage(
        IplImage *o_img, 
        IplImage *o_alpha, 
        const wsp::Image<T> &in_img,
        bool invert_byte_order=false
    );
    
    template<typename T> 
    wsp::State cvConvertIplImageToWspImage(
        wsp::Image<T> &o_img, 
        const IplImage *in_img, 
        const IplImage *in_alpha
    );



    // ==============================
    //  Interpolation 
    // ==============================
    template<typename T> wsp::State cvInpaint(T *io_img, const uchar *in_mask, int width, int height, int nChannels, int algType);
    template<typename T> inline wsp::State cvInpaintNavierStokes(T *io_img, const uchar *in_mask, int width, int height, int nChannels);
    template<typename T> inline wsp::State cvInpaintAlexandruTelea(T *io_img, const uchar *in_mask, int width, int height, int nChannels);

    template<typename T> wsp::State cvInpaintAlpha(T *io_img, const uchar *in_alpha, int width, int height, int nChannels, int algType);
    template<typename T> inline wsp::State cvInpaintAlphaNavierStokes(T *io_img, const uchar *in_alpha, int width, int height, int nChannels);
    template<typename T> inline wsp::State cvInpaintAlphaAlexandruTelea(T *io_img, const uchar *in_alpha, int width, int height, int nChannels);

    template<typename T> wsp::State cvInpaintState(T *io_img, const uchar *in_state, int width, int height, int nChannels, wsp::img::PixelState target_state, int algType);
    template<typename T> inline wsp::State cvInpaintStateNavierStokes(T *io_img, const uchar *in_state, int width, int height, int nChannels, wsp::img::PixelState target_state);
    template<typename T> inline wsp::State cvInpaintStateAlexandruTelea(T *io_img, const uchar *in_state, int width, int height, int nChannels, wsp::img::PixelState target_state);

    /* =================================
            Other
       =================================*/
    template<typename T>
    wsp::State cvGetSaliencyMap(
        const wsp::Image<T> &in_src,
        wsp::Image<T> &o_saliency_map
    );

    #endif

    #ifdef USE_QT
    inline int qtGetNumOfChannels( const QImage &in_img )
    {
        switch( in_img.format() )
        {
        case QImage::Format_Indexed8:
        case QImage::Format_Mono:
        case QImage::Format_MonoLSB:
            return 1;
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied:
        case QImage::Format_ARGB4444_Premultiplied:
        case QImage::Format_ARGB6666_Premultiplied:
        case QImage::Format_ARGB8555_Premultiplied:
        case QImage::Format_ARGB8565_Premultiplied:
            return 4;
        case QImage::Format_RGB32:
        case QImage::Format_RGB16:
        case QImage::Format_RGB444:
        case QImage::Format_RGB555:
        case QImage::Format_RGB888:
            return 3;
        case QImage::Format_Invalid:
            break;
        }
        return WSP_STATE_INVALID_PARAMETER;
    }
    inline int qtGetNumOfChannelsExceptAlpha( const QImage &in_img )
    {
        switch( in_img.format() )
        {
        case QImage::Format_Indexed8:
        case QImage::Format_Mono:
        case QImage::Format_MonoLSB:
            return 1;
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied:
        case QImage::Format_ARGB4444_Premultiplied:
        case QImage::Format_ARGB6666_Premultiplied:
        case QImage::Format_ARGB8555_Premultiplied:
        case QImage::Format_ARGB8565_Premultiplied:
        case QImage::Format_RGB32:
        case QImage::Format_RGB16:
        case QImage::Format_RGB444:
        case QImage::Format_RGB555:
        case QImage::Format_RGB888:
            return 3;
        case QImage::Format_Invalid:
            break;
        }
        return WSP_STATE_INVALID_PARAMETER;
    }
    inline const char* qtGetFormatAsString( const QImage::Format format )
    {
        switch( format )
        {
        case QImage::Format_Indexed8               : return "Format_Indexed8              ";
        case QImage::Format_Mono                   : return "Format_Mono                  ";
        case QImage::Format_MonoLSB                : return "Format_MonoLSB               ";
        case QImage::Format_ARGB32                 : return "Format_ARGB32                ";
        case QImage::Format_ARGB32_Premultiplied   : return "Format_ARGB32_Premultiplied  ";
        case QImage::Format_ARGB4444_Premultiplied : return "Format_ARGB4444_Premultiplied";
        case QImage::Format_ARGB6666_Premultiplied : return "Format_ARGB6666_Premultiplied";
        case QImage::Format_ARGB8555_Premultiplied : return "Format_ARGB8555_Premultiplied";
        case QImage::Format_ARGB8565_Premultiplied : return "Format_ARGB8565_Premultiplied";
        case QImage::Format_RGB32                  : return "Format_RGB32                 ";
        case QImage::Format_RGB16                  : return "Format_RGB16                 ";
        case QImage::Format_RGB444                 : return "Format_RGB444                ";
        case QImage::Format_RGB555                 : return "Format_RGB555                ";
        case QImage::Format_RGB888                 : return "Format_RGB888                ";
        case QImage::Format_Invalid                : return "Format_Invalid               ";
        }
        return "Unknown";
    }

    template<typename T> wsp::State qtArrayToQImage(QImage &o_rgb, QImage *o_alpha, const T *in_img, const uchar *in_alpha, u32 width, u32 height, u32 nChannel);
    template<typename T> wsp::State qtQImageToArray(
                                        T *o_img, 
                                        uchar *o_alpha, 
                                        u32 width, u32 height, u32 num_channels,
                                        const QImage &in_img
                                    );
    template<typename T> wsp::State qtWspImageToQImage(QImage &o_rgb, QImage *o_alpha, const wsp::Image<T> &in_img);
    template<typename T> wsp::State qtQImageToWspImage(wsp::Image<T> &o_img, const QImage &in_img);
    template <typename T> 
    inline wsp::State qtArrayToQLabel(QLabel &o_img_label, const T *in_data, u32 width, u32 height, u32 num_of_channels)
    {
        QImage qimg(width, height, QImage::Format_RGB32);

        u32 len = width*height*num_of_channels;
        uchar* tmp_img = new  uchar[len];

        wsp::ToU8Array(tmp_img, in_data, len); //! convert into uchar image

        wsp::State wstat = wsp::img::qtArrayToQImage<T>(qimg, NULL, in_data, NULL, width, height, num_of_channels);
        if(wstat==WSP_STATE_FAILURE){ WSP_COMMON_ERROR_LOG("Type conversion failed\n"); return WSP_STATE_FAILURE; }

        o_img_label.setPixmap(QPixmap::fromImage(qimg));
        delete[] tmp_img;
        return WSP_STATE_SUCCESS;
    }
    template<typename T> inline wsp::State qtWspImageToQLabel(QLabel &o_imgLabel, const wsp::Image<T> &in_img){
        return wsp::img::qtArrayToQLabel<T>(o_imgLabel, in_img.data(), in_img.width(), in_img.height(), in_img.num_of_channels());
    }
    template<typename T> inline wsp::State qtWspImageAlphaToQLabel(QLabel &o_imgLabel, const wsp::Image<T> &in_img){
        return wsp::img::qtArrayToQLabel<uchar>(o_imgLabel, in_img.alpha_data(), in_img.width(), in_img.height(), 1);
    }
    template<typename T> inline wsp::State qtWspImageStateToQLabel(QLabel &o_imgLabel, const wsp::Image<T> &in_img){
        return wsp::img::qtArrayToQLabel<uchar>(o_imgLabel, in_img.state_data(), in_img.width(), in_img.height(), 1);
    }
    template<typename T> inline wsp::State qtWspRgbImageToQLabel(const wsp::Image<T> &in_img, QLabel &o_image_label, bool mask_alpha, bool normalize){
        //! copy into image label
        int w = in_img.width(), h = in_img.height(), npix = w*h, src_nc= in_img.num_of_channels();
        QImage tmp_img(w, h, QImage::Format_RGB32);
        int dst_nc = 4;
        T *src_ptr = const_cast<T*>(in_img.data());
        uchar *dst_ptr = tmp_img.bits();
        uchar *alpha_ptr = const_cast<uchar*>(in_img.alpha_data());
        memset(dst_ptr, 0, npix*dst_nc);
        wsp::img::CopyOneChannel(src_ptr, alpha_ptr, dst_ptr, npix, src_nc, dst_nc, 0, 2, mask_alpha, normalize);
        wsp::img::CopyOneChannel(src_ptr, alpha_ptr, dst_ptr, npix, src_nc, dst_nc, 1, 1, mask_alpha, normalize);
        wsp::img::CopyOneChannel(src_ptr, alpha_ptr, dst_ptr, npix, src_nc, dst_nc, 2, 0, mask_alpha, normalize);
        o_image_label.setPixmap(QPixmap().fromImage(tmp_img.convertToFormat(QImage::Format_ARGB32)));
        return WSP_STATE_SUCCESS;
    }
    template<typename T> inline wsp::State qtWspImageToQLabel(const wsp::Image<T> &in_img, QLabel &o_image_label, bool mask_alpha, bool normalize){
        //! copy into image label
        int w = in_img.width(), h = in_img.height(), npix = w*h, src_nc= in_img.num_of_channels();
        QImage tmp_img(w, h, QImage::Format_RGB32);
        int dst_nc = 4;
        T *src_ptr = const_cast<T*>(in_img.data());
        uchar *dst_ptr = tmp_img.bits();
        uchar *alpha_ptr = const_cast<uchar*>(in_img.alpha_data());
        memset(dst_ptr, 0, npix*dst_nc);
        wsp::img::CopyAllChannels(src_ptr, alpha_ptr, dst_ptr, npix, src_nc, dst_nc, mask_alpha, normalize);
        o_image_label.setPixmap(QPixmap().fromImage(tmp_img.convertToFormat(QImage::Format_ARGB32)));
        return WSP_STATE_SUCCESS;
    }
    #endif

}}

#include "gpl_tpl_fn-imgproc.inl"

#endif
