/**
 * @file fn-graphic_proc.inl
 * 
 */

#ifndef __WSP_GRAPHIC_FUNCTION_GRAPHIC_PROC_INL__
#define __WSP_GRAPHIC_FUNCTION_GRAPHIC_PROC_INL__

#include "fn-graphic_proc.h"
#include "st-depth_to_mesh_param.h"
#include "st-persp_view_param.h"

inline wsp::State wsp::graphic::CreateTriMeshFromDepthMap(
    wsp::Vector<wsp::Point<float>> &o_vtxArray, 
    wsp::Vector<int> &o_vtxCountOnFace, 
    wsp::Vector<int> &o_faceConnects, 
    wsp::Vector<int> *o_edge_indices,
    wsp::Vector<wsp::Vector3<float>> *o_normals,
    wsp::Vector<float> *o_uvs,
    const char *filename, 
    const wsp::graphic::PerspViewParam &view_param,
    const wsp::graphic::DepthToMeshParam &d2m_param
){
    return wsp::graphic::CreateTriMeshFromDepthMap(
        o_vtxArray, 
        o_vtxCountOnFace, 
        o_faceConnects, 
        o_edge_indices,
        o_normals,
        o_uvs,
        filename, 
        view_param,
        d2m_param.face_depth_thre, 
        d2m_param.sampling_step, 
        d2m_param.alpha_thre, 
        d2m_param.scale,
        d2m_param.invert_depth
    );
}

#endif