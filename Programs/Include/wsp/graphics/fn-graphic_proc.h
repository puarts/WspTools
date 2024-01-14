#pragma once

#include "_define_graphic.h"

#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#include <wsp/common/tpl_cl-vector.h>
#endif
#include <wsp/common/tpl_cl-vector2.h>
#include <wsp/common/tpl_cl-vector3.h>
#include <wsp/math/tpl_fn-plane.hpp>


namespace wsp{ namespace graphic{

    WSP_DLL_EXPORT void CreateNormalsFromVertices(
        wsp::Vector<wsp::Vector3<float>>& o_normals,
        const wsp::Vector<wsp::Vector3<float>>& positions,
        const wsp::Vector<int>& face_point_indices
        );

    WSP_DLL_EXPORT void CreateNormalsFromVertices(
        wsp::Vector3<float>* o_normal_array, int normal_array_length,
        const wsp::Vector3<float>* in_position_array, int position_array_length,
        const int* in_face_vertex_indiex_array, int face_vertex_index_array_length);


    /**
     * @brief Create taurus primitive.
     * @param o_vtx output vertex array, the memory size must be [(blocks + 1) * (blocks + 1) * 8]
     * @param o_indices output index array, the memory size must be [blocks * (blocks + 1) * 2]
     */
    WSP_DLL_EXPORT void CreateTaurusPoly(
        float *o_vtx, 
        float *o_nomals,
        float *o_uvs,
        int *o_indices, 
        int blocks=36
    );

    /**
     * 
     */
    WSP_DLL_EXPORT void CreateVtxNormalsOnTriangleMesh(
        const wsp::Vector<wsp::Point<float>> &in_vtx_pos, 
        const wsp::Vector<int> &in_vid_for_faces,
        wsp::Vector<wsp::Vector3<float>> &o_normals
    );
    
    WSP_DLL_EXPORT void CreateRectanglePolygon(
        wsp::Vector<wsp::Vector3<float>>& positions,
        wsp::Vector<wsp::Vector2<float>>& uvs,
        wsp::Vector<int>& indexes);

    WSP_DLL_EXPORT void CreateCircleEdgePolygon(
        wsp::Vector<wsp::Vector3<float>>& positions,
        wsp::Vector<wsp::Vector2<float>>& uvs,
        wsp::Vector<int>& indexes);

    WSP_DLL_EXPORT void CreateCirclePolygon(
        wsp::Vector<wsp::Vector3<float>>& positions,
        wsp::Vector<wsp::Vector2<float>>& uvs,
        wsp::Vector<int>& indexes);

    //! \ingroup WspGraphic
    //! \brief create qudratic mesh from depth map.
    WSP_DLL_EXPORT wsp::State CreateQuadMeshFromDepthMap(
        wsp::Vector<wsp::Point<float>> &o_vtxArray, 
        wsp::Vector<int> &o_vtxCountOnFace, 
        wsp::Vector<int> &o_faceConnects, 
        wsp::Vector<int> *o_edge_indices,
        const char *filename,                                                     
        const wsp::graphic::PerspViewParam &view_param,
        double face_depth_thre=20.0, 
        int samplingStep=1, 
        uchar alpha_thre=127,
        bool invert_depth=false
    );

    //! \ingroup WspGraphic
    //! \brief create triangle mesh from depth map.
    WSP_DLL_EXPORT wsp::State CreateTriMeshFromDepthMap(
        wsp::Vector<wsp::Point<float>> &o_vtxArray, 
        wsp::Vector<int> &o_vtxCountOnFace, 
        wsp::Vector<int> &o_faceConnects, 
        wsp::Vector<int> *o_edge_indices,
        wsp::Vector<wsp::Vector3<float>> *o_normals,
        wsp::Vector<float> *o_uvs,
        const char *filename, 
        const wsp::graphic::PerspViewParam &view_param,
        double face_depth_thre=20.0, 
        int samplingStep=1, 
        uchar alpha_thre=127, 
        double scale=1.0,
        bool invert_depth=false,
        float offset_x=0.0f, float offset_y=0.0f, float offset_z=0.0f
    );

    inline wsp::State CreateTriMeshFromDepthMap(
        wsp::Vector<wsp::Point<float>> &o_vtxArray, 
        wsp::Vector<int> &o_vtxCountOnFace, 
        wsp::Vector<int> &o_faceConnects, 
        wsp::Vector<int> *o_edge_indices,
        wsp::Vector<wsp::Vector3<float>> *o_normals,
        wsp::Vector<float> *o_uvs,
        const char *filename, 
        const wsp::graphic::PerspViewParam &view_param,
        const wsp::graphic::DepthToMeshParam &d2m_param
    );
}}

#include "fn-graphic_proc.inl"

