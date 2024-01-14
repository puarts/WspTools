/**
 * @file tpl_cl-vector3.h
 * 
 */

#ifndef __WSP_VECTOR_3_H__
#define __WSP_VECTOR_3_H__

#include <math.h>
#include <iostream>

#include "tpl_cl-vector_n.h"

namespace wsp{

    template<class T=double>
    class WSP_DLL_EXPORT Vector3
        :public VectorN<T, 3>
    {
    public:
        inline Vector3(): wsp::VectorN<T, 3>()
        {
        }

        template<typename U>
        inline Vector3(U in_x, U in_y, U in_z)
        {
            wsp::VectorN<T, 3>::data_[0]=static_cast<T>(in_x); 
            wsp::VectorN<T, 3>::data_[1]=static_cast<T>(in_y); 
            wsp::VectorN<T, 3>::data_[2]=static_cast<T>(in_z);
        }

        template<typename U>
        inline explicit Vector3(U value)
        {
            wsp::VectorN<T, 3>::data_[0]=static_cast<T>(value); 
            wsp::VectorN<T, 3>::data_[1]=static_cast<T>(value); 
            wsp::VectorN<T, 3>::data_[2]=static_cast<T>(value);
        }
    #ifdef USE_QT
        explicit inline Vector3(const QRgb& in_data);
    #endif
        inline Vector3(const Vector3<T>& data); //: wsp::VectorN<T, 3>(data){}
        explicit inline Vector3(const VectorN<T, 3>& data): wsp::VectorN<T, 3>(data){}


        // Getter ----------------------------------------------------

        inline Vector3<T> GetCrossProduct(const Vector3<T> &rhs) const{
            return Vector3<T>(wsp::VectorN<T, 3>::data_[1]*rhs[2]-wsp::VectorN<T, 3>::data_[2]*rhs[1], 
                              wsp::VectorN<T, 3>::data_[2]*rhs[0]-wsp::VectorN<T, 3>::data_[0]*rhs[2],
                              wsp::VectorN<T, 3>::data_[0]*rhs[1]-wsp::VectorN<T, 3>::data_[1]*rhs[0] );
        }
        // Mutator ----------------------------------------------------

        // Accessor
        inline T x() const { return wsp::VectorN<T, 3>::data_[0]; }
        inline T y() const { return wsp::VectorN<T, 3>::data_[1]; }
        inline T z() const { return wsp::VectorN<T, 3>::data_[2]; }

        // mutator
        inline void set_x(T x) { wsp::VectorN<T, 3>::data_[0] = x; }
        inline void set_y(T y) { wsp::VectorN<T, 3>::data_[1] = y; }
        inline void set_z(T z) { wsp::VectorN<T, 3>::data_[2] = z; }

        // Operators --------------------
        inline Vector3<T>& operator=(const VectorN<T, 3>& rhs){
            memcpy(wsp::VectorN<T, 3>::data_, rhs.data(), sizeof(T)*3);
            return *this;
        }
    };


    template<typename T=float>
    class WSP_DLL_EXPORT Point: public wsp::Vector3<T>
    {
    public:
        inline Point(){}
        inline explicit Point(int value){} // for int conversion
        inline Point(T x, T y, T z=0): wsp::Vector3<T>(x, y, z){}
        inline explicit Point(const wsp::Vector3<T> &vec)
        {
            wsp::VectorN<T, 3>::data_[0] = vec[0];
            wsp::VectorN<T, 3>::data_[1] = vec[1];
            wsp::VectorN<T, 3>::data_[2] = vec[2];
        }
        explicit inline Point(const wsp::VectorN<T, 3> &vec)
        {
            wsp::VectorN<T, 3>::data_[0] = vec[0];
            wsp::VectorN<T, 3>::data_[1] = vec[1];
            wsp::VectorN<T, 3>::data_[2] = vec[2];
        }
        inline Point(const wsp::Point<T> &point)
        {
            wsp::VectorN<T, 3>::data_[0] = point[0];
            wsp::VectorN<T, 3>::data_[1] = point[1];
            wsp::VectorN<T, 3>::data_[2] = point[2];
        }
    };

    template class Point<float>;

    template<typename T=float>
    class WSP_DLL_EXPORT Color: public wsp::Vector3<T>
    {
    public:
        inline Color(){}
        inline explicit Color(const wsp::Vector3<T> &vec)
        {
            wsp::VectorN<T, 3>::data_[0] = vec[0];
            wsp::VectorN<T, 3>::data_[1] = vec[1];
            wsp::VectorN<T, 3>::data_[2] = vec[2];
        }
        inline explicit Color(const wsp::VectorN<T, 3> &vec)
        {
            wsp::VectorN<T, 3>::data_[0] = vec[0];
            wsp::VectorN<T, 3>::data_[1] = vec[1];
            wsp::VectorN<T, 3>::data_[2] = vec[2];
        }
        inline explicit Color(T r, T g, T b): wsp::Vector3<T>(r, g, b)
        {
        }

        inline explicit Color(const wsp::Color<T> &color)
        {
            wsp::VectorN<T, 3>::data_[0] = color[0];
            wsp::VectorN<T, 3>::data_[1] = color[1];
            wsp::VectorN<T, 3>::data_[2] = color[2];
        }
        // Accessors
        inline T r() const{ return wsp::VectorN<T, 3>::data_[0]; }
        inline T g() const{ return wsp::VectorN<T, 3>::data_[1]; }
        inline T b() const{ return wsp::VectorN<T, 3>::data_[2]; }

        // Mutators
        inline void set_r(T r){ wsp::VectorN<T, 3>::data_[0]=r; }
        inline void set_g(T g){ wsp::VectorN<T, 3>::data_[1]=g; }
        inline void set_b(T b){ wsp::VectorN<T, 3>::data_[2]=b; }
    };

    template class Color<float>;

    typedef Vector3<float> Vec3F;
    typedef Vector3<float> Vector3F;
}

// Constructor, Destructor ----------------------------------

template<class T>
inline wsp::Vector3<T>::Vector3(const wsp::Vector3<T> &data)//: wsp::VectorN<T, 3>(data)
{
    this->operator=(data);
}

#endif