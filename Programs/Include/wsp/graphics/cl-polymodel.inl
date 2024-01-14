#pragma once

#include <wsp/common/_define_qt.h>
#include <wsp/graphics/tpl_fn-graphic_proc.hpp>


// accessors --------
inline wsp::Vector<wsp::Point<float>>* wsp::graphic::PolygonModel::points()
{ 
    return &points_; 
}

inline wsp::Vector<wsp::Vector3<float>>* wsp::graphic::PolygonModel::normals()
{ 
    return &normals_; 
}

inline int wsp::graphic::PolygonModel::GetNumFaces() const 
{ 
    return points_.length() / 3; 
}

inline int wsp::graphic::PolygonModel::GetNumEdges() const 
{ 
    return edge_point_indices_.length() / 2; 
}

inline int wsp::graphic::PolygonModel::GetNumPoints() const
{ 
    return points_.length(); 
}

inline bool wsp::graphic::PolygonModel::use_self_transform()
{ 
    return use_self_transform_; 
}

// mutators ---------------
inline void wsp::graphic::PolygonModel::set_use_self_transform(bool use_self_transform)
{ 
    use_self_transform_ = use_self_transform; 
}

inline void wsp::graphic::PolygonModel::set_view_mtx(wsp::graphic::TransformMatrix<float> *view_mtx)
{ 
    view_mtx_ = view_mtx; 
}

inline void wsp::graphic::PolygonModel::set_display_type(wsp::graphic::PolyDisplayType disp_type)
{
    printf("set_display_type: before %d, after %d\n", display_type_, disp_type);
    display_type_=disp_type; 
}
inline void wsp::graphic::PolygonModel::set_model_mtx(const wsp::graphic::TransformMatrix<float> &model_mat)
{ 
    model_mtx_ = model_mat; 
}

inline void wsp::graphic::PolygonModel::set_model_mtx(
    const wsp::Vec3F &axis_x, 
    const wsp::Vec3F &axis_y,
    const wsp::Vec3F &axis_z,
    const wsp::Vec3F &translate
){
    model_mtx_[0]=axis_x[0]; model_mtx_[1]=axis_x[1]; model_mtx_[2] =axis_x[2]; model_mtx_[3] =translate[0];
    model_mtx_[4]=axis_y[0]; model_mtx_[5]=axis_y[1]; model_mtx_[6] =axis_y[2]; model_mtx_[7] =translate[1];
    model_mtx_[8]=axis_z[0]; model_mtx_[9]=axis_z[1]; model_mtx_[10]=axis_z[2]; model_mtx_[11]=translate[2];
    model_mtx_[12]=0.0f;     model_mtx_[13]=0.0f;     model_mtx_[14]=0.0f;      model_mtx_[15]=1.0f;
}
inline void wsp::graphic::PolygonModel::Clear()
{
    points_.Clear();
    normals_.Clear();
    uvs_.Clear();
    vtx_point_indices_.Clear();
    edge_point_indices_.Clear();
    face_point_indices_.Clear();
    model_mtx_.SetToIdentity();
    view_mtx_=NULL;
}

inline wsp::State wsp::graphic::PolygonModel::LoadDepthMap(
    const char *filepath, 
    const wsp::graphic::DepthToMeshParam *param
)
{
    return wsp::graphic::PolygonModel::LoadDepthMap(
        filepath, 
        param->face_depth_thre, 
        param->sampling_step, 
        param->alpha_thre,
        param->scale,
        param->invert_depth
    );
}

template<typename _DepthType>
wsp::State wsp::graphic::PolygonModel::CreateDepthMesh(
    const wsp::Image<_DepthType> &depth_map,
    float fov,
    float near_clip_ratio,
    float scale,
    float face_depth_thre,
    float alpha_thre,
    float samplingStep, 
    bool invert_depth,
    float offset_x, float offset_y, float offset_z,
    _DepthType depth_min, _DepthType depth_max
)
{
    wsp::State state;
    Clear();

    wsp::Vector<int> vtx_count_list;
    //double depth_range;
    //if(depth_min==depth_max){
    //    wsp::GetMinMax(depth_map.data(), depth_min, depth_max, depth_map.length());
    //}
    //depth_range = depth_max - depth_min;

    float near_pos = near_clip_ratio*scale ;//*depth_range;
    float far_pos = scale                  ;//*depth_range;

    wsp::graphic::PerspViewParam param = {fov, near_pos, far_pos, 0};

    //wsp::graphic::PerspViewParam param(fov, near_clip_ratio*scale, scale, 0);

    state = wsp::graphic::CreateTriMeshFromDepthMap<_DepthType>(
        depth_map,
        points_, 
        vtx_count_list, 
        face_point_indices_, 
        &edge_point_indices_,
        &normals_,
        &uvs_,
        param,
        face_depth_thre, 
        samplingStep, 
        alpha_thre, 
        scale, 
        invert_depth,
        offset_x, offset_y, offset_z,
        depth_min, depth_max
    );

    if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Creating mesh failed\n"); return state; }
    printf("num of points=%d\n", points_.length());
    printf("num of faces=%d\n", vtx_count_list.length());
    printf("num of vtxs on face=%d\n", face_point_indices_.length());

    // set up point indices
    vtx_point_indices_.SetLength(points_.length());
    int len = points_.length();
    {
        for(int i=0; i<len; ++i){
            vtx_point_indices_[i] = i;
        }
    }
    return WSP_STATE_SUCCESS;
}
