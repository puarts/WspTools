#pragma once

#ifndef __WSP_IMAGEUTIL_DEFINE_IMAGEUTIL_H__
#include <wsp/common/_define_commoncore.h>
#endif

#include <wsp/common/common_types.h>

namespace wsp{ namespace libpng{
    enum LibpngResult {
        LIBPNG_RESULT_NO_ERROR,
        LIBPNG_RESULT_COULD_NOT_OPEN_FILE,
        LIBPNG_RESULT_NOT_PNG_FILE,
        LIBPNG_RESULT_CREATE_PNG_STRUCT_FAILED,
        LIBPNG_RESULT_CREATE_INFO_STRUCT_FAILED,
        LIBPNG_RESULT_INIT_IO_FAILED,
        LIBPNG_RESULT_READ_IMAGE_FAILED,
        LIBPNG_RESULT_WRITE_HEADER_FAILED,
        LIBPNG_RESULT_WRITE_IMAGE_FAILED,
        LIBPNG_RESULT_END_OF_WRITE_FAILED,
        LIBPNG_RESULT_NOT_IMPLEMENTED,
        LIBPNG_RESULT_INSUFFICIENT_MEMORY,
        LIBPNG_RESULT_UNKOWN_ERROR
    };

    WSP_DLL_EXPORT LibpngResult LoadPng(
        u8 **o_image, 
        int* o_width, 
        int* o_height, 
        int* o_num_channels, 
        int* o_width_step,
        const char *file_path);

    WSP_DLL_EXPORT LibpngResult SavePng(
        const u8 *in_image,
        int width,
        int height,
        int num_channels,
        int width_step,
        const char* file_path);

    WSP_DLL_EXPORT void FreePng(u8 **io_image);
}}

