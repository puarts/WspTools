// @file cl-imagehandler.cpp
// 


#include "cl-imagehandler.h"
#ifndef __WSP_COMMONUTIL_FN_UTIL_H__
#include <wsp/common/fn-util.h>
#endif

wsp::DebugMode wsp::ImageHandler::dbgmode = WSP_DEBUGMODE_OFF ;

// Constructor, Destructor --------------------------------------
#define INIT_WSP_IMAGE_HANDLER image_8u_(NULL), num_image_8u_(0)

inline void wsp::ImageHandler::Init(){
    strcpy(dest_dir_,".");
}
wsp::ImageHandler::ImageHandler()
    :INIT_WSP_IMAGE_HANDLER
{
    image_8u_ = WSP_NEW wsp::Image<uchar>[1];
    num_image_8u_ = 1;
    image_64f_ = WSP_NEW wsp::Image<double>[1];
    num_image_64f_ = 1;
    Init();
}
wsp::ImageHandler::ImageHandler(int num_8u, int num_32f)
    :INIT_WSP_IMAGE_HANDLER
{
    Init(); 
    image_8u_ = WSP_NEW wsp::Image<uchar>[num_8u];
    num_image_8u_ = num_8u;
    image_64f_ = WSP_NEW wsp::Image<double>[num_32f];
    num_image_64f_ = num_32f;
}
wsp::ImageHandler::ImageHandler(const char *filename_col, const char *filename_depth)
    :INIT_WSP_IMAGE_HANDLER
{
    Init(); 
    char ext[MAX_LENGTH_OF_NAME];
    image_8u_ = WSP_NEW wsp::Image<uchar>[1];
    num_image_8u_=1;

    wsp::GetExtension(ext, filename_col);
    wsp::GetExtension(ext, filename_col);
}
wsp::ImageHandler::~ImageHandler()
{
    delete[] image_8u_;
}

void wsp::ImageHandler::InitImage8u(int num_img)
{
    if(num_img==0){ return; }
    delete[] image_8u_;
    image_8u_ = WSP_NEW wsp::Image<uchar>[num_img];
    num_image_8u_ = num_img;
}

void wsp::ImageHandler::InitImage32f(int num_img)
{
    if(num_img==0){ return; }
    delete[] image_64f_;
    image_64f_ = WSP_NEW wsp::Image<double>[num_img];
    num_image_64f_ = num_img;
}