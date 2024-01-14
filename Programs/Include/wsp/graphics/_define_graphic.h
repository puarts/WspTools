/**
 * @file _define_graphic.h
 * 
 * definition of graphic component
 */

#ifndef WSP_GRAPHIC_DEFINE_H_
#define WSP_GRAPHIC_DEFINE_H_

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include <wsp/common/_define_commoncore.h>
#endif
//#include <wsp/math/_define_mathutil.h>

#define BEGIN_WSP_GRAPHIC_NAMESPACE BEGIN_WSP_CORE_NAMESPACE namespace graphic{
#define END_WSP_GRAPHIC_NAMESPACE } END_WSP_CORE_NAMESPACE

#ifdef __cplusplus
namespace wsp{ namespace graphic{
#endif
enum WSP_PolyDisplayType{
    POLY_DISPLAY_WIREFRAME,
    POLY_DISPLAY_POINTS,
    POLY_DISPLAY_TRIANGLES,
    POLY_DISPLAY_QUADS
};

enum WSP_PolyPrimitiveType{
    POLY_PRIMITIVE_PLANE,
    POLY_PRIMITIVE_SPHERE,
    POLY_PRIMITIVE_TORUS
};
#ifdef __cplusplus
}}
#endif

struct WSP_TexCoord{
    float u, v;
};

namespace wsp{ 
/**
 * @brief Collection of functions and classes for WspGraphic module
 */
namespace graphic{
    typedef WSP_PolyDisplayType   PolyDisplayType;
    typedef WSP_PolyPrimitiveType PolyPrimitiveType;

    typedef WSP_TexCoord          TexCoord;
}}


#include "_decl_graphic.h"

#include "_define_graphic.inl"

#endif