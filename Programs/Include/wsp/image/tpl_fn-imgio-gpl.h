// @file tpl_fn-imgio-gpl.h
// @author Jun-ichi Nishikata
// Copyright (C) 2010 - 2012 WestSidePure.com All Rights Reserved.

#ifndef _WSP_TEMPLATE_BASE_FN_IMG_IO_GPL_H_
#define _WSP_TEMPLATE_BASE_FN_IMG_IO_GPL_H_

#ifndef __WSP_IMAGEUTIL_DEFINE_IMAGEUTIL_H__
#include "_define_image.h"
#endif

#ifdef USE_DEVIL
#include <IL/il.h>
#endif

namespace wsp{ namespace img{

    #ifdef USE_DEVIL
    //! base function for loading image with DevIL
    template<typename T> 
    wsp::State ilLoad8BitImage(
        T **o_color, 
        uchar **o_alpha, 
        int &o_width, int &o_height, int &o_nChannel, 
        const char *in_filename, 
        bool separate_color_and_alpha=true, 
        int current_length=0, int current_num_channels=0, int alloc_step=1, bool show_info=false);

    //! interit functions
    template<typename T> 
    inline wsp::State ilLoadRGBAImage(
        T **o_img, 
        int &o_width, int &o_height, int &o_nChannel, 
        const char *in_filename,
        int current_length=0, int current_num_channels=0, int alloc_step=1, bool show_info=false);

    template<typename T> 
    inline wsp::State ilLoadRGBImageAndAlpha(
        T **o_img, uchar **o_alpha, int &o_width, int &o_height, int &o_nChannel, 
        const char *in_filename, int current_length=0, int current_num_channels=0, int alloc_step=1, bool show_info=false);
    //! base function for saving image with DevIL
    template<typename T> 
    wsp::State ilSaveAs8BitImage(const T *in_img, const uchar *in_alpha, int width, int height, int num_channels, const char *in_filename);
    #endif

}}


#ifdef USE_DEVIL
template<typename T> 
wsp::State wsp::img::ilLoad8BitImage(
    T **o_color, uchar **o_alpha, int &o_width, int &o_height, int &o_num_channels, 
    const char *in_filename, bool separate_color_and_alpha,
    int current_length, int current_num_channels, int alloc_step, bool show_info)
{
    ilInit();

    int size, len, bytes_per_pixel, bytes_per_channel;
    ILboolean success = 1;
    ILuint il_img = 0;
    bool exist_alpha = false;

    ilGenImages(1, &il_img);
    ilBindImage(il_img);
    success = ::ilLoadImage((const ILstring)in_filename) ;
    if(!success){
        ILenum err = ilGetError(); 
        WSP_COMMON_ERROR_LOG("DevIL Error %d: %s\n", err, ilGetString(err));
        throw std::exception(ilGetString(err));
        return WSP_STATE_FAILURE;
    }

    size = ilGetInteger( IL_IMAGE_SIZE_OF_DATA );
    o_width =ilGetInteger(IL_IMAGE_WIDTH);
    o_height =ilGetInteger(IL_IMAGE_HEIGHT);
    bytes_per_pixel = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
    len = o_width * o_height;

    //! Converting image by format
    //printf("    image format=");
	int source_num_channels = 1;
    switch(ilGetInteger(IL_IMAGE_FORMAT))
    {
    case IL_RGB:
    case IL_BGR:
        success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
        o_num_channels = 3;
		source_num_channels = 3;
		bytes_per_channel = bytes_per_pixel / 3;
        break;
    case IL_RGBA:
    case IL_BGRA:
        success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		source_num_channels = 4;
		o_num_channels = separate_color_and_alpha ? 3 : 4;
		bytes_per_channel = bytes_per_pixel / 4;
		exist_alpha = true;
        break;
    case IL_COLOR_INDEX:
    case IL_LUMINANCE:
        o_num_channels=1;
		bytes_per_channel = bytes_per_pixel;
		break;
    default:
        WSP_COMMON_ERROR_LOG("Unsupported image format\n"); 
        throw std::exception("Unsupported image format");
		return WSP_STATE_UNKNOWN_PARAMETER;
    }

    if(!success)
	{
        ILenum err = ilGetError();
        WSP_COMMON_ERROR_LOG("ilConvertImage failed\n"); 
        throw std::exception(ilGetString(err));
        return WSP_STATE_FAILURE;
    }

	ILubyte *il_bytes = ilGetData();

	if (ilGetInteger(IL_IMAGE_ORIGIN) == IL_ORIGIN_LOWER_LEFT)
	{
		wsp::Flip2dVer(il_bytes, o_width, o_height, source_num_channels);
    }

    if(show_info)
	{
        printf("%s successfully loaded:\n", in_filename);
        printf("    width=%d, height=%d, data size=%d\n", o_width, o_height, size);
        printf("    bytes_per_pixel=%d, converted bytes_per_pixel=%d,", bytes_per_pixel, o_num_channels);
        printf(" origin=%s\n", ilGetInteger(IL_ORIGIN_MODE)==IL_ORIGIN_LOWER_LEFT?"lower-left":"upper-left");
    }

    // Copy data ---------------------------------------------------------
    //! memory allocation
    if(o_color!=NULL)
	{
        wsp::ReallocArrayMemByStep(o_color, len*o_num_channels, current_length*current_num_channels, 0, alloc_step);
    }

    if(o_alpha!=NULL)
	{
        wsp::ReallocArrayMemByStep(o_alpha, len, current_length, 0xff, alloc_step);
    }


    // Copy
    {
        T *dst_ptr, *end_ptr;
        uchar *src_ptr;
        uchar *alpha_ptr, *end_alpha_ptr;

        if(o_alpha!=NULL && exist_alpha){
            alpha_ptr = *o_alpha;
            end_alpha_ptr = *o_alpha + len;
			src_ptr = il_bytes + source_num_channels - 1;
			for (; alpha_ptr != end_alpha_ptr; ++alpha_ptr, src_ptr += source_num_channels)
			{
                *alpha_ptr = *src_ptr;
            }
        }

        dst_ptr = *o_color;
        end_ptr = *o_color+len*o_num_channels;
        src_ptr = il_bytes;
		int nc_diff = source_num_channels - o_num_channels;
        if(nc_diff==0)
		{
            for(; dst_ptr!=end_ptr; ++dst_ptr, ++src_ptr)
			{
                *dst_ptr = static_cast<T>(*src_ptr);
            }
        }
		else
		{
            int cnt=0;
            for(; dst_ptr!=end_ptr; ++dst_ptr, ++src_ptr)
			{
                *dst_ptr = static_cast<T>(*src_ptr);
                ++cnt;
                if(cnt==o_num_channels)
				{
                    src_ptr+=nc_diff;
                    cnt=0;
                }
            }
        }
    }

    ilDeleteImages(1, &il_img);
    return WSP_STATE_SUCCESS;
}
template<typename T> 
inline wsp::State wsp::img::ilLoadRGBAImage(T **o_img, int &o_width, int &o_height, int &o_num_channels, const char *in_filename,
                                            int current_length, int current_num_channels, int alloc_step, bool show_info)
{
    return wsp::img::ilLoad8BitImage(o_img, NULL, o_width, o_height, o_num_channels, in_filename, false, 
                                        current_length, current_num_channels, alloc_step, show_info);
}
template<typename T>
inline wsp::State wsp::img::ilLoadRGBImageAndAlpha(T **o_img, uchar **o_alpha, int &o_width, int &o_height, int &o_num_channels, 
                                            const char *in_filename, 
                                            int current_length, int current_num_channels, int alloc_step, bool show_info)
{
    return wsp::img::ilLoad8BitImage(o_img, o_alpha, o_width, o_height, o_num_channels, in_filename, true, 
                                        current_length, current_num_channels, alloc_step, show_info);
}
template<typename T> 
wsp::State wsp::img::ilSaveAs8BitImage(const T *in_img, const uchar *in_alpha, int width, int height, int num_channels, const char *in_filename)
{
    // copy data in rgba data ---------------------------------------------
    int dst_nc = 3;
    if(num_channels==1){ dst_nc=1; }
    if(in_alpha!=NULL){ dst_nc=4; } //! include alpha in number of channels
    uchar *tmp_rgba = new uchar[width*height*dst_nc];

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

    // DevIL operations ---------------------------------------------------
    ilInit();

    ILboolean success;
    ILuint img = 0;

    ilHint(IL_COMPRESSION_HINT, IL_USE_COMPRESSION);
    ilSetInteger (IL_JPG_QUALITY, 80);

    //if(ilGetInteger(IL_IMAGE_ORIGIN)==IL_ORIGIN_LOWER_LEFT){
    //    wsp::Flip2dVer(tmp_img, width, height, num_channels);
    //}    printf("Saving image as %s:\n", filename);
    ilGenImages(1, &img);
    ilBindImage(img);

    if(dst_nc==1){
        success = ilTexImage(width, height, 1, dst_nc, IL_LUMINANCE, IL_UNSIGNED_BYTE, reinterpret_cast<void*>(tmp_rgba));
    }else{
        success = ilTexImage(width, height, 1, dst_nc, IL_RGBA, IL_UNSIGNED_BYTE, reinterpret_cast<void*>(tmp_rgba));
    }
    if(!success){ WSP_COMMON_ERROR_LOG("ilTexImage failed\n"); return WSP_STATE_FAILURE; }

    ilEnable(IL_FILE_OVERWRITE);

    success = ::ilSaveImage((const ILstring)in_filename);
    if(!success){
        ILenum err = ilGetError();
        WSP_COMMON_ERROR_LOG("DevIL Error %d: %s\n", err, ilGetString(err));
        return WSP_STATE_FAILURE;
    }

    //printf("%s successfully saved\n", in_filename);
    //printf("    width=%d, height=%d, data size=%d\n", width, height, width*height*num_channels);
    //printf("    number of channels %d, ", num_channels);
    //printf(" origin=%s\n", ilGetInteger(IL_ORIGIN_MODE)==IL_ORIGIN_LOWER_LEFT?"lower-left":"upper-left");

    ilDeleteImages(1,&img);
    // ------------------------------------------------------------

    delete[] tmp_rgba;
    return WSP_STATE_SUCCESS;
}
#endif

#endif