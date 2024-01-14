/**
 * @file fn-imgio-gpl.cpp
 * 
 */

#include "fn-png_io.h"

#include <wsp/common/_tpl_fn-array.hpp>
#include <wsp/common/fn-debug.h>
#include <wsp/common/ScopedFileOpen.h>

#ifdef __cplusplus
extern "C"{
#endif
#include <png.h>
#ifdef __cplusplus
}
#endif

#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace wsp::libpng;

namespace {
    class ScopedPngReadStructAllocator
    {
    public:
        ScopedPngReadStructAllocator()
        {
            png_struct_ptr_ = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
            if (png_struct_ptr_ == NULL)
            {
                throw LIBPNG_RESULT_CREATE_PNG_STRUCT_FAILED;
            }

            png_info_ptr_ = png_create_info_struct(png_struct_ptr_);
            if (png_info_ptr_ == NULL)
            {
                png_destroy_read_struct(&png_struct_ptr_, NULL, NULL);
                throw LIBPNG_RESULT_CREATE_INFO_STRUCT_FAILED;
            }
        }

        ~ScopedPngReadStructAllocator() throw()
        {
            png_destroy_read_struct(&png_struct_ptr_, &png_info_ptr_, (png_infopp)NULL);
        }

        png_structp png_struct_ptr() throw()
        {
            return png_struct_ptr_;
        }

        png_infop png_info_ptr() throw()
        {
            return png_info_ptr_;
        }

    private:
        png_structp png_struct_ptr_;
        png_infop png_info_ptr_;
    };

    class ScopedPngWriteStructAllocator
    {
    public:
        ScopedPngWriteStructAllocator()
        {
            png_struct_ptr_ = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
            if (png_struct_ptr_ == NULL)
            {
                throw LIBPNG_RESULT_CREATE_PNG_STRUCT_FAILED;
            }

            png_info_ptr_ = png_create_info_struct(png_struct_ptr_);
            if (png_info_ptr_ == NULL)
            {
                png_destroy_write_struct(&png_struct_ptr_, NULL);
                throw LIBPNG_RESULT_CREATE_INFO_STRUCT_FAILED;
            }
        }

        ~ScopedPngWriteStructAllocator() throw()
        {
            png_destroy_write_struct(&png_struct_ptr_, &png_info_ptr_);
        }

        png_structp png_struct_ptr() throw()
        {
            return png_struct_ptr_;
        }

        png_infop png_info_ptr() throw()
        {
            return png_info_ptr_;
        }

    private:
        png_structp png_struct_ptr_;
        png_infop png_info_ptr_;
    };
}

LibpngResult wsp::libpng::SavePng(
    const u8 *in_image,
    int width,
    int height,
    int num_channels,
    int width_step,
    const char* file_path)
{
    unsigned char header[8];    // 8 is the maximum size that can be checked
    png_structp png_ptr;
    png_infop info_ptr;

    /* create file */
    wsp::common::ScopedOpenFile scoped_open_file(file_path, "wb");
    FILE *fp = scoped_open_file.GetOpenedFilePointer();
    if (!fp)
    {
        return LIBPNG_RESULT_COULD_NOT_OPEN_FILE;
    }

    try
    {
        /* initialize stuff */
        ScopedPngWriteStructAllocator allocator;
        png_structp png_ptr = allocator.png_struct_ptr();
        png_infop info_ptr = allocator.png_info_ptr();

        if (setjmp(png_jmpbuf(png_ptr)))
        {
            return LIBPNG_RESULT_INIT_IO_FAILED;
        }

        png_init_io(png_ptr, fp);

        /* write header */
        if (setjmp(png_jmpbuf(png_ptr)))
        {
            return LIBPNG_RESULT_WRITE_HEADER_FAILED;
        }

        int bit_depth = 8;
        int color_type;
        switch (num_channels)
        {
        case 1:
            color_type = PNG_COLOR_TYPE_GRAY;
            break;
        case 2:
            color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
            break;
        case 3:
            color_type = PNG_COLOR_TYPE_RGB;
            break;
        case 4:
            color_type = PNG_COLOR_TYPE_RGBA;
            break;
        default:
            WSP_FATAL("invalid number of channels");
        }

        png_set_IHDR(png_ptr, info_ptr, width, height,
            bit_depth, color_type, PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

        png_write_info(png_ptr, info_ptr);

        /* write bytes */
        if (setjmp(png_jmpbuf(png_ptr)))
        {
            return LIBPNG_RESULT_WRITE_IMAGE_FAILED;
        }

        const u8* image_ptr = in_image;
        for (int y = 0; y < height; ++y)
        {
            png_write_row(png_ptr, image_ptr);
            image_ptr += width_step;
        }

        /* end write */
        if (setjmp(png_jmpbuf(png_ptr)))
        {
            return LIBPNG_RESULT_END_OF_WRITE_FAILED;
        }

        png_write_end(png_ptr, NULL);
    }
    catch (const LibpngResult& result)
    {
        return result;
    }

    return LIBPNG_RESULT_NO_ERROR;
}

void wsp::libpng::FreePng(u8 **io_image)
{
    free(*io_image);
    *io_image = NULL;
}


LibpngResult wsp::libpng::LoadPng(
    u8 **o_image,
    int* o_width,
    int* o_height,
    int* o_num_channels,
    int* o_width_step,
    const char *file_path)
{
    WSP_ASSERT_NOT_NULL(o_width);
    WSP_ASSERT_NOT_NULL(o_height);

    unsigned char header[8];    // 8 is the maximum size that can be checked

                                /* open file and test for it being a png */
    wsp::common::ScopedOpenFile scoped_open_file(file_path, "rb");
    FILE *fp = scoped_open_file.GetOpenedFilePointer();
    if (!fp)
    {
        return LIBPNG_RESULT_COULD_NOT_OPEN_FILE;
    }

    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8))
    {
        return LIBPNG_RESULT_NOT_PNG_FILE;
    }

    try
    {
        /* initialize stuff */
        ScopedPngReadStructAllocator allocator;
        png_structp png_ptr = allocator.png_struct_ptr();
        png_infop info_ptr = allocator.png_info_ptr();
        if (setjmp(png_jmpbuf(png_ptr)))
        {
            return LIBPNG_RESULT_INIT_IO_FAILED;
        }

        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, 8);

        png_read_info(png_ptr, info_ptr);

        int width = png_get_image_width(png_ptr, info_ptr);
        int height = png_get_image_height(png_ptr, info_ptr);
        int num_channels = png_get_channels(png_ptr, info_ptr);
        int width_step = png_get_rowbytes(png_ptr, info_ptr);

        *o_width = width;
        *o_height = height;
        *o_num_channels = num_channels;
        *o_width_step = width_step;

        png_byte color_type = png_get_color_type(png_ptr, info_ptr);
        png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        if (bit_depth != 8)
        {
            return LIBPNG_RESULT_NOT_IMPLEMENTED;
        }

        int number_of_passes = png_set_interlace_handling(png_ptr);
        WSP_UNUSED(number_of_passes);

        png_read_update_info(png_ptr, info_ptr);


        /* read file */
        if (setjmp(png_jmpbuf(png_ptr)))
        {
            return LIBPNG_RESULT_READ_IMAGE_FAILED;
        }

        size_t image_size = height * width_step;
        *o_image = (u8*)malloc(image_size);
        if (*o_image == NULL)
        {
            png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
            return LIBPNG_RESULT_INSUFFICIENT_MEMORY;
        }

        //１ラインづつ読み込んでいく。
        for (int i = 0, offset_point = 0; i < height; ++i, offset_point += width_step)
        {
            png_read_row(png_ptr, *o_image + offset_point, NULL);
        }

        //読み込み終了処理。
        png_read_end(png_ptr, NULL); //イメージデータの後ろにあるチャンクをスキップ。
    }
    catch (const LibpngResult& result)
    {
        return result;
    }
    catch (...)
    {
        return LIBPNG_RESULT_UNKOWN_ERROR;
    }

    return LIBPNG_RESULT_NO_ERROR;
}