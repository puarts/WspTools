/**
 * @file tpl_cl-plane.hpp
 * 
 */

#ifndef    WSP_MATH_BASE_TEMPLATE_CLASS_PLANE_H_
#define    WSP_MATH_BASE_TEMPLATE_CLASS_PLANE_H_

#include "_fn-math.h"

namespace wsp{
    template<typename T> 
    class Plane
    {
    private:
        //wsp::Vector3<T> normal_;
        T normal_[3];

        /** @brief  
         * dot_ = a x + b y + c z 
         *      = Dot product between point on plane<x, y, z> and normal vector<a, b, c> 
         */
        T dot_;

    public:
        Plane();
        Plane( const T normal[3], const T dot );
        Plane( const T normal[3], const T point_on_plane[3] );
        //Plane( const wsp::Vector3<T> &normal, const T dot );
        //Plane( const wsp::Vector3<T> &normal, const wsp::Vector3<T> &point_on_plane );

        /* ------------------------------------------
                Accessors
           ------------------------------------------*/
        inline wsp::Vector3<T>& normal(){ return normal_; }
        inline T dot(){ return dot_; }

        /* ------------------------------------------
                Mutators
           ------------------------------------------*/
        inline void set_normal(T normal[3]){
            memcpy(normal_, normal, sizeof(T)*3);
        }
        inline void set_dot(T dot){
            dot_ = normal[0] * point_on_plane[0] + normal[1] * point_on_plane[1] + normal[2] * point_on_plane[2];
        }
    };

    typedef wsp::Plane<double> PlaneD;
    typedef wsp::Plane<float> PlaneF;

}



#include "tpl_cl-plane.inl"


#endif
