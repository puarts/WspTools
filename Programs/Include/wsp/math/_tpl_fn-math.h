/**
 * @file _tpl_fn-math.h
 * 
 */

#ifndef WSP_MATH_TEMPLATE_FUNCTION_MATH_H__
#define WSP_MATH_TEMPLATE_FUNCTION_MATH_H__

#include "_define_mathutil.h"

namespace wsp{ namespace math{
    /* ------------------------------------------
            Basic
       ------------------------------------------*/
    template<typename _Type>
    void GetNearestPowerOfTwo(
        _Type* o_power_of_two_value, 
        s32* o_exponent, 
        _Type input_value);

    template<typename _Type>
    _Type GetNearestPowerOfTwo(_Type input_value);

    template<typename _Type>
    bool IsPowerOfTwo(_Type value);

    template<typename _Type>
    s32 GetExponentOfNearestPowerOfTwo(_Type value);

    template<typename T> 
    inline void SetVector3(T x, T y, T z, T o_vec[3]);
    
    template<typename T> 
    inline void NormalizeLengthVec3(T *io_x, T *io_y, T *io_z);
    
    template<typename T> 
    inline void NormalizeLengthVec3(T io_vec[3]);
    
    template<typename T> 
    inline T GetDotProduct(
        const T in_vec_l[3], 
        const T in_vec_r[3]
    );

    template<typename T> inline void GetCrossProduct(
        const T in_vec_l[3], 
        const T in_vec_r[3], 
        T o_vec[3]
    );

    template<typename T> inline void GetCrossProduct(
        T* o_vec_x,
        T* o_vec_y,
        T* o_vec_z,
        T in_vec_l_x,
        T in_vec_l_y,
        T in_vec_l_z,
        T in_vec_r_x,
        T in_vec_r_y,
        T in_vec_r_z
        );

    template<typename T> inline void OrthoNormalize(
        T first_prior_axis[3], 
        T second_prior_axis[3], 
        T third_prior_axis[3]
    );

    //template<typename T> double GetCorrelationCoefficient(T v1, T v2, T average1, T average2){ return (); }

    template<typename T>
    double GetSumOfCorr(
            const T *in_main, 
            const T *in_sub, 
            int length,
            double (*eval_func)(double, double)
        );

    template<typename T, typename U> U GetSAD(const T *in_vec1, const T *in_vec2, u32 length);
    template<typename T, typename U> U GetSSD(const T *in_vec1, const T *in_vec2, u32 length);
    template<typename T, typename U> U GetCorrelationCoefficient(const T *in_vec1, const T *in_vec2, u32 length);

    template<typename T>
    bool GetInvertMatrix4x4(const T in_mat4x4[16], T o_inverse4x4[16]);
}}

#include "_tpl_fn-math.inl"


#endif