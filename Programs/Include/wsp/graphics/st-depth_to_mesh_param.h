/**
 * @file st-depth_to_mesh_param.h
 * 
 */

#ifndef WSP_GRAPHIC_STRUCT_DEPTH_TO_MESH_PARAM_H_
#define WSP_GRAPHIC_STRUCT_DEPTH_TO_MESH_PARAM_H_

#include "_define_graphic.h"

struct WSP_DepthToMeshParam{
    f64  face_depth_thre;
    s32  sampling_step;
    u8   alpha_thre; 
    f64  scale;
    bool invert_depth;
};

#ifdef __cplusplus
namespace wsp{ namespace graphic{
#endif
    const struct WSP_DepthToMeshParam DEFAULT_DEPTH_TO_MESH_PARAM = {
        20.0,
        1,
        127,
        1.0,
        false
    };
#ifdef __cplusplus
}}
#endif

#ifdef __cplusplus
namespace wsp{ namespace graphic{
    typedef struct WSP_DepthToMeshParam DepthToMeshParam;
}}
#endif

#endif
