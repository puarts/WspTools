

#ifndef LOAD_LIBAV_H_
#define LOAD_LIBAV_H_

#ifdef HAVE_AV_CONFIG_H
    #undef HAVE_AV_CONFIG_H
#endif

#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avfilter.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "swscale.lib")

#endif
