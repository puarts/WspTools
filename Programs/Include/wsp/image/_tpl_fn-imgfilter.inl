/**
* @file _tpl_fn-imgfilter.hpp
* 
*/

#ifndef _WSP_TEMPLATE_BASE_FN_IMAGE_FILETER_INL_
#define _WSP_TEMPLATE_BASE_FN_IMAGE_FILETER_INL_

#include "_tpl_fn-imgfilter.hpp"

#include <omp.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <assert.h>

namespace{

    template<typename _InType, typename _OutType, typename _KernelType>
    void _ApplyImageFilter(
        const _InType *in_src,
        _OutType *o_dest,
        s32 width,
        s32 height,
        s32 num_of_channels,
        const _KernelType *in_kernel,
        s32 kernel_size_x,
        s32 kernel_size_y
        )
    {
        int kernel_hsize_x = kernel_size_x / 2;
        int kernel_hsize_y = kernel_size_y / 2;

        /* Unoptimized code */
#ifdef _OPENMP
#pragma omp parallel
        {
            int x, y;
            int u, v;
            int ki; /* iteration variables within kernel */
            int c, i;
            int x_end = width - kernel_hsize_x;
            int y_end = height - kernel_hsize_y;

            _KernelType *convolutions = (_KernelType*)malloc(sizeof(_KernelType)* num_of_channels);

#pragma omp for //private(y, x, i, c, u, v, ki, convolutions)
            for (y = kernel_hsize_y; y < y_end; ++y)
            {
                for (x = kernel_hsize_x; x < x_end; ++x)
                {
                    i = (y * width + x)*num_of_channels;
                    memset(convolutions, 0, sizeof(_KernelType)*num_of_channels);
                    const _KernelType *ker_ptr = in_kernel;
                    for (u = -kernel_hsize_y; u <= kernel_hsize_y; ++u)
                    {
                        for (v = -kernel_hsize_x; v <= kernel_hsize_x; ++v, ++ker_ptr)
                        {
                            ki = ((y + u)*width + x + v)*num_of_channels;
                            for (c = 0; c<num_of_channels; ++c)
                            {
                                convolutions[c] += (*ker_ptr) * in_src[ki + c];
                            }
                        }
                    }
                    for (c = 0; c<num_of_channels; ++c)
                    {
                        o_dest[i + c] = convolutions[c];
                    }
                }
            }
            free(convolutions);
        }
#else
        /* Optimized code without parallelization */
        {
            int x, y;
            int u, v;
            int kx, ky, ki; /* iteration variables within kernel */


            _KernelType *convolutions = (_KernelType*)malloc(sizeof(_KernelType)* num_of_channels);


            const _InType *src_ptr = in_src;
            _OutType *dst_ptr = o_dest;
            _KernelType *conv_ptr;
            _KernelType *conv_end_ptr = convolutions + num_of_channels;

            /*
            * To speed up this code, separation of edge loop and inner loop is required
            */
            for (y = 0; y < height; ++y)
            {
                for (x = 0; x < width; ++x)
                {
                    /* Initialize convolution array */
                    memset(convolutions, 0, sizeof(_KernelType)*num_of_channels);

                    /* Get convolution of local block */
                    const _KernelType *ker_ptr = in_kernel;
                    for (u = -kernel_hsize_y; u <= kernel_hsize_y; ++u)
                    {
                        ky = y + u;
                        //if( ky<0 || ky>=height){
                        //    continue;
                        //}
                        //if( ky < 0 )
                        //{
                        //    u -= ky;
                        //    continue;
                        //}
                        //if(ky >= height ){ break; }

                        for (v = -kernel_hsize_x; v <= kernel_hsize_x; ++v, ++ker_ptr)
                        {
                            kx = x + v;
                            if (ky<0 || ky >= height || kx<0 || kx >= width){
                                continue;
                            }
                            //if( kx<0 || kx>=width){
                            //    continue;
                            //}
                            //if( kx < 0 ){
                            //    v -= kx;
                            //    continue;
                            //}
                            //if( kx >= width ){ break; }

                            ki = (ky*width + kx)*num_of_channels;
                            src_ptr = in_src + ki;
                            conv_ptr = convolutions;
                            for (; conv_ptr != conv_end_ptr; ++src_ptr, ++conv_ptr)
                            {
                                *conv_ptr += (*ker_ptr) * (*src_ptr);
                            }
                        }
                    }

                    /* Store convolution to destination */
                    conv_ptr = convolutions;
                    for (; conv_ptr != conv_end_ptr; ++dst_ptr, ++conv_ptr)
                    {
                        *dst_ptr = *conv_ptr;
                    }
                }
            }
            free(convolutions);
        }
#endif 
    }

    template<typename T>
    inline wsp::State _ApplyFilterIter(
        wsp::State(*filter_func)(
        T *o_img,
        wsp::img::ImageParam &param,
        int filter_size,
        const uchar *in_mask
        ),
        T *io_img,
        wsp::img::ImageParam &img_param,
        wsp::img::ImgFilterParam &filter_param,
        const uchar *in_mask
        )
    {
        wsp::State state;
        for (int i = 0; i<filter_param.num_of_iteration; i++){
            state = filter_func(io_img, img_param, filter_param.filter_size, in_mask);
            if (state != WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Applying image filter failed\n"); return state; }
        }
        return WSP_STATE_SUCCESS;
    }
}

template<typename T>
void wsp::img::StoreGaborKernel(
    T *o_kernel_data,
    int kernel_size_x,
    int kernel_size_y,
    const wsp::img::GaborFilterParam &param
    )
{
    int kernel_hsize_x = (kernel_size_x - 1) / 2;
    int kernel_hsize_y = (kernel_size_y - 1) / 2;
    double delta_x = 2.0 / (kernel_size_x - 1);
    double delta_y = 2.0 / (kernel_size_y - 1);
    double x_theta;
    double y_theta;

    WSP_COMMON_DEBUG_LOG(
        "Making gabor filter kernel..\n"
        "orienation   = %f deg\n"
        "gauss sigma  = %f\n"
        "cos freqency = %f\n"
        "cos offset   = %f deg\n"
        , param.ori_angle_rad * 180.0 / M_PI
        , param.gauss_dist_sigma
        , param.cos_frequency
        , param.cos_offset_rad * 180.0 / M_PI
        );

    int index;
    for (int y = -kernel_hsize_y; y <= kernel_hsize_y; ++y)
    {
        for (int x = -kernel_hsize_x; x <= kernel_hsize_x; ++x)
        {
            index = (kernel_hsize_y + y) * kernel_size_x + kernel_hsize_x + x;

            /* rotate filter weight */
            x_theta = (x * cos(param.ori_angle_rad) + y * sin(param.ori_angle_rad)) * delta_x;
            y_theta = (-x * sin(param.ori_angle_rad) + y * cos(param.ori_angle_rad)) * delta_y;

            /* gaussian distribution weight */
            double gauss_numerator = (x_theta * x_theta + y_theta * y_theta);
            double gauss_denominator = (2.0 * param.gauss_dist_sigma * param.gauss_dist_sigma);
            double gauss_normalize_factor = 1.0 / (sqrt(2.0 * M_PI * param.gauss_dist_sigma * param.gauss_dist_sigma));
            double gauss_weight = gauss_normalize_factor * exp(-gauss_numerator / gauss_denominator);
            //double gauss_weight = exp(-0.5*(pow(x_theta,2) + pow(y_theta,2))/pow(sigma,2));

            /* cosine wave weight */
            double cos_weight = cos(2 * M_PI * x_theta * param.cos_frequency + param.cos_offset_rad);

            double gabor_weight = gauss_weight * cos_weight;

            o_kernel_data[index] = (T)gabor_weight;

            WSP_COMMON_DEBUG_LOG("%3f\t", gabor_weight);
        }
        WSP_COMMON_DEBUG_LOG("\n");
    }
}

inline double wsp::img::GetGaborWeight(
    int x, int y,
    int kernel_size_x,
    int kernel_size_y,
    const wsp::img::GaborFilterParam &param
)
{
    assert(param.gauss_dist_sigma > 0.0);

    int kernel_hsize_x = kernel_size_x / 2;
    int kernel_hsize_y = kernel_size_y / 2;

    int dx = x - kernel_hsize_x;
    int dy = y - kernel_hsize_y;

    double delta_x = 2.0/(kernel_size_x-1);
    double delta_y = 2.0/(kernel_size_y-1);

    int index = (kernel_hsize_y + dy) * kernel_size_x + kernel_hsize_x + dx;

    /* rotate filter weight */
    double x_theta = ( dx * cos(param.ori_angle_rad) + dy * sin(param.ori_angle_rad) ) * delta_x;
    double y_theta = (-dx * sin(param.ori_angle_rad) + dy * cos(param.ori_angle_rad) ) * delta_y;

    /* gaussian distribution weight */
    double gauss_numerator        = (x_theta * x_theta + y_theta * y_theta);
    double gauss_denominator      = (2.0 * param.gauss_dist_sigma * param.gauss_dist_sigma);
    double gauss_normalize_factor = 1.0 / (sqrt(2.0 * M_PI * param.gauss_dist_sigma * param.gauss_dist_sigma));
    double gauss_weight = gauss_normalize_factor * exp( - gauss_numerator / gauss_denominator );

    /* cosine wave weight */
    double cos_weight = cos( 2 * M_PI * x_theta * param.cos_frequency + param.cos_offset_rad );

    return gauss_weight * cos_weight;
}





template<typename _InType, typename _OutType>
wsp::State wsp::img::GaborFilter(
    const _InType *in_src, 
    _OutType *o_dest,
    int width, 
    int height, 
    int kernel_size_x,
    int kernel_size_y,
    const wsp::img::GaborFilterParam &param
)
{
    u32 num_of_channels = 1;

    double *kernel = (double*)malloc(sizeof(double) * kernel_size_x * kernel_size_y);
    wsp::img::StoreGaborKernel( kernel, kernel_size_x, kernel_size_y, param );
    /* Print kernel */
    //for( i=0; i<kernel_size_y; ++i )
    //{
    //    for( j=0; j<kernel_size_x; ++j )
    //    {
    //        printf("%3f\t", kernel[ i * kernel_size_x + j ]);
    //    }
    //    printf("\n");
    //}

    memset( o_dest, 0, sizeof(_OutType)*width*height );
    _ApplyImageFilter(
        in_src,
        o_dest,
        (u32) width,
        (u32) height,
        (u32) num_of_channels,
        kernel,
        (u32) kernel_size_x,
        (u32) kernel_size_y
    );

    free(kernel);
    return WSP_STATE_SUCCESS;
}


//template<typename T>
//wsp::State wsp::img::GaussianBlurSimple(
//    const T *in_src, 
//    T *o_dst,
//    int width, 
//    int height, 
//    int num_channels,
//    int width_step
//)    
//{
//    /* Not implemented yet */
//    {
//        int len = height * width_step;
//        const T *src_ptr = in_src;
//        T *dst_ptr = o_dst;
//        T *end_ptr = dst_ptr + len;
//        for(;;)
//        {
//        }
//    }
//}




template<typename T> 
wsp::State wsp::img::Median(T *io_img, wsp::img::ImageParam &param, int block_size, const uchar *in_mask)
{
    int width = param.width;
    int height = param.height;
    int num_channels = param.num_channels;
    int size = width*height*num_channels;


    // copy source image ---------
    T *src = WSP_NEW T[size];
    T *src_ptr = io_img;
    T *dst_ptr = src;
    T *end_ptr = src+size;
    for(; dst_ptr!=end_ptr; ++dst_ptr, ++src_ptr){
        *dst_ptr = *src_ptr;
    }
    // --------------------------

    uchar thre_alpha = 128;

    if(width*height==0 || block_size == 0){ return WSP_STATE_FAILURE; }

    printf("MEDIAN: filter_size=%d\n\n", block_size);

    {
        int itr_x, itr_y, itr_i, itr_j, c; //! iterators
        int index, cnt;
        int xx, yy, ii, iinc; //! coord within block
        T pix;
        T **bufary = WSP_NEW T*[num_channels];
        for(c=0; c<num_channels; c++){
            bufary[c] = WSP_NEW T[(block_size*2+1)*(block_size*2+1)];
        }

        for(itr_y = 0; itr_y < height; itr_y++){
            for(itr_x = 0; itr_x < width; itr_x++){
                index = itr_y * width + itr_x;
                if(in_mask!=NULL){
                    if(in_mask[index]<thre_alpha){ 
                        //printf("skipped: %d\n", in_mask[index]); 
                        continue; 
                    }
                }
                cnt=0;
                //! iteration within block
                for(itr_i = -block_size; itr_i <= block_size; itr_i++){
                    for(itr_j = -block_size; itr_j <= block_size; itr_j++){
                        yy = itr_y + itr_i;
                        xx = itr_x + itr_j;
                        ii = yy * width + xx;
                        iinc = ii * num_channels;
                        if( xx<0 || xx>=width || yy<0 || yy>=height ){
                            continue;
                        }
                        if(in_mask!=NULL){
                            if(in_mask[ii]<thre_alpha){ continue; }
                        }
                        for(c=0; c<num_channels; c++){
                            bufary[c][cnt] = src[iinc+c];
                        }
                        //bufary[cnt] = src.GetElem((u32)xx, (u32)yy);
                        cnt++;
                    }
                }
                if(cnt==0){ continue; }
                //wsp::QuickSort<T>(bufary, (u32)cnt);
                for(c=0; c<num_channels; c++){
                    qsort(bufary[c], cnt, sizeof(T), wsp::CompA<T>);
                    pix = bufary[c][cnt/2];
                    io_img[(itr_y*width+itr_x)*num_channels+c] = pix;
                }
            }
        }
        //! freeing memory
        for(c=0; c<num_channels; c++){
            delete[] bufary[c];
        }
        delete[] bufary;
    }
    delete[] src;
    return WSP_STATE_SUCCESS;
}

template<typename _OutType, typename _InType>
void wsp::img::ApplyLaplacianFilter(
    _OutType *o_dest,
    const _InType *in_src,
    u32 width,
    u32 height,
    u32 num_of_channels)
{
    const u32 kernel_size_x = 3;
    const u32 kernel_size_y = 3;
    const u32 kernel_size = kernel_size_x * kernel_size_y;
    double kernel[kernel_size] =
    {
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1
    };

    memset(o_dest, 0, sizeof(_OutType)*width*height*num_of_channels);
    _ApplyImageFilter(
        in_src,
        o_dest,
        (u32)width,
        (u32)height,
        (u32)num_of_channels,
        kernel,
        (u32)kernel_size_x,
        (u32)kernel_size_y);

    // force to set corner pixels to zero
    _OutType *last_line = o_dest + (height - 1)*width*num_of_channels;
    memset(o_dest, 0, sizeof(_OutType)*width*num_of_channels);
    memset(last_line, 0, sizeof(_OutType)*width*num_of_channels);
    {
        for (int y = 1; y < (int)height - 1; ++y)
        {
            memset(&o_dest[(y * width) * num_of_channels], 0, sizeof(_OutType)*num_of_channels);
            memset(&o_dest[(y * width + width - 1) * num_of_channels], 0, sizeof(_OutType)*num_of_channels);
        }
    }
}



template<typename T>
inline wsp::State wsp::img::MedianFilterIter(
    T *io_img,
    wsp::img::ImageParam &img_param,
    wsp::img::ImgFilterParam &filter_param,
    const uchar *in_mask)
{
    return _ApplyFilterIter<T>(
        wsp::img::Median<T>,
        io_img,
        img_param,
        filter_param,
        in_mask);
}

#endif
