/**
 * @file st-persp_view_param.h
 * 
 */

#ifndef WSP_GRAPHIC_STRUCT_PERSP_VIEW_PARAM_H_
#define WSP_GRAPHIC_STRUCT_PERSP_VIEW_PARAM_H_

#include "_define_graphic.h"

struct WSP_PerspViewParam{
    f64 field_of_view;
    f64 near_clip;
    f64 far_clip;
    f32 aspect_ratio;
};

#ifdef __cplusplus
namespace wsp{ namespace graphic{
#endif
    const struct WSP_PerspViewParam DEFAULT_PERSP_VIEW_PARAM = {
        35.0,
        0.1,
        1000.0,
        1.333f
    };
#ifdef __cplusplus
}}
#endif

#ifdef __cplusplus
namespace wsp{ namespace graphic{
    typedef struct WSP_PerspViewParam PerspViewParam;
}}
#endif

#endif
