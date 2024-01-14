/**
 * @file fn-maya.hpp
 * @author Jun-ichi Nishikata
 * @brief OpenMaya wrapper functions.
 */

#ifndef WSP_MAYA_FUNCTION_MAYA_HPP__
#define WSP_MAYA_FUNCTION_MAYA_HPP__

#include "_define_maya.h"

class MObject;

namespace wsp{ namespace maya{
    WSP_DLL_EXPORT void CreateMesh(
        float src_vtx_list[][4], int num_vtx,
        int *src_poly_cnt_list, int num_poly,
        int *src_poly_connects, int num_poly_connects,
        MObject *o_mesh_obj
    );
}}

#endif
