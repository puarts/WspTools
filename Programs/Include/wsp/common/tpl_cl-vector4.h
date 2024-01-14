/**
 * @file tpl_cl-vector4.h
 * 
 */

#ifndef __WSP_CORE_TEMPLATE_CLASS_VECTOR_4_H__
#define __WSP_CORE_TEMPLATE_CLASS_VECTOR_4_H__

#include <math.h>
#include <iostream>

#include "tpl_cl-vector_n.h"

namespace wsp{

    template<class T=double>
    class Vector4
        :public VectorN<T, 4>
    {
    //protected:
    //    T wsp::VectorN<T, 4>::data_[3];

    public:
        inline Vector4(): wsp::VectorN<T, 4>(){}
        template<typename U>
        inline Vector4(U in_x, U in_y, U in_z, U in_w){
            wsp::VectorN<T, 4>::data_[0]=static_cast<T>(in_x); 
            wsp::VectorN<T, 4>::data_[1]=static_cast<T>(in_y); 
            wsp::VectorN<T, 4>::data_[2]=static_cast<T>(in_z);
            wsp::VectorN<T, 4>::data_[3]=static_cast<T>(in_w);
        }
        template<typename U>
        inline explicit Vector4(U value){
            wsp::VectorN<T, 4>::data_[0]=static_cast<T>(value); 
            wsp::VectorN<T, 4>::data_[1]=static_cast<T>(value); 
            wsp::VectorN<T, 4>::data_[2]=static_cast<T>(value);
            wsp::VectorN<T, 4>::data_[3]=static_cast<T>(value);
        }

        inline Vector4(const Vector4<T>& data); //: wsp::VectorN<T, 4>(data){}
        inline explicit Vector4(const VectorN<T, 4>& data): wsp::VectorN<T, 4>(data){}

        // Getter ----------------------------------------------------
        T x() const {
            return data_[0];
        }

        T y() const {
            return data_[1];
        }

        T z() const {
            return data_[2];
        }

        T w() const {
            return data_[3];
        }

        // Mutator ----------------------------------------------------

        // Operators --------------------
        inline Vector4<T>& operator=(const VectorN<T, 4>& rhs){
            memcpy(wsp::VectorN<T, 4>::data_, rhs.data(), sizeof(T)*4);
            return *this;
        }
    };



    template<typename T=float>
    class Point4: public wsp::Vector4<T>
    {
    public:
        inline Point4(){}
        inline Point4(T x, T y, T z, T w): wsp::Vector4<T>(x, y, z, w){}
        inline explicit Point4(const wsp::Vector4<T> &vec){
            wsp::VectorN<T, 4>::data_[0] = vec[0]; wsp::VectorN<T, 4>::data_[1] = vec[1]; wsp::VectorN<T, 4>::data_[2] = vec[2]; wsp::VectorN<T, 4>::data_[3] = vec[3];
        }
        inline explicit Point4(const wsp::VectorN<T, 4> &vec){
            wsp::VectorN<T, 4>::data_[0] = vec[0]; wsp::VectorN<T, 4>::data_[1] = vec[1]; wsp::VectorN<T, 4>::data_[2] = vec[2]; wsp::VectorN<T, 4>::data_[3] = vec[3];
        }
        inline explicit Point4(const wsp::Point4<T> &point){
            wsp::VectorN<T, 4>::data_[0] = point[0]; wsp::VectorN<T, 4>::data_[1] = point[1]; wsp::VectorN<T, 4>::data_[2] = point[2]; wsp::VectorN<T, 4>::data_[3] = point[3];
        }
        // Accessor
        inline T x() const{ return wsp::VectorN<T, 4>::data_[0]; }
        inline T y() const{ return wsp::VectorN<T, 4>::data_[1]; }
        inline T z() const{ return wsp::VectorN<T, 4>::data_[2]; }
        inline T w() const{ return wsp::VectorN<T, 4>::data_[3]; }

        // mutator
        inline void set_x(T x){ wsp::VectorN<T, 4>::data_[0]=x; }
        inline void set_y(T y){ wsp::VectorN<T, 4>::data_[1]=y; }
        inline void set_z(T z){ wsp::VectorN<T, 4>::data_[2]=z; }
        inline void set_w(T w){ wsp::VectorN<T, 4>::data_[3]=w; }
    };

    template<typename T=float>
    class Color4: public wsp::Vector4<T>
    {
    public:
        inline Color4(){}
        inline explicit Color4(const wsp::Vector4<T> &vec){
            wsp::VectorN<T, 4>::data_[0] = vec[0]; wsp::VectorN<T, 4>::data_[1] = vec[1]; wsp::VectorN<T, 4>::data_[2] = vec[2]; wsp::VectorN<T, 4>::data_[3] = vec[3];
        }
        inline explicit Color4(const wsp::VectorN<T, 4> &vec){
            wsp::VectorN<T, 4>::data_[0] = vec[0]; wsp::VectorN<T, 4>::data_[1] = vec[1]; wsp::VectorN<T, 4>::data_[2] = vec[2]; wsp::VectorN<T, 4>::data_[3] = vec[3];
        }
        inline explicit Color4(const wsp::Color4<T> &color){
            wsp::VectorN<T, 4>::data_[0] = color[0]; wsp::VectorN<T, 4>::data_[1] = color[1]; wsp::VectorN<T, 4>::data_[2] = color[2]; wsp::VectorN<T, 4>::data_[3] = color[3];
        }
        inline Color4(T r, T g, T b): wsp::Vector4<T>(r, g, b){}
        // Accessors
        inline T r() const{ return wsp::VectorN<T, 4>::data_[0]; }
        inline T g() const{ return wsp::VectorN<T, 4>::data_[1]; }
        inline T b() const{ return wsp::VectorN<T, 4>::data_[2]; }
        inline T a() const{ return wsp::VectorN<T, 4>::data_[3]; }
        #ifdef USE_QT
        inline QColor toQColor(){
            return QColor(static_cast<int>(255*wsp::VectorN<T, 4>::data_[0]),static_cast<int>(255*wsp::VectorN<T, 4>::data_[1]), static_cast<int>(255*wsp::VectorN<T, 4>::data_[2])); 
        }
        #endif

        // Mutators
        inline void set_r(T r){ wsp::VectorN<T, 4>::data_[0]=r; }
        inline void set_g(T g){ wsp::VectorN<T, 4>::data_[1]=g; }
        inline void set_b(T b){ wsp::VectorN<T, 4>::data_[2]=b; }
        inline void set_a(T a){ wsp::VectorN<T, 4>::data_[3]=a; }
    };
    typedef Vector4<float> Vec4F;
    typedef Vector4<float> Vector4F;
}

template<class T>
inline wsp::Vector4<T>::Vector4(const wsp::Vector4<T> &data)//: wsp::VectorN<T, 4>(data)
{
    this->operator=(data);
}

#endif