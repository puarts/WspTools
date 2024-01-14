/**
 * @file gpl_cl-saliency_map.h
 * @author Junichi Nishikata
 */


#ifndef WSP_IMAGE_GPL_CLASS_SALIENCY_MAP_HPP_
#define WSP_IMAGE_GPL_CLASS_SALIENCY_MAP_HPP_

//******************************************************************************
// Include Files
//******************************************************************************

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <tchar.h>

#ifndef __WSP_IMAGEUTIL_DEFINE_IMAGEUTIL_H__
#include "_define_image.h"
#endif

#ifdef USE_OPENCV
#include <opencv/cv.h>
#include <opencv/highgui.h>
//#include <opencv/videoInput.h>

//******************************************************************************
// Constant & Definitions
//******************************************************************************

static const float RANGEMAX     = 255.00;
static const float SCALE_GAUSS_PYRAMID    = 1.7782794100389228012254211951927;    // = 100^0.125
static const float DEFAULT_STEP_LOCAL    = 8;

//******************************************************************************
// Class Definition
//******************************************************************************
typedef struct{
    float weight_intensity;
    float weight_color;
    float weight_orientation;
    float weight_motion;
    int   ori_gabor_kernel_size;
    float ori_gauss_sigma;
    float ori_cos_frequency;
    float ori_cos_offset_rad;
} WSP_SaliencyMapParam;

inline void WSP_SaliencyMapParam_SetToDefault( WSP_SaliencyMapParam *param )
{
    (*param).weight_intensity      = 0.30f;
    (*param).weight_color          = 0.30f;
    (*param).weight_orientation    = 0.20f;
    (*param).weight_motion         = 0.20f;
    (*param).ori_gabor_kernel_size = 11;
    (*param).ori_gauss_sigma       = 1.7f;
    (*param).ori_cos_frequency     = 1.0f;
    (*param).ori_cos_offset_rad    = 0.0f;
}

namespace wsp{ namespace img{
    class SaliencyMapParam
        : public WSP_SaliencyMapParam
    {
    public:
        SaliencyMapParam(){
            WSP_SaliencyMapParam_SetToDefault( this );
        }
    };
}}

namespace wsp{
    class WSP_DLL_EXPORT cvSaliencyMap
    {
    public:
        cvSaliencyMap(int height, int width, const wsp::img::SaliencyMapParam &param);

        /**
         * @brief Core function for saliency map generation. 
         */
        CvMat* GetSaliencyMap(IplImage *src, wsp::img::SaliencyMapParam &param);

        CvMat* GetSaliencyMapFromVideoFrame(CvCapture * input_video, IplImage * &inputFrame_cur, int frameNo);

        //CvMat* GetSaliencyMapFromVideoFrameWebcam(videoInput &vi, int dev_id, IplImage * &inputFrame_cur);
        //~ParallelcvSaliencyMap(void);

    private:

        //CvMat *R, *G, *B, *I;
        CvMat *prev_frame;
        CvMat *GaborKernel0;
        CvMat *GaborKernel45;
        CvMat *GaborKernel90;
        CvMat *GaborKernel135;

        wsp::img::SaliencyMapParam saliency_map_param_;

    private:

        /** @brief  Splitting color channels. */
        void ExtractRGBI(IplImage * inputImage, CvMat * &R, CvMat * &G, CvMat * &B, CvMat * &I);

        /** @brief  Extracting feature maps. */
        void GetIntensityFeatureMap(CvMat * src, CvMat * dst[6]);
        void GetColorFeatureMap(
            CvMat *red_map, 
            CvMat *green_map, 
            CvMat *blue_map, 
            CvMat *red_green_feature_maps[6], 
            CvMat *blue_yellow_feature_maps[6]
        );
        void GetOrientationFeatureMap(CvMat *intensity_map, CvMat * dst[24]);
        void GetMotionFeatureMap(CvMat *intensity_map, CvMat *dst_x[6], CvMat *dst_y[6]);

        /** @brief  Normalization. */
        void NormalizeFeatureMaps(CvMat * FM[6], CvMat * NFM[6], int width, int height, int num_maps);

        /** @brief  Itti normalization */
        CvMat* ApplyIttiNormalization(CvMat * src);    

        /** @brief  dynamic range normalization */
        CvMat* NormalizeDynamicRange(CvMat * src);    

        /** @brief  Extracting conspicuity maps. */
        CvMat* GetIntensityConspicuityMaps(CvMat *IFM[6], CvSize size);
        CvMat* GetColorConspicuityMaps(CvMat *CFM_RG[6], CvMat *CFM_BY[6], CvSize size);
        CvMat* GetOrientationConspicuityMaps(CvMat *OFM[24], CvSize size);
        CvMat* GetMotionConspicuityMaps(CvMat *MFM_X[6], CvMat *MFM_Y[6], CvSize size);

    };
}

#endif

#endif
