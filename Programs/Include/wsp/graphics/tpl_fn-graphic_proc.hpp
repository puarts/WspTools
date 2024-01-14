/**
 * @file tpl_fn-graphic_proc.hpp
 * 
 */

#ifndef __WSP_GRAPHIC_TEMPLATE_FUNCTION_GRAPHIC_PROC_HPP__
#define __WSP_GRAPHIC_TEMPLATE_FUNCTION_GRAPHIC_PROC_HPP__

#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#include <wsp/common/tpl_cl-vector.h>
#endif
#include <wsp/common/tpl_cl-vector3.h>
#include <wsp/math/tpl_fn-plane.hpp>
#include "_define_graphic.h"

namespace wsp{
    template<class T, int> class Image;
}

namespace wsp{ namespace graphic{

    /**
     * Correct vertex index in vertex connection list.
     */
    template<typename IndexType>
    wsp::State Reindex(
        const IndexType *in_vtx_indices,
        IndexType *io_vtx_connects,
        int num_vtcs, 
        int num_vtx_connects
    );


    /**
     * Bisect polygonal mesh according to given plane.
     */
    template<typename T> 
    inline wsp::State BisectPolygonalMesh(
        const wsp::Vector<T> &src_points, 
        const wsp::Vector<int> &src_fvtx_counts,
        const wsp::Vector<int> &src_fvtx_connects,
        wsp::Vector<T> &p_points, wsp::Vector<int> &p_fvtx_counts, wsp::Vector<int> &p_fvtx_connects, 
        wsp::Vector<T> &n_points, wsp::Vector<int> &n_fvtx_counts, wsp::Vector<int> &n_fvtx_connects,
        wsp::math::PlaneParam<T> &bisec_plane
    );

    /**
     * Bisect polygonal mesh according to given two voronoi points.
     */
    template<typename T>
    inline wsp::State BisectPolygonalMesh(
        const wsp::Vector<T> &src_points, 
        const wsp::Vector<int> &src_fvtx_counts,
        const wsp::Vector<int> &src_fvtx_connects,
        wsp::Vector<T> &p_points, wsp::Vector<int> &p_fvtx_counts, wsp::Vector<int> &p_fvtx_connects, 
        wsp::Vector<T> &n_points, wsp::Vector<int> &n_fvtx_counts, wsp::Vector<int> &n_fvtx_connects,
        const T in_voronoi_point_a[3], const T in_voronoi_point_b[3]
    );

    /**
     * @group WspGraphic
     * @param in_depth depth data. Recommend that you make the range of depth map to be 0.0 to 1.0.
     * @brief create triangle mesh from depth map
     */
    template<typename _DepthType>
    wsp::State CreateTriMeshFromDepthMap(
        const wsp::Image<_DepthType> &in_depth,
        wsp::Vector<wsp::Point<float>> &o_vtxArray, 
        wsp::Vector<int> &o_vtxCountOnFace, 
        wsp::Vector<int> &o_faceConnects, 
        wsp::Vector<int> *o_edge_indices,
        wsp::Vector<wsp::Vector3<float>> *o_normals,
        wsp::Vector<float> *o_uvs,
        const wsp::graphic::PerspViewParam &view_param,
        double face_depth_thre=20.0, 
        int samplingStep=1, 
        uchar alpha_thre=127, 
        double scale=1.0,
        bool invert_depth=false,
        float offset_x=0.0f, float offset_y=0.0f, float offset_z=0.0f,
        _DepthType depth_min=0.0, _DepthType depth_max=0.0
    );
}}

#include "tpl_fn-graphic_proc.inl"

#endif

