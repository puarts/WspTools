#pragma once

#include <wsp/common/_define_commoncore.h>
#include <wsp/common/common_types.h>
#include <wsp/devil/wrap_fn-il.h>

namespace wsp{ namespace devil{

    //! base function for loading image with DevIL
    template<typename T> 
    wsp::State ilLoad8BitImage(
        T **o_color, 
        uchar **o_alpha, 
        int &o_width, int &o_height, int &o_nChannel, 
        const char *in_filename, 
        bool separate_color_and_alpha=true, 
        int current_length=0, int current_num_channels=0, int alloc_step=1);

    //! interit functions
    template<typename T> 
    inline wsp::State ilLoadRGBAImage(
        T **o_img, 
        int &o_width, int &o_height, int &o_nChannel, 
        const char *in_filename,
        int current_length=0, int current_num_channels=0, int alloc_step=1);

    template<typename T> 
    inline wsp::State ilLoadRGBImageAndAlpha(
        T **o_img, uchar **o_alpha, int &o_width, int &o_height, int &o_nChannel, 
        const char *in_filename, int current_length=0, int current_num_channels=0, int alloc_step=1);
    
    //! base function for saving image with DevIL
    template<typename T> 
    wsp::State ilSaveAs8BitImage(const T *in_img, const uchar *in_alpha, int width, int height, int num_channels, const char *in_filename);
}}

template<typename T> 
wsp::State wsp::devil::ilLoad8BitImage(
    T **o_color, uchar **o_alpha, int &o_width, int &o_height, int &o_num_channels, 
    const char *in_filename, bool separate_color_and_alpha,
    int current_length, int current_num_channels, int alloc_step)
{
    u8 *tmp_data;
    int width, height, num_channels;
    WSP_Result result = WSP_IlLoadImage(&tmp_data, &width, &height, &num_channels, in_filename);
    if (result.level != WSP_RESULT_LEVEL_SUCCESS)
    {
        return WSP_STATE_FAILURE;
    }

    int len = width * height;

    // Copy data ---------------------------------------------------------
    //! memory allocation
    if(o_color!=NULL)
    {
        wsp::ReallocArrayMemByStep(o_color, len*num_channels, current_length*current_num_channels, 0, alloc_step);
    }

    if(o_alpha!=NULL)
    {
        wsp::ReallocArrayMemByStep(o_alpha, len, current_length, 0xff, alloc_step);
    }

    o_width = width;
    o_height = height;
    o_num_channels = num_channels;

    // Copy
    {
        T *dst_ptr, *end_ptr;
        uchar *src_ptr;
        uchar *alpha_ptr, *end_alpha_ptr;

        if(o_alpha!=NULL && num_channels == 4){
            alpha_ptr = *o_alpha;
            end_alpha_ptr = *o_alpha + len;
            src_ptr = tmp_data + 3;
            for (; alpha_ptr != end_alpha_ptr; ++alpha_ptr, src_ptr += 4)
            {
                *alpha_ptr = *src_ptr;
            }
        }

        dst_ptr = *o_color;
        end_ptr = *o_color+len*o_num_channels;
        src_ptr = tmp_data;
        for(; dst_ptr!=end_ptr; ++dst_ptr, ++src_ptr)
        {
            *dst_ptr = static_cast<T>(*src_ptr);
        }
    }

    WSP_IlFreeImage(&tmp_data);
    return WSP_STATE_SUCCESS;
}
template<typename T> 
inline wsp::State wsp::devil::ilLoadRGBAImage(T **o_img, int &o_width, int &o_height, int &o_num_channels, const char *in_filename,
                                            int current_length, int current_num_channels, int alloc_step)
{
    return wsp::devil::ilLoad8BitImage(o_img, NULL, o_width, o_height, o_num_channels, in_filename, false,
                                        current_length, current_num_channels, alloc_step);
}
template<typename T>
inline wsp::State wsp::devil::ilLoadRGBImageAndAlpha(T **o_img, uchar **o_alpha, int &o_width, int &o_height, int &o_num_channels,
                                            const char *in_filename, 
                                            int current_length, int current_num_channels, int alloc_step)
{
    return wsp::devil::ilLoad8BitImage(o_img, o_alpha, o_width, o_height, o_num_channels, in_filename, true, 
                                        current_length, current_num_channels, alloc_step);
}
template<typename T> 
wsp::State wsp::devil::ilSaveAs8BitImage(const T *in_img, const uchar *in_alpha, int width, int height, int num_channels, const char *in_filename)
{
    // copy data in rgba data ---------------------------------------------
    int dst_nc = 3;
    if(num_channels==1){ dst_nc=1; }
    if(in_alpha!=NULL){ dst_nc=4; } //! include alpha in number of channels
    uchar *tmp_rgba = WSP_NEW uchar[width*height*dst_nc];

    printf("dst num of channels=%d, src num of channels=%d\n", dst_nc, num_channels);
    //#ifdef _OPENMP
    //#pragma omp parallel
    //#endif
    {
        int len = width*height;
        int dst_w_size = width*dst_nc;
        const T *src_ptr = in_img;
        uchar *dst_ptr = tmp_rgba + len*dst_nc - dst_w_size;
        uchar *end_dst_y_ptr = tmp_rgba - dst_w_size;
        uchar *end_dst_x_ptr;
        //#ifdef _OPENMP
        //#pragma omp for
        //#endif
        int w2 = width * 2 * dst_nc;
        int cnt;
        int nc_diff = dst_nc - num_channels;
        uchar *end_dst_nc_ptr;
        // copy data with inverting vertically -------
        for(; dst_ptr!=end_dst_y_ptr; dst_ptr-=w2){ //! invert image vertically
            end_dst_x_ptr = dst_ptr + dst_w_size;
            cnt=0;
            for(; dst_ptr!=end_dst_x_ptr; ++src_ptr, ++dst_ptr){
                *dst_ptr = static_cast<uchar>(*src_ptr);
                ++cnt;
                if(cnt==num_channels){
                    end_dst_nc_ptr=dst_ptr+nc_diff;
                    for(;dst_ptr!=end_dst_nc_ptr;++dst_ptr){
                        *dst_ptr = static_cast<uchar>(*src_ptr);
                    }
                    cnt=0;
                    continue;
                }
            }
        }

        // copy data ----------------------------
        //const T *end_ptr = in_img + len*num_channels;
        //int cnt = 0;
        //int nc_diff = dst_nc - num_channels;
        //for(; src_ptr!=end_ptr; dst_ptr++, src_ptr++){
        //    *dst_ptr = static_cast<uchar>(*src_ptr);
        //    cnt++;
        //    if(cnt==num_channels){
        //        //*dst_ptr = *alpha_ptr; //! alpha
        //        //alpha_ptr++;
        //        dst_ptr+=nc_diff;
        //        cnt=0;
        //    }
        //}

        //! copy alpha --------------------------
        if(in_alpha!=NULL){
            const uchar *alpha_ptr = in_alpha;
            dst_ptr = tmp_rgba + dst_nc - 1 + len*dst_nc - dst_w_size;
            end_dst_y_ptr = tmp_rgba + dst_nc - 1 - dst_w_size;
            //for(;alpha_ptr!=end_alpha_ptr; ++alpha_ptr, dst_ptr+=dst_nc){
            //    *dst_ptr = *alpha_ptr;
            //}
            for(; dst_ptr!=end_dst_y_ptr; dst_ptr-=w2){ //! invert image vertically
                end_dst_x_ptr = dst_ptr + dst_w_size;
                for(; dst_ptr!=end_dst_x_ptr; ++alpha_ptr, dst_ptr+=dst_nc){
                    *dst_ptr = *alpha_ptr;
                }
            }
        }
    }

    WSP_IlSaveImage(tmp_rgba, width, height, dst_nc, in_filename);

    delete[] tmp_rgba;
    return WSP_STATE_SUCCESS;
}
