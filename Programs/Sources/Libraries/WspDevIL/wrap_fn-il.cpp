/**
 * @file wrap_fn-il.c
 * @author 
 */

#include "wrap_fn-il.h"
#include "image_result_converter.h"

#include <wsp/common/fn-io.h>
#include <wsp/common/fn-debug.h>
#include <IL/il.h>

#include <stdlib.h>

void _Flip2dVertical(u8 *io_ary, int width, int height, int spp)
{
    int x, y, i, size = width*height*spp*sizeof(u8);
    u8 *tmp_ary = (u8*)malloc(size);
    memcpy(tmp_ary, io_ary, size);

    for (y = 0; y < height; ++y)
    {
        for (x = 0; x < width; ++x)
        {
            for (i = 0; i < spp; ++i)
            {
                io_ary[(y*width + x)*spp + i] = tmp_ary[((height - y - 1)*width + x)*spp + i];
            }
        }
    }
    free(tmp_ary);
}

char* WSP_IlGetErrorMessage(ILenum errCode)
{
    switch(errCode)
    {
    case IL_NO_ERROR             : return "IL_NO_ERROR            ";
    case IL_INVALID_ENUM         : return "IL_INVALID_ENUM        ";
    case IL_OUT_OF_MEMORY        : return "IL_OUT_OF_MEMORY       ";
    case IL_FORMAT_NOT_SUPPORTED : return "IL_FORMAT_NOT_SUPPORTED";
    case IL_INTERNAL_ERROR       : return "IL_INTERNAL_ERROR      ";
    case IL_INVALID_VALUE        : return "IL_INVALID_VALUE       ";
    case IL_ILLEGAL_OPERATION    : return "IL_ILLEGAL_OPERATION: There is currently no image bound. Use ilGenImages and ilBindImage before calling this function.";
    case IL_ILLEGAL_FILE_VALUE   : return "IL_ILLEGAL_FILE_VALUE ";
    case IL_INVALID_FILE_HEADER  : return "IL_INVALID_FILE_HEADER";
    case IL_INVALID_PARAM        : return "IL_INVALID_PARAM: FileName was not valid. It was most likely NULL.";
    case IL_COULD_NOT_OPEN_FILE  : return "IL_COULD_NOT_OPEN_FILE: The file pointed to by FileName could not be opened for writing.";
    case IL_INVALID_EXTENSION    : return "IL_INVALID_EXTENSION: The file could not be saved based on FileName's extension.";
    case IL_FILE_ALREADY_EXISTS  : return "IL_FILE_ALREADY_EXISTS";
    case IL_OUT_FORMAT_SAME      : return "IL_OUT_FORMAT_SAME    ";
    case IL_STACK_OVERFLOW       : return "IL_STACK_OVERFLOW     ";
    case IL_STACK_UNDERFLOW      : return "IL_STACK_UNDERFLOW    ";
    case IL_INVALID_CONVERSION   : return "IL_INVALID_CONVERSION ";
    case IL_BAD_DIMENSIONS       : return "IL_BAD_DIMENSIONS     ";
    case IL_FILE_READ_ERROR      : return "IL_FILE_READ_ERROR or IL_FILE_WRITE_ERROR";
    default: return "Unknown Error";
    }
}


WSP_Result WSP_IlLoadImage(
    u8 **o_img, 
    int *o_width, int *o_height, int *o_num_channels, 
    const char *filename
)
{
    int spp;
    int size;
    int w, h, bytes_per_pixel;
    ILint format;
    ILboolean success;
    ILuint il_image_id = 0;
    ILubyte *bytes;
    ILenum il_err;
    ILint image_origin;

    ilInit();

    ilGenImages(1, &il_image_id);
    ilBindImage(il_image_id);

    success = ilLoadImage((const ILstring)filename) ;
    if(!success){
        il_err = ilGetError();
        WSP_COMMON_ERROR_LOG("DevIL Error %d: %s\n", il_err, ilGetString(il_err));
        return WSP_MakeIlResult( il_err );
    }

    size = ilGetInteger( IL_IMAGE_SIZE_OF_DATA );
    w =ilGetInteger(IL_IMAGE_WIDTH);
    h =ilGetInteger(IL_IMAGE_HEIGHT);
    bytes_per_pixel = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);

    format = ilGetInteger(IL_IMAGE_FORMAT);
    switch(format)
    {
    case IL_RGB:
        WSP_COMMON_LOG("image format = RGB\n");
        success = true;
        spp=3;
        break;
    case IL_BGR:
        WSP_COMMON_LOG("image format = BGR\n");
        success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
        spp = 3;
        break;
    case IL_RGBA:
        WSP_COMMON_LOG("image format = RGBA\n");
        success = true;
        spp = 4;
        break;
    case IL_BGRA:
        WSP_COMMON_LOG("image format = BGRA\n");
        success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        spp=4;
        break;
    case IL_COLOR_INDEX:
        WSP_COMMON_LOG("image format = COLOR INDEX\n");
        spp = 1;
        break;
    case IL_LUMINANCE:
        WSP_COMMON_LOG("image format = LUMINANCE\n");
        spp=1;
        break;
    default:
        WSP_FATAL("Not still implemetend format %d", format);
    }
    if(!success){
        il_err = ilGetError();
        WSP_COMMON_ERROR_LOG("DevIL Error %d: %s\n", il_err, ilGetString(il_err));
        return WSP_MakeIlResult( il_err );
    }

    bytes = ilGetData();
    image_origin = ilGetInteger(IL_IMAGE_ORIGIN);
    if (image_origin == IL_ORIGIN_LOWER_LEFT)
    {
        _Flip2dVertical(bytes, w, h, spp);
    }

    *o_img = (u8*)malloc(size);
    if (*o_img == NULL)
    {
        return WSP_ResultIlOutOfMemory();
    }

    *o_width = (int)w;
    *o_height = (int)h;
    *o_num_channels = spp;
    memcpy(*o_img, bytes, size);

    ilDeleteImages(1, &il_image_id);

    WSP_COMMON_LOG(
        "%s successfully loaded\n"
        "    width=%d, height=%d, data size=%d\n"
        "    bytes per pixel=%d, number of channels=%d,"
        "    origin=%s\n"
        , filename
        , *o_width, *o_height, size
        , bytes_per_pixel, *o_num_channels
        , image_origin == IL_ORIGIN_LOWER_LEFT ? "lower-left" : "upper-left");

    return WSP_ResultSuccess();
}

void WSP_IlFreeImage(u8 **io_image)
{
    free(*io_image);
    *io_image = NULL;
}

WSP_Result WSP_IlLoadImageAsU8WithAlpha(
    u8 **o_img, u8 **o_alpha, 
    int *o_width, int *o_height, int *o_num_channels, 
    const char *filename
)
{
    u8 *tmp_img = NULL;
    WSP_Result result;
    int length;
    int size;
    int cnt;
    u8 *dst_ptr, *src_ptr, *end_ptr;

    result = WSP_IlLoadImage(&tmp_img, o_width, o_height, o_num_channels, filename);
    if (result.level != WSP_RESULT_LEVEL_SUCCESS)
    {
        return result;
    }

    length = (*o_width) * (*o_height);
    size = length * (*o_num_channels);

    if (*o_num_channels == 4)
    {
        *o_img = (u8*)malloc( length*3 );
        if(o_alpha != NULL)
        {
            *o_alpha = (u8*)malloc(length*sizeof(u8));
            dst_ptr = *o_alpha;
            end_ptr = *o_alpha + length;
            src_ptr = tmp_img + 3;
            for (; dst_ptr != end_ptr; ++dst_ptr, src_ptr += 4)
            {
                *dst_ptr = *src_ptr;
            }
        }

        *o_num_channels = 3;
        cnt = 0;
        dst_ptr = *o_img;
        end_ptr = *o_img + size;
        src_ptr = tmp_img;
        for (; dst_ptr != end_ptr; ++dst_ptr, ++src_ptr)
        {
            *dst_ptr = *src_ptr;
            ++cnt;
            if (cnt == *o_num_channels)
            {
                ++src_ptr;
                cnt = 0;
            }
        }
    }
    else
    {
        *o_img = (u8*)malloc( size );
        memcpy( *o_img, tmp_img, size );
    }

    WSP_IlFreeImage( &tmp_img );

    return WSP_ResultSuccess();
}

WSP_Result WSP_IlSaveImage(
    const u8 *in_image,
    int width, int height,
    int num_channels,
    const char *filename)
{
    const ILint origin = IL_ORIGIN_UPPER_LEFT;
    ILenum il_err;
    ILboolean success;
    ILuint img = 0;

    ilInit();

    ilHint(IL_COMPRESSION_HINT, IL_USE_COMPRESSION);
    ilSetInteger(IL_JPG_QUALITY, 80);
    ilSetInteger(IL_ORIGIN_MODE, origin);

    ilGenImages(1, &img);
    ilBindImage(img);

    if (num_channels == 1)
    {
        success = ilTexImage(
            width, height, 1, 1,
            IL_LUMINANCE, IL_UNSIGNED_BYTE,
            (void*)in_image
            );
    }
    else
    {
        success = ilTexImage(
            width, height, 1, num_channels,
            IL_RGBA, IL_UNSIGNED_BYTE,
            (void*)in_image
            );
    }
    if (!success)
    {
        il_err = ilGetError();
        WSP_COMMON_ERROR_LOG("DevIL Error %d: %s\n", il_err, ilGetString(il_err));
        return WSP_MakeIlResult(il_err);
    }

    ilEnable(IL_FILE_OVERWRITE);
    success = ilSaveImage((const ILstring)filename);
    if (!success)
    {
        il_err = ilGetError();
        WSP_COMMON_ERROR_LOG("DevIL Error %d: %s\n", il_err, ilGetString(il_err));
        return WSP_MakeIlResult(il_err);
    }

    ilDeleteImages(1, &img);

    WSP_COMMON_LOG(
        "%s successfully saved\n"
        "    width=%d, height=%d, data size=%d\n"
        "    number of channels=%d, origin=%s\n"
        , filename
        , width, height, width*height*num_channels
        , num_channels
        , origin ? "lower-left" : "upper-left"
        );

    return WSP_ResultSuccess();
}
