/**
 * @file _fnimgio.h
 * 
 * Image format input and output
 */

#ifndef _WSP_IMAGE_BASE_FN_IMG_IO_H_
#define _WSP_IMAGE_BASE_FN_IMG_IO_H_

#include "_define_image.h"

namespace wsp{ namespace img{

    // =====================================
    // Potrable Any Map I/O
    // =====================================
    WSP_DLL_EXPORT
        wsp::State LoadPNM(u8 **o_data, int *o_width, int *o_height, const char *file_path);

    WSP_DLL_EXPORT
    void FreePNM(u8 **io_data);

    WSP_DLL_EXPORT
        wsp::State SaveU24AsPPM(const u8 *in_rgb24, int width, int height, const char *file_path);

    WSP_DLL_EXPORT
        wsp::State SaveU8AsPGM(const u8 *in_u8, int width, int height, const char *file_path);

    // =====================================
    // Bitmap Map I/O
    // =====================================
    WSP_DLL_EXPORT
        wsp::State LoadBitmapImage(u8 **o_data, int *o_width, int *o_height, int *o_channel_count, const char *file_path);

    WSP_DLL_EXPORT
        void FreeBitmapImage(u8 **io_data);
}
}


#endif