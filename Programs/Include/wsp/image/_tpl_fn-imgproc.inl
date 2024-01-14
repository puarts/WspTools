/**
 * @file _tpl_fn-imgproc.inl
 * 
 * @description Base template functions for image processing
 */

#ifndef _WSP_TEMPLATE_BASE_FN_IMG_PROC_INL_
#define _WSP_TEMPLATE_BASE_FN_IMG_PROC_INL_

#include <math.h>
#ifndef __WSP_COMMONCORE_RESULT_H__
#include <wsp/common/result.h>
#endif

#include "_tpl_fn-imgproc.hpp"
#include "fn-image_log.h"
#include "tpl_cl-image.h"
#include <wsp/common/fn-debug.h>
#include <wsp/math/fn-basic_math.h>


template<typename _Type>
void wsp::img::SwapQuadrantHorizontally(
    _Type *io_img, u32 width, u32 height, u32 num_of_channels)
{
    s32 x, y, i;
    s32 w = (s32)width;
    s32 h = (s32)height;
    s32 nc = (s32)num_of_channels;
    s32 length = w*h*nc;
    s32 hw = w / 2;
    s32 hh = h / 2;
    s32 pixel_size = sizeof(_Type)* nc;

    _Type *tmp_img = (_Type*)malloc(sizeof(_Type)* length);

    memcpy(tmp_img, io_img, sizeof(_Type)*length);

    // swap
    _Type* data_ptr = io_img;
    for (y = 0; y < h; ++y)
    {
        for (x = 0; x < w; ++x, data_ptr += nc)
        {
            if (y < hh && x < hw)
            {
                // second quadrant
                i = (y*w + x + hw) * nc;
            }
            else if (y < hh && hw <= x)
            {
                // first quadrant
                i = (y*w + x - hw) * nc;
            }
            else if (hh <= y && x < hw)
            {
                // third quadrant
                i = (y*w + x + hw) * nc;
            }
            else
            {
                // fourth quadrant
                i = (y*w + x - hw) * nc;
            }

            memcpy(data_ptr, (tmp_img + i), pixel_size);
        }
    }

    free(tmp_img);
}

template<typename _Type>
void wsp::img::SwapQuadrantVertically(
    _Type *io_img, u32 width, u32 height, u32 num_of_channels)
{
    s32 x, y, i;
    s32 w = (s32)width;
    s32 h = (s32)height;
    s32 nc = (s32)num_of_channels;
    s32 length = w*h*nc;
    s32 hw = w / 2;
    s32 hh = h / 2;
    s32 pixel_size = sizeof(_Type) * nc;

    _Type *tmp_img = (_Type*)malloc(sizeof(_Type) * length);

    memcpy(tmp_img, io_img, sizeof(_Type) * length);

    // swap
    _Type* data_ptr = io_img;
    for (y = 0; y < h; ++y)
    {
        for (x = 0; x < w; ++x, data_ptr += nc)
        {
            if (y<hh && x<hw)
            {
                // second quadrant
                i = ((y + hh)*w + x) * nc;
            }
            else if (y<hh && x >= hw)
            {
                // first quadrant
                i = ((y + hh)*w + x) * nc;
            }
            else if (y >= hh && x<hw)
            {
                // third quadrant
                i = ((y - hh)*w + x) * nc;
            }
            else
            {
                // fourth quadrant
                i = ((y - hh)*w + x) * nc;
            }

            memcpy(data_ptr, (tmp_img + i), pixel_size);
        }
    }

    free(tmp_img);
}



template<typename _Type>
void wsp::img::GenerateGrayGradientTexture(
    _Type *image_ptr, s32 width, s32 height, s32 num_channels, _Type max_value )
{
    s32 index;
    for (int y=0; y<width; ++y)
    {
        for (int x=0; x<height; ++x)
        {
            index = (y * width + x)*num_channels;
            for( s32 c=0; c<num_channels; ++c )
            {
                image_ptr[index + c] = (x+y)/(f32)(width+height) * max_value;
            }
        }
    }
}


template<typename _TopImgType, typename _BottomImgType>
void wsp::img::PlaceImageOnImage(
    _BottomImgType *io_bottom_img,
    s32 btm_img_w, s32 btm_img_h,
    const _TopImgType *in_top_img,
    s32 top_img_w, s32 top_img_h, s32 num_channels,
    s32 center_x, s32 center_y,
    s32 place_x, s32 place_y,
    bool use_last_channel_as_alpha
)
{
    WSP_ASSERT_NOT_NULL( io_bottom_img );
    WSP_ASSERT_NOT_NULL( in_top_img );
    WSP_ASSERT_NOT_NEGATIVE_INT( btm_img_w );
    WSP_ASSERT_NOT_NEGATIVE_INT( btm_img_h );
    WSP_ASSERT_NOT_NEGATIVE_INT( top_img_w );
    WSP_ASSERT_NOT_NEGATIVE_INT( top_img_h );
    WSP_ASSERT_NOT_NEGATIVE_INT( num_channels );

    if( use_last_channel_as_alpha )
    {
        s32 last_channel = num_channels - 1;
        for( s32 y=0; y<top_img_h; ++y )
        {
            s32 y_dst = y + place_y - center_y;
            if( y_dst >= btm_img_h )
            {
                break;
            }
            if( y_dst < 0 )
            {
                y = y - y_dst - 1;
                continue;
            }
            for( s32 x=0; x<top_img_w; ++x )
            {
                s32 x_dst = x + place_x - center_x;
                if( x_dst >= btm_img_w )
                {
                    break;
                }
                if( x_dst < 0 )
                {
                    x = x - x_dst - 1;
                    continue;
                }
                /* skip if alpha is zero */
                s32 i_src = ( y*top_img_w + x ) * num_channels;
                if( in_top_img[i_src+last_channel] == 0 )
                {
                    continue;
                }
                s32 i_dst = ( y_dst*btm_img_w + x_dst ) * num_channels;
                f32 top_ratio = in_top_img[i_src+last_channel]/(f32)255.0f;// assume the max of alpha is 255
                for( s32 c=0; c<last_channel; ++c )
                {
                    io_bottom_img[i_dst+c] = io_bottom_img[i_dst+c]*(1.0f-top_ratio) + in_top_img[i_src+c]*top_ratio;
                }
                /* put alpha */
                io_bottom_img[i_dst+last_channel] = 0xff;
            }
        }
    }
    else
    {
        for( s32 y=0; y<top_img_h; ++y )
        {
            s32 y_dst = y + place_y - center_y;
            if( y_dst >= btm_img_h )
            {
                break;
            }
            if( y_dst < 0 )
            {
                y = y - y_dst - 1;
                continue;
            }
            for( s32 x=0; x<top_img_w; ++x )
            {
                s32 x_dst = x + place_x - center_x;
                if( x_dst >= btm_img_w )
                {
                    break;
                }
                if( x_dst < 0 )
                {
                    x = x - x_dst - 1;
                    continue;
                }
                s32 i_dst = ( y_dst*btm_img_w + x_dst ) * num_channels;
                s32 i_src = ( y*top_img_w + x ) * num_channels;
                for( s32 c=0; c<num_channels; ++c )
                {
                    io_bottom_img[i_dst+c] = in_top_img[i_src+c];
                }
            }
        }
    }
}

//#define USE_OPT_CODE_ExtractUserDomain
template<typename _UserDomainMapType, typename _SrcColorType, typename _OutType>
void wsp::img::ExtractUserDomain(
    _OutType *o_image,
    s32 *o_left_x, s32 *o_top_y, 
    s32 *o_width,  s32 *o_height,
    const _UserDomainMapType *in_user_domain_map,
    const _SrcColorType      *in_src_color_map,
    s32 width, s32 height,
    s32 num_channels
)
{
    WSP_ASSERT_NOT_NULL( o_image  );
    WSP_ASSERT_NOT_NULL( o_left_x );
    WSP_ASSERT_NOT_NULL( o_top_y  );
    WSP_ASSERT_NOT_NULL( o_width  );
    WSP_ASSERT_NOT_NULL( o_height );

    s32 top_y    = -1;
    s32 bottom_y = -1;
    s32 left_x   = -1;
    s32 right_x  = -1;

    /* find top coordinate of y */
    {
        const _UserDomainMapType *user_ptr = in_user_domain_map;
        for ( s32 y=0; y<height; ++y)
        {
            for ( s32 x=0; x < width; ++x, ++user_ptr)
            {
                bool is_user_area = (*user_ptr) != 0;
                if( is_user_area )
                {
                    top_y = y;
                    break;
                }
            }
            if( top_y != -1 )
            {
                break;
            }
        }
    }
    /* find bottom coordinate of y */
    {
        const _UserDomainMapType *user_ptr = in_user_domain_map;
        for ( s32 y=height-1; y>=0; --y)
        {
            for ( s32 x=width-1; x>=0; --x, --user_ptr)
            {
                bool is_user_area = (*user_ptr) != 0;
                if( is_user_area )
                {
                    bottom_y = y;
                    break;
                }
            }
            if( bottom_y != -1 )
            {
                break;
            }
        }
    }
    /* find left coordinate of x */
    {
        const _UserDomainMapType *user_ptr = in_user_domain_map;
        for ( s32 x=0; x<width; ++x)
        {
            for ( s32 y=0; y< height; ++y )        
            {
                bool is_user_area = user_ptr[y*width + x] != 0;
                if( is_user_area )
                {
                    left_x = x;
                    break;
                }
            }
            if( left_x != -1 )
            {
                break;
            }
        }
    }
    /* find right coordinate of x */
    {
        const _UserDomainMapType *user_ptr = in_user_domain_map;
        for ( s32 x=width-1; x>=0; --x)
        {
            for ( s32 y=0; y<height; ++y )
            {
                bool is_user_area = user_ptr[y*width + x] != 0;
                if( is_user_area )
                {
                    right_x = x;
                    break;
                }
            }
            if( right_x != -1 )
            {
                break;
            }
        }
    }
    if( top_y    == -1
     || bottom_y == -1
     || left_x   == -1
     || right_x  == -1
    )
    {
        WSP_IMAGE_DEBUG_LOG("%d %d %d %d", top_y, bottom_y, left_x, right_x);
        *o_left_x = 0;
        *o_top_y  = 0;
        *o_width  = 0;
        *o_height = 0;
    }
    else
    {
        *o_left_x = left_x;
        *o_top_y  = top_y;
        *o_width  = (right_x-left_x);
        *o_height = (bottom_y-top_y);
        wsp::img::CropImage(
                in_src_color_map,
                o_image,
                width, height, num_channels, 
                left_x, top_y, (*o_width), (*o_height) 
            );
    }
}



#define USE_OPT_CODE_FlipImageHorizontally
template<typename InType, typename OutType> 
void wsp::img::FlipImageHorizontally(
    const InType *in_img, 
    OutType *o_img, 
    s32 width, s32 height, s32 num_channels
)
{
#ifndef USE_OPT_CODE_FlipImageHorizontally
    for( s32 y=0; y<height; ++y )
    {
        for( s32 x=0; x<width; ++x )
        {
            u32 i = (y*width + x)*num_channels;
            u32 i_inv_x = (y*width + width - x - 1)*num_channels;
            for( s32 c=0; c<num_channels; ++c )
            {
                o_img[i+c] = in_img[i_inv_x+num_channels-c-1];
            }
        }
    }
#else
    /* optimized code */
    s32 w_step = width * num_channels;
    const InType  *in_ptr = in_img;
    OutType *o_ptr  = o_img;
    OutType *o_end_ptr = o_ptr + w_step * height;
    OutType *o_c_end_ptr;
    for( ; o_ptr!=o_end_ptr; )
    {
        OutType *o_x_end_ptr = o_ptr + w_step;
        in_ptr += w_step;
        for( ; o_ptr!=o_x_end_ptr;  )
        {
            o_c_end_ptr = o_ptr + num_channels;
            in_ptr -= num_channels;
            for( ; o_ptr!=o_c_end_ptr; ++o_ptr, ++in_ptr )
            {
                *o_ptr = *in_ptr;
            }
            in_ptr -= num_channels;
        }
        in_ptr += w_step;
    }
#endif
}

template<typename InType, typename OutType>
void wsp::img::FlipImageVertically(
    const InType *in_img,
    OutType *o_img,
    s32 width, s32 height, s32 num_channels
    )
{
    for (s32 y = 0; y<height; ++y)
    {
        for (s32 x = 0; x<width; ++x)
        {
            u32 i = (y*width + x)*num_channels;
            u32 i_inv_y = ((height - y - 1) * width + x) * num_channels;
            for (s32 c = 0; c<num_channels; ++c)
            {
                o_img[i + c] = in_img[i_inv_y + c];
            }
        }
    }
}


template<typename T>
void wsp::img::ChromaKeyRgb(
    const T *in_img,
    u8 *o_mask,
    s32 width, s32 height,
    s32 r_min, s32 r_max,
    s32 g_min, s32 g_max,
    s32 b_min, s32 b_max
)
{
    #ifdef _OPENMP
    #pragma omp parallel
    #endif
    {
        s32 x, y, i, ic;
        #ifdef _OPENMP
        #pragma omp for
        #endif
        for( y=0; y<height; ++y )
        {
            for( x=0; x<width; ++x )
            {
                i  = ( y * width + x );
                ic = i * 3;
                if(
                    ( r_min<in_img[ic+0] && in_img[ic+0]<r_max ) &&
                    ( g_min<in_img[ic+1] && in_img[ic+1]<g_max ) &&
                    ( b_min<in_img[ic+2] && in_img[ic+2]<b_max ) 
                )
                {
                    o_mask[i] = 0;
                    continue;
                }
                o_mask[i] = 255;
            }
        }
    }
}



template<typename InType, typename OutType>
void wsp::img::CropImage(
    const InType *in_img, 
    OutType *o_img, 
    s32 width, s32 height, s32 num_channels, 
    s32 crop_x, s32 crop_y, 
    s32 crop_width, s32 crop_height
){
    WSP_ASSERT_NOT_NULL( in_img );
    WSP_ASSERT_NOT_NULL( o_img!=NULL );
    WSP_ASSERT_POSITIVE_INT( width );
    WSP_ASSERT_POSITIVE_INT( height );
    WSP_ASSERT_POSITIVE_INT( num_channels );
    WSP_ASSERT_POSITIVE_INT( crop_x );
    WSP_ASSERT_POSITIVE_INT( crop_y );
    WSP_ASSERT_POSITIVE_INT( crop_width );
    WSP_ASSERT_POSITIVE_INT( crop_height );

    s32 x, y, c, bx, by, i, bi;
    for(y=crop_y, by=0; by<crop_height; ++y, ++by)
    {
        for(x=crop_x, bx=0; bx<crop_width; ++x, ++bx)
        {
            i = (y*width + x) * num_channels;
            bi = (by*crop_width + bx) * num_channels;
            for(c=0; c<num_channels; ++c)
            {
                //o_img[bi+c] = static_cast<OutType>(in_img[i+c]);
                o_img[bi+c] = in_img[i+c];
            }
        }
    }
}

template<typename InType, typename OutType> 
void wsp::img::ShiftImageHor(
    const InType *in_img, 
    OutType *o_img, 
    s32 width, 
    s32 height, 
    s32 num_channels, 
    s32 shift_pixels
)
{
    WSP_ASSERT_POSITIVE_INT( width );
    WSP_ASSERT_POSITIVE_INT( height );
    WSP_ASSERT_POSITIVE_INT( num_channels );

    s32 npix = width*height;
    s32 w_step = width*num_channels;
    // horizontal shift
    if(abs(shift_pixels)<width)
    {
        // unoptimized code ----------------------------
        s32 x, y, ys, i, c;
        s32 shift_elem = shift_pixels*num_channels;
        if(0<shift_pixels){
            for(y=0; y<height; ++y){
                ys = y*w_step;
                // fill up empty area
                for(x=0; x<shift_pixels; ++x){
                    i = ys + x*num_channels;
                    for(c=0; c<num_channels; ++c){
                        o_img[i+c] = 0;
                    }
                }
                for(;x<width; ++x){
                    i = ys + x*num_channels;
                    for(c=0; c<num_channels; ++c){
                        o_img[i+c] = in_img[i-shift_elem+c];
                    }
                }
            }
        }else{
            s32 end_x = width + shift_pixels;
            for(y=0; y<height; ++y){
                ys = y*w_step;
                for(x=0; x<end_x; ++x){
                    i = ys + x*num_channels;
                    for(c=0; c<num_channels; ++c){
                        o_img[i+c] = in_img[i-shift_elem+c];
                    }
                }
                for(; x<width; ++x){
                    i = ys + x*num_channels;
                    for(c=0; c<num_channels; ++c){
                        o_img[i+c] = 0;
                    }
                }
            }
        }
    }else{
        memset(o_img, 0, npix*num_channels*sizeof(OutType));
    }
}
template<typename InType, typename OutType> 
void wsp::img::ShiftImageVer(
    const InType *in_img, 
    OutType *o_img, 
    s32 width, 
    s32 height, 
    s32 num_channels, 
    s32 shift_pixels
)
{
    WSP_ASSERT_POSITIVE_INT( width );
    WSP_ASSERT_POSITIVE_INT( height );
    WSP_ASSERT_POSITIVE_INT( num_channels );

    s32 npix = width*height;
    s32 w_step = width*num_channels;
    // horizontal shift
    if(abs(shift_pixels) < height)
    {
        // vertical shift
        // unoptimized code ----------------------------
        s32 x, y, i, c;
        s32 shift_elem = shift_pixels*w_step;
        if(0<shift_pixels){
            for(x=0; x<width; ++x){
                // fill up empty area
                for(y=0; y<shift_pixels; ++y){
                    i = (y*width + x)*num_channels;
                    for(c=0; c<num_channels; ++c){
                        o_img[i+c] = 0;
                    }
                }
                for(; y<height; ++y){
                    i = (y*width + x)*num_channels;
                    for(c=0; c<num_channels; ++c){
                        o_img[i+c] = in_img[i-shift_elem+c];
                    }
                }
            }
        }else{
            s32 end_y = height+shift_pixels;
            for(x=0; x<width; ++x){
                for(y=0; y<end_y; ++y){
                    i = (y*width + x)*num_channels;
                    for(c=0; c<num_channels; ++c){
                        o_img[i+c] = in_img[i-shift_elem+c];
                    }
                }
                // fill up empty area
                for(; y<height; ++y){
                    i = (y*width + x)*num_channels;
                    for(c=0; c<num_channels; ++c){
                        o_img[i+c] = 0;
                    }
                }
            }
        }
    }else{
        memset(o_img, 0, npix*num_channels*sizeof(OutType));
    }
}

//////////////////////////////////////////////////////////////////////////////



template<typename T>
void wsp::img::GetDirectionalSumOfCorr(
    const T *in_main, 
    const T *in_sub, 
    double *o_corr_sum,
    s32 width, s32 height, s32 num_channels,
    s32 disp_x, s32 disp_y,
    double (*eval_func)(double, double), 
    double default_corr
){
    WSP_ASSERT_POSITIVE_INT( width );
    WSP_ASSERT_POSITIVE_INT( height );
    WSP_ASSERT_POSITIVE_INT( num_channels );

    s32 dx_step = disp_x*num_channels;
    s32 width_step = width*num_channels;
    s32 length = width * height;
    s32 num_elem = length * num_channels;

    // Get horizontal correlation sum ---------
    {
        s32 x, y, c;
        s32 dx, dy;
        // optimized code -------------------------------------------------
        const T *end_c_ptr, *end_wd_ptr;
        const T *ptr_main = in_main;
        const T *end_ptr = ptr_main + num_elem;
        const T *ptr_sub = in_sub + dx_step;
        double *dst_ptr = o_corr_sum;
        for(y=0; ptr_main!=end_ptr; ++y)
        {
            double sum = 0.0;
            const T *end_w_ptr = ptr_main + width_step;
            for(x=0; ptr_main!=end_w_ptr; ++dst_ptr, ++x){
                dx = x + disp_x;
                dy = y + disp_y;
                if(dx<0 || dx>=width || dy<0 || dy>=height){
                    sum += default_corr;
                    ptr_main+=num_channels;
                    ptr_sub+=num_channels;
                }else{
                    end_c_ptr = ptr_main + num_channels;
                    for(; ptr_main!=end_c_ptr; ++ptr_main, ++ptr_sub){
                        sum += eval_func(static_cast<double>(*ptr_main), static_cast<double>(*ptr_sub));
                    }
                }
                *dst_ptr =  sum;
            }
        }
        // optimize code 2(not completed yet) -------------------------------
        //if(disp_x<0){
        //    for(; ptr_main!=end_ptr; ){
        //        sum = 0.0;
        //        end_wd_ptr = ptr_main - dx_step;
        //        end_w_ptr = ptr_main + width_step;
        //        for(; ptr_main<end_wd_ptr; ++ptr_main, ++ptr_sub, ++dst_ptr){
        //            sum += wsp::math::kDefaultCorr;
        //            *dst_ptr = sum;
        //        }
        //        for(; ptr_main!=end_w_ptr; ++ptr_main, ++ptr_sub, ++dst_ptr){
        //            end_c_ptr = ptr_main + num_channels;
        //            for(; ptr_main!=end_c_ptr; ++ptr_main, ++ptr_sub){
        //                sum += eval_func(static_cast<double>(*ptr_main), static_cast<double>(*ptr_sub));
        //            }
        //            *dst_ptr = sum/static_cast<double>(num_channels);
        //        }
        //    }
        //}else if(disp_x>0){
        //    for(; ptr_main!=end_ptr;){
        //        sum = 0.0;
        //        end_w_ptr = ptr_main + width_step;
        //        end_wd_ptr = end_w_ptr - dx_step;
        //        for(; ptr_main!=end_wd_ptr; ++ptr_main, ++ptr_sub, ++dst_ptr){
        //            end_c_ptr = ptr_main + num_channels;
        //            for(; ptr_main!=end_c_ptr; ++ptr_main, ++ptr_sub){
        //                sum += eval_func(static_cast<double>(*ptr_main), static_cast<double>(*ptr_sub));
        //            }
        //            *dst_ptr = sum/static_cast<double>(num_channels);
        //        }
        //        for(; ptr_main!=end_w_ptr; ++ptr_main, ++ptr_sub, ++dst_ptr){
        //            sum += wsp::math::kDefaultCorr;
        //            *dst_ptr = sum;
        //        }
        //    }
        //}
        // unoptimized code ------------------------------------------------
        //s32 index, index_d;
        //for(y=0; y<height; y++){
        //    sum = 0;
        //    for(x=0; x<width; x++){
        //        dx = (s32)x + disp_x;
        //        dy = (s32)y + disp_y;
        //        if(dx<0 || dx>=(s32)width || dy<0 || dy>=(s32)height){
        //            sum += wsp::math::kDefaultCorr;
        //        }else{
        //            index = (y*width + x) * num_channels;
        //            index_d = (dy*width + dx) * num_channels;
        //            for(c=0; c<num_channels; ++c){
        //                //subst = (s32)(in_main[index + c] - in_sub[index_d + c]);
        //                //sum += (double)subst*subst;
        //                sum += eval_func(static_cast<double>(in_main[index+c]), static_cast<double>(in_sub[index_d+c]));
        //            
        //            }
        //            //subst = (s32)(in_main.GetElem(x, y).r-in_sub.GetElem((u32)dx, (u32)dy).r);
        //            //sum += (double)subst*subst;
        //            //subst = (in_main.GetElem(x, y).g-in_sub.GetElem((u32)dx, (u32)dy).g);
        //            //sum += (double)subst*subst;
        //            //subst = (in_main.GetElem(x, y).b-in_sub.GetElem((u32)dx, (u32)dy).b);
        //            //sum += (double)subst*subst;
        //        }
        //        corr_sum_map[y*width+x] = sum;
        //    }
        //}
    }

    // Get full sum ---------------
    {
        double sum;
        s32 x, y;
        // optimized code --------------------
        double *corr_v_ptr, *end_v_ptr;
        double *corr_ptr = o_corr_sum;
        double *end_ptr = corr_ptr + width;
        for(; corr_ptr!=end_ptr; ++corr_ptr){
            sum=0.0;
            corr_v_ptr = corr_ptr;
            end_v_ptr = corr_ptr + length;
            for(; corr_v_ptr!=end_v_ptr; corr_v_ptr+=width){
                sum += *corr_v_ptr;
                *corr_v_ptr = sum;
            }
        }
        // unoptimized code ------------------
        //for(x=0; x<width; ++x){
        //    sum=0.0;
        //    for(y=0; y<height; ++y){
        //        s32 index = y*width+x;
        //        sum += tmp_corr_sum[index];
        //        tmp_corr_sum[index] = sum;
        //    }
        //}
    }
}




template<typename InType, typename OutType> void wsp::img::ScaleImageNearestNeighbor(
    const InType *in_src, s32 src_width, s32 src_height, s32 num_channels,
          OutType *o_dst,  s32 dst_width, s32 dst_height
){
    f32 scale_w = (src_width)/static_cast<f32>(dst_width);
    f32 scale_h = (src_height)/static_cast<f32>(dst_height);
    s32 dst_size = dst_width * dst_height * num_channels;
    s32 src_size = src_width * src_height * num_channels;
    //printf("scaling factor of width %f\n", scale_w);
    //printf("scaling factor of height %f\n", scale_h);

    {
        s32 x, y, c, i_dst, i_src;
        s32 src_y, src_x;
        // unoptimized code -----------------------
        // This is faster than optimized code
        for(y=0; y<dst_height; ++y){
            src_y = static_cast<s32>(y*scale_h);
            for(x=0; x<dst_width; ++x){
                src_x = static_cast<s32>(x*scale_w);
                i_dst = (y * dst_width + x) * num_channels;
                i_src = (src_y * src_width + src_x) * num_channels;
                for(c=0; c<num_channels; ++c){
                    o_dst[i_dst+c] = static_cast<OutType>(in_src[i_src+c]);
                }
            }
        }
        // optimized code -------------------------
        //T *dst_ptr = o_dst;
        //T *end_ptr = dst_ptr + dst_size;
        //const T *src_ptr;
        //T *end_w_ptr;
        //for(x=0, y=0; dst_ptr!=end_ptr; ++x){
        //    i_src = (yw_src + static_cast<s32>(x*scale_w)) * num_channels;
        //    src_ptr = in_src + i_src;
        //    end_w_ptr = dst_ptr + num_channels;
        //    for(; dst_ptr!=end_w_ptr; ++dst_ptr, ++src_ptr){
        //        *dst_ptr = *src_ptr;
        //    }
        //    if(x==dst_width){
        //        x=0;
        //        ++y;
        //        yw_src = static_cast<s32>(y*scale_h)*src_width;
        //    }
        //}
    }
}


template<typename _Type>
void wsp::img::AffineTransform( 
    const _Type *in_src, 
    s32 src_width, s32 src_height, s32 src_num_channels, 
    _Type *o_dest,
    s32 width, s32 height, s32 num_channels, 
    const struct wsp::img::AffineParam &param
)
{
    WSP_ASSERT_POSITIVE_INT( width );
    WSP_ASSERT_POSITIVE_INT( height );
    WSP_ASSERT_POSITIVE_INT( num_channels );
    WSP_ASSERT_POSITIVE_INT( src_width );
    WSP_ASSERT_POSITIVE_INT( src_height );
    WSP_ASSERT_POSITIVE_INT( src_num_channels );
    WSP_ASSERT_EQUAL( num_channels, src_num_channels );
    WSP_ASSERT_POSITIVE_FLOAT( param.scale_x );
    WSP_ASSERT_POSITIVE_FLOAT( param.scale_y );

    s32 src_width_step = src_width * src_num_channels;
    s32 width_step = width * num_channels;
    memset( o_dest, 0, sizeof(_Type) * width_step * height );

    s32 x, y;
    f32 centered_x, centered_y;
    s32 src_x, src_y;

    f32 rotate_rad = param.rotate_angle * M_PI / 180.0f;
    f32 sx_mult = 1.0f / param.scale_x;
    f32 sy_mult = 1.0f / param.scale_y;
    f32 tx = param.translate_x;
    f32 ty = param.translate_y;
    f32 ax = param.anchor_x;
    f32 ay = param.anchor_y;
    f32 ax_tx = ax + tx;
    f32 ay_ty = ay + ty;

    f32 cos_sx_precalc = cos(rotate_rad) * sx_mult;
    f32 sin_sx_precalc = sin(rotate_rad) * sx_mult;
    f32 cos_sy_precalc = cos(rotate_rad) * sy_mult;
    f32 sin_sy_precalc = sin(rotate_rad) * sy_mult;

    _Type *dst_ptr = o_dest;
    const _Type *src_ptr, *end_ptr;
    for( y=0; y<height; ++y )
    {
        for( x=0; x<width; ++x )
        {
            /**
             * - Rotate 
             *     f32 affine_x = (x - ax) * cos(rotate_rad) - (y - ay) * sin(rotate_rad) + ax;
             *     f32 affine_y = (x - ax) * sin(rotate_rad) + (y - ay) * cos(rotate_rad) + ay;
             * 
             * - Scale 
             *     affine_x = (affine_x - ax) / sx + ax;
             *     affine_y = (affine_y - ay) / sy + ay;
             * 
             * - Translate 
             *     affine_x += tx;
             *     affine_y += ty;
             */
            centered_x = x - ax_tx;
            centered_y = y - ay_ty;

            src_x = (s32)( centered_x * cos_sx_precalc - centered_y * sin_sx_precalc + ax );
            src_y = (s32)( centered_x * sin_sy_precalc + centered_y * cos_sy_precalc + ay );
            if( src_y<0 || src_y>=src_height || src_x<0 || src_x>=src_width )
            {
                dst_ptr += num_channels;
                continue;
            }
            src_ptr = in_src + src_y * src_width_step + src_x * src_num_channels;
            end_ptr = dst_ptr + num_channels;
            for( ; dst_ptr!=end_ptr; ++src_ptr, ++dst_ptr )
            {
                *dst_ptr = *src_ptr;
            }
        }
    }
}

template<typename InType, typename OutType> 
void wsp::img::ScaleImageBilinear1(
    const InType *in_src, 
    s32 src_width, s32 src_height, s32 num_channels,
    OutType *o_dst,  
    s32 dst_width, s32 dst_height
)
{
    f32 offset_corner_w = (dst_width/static_cast<f32>(src_width))/2.0;
    f32 offset_corner_h = (dst_height/static_cast<f32>(src_height))/2.0;
    printf("offset corner: %f,  %f\n", offset_corner_w, offset_corner_h);
    f32 scale_w = (src_width-1)/static_cast<f32>(dst_width-1-offset_corner_w*2);
    f32 scale_h = (src_height-1)/static_cast<f32>(dst_height-1-offset_corner_h*2);
    s32 dst_size = dst_width * dst_height * num_channels;
    s32 src_size = src_width * src_height * num_channels;
    //printf("scaling factor of width %f\n", scale_w);
    //printf("scaling factor of height %f\n", scale_h);

    {
        s32 x, y, c, i_dst;
        s32 i_src0, i_src1, i_src2, i_src3;
        s32 yw_dst, x_src, y_src;
        s32 yw_src;
        f32 scale_pos;
        // coefficients for compute linear
        f32 y_coef0, y_coef1, x_coef0, x_coef1; 
        memset(o_dst, 0, dst_size*sizeof(OutType));
        // unoptimized code -----------------------
        for(y=0; y<dst_height-offset_corner_h*2; ++y){
            //i_dst = y*dst_width;
            //i_src0 = (yw_src + x_src) * num_channels;
            //for(c=0; c<num_channels; ++c){
            //    o_dst[i_dst+c] = in_src[i_src0+c];
            //}
            scale_pos = (y)*scale_h+0.5;
            printf("%d: scale_pos = %f\n", y, scale_pos);
            y_src = static_cast<s32>(scale_pos);
            yw_src = y_src*src_width;
            y_coef1 = scale_pos - static_cast<s32>(scale_pos);
            y_coef0 = 1.0f - y_coef1;
            yw_dst = y*dst_width;
            for(x=0; x<dst_width-offset_corner_w*2; ++x){
                scale_pos = (x)*scale_w+0.5;
                x_src = static_cast<s32>(scale_pos);
                x_coef1 = scale_pos - x_src;
                x_coef0 = 1.0f - x_coef1;

                i_dst = (yw_dst + dst_width*ceil(offset_corner_h) + x + ceil(offset_corner_w)) * num_channels;
                i_src0 = (yw_src + x_src) * num_channels;
                i_src1 = (yw_src + ((x_src+1))) * num_channels;
                i_src2 = (yw_src+src_width + ((x_src))) * num_channels;
                i_src3 = (yw_src+src_width + ((x_src+1))) * num_channels;

                for(c=0; c<num_channels; ++c){
                    // horizontal and vertical s32erpolation
                    o_dst[i_dst+c] = static_cast<OutType>(
                                        (in_src[i_src0+c]*x_coef0+in_src[i_src1+c]*x_coef1)*y_coef0 
                                        + (in_src[i_src2+c]*x_coef0+in_src[i_src3+c]*x_coef1)*y_coef1
                                    );
                }
                //if(x==dst_width-1){
                //    printf("dst(%d, %d)=src(%d, %d)\n", x, y, x_src, y_src);
                //}
            }
        }
        s32 i_src_0, i_src_1, i_dst_0, i_dst_1;
        // fill horizontal corner

        //for(x=1; x<dst_width-1; ++x){
        //    i_dst_0 = x;
        //    i_src_0 = i_dst_0 + dst_width;
        //    i_dst_1 = (dst_height-1)*dst_width + x;
        //    i_src_1 = i_dst_1 - dst_width;
        //    for(c=0; c<num_channels; ++c){
        //        o_dst[i_dst_0+c] = o_dst[i_src_0+c];
        //        o_dst[i_dst_1+c] = o_dst[i_src_1+c];
        //    }
        //}
        //// fill vertical corner
        //for(y=0; y<dst_height; ++y){
        //    i_dst_0 = y*dst_width;
        //    i_src_0 = i_dst_0 + 1;
        //    i_dst_1 = (y+1)*dst_width - 1;
        //    i_src_1 = i_dst_1 - 1;
        //    for(c=0; c<num_channels; ++c){
        //        o_dst[i_dst_0+c] = o_dst[i_src_0+c];
        //        o_dst[i_dst_1+c] = o_dst[i_src_1+c];
        //    }
        //}

    }
}

template<typename _InType, typename _OutType>
void wsp::img::RotateImageClockwise(
    _OutType *o_img,
    const _InType *in_img,
    s32 width, s32 height, s32 num_channels
)
{
    wsp::img::AffineParam affine_param;
    affine_param.anchor_x = width/2.0f;
    affine_param.anchor_y = height/2.0f;
    affine_param.rotate_angle = 90.0f;
    affine_param.scale_x = 1.0f;
    affine_param.scale_y = 1.0f;
    affine_param.translate_x = 0.0f;
    affine_param.translate_y = 0.0f;

    wsp::img::AffineTransform(
            in_img, width, height, num_channels, 
            o_img, width, height, num_channels,
            affine_param 
        );
}

template<typename InType, typename OutType> 
void wsp::img::ScaleImageBilinear(
    const InType *in_src, 
    s32 src_width, s32 src_height, s32 num_channels,
    OutType *o_dst, 
    s32 dst_width, s32 dst_height
)
{
    f32 x_src_step = 1.0f/(f32)(src_width);
    f32 y_src_step = 1.0f/(f32)(src_height);
    f32 x_dst_step = 1.0f/(f32)(dst_width);
    f32 y_dst_step = 1.0f/(f32)(dst_height);
    f32 zero_xpos_src = x_src_step/2.0f;
    f32 zero_xpos_dst = x_dst_step/2.0f;
    f32 zero_ypos_src = y_src_step/2.0f;
    f32 zero_ypos_dst = y_dst_step/2.0f;

    {
        s32 c;
        s32 i_s0, i_s1, i_s2, i_s3, i_d;
        s32 yd, xd, ys, xs;
        f32 x_coef0, x_coef1, y_coef0, y_coef1;
        f32 x_src_f, y_src_f, x_dst_f, y_dst_f;

        y_src_f=zero_ypos_src;
        y_dst_f=zero_ypos_dst;
        for(yd=0, ys=0; yd<dst_height; ++yd, y_dst_f+=y_dst_step)
        {
            if(y_src_f+y_src_step <= y_dst_f){
                y_src_f+=y_src_step;
                ++ys;
            }
            y_coef1 = (y_dst_f-y_src_f)/y_src_step;
            y_coef0 = 1.0f - y_coef1;
            x_dst_f=zero_xpos_dst;
            x_src_f=zero_xpos_src;
            for(xd=0, xs=0; xd<dst_width; ++xd, x_dst_f+=x_dst_step){
                if(x_src_f+x_src_step <= x_dst_f){
                    x_src_f+=x_src_step;
                    ++xs;
                }
                if(  zero_xpos_src<=x_dst_f && x_dst_f<1.0f-zero_xpos_src
                  && zero_ypos_src<=y_dst_f && y_dst_f<1.0f-zero_ypos_src)
                {
                    x_coef1 = (x_dst_f-x_src_f)/x_src_step;
                    x_coef0 = 1.0f - x_coef1;
                    i_d = (yd*dst_width+xd)*num_channels;
                    i_s0 = ((ys)*src_width+(xs))*num_channels;
                    i_s1 = i_s0 + num_channels;
                    i_s2 = i_s0 + src_width*num_channels;
                    i_s3 = i_s1 + src_width*num_channels;
                    
                    for(c=0; c<num_channels; ++c){
                        // compute linear s32erpolation
                        o_dst[i_d+c] = static_cast<OutType>(
                            (in_src[i_s0+c]*x_coef0 + in_src[i_s1+c]*x_coef1)*y_coef0
                            + (in_src[i_s2+c]*x_coef0 + in_src[i_s3+c]*x_coef1)*y_coef1
                        );
                    }
                }
            }
        }
    }
    // copy corner
    {
        s32 x, y, c, i_dst0, i_src0, i_dst1, i_src1;
        s32 x_copy_top, x_copy_tail, y_copy_top, y_copy_tail;
        f32 y_dst_f, x_dst_f;

        // compute position of source of copy 
        for(y_copy_top=0, y_dst_f=zero_ypos_dst; y_dst_f<zero_ypos_src; ++y_copy_top, y_dst_f+=y_dst_step){}
        for(x_copy_top=0, x_dst_f=zero_xpos_dst; x_dst_f<zero_xpos_src; ++x_copy_top, x_dst_f+=x_dst_step){}
        x_copy_tail = dst_width - 1 - x_copy_top;
        y_copy_tail = dst_height - 1 - y_copy_top;
        printf("ytop:%d, xtop:%d, ytail:%d, xtail:%d\n", y_copy_top, x_copy_top, y_copy_tail, x_copy_tail);

        // copy horizontal lines in corner
        y_dst_f = zero_ypos_dst;
        for(y=0; y_dst_f<zero_ypos_src; ++y, y_dst_f+=y_dst_step){
            for(x=0; x<dst_width; ++x){ 
                i_dst0 = (y*dst_width+x)*num_channels;
                i_src0 = (y_copy_top*dst_width+x)*num_channels;
                i_dst1 = ((dst_height-y-1)*dst_width+x)*num_channels;
                i_src1 = (y_copy_tail*dst_width+x)*num_channels;
                for(c=0; c<num_channels; ++c){
                    // copy top x corner
                    o_dst[i_dst0+c] = o_dst[i_src0+c]; 
                    // copy bottom x corner
                    o_dst[i_dst1+c] = o_dst[i_src1+c];
                }
            }
        }
        // copy vertical lines in corner
        x_dst_f = zero_xpos_dst;
        for(x=0; x_dst_f<zero_xpos_src; ++x, x_dst_f+=x_dst_step){
            for(y=0; y<dst_height; ++y){ 
                i_dst0 = (y*dst_width+x)*num_channels;
                i_src0 = (y*dst_width+x_copy_top)*num_channels;
                i_dst1 = ((y+1)*dst_width-x-1)*num_channels;
                i_src1 = (y*dst_width+x_copy_tail)*num_channels;
                for(c=0; c<num_channels; ++c){
                    // copy left y corner
                    o_dst[i_dst0+c] = o_dst[i_src0+c]; 
                    // copy right y corner
                    o_dst[i_dst1+c] = o_dst[i_src1+c];
                }
            }
        }
    }
}

//template<typename T> void wsp::img::ScaleImageBilinear(
//    const T *in_src, s32 src_width, s32 src_height, s32 num_channels,
//            T *o_dst,  s32 dst_width, s32 dst_height
//){
//    f32 offset_corner_w = (dst_width/static_cast<f32>(src_width))/2.0;
//    f32 offset_corner_h = (dst_height/static_cast<f32>(src_height))/2.0;
//    printf("offset corner: %f,  %f\n", offset_corner_w, offset_corner_h);
//    f32 scale_w = (src_width-1)/static_cast<f32>(dst_width-1-offset_corner_w*2);
//    f32 scale_h = (src_height-1)/static_cast<f32>(dst_height-1-offset_corner_h*2);
//    s32 dst_size = dst_width * dst_height * num_channels;
//    s32 src_size = src_width * src_height * num_channels;
//    //printf("scaling factor of width %f\n", scale_w);
//    //printf("scaling factor of height %f\n", scale_h);
//
//    {
//        s32 x, y, c, i_dst;
//        s32 i_src0, i_src1, i_src2, i_src3;
//        s32 yw_dst, x_src, y_src;
//        s32 yw_src;
//        f32 scale_pos;
//        // coefficients for compute linear
//        f32 y_coef0, y_coef1, x_coef0, x_coef1; 
//        memset(o_dst, 0, dst_size*sizeof(T));
//        // unoptimized code -----------------------
//        for(y=0; y<dst_height-offset_corner_h*2; ++y){
//            //i_dst = y*dst_width;
//            //i_src0 = (yw_src + x_src) * num_channels;
//            //for(c=0; c<num_channels; ++c){
//            //    o_dst[i_dst+c] = in_src[i_src0+c];
//            //}
//            scale_pos = (y)*scale_h+0.5;
//            printf("%d: scale_pos = %f\n", y, scale_pos);
//            y_src = static_cast<s32>(scale_pos);
//            yw_src = y_src*src_width;
//            y_coef1 = scale_pos - static_cast<s32>(scale_pos);
//            y_coef0 = 1.0f - y_coef1;
//            yw_dst = y*dst_width;
//            for(x=0; x<dst_width-offset_corner_w*2; ++x){
//                scale_pos = (x)*scale_w+0.5;
//                x_src = static_cast<s32>(scale_pos);
//                x_coef1 = scale_pos - x_src;
//                x_coef0 = 1.0f - x_coef1;
//
//                i_dst = (yw_dst + dst_width*ceil(offset_corner_h) + x + ceil(offset_corner_w)) * num_channels;
//                i_src0 = (yw_src + x_src) * num_channels;
//                i_src1 = (yw_src + ((x_src+1))) * num_channels;
//                i_src2 = (yw_src+src_width + ((x_src))) * num_channels;
//                i_src3 = (yw_src+src_width + ((x_src+1))) * num_channels;
//
//                for(c=0; c<num_channels; ++c){
//                    // horizontal and vertical s32erpolation
//                    o_dst[i_dst+c] = (in_src[i_src0+c]*x_coef0+in_src[i_src1+c]*x_coef1)*y_coef0 
//                                    + (in_src[i_src2+c]*x_coef0+in_src[i_src3+c]*x_coef1)*y_coef1;
//                }
//                //if(x==dst_width-1){
//                //    printf("dst(%d, %d)=src(%d, %d)\n", x, y, x_src, y_src);
//                //}
//            }
//        }
//        s32 i_src_0, i_src_1, i_dst_0, i_dst_1;
//        // fill horizontal corner
//
//        //for(x=1; x<dst_width-1; ++x){
//        //    i_dst_0 = x;
//        //    i_src_0 = i_dst_0 + dst_width;
//        //    i_dst_1 = (dst_height-1)*dst_width + x;
//        //    i_src_1 = i_dst_1 - dst_width;
//        //    for(c=0; c<num_channels; ++c){
//        //        o_dst[i_dst_0+c] = o_dst[i_src_0+c];
//        //        o_dst[i_dst_1+c] = o_dst[i_src_1+c];
//        //    }
//        //}
//        //// fill vertical corner
//        //for(y=0; y<dst_height; ++y){
//        //    i_dst_0 = y*dst_width;
//        //    i_src_0 = i_dst_0 + 1;
//        //    i_dst_1 = (y+1)*dst_width - 1;
//        //    i_src_1 = i_dst_1 - 1;
//        //    for(c=0; c<num_channels; ++c){
//        //        o_dst[i_dst_0+c] = o_dst[i_src_0+c];
//        //        o_dst[i_dst_1+c] = o_dst[i_src_1+c];
//        //    }
//        //}
//
//    }
//}

template<typename T>
wsp::State wsp::img::CutOff(T *io_img, uchar *io_alpha, s32 length, s32 num_channels, T thre_min, T thre_max)
{
    if(thre_min>=thre_max)
    { 
        WSP_COMMON_ERROR_LOG("min threshold must be smaller than max threshold\n"); 
        return WSP_STATE_INVALID_PARAMETER; 
    }
    T *ptr=io_img;
    T *end_ptr=io_img + length*num_channels;
    uchar *alpha_ptr = io_alpha;
    for(; ptr!=end_ptr; ++alpha_ptr, ptr+=num_channels){
        if(*ptr<thre_min){ *alpha_ptr=0; }
        if(*ptr>thre_max){ *alpha_ptr=0; }
    }
    return WSP_STATE_SUCCESS;
}


template<typename InType, typename OutType> 
inline wsp::State wsp::img::CopyOneChannel(
    const InType *in_src, 
    const uchar *in_alpha, 
    OutType *o_dst, 
    s32 length, s32 src_num_channels, s32 dst_num_channels, 
    s32 target_src_channel_index, 
    s32 target_dst_channel_index, 
    bool mask_alpha, 
    bool normalize
){
    WSP_ASSERT_LARGER_THAN( src_num_channels, dst_num_channels );
    WSP_ASSERT_LARGER_THAN( target_src_channel_index+1, src_num_channels );
    WSP_ASSERT_LARGER_THAN( target_dst_channel_index+1, dst_num_channels );

    const InType *src_ptr;
    OutType *dst_ptr, *end_ptr;
    const uchar *alpha_ptr = in_alpha;
    src_ptr = in_src + target_src_channel_index;
    dst_ptr = o_dst + target_dst_channel_index;
    end_ptr = o_dst + target_dst_channel_index + length*dst_num_channels;

    if(normalize){
        // find min and max
        InType min = in_src[0];
        InType max = in_src[0];
        const InType *ptr = in_src+1;
        const InType *end = in_src + length*src_num_channels;
        for(; ptr!=end; ++ptr){
            if(min>*ptr){ min=*ptr; continue; }
            if(max<*ptr){ max=*ptr; }
        }
        f32 range = static_cast<f32>(max - min);
        if(range!=0){
            f32 mult = 255.0/range;
            if(mask_alpha==false){
                for(; dst_ptr!=end_ptr; dst_ptr+=dst_num_channels, src_ptr+=src_num_channels){
                    *dst_ptr = static_cast<OutType>((*src_ptr-min)*mult);
                }
            }else{
                f32 mult = range;
                for(; dst_ptr!=end_ptr; dst_ptr+=dst_num_channels, src_ptr+=src_num_channels, ++alpha_ptr){
                    *dst_ptr = static_cast<OutType>((*src_ptr-min)*(*alpha_ptr)/range);
                }
            }
        }
    }else{
        if(mask_alpha==false){
            for(; dst_ptr!=end_ptr; dst_ptr+=dst_num_channels, src_ptr+=src_num_channels){
                *dst_ptr = static_cast<OutType>(*src_ptr);
            }
        }else{
            for(; dst_ptr!=end_ptr; dst_ptr+=dst_num_channels, src_ptr+=src_num_channels, ++alpha_ptr){
                *dst_ptr = static_cast<OutType>(*src_ptr*(*alpha_ptr)/255);
            }
        }
    }
    return WSP_STATE_SUCCESS;
}

//template<typename InType, typename OutType>
//inline wsp::State wsp::img::CopyImage(const InType *in_src, const uchar *in_alpha, OutType *o_dst, s32 length, s32 src_num_channels, s32 dst_num_channels, 
//                        bool mask_alpha, bool invert_channels)
//{
//    if(src_num_channels>dst_num_channels){
//        WSP_COMMON_ERROR_LOG("number of destination channels must be larger than number of source channels\n");
//        return WSP_STATE_FAILURE;
//    }
//    const InType *src_ptr = in_src;
//    const InType *end_ptr = in_src + length*src_num_channels;
//    const uchar *alpha_ptr = in_alpha;
//    OutType *dst_ptr = o_dst;
//    s32 nc_diff = dst_num_channels - src_num_channels;
//    s32 cnt=0;
//    s32 offset=src_num_channels-1;
//    if(mask_alpha==false){
//        if(invert_channels){
//            for(;src_ptr!=end_ptr; ++src_ptr){
//                *(dst_ptr+offset-cnt) = static_cast<OutType>(*src_ptr); //! invert RGB to BGR
//                ++cnt;
//                if(cnt==src_num_channels){ dst_ptr+=dst_num_channels; cnt=0; }
//            }
//        }else{
//            for(;src_ptr!=end_ptr; ++src_ptr){
//                *dst_ptr = static_cast<OutType>(*src_ptr); //! invert RGB to BGR
//                ++cnt;
//                if(cnt==src_num_channels){ dst_ptr+=dst_num_channels; cnt=0; }
//            }
//        }
//    }else{
//        if(invert_channels){
//            for(;src_ptr!=end_ptr; ++src_ptr){
//                *(dst_ptr+offset-cnt) = static_cast<OutType>(*(src_ptr)*(*alpha_ptr)/255); //! invert RGB to BGR
//                ++cnt;
//                if(cnt==src_num_channels){ dst_ptr+=dst_num_channels; ++alpha_ptr; cnt=0; }
//            }
//        }else{
//            for(;src_ptr!=end_ptr; ++src_ptr){
//                *dst_ptr = static_cast<OutType>(*(src_ptr)*(*alpha_ptr)/255); //! invert RGB to BGR
//                ++cnt;
//                if(cnt==src_num_channels){ dst_ptr+=dst_num_channels; ++alpha_ptr; cnt=0; }
//            }
//        }
//    }
//    return WSP_STATE_SUCCESS;
//}

// =========================================================
// =========================================================
//**
//                    Interpolation
//**
// =========================================================
// =========================================================


template<typename T> 
s32 wsp::img::FillUpGaussianAverage(
        T *io_img, 
        uchar *io_alpha, 
        uchar *io_state, 
        const uchar *in_mask, 
        s32 width, s32 height, s32 nChannels,  
        wsp::img::PixelState target_state, 
        s32 block_size, 
        double variance, 
        bool use_alpha_weight,
        uchar thre_transparent_alpha
){
    WSP_ASSERT_NOT_NULL( io_img );
    WSP_ASSERT_POSITIVE_INT( block_size );

    if(target_state==WSP_PIXEL_STATE_TRANSPARENT && io_alpha==NULL){
        WSP_COMMON_ERROR_LOG("alpha is NULL, nevertheless transparency is declared as target state\n"); return -1;
    }
    s32 occlusion_amount=0;
    wsp::Image<T> src(io_img, io_alpha, io_state, width, height, nChannels);

    if(target_state==WSP_PIXEL_STATE_TRANSPARENT)
    {
        src.SetTransparentStateByAlpha(thre_transparent_alpha);
    }
    
    //src.SaveImage("tmp_inpas32_gauss.tif");
    //src.SaveAlpha("tmp_inpas32_alpha.tif");
    //src.SaveState("tmp_inpas32_normal.tif", WSP_PIXEL_STATE_NORMAL);
    //src.SaveState("tmp_inpas32_transparent.tif", WSP_PIXEL_STATE_TRANSPARENT);
    //return WSP_STATE_SUCCESS;

    #ifdef _OPENMP
    #pragma omp parallel
    #endif
    {
        double distance, weight, sumWeight, sumAlpha;
        double *sum = WSP_NEW double[nChannels];
        s32 xx, yy, ii, cnt;
        s32 x, y, i, j, c;
        s32 index;

        #ifdef _OPENMP
        #pragma omp for private(y, x, i, j, c) reduction(+:occlusion_amount)
        #endif
        for(y=0; y<height; ++y)
        {
            for(x=0; x<width; ++x)
            {
                index = y*width + x;
                if(in_mask!=NULL)
                {
                    if(in_mask[index]!=255)
                    {
                        io_alpha[index] = 0; continue;
                    }
                }
                if(src.GetState(index)!=target_state){ continue; }

                ++occlusion_amount;

                //! init sums
                sumWeight=0.0;
                sumAlpha=0.0;
            
                for(c=0; c<nChannels; c++){ sum[c]=0.0; }
                cnt=0;
                // get weighted sum of arround pixels
                for(i=-block_size; i<=block_size; i++)
                {
                    for(j=-block_size; j<=block_size; j++)
                    {
                        yy = y + i;
                        xx = x + j;
                        if(xx<0 || xx>=width || yy<0 || yy>=height){
                            continue;
                        }
                        ii = yy*width + xx;
                        if(in_mask!=NULL){
                            if(in_mask[ii]!=255){ continue; }
                        }
                        distance = sqrt((double)(i*i+j*j));
                        if(distance>(double)block_size
                            //|| src.GetState(xx, yy)!=WSP_PIXEL_STATE_NORMAL
                            || ((src.GetState(xx, yy)!=WSP_PIXEL_STATE_TRANSPARENT) && (src.GetState(xx, yy)!=WSP_PIXEL_STATE_NORMAL))
                            || (src.GetAlpha(xx, yy)==0)
                            || (use_alpha_weight==false && src.GetAlpha(xx, yy)<thre_transparent_alpha))
                        { continue; }
                        //weight = 1.0;//wsp::math::GetGaussianWeight(distance, variance);
                        //weight = wsp::math::GetGaussianWeight(distance, variance) * (double)src.GetAlpha(xx, yy);
                        weight = wsp::math::GetGaussianWeight(distance, variance);
                        if(use_alpha_weight){ weight*=(double)src.GetAlpha(xx, yy); }

                        for(c=0; c<nChannels; c++){
                            sum[c] += ((double)src[ii*nChannels+c]) * weight;
                        }
                        sumWeight+= weight;
                        sumAlpha += weight;
                        ++cnt;
                    }
                }
                if(cnt==0){ continue; }
                //printf("(%d %d): cnt=%d\n", x, y, cnt);
                for(c=0; c<nChannels; c++){
                    io_img[index*nChannels+c]=(T)(sum[c]/sumWeight);
                }
                //io_img[i*nChannels+0]=255;
                //io_img[i*nChannels+1]=0;
                //io_img[i*nChannels+2]=0;

                //io_img.SetAlpha(255, x, y);
                if(io_alpha!=NULL){
                    if(use_alpha_weight==false){
                        io_alpha[index] = 255;
                    }else{
                        if((double)io_alpha[index]+sumAlpha>=255){ 
                            io_alpha[index]=255; 
                        }else{ 
                            io_alpha[index]+=(uchar)sumAlpha; 
                        }
                    }
                }
                //io_img.AddAlpha(sumAlpha, x, y);
                //src.SetState(WSP_PIXEL_STATE_NORMAL, i);
                if(io_state!=NULL){
                    io_state[index] = WSP_PIXEL_STATE_NORMAL;
                }
                //io_img.SetState(WSP_PIXEL_STATE_NORMAL, x, y);
            }
        }
        delete[] sum;
    }
    return occlusion_amount;
}

template<typename T> s32 wsp::img::FillUpGaussianAverageHorizontalDirection(
        T *io_img, 
        uchar *io_alpha, 
        uchar *io_state, 
        const uchar *in_mask, 
        s32 width, s32 height, s32 nChannels,
        wsp::img::PixelState target_state,
        s32 block_size, 
        double variance, 
        bool use_alpha_weight
){
    WSP_ASSERT_NOT_NULL( io_img );
    WSP_ASSERT_POSITIVE_INT( block_size );

    if(target_state==WSP_PIXEL_STATE_TRANSPARENT && io_alpha==NULL){
        WSP_COMMON_ERROR_LOG("alpha is NULL, nevertheless transparency is declared as target state\n"); return -1;
    }
    s32 occlusion_amount=0;
    wsp::Image<T> src(io_img, io_alpha, io_state, width, height, nChannels);

    if(target_state==WSP_PIXEL_STATE_TRANSPARENT){
        src.SetTransparentStateByAlpha();
    }
    
    //src.SaveImage("tmp_inpas32_gauss.tif");
    //src.SaveAlpha("tmp_inpas32_alpha.tif");
    //src.SaveState("tmp_inpas32_normal.tif", WSP_PIXEL_STATE_NORMAL);
    //src.SaveState("tmp_inpas32_transparent.tif", WSP_PIXEL_STATE_TRANSPARENT);
    //return WSP_STATE_SUCCESS;

    double thre_occ_num=5;

    #ifdef _OPENMP
    #pragma omp parallel
    #endif
    {
        double distance, weight, sumWeight, sumAlpha;
        double *sum = WSP_NEW double[nChannels];
        s32 xx, yy, ii, jj, cnt;
        s32 x, y, i, j, c;
        s32 index;
        s32 prev_occ_cnt;

        #ifdef _OPENMP
        #pragma omp for private(y, x, i, j, c) reduction(+:occlusion_amount)
        #endif
        for(y=0; y<height; ++y){
            prev_occ_cnt=0;
            for(x=0; x<width; ++x){
                index = y*width + x;
                if(x-thre_occ_num>=0){
                    if(src.GetState(y*width+x-thre_occ_num)==target_state){
                        --prev_occ_cnt;
                    }
                }
                if(in_mask!=NULL){
                    if(in_mask[index]!=255){
                        io_alpha[index] = 0; continue;
                    }
                }
                if(src.GetState(index)!=target_state){
                    continue;
                }
                ++prev_occ_cnt;

                ++occlusion_amount;

                //! init sums
                sumWeight=0.0;
                sumAlpha=0.0;
            
                for(c=0; c<nChannels; c++){ sum[c]=0.0; }
                cnt=0;
                // get weighted sum of arround pixels
                for(i=-block_size; i<=block_size; i++){
                    for(j=-block_size; j<=block_size; j++){
                        yy = y + i;
                        xx = x + j;
                        if(xx<0 || xx>=width || yy<0 || yy>=height){
                            continue;
                        }
                        ii = yy*width + xx;
                        if(in_mask!=NULL){
                            if(in_mask[ii]!=255){ continue; }
                        }
                        distance = sqrt((double)(i*i+j*j));
                        if(distance>(double)block_size
                            //|| src.GetState(xx, yy)!=WSP_PIXEL_STATE_NORMAL
                            || ((src.GetState(xx, yy)!=WSP_PIXEL_STATE_TRANSPARENT) && (src.GetState(xx, yy)!=WSP_PIXEL_STATE_NORMAL))
                            || (src.GetAlpha(xx, yy)==0)
                            || (use_alpha_weight==false && src.GetAlpha(xx, yy)<255))
                        { continue; }
                        //weight = 1.0;//wsp::math::GetGaussianWeight(distance, variance);
                        //weight = wsp::math::GetGaussianWeight(distance, variance) * (double)src.GetAlpha(xx, yy);
                        weight = wsp::math::GetGaussianWeight(distance, variance);
                        if(use_alpha_weight){ weight*=(double)src.GetAlpha(xx, yy); }

                        for(c=0; c<nChannels; c++){
                            sum[c] += ((double)src[ii*nChannels+c]) * weight;
                        }
                        sumWeight+= weight;
                        sumAlpha += weight;
                        ++cnt;
                    }
                }
                if(cnt==0){ continue; }
                //printf("(%d %d): cnt=%d\n", x, y, cnt);
                for(c=0; c<nChannels; c++){
                    io_img[index*nChannels+c]=(T)(sum[c]/sumWeight);
                }
                //io_img[i*nChannels+0]=255;
                //io_img[i*nChannels+1]=0;
                //io_img[i*nChannels+2]=0;

                //io_img.SetAlpha(255, x, y);
                if(io_alpha!=NULL){
                    if(use_alpha_weight==false){
                        io_alpha[index] = 255;
                    }else{
                        if((double)io_alpha[index]+sumAlpha>=255){ 
                            io_alpha[index]=255; 
                        }else{ 
                            io_alpha[index]+=(uchar)sumAlpha; 
                        }
                    }
                }
                //io_img.AddAlpha(sumAlpha, x, y);
                //src.SetState(WSP_PIXEL_STATE_NORMAL, i);
                if(io_state!=NULL){
                    io_state[index] = WSP_PIXEL_STATE_NORMAL;
                }
                //io_img.SetState(WSP_PIXEL_STATE_NORMAL, x, y);
            }
        }
        delete[] sum;
    }
    return occlusion_amount;
}

template<typename T> 
wsp::State wsp::img::FillUpZeroAlphaHorizontalLine(
        T *io_img, uchar *io_alpha,
        s32 width, s32 height, s32 num_channels,
        bool right_to_left,
        bool fill_alpha, 
        bool fill_around_edge
)
{
    s32 length = width*height;
    s32 width_step = width*num_channels;


    // fill up all pixels ---------------------------------------
    if(right_to_left==false){
        {
            uchar *alpha_ptr = io_alpha+1;
            uchar *end_ptr = io_alpha + length;
            T *io_ptr = io_img+num_channels;
            T *io_c_end_ptr;
            for(; alpha_ptr!=end_ptr; ++alpha_ptr){
                if(*alpha_ptr!=0){ io_ptr+=num_channels; continue; }

                io_c_end_ptr = io_ptr + num_channels;
                for(;io_ptr!=io_c_end_ptr; ++io_ptr){
                    *io_ptr = *(io_ptr-num_channels);
                }
                if(fill_alpha){
                    *alpha_ptr = 255;
                }
            }
        }
        // fill up pixels around edge -------------------------------
        {
            uchar *alpha_edge_ptr, *end_edge;
            uchar *alpha_ptr = io_alpha;
            uchar *end_ptr = io_alpha + length;
            T *io_ptr = io_img;
            T *io_c_end_ptr, *io_edge_ptr;
            if(fill_around_edge){
                for(; alpha_ptr!=end_ptr; alpha_ptr+=width, io_ptr+=width_step){
                    alpha_edge_ptr = alpha_ptr;
                    io_edge_ptr = io_ptr;
                    end_edge = alpha_ptr+width;
                    // find zero alpha around edge
                    for(;*alpha_edge_ptr==0 && alpha_edge_ptr!=end_edge; ++alpha_edge_ptr, io_edge_ptr+=num_channels){}
                    if(alpha_edge_ptr!=end_edge && alpha_edge_ptr!=alpha_ptr){
                        // fill found pixels
                        end_edge = alpha_ptr-1;
                        alpha_edge_ptr-=1;
                        io_edge_ptr-=1;
                        for(;alpha_edge_ptr!=end_edge; --alpha_edge_ptr){
                            io_c_end_ptr = io_edge_ptr-num_channels;
                            for(;io_edge_ptr!=io_c_end_ptr; --io_edge_ptr){
                                *io_edge_ptr = *(io_edge_ptr+num_channels);
                            }
                        }
                    }
                }
            }else{
                for(; alpha_ptr!=end_ptr; alpha_ptr+=width, io_ptr+=width_step){
                    alpha_edge_ptr = alpha_ptr;
                    io_edge_ptr = io_ptr;
                    end_edge = alpha_ptr+width;
                    // find zero alpha around edge
                    for(;*alpha_edge_ptr==0 && alpha_edge_ptr!=end_edge; ++alpha_edge_ptr, io_edge_ptr+=num_channels){}
                    if(alpha_edge_ptr!=end_edge && alpha_edge_ptr!=alpha_ptr){
                        // fill found pixels
                        end_edge = alpha_ptr-1;
                        alpha_edge_ptr-=1;
                        io_edge_ptr-=1;
                        for(;alpha_edge_ptr!=end_edge; --alpha_edge_ptr){
                            io_c_end_ptr = io_edge_ptr-num_channels;
                            for(;io_edge_ptr!=io_c_end_ptr; --io_edge_ptr){
                                *io_edge_ptr = 0;//*(io_edge_ptr+num_channels);
                            }
                        }
                    }
                }
            }
        }
    }else{
        {
            uchar *alpha_ptr = io_alpha + length - 2;
            uchar *end_ptr = io_alpha - 1;
            T *io_ptr = io_img + length*num_channels - num_channels - 1;
            T *io_c_end_ptr;
            for(; alpha_ptr!=end_ptr; --alpha_ptr){
                if(*alpha_ptr!=0){ io_ptr-=num_channels; continue; }

                io_c_end_ptr = io_ptr - num_channels;
                for(;io_ptr!=io_c_end_ptr; --io_ptr){
                    *io_ptr = *(io_ptr+num_channels);
                    //*io_ptr = 0;
                }
                if(fill_alpha){
                    *alpha_ptr = 255;
                }

            }
        }
        // fill up pixels around edge -------------------------------
        {
            uchar *alpha_edge_ptr, *end_edge;
            uchar *alpha_ptr = io_alpha + width - 1;
            uchar *end_ptr = io_alpha + width - 1 + length;
            T *io_ptr = io_img + width_step - 1;
            T *io_c_end_ptr, *io_edge_ptr;
            if(fill_around_edge){
                for(; alpha_ptr!=end_ptr; alpha_ptr+=width, io_ptr+=width_step){
                    alpha_edge_ptr = alpha_ptr;
                    io_edge_ptr = io_ptr;
                    end_edge = alpha_ptr-width;

                    // find zero alpha around edge
                    for(;*alpha_edge_ptr==0 && alpha_edge_ptr!=end_edge; --alpha_edge_ptr, io_edge_ptr-=num_channels){}
                    if(alpha_edge_ptr!=end_edge && alpha_edge_ptr!=alpha_ptr){
                        // fill found pixels
                        end_edge = alpha_ptr+1;
                        alpha_edge_ptr+=1;
                        io_edge_ptr+=1;
                        for(;alpha_edge_ptr!=end_edge; ++alpha_edge_ptr){
                            io_c_end_ptr = io_edge_ptr+num_channels;
                            for(;io_edge_ptr!=io_c_end_ptr; ++io_edge_ptr){
                                *io_edge_ptr = *(io_edge_ptr-num_channels);
                            }
                        }
                    }
                }
            }else{
                for(; alpha_ptr!=end_ptr; alpha_ptr+=width, io_ptr+=width_step){
                    alpha_edge_ptr = alpha_ptr;
                    io_edge_ptr = io_ptr;
                    end_edge = alpha_ptr-width;

                    // find zero alpha around edge
                    for(;*alpha_edge_ptr==0 && alpha_edge_ptr!=end_edge; --alpha_edge_ptr, io_edge_ptr-=num_channels){}
                    if(alpha_edge_ptr!=end_edge && alpha_edge_ptr!=alpha_ptr){
                        // fill found pixels
                        end_edge = alpha_ptr+1;
                        alpha_edge_ptr+=1;
                        io_edge_ptr+=1;
                        for(;alpha_edge_ptr!=end_edge; ++alpha_edge_ptr){
                            io_c_end_ptr = io_edge_ptr+num_channels;
                            for(;io_edge_ptr!=io_c_end_ptr; ++io_edge_ptr){
                                *io_edge_ptr = 0;//*(io_edge_ptr-num_channels);
                            }
                        }
                    }
                }
            }
        }
    }
    
    // fill up all alpha
    //if(fill_alpha){
    //    memset(io_alpha, 255, length);
    //}
    return WSP_STATE_SUCCESS;
}

template<typename T> 
void wsp::img::Inpas32VectorWithMirroredPatternAscending(
    T *io_vector, uchar *io_alpha,
    T *tmp_pattern_data,
    s32 length, s32 num_channels, 
    uchar thre_alpha,
    bool fill_alpha
)
{
    s32 alpha_i, ptn_i, i, c;
    T *hor_st_ptr = io_vector;
    T *hor_ptr = hor_st_ptr;
    T *end_ptr = hor_ptr + length*num_channels;
    uchar *alpha_ptr = io_alpha;
    uchar *end_alpha_ptr = alpha_ptr + length;


    i=0;

    // find edge region
    for(; hor_ptr!=end_ptr; hor_ptr+=num_channels, ++alpha_ptr, ++i){
        if(*alpha_ptr>=thre_alpha){ break; }
    }

    //printf("edge: %d\n", i);

    for(;hor_ptr<end_ptr; hor_ptr+=num_channels, ++alpha_ptr, ++i){
        if(*alpha_ptr>=thre_alpha){ continue; }

        // find number of continuous alpha ----------
        uchar *fw_alpha_ptr = alpha_ptr;
        s32 cont_alpha = 1;
        for(;fw_alpha_ptr<end_alpha_ptr; ++fw_alpha_ptr, ++cont_alpha){
            if(*fw_alpha_ptr>=thre_alpha){ break; }
        }
        // fetch merrored pattern -------------------
        s32 pattern_len = 0;
        T *back_ptr = hor_ptr - num_channels;
        T *ptn_ptr = tmp_pattern_data;
        uchar *back_alpha_ptr = alpha_ptr - 1;
        for(alpha_i=0;
            alpha_i<cont_alpha;
            back_ptr-=num_channels, 
            --back_alpha_ptr, 
            ++alpha_i
        ){
            if(back_ptr==hor_st_ptr){ break; }
            if(*back_alpha_ptr<thre_alpha){ break; }
            for(c=0; c<num_channels; ++c){
                *(ptn_ptr+c) = *(back_ptr+c);
            }
            ptn_ptr+=num_channels;
            ++pattern_len;
        }
        //printf("%d: cont_alpha=%d, pattern_len=%d\n", i, cont_alpha, pattern_len);

        // fill up with pattern
        T *end_blank = hor_ptr + (cont_alpha - 1)*num_channels;
        if(end_blank>=end_ptr){
            end_blank = end_ptr;
        }
        ptn_ptr = tmp_pattern_data;
        T *st_ptn = ptn_ptr;
        T *end_ptn = ptn_ptr + pattern_len*num_channels;
        bool is_backward = false;
        for(; hor_ptr!=end_blank; 
            hor_ptr+=num_channels, 
            ++alpha_ptr, 
            ++i
        ){
            if(fill_alpha){
                *alpha_ptr = 255;
            }
            for(c=0; c<num_channels; ++c){
                *(hor_ptr+c) = *(ptn_ptr+c);
            }
            if(is_backward==true){
                ptn_ptr-=num_channels;
            }else{
                ptn_ptr+=num_channels;
            }
            if(ptn_ptr==end_ptn){
                is_backward = true;
            }else if(ptn_ptr==st_ptn){
                is_backward = false;
            }
        }
    }
    //if(fill_alpha){
    //    memset(io_alpha, 0xff, length);
    //}
}


template<typename T> void wsp::img::Inpas32VectorWithMirroredPatternDescending(
    T *io_vector, uchar *io_alpha,
    T *tmp_pattern_data, /* temporary buffer for pattern array(memory size must be larger than length*num_channels) */
    s32 length, s32 num_channels, 
    uchar thre_alpha,
    bool fill_alpha
){
    s32 alpha_i, ptn_i, i, c;
    T *hor_st_ptr = io_vector + (length-1)*num_channels;
    T *hor_ptr = hor_st_ptr;
    T *end_ptr = io_vector - num_channels;
    uchar *alpha_ptr = io_alpha + length - 1;
    uchar *end_alpha_ptr = io_alpha - 1;


    i=length-1;

    // find edge region
    for(; hor_ptr!=end_ptr; hor_ptr-=num_channels, --alpha_ptr, --i){
        if(*alpha_ptr>=thre_alpha){ break; }
    }

    //printf("edge: %d\n", i);

    for(;hor_ptr>end_ptr; hor_ptr-=num_channels, --alpha_ptr, --i){
        if(*alpha_ptr>=thre_alpha){ continue; }

        // find number of continuous alpha ----------
        uchar *fw_alpha_ptr = alpha_ptr;
        s32 cont_alpha = 1;
        for(;fw_alpha_ptr>end_alpha_ptr; --fw_alpha_ptr, ++cont_alpha){
            if(*fw_alpha_ptr>=thre_alpha){ break; }
        }
        // fetch merrored pattern -------------------
        s32 pattern_len = 0;
        T *back_ptr = hor_ptr + num_channels;
        T *ptn_ptr = tmp_pattern_data;
        uchar *back_alpha_ptr = alpha_ptr + 1;
        for(alpha_i=0;
            alpha_i<cont_alpha;
            back_ptr+=num_channels, 
            ++back_alpha_ptr, 
            ++alpha_i
        ){
            if(back_ptr==hor_st_ptr){ break; }
            if(*back_alpha_ptr<thre_alpha){ break; }
            for(c=0; c<num_channels; ++c){
                *(ptn_ptr+c) = *(back_ptr+c);
            }
            ptn_ptr+=num_channels;
            ++pattern_len;
        }
        //printf("%d: cont_alpha=%d, pattern_len=%d\n", i, cont_alpha, pattern_len);

        // fill up with pattern
        T *end_blank = hor_ptr - (cont_alpha - 1)*num_channels;
        if(end_blank<end_ptr){
            end_blank = end_ptr;
        }
        ptn_ptr = tmp_pattern_data;
        T *st_ptn = ptn_ptr;
        T *end_ptn = ptn_ptr + (pattern_len-1)*num_channels;
        bool is_backward = false;
        for(; hor_ptr>end_blank; 
            hor_ptr-=num_channels, 
            --alpha_ptr, 
            --i
        ){
            if(fill_alpha){
                *alpha_ptr = 255;
            }
            for(c=0; c<num_channels; ++c){
                *(hor_ptr+c) = *(ptn_ptr+c);
            }
            if(is_backward==true){
                ptn_ptr-=num_channels;
            }else{
                ptn_ptr+=num_channels;
            }
            if(ptn_ptr>=end_ptn){
                is_backward = true;
            }else if(ptn_ptr<=st_ptn){
                is_backward = false;
            }
        }
    }
    //if(fill_alpha){
    //    memset(io_alpha, 0xff, length);
    //}
}


template<typename T> wsp::State wsp::img::Inpas32WithHorizontalPatternAscending(
    T *io_img, uchar *io_alpha,
    s32 width, s32 height, s32 num_channels, 
    uchar thre_alpha,
    bool fill_alpha
){
    s32 w_step = width*num_channels;
    s32 nelem = height*w_step;
    {
        T *pattern_data = (T*)malloc(sizeof(T)*w_step);
        s32 x, y, c, i, pt_i;
        for(y=0; y<height; ++y){
            T *hor_st_ptr = io_img + y*w_step;
            uchar *alpha_ptr = io_alpha + y*width;
            wsp::img::Inpas32VectorWithMirroredPatternAscending(
                hor_st_ptr, alpha_ptr, pattern_data, 
                width, num_channels, thre_alpha, fill_alpha
            );
        }
        free(pattern_data);
    }
    return WSP_STATE_SUCCESS;
}

template<typename T> wsp::State wsp::img::Inpas32WithHorizontalPatternDescending(
    T *io_img, uchar *io_alpha,
    s32 width, s32 height, s32 num_channels, 
    uchar thre_alpha,
    bool fill_alpha
){
    s32 w_step = width*num_channels;
    s32 nelem = height*w_step;
    {
        T *pattern_data = (T*)malloc(sizeof(T)*w_step);
        s32 x, y, c, i, pt_i;
        for(y=0; y<height; ++y){
            T *hor_st_ptr = io_img + y*w_step;
            uchar *alpha_ptr = io_alpha + y*width;
            wsp::img::Inpas32VectorWithMirroredPatternDescending(
                hor_st_ptr, alpha_ptr, pattern_data, 
                width, num_channels, thre_alpha, fill_alpha
            );
        }
        free(pattern_data);
    }
    return WSP_STATE_SUCCESS;
}

template<typename T> void BlendAcendingAndDescending(
    const T *in_ascending_data, const T *in_descending_data, const uchar *in_alpha,
    T *o_data,
    s32 length, s32 num_channels, uchar thre_alpha
){
    s32 i, j, c;
    const uchar *alpha_ptr = in_alpha;
    const T *ds_ptr = in_descending_data;
    const T *as_ptr = in_ascending_data;
    T *dst_ptr = o_data;
    for(i=0; i<length; ){
        if(*alpha_ptr>=thre_alpha){
            ++i, 
            ++alpha_ptr,
            ds_ptr+=num_channels,
            as_ptr+=num_channels,
            dst_ptr+=num_channels;
            continue;
        }
        // count continuous alpha
        s32 cont_alpha = 0;
        for(; *alpha_ptr<thre_alpha && i<length; 
            ++alpha_ptr,
            ++i,
            ++cont_alpha
        ){}
        double norm_dist; /* 1.0-0.0 distance */
        double ascending_weight, descending_weight;
        double denom = static_cast<double>(cont_alpha-1);
        for(j=0; j<cont_alpha; ++j, dst_ptr+=num_channels, as_ptr+=num_channels, ds_ptr+=num_channels){
            norm_dist = j/denom;
            // gaussian ------
            ascending_weight = exp(-(norm_dist*norm_dist)*4.0);
            descending_weight = 1.0 - ascending_weight;
            // ---------------

            // linear -----
            //descending_weight = j/denom;
            //ascending_weight = 1.0 - descending_weight;
            // ------------
            for(c=0; c<num_channels; ++c){
                *(dst_ptr+c) = (*(as_ptr+c))*ascending_weight + (*(ds_ptr+c))*descending_weight; 
            }
        }
    }
}

template<typename T> void wsp::img::Inpas32WithHorizontalPatternBlend(
    T *io_img, uchar *io_alpha,
    s32 width, s32 height, s32 num_channels, 
    uchar thre_alpha,
    bool fill_alpha
){
    s32 w_step = width*num_channels;
    s32 nelem = height*w_step;
    {
        T *pattern_data = (T*)malloc(sizeof(T)*w_step);
        T *tmp_ds_line = (T*)malloc(sizeof(T)*w_step);
        T *tmp_as_line = (T*)malloc(sizeof(T)*w_step);
        uchar *tmp_alpha = (uchar*)malloc(width);
        s32 x, y, c, i, pt_i;
        for(y=0; y<height; ++y){
            T *hor_st_ptr = io_img + y*w_step;
            uchar *alpha_ptr = io_alpha + y*width;
            wsp::CopyArray(tmp_ds_line, hor_st_ptr, w_step);
            wsp::CopyArray(tmp_as_line, hor_st_ptr, w_step);
            wsp::CopyArray(tmp_alpha, alpha_ptr, width);
            wsp::img::Inpas32VectorWithMirroredPatternAscending(
                tmp_as_line, tmp_alpha, pattern_data, 
                width, num_channels, thre_alpha, fill_alpha
            );
            wsp::CopyArray(tmp_alpha, alpha_ptr, width);
            wsp::img::Inpas32VectorWithMirroredPatternDescending(
                tmp_ds_line, tmp_alpha, pattern_data, 
                width, num_channels, thre_alpha, fill_alpha
            );
            BlendAcendingAndDescending(
                tmp_as_line, tmp_ds_line, alpha_ptr, hor_st_ptr, 
                width, num_channels, thre_alpha
            );
            //memcpy(hor_st_ptr, tmp_ds_line, w_step*sizeof(T));
            
        }
        free(pattern_data);
        free(tmp_ds_line);
        free(tmp_as_line);
        free(tmp_alpha);
    }
    if(fill_alpha){
        memset(io_alpha, 0xff, width*height);
    }
}

template<typename T>
wsp::State wsp::img::FillUpTimes(
        T *io_img, 
        uchar *io_alpha, 
        uchar *io_state, 
        const uchar *in_mask, 
        s32 width, s32 height, s32 nChannels,  
        wsp::img::PixelState target_state, 
        s32 init_blocksize, 
        s32 blocksize_step, 
        double variance, 
        bool use_alpha_weight,
        bool is_directional,
        uchar thre_transparent_alpha
){
    s32 blocksize = init_blocksize;
    s32 occlusion_amount[3] = {0, width*height, width*height};
    char filename[64];

    s32 loopCnt=0;
    do{
        if(is_directional){
            occlusion_amount[0] = wsp::img::FillUpGaussianAverageHorizontalDirection<T>(
                                        io_img, io_alpha, io_state, in_mask, 
                                        width, height, nChannels,
                                        target_state, blocksize, variance, use_alpha_weight
            );
        }else{
            occlusion_amount[0] = wsp::img::FillUpGaussianAverage<T>(
                                        io_img, io_alpha, io_state, in_mask, 
                                        width, height, nChannels,
                                        target_state, blocksize, variance, use_alpha_weight,
                                        thre_transparent_alpha
            );
        }
        //return;

        printf("fill up: rest of occlusions are %d pixels, size=%d, variance=%f\n", occlusion_amount[0], blocksize, variance);
        blocksize+=blocksize_step;
        if(occlusion_amount[0]==0){
            printf("all of occlusions are filled.\n\n\n");
            break;
        }
        if(   occlusion_amount[0] == occlusion_amount[1] 
           && occlusion_amount[1] == occlusion_amount[2]
        ){
            printf("The amounts of occlusions are the same as previous operation\n");
            break; 
        }
        occlusion_amount[2] = occlusion_amount[1];
        occlusion_amount[1] = occlusion_amount[0];
        loopCnt++;
    }while(occlusion_amount[0]>0 && loopCnt<=MAX_WHILE_LOOP);


    return WSP_STATE_SUCCESS;
}


// ===========================================
//                   Stereo
// ===========================================

template<typename InType, typename OutType>
wsp::State wsp::img::StereoStandardBM(
    const InType *in_main,
    const InType *in_sub, 
    OutType *o_disp, 
    s32 width, s32 height, s32 num_channels,
    const wsp::img::StereoBMParams &in_param
){
    s32 w_step = width*num_channels;
    s32 num_pix = width*height;
    s32 num_elem = num_pix * num_channels;
    s32 blk_hw = in_param.block_min*num_channels;
    s32 blk_hh = in_param.block_min;
    s32 blk_w = blk_hw*2+1;
    s32 blk_h = blk_hh*2+1;
    s32 blk_len = blk_h*blk_w*num_channels;
    {
        s32 x, y, index;
        s32 i, j, dx, dy;
        InType *main_buf = (InType*)malloc(sizeof(InType)*blk_len);
        InType *sub_buf = (InType*)malloc(sizeof(InType)*blk_len);
        //const InType *main_ptr = in_main;
        //const InType *sub_ptr = in_sub;
        //OutType *disp_ptr = o_disp;
        //const InType *end_ptr = main_ptr + num_elem;

        //for(;main_ptr!=end_ptr; ++main_ptr, ++sub_ptr, ++disp_ptr){
        for(y=0; y<height; ++y){
            for(x=0; x<width; ++x){
                index = (y*width + x)*num_channels;
                for(i=-blk_hh; i<=blk_hh; ++i){
                    for(j=-blk_hw; j<=blk_hw; ++j){
                        main_buf[i*blk_w+j]=in_main[index+i*w_step+j];
                    }
                }
                for(dy=in_param.dispy_min; dy<=in_param.dispy_max; ++dy){
                    for(dx=in_param.dispx_min; dx<=in_param.dispx_max; ++dx){
                        for(i=-blk_hh; i<=blk_hh; ++i){
                            for(j=-blk_hw; j<=blk_hw; ++j){
                                sub_buf[(i+blk_hh)*blk_w+j+blk_hw] = in_main[index+(i+blk_hh)*w_step+j+blk_hw];
                            }
                        }

                    }
                }
            }
        }
        free(main_buf);
        free(sub_buf);
    }
    return WSP_STATE_SUCCESS;
}


// ===========================================
//               Segmentation
// ===========================================
template<typename _SegmIntegerType, typename _RefType, typename _OutSegmType>
wsp::State wsp::img::FindPrincipalSegments(
        const _SegmIntegerType *in_segment_map, 
        const _RefType *in_ref_map, 
        _OutSegmType *o_principal_segm_map,
        s32 length, 
        f32 hit_percentage_thre,
        _SegmIntegerType segm_min_level,
        _SegmIntegerType segm_max_level
)
{
    if( hit_percentage_thre<=0 || hit_percentage_thre>=1.0f ){
        WSP_COMMON_ERROR_LOG("hit_percentage_thre must be 0.0 to 1.0: %f\n", hit_percentage_thre);
        return WSP_STATE_INVALID_PARAMETER;
    }
    const _OutSegmType FILL_VALUE = 255;
    s32 num_segm_id = (segm_max_level-segm_min_level);
    s32 *hit_id_cnt_list   = (s32*)malloc( num_segm_id * sizeof(s32) );
    s32 *total_id_cnt_list = (s32*)malloc( num_segm_id * sizeof(s32) );
    s32 *principal_id_list = (s32*)malloc( num_segm_id * sizeof(s32) );

    if( hit_id_cnt_list==NULL || total_id_cnt_list==NULL || principal_id_list==NULL ){
        WSP_COMMON_ERROR_LOG("Memory allocation failed\n");
        free(hit_id_cnt_list); free(total_id_cnt_list); free(principal_id_list);
        return WSP_STATE_INSUFFICIENT_MEMORY;
    }

    memset( o_principal_segm_map, 0, length * sizeof(_OutSegmType) );

    /* Get id count list */
    {
        s32 segm_cnt = 0;
        s32 index;
        const _SegmIntegerType *segm_ptr = in_segment_map;
        const _RefType *ref_ptr = in_ref_map;
        const _RefType *end_ptr = ref_ptr + length;
        for( ; ref_ptr!=end_ptr; ++segm_ptr, ++ref_ptr)
        {
            index = *segm_ptr-segm_min_level;
            ++(total_id_cnt_list[index]);
            if( *ref_ptr < 128 ){ continue; }

            ++(hit_id_cnt_list[index]);
        }
    }

    /* Determine which are the principal segments */
    {
        f32 hit_percentage;
        s32 *hit_ptr       = hit_id_cnt_list;
        s32 *total_ptr     = total_id_cnt_list;
        s32 *principal_ptr = principal_id_list;
        s32 *end_ptr       = hit_ptr + num_segm_id;
        for( s32 segm_id=segm_min_level; 
             hit_ptr!=end_ptr; 
             ++hit_ptr, 
             ++total_ptr, 
             ++principal_ptr, 
             ++segm_id 
        )
        {
            if( *total_ptr==0 ){ continue; }

            hit_percentage = (*hit_ptr)/(f32)(*total_ptr);
            if( hit_percentage < hit_percentage_thre ){ continue; }

            WSP_COMMON_DEBUG_LOG("segm id %d hits ( %f >= %f )\n", segm_id, hit_percentage, hit_percentage_thre);

            /* Fill up a pricipal segment in destination map */
            s32 segm_cnt = 0;
            const _SegmIntegerType *segm_ptr = in_segment_map;
            _OutSegmType *dst_ptr = o_principal_segm_map;
            _OutSegmType *end_ptr = dst_ptr + length;
            for( ; dst_ptr!=end_ptr; ++segm_ptr, ++dst_ptr)
            {
                if( *segm_ptr==segm_id )
                {
                    *dst_ptr = FILL_VALUE;
                }
            }
        }
    }
    free( hit_id_cnt_list );
    free( total_id_cnt_list );

    return WSP_STATE_SUCCESS;
}

// ===========================================
//                   Other
// ===========================================

template<typename T>
void wsp::img::EstimateBackgroundArea(
    const T *in_segm, 
    uchar   *o_state, 
    s32     width, 
    s32     height, 
    s32     num_segm, 
    double  bg_top_perct, 
    double  top_height_perct, 
    double  far_depth_perct,
    bool    lower_segm_is_near, 
    uchar   bg_state
)
{
    s32 top_h = static_cast<s32>(height * top_height_perct);
    s32 far_d = static_cast<s32>(num_segm * far_depth_perct);
    s32 len = width*height;
    {
        s32 y, i;
        s32 all_top_pix = width*top_h;
        s32 *segm_cnt_buf = (s32*)malloc(sizeof(s32)*num_segm);
        //uchar *segm_state_buf = (uchar*)malloc(num_segm);

        memset(segm_cnt_buf, 0, sizeof(s32)*num_segm);
        //memset(segm_state_buf, 0, num_segm);

        const T *segm_ptr;
        uchar *state_ptr, *end_ptr;
        s32 step;

        for( y=0; y<top_h; ++y )
        {
            step = y*width;
            segm_ptr = in_segm+step;
            state_ptr = o_state+step;
            end_ptr = state_ptr + width;
            for(; state_ptr!=end_ptr; ++state_ptr, ++segm_ptr)
            {
                ++(segm_cnt_buf[static_cast<s32>(*segm_ptr)]);
            }
        }
        s32 start_far, end_far;
        if(lower_segm_is_near)
        {
            start_far = far_d;
            end_far = num_segm;
        }
        else
        {
            start_far = 0;
            end_far = far_d+1;
        }
        for( i=start_far; i<end_far; ++i )
        {
            double top_perct = segm_cnt_buf[i]/static_cast<double>(all_top_pix);
            if(top_perct>bg_top_perct)
            {
                segm_ptr = in_segm;
                state_ptr = o_state;
                end_ptr = state_ptr + len;
                for(; state_ptr!=end_ptr; ++state_ptr, ++segm_ptr)
                {
                    if(*segm_ptr==i)
                    {
                        *state_ptr = bg_state;
                    }
                }
            }
        }
        free(segm_cnt_buf);
    }
}


template<typename T>
void wsp::img::GetCharacteristicMask(
    uchar *o_state, 
    const T *in_src, 
    s32 width, s32 height, s32 nChannels, 
    s32 win_size, 
    double thre_av, 
    double (*eval_func)(double, double),
    const double *in_winSizeMap
){
    if(in_winSizeMap!=NULL){
        WSP_IMAGE_ERROR_LOG("dynamic window is not supported yet\n");
        return;
    }
    s32 x, y, i, j, c;
    s32 index, cnt;
    s32 w_size = win_size;
    s32 w=width, h=height, nc=nChannels;
    double *avs = WSP_NEW double[nc];
    double *vars= WSP_NEW double[nc];
    double var_av;

    for(y=0; y<h; y++){
        WSP_IMAGE_DEBUG_LOG("progress: %d / %d\r", y, h);
        //yw = y*w;
        for(x=0; x<w; x++){
            //index = yw+x;
            //if(in_winSizeMap!=NULL){
            //    w_size = (s32)in_winSizeMap[y*w + x];
            //}

            // initialize sum
            for(c=0; c<nc; c++){
                avs[c] = 0.0;
                vars[c] = 0.0;
            }
            cnt=0;
            // get average within block
            for(i=-w_size; i<=w_size; i++){
                for(j=-w_size; j<=w_size; j++){
                    if(y+i<0 || y+i>=h || x+j<0 || x+j>=w){ continue; }

                    index = ((y+i)*w + (x+j)) * nc;
                    for(c=0; c<nc; c++){
                        avs[c] += (double)in_src[index+c];
                    }
                    cnt++;
                }
            }

            for(c=0; c<nc; c++){ avs[c] /= (double)cnt; }

            cnt=0;
            // get variance
            for(i=-w_size; i<=w_size; i++){
                for(j=-w_size; j<=w_size; j++){
                    if(y+i<0 || y+i>=h || x+j<0 || x+j>=w){ continue; }

                    index = ((y+i)*w + (x+j)) * nc;
                    for(c=0; c<nc; c++){
                        //vars[c] += ((double)in_src[index+c] - avs[c])*((double)in_src[index+c] - avs[c]);
                        vars[c] += eval_func(static_cast<double>(in_src[index+c]), static_cast<double>(avs[c]));
                    }
                    cnt++;
                }
            }

            // get sum of average of each channels
            var_av = 0.0;
            for(c=0; c<nc; c++){ 
                vars[c] /= (double)cnt; 
                var_av += vars[c];
            }

            var_av/=(double)nc;

            if(var_av > thre_av){
                o_state[y*w + x] = WSP_PIXEL_STATE_NORMAL;
            }else{
                o_state[y*w + x] = WSP_PIXEL_STATE_MASKED;
            }
        }
    }
    delete[] avs;
    delete[] vars;
}

template<typename T>
void wsp::img::MultiplyGausssianDistributionWeight(
    T *io_img,
    s32 width, s32 height, s32 num_channels,
    s32 gauss_center_x, 
    s32 gauss_center_y,
    double gauss_sigma,
    double gauss_max
)
{
    #ifdef _OPENMP
    #pragma omp parallel
    #endif
    {
        s32 x, y, i, c;
        double xv, yv;
        double weight;

        #ifdef _OPENMP
        #pragma omp for
        #endif
        for(y=0; y<height; ++y)
        {
            for(x=0; x<width; ++x)
            {
                i = (y * width + x) * num_channels;
                xv = (double)((s32)x-(s32)gauss_center_x);
                yv = (double)((s32)y-(s32)gauss_center_y);
                weight = exp( - (xv*xv + yv*yv) / (2*gauss_sigma*gauss_sigma) ) * gauss_max;
                for(c=0; c<num_channels; ++c)
                {
                    io_img[i+c] *= weight;
                }
            }
        }
    }
}


#endif