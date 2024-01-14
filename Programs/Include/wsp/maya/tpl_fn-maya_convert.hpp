/**
 * @file tpl_fn-maya_convert.hpp
 * @author Jun-ichi Nishikata
 * @brief Conversion between OpenMaya and other libraries.
 */

#ifndef WSP_MAYA_TEMPLATE_FUNCTION_MAYA_CONVERT_HPP__
#define WSP_MAYA_TEMPLATE_FUNCTION_MAYA_CONVERT_HPP__

#include "_define_maya.h"

namespace wsp{
    template<typename T, int MemAllocStep> class Vector;
}
class MIntArray;
class MPointArray;
class MFloatPointArray;

namespace wsp{ namespace maya{

    /* Conversion */
    template<typename T, int MemAllocStep>
    void Convert(const MIntArray &in_marray, wsp::Vector<T, MemAllocStep> &o_warray);

    template<typename T, int MemAllocStep>
    void Convert(const MPointArray &in_marray, wsp::Vector<T, MemAllocStep> &o_warray);

    template<typename T, int MemAllocStep>
    void Convert(const MFloatPointArray &in_marray, wsp::Vector<T, MemAllocStep> &o_warray);

    template<typename T, int MemAllocStep>
    void Convert(const wsp::Vector<T, MemAllocStep> &in_warray, MIntArray &o_marray);

    template<typename T, int MemAllocStep>
    void Convert(const wsp::Vector<T, MemAllocStep> &in_warray, MPointArray &o_marray);

    template<typename T, int MemAllocStep>
    void Convert(const wsp::Vector<T, MemAllocStep> &in_warray, MFloatPointArray &o_marray);

    template<typename T> 
    void Convert(const T *in_ary, MIntArray &o_marray, int length);

    template<typename T> 
    void Convert(const T *in_ary, MFloatPointArray &o_marray, int length, int num_chennels);

    /* I/O */
    
}}

#include "tpl_fn-maya_convert.inl"

#endif
