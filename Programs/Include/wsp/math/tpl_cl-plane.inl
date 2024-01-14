/**
 * @file tpl_cl-plane.inl
 * 
 */

#ifndef    WSP_MATH_BASE_TEMPLATE_CLASS_PLANE_INL_
#define    WSP_MATH_BASE_TEMPLATE_CLASS_PLANE_INL_

template<typename T> 
wsp::Plane<T>::Plane()
    : dot_(0)
{}

template<typename T> 
wsp::Plane<T>::Plane( const T normal[3], const T dot )
    : dot_(dot)
{
    memcpy(normal_, normal, sizeof(T)*3);
}

template<typename T> 
wsp::Plane<T>::Plane( const T normal[3], const T point_on_plane[3] ) 
{
    memcpy(normal_, normal, sizeof(T)*3);
    dot_ = normal[0] * point_on_plane[0] + normal[1] * point_on_plane[1] + normal[2] * point_on_plane[2];
}

#endif

