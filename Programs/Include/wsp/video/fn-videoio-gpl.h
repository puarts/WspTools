/**
 * @file fn-video-gpl.h
 * @author Junichi Nishikata
 * Video I/O functions using GPL library like FFmpeg.
 */

#ifndef _WSP_VIDEO_FN_VIDEOIO_GPL_H_
#define _WSP_VIDEO_FN_VIDEOIO_GPL_H_

#include "_define_video.h"

namespace wsp{ namespace video{
    WSP_DLL_EXPORT wsp::State avLoadVideoAsU8(
        uchar **o_rgba, 
        int &o_width, 
        int &o_height, 
        int &o_num_channels, 
        const char *filename
    );

}}

#endif