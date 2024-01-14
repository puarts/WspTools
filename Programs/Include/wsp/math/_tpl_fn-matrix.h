/**
 * @file _tpl_fn-matrix.h
 * 
 */

#ifndef _WSP_MATH_BASE_TEMPLATE_FUNCTION_MATRIX_H
#define _WSP_MATH_BASE_TEMPLATE_FUNCTION_MATRIX_H

#include "_define_mathutil.h"

namespace wsp{ namespace math{
    // Matrix Processing ---------------------------------------
    template<typename T> void Mult(T *m_l, T *m_r, T *m_res, int row, int col);

    template<typename T> inline void Transpose(T *io_img, int row_size, int col_size);
    template<typename T> inline void SwapQuadrantHor(const T *in_src, T *o_dst, int row_size, int col_size);
    template<typename T> inline void SwapQuadrantVer(const T *in_src, T *o_dst, int row_size, int col_size);
    template<typename T> inline void SwapQuadrantHV(T *tmp_img, T *io_img, int row_size, int col_size);

    template<typename T> inline void SwapComplexQuadrantHor(const T *in_src, T *o_dst, int row_size, int col_size);
    template<typename T> inline void SwapComplexQuadrantVer(const T *in_src, T *o_dst, int row_size, int col_size);
    template<typename T> inline void SwapComplexQuadrantHV(T *tmp_img, T *io_img, int row_size, int col_size);
}}

#include "_tpl_fn-matrix.inl"

#endif