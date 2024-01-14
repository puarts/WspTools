/**
 * @file cl-transform.h
 * 
 */

#ifndef _WSP_GRAPHIC_FUNCTION_TRANSFORM_H_
#define _WSP_GRAPHIC_FUNCTION_TRANSFORM_H_

#include "_define_graphic.h"
#include "tpl_cl-transform_matrix.h"

namespace wsp{
    class WSP_DLL_EXPORT Transform
    {
    private:
        wsp::graphic::TransformMatrix<float> transform_mat_;

    public:
    };
}

#endif