// @file tpl_cl-stereoimage.h
// 

#ifndef _WSP_IMAGE_TEMPLATE_CLASS_STEREO_IMAGE_H_
#define _WSP_IMAGE_TEMPLATE_CLASS_STEREO_IMAGE_H_

#include "tpl_cl-image.h"
#include "tpl_fn-imgproc.hpp"
#include "fn-imgproc.h"

namespace wsp{
    template<typename T>
    class StereoImage
    {
    private:
        wsp::Image<T> *m_imgL;
        wsp::Image<T> *m_imgR;
        wsp::Image<double> *m_depthL;
        wsp::Image<double> *m_depthR;

    private:
        void init();

    public:
        StereoImage();
        StereoImage(wsp::Image<T> &leftImg, wsp::Image<T> &rightImg);
        ~StereoImage();

        // Getter
        inline const wsp::Image<T>& GetImgL() const;
        inline const wsp::Image<T>& GetImgR() const;
        inline const wsp::Image<double>& GetDepthL() const;
        inline const wsp::Image<double>& GetDepthR() const;

        // Setter
        inline void SetImgL(const wsp::Image<T> &in_img);
        inline void SetImgR(const wsp::Image<T> &in_img);

        wsp::State ComputeDepth(int search_min, int search_max, u32 block_min, u32 block_max);

        // I/O
        inline wsp::State LoadImgL(const char *fileName);
        inline wsp::State LoadImgR(const char *fileName);
        inline wsp::State SaveDepthL(const char *fileName);
        inline wsp::State SaveDepthR(const char *fileName);
    };
}


// Constructor, Destructor -------------------------------------
#define INIT_StereoImage m_imgL(NULL),m_imgR(NULL),m_depthL(NULL), m_depthR(NULL)

template<typename T>
void wsp::StereoImage<T>::init(){
    m_imgL = WSP_NEW wsp::Image<T>();
    m_imgR = WSP_NEW wsp::Image<T>();
    m_depthL = WSP_NEW wsp::Image<double>();
    m_depthR = WSP_NEW wsp::Image<double>();
}

template<typename T> wsp::StereoImage<T>::StereoImage()
    :INIT_StereoImage
{
    init();
}
template<typename T> wsp::StereoImage<T>::StereoImage(wsp::Image<T> &leftImg, wsp::Image<T> &rightImg)
    :INIT_StereoImage
{
    init();
    *m_imgL=leftImg;
    *m_imgR=rightImg;
}
template<typename T> wsp::StereoImage<T>::~StereoImage()
{
    delete m_imgL;
    delete m_imgR;
    delete m_depthL;
    delete m_depthR;
}


template<typename T> wsp::State wsp::StereoImage<T>::ComputeDepth(int search_min, int search_max, u32 block_min, u32 block_max)
{
    int smin, smax, bmin, bmax;
    wsp::State stat;

    smin = search_min;      smax = search_max;
    bmin = (int)block_min;  bmax = (int)block_max;

    //! Get depth for each image by stereo matching
    wsp::img::StereoBMParams params;
    params.dispx_min=smin;  params.dispx_max=smax;
    params.block_min=bmin;  params.block_max=bmax;
    stat=wsp::img::StereoFastBM_core(*m_depthL, *m_imgL, *m_imgR, params);
    if(stat!=WSP_STATE_SUCCESS){ return stat; }

    stat=wsp::img::StereoFastBM_core(*m_depthR, *m_imgR, *m_imgL, params);
    if(stat!=WSP_STATE_SUCCESS){ return stat; }

    //! Cross check to find occlusions
    stat=wsp::img::CrossCheck(*m_depthL, *m_depthR);
    if(stat!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Cross Check failed"); return stat; }
    stat=wsp::img::CrossCheck(*m_depthR, *m_depthL);
    if(stat!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Cross Check failed"); return stat; }

    //! Fill up found occlusions
    wsp::img::FillUpTimes<double>(*m_depthL, NULL, WSP_PIXEL_STATE_OCCLUDED, 1);
    wsp::img::FillUpTimes<double>(*m_depthR, NULL, WSP_PIXEL_STATE_OCCLUDED, 1);

    return WSP_STATE_SUCCESS;
}



// Getter -------------------------------------------
template<typename T>
inline const wsp::Image<T>& wsp::StereoImage<T>::GetImgL() const{
    wsp::Image<T> *im;
    *im = *m_imgL;
    return *im;
}
template<typename T>
inline const wsp::Image<T>& wsp::StereoImage<T>::GetImgR() const{
    wsp::Image<T> *im;
    *im = *m_imgR;
    return *im;
}
template<typename T>
inline const wsp::Image<double>& wsp::StereoImage<T>::GetDepthL() const{
    wsp::Image<double> *im;
    *im = *m_depthL;
    return *im;
}
template<typename T>
inline const wsp::Image<double>& wsp::StereoImage<T>::GetDepthR() const{
    wsp::Image<double> *im;
    *im = *m_depthR;
    return *im;
}

// Setter -------------------------------------------
template<typename T>
inline void wsp::StereoImage<T>::SetImgL(const wsp::Image<T> &in_img){
    *m_imgL=in_img;
}
template<typename T>
inline void wsp::StereoImage<T>::SetImgR(const wsp::Image<T> &in_img){
    *m_imgR=in_img;
}


// I/O --------------------------------------------------
template<typename T>
inline wsp::State wsp::StereoImage<T>::LoadImgL(const char *fileName){
    return m_imgL->LoadImageFile(fileName);
}
template<typename T>
inline wsp::State wsp::StereoImage<T>::LoadImgR(const char *fileName){
    return m_imgR->LoadImageFile(fileName);
}
template<typename T>
inline wsp::State wsp::StereoImage<T>::SaveDepthL(const char *fileName){
    return m_depthL->SaveImage(fileName);
}
template<typename T>
inline wsp::State wsp::StereoImage<T>::SaveDepthR(const char *fileName){
    return m_depthR->SaveImage(fileName);
}

#endif