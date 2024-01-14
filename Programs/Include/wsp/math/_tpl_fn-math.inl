/**
 * @file _tpl_fn-math.inl
 * 
 */

#ifndef WSP_MATH_TEMPLATE_FUNCTION_MATH_INL__
#define WSP_MATH_TEMPLATE_FUNCTION_MATH_INL__

#include "_tpl_fn-math.h"
#include <wsp/common/fn-debug.h>
#include <wsp/common/_tpl_fn-array.hpp>

#ifndef M_PI
#define _USE_MATH_DEFINES
#endif
#include <math.h>

template<typename _Type>
void wsp::math::GetNearestPowerOfTwo(_Type* o_power_of_two_value, s32* o_exponent, _Type input_value)
{
    if (input_value == 1)
    {
        *o_power_of_two_value = 1;
        *o_exponent = 0;
        return;
    }

    *o_power_of_two_value = 1;
    *o_exponent = 1;
    for (_Type value = 1; (value *= 2) < input_value; ++(*o_exponent)){}
    for (_Type exp = 0; exp < (*o_exponent); ++exp, *o_power_of_two_value *= 2){}
}

template<typename _Type>
_Type wsp::math::GetNearestPowerOfTwo(_Type input_value)
{
    _Type power_of_two_value;
    s32 exponent;
    wsp::math::GetNearestPowerOfTwo(&power_of_two_value, &exponent, input_value);
    return power_of_two_value;
}

template<typename _Type>
bool wsp::math::IsPowerOfTwo(_Type value)
{
    return value == wsp::math::GetNearestPowerOfTwo(value);
}

template<typename _Type>
s32 wsp::math::GetExponentOfNearestPowerOfTwo(_Type value)
{
    _Type power_of_two_value;
    s32 exponent;
    wsp::math::GetNearestPowerOfTwo(&power_of_two_value, &exponent, value);
    return exponent;
}

template<typename T> 
inline void wsp::math::SetVector3(T x, T y, T z, T o_vec[3])
{
    o_vec[0] = x;
    o_vec[1] = y;
    o_vec[2] = z;
}

template<typename T>
inline void wsp::math::NormalizeLengthVec3(T *io_x, T *io_y, T *io_z)
{
    T mult = 1.0f/sqrt((*io_x)*(*io_x) + (*io_y)*(*io_y) + (*io_z)*(*io_z));
    (*io_x) *= mult;
    (*io_y) *= mult;
    (*io_z) *= mult;
}

template<typename T>
inline void  wsp::math::NormalizeLengthVec3(T io_vec[3])
{
    wsp::math::NormalizeLengthVec3<T>(&io_vec[0], &io_vec[1], &io_vec[2]);
}

template<typename T> inline T wsp::math::GetDotProduct( const T in_vec_l[3], const T in_vec_r[3] )
{
    return in_vec_l[0]*in_vec_r[0] + in_vec_l[1]*in_vec_r[1] + in_vec_l[2]*in_vec_r[2];
}

template<typename T>
void  wsp::math::GetCrossProduct(const T in_vec_l[3], const T in_vec_r[3], T o_vec[3])
{
    T x, y, z;
    x = (in_vec_l)[1]*(in_vec_r)[2] - (in_vec_l)[2]*(in_vec_r)[1];
    y = (in_vec_l)[2]*(in_vec_r)[0] - (in_vec_l)[0]*(in_vec_r)[2];
    z = (in_vec_l)[0]*(in_vec_r)[1] - (in_vec_l)[1]*(in_vec_r)[0];
    (o_vec)[0] = x;
    (o_vec)[1] = y;
    (o_vec)[2] = z;
}

template<typename T>
void wsp::math::GetCrossProduct(
    T* o_vec_x,
    T* o_vec_y,
    T* o_vec_z,
    T in_vec_l_x,
    T in_vec_l_y,
    T in_vec_l_z,
    T in_vec_r_x,
    T in_vec_r_y,
    T in_vec_r_z
    )
{
    T x, y, z;
    x = in_vec_l_y * in_vec_r_z - in_vec_l_z * in_vec_r_y;
    y = in_vec_l_z * in_vec_r_x - in_vec_l_x * in_vec_r_z;
    z = in_vec_l_x * in_vec_r_y - in_vec_l_y * in_vec_r_x;
    *o_vec_x = x;
    *o_vec_y = y;
    *o_vec_z = z;
}

template<typename T>
inline void  wsp::math::OrthoNormalize(
    T first_prior_axis[3], 
    T second_prior_axis[3], 
    T third_prior_axis[3]
)
{
    wsp::math::NormalizeLengthVec3(first_prior_axis);
    wsp::math::NormalizeLengthVec3(second_prior_axis);

    wsp::math::GetCrossProduct(first_prior_axis, second_prior_axis, third_prior_axis);
    wsp::math::NormalizeLengthVec3(third_prior_axis);

    wsp::math::GetCrossProduct(third_prior_axis, first_prior_axis, second_prior_axis);
    wsp::math::NormalizeLengthVec3(second_prior_axis);
}

// Correlation -------------------------------------------------
template<typename T>
double wsp::math::GetSumOfCorr(
    const T *in_main, 
    const T *in_sub, 
    s32 length,
    double (*eval_func)(double, double)
){
    const T *main_ptr = in_main;
    const T *sub_ptr = in_sub;
    const T *end_ptr = main_ptr + length;
    double sum_corr = 0.0;
    for(; main_ptr!=end_ptr; ++main_ptr, ++sub_ptr){
        sum_corr += eval_func(static_cast<double>(*main_ptr), static_cast<double>(*sub_ptr));
    }
    return sum_corr;
}


template<typename T, typename U> 
U wsp::math::GetSAD(const T *in_vec1, const T *in_vec2, u32 length)
{
    if(length==0){ return static_cast<U>(0); }
    T corr = abs(in_vec1[0] - in_vec2[0]);
    for(u32 i=1; i<length; i++){
        corr += abs(in_vec1[i] - in_vec2[i]);
    }
    return static_cast<U>(corr);
}

template<typename T, typename U> 
U wsp::math::GetSSD(const T *in_vec1, const T *in_vec2, u32 length)
{
    if(length==0){ return static_cast<U>(0); }
    T corr = (in_vec1[0] - in_vec2[0])*(in_vec1[0] - in_vec2[0]);
    for(u32 i=1; i<length; i++){
        corr += (in_vec1[i] - in_vec2[i])*(in_vec1[i] - in_vec2[i]);
    }
    return static_cast<U>(corr);
}

template<typename T, typename U> 
U wsp::math::GetCorrelationCoefficient(const T *in_vec1, const T *in_vec2, u32 length)
{
    T av1 = wsp::GetAverage(in_vec1, length);
    T av2 = wsp::GetAverage(in_vec2, length);

    T cross_corr = (in_vec1[0] - av1)*(in_vec2[0] - av2);
    T var1 = (in_vec1[0] - av1)*(in_vec1[0] - av1);
    T var2 = (in_vec2[0] - av2)*(in_vec2[0] - av2);
    for(u32 i=1; i<length; i++){
        cross_corr += (in_vec1[i] - av1)*(in_vec2[i] - av2);
        var1 += (in_vec1[i] - av1)*(in_vec1[i] - av1);
        var2 += (in_vec2[i] - av2)*(in_vec2[i] - av2);
    }
    return static_cast<U>(cross_corr) / static_cast<U>(var1*var2);
}

template<typename T>
bool wsp::math::GetInvertMatrix4x4(const T m[16], T invOut[16])
{
    T inv[16], det;
    int i;

    inv[0] = m[5] * m[10] * m[15] -
        m[5] * m[11] * m[14] -
        m[9] * m[6] * m[15] +
        m[9] * m[7] * m[14] +
        m[13] * m[6] * m[11] -
        m[13] * m[7] * m[10];

    inv[4] = -m[4] * m[10] * m[15] +
        m[4] * m[11] * m[14] +
        m[8] * m[6] * m[15] -
        m[8] * m[7] * m[14] -
        m[12] * m[6] * m[11] +
        m[12] * m[7] * m[10];

    inv[8] = m[4] * m[9] * m[15] -
        m[4] * m[11] * m[13] -
        m[8] * m[5] * m[15] +
        m[8] * m[7] * m[13] +
        m[12] * m[5] * m[11] -
        m[12] * m[7] * m[9];

    inv[12] = -m[4] * m[9] * m[14] +
        m[4] * m[10] * m[13] +
        m[8] * m[5] * m[14] -
        m[8] * m[6] * m[13] -
        m[12] * m[5] * m[10] +
        m[12] * m[6] * m[9];

    inv[1] = -m[1] * m[10] * m[15] +
        m[1] * m[11] * m[14] +
        m[9] * m[2] * m[15] -
        m[9] * m[3] * m[14] -
        m[13] * m[2] * m[11] +
        m[13] * m[3] * m[10];

    inv[5] = m[0] * m[10] * m[15] -
        m[0] * m[11] * m[14] -
        m[8] * m[2] * m[15] +
        m[8] * m[3] * m[14] +
        m[12] * m[2] * m[11] -
        m[12] * m[3] * m[10];

    inv[9] = -m[0] * m[9] * m[15] +
        m[0] * m[11] * m[13] +
        m[8] * m[1] * m[15] -
        m[8] * m[3] * m[13] -
        m[12] * m[1] * m[11] +
        m[12] * m[3] * m[9];

    inv[13] = m[0] * m[9] * m[14] -
        m[0] * m[10] * m[13] -
        m[8] * m[1] * m[14] +
        m[8] * m[2] * m[13] +
        m[12] * m[1] * m[10] -
        m[12] * m[2] * m[9];

    inv[2] = m[1] * m[6] * m[15] -
        m[1] * m[7] * m[14] -
        m[5] * m[2] * m[15] +
        m[5] * m[3] * m[14] +
        m[13] * m[2] * m[7] -
        m[13] * m[3] * m[6];

    inv[6] = -m[0] * m[6] * m[15] +
        m[0] * m[7] * m[14] +
        m[4] * m[2] * m[15] -
        m[4] * m[3] * m[14] -
        m[12] * m[2] * m[7] +
        m[12] * m[3] * m[6];

    inv[10] = m[0] * m[5] * m[15] -
        m[0] * m[7] * m[13] -
        m[4] * m[1] * m[15] +
        m[4] * m[3] * m[13] +
        m[12] * m[1] * m[7] -
        m[12] * m[3] * m[5];

    inv[14] = -m[0] * m[5] * m[14] +
        m[0] * m[6] * m[13] +
        m[4] * m[1] * m[14] -
        m[4] * m[2] * m[13] -
        m[12] * m[1] * m[6] +
        m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] +
        m[1] * m[7] * m[10] +
        m[5] * m[2] * m[11] -
        m[5] * m[3] * m[10] -
        m[9] * m[2] * m[7] +
        m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] -
        m[0] * m[7] * m[10] -
        m[4] * m[2] * m[11] +
        m[4] * m[3] * m[10] +
        m[8] * m[2] * m[7] -
        m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] +
        m[0] * m[7] * m[9] +
        m[4] * m[1] * m[11] -
        m[4] * m[3] * m[9] -
        m[8] * m[1] * m[7] +
        m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] -
        m[0] * m[6] * m[9] -
        m[4] * m[1] * m[10] +
        m[4] * m[2] * m[9] +
        m[8] * m[1] * m[6] -
        m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return false;

    det = static_cast<T>(1.0 / det);

    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;

    return true;
}

#endif
