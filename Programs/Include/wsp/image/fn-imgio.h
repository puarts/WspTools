/**
 * @file fn-imgio.h
 * 
 * Image format input and output
 */

#ifndef __WSP_IMAGECORE_FN_IMGIO_H__
#define __WSP_IMAGECORE_FN_IMGIO_H__

#include "_define_imagecore.h"
#include "_image_types.h"

#ifdef __cplusplus
extern "C"{
#endif

/* Potrable Any Map I/O */
WSP_DLL_EXPORT WSP_ImageState WSP_LoadPNM(
    u8 *o_dst, int *o_width, int *o_height, const char *filename);

WSP_DLL_EXPORT WSP_ImageState WSP_SaveU24AsPPM(
    const u8 *in_rgb24, int width, int height, const char *filename);

WSP_DLL_EXPORT WSP_ImageState WSP_SaveU8AsPGM(
    const u8 *in_u8, int width, int height, const char *filename);

#ifdef __cplusplus
}
#endif


#endif