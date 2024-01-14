// @file _fn-imgproc-gpl.cpp
// @author Jun-ichi Nishikata

#include "_fn-imgproc-gpl.h"
#include "tpl_fn-imgproc-gpl.h"

#include <wsp/common/fn-debug.h>
#include <wsp/image/tpl_cl-image.h>

#include <opencv2/opencv.hpp>

#define USE_OPTIMIZED_CODE

//! Segmentation --------------------------------------------------------------

wsp::State wsp::opencv::cvSegmPyrMeanShiftFiltering(int *o_segm, const uchar *in_img, 
                                                 u32 width, u32 height, u32 nChannels, 
                                                 double winRad_space, double winRad_colSpace, int pyrLevel)
{
    if(o_segm==NULL || in_img==NULL){
        WSP_COMMON_ERROR_LOG("Input or output image is NULL\n");
        return WSP_STATE_FAILURE;
    }

    int w=(int)width, h=(int)height;
    int x, y;
    int len = (u32)(w*h);
    int i, i_cv;
    IplImage *src_img=NULL, *mask_img=NULL, *dst_img=NULL;

    int depthType=IPL_DEPTH_8U;
    CvRect roi;

    src_img = cvCreateImage(cvSize(w, h), depthType, (int)nChannels);
    //dst_img = cvCreateImage(cvSize(w, h), depthType, (int)nChannels);

    printf("w:%d, h:%d, ws:%d\n", src_img->width, src_img->height, src_img->widthStep);

    wsp::opencv::cvConvertArrayToIplImage<uchar>(src_img, NULL, in_img, NULL, width, height, nChannels);

    roi.x = roi.y = 0;
    roi.width = src_img->width & -(1 << pyrLevel);
    roi.height = src_img->height & -(1 << pyrLevel);
    cvSetImageROI(src_img, roi);

    dst_img = cvCloneImage (src_img);

    //!--------------------------------------------------------------------------
    //! void cvPyrMeanShiftFiltering( const CvArr* src, CvArr* dst, 
    //!        double sp, double sr, int max_level=1,
    //!        CvTermCriteria termcrit=cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS,5,1));
    //! src: input image. 8bit, 3 channels. 
    //! dst: output image. This must be the same size and format as input. 
    //! sp: the radius of space window. 
    //! sr: the radius of color space window. 
    //! max_level: maximum level of pyramid for segmentation.
    //! termcrit: condition for exiting iteration of Mean-Shift 
    cvPyrMeanShiftFiltering(src_img, dst_img, winRad_space, winRad_colSpace, pyrLevel, cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 5, 1));
    //!--------------------------------------------------------------------------

    //cvNamedWindow ("Source", CV_WINDOW_AUTOSIZE);
    //cvNamedWindow ("MeanShift", CV_WINDOW_AUTOSIZE);
    //cvShowImage ("Source", src_img);
    //cvShowImage ("MeanShift", dst_img);
    //cvWaitKey (0);
    //cvDestroyWindow ("Source");
    //cvDestroyWindow ("MeanShift");

    wsp::Image<uchar> rgbSegm(width, height, 3);

    wsp::opencv::cvConvertIplImageToArray<uchar>(rgbSegm.data(), NULL, dst_img, NULL);

    //! put red channel, this must be modified
    for(u32 i=0; i<width*height; i++){
        o_segm[i] = static_cast<int>(rgbSegm[i*3]);
    }

    cvReleaseImage(&src_img);
    cvReleaseImage(&dst_img);

    return WSP_STATE_SUCCESS;
}

wsp::State wsp::opencv::cvSegmPyr(int *o_segm, const uchar *in_img, u32 width, u32 height, u32 nChannels,
                                     int pyrLevel, double thre_link, double thre_segmClast)
{
    if(o_segm==NULL || in_img==NULL){
        WSP_COMMON_ERROR_LOG("Input or output image is NULL\n");
        return WSP_STATE_FAILURE;
    }

    int w=(int)width, h=(int)height;
    int x, y;
    int len = (u32)(w*h);
    int i, i_cv;
    IplImage *src_img=NULL, *mask_img=NULL, *dst_img=NULL;

    int depthType=IPL_DEPTH_8U;
    CvRect roi;
    CvMemStorage *storage = 0;
    CvSeq *comp = 0;

    src_img = cvCreateImage(cvSize(w, h), depthType, (int)nChannels);

    printf("w:%d, h:%d, ws:%d\n", src_img->width, src_img->height, src_img->widthStep);

    wsp::opencv::cvConvertArrayToIplImage<uchar>(src_img, NULL, in_img, NULL, width, height, nChannels);

    roi.x = roi.y = 0;
    roi.width = src_img->width & -(1 << pyrLevel);
    roi.height = src_img->height & -(1 << pyrLevel);
    cvSetImageROI(src_img, roi);

    dst_img = cvCloneImage (src_img);
    storage = cvCreateMemStorage (0);

    //------------------------------------------------------------------------------------------
    //! void cvPyrSegmentation( IplImage* src, IplImage* dst, CvMemStorage* storage, CvSeq** comp,
    //!                       int level, double threshold1, double threshold2 );
    //! src: input image
    //! dst: output image 
    //! storage: 結果として得られる接続成分のシーケンスを保存するための領域． 
    //! comp: pointer of output sequence for segmentated element. 
    //! level: muximum level for segmentation 
    //! threshold1: error threshold for building linking. 
    //! threshold2: error threshold for segment clastering. 
    cvPyrSegmentation(src_img, dst_img, storage, &comp, pyrLevel, thre_link, thre_segmClast);
    //------------------------------------------------------------------------------------------

    //cvNamedWindow ("Source", CV_WINDOW_AUTOSIZE);  
    //cvNamedWindow ("Segmentation", CV_WINDOW_AUTOSIZE); 
    //cvShowImage ("Source", src_img); 
    //cvShowImage ("Segmentation", dst_img); 
    //cvWaitKey (0); 
    //cvDestroyWindow ("Source"); 
    //cvDestroyWindow ("Segmentation");

    wsp::opencv::cvConvertIplImageToArray(o_segm, NULL, dst_img, NULL);

    //! put red channel, this must be modified
    cvReleaseImage(&src_img);
    cvReleaseImage(&dst_img);

    return WSP_STATE_SUCCESS;
}



IplImage *dsp_img=NULL, *markers=NULL;
static int mouse_drug=0;
static int seed_num = (int)(255*((double)rand()/(double)RAND_MAX));
void on_mouse (int event, int x, int y, int flags, void *param)
{
    int seed_rad = 10;
    CvPoint pt;

    if(mouse_drug==1) {
        //seed_num++;
        pt = cvPoint (x, y);
        cvCircle (markers, pt, seed_rad, cvScalarAll (seed_num), CV_FILLED, 8, 0);
        cvCircle (dsp_img, pt, seed_rad, cvScalarAll (seed_num), CV_FILLED, 8, 0);
        cvShowImage("image", dsp_img);
    }

    // (1)クリックにより中心を指定し，円形のシード領域を設定する
    if (event == CV_EVENT_LBUTTONDOWN) {
        seed_num = (int)(255*((double)rand()/(double)RAND_MAX)); //0;
        mouse_drug=1;
    }else if(event==CV_EVENT_LBUTTONUP){
        mouse_drug=0;
    }
}
wsp::State wsp::opencv::cvSegmWatershedPaintMarkers(int *o_segm, const uchar *in_img, u32 width, u32 height, u32 nChannels)
{
    if(o_segm==NULL || in_img==NULL){
        WSP_COMMON_ERROR_LOG("Input or output image is NULL\n");
        return WSP_STATE_FAILURE;
    }

    int w=(int)width, h=(int)height;
    
    int len = (u32)(w*h);
    int *idx;
    IplImage *src_img=NULL, *dst_gray_img=NULL, *dst_img=NULL;
    //IplImage *dsp_img=NULL, *markers=NULL;

    int depthType=IPL_DEPTH_8U;

    src_img = cvCreateImage(cvSize(w, h), depthType, (int)nChannels);

    printf("w:%d, h:%d, ws:%d\n", src_img->width, src_img->height, src_img->widthStep);

    wsp::opencv::cvConvertArrayToIplImage<uchar>(src_img, NULL, in_img, NULL, width, height, nChannels);

    dsp_img = cvCloneImage (src_img);
    dst_img = cvCloneImage (src_img);

    markers = cvCreateImage (cvGetSize(src_img), IPL_DEPTH_32S, 1);
    dst_gray_img = cvCreateImage (cvGetSize(src_img), IPL_DEPTH_8U, 1);
    cvZero (markers);

    // (3)入力画像を表示しシードコンポーネント指定のためのマウスイベントを登録する
    cvNamedWindow ("image", CV_WINDOW_AUTOSIZE);
    cvShowImage ("image", src_img);
    cvSetMouseCallback ("image", on_mouse, 0);
    cvWaitKey (0);

    // (4)watershed分割を実行する  
    //!--------------------------------------------------------------------------------
    //! void cvWatershed( const CvArr* image, CvArr* markers );
    //! image: 入力画像．8ビット，3チャンネル画像． 
    //! markers: 入出力画像．32ビットシングルチャンネルのマーカー画像（マップ）． 
    //! 関数 cvWatershed は，参考文献[Meyer92]にある，ノンパラメトリックマーカーベースのセグメンテーションアルゴリズムである
    //! watershedの一種を実装する． 画像をこの関数に渡す前に，ユーザーは大まかに画像markers中の処理対象領域を， 正（>0）の
    //! インデックスを用いて区切っておかなければならない．例えば，各領域がピクセル値として1,2,3...を持ち，一つあるいはそれ以上の
    //! 連結成分で表現されるなど．これらの成分は，処理における各画像領域の「シード」となる．区切られた領域との関係が未知なその他の
    //! 領域は，このアルゴリズムによって関係が定義される．関係が未知の領域の値は0にセットしておく必要がある．関数の出力時には，
    //! markers中の各ピクセル値は，シード成分に振られていた値か，領域の間に属する場合の-1，のいずれかがセットされる． 
    //! 連結成分のそれぞれ二つの隣接間が，watershed境界（値が-1のピクセル）で分離されている必要がないことに注意する．例えば，
    //! 初期マーカー画像中に接成分（tangent components）が存在する場合など．デモとこの関数の使用方法については，OpenCVサンプルディレクトリ中の
    //! watershed.cpp を参照． 
    cvWatershed (src_img, markers);
    //!--------------------------------------------------------------------------------

    // (5)実行結果の画像中のwatershed境界（ピクセル値=-1）を結果表示用画像上に表示する
    //! copy into destination array
    int blk_size = 3;
    {
        int x, y, c, index, ic;
        int *dst_ptr = o_segm;
        int *end_ptr = dst_ptr + width*height;
        int *marker_ptr = reinterpret_cast<int*>(markers->imageData);
        uchar *disp_segm_ptr = reinterpret_cast<uchar*>(dst_gray_img->imageData);
        uchar *disp_ptr = reinterpret_cast<uchar*>(dst_img->imageData);

#ifdef USE_OPTIMIZED_CODE
        for(x=0, y=0; dst_ptr!=end_ptr; ++dst_ptr, ++marker_ptr, disp_ptr+=nChannels, ++disp_segm_ptr, ++x){
            if(x==width){ x=0; ++y; }
            if(*marker_ptr == -1){
                for(c=0; c<nChannels; ++c){
                    *(disp_ptr+c) = 255;
                }
                if(x!=0 && x!=width-1 && y!=0 && y!=height-1){
                    //! find arounded non-boundry pixels 
                    int *blk_ptr = marker_ptr-width-1;
                    int *end_blk_ptr = marker_ptr+width+2;
                    for(c=0; blk_ptr!=end_blk_ptr; ++blk_ptr, ++c){
                        if(c==blk_size){ c=0; blk_ptr+=width-blk_size; }
                        if(*blk_ptr!=-1){
                            *dst_ptr = *blk_ptr; 
                            *disp_segm_ptr = *blk_ptr;
                            break; 
                        }
                    }
                }else{
                    *dst_ptr = 255;
                    *disp_segm_ptr = 255;
                }
                continue;
            }
            *dst_ptr = *marker_ptr;
            *disp_segm_ptr = static_cast<uchar>(*marker_ptr);
        }
#else
        for (y = 0; y<markers->height; ++y) {
            for (x = 0; x<markers->width; ++x) {
                index = y*width + x;
                idx = (int *) cvPtr2D(markers, y, x, NULL);
                if (marker_ptr[index] == -1){
                    ic = index * nChannels;
                    for(c=0; c<nChannels; ++c){
                        disp_ptr[ic+c] = 255;
                    }
                    disp_segm_ptr[index] = 255;
                    o_segm[index] = 255;
                    continue;
                }
                disp_segm_ptr[index] = static_cast<uchar>(marker_ptr[index]);
                dst_ptr[index] = disp_segm_ptr[index];
                dst_ptr[index] = marker_ptr[index];
            }
        }
#endif
    }


    cvDestroyWindow ("image");
    cvNamedWindow ("watershed transform", CV_WINDOW_AUTOSIZE);
    cvShowImage ("watershed transform", dst_img);
    cvWaitKey(0);


    //wsp::opencv::cvConvertIplImageToArray(o_segm, NULL, dst_gray_img, NULL);

    cvDestroyWindow ("watershed transform");
    cvReleaseImage (&markers);
    cvReleaseImage (&dsp_img);
    cvReleaseImage (&src_img);
    cvReleaseImage (&dst_img);
    cvReleaseImage (&dst_gray_img);

    return WSP_STATE_SUCCESS;
}

wsp::State wsp::opencv::cvSegmWatershed(int *io_segm, const uchar *in_img, u32 width, u32 height, u32 nChannels)
{
    if(io_segm==NULL || in_img==NULL){
        WSP_COMMON_ERROR_LOG("Input or output image is NULL\n");
        return WSP_STATE_FAILURE;
    }

    int w=(int)width, h=(int)height;
    int x, y;
    int len = (u32)(w*h);
    int *idx, id, i, j;
    IplImage *src_img=NULL, *dst_gray_img=NULL, *marker_img=NULL;
    //IplImage *dsp_img=NULL, *markers=NULL;

    int depthType=IPL_DEPTH_8U;

    src_img = cvCreateImage(cvSize(w, h), depthType, (int)nChannels);
    marker_img = cvCreateImage (cvSize(w, h), IPL_DEPTH_32S, 1);
    dst_gray_img = cvCreateImage (cvSize(w, h), IPL_DEPTH_8U, 1);

    printf("w:%d, h:%d, ws:%d\n", src_img->width, src_img->height, src_img->widthStep);

    wsp::opencv::cvConvertArrayToIplImage<uchar>(src_img, NULL, in_img, NULL, width, height, nChannels);
    wsp::opencv::cvConvertArrayToIplImage<int>(marker_img, NULL, io_segm, NULL, width, height, 1);

    //cvNamedWindow ("watershed transform", CV_WINDOW_AUTOSIZE);
    //cvShowImage ("watershed transform", src_img);
    //cvWaitKey (0);
    //cvDestroyWindow ("watershed transform");

    cvWatershed(src_img, marker_img);

    //int *data = (int*)marker_img->imageData;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            //idx = (int *) cvPtr2D(marker_img, i, j, NULL);
            id = *((int*)(&marker_img->imageData[i*marker_img->widthStep + j*sizeof(int)]));
            if(id==-1){
                dst_gray_img->imageData[i*dst_gray_img->widthStep + j]=0;
                continue;
            }
            dst_gray_img->imageData[i*dst_gray_img->widthStep + j]=(uchar)id;
            //printf("%d\n", id);
            //if (*idx == -1){
            //    cvSet2D(dst_gray_img, i, j, cvScalarAll (255));
            //    continue;
            //}
            //cvSet2D(dst_gray_img, i, j, cvScalarAll(*idx));
        }
    }

    //cvNamedWindow ("watershed transform", CV_WINDOW_AUTOSIZE);
    //cvShowImage ("watershed transform", dst_gray_img);
    //cvWaitKey (0);
    //cvDestroyWindow ("watershed transform");

    wsp::opencv::cvConvertIplImageToArray<int>(io_segm, NULL, marker_img, NULL);

    cvReleaseImage (&marker_img);
    cvReleaseImage (&src_img);

    return WSP_STATE_SUCCESS;
}
