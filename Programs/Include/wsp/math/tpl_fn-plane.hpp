/**
 * @file tpl_fn-plane.hpp
 * 
 */

#ifndef    WSP_MATH_BASE_TEMPLATE_FUNCTION_PLANE_HPP_
#define    WSP_MATH_BASE_TEMPLATE_FUNCTION_PLANE_HPP_

#include "_math_types.h"
#include "_define_mathutil.h"


namespace wsp{ namespace math{

    template<typename T=float>
    struct PlaneParam{
        T normal[3];
        T pivot[3];
        T dot;
    };

    //template<typename T> 
    //wsp::State CalcVerticalCapitationPlane(
    //    const wsp::Vector3<T>& point_a,
    //    const wsp::Vector3<T>& point_b,
    //    wsp::Plane<T>& plane 
    //)
    //{
    //    if( point_a == point_b ){ return WSP_STATE_FAILURE; }

    //    wsp::Vector3<T> normal = point_b - point_a;
    //    wsp::math::Normalize( normal );
    //    plane = wsp::Plane<T>(
    //        normal,
    //        ( point_a + point_b ) / 2 
    //    );

    //    return WSP_STATE_SUCCESS;
    //}

    /**
     * Calculate voronoi bisector plane of given two points.
     * @param in_point_a One of input points
     * @param in_point_b One of input points
     * @param o_bisec_plane_normal Normal parameter for output bisector plane
     * @param o_bisec_plane_dot_product Dot product parameter for output bisector plane
     */
    template<typename T> 
    wsp::math::State GetBisectorPlaneOfTwoPoints(
        const T in_point_a[3],
        const T in_point_b[3],
        wsp::math::PlaneParam<T> &o_bisec_plane_param
    );

    template<typename T> 
    wsp::math::State GetIntersectionPointOfPlaneAndLine(    
        const T in_point_a[3],
        const T in_point_b[3],
        const wsp::math::PlaneParam<T> &in_plane,
        T o_point_intersection[3],
        T parallel_threshold=1.0e-3
    );

    /**
     * Calculate intersection line of two planes
     */
    template<typename T> 
    wsp::math::State GetIntersectionLineOfTwoPlanes(
        const wsp::math::PlaneParam<T> &in_plane_a,
        const wsp::math::PlaneParam<T> &in_plane_b,
        T o_vec_intersect_line[3],
        T o_point_on_intersect_line[3]
    );
}}

#include "tpl_fn-plane.inl"

#endif
