// @file cl-uniontypeimage.h
// 
// Description: Class for union type image


#ifndef __WSP_IMAGE_CLASS_UNION_TYPE_IMAGE_H__
#define __WSP_IMAGE_CLASS_UNION_TYPE_IMAGE_H__

#include <wsp/common/cl-uniontypevector.h>

namespace wsp{

    class UnionTypeImage
        : public wsp::UnionTypeVector
    {
        int width_, height_;
        int width_step_;

    public:
        // constructors
        inline UnionTypeImage(void* data, int width, int height, int num_channels, size_t size);

    };
}

// constructors
#define INIT_UNION_TYPE_IMAGE \
    width_(0)\
    ,height_(0)\
    ,width_step_(0)

inline wsp::UnionTypeImage::UnionTypeImage(void* data, int width, int height, int num_channels, size_t size)
    : wsp::UnionTypeVector(data, width*height, num_channels, size)
{
    if(data!=NULL){
        width_ = width;
        height_ = height;
        width_step_ = width * size * num_channels;
    }
}


#endif