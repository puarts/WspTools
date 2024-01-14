/**
 * @file tpl_fn-imgproc-gpl.h
 * @author Jun-ichi Nishikata
 */

#ifndef WspImg_WspFnImgProcGpl_tpl_H_
#define WspImg_WspFnImgProcGpl_tpl_H_

//#include "WspImg_Index.h>
#include "_tpl_fn-imgproc.h"
#include "tpl_cl-image.h"

#ifdef USE_QT
	#include <QtGui/QtGui>
#endif
#ifdef USE_OPENCV
	#include <load_opencv.h>
#endif


namespace wsp{ namespace img{
	// ==============================
	//! Resize
	// ==============================
#ifdef USE_OPENCV
	//! 0 - INTER_NEAREST 最近傍補間
	//! 1 - INTER_LINEAR バイリニア補間（デフォルト）
	//! 2 - INTER_AREA ピクセル領域の関係を利用したリサンプリング．画像を大幅に縮小する場合は，モアレを避けることができる良い手法です．しかし，画像を拡大する場合は， INTER_NEAREST メソッドと同様になります
	//! 3 - INTER_CUBIC 4x4 の近傍領域を利用するバイキュービック補間
	//! 4 - INTER_LANCZOS4 8x8 の近傍領域を利用する Lanczos法の補間
	template<typename T> wsp::State cvResizeImage(wsp::Image<T> &io_img, uint newWidth, uint newHeight, int algorithm=1);
	template<typename T> wsp::State cvResizeU8Image(wsp::Image<T> &io_img, uint newWidth, uint newHeight, int algorithm=1);
	template<typename T> wsp::State cvResizeImageNearestNeighbor(wsp::Image<T> &io_img, uint newWidth, uint newHeight);
	template<typename T> wsp::State cvResizeImageLinear(wsp::Image<T> &io_img, uint newWidth, uint newHeight);
	template<typename T> wsp::State cvResizeImageArea(wsp::Image<T> &io_img, uint newWidth, uint newHeight);
	template<typename T> wsp::State cvResizeImageBicubic(wsp::Image<T> &io_img, uint newWidth, uint newHeight);
#endif
	// ==============================
	//! Type Conversion
	// ==============================
#ifdef USE_OPENCV
	template<typename T, typename U> wsp::State cvArray2IplImage(IplImage *o_img, IplImage *o_alpha, const T *in_img, const uchar *in_alpha, uint width, uint height, uint nChannel);
	template<typename T> wsp::State cvArray2IplImage(IplImage *o_img, IplImage *o_alpha, const T *in_img, const uchar *in_alpha, uint width, uint height, uint nChannel);
	template<typename T> wsp::State cvIplImage2Array(T *o_img, uchar *o_alpha, const IplImage *in_img, const IplImage *in_alpha);

	template<typename T> wsp::State cvWspImg2IplImage(IplImage *o_img, IplImage *o_alpha, const wsp::Image<T> &in_img);
	template<typename T> wsp::State cvIplImage2WspImg(wsp::Image<T> &o_img, const IplImage *in_img, const IplImage *in_alpha);
	//inline wsp::State cvIplImage2RgbImg(wsp::Image<WspRgb> &o_rgba, const IplImage *in_rgb, const IplImage *in_alpha);
	//inline wsp::State cvRgbImage2IplImage(IplImage *o_rgb, IplImage *o_alpha, const wsp::Image<WspRgb> &in_rgba);
	//inline wsp::State cvIplImage2DoubleImg(wsp::Image<double> &o_img, const IplImage *in_img, const IplImage *in_alpha);
	//inline wsp::State cvDoubleImage2IplImage(IplImage *o_img, IplImage *o_alpha, const wsp::Image<double> &in_img);
#endif

#ifdef USE_QT
	template<typename T> wsp::State qtArray2QImage(QImage &o_rgb, QImage *o_alpha, const T *in_img, const uchar *in_alpha, uint width, uint height, uint nChannel);
	template<typename T> wsp::State qtQImage2Array(T *o_img, uchar *o_alpha, const QImage &in_img, const QImage *in_alpha, uint nChannel);
	template<typename T> wsp::State qtWspImg2QImage(QImage &o_rgb, QImage *o_alpha, const wsp::Image<T> &in_img);
	template<typename T> wsp::State qtQImage2WspImg(wsp::Image<T> &o_img, const QImage &in_img, const QImage *in_alpha, uint nChannel);
	template <typename T> 
	inline wsp::State qtArray2QLabel(QLabel &o_img_label, const T *in_data, uint width, uint height, uint num_of_channels)
	{
		QImage qimg(width, height, QImage::Format_RGB32);

		uint len = width*height*num_of_channels;
		uchar* tmp_img = new  uchar[len];

		wsp::ToU8Array(tmp_img, in_data, len); //! convert into uchar image

		wsp::State wstat = wsp::img::qtArray2QImage<T>(qimg, NULL, in_data, NULL, width, height, num_of_channels);
		if(wstat==WSP_STATE_FAILURE){ WSPERROR("Type conversion failed\n"); return WSP_STATE_FAILURE; }

		o_img_label.setPixmap(QPixmap::fromImage(qimg));
		delete[] tmp_img;
		return WSP_STATE_SUCCESS;
	}
	template<typename T> inline wsp::State qtWspImg2QLabel(QLabel &o_imgLabel, const wsp::Image<T> &in_img){
		return wsp::img::qtArray2QLabel<T>(o_imgLabel, in_img.data(), in_img.width(), in_img.height(), in_img.num_of_channels());
	}
	template<typename T> inline wsp::State qtWspImgAlpha2QLabel(QLabel &o_imgLabel, const wsp::Image<T> &in_img){
		return wsp::img::qtArray2QLabel<uchar>(o_imgLabel, in_img.alpha_data(), in_img.width(), in_img.height(), 1);
	}
	template<typename T> inline wsp::State qtWspImgState2QLabel(QLabel &o_imgLabel, const wsp::Image<T> &in_img){
		return wsp::img::qtArray2QLabel<uchar>(o_imgLabel, in_img.state_data(), in_img.width(), in_img.height(), 1);
	}
	template<typename T> inline wsp::State qtWspRgbImg2QLabel(const wsp::Image<T> &in_img, QLabel &o_image_label, bool mask_alpha, bool normalize){
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
	template<typename T> inline wsp::State qtWspImg2QLabel(const wsp::Image<T> &in_img, QLabel &o_image_label, bool mask_alpha, bool normalize){
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

	// ==============================
	//! Pixel Operation
	// ==============================

	// ==============================
	//! Occlusion Processing 
	// ==============================

	// ==============================
	//! Segmentation 
	// ==============================

	// ==============================
	//! Interpolation 
	// ==============================
#ifdef USE_OPENCV
	template<typename T> wsp::State cvInpaint(T *io_img, const uchar *in_mask, int width, int height, int nChannels, int algType);
	template<typename T> inline wsp::State cvInpaintNavierStokes(T *io_img, const uchar *in_mask, int width, int height, int nChannels);
	template<typename T> inline wsp::State cvInpaintAlexandruTelea(T *io_img, const uchar *in_mask, int width, int height, int nChannels);

	template<typename T> wsp::State cvInpaintAlpha(T *io_img, const uchar *in_alpha, int width, int height, int nChannels, int algType);
	template<typename T> inline wsp::State cvInpaintAlphaNavierStokes(T *io_img, const uchar *in_alpha, int width, int height, int nChannels);
	template<typename T> inline wsp::State cvInpaintAlphaAlexandruTelea(T *io_img, const uchar *in_alpha, int width, int height, int nChannels);

	template<typename T> wsp::State cvInpaintState(T *io_img, const uchar *in_state, int width, int height, int nChannels, wsp::img::PixelState target_state, int algType);
	template<typename T> inline wsp::State cvInpaintStateNavierStokes(T *io_img, const uchar *in_state, int width, int height, int nChannels, wsp::img::PixelState target_state);
	template<typename T> inline wsp::State cvInpaintStateAlexandruTelea(T *io_img, const uchar *in_state, int width, int height, int nChannels, wsp::img::PixelState target_state);
#endif
}}

#include "tpl_fn-imgproc-gpl.inl"

#endif