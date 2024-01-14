/**
 * @file fn-basic_math.h
 * 
 */


#ifndef __WSP_MATHUTIL_FUNCTION_BASIC_MATH_H__
#define __WSP_MATHUTIL_FUNCTION_BASIC_MATH_H__

#include <stdlib.h>
#include <math.h>

#ifndef __WSP_MATHCORE_MATH_TYPES_H__
#include "_math_types.h"
#endif

namespace wsp{ namespace math{

    inline wsp::math::FittingFunction DecideFittingFuncByCorrFunc(wsp::math::CorrelationFunction corr_func);


    // decorrelation functions(the more value is lower, the more given two regions are similer)


    inline bool IsPowerOfTwo(int value)
    {
        int n = 1;
        while(n<value)
        { 
            n = n << 1; 
        }

        if(n==value)
        { 
            return true; 
        }

        return false;
    }

    inline int GetPowerOfTwo(int value)
    {
        int n=1;
        while(n<value)
        { 
            n = n<<1; 
        }

        return n;
    }

    inline int GetExponentOfPowerOfTwo(int value)
    {
        int n=1, exp=0;
        while(n<value)
        { 
            n = n<<1;
            ++exp;
        }

        return exp;
    }



    // basic inline functions
    inline double Rand(double min, double max)
    {
        return (rand()/static_cast<double>(RAND_MAX))*(max - min) + min;
    }

    inline double Round(double d){ return floor(d + 0.5); }
    inline double GetGaussianWeight(double distance, double variance){
        return exp(-(distance*distance) / (2*variance));
    }
    inline double GetGaussianWeightNormalized(double distance, double deviation){
        return GetGaussianWeight(distance, deviation) / (2.0*wsp::math::kPI*deviation*deviation);
    }
}}


inline wsp::math::FittingFunction wsp::math::DecideFittingFuncByCorrFunc(wsp::math::CorrelationFunction corr_func){
    switch(corr_func){
    case WSP_CORRELATION_FUNCTION_SAD:
        return WSP_FITTING_FUNCTION_EQUIANGULAR_LINE;
    default:
        return WSP_FITTING_FUNCTION_PARABOLA;
    }
}


#endif
