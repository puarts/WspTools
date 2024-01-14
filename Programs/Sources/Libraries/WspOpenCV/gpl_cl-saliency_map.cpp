/**
* @file gpl_cl-saliency_map.cpp
* @author Junichi Nishikata
*/

#include "gpl_cl-saliency_map.hpp"

#include <wsp/image/_tpl_fn-imgfilter.hpp>

#define SALIENCY_PYRAM_DEPTH 9

void FMGaussianPyrCenterSurroundDifferece(CvMat* src, CvMat* dst[6]);
void CreateGaussianPyr(CvMat* src, CvMat* dst[SALIENCY_PYRAM_DEPTH]);
void GetCenterSurroundDiff(CvMat* GaussianMap[SALIENCY_PYRAM_DEPTH], CvMat* dst[6]);
double SMAvgLocalMax(CvMat* src);



/* Constructor */

wsp::opencv::cvSaliencyMap::cvSaliencyMap(
    int height, int width, 
    const wsp::opencv::SaliencyMapParam &in_param
    )
{    
    // previous frame information
    prev_frame = NULL;

    int kernel_size_x = in_param.ori_gabor_kernel_size;
    if( (kernel_size_x%2) == 0 ){
        ++kernel_size_x;
    }
    int kernel_size_y = kernel_size_x;

    //Set Gabor Kernel (9x9)
    GaborKernel0   = cvCreateMat(kernel_size_x, kernel_size_y, CV_32FC1);
    GaborKernel45  = cvCreateMat(kernel_size_x, kernel_size_y, CV_32FC1);
    GaborKernel90  = cvCreateMat(kernel_size_x, kernel_size_y, CV_32FC1);
    GaborKernel135 = cvCreateMat(kernel_size_x, kernel_size_y, CV_32FC1);

    wsp::img::GaborFilterParam param;

    param.cos_frequency    = in_param.ori_cos_frequency;
    param.cos_offset_rad   = in_param.ori_cos_offset_rad;
    param.gauss_dist_sigma = in_param.ori_gauss_sigma;


    // 0 degree orientation
    param.ori_angle_rad = 0.0;
    for(int y=0; y<kernel_size_y; ++y) 
    {
        for(int x=0; x<kernel_size_x; ++x)
        {
            cvmSet(GaborKernel0, y, x, wsp::img::GetGaborWeight( x, y, kernel_size_x, kernel_size_y, param ));     
        }
    }

    // 45 degree orientation
    param.ori_angle_rad = 45.0*M_PI/180.0;
    for(int y=0; y<kernel_size_y; ++y) 
    {
        for(int x=0; x<kernel_size_x; ++x)
        {
            cvmSet(GaborKernel45, y, x, wsp::img::GetGaborWeight( x, y, kernel_size_x, kernel_size_y, param )); 
        }
    }
    // 90 degree orientation
    param.ori_angle_rad = 90.0*M_PI/180.0;
    for(int y=0; y<kernel_size_y; ++y) 
    {
        for(int x=0; x<kernel_size_x; ++x)
        {
            cvmSet(GaborKernel90, y, x, wsp::img::GetGaborWeight( x, y, kernel_size_x, kernel_size_y, param )); 
        }
    }
    // 135 degree orientation
    param.ori_angle_rad = 135.0*M_PI/180.0;
    for(int y=0; y<kernel_size_y; ++y) 
    {
        for(int x=0; x<kernel_size_x; ++x)
        {
            cvmSet(GaborKernel135, y, x, wsp::img::GetGaborWeight( x, y, kernel_size_x, kernel_size_y, param )); 
        }
    }

}


//まずは，コア関数であるGetSaliencyMapの実装を示します．
//オリジナル http://ilab.usc.edu/publications/Itti_etal98pami.html で用いられていた，輝度・色差・エッジ方向に加えて，映像応用を見据えて動きベクトルも基本特徴として加えてあります．
//また，各基本特徴に重みをつけて統合する機能も含まれています．
CvMat* wsp::opencv::cvSaliencyMap::GetSaliencyMap(IplImage *src, wsp::opencv::SaliencyMapParam &param)
{
    int input_width = src->width;    // width of the image
    int input_height = src->height;    // height of the image
    CvSize src_size = cvSize(input_width, input_height);
    saliency_map_param_ = param;

    //======================================================================================
    // Intensity and RGB Extraction
    //======================================================================================
    CvMat *red_map, *green_map, *blue_map, *intensity_map;
    ExtractRGBI(src, red_map, green_map, blue_map, intensity_map);

    //======================================================================================
    // Feature Map Extraction
    //======================================================================================

    // intensity feature maps
    CvMat* intensity_feature_map[6];
    memset(intensity_feature_map, 0, sizeof(intensity_feature_map));
    if( saliency_map_param_.weight_intensity != 0.0 ){
        GetIntensityFeatureMap(intensity_map, intensity_feature_map);
    }

    // color feature maps
    CvMat* red_green_feature_map[6];
    CvMat* blue_yellow_feature_map[6];
    memset(red_green_feature_map, 0, sizeof(red_green_feature_map));
    memset(blue_yellow_feature_map, 0, sizeof(blue_yellow_feature_map));
    if( saliency_map_param_.weight_color != 0.0 ){
        GetColorFeatureMap(red_map, green_map, blue_map, red_green_feature_map, blue_yellow_feature_map);
    }

    // orientation feature maps
    CvMat* ori_feature_map[24];
    memset(ori_feature_map, 0, sizeof(ori_feature_map));
    if( saliency_map_param_.weight_orientation != 0.0 ){
        GetOrientationFeatureMap(intensity_map, ori_feature_map);
    }

    // motion feature maps
    CvMat* motion_x_feature_map[6];
    CvMat* motion_y_feature_map[6];
    memset(motion_x_feature_map, 0, sizeof(motion_x_feature_map));
    memset(motion_y_feature_map, 0, sizeof(motion_y_feature_map));
    if( saliency_map_param_.weight_motion != 0.0 ){
        GetMotionFeatureMap(intensity_map, motion_x_feature_map, motion_y_feature_map);
    }

    cvReleaseMat(&red_map);
    cvReleaseMat(&green_map);
    cvReleaseMat(&blue_map);
    cvReleaseMat(&intensity_map);

    //======================================================================================
    // Conspicuity Map Generation
    //          and
    // Saliency Map Generation
    //======================================================================================
    CvMat *intensity_conspicuity_map_norm = NULL;
    CvMat *col_conspicuity_map_norm = NULL;
    CvMat *ori_conspicuity_map_norm = NULL;
    CvMat *motion_conspicuity_map_norm = NULL;

    if( saliency_map_param_.weight_intensity != 0.0 ){
        CvMat *intensity_conspicuity_map = NULL;
        intensity_conspicuity_map = GetIntensityConspicuityMaps(intensity_feature_map, src_size);
        for(int i=0; i<6; ++i)
        {
            cvReleaseMat(&intensity_feature_map[i]);
        }
        // Normalize conspicuity maps
        intensity_conspicuity_map_norm = ApplyIttiNormalization(intensity_conspicuity_map);
        cvReleaseMat(&intensity_conspicuity_map);
    }
    if( saliency_map_param_.weight_color != 0.0 ){
        CvMat *col_conspicuity_map = NULL;
        col_conspicuity_map = GetColorConspicuityMaps(red_green_feature_map, blue_yellow_feature_map, src_size);
        for(int i=0; i<6; ++i)
        {
            cvReleaseMat(&red_green_feature_map[i]);
            cvReleaseMat(&blue_yellow_feature_map[i]);
        }
        col_conspicuity_map_norm = ApplyIttiNormalization(col_conspicuity_map);
        cvReleaseMat(&col_conspicuity_map);
    }
    if( saliency_map_param_.weight_orientation != 0.0 ){
        CvMat *ori_conspicuity_map = NULL;
        ori_conspicuity_map = GetOrientationConspicuityMaps(ori_feature_map, src_size);
        for(int i=0; i<24; ++i)
        { 
            cvReleaseMat(&ori_feature_map[i]);
        }
        ori_conspicuity_map_norm = ApplyIttiNormalization(ori_conspicuity_map);
        cvReleaseMat(&ori_conspicuity_map);
    }
    if( saliency_map_param_.weight_motion != 0.0 ){
        CvMat *motion_conspicuity_map = NULL;
        motion_conspicuity_map = GetMotionConspicuityMaps(motion_x_feature_map, motion_y_feature_map, src_size);
        for(int i=0; i<6; ++i)
        {
            cvReleaseMat(&motion_x_feature_map[i]);
            cvReleaseMat(&motion_y_feature_map[i]);
        }
        motion_conspicuity_map_norm = ApplyIttiNormalization(motion_conspicuity_map);
        cvReleaseMat(&motion_conspicuity_map);
    }


    // Adding all the CMs to form Saliency Map
    CvMat* saliency_map = cvCreateMat(src_size.height, src_size.width, CV_32FC1);
    memset( saliency_map->data.ptr, 0, saliency_map->height * saliency_map->step);

    if( saliency_map_param_.weight_intensity   != 0.0 ){
        cvAddWeighted(
            intensity_conspicuity_map_norm, saliency_map_param_.weight_intensity, 
            saliency_map, 1.00, 
            0.0, saliency_map
            );
        cvReleaseMat(&intensity_conspicuity_map_norm);
    }
    if( saliency_map_param_.weight_orientation != 0.0){
        cvAddWeighted(
            ori_conspicuity_map_norm, saliency_map_param_.weight_orientation, 
            saliency_map, 1.00,
            0.0, saliency_map
            );
        cvReleaseMat(&ori_conspicuity_map_norm);
    }
    if( saliency_map_param_.weight_color != 0.0 ){
        cvAddWeighted(
            col_conspicuity_map_norm, saliency_map_param_.weight_color, 
            saliency_map,   1.00, 
            0.0, saliency_map
            );
        cvReleaseMat(&col_conspicuity_map_norm);
    }
    if( saliency_map_param_.weight_motion != 0.0 ){
        cvAddWeighted(
            motion_conspicuity_map_norm, saliency_map_param_.weight_motion, 
            saliency_map,   1.00, 
            0.0, saliency_map
            );
        cvReleaseMat(&motion_conspicuity_map_norm);
    }

    // Output Result Map
    CvMat* saliency_map_norm = NormalizeDynamicRange(saliency_map);
    cvReleaseMat(&saliency_map);

    return saliency_map_norm;
}

//以降に，各関数の実装を記載します．
//
//画像のチャネル分解 ExtractRGBI
//
//単純に，RGB画像を各チャネルに分解し，同時に輝度画像も取り出します．
void wsp::opencv::cvSaliencyMap::ExtractRGBI(IplImage* inputImage, CvMat* &R, CvMat* &G, CvMat* &B, CvMat* &I)
{

    int height = inputImage->height;
    int width = inputImage->width;
    // convert scale of array elements
    CvMat * src = cvCreateMat(height, width, CV_32FC3);
    cvConvertScale(inputImage, src, 1/256.0);

    // initalize matrix for I,R,G,B
    R = cvCreateMat(height, width, CV_32FC1);
    G = cvCreateMat(height, width, CV_32FC1);
    B = cvCreateMat(height, width, CV_32FC1);
    I = cvCreateMat(height, width, CV_32FC1);

    // split
    cvSplit(src, B, G, R, NULL);

    // extract intensity image
    cvCvtColor(src, I, CV_BGR2GRAY);

    // release
    cvReleaseMat(&src);

}

//Feature mapの作成 FMGetFM
//
//まず，輝度・色差・エッジ方向・動きベクトルの各基本特徴を取り出します．
//次に，それぞれの多重解像度表現を導出します．元の解像度の画像を含めて9階層の画像を生成します．
//最後に，階層間の差分画像を取ることで，近似的にDoFフィルタ画像を抽出し，これをfeature mapとします．

void wsp::opencv::cvSaliencyMap::GetIntensityFeatureMap(CvMat* src, CvMat* dst[6])
{
    FMGaussianPyrCenterSurroundDifferece(src, dst);    
}

void wsp::opencv::cvSaliencyMap::GetColorFeatureMap(CvMat* R, CvMat* G, CvMat* B, CvMat* RGFM[6], CvMat* BYFM[6])
{
    // allocate
    int height = R->height;
    int width = R->width;
    CvMat* tmp1 = cvCreateMat(height, width, CV_32FC1);
    CvMat* tmp2 = cvCreateMat(height, width, CV_32FC1);
    CvMat* RGBMax = cvCreateMat(height, width, CV_32FC1);
    CvMat* RGMin = cvCreateMat(height, width, CV_32FC1);
    CvMat* RGMat = cvCreateMat(height, width, CV_32FC1);
    CvMat* BYMat = cvCreateMat(height, width, CV_32FC1);
    // Max(R,G,B)
    cvMax(R, G, tmp1);
    cvMax(B, tmp1, RGBMax);
    cvMaxS(RGBMax, 0.0001, RGBMax); // to prevent dividing by 0
    // Min(R,G)
    cvMin(R, G, RGMin);

    // R-G
    cvSub(R, G, tmp1);
    // B-Min(R,G)
    cvSub(B, RGMin, tmp2);
    // RG = (R-G)/Max(R,G,B)
    cvDiv(tmp1, RGBMax, RGMat);
    // BY = (B-Min(R,G)/Max(R,G,B)
    cvDiv(tmp2, RGBMax, BYMat);

    // Clamp negative value to 0 for the RG and BY maps
    cvMaxS(RGMat, 0, RGMat);
    cvMaxS(BYMat, 0, BYMat);

    // Obtain [RG,BY] color opponency feature map by generating Gaussian pyramid and performing center-surround difference
    FMGaussianPyrCenterSurroundDifferece(RGMat, RGFM);
    FMGaussianPyrCenterSurroundDifferece(BYMat, BYFM);

    // release
    cvReleaseMat(&tmp1);    
    cvReleaseMat(&tmp2);    
    cvReleaseMat(&RGBMax);    
    cvReleaseMat(&RGMin);    
    cvReleaseMat(&RGMat);
    cvReleaseMat(&BYMat);
}

void wsp::opencv::cvSaliencyMap::GetOrientationFeatureMap(CvMat* intensity_map, CvMat* dst[24])
{
    // Create gaussian pyramid
    CvMat* gaussian_intensity_pyr[SALIENCY_PYRAM_DEPTH];
    CreateGaussianPyr(intensity_map, gaussian_intensity_pyr);

    // Convolution Gabor filter with intensity feature maps to extract orientation feature
    CvMat* tempGaborOutput0[SALIENCY_PYRAM_DEPTH];
    CvMat* tempGaborOutput45[SALIENCY_PYRAM_DEPTH];
    CvMat* tempGaborOutput90[SALIENCY_PYRAM_DEPTH];
    CvMat* tempGaborOutput135[SALIENCY_PYRAM_DEPTH];
    for(int j=2; j < SALIENCY_PYRAM_DEPTH; ++j)
    {
        int current_height = gaussian_intensity_pyr[j]->height;
        int current_width = gaussian_intensity_pyr[j]->width;

        tempGaborOutput0[j] = cvCreateMat(current_height, current_width, CV_32FC1);
        tempGaborOutput45[j] = cvCreateMat(current_height, current_width, CV_32FC1);
        tempGaborOutput90[j] = cvCreateMat(current_height, current_width, CV_32FC1);
        tempGaborOutput135[j] = cvCreateMat(current_height, current_width, CV_32FC1);

        cvFilter2D(gaussian_intensity_pyr[j], tempGaborOutput0[j], GaborKernel0);
        cvFilter2D(gaussian_intensity_pyr[j], tempGaborOutput45[j], GaborKernel45);
        cvFilter2D(gaussian_intensity_pyr[j], tempGaborOutput90[j], GaborKernel90);
        cvFilter2D(gaussian_intensity_pyr[j], tempGaborOutput135[j], GaborKernel135);
    }
    for(int j=0; j < SALIENCY_PYRAM_DEPTH; ++j){ 
        cvReleaseMat(&(gaussian_intensity_pyr[j]));
    }

    // calculate center surround difference for each orientation
    CvMat *temp0[6];
    CvMat *temp45[6];
    CvMat *temp90[6];
    CvMat *temp135[6];
    GetCenterSurroundDiff(tempGaborOutput0, temp0);
    GetCenterSurroundDiff(tempGaborOutput45, temp45);
    GetCenterSurroundDiff(tempGaborOutput90, temp90);
    GetCenterSurroundDiff(tempGaborOutput135, temp135);
    for(int i=2; i < SALIENCY_PYRAM_DEPTH; ++i)
    {
        cvReleaseMat(&(tempGaborOutput0[i]));
        cvReleaseMat(&(tempGaborOutput45[i]));
        cvReleaseMat(&(tempGaborOutput90[i]));
        cvReleaseMat(&(tempGaborOutput135[i]));
    }
    // saving the 6 center-surround difference feature map of each angle configuration to the destination pointer
    for(int i=0; i<6; ++i)
    {
        dst[i] = temp0[i];
        dst[i+6] = temp45[i];
        dst[i+12] = temp90[i];
        dst[i+18] = temp135[i];
    }
}

void wsp::opencv::cvSaliencyMap::GetMotionFeatureMap(CvMat* I, CvMat* dst_x[], CvMat* dst_y[])
{

    int height = I->height;
    int width = I->width;
    // convert
    CvMat* I8U = cvCreateMat(height, width, CV_8UC1);
    cvConvertScale(I, I8U, 256);

    // obtain optical flow information
    CvMat* flowx = cvCreateMat(height, width, CV_32FC1);
    CvMat* flowy = cvCreateMat(height, width, CV_32FC1);
    cvSetZero(flowx);
    cvSetZero(flowy);
    if(this->prev_frame!=NULL)
    {
        cvCalcOpticalFlowLK(this->prev_frame, I8U, cvSize(7,7), flowx, flowy);
        cvReleaseMat(&(this->prev_frame));
    }
    // create Gaussian pyramid
    FMGaussianPyrCenterSurroundDifferece(flowx, dst_x);
    FMGaussianPyrCenterSurroundDifferece(flowy, dst_y);

    // update
    this->prev_frame = cvCloneMat(I8U);

    // release
    cvReleaseMat(&flowx);
    cvReleaseMat(&flowy);
    cvReleaseMat(&I8U);
}

void FMGaussianPyrCenterSurroundDifferece(CvMat* src, CvMat* dst[6])
{

    CvMat *GaussianMap[SALIENCY_PYRAM_DEPTH];
    CreateGaussianPyr(src, GaussianMap);
    GetCenterSurroundDiff(GaussianMap, dst);
    for(int i=0; i < SALIENCY_PYRAM_DEPTH; ++i){ 
        cvReleaseMat(&(GaussianMap[i]));
    }

}

void CreateGaussianPyr(CvMat* src, CvMat* dst[SALIENCY_PYRAM_DEPTH])
{

    dst[0] = cvCloneMat(src);
    for(int i=1; i < SALIENCY_PYRAM_DEPTH; ++i)
    {
        int pyr_down_h = dst[i-1]->height/2;
        int pyr_down_w = dst[i-1]->width/2;
        if( pyr_down_h==0 || pyr_down_w==0 )
        {
            pyr_down_h = 1;
            pyr_down_w = 1;
            for( ; i < SALIENCY_PYRAM_DEPTH; ++i)
            {
                WSP_COMMON_DEBUG_LOG(
                    "pyramid down (width, height) = (%d, %d)\n"
                    , pyr_down_w, pyr_down_h
                );
                dst[i] = cvCreateMat(pyr_down_h, pyr_down_w, CV_32FC1);
                memset( dst[i]->data.fl, 0, dst[i]->rows * dst[i]->cols * sizeof(float) );
            }
            return;
        }
        WSP_COMMON_DEBUG_LOG(
            "pyramid down (width, height) = (%d, %d)\n"
            , pyr_down_w, pyr_down_h
        );

        dst[i] = cvCreateMat(pyr_down_h, pyr_down_w, CV_32FC1);
        cvPyrDown(dst[i-1], dst[i], CV_GAUSSIAN_5x5);
    }

}

void GetCenterSurroundDiff(CvMat* GaussianMap[SALIENCY_PYRAM_DEPTH], CvMat* dst[6])
{

    int i=0;
    for(int s=2; s<5; s++, i+=2)
    {
        int current_height = GaussianMap[s]->height;
        int current_width  = GaussianMap[s]->width;
        CvMat * tmp_mat    = cvCreateMat(current_height, current_width, CV_32FC1);

        dst[i]   = cvCreateMat(current_height, current_width, CV_32FC1);
        dst[i+1] = cvCreateMat(current_height, current_width, CV_32FC1);

        cvResize(GaussianMap[s+3], tmp_mat, CV_INTER_LINEAR);
        cvAbsDiff(GaussianMap[s], tmp_mat, dst[i]);

        cvResize(GaussianMap[s+4], tmp_mat, CV_INTER_LINEAR);
        cvAbsDiff(GaussianMap[s], tmp_mat, dst[i+1]);

        cvReleaseMat(&tmp_mat);
    }

}

//各種正規化処理
//
//Saliency map抽出には，2種類の正規化処理が含まれます．
//1つは一般的な値の正規化 (NormalizeDynamicRange)，もう1つがノイズ抑制のための正規化 (ApplyIttiNormalization) となります．
void wsp::opencv::cvSaliencyMap::NormalizeFeatureMaps(
    CvMat *in_feature_maps[], 
    CvMat *o_norm_feature_maps[], 
    int width, int height, int num_maps
)
{
    for(int i=0; i<num_maps; ++i)
    {
        CvMat * normalizedImage = ApplyIttiNormalization(in_feature_maps[i]);
        o_norm_feature_maps[i] = cvCreateMat(height, width, CV_32FC1);
        cvResize(normalizedImage, o_norm_feature_maps[i], CV_INTER_LINEAR);
        cvReleaseMat(&normalizedImage);
    }

}

CvMat* wsp::opencv::cvSaliencyMap::ApplyIttiNormalization(CvMat* src)
{
    CvMat* result = cvCreateMat(src->height, src->width, CV_32FC1);

    // normalize so that the pixel value lies between 0 and 1
    CvMat* tempResult = NormalizeDynamicRange(src);    
    // single-peak emphasis / multi-peak suppression
    double lmaxmean = SMAvgLocalMax(tempResult);
    double normCoeff = (1-lmaxmean)*(1-lmaxmean);
    cvConvertScale(tempResult, result, normCoeff);
    cvReleaseMat(&tempResult);    
    return result;    
}

CvMat* wsp::opencv::cvSaliencyMap::NormalizeDynamicRange(CvMat* src)
{

    double maxx, minn;
    cvMinMaxLoc(src, &minn, &maxx);
    CvMat* result = cvCreateMat(src->height, src->width, CV_32FC1);
    if(maxx!=minn) cvConvertScale(src, result, 1/(maxx-minn), minn/(minn-maxx));
    else cvConvertScale(src, result, 1, -minn);
    return result;

}

double SMAvgLocalMax(CvMat* src)
{    

    int stepsize = DEFAULT_STEP_LOCAL;
    int numlocal = 0;
    double lmaxmean = 0, lmax = 0, dummy = 0;
    CvMat localMatHeader;
    cvInitMatHeader(&localMatHeader, stepsize, stepsize, CV_32FC1, src);
    for(int y=0; y<src->height-stepsize; y+=stepsize) // Note: the last several pixels may be ignored.
    {
        for(int x=0; x<src->width-stepsize; x+=stepsize)
        {
            localMatHeader.data.ptr = src->data.ptr+sizeof(float)*x+src->step*y;    // get local matrix by pointer trick
            cvMinMaxLoc(&localMatHeader, &dummy, &lmax);
            lmaxmean += lmax;
            numlocal++;
        }
    }
    return lmaxmean/numlocal;
}
//Conspicuity mapの生成 CMGetCM
//
//Conspicuity mapは，いわばsaliency mapを構成する直前の中間表現で，各基本特徴から抽出したfeature mapsを正規化して統合した結果です．

CvMat * wsp::opencv::cvSaliencyMap::GetIntensityConspicuityMaps(CvMat *intensity_feature_map[], CvSize size)
{

    int num_FMs = 6;
    // Normalize all intensity feature maps
    CvMat * Nintensity_feature_map[6];
    NormalizeFeatureMaps(intensity_feature_map, Nintensity_feature_map, size.width, size.height, num_FMs);

    // Formulate intensity conspicuity map by summing up the normalized intensity feature maps
    CvMat *intensity_conspicuity_map = cvCreateMat(size.height, size.width, CV_32FC1);
    cvSetZero(intensity_conspicuity_map);
    for (int i=0; i<num_FMs; ++i)
    {
        cvAdd(intensity_conspicuity_map, Nintensity_feature_map[i], intensity_conspicuity_map);
        cvReleaseMat(&Nintensity_feature_map[i]);    
    }
    return intensity_conspicuity_map;

}

CvMat * wsp::opencv::cvSaliencyMap::GetColorConspicuityMaps(CvMat *red_green_feature_map[], CvMat *blue_yellow_feature_map[], CvSize size)
{

    int num_FMs = 6;
    CvMat* col_conspicuity_map_RG = GetIntensityConspicuityMaps(red_green_feature_map, size);
    CvMat* col_conspicuity_map_BY = GetIntensityConspicuityMaps(blue_yellow_feature_map, size);
    CvMat *col_conspicuity_map = cvCreateMat(size.height, size.width, CV_32FC1);
    cvAdd(col_conspicuity_map_BY, col_conspicuity_map_RG, col_conspicuity_map);

    cvReleaseMat(&col_conspicuity_map_BY);
    cvReleaseMat(&col_conspicuity_map_RG);

    return col_conspicuity_map;

}

CvMat* wsp::opencv::cvSaliencyMap::GetOrientationConspicuityMaps(CvMat *ori_feature_map[], CvSize size)
{
    int num_feature_maps_per_angle = 6;
    int num_angles = 4;
    int num_FMs = num_feature_maps_per_angle * num_angles;

    // split feature maps into four sets
    CvMat *ori_feature_map0[6];
    CvMat *ori_feature_map45[6];
    CvMat *ori_feature_map90[6];
    CvMat *ori_feature_map135[6];
    for (int i=0; i<num_feature_maps_per_angle; ++i)
    {
        ori_feature_map0[i]   = ori_feature_map[0*num_feature_maps_per_angle + i];
        ori_feature_map45[i]  = ori_feature_map[1*num_feature_maps_per_angle + i];
        ori_feature_map90[i]  = ori_feature_map[2*num_feature_maps_per_angle + i];
        ori_feature_map135[i] = ori_feature_map[3*num_feature_maps_per_angle + i];
    }

    // extract conspicuity map for each angle
    CvMat * norm_ori_feature_map_tmp[4];
    norm_ori_feature_map_tmp[0] = GetIntensityConspicuityMaps(ori_feature_map0,   size);
    norm_ori_feature_map_tmp[1] = GetIntensityConspicuityMaps(ori_feature_map45,  size);
    norm_ori_feature_map_tmp[2] = GetIntensityConspicuityMaps(ori_feature_map90,  size);
    norm_ori_feature_map_tmp[3] = GetIntensityConspicuityMaps(ori_feature_map135, size);

    // Normalize all orientation features map grouped by their orientation angles
    CvMat* norm_ori_feature_map[4];
    for (int i=0; i<4; ++i)
    {
        norm_ori_feature_map[i] = ApplyIttiNormalization(norm_ori_feature_map_tmp[i]);
        cvReleaseMat(&norm_ori_feature_map_tmp[i]);
    }

    // Sum up all orientation feature maps, and form orientation conspicuity map
    CvMat *ori_conspicuity_map = cvCreateMat(size.height, size.width, CV_32FC1);
    cvSetZero(ori_conspicuity_map);
    for(int i=0; i<4; ++i)
    {
        cvAdd(norm_ori_feature_map[i], ori_conspicuity_map, ori_conspicuity_map);
        cvReleaseMat(&norm_ori_feature_map[i]);    
    }

    return ori_conspicuity_map;
}

CvMat* wsp::opencv::cvSaliencyMap::GetMotionConspicuityMaps(CvMat *motion_x_feature_map[], CvMat *motion_y_feature_map[], CvSize size)
{
    return GetColorConspicuityMaps(motion_x_feature_map, motion_y_feature_map, size);
}
