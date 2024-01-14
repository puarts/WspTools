/**
 * @file tpl_fn-basic_math.h
 * 
 */

#ifndef __WSP_MATHUTIL_TPL_FN_BASIC_MATH_H__
#define __WSP_MATHUTIL_TPL_FN_BASIC_MATH_H__

#include <math.h>

#ifndef __WSP_MATHCORE_MATH_TYPES_H__
#include "_math_types.h"
#endif

namespace wsp{ namespace math{

    // decorrelation functions(the more value is lower, the more given two regions are similer)
    template<typename T> inline T GetAbsDiff(T v1, T v2){ return abs(v1-v2); }
    template<typename T> inline T GetSquaredDiff(T v1, T v2){ return (v1-v2)*(v1-v2); }
    template<typename T> inline T GetCrossDecorr(T v1, T v2){ return -v1*v2; }
    template<typename T> inline T GetNormalizedCrossDecorr(T v1, T v2, T norm1, T norm2){
        return - v1*v2 / (norm1*norm2);
    }
    template<typename T> inline T GetZeroMeanNormalizedCrossDecorr(T v1, T v2, T mean1, T mean2, T zero_mean_norm1, T zero_mean_norm2){
        return - (v1-mean1)*(v2-mean2) / (zero_mean_norm1*zero_mean_norm2);
    }
    // correlation functions(the more value is higher, the more given two regions are similer)
    template<typename T> inline T GetAbsDiffCorr(T v1, T v2){ return -abs(v1-v2); }
    template<typename T> inline T GetSquaredDiffCorr(T v1, T v2){ return -(v1-v2)*(v1-v2); }
    template<typename T> inline T GetCrossCorr(T v1, T v2){ return v1*v2; }
    template<typename T> inline T GetNormalizedCrossCorr(T v1, T v2, T norm1, T norm2){
        return v1*v2 / (norm1*norm2);
    }
    template<typename T> inline T GetZeroMeanNormalizedCrossCorr(T v1, T v2, T mean1, T mean2, T zero_mean_norm1, T zero_mean_norm2){
        return (v1-mean1)*(v2-mean2) / (zero_mean_norm1*zero_mean_norm2);
    }

    template<class T> inline T mult(const T& left, const T& right){ return left * right; }
    template<class T> inline T div (const T& left, const T& right){ return left / right; }
    template<class T> inline T sub (const T& left, const T& right){ return left - right; }
    template<class T> inline T add (const T& left, const T& right){ return left + right; }
    template<class T> inline double cot(const T& x){ return cos((double)x) / sin((double)x); }


    /** @brief  Returns -1 for negative numbers, 1 for positive, and 0 for 0. */
    template<typename T>
    int Sign( T x ){
        return x < T( 0 ) ? -1 : ( x > T( 0 ) ? 1 : 0 );
    }
    /** @brief  Computes smoothstep function from Ken Perlin's original noise implementation. */
    template<typename T>
    T SmoothStep( T v0, T v1, T v ){
        T x = (v-v0)/(v1-v0);
        if ( x > T(0) )
        {
            if ( x < T(1) )
            {
                return (T(3)-T(2)*x)*x*x;
            }
            return T(1);
        }
        return T(0);
    }


    // get max value for type T by data size
    template<typename T> inline T Max(){
        size_t size = sizeof(T);
        int m = 1;
        for(u32 i=1; i<size*ONE_BYTE; i++){
            m = (m<<1) + 1;
        }
        return T(m);
    }

    template<typename T>
    inline void Limit(T &val, T min, T max){
        if(val<min){ val=min; }
        else if(val>max){ val=max; }
    }
}}



#endif
