// @file fn-imgproc-gpl.cpp
// @author Junichi Nishikata
// E-mail: junichi.nishikata@gmail.com

#include <iostream>
#include <float.h>
#include <math.h>

//#include <wsp/common/tpl_cl-vector3.h>
//#include <wsp/common/WspRgb.h>
//#include <wsp/common/WspPoint.h>
#ifdef USE_FFTW
#include <wsp/fftw/fn-math-gpl.h>
#endif

#include <wsp/math/_fn-math.h>
extern "C"{
#include <wsp/math/_fn-math_core.h>
}

#include <wsp/image/gpl_tpl_fn-imgproc.h>
#include <wsp/image/tpl_fn-imgproc.hpp>
#include <wsp/image/fn-imgproc.h>
#include <wsp/image/_fn-imgproc.h>

#include <opencv2/gpu/gpu.hpp>

#include "gpl_fn-imgproc.h"

using namespace std;

// Stereo -------------------------------------------------------

void wsp::img::cvGpuStereoMatching(wsp::Image<double> &o_depth, const char *filenameL, const char *filenameR, int maxdisp, int winsize, int method){
    cv::Mat left_src, right_src;
    cv::Mat left, right;
    cv::gpu::GpuMat d_left, d_right;
    cv::gpu::StereoBM_GPU bm;
    cv::gpu::StereoBeliefPropagation bp;
    cv::gpu::StereoConstantSpaceBP csbp;

    
    // Load images
    left_src = (cv::gpu::GpuMat)cv::imread(filenameL);
    right_src = (cv::gpu::GpuMat)cv::imread(filenameR);
    if (left_src.empty()) throw runtime_error("can't open file \"" + std::string(filenameL) + "\"");
    if (right_src.empty()) throw runtime_error("can't open file \"" + std::string(filenameR) + "\"");
    cv::cvtColor(left_src, left, CV_BGR2GRAY);
    cv::cvtColor(right_src, right, CV_BGR2GRAY);
    d_left = left;
    d_right = right;

    //imshow("left", left);
    //imshow("right", right);

    // Set common parameters
    bm.ndisp = maxdisp;
    bp.ndisp = maxdisp;
    csbp.ndisp = maxdisp;

    bm.winSize = winsize;

    // Prepare disparity map of specified type
    cv::Mat disp(left.size(), CV_8U);
    cv::gpu::GpuMat d_disp(left.size(), CV_8U);

    cout << endl;

    //workBegin();
    switch (method)
    {
    case 0: 
        if (d_left.channels() > 1 || d_right.channels() > 1)
        {
            cout << "BM doesn't support color images\n";
            cvtColor(left_src, left, CV_BGR2GRAY);
            cvtColor(right_src, right, CV_BGR2GRAY);
            cout << "image_channels: " << left.channels() << endl;
            d_left = left;
            d_right = right;
            imshow("left", left);
            imshow("right", right);
        }
        bm(d_left, d_right, d_disp); 
        break;
    case 1: bp(d_left, d_right, d_disp); break;
    case 2: csbp(d_left, d_right, d_disp); break;
    }
    //workEnd();

    // Show results
    disp = d_disp;
    cv::normalize(disp, disp, 0, 256, CV_MINMAX, CV_8U);
    //putText(disp, text(), Point(5, 25), FONT_HERSHEY_SIMPLEX, 1.0, Scalar::all(255));
    //imshow("disparity", disp);

    o_depth.SetSize((u32)left.cols, (u32)left.rows);
    for(int y=0; y<left.rows; y++){
        for(int x=0; x<left.cols; x++){
            o_depth.SetElem(disp.at<uchar>(y, x), (u32)x, (u32)y);
        }
    }

    //handleKey((char)waitKey(3));
    //cv::waitKey();
}

// Type Conversion ------------------------------------------------
// Resize ------------------------------------------------

//wsp::State wsp::img::cvResizeImage(wsp::Image<double> &io_img, u32 newWidth, u32 newHeight, int interpolation){
//    if(io_img.data()==NULL){
//        WSP_COMMON_ERROR_LOG("Input image is NULL\n");
//        return WSP_STATE_FAILURE;
//    }
//
//    int w=(int)io_img.width(), h=(int)io_img.height();
//    int x, y;
//    int len = (u32)(w*h);
//    int i, i_cv, nChannel=1;
//    IplImage *src_img=NULL, *mask_img=NULL, *dst_img;
//
//    src_img = cvCreateImage(cvSize(w, h), IPL_DEPTH_64F, nChannel);
//    dst_img = cvCreateImage(cvSize((int)newWidth, (int)newHeight), IPL_DEPTH_64F, nChannel);
//
//    //wsp::img::cvDoubleImage2IplImage(src_img, NULL, io_img);
//    wsp::img::cvConvertWspImageToIplImage<double>(src_img, NULL, io_img);
//
//    cvResize (src_img, dst_img, interpolation);
//
//    // (4)結果を表示する
//    //cvNamedWindow ("src", CV_WINDOW_AUTOSIZE);
//    //cvNamedWindow ("dst", CV_WINDOW_AUTOSIZE);
//    //cvShowImage ("src", src_img);
//    //cvShowImage ("dst", dst_img);
//    //cvWaitKey (0);
//
//    //cvDestroyWindow ("src");
//    //cvDestroyWindow ("dst");
//    //cvReleaseImage (&src_img);
//    //cvReleaseImage (&dst_img);
//
//    io_img.SetSize(newWidth, newHeight);
//
//    //wsp::img::cvIplImage2DoubleImg(io_img, dst_img, NULL);
//    wsp::img::cvIplImage2wsp::Image<double>(io_img, dst_img, NULL);
//
//    return WSP_STATE_SUCCESS;
//}
//wsp::State wsp::img::cvResizeImage(wsp::Image<WspRgb> &io_img, u32 newWidth, u32 newHeight, int iterpolation){
//    return WSP_STATE_SUCCESS;
//}



// Interpolation -----------------------------------------
//#ifdef USE_OPENCV
//
//
//wsp::State wsp::img::cvInpaint(wsp::Image<WspRgb> &io_img, int flags)
//{
//    if(io_img.data()==NULL){
//        WSP_COMMON_ERROR_LOG("Input image is NULL\n");
//        return WSP_STATE_FAILURE;
//    }
//
//    int w=(int)io_img.width(), h=(int)io_img.height();
//    int x, y;
//    int len = (u32)(w*h);
//    int i, i_cv, nChannel=3;
//    IplImage *src_img=NULL, *mask_img=NULL, *dst_img;
//
//    src_img = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, (int)nChannel);
//    dst_img = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, nChannel);
//    mask_img= cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
//
//    printf("w:%d, h:%d, ws:%d", src_img->width, src_img->height, src_img->widthStep);
//
//    //wsp::img::cvRgbImage2IplImage(src_img, mask_img, io_img);
//    wsp::img::cvConvertArrayToIplImage<uchar>(src_img, mask_img, (uchar*)io_img.data(), io_img.alpha_data(), (u32)w, (u32)h, 3);
//    //wsp::img::cvConvertWspImageToIplImage<WspRgb>(src_img, mask_img, io_img);
//
//
//    cvInpaint(src_img, mask_img, dst_img, 10.0, flags);
//
//    //cvNamedWindow ("Source");
//    //cvNamedWindow ("Mask");
//    //cvNamedWindow ("Inpaint");
//    //cvShowImage ("Source", src_img);
//    //cvShowImage ("Mask", mask_img);
//    //cvShowImage ("Inpaint", dst_img);
//    //cvWaitKey (0);
//
//    //cvDestroyWindow ("Source");
//    //cvDestroyWindow ("Mask");
//    //cvDestroyWindow ("Inpaint");
//
//    //wsp::img::cvIplImage2RgbImg(io_img, dst_img, NULL);
//    wsp::img::cvConvertIplImageToArray<uchar>((uchar*)io_img.data(), NULL, dst_img, NULL);
//
//    cvReleaseImage(&src_img);
//    cvReleaseImage(&mask_img);
//    cvReleaseImage(&dst_img);
//
//    return WSP_STATE_SUCCESS;
//}
//
//
//#endif

// Generator ---------------------------------------------



// Segmentation --------------------------------------------------------------
//#ifdef USE_OPENCV
//wsp::State wsp::img::cvSegmPyrMeanShiftFiltering(int *o_segm, uchar *o_colorSegm, const uchar *in_img, u32 width, u32 height, u32 nChannels, 
//                                                        int pyrLevel, double winRad_space, double winRad_colSpace)
//{
//    if((o_segm==NULL && o_colorSegm==NULL) || in_img==NULL){
//        WSP_COMMON_ERROR_LOG("Input or output image is NULL\n"); return WSP_STATE_FAILURE;
//    }
//
//    int w=(int)width, h=(int)height;
//    int x, y;
//    int len = (u32)(w*h);
//    int i, i_cv, c;
//    IplImage *src_img=NULL, *dst_img=NULL;
//
//    int depthType = IPL_DEPTH_8U;
//    CvRect roi;
//
//    src_img = cvCreateImage(cvSize(w, h), depthType, (int)nChannels);
//    //dst_img = cvCreateImage(cvSize(w, h), depthType, (int)nChannels);
//
//    printf("w:%d, h:%d, ws:%d\n", src_img->width, src_img->height, src_img->widthStep);
//
//    wsp::img::cvConvertArrayToIplImage<uchar>(src_img, NULL, in_img, NULL, width, height, nChannels);
//
//
//    roi.x = roi.y = 0;
//    roi.width = src_img->width & -(1 << pyrLevel);
//    roi.height = src_img->height & -(1 << pyrLevel);
//    cvSetImageROI(src_img, roi);
//
//    dst_img = cvCloneImage (src_img);
//
//    //**--------------------------------------------------------------------------
//    // void cvPyrMeanShiftFiltering( const CvArr* src, CvArr* dst, 
//    //        double sp, double sr, int max_level=1,
//    //        CvTermCriteria termcrit=cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS,5,1));
//    // src: input image. 8bit, 3 channels. 
//    // dst: output image. This must be the same size and format as input. 
//    // sp: the radius of space window. 
//    // sr: the radius of color space window. 
//    // max_level: maximum level of pyramid for segmentation.
//    // termcrit: condition for exiting iteration of Mean-Shift 
//    cvPyrMeanShiftFiltering(src_img, dst_img, winRad_space, winRad_colSpace, pyrLevel, cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 5, 1));
//    //**--------------------------------------------------------------------------
//
//    wsp::Image<uchar> rgbSegm(width, height, nChannels);
//
//    wsp::img::cvConvertIplImageToArray<uchar>(rgbSegm.data(), NULL, dst_img, NULL);
//
//    // put red channel, this must be modified
//    for(i=0; i<width*height; i++){
//        if(o_segm!=NULL){ o_segm[i] = 0; }
//        for(c=0; c<nChannels; c++){
//            if(o_segm!=NULL){
//                o_segm[i]  |= static_cast<int>(rgbSegm[i*nChannels+c])<<(8*c);
//            }
//            if(o_colorSegm!=NULL){
//                o_colorSegm[i*nChannels+c] = rgbSegm[i*nChannels+c];
//            }
//        }
//    }
//
//    cvReleaseImage(&src_img);
//    cvReleaseImage(&dst_img);
//
//    return WSP_STATE_SUCCESS;
//}
//
// o_segm can have 1 channel, o_colorSegm must have the same number of channels as nChannels
//wsp::State wsp::img::cvSegmPyr(int *o_segm, uchar *o_colorSegm, const uchar *in_img, u32 width, u32 height, u32 nChannels,
//                                     int pyrLevel, double thre_link, double thre_segmClast)
//{
//    if((o_segm==NULL && o_colorSegm==NULL) || in_img==NULL){
//        WSP_COMMON_ERROR_LOG("Input or output image is NULL\n");
//        return WSP_STATE_FAILURE;
//    }
//
//    int w=(int)width, h=(int)height;
//    int x, y;
//    int len = (u32)(w*h);
//    int i, i_cv, c;
//    IplImage *src_img=NULL, *dst_img=NULL;
//
//    int depthType=IPL_DEPTH_8U;
//    CvRect roi;
//    CvMemStorage *storage = NULL;
//    CvSeq *comp = NULL;
//
//    src_img = cvCreateImage(cvSize(w, h), depthType, (int)nChannels);
//
//    printf("w:%d, h:%d, ws:%d\n", src_img->width, src_img->height, src_img->widthStep);
//
//    wsp::img::cvConvertArrayToIplImage<uchar>(src_img, NULL, in_img, NULL, width, height, nChannels);
//
//    roi.x = roi.y = 0;
//    roi.width = src_img->width & -(1 << pyrLevel);
//    roi.height = src_img->height & -(1 << pyrLevel);
//    cvSetImageROI(src_img, roi);
//
//    dst_img = cvCloneImage(src_img);
//    storage = cvCreateMemStorage(0);
//
//    //------------------------------------------------------------------------------------------
//    // void cvPyrSegmentation( IplImage* src, IplImage* dst, CvMemStorage* storage, CvSeq** comp,
//    //                       int level, double threshold1, double threshold2 );
//    // src: input image
//    // dst: output image 
//    // storage: 結果として得られる接続成分のシーケンスを保存するための領域． 
//    // comp: pointer of output sequence for segmentated element. 
//    // level: muximum level for segmentation 
//    // threshold1: error threshold for building linking. 
//    // threshold2: error threshold for segment clastering. 
//    cvPyrSegmentation(src_img, dst_img, storage, &comp, pyrLevel, thre_link, thre_segmClast);
//    //------------------------------------------------------------------------------------------
//
//    wsp::Image<uchar> rgbSegm(width, height, nChannels);
//
//    wsp::img::cvConvertIplImageToArray<uchar>(rgbSegm.data(), NULL, dst_img, NULL);
//
//    for(i=0; i<width*height; i++){
//        if(o_segm!=NULL){ o_segm[i] = 0; }
//        for(c=0; c<nChannels; c++){
//            if(o_segm!=NULL){
//                o_segm[i]  |= static_cast<int>(rgbSegm[i*nChannels+c])<<(8*c);
//            }
//            if(o_colorSegm!=NULL){
//                o_colorSegm[i*nChannels+c] = rgbSegm[i*nChannels+c];
//            }
//        }
//    }
//
//
//    // put red channel, this must be modified
//    cvReleaseImage(&src_img);
//    cvReleaseImage(&dst_img);
//    cvReleaseMemStorage(&storage);
//
//    return WSP_STATE_SUCCESS;
//}
//
//
//
//IplImage *dsp_img=NULL, *markers=NULL;
//static int mouse_drug=0;
//static int seed_num = (int)(255*((double)rand()/(double)RAND_MAX));
//void on_mouse (int event, int x, int y, int flags, void *param)
//{
//    int seed_rad = 10;
//    CvPoint pt;
//
//    if(mouse_drug==1) {
//        //seed_num++;
//        pt = cvPoint (x, y);
//        cvCircle (markers, pt, seed_rad, cvScalarAll (seed_num), CV_FILLED, 8, 0);
//        cvCircle (dsp_img, pt, seed_rad, cvScalarAll (seed_num), CV_FILLED, 8, 0);
//        cvShowImage("image", dsp_img);
//    }
//
//    // (1)クリックにより中心を指定し，円形のシード領域を設定する
//    if (event == CV_EVENT_LBUTTONDOWN) {
//        seed_num = (int)(255*((double)rand()/(double)RAND_MAX)); //0;
//        mouse_drug=1;
//    }else if(event==CV_EVENT_LBUTTONUP){
//        mouse_drug=0;
//    }
//}
//wsp::State wsp::img::cvSegmWatershedPaintMarkers(uchar *o_segm, const uchar *in_img, u32 width, u32 height, u32 nChannels)
//{
//    if(o_segm==NULL || in_img==NULL){
//        WSP_COMMON_ERROR_LOG("Input or output image is NULL\n");
//        return WSP_STATE_FAILURE;
//    }
//
//    int w=(int)width, h=(int)height;
//    int x, y;
//    int len = (u32)(w*h);
//    int *idx, i, j;
//    IplImage *src_img=NULL, *dst_gray_img=NULL, *dst_img=NULL;
//    //IplImage *dsp_img=NULL, *markers=NULL;
//
//    int depthType=IPL_DEPTH_8U;
//
//    src_img = cvCreateImage(cvSize(w, h), depthType, (int)nChannels);
//
//    printf("w:%d, h:%d, ws:%d\n", src_img->width, src_img->height, src_img->widthStep);
//
//    wsp::img::cvConvertArrayToIplImage<uchar>(src_img, NULL, in_img, NULL, width, height, nChannels);
//
//    dsp_img = cvCloneImage (src_img);
//    dst_img = cvCloneImage (src_img);
//
//    markers = cvCreateImage (cvGetSize (src_img), IPL_DEPTH_32S, 1);
//    dst_gray_img = cvCreateImage (cvGetSize (src_img), IPL_DEPTH_8U, 1);
//    cvZero (markers);
//
//    // (3)入力画像を表示しシードコンポーネント指定のためのマウスイベントを登録する
//    cvNamedWindow ("image", CV_WINDOW_AUTOSIZE);
//    cvShowImage ("image", src_img);
//    cvSetMouseCallback ("image", on_mouse, 0);
//    cvWaitKey (0);
//
//    // (4)watershed分割を実行する  
//    //**--------------------------------------------------------------------------------
//    // void cvWatershed( const CvArr* image, CvArr* markers );
//    // image: 入力画像．8ビット，3チャンネル画像． 
//    // markers: 入出力画像．32ビットシングルチャンネルのマーカー画像（マップ）． 
//    // 関数 cvWatershed は，参考文献[Meyer92]にある，ノンパラメトリックマーカーベースのセグメンテーションアルゴリズムである
//    // watershedの一種を実装する． 画像をこの関数に渡す前に，ユーザーは大まかに画像markers中の処理対象領域を， 正（>0）の
//    // インデックスを用いて区切っておかなければならない．例えば，各領域がピクセル値として1,2,3...を持ち，一つあるいはそれ以上の
//    // 連結成分で表現されるなど．これらの成分は，処理における各画像領域の「シード」となる．区切られた領域との関係が未知なその他の
//    // 領域は，このアルゴリズムによって関係が定義される．関係が未知の領域の値は0にセットしておく必要がある．関数の出力時には，
//    // markers中の各ピクセル値は，シード成分に振られていた値か，領域の間に属する場合の-1，のいずれかがセットされる． 
//    // 連結成分のそれぞれ二つの隣接間が，watershed境界（値が-1のピクセル）で分離されている必要がないことに注意する．例えば，
//    // 初期マーカー画像中に接成分（tangent components）が存在する場合など．デモとこの関数の使用方法については，OpenCVサンプルディレクトリ中の
//    // watershed.cpp を参照． 
//    cvWatershed (src_img, markers);
//    //**--------------------------------------------------------------------------------
//
//    // (5)実行結果の画像中のwatershed境界（ピクセル値=-1）を結果表示用画像上に表示する
//    for (i = 0; i < markers->height; i++) {
//        for (j = 0; j < markers->width; j++) {
//            idx = (int *) cvPtr2D(markers, i, j, NULL);
//            if (*idx == -1){
//                cvSet2D(dst_img, i, j, cvScalarAll (255));
//                cvSet2D(dst_gray_img, i, j, cvScalarAll (255));
//                continue;
//            }
//            cvSet2D(dst_gray_img, i, j, cvScalarAll(*idx));
//        }
//    }
//
//    cvDestroyWindow ("image");
//    cvNamedWindow ("watershed transform", CV_WINDOW_AUTOSIZE);
//    cvShowImage ("watershed transform", dst_img);
//    cvWaitKey (0);
//
//    wsp::img::cvConvertIplImageToArray<uchar>(o_segm, NULL, dst_gray_img, NULL);
//
//    cvDestroyWindow ("watershed transform");
//    cvReleaseImage (&markers);
//    cvReleaseImage (&dsp_img);
//    cvReleaseImage (&src_img);
//    cvReleaseImage (&dst_img);
//
//    return WSP_STATE_SUCCESS;
//}
//
//wsp::State wsp::img::cvSegmWatershed(int *io_segm, const uchar *in_img, u32 width, u32 height, u32 nChannels)
//{
//    if(io_segm==NULL || in_img==NULL){
//        WSP_COMMON_ERROR_LOG("Input or output image is NULL\n");
//        return WSP_STATE_FAILURE;
//    }
//
//    int w=(int)width, h=(int)height;
//    int x, y;
//    int len = (u32)(w*h);
//    int *idx, id, i, j;
//    IplImage *src_img=NULL, *dst_gray_img=NULL, *marker_img=NULL;
//    //IplImage *dsp_img=NULL, *markers=NULL;
//
//    int depthType=IPL_DEPTH_8U;
//
//    src_img = cvCreateImage(cvSize(w, h), depthType, (int)nChannels);
//    marker_img = cvCreateImage (cvSize(w, h), IPL_DEPTH_32S, 1);
//    dst_gray_img = cvCreateImage (cvSize(w, h), IPL_DEPTH_32S, 1);
//
//    printf("w:%d, h:%d, ws:%d\n", src_img->width, src_img->height, src_img->widthStep);
//
//    wsp::img::cvConvertArrayToIplImage<uchar>(src_img, NULL, in_img, NULL, width, height, nChannels);
//    wsp::img::cvConvertArrayToIplImage<int>(marker_img, NULL, io_segm, NULL, width, height, 1);
//
//    //cvNamedWindow ("source image", CV_WINDOW_AUTOSIZE);
//    //cvShowImage ("source image", src_img);
//    //cvNamedWindow ("marker image", CV_WINDOW_AUTOSIZE);
//    //cvShowImage ("marker image", marker_img);
//    //cvWaitKey (0);
//    //cvDestroyWindow ("source image");
//    //cvDestroyWindow ("marker image");
//
//    cvWatershed(src_img, marker_img);
//
//    //int *data = (int*)marker_img->imageData;
//    for (i = 0; i < height; i++) {
//        for (j = 0; j < width; j++) {
//            //idx = (int *) cvPtr2D(marker_img, i, j, NULL);
//            id = *(reinterpret_cast<int*>(&marker_img->imageData[i*marker_img->widthStep + j*sizeof(int)]));
//            if(id==-1 && 0<j){
//                //printf("id=%d, x=%d\n", id, j);
//                //dst_gray_img->imageData[i*dst_gray_img->widthStep + j]=0;
//                dst_gray_img->imageData[i*dst_gray_img->widthStep + j*sizeof(int)] = dst_gray_img->imageData[i*dst_gray_img->widthStep + (j-1)*sizeof(int)];
//                continue;
//            }
//            dst_gray_img->imageData[i*dst_gray_img->widthStep + j*sizeof(int)] = id;
//
//            //printf("%d\n", id);
//            //if (*idx == -1){
//            //    cvSet2D(dst_gray_img, i, j, cvScalarAll (255));
//            //    continue;
//            //}
//            //cvSet2D(dst_gray_img, i, j, cvScalarAll(*idx));
//        }
//    }
//
//    //cvNamedWindow ("watershed transform", CV_WINDOW_AUTOSIZE);
//    //cvShowImage ("watershed transform", dst_gray_img);
//    //cvWaitKey (0);
//    //cvDestroyWindow ("watershed transform");
//
//    //wsp::img::cvConvertIplImageToArray<int>(io_segm, NULL, marker_img, NULL);
//    wsp::img::cvConvertIplImageToArray<int>(io_segm, NULL, dst_gray_img, NULL);
//
//    cvReleaseImage(&marker_img);
//    cvReleaseImage(&src_img);
//    cvReleaseImage(&dst_gray_img);
//
//    return WSP_STATE_SUCCESS;
//}
//#endif
