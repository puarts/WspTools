/**
 * @file tpl_fn-maya.hpp
 * @author Jun-ichi Nishikata
 * @brief OpenMaya wrapper template functions.
 */

#ifndef WSP_MAYA_TEMPLATE_FUNCTION_MAYA_HPP__
#define WSP_MAYA_TEMPLATE_FUNCTION_MAYA_HPP__

#include "_define_maya.h"

class MObject;

namespace wsp{ namespace maya{
    template<typename T> void CreateLocator(const T in_pos[3]);
}}

#include "tpl_fn-maya.inl"

#endif
