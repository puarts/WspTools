/**
 * @file _tpl_fn-imgfilter.hpp
 * 
 */

#ifndef _WSP_TEMPLATE_BASE_FN_IMG_FILTER_H_
#define _WSP_TEMPLATE_BASE_FN_IMG_FILTER_H_

#define _USE_MATH_DEFINES
#include <math.h>
#include <assert.h>


#ifndef __WSP_IMAGEUTIL_DEFINE_IMAGEUTIL_H__
#include "_define_image.h"
#endif

typedef struct{
    float gauss_dist_sigma;
    float ori_angle_rad;
    float cos_frequency;
    float cos_offset_rad;
} WSP_GaborFilterParam;

inline void WSP_GaborFilterParam_SetToDefault( WSP_GaborFilterParam *param )
{
    (*param).cos_frequency    = 1.0f;
    (*param).cos_offset_rad   = 0.0f;
    (*param).gauss_dist_sigma = 1.7f;
    (*param).ori_angle_rad    = 0.0f;
}

namespace wsp{ namespace img{

    class GaborFilterParam
        : public WSP_GaborFilterParam
    {
    public:
        GaborFilterParam(){
            WSP_GaborFilterParam_SetToDefault(this);
        }
    };

    /**
     * @param x The horizontal coordinate within kernel(from 0 to kernel_size_x-1)
     * @param y The vertical coordinate wihin kernel(from 0 to kernel_size_y-1)
     */
    inline double GetGaborWeight(
        int x, int y,
        int kernel_size_x,
        int kernel_size_y,
        const wsp::img::GaborFilterParam &param
    );

    template<typename T>
    void StoreGaborKernel(
        T *o_kernel_data,
        int kernel_size_x,
        int kernel_size_y,
        const wsp::img::GaborFilterParam &param
        );

    template<typename _InType, typename _OutType>
    wsp::State GaborFilter(
        const _InType *in_src, 
        _OutType *o_dest,
        int width, 
        int height, 
        int kernel_x,
        int kernel_y,
        const wsp::img::GaborFilterParam &param
    );

    template<typename _InType, typename _OutType>
    inline wsp::State GaborFilter(
        const _InType *in_src, 
        _OutType *o_dest,
        int width, 
        int height, 
        int kernel_size,
        const wsp::img::GaborFilterParam &param
    )
    {
        return wsp::img::GaborFilter(
                in_src, 
                o_dest, 
                width, height, 
                kernel_size, kernel_size,
                param
            );
    }

    //template<typename T>
    //wsp::State GaussianBlurSimple(
    //    const T *in_src, 
    //    T *o_dest,
    //    int width, 
    //    int height, 
    //    int num_channels
    //);

    //! core functions ---------------------------------
    template<typename T> wsp::State Median(
        T *io_img, 
        wsp::img::ImageParam &param, 
        int block_size, 
        const uchar *in_mask=NULL
    );

    //! overwrapped functions --------------------------
    template<typename T> 
    inline wsp::State MedianFilterIter(
        T *io_img,
        wsp::img::ImageParam &img_param,
        wsp::img::ImgFilterParam &filter_param,
        const uchar *in_mask = NULL);

    template<typename _OutType, typename _InType>
    void ApplyLaplacianFilter(
        _OutType *o_dest,
        const _InType *in_src,
        u32 width,
        u32 height, 
        u32 num_of_channels);
}}

#include "_tpl_fn-imgfilter.inl"

#endif