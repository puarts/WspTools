#pragma once

#include <wsp/image/_tpl_fn-imgproc.hpp>
#include <wsp/image/tpl_cl-image.h>

#include <opencv2/opencv.hpp>

namespace wsp{ namespace opencv{
	// ==============================
	//! Resize
	// ==============================

	//! 0 - INTER_NEAREST 最近傍補間
	//! 1 - INTER_LINEAR バイリニア補間（デフォルト）
	//! 2 - INTER_AREA ピクセル領域の関係を利用したリサンプリング．画像を大幅に縮小する場合は，モアレを避けることができる良い手法です．しかし，画像を拡大する場合は， INTER_NEAREST メソッドと同様になります
	//! 3 - INTER_CUBIC 4x4 の近傍領域を利用するバイキュービック補間
	//! 4 - INTER_LANCZOS4 8x8 の近傍領域を利用する Lanczos法の補間
	template<typename T> wsp::State cvResizeImage(wsp::Image<T> &io_img, u32 newWidth, u32 newHeight, int algorithm=1);
	template<typename T> wsp::State cvResizeU8Image(wsp::Image<T> &io_img, u32 newWidth, u32 newHeight, int algorithm=1);
	template<typename T> wsp::State cvResizeImageNearestNeighbor(wsp::Image<T> &io_img, u32 newWidth, u32 newHeight);
	template<typename T> wsp::State cvResizeImageLinear(wsp::Image<T> &io_img, u32 newWidth, u32 newHeight);
	template<typename T> wsp::State cvResizeImageArea(wsp::Image<T> &io_img, u32 newWidth, u32 newHeight);
	template<typename T> wsp::State cvResizeImageBicubic(wsp::Image<T> &io_img, u32 newWidth, u32 newHeight);

	// ==============================
	//! Type Conversion
	// ==============================
	template<typename T, typename U> 
    wsp::State cvArray2IplImage(
        IplImage *o_img, IplImage *o_alpha, const T *in_img, const uchar *in_alpha, u32 width, u32 height, u32 nChannel);
	
    template<typename T> wsp::State cvArray2IplImage(IplImage *o_img, IplImage *o_alpha, const T *in_img, const uchar *in_alpha, u32 width, u32 height, u32 nChannel);
	template<typename T> wsp::State cvIplImage2Array(T *o_img, uchar *o_alpha, const IplImage *in_img, const IplImage *in_alpha);

	template<typename T> wsp::State cvWspImg2IplImage(IplImage *o_img, IplImage *o_alpha, const wsp::Image<T> &in_img);
	template<typename T> wsp::State cvIplImage2WspImg(wsp::Image<T> &o_img, const IplImage *in_img, const IplImage *in_alpha);
	//inline wsp::State cvIplImage2RgbImg(wsp::Image<WspRgb> &o_rgba, const IplImage *in_rgb, const IplImage *in_alpha);
	//inline wsp::State cvRgbImage2IplImage(IplImage *o_rgb, IplImage *o_alpha, const wsp::Image<WspRgb> &in_rgba);
	//inline wsp::State cvIplImage2DoubleImg(wsp::Image<double> &o_img, const IplImage *in_img, const IplImage *in_alpha);
	//inline wsp::State cvDoubleImage2IplImage(IplImage *o_img, IplImage *o_alpha, const wsp::Image<double> &in_img);
	// ==============================
	//! Interpolation 
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
}}

#include "tpl_fn-imgproc-gpl.inl"
