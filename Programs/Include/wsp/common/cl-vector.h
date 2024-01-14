// File Name: _cl-vector.h
// Description: Base class for vector type class

#ifndef __WSP_CORE_CLASS_VECTOR_H__
#define __WSP_CORE_CLASS_VECTOR_H__

#include <omp.h>

#include "_define_commonutil.h"

namespace wsp{
    class Vector{
        union data{
            uchar    *as_uchar;
            int        *as_int;
            double    *as_double;
        };
    public:
        Vector();
        ~Vector();
    };
}