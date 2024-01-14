/**
 * @file fn-maya_array.hpp
 * @author Jun-ichi Nishikata
 * @brief OpenMaya wrapper functions.
 */

#ifndef WSP_MAYA_FUNCTION_MAYA_ARRAY_HPP__
#define WSP_MAYA_FUNCTION_MAYA_ARRAY_HPP__

#include <maya/MIntArray.h>
#include <maya/MPointArray.h>
#include <maya/MFloatPointArray.h>

#include "_define_maya.h"



namespace wsp{ namespace maya{
    inline void PrintArray(const MIntArray &in_array);
    inline void PrintArray(const MPointArray &in_array);
    inline void PrintArray(const MFloatPointArray &in_array);
}}

inline void wsp::maya::PrintArray(const MIntArray &in_array)
{
    int len = (int)in_array.length();
    for(int i=0; i<len; ++i){
        fprintf(stdout, "%d\n", in_array[i]);
    }
}

inline void wsp::maya::PrintArray(const MPointArray &in_array)
{
    int len = (int)in_array.length();
    for(int i=0; i<len; ++i){
        fprintf(
            stdout, 
            "%f, %f, %f, %f\n"
            , in_array[i].x, in_array[i].y, in_array[i].z, in_array[i].w
        );
    }
}

inline void wsp::maya::PrintArray(const MFloatPointArray &in_array)
{
    int len = (int)in_array.length();
    for(int i=0; i<len; ++i){
        fprintf(
            stdout, 
            "%f, %f, %f, %f\n"
            , in_array[i].x, in_array[i].y, in_array[i].z, in_array[i].w
        );
    }
}

#endif
