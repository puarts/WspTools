/** 
 * @file tpl_cl-vector_n.h
 * 
 */


#ifndef __WSP_VECTOR_N_H__
#define __WSP_VECTOR_N_H__

#include <math.h>
#include <iostream>

#include "_tpl_cl-stackarray.h"
#include "_tpl_fn-array.hpp"
#ifdef USE_QT
    #include <QtGui/QtGui>
#endif

BEGIN_WSP_CORE_NAMESPACE

//! T is type, N_ELEM is number of elements
template<class T, int N_ELEM=3>
class VectorN
    //:public wsp::StackArray<T, N_ELEM>
{
public:
    // Constructors -------------------------------------------
    inline VectorN();
    template<typename U>
    explicit inline VectorN(const U in_vec[N_ELEM])
    {
        memcpy(data_, in_vec, sizeof(U)*N_ELEM);
    }
#ifdef USE_QT
    explicit inline VectorN(const QRgb& in_data);
#endif
    inline VectorN(const VectorN<T, N_ELEM>& data);

    // Getter ----------------------------------------------------
    inline const T* data()const { return &data_[0]; }
    inline T* data(){ return &data_[0]; }
    inline T GetNormSqure(const VectorN<T, N_ELEM> &target) const{
        T sum = 0;
        for(int i=0; i<N_ELEM; i++){ sum+=(data_[i]-target[i])*(data_[i]-target[i]); }
        return sum;
    }
    inline T GetNorm(const VectorN<T, N_ELEM> &target) const{
        return sqrt(GetNormSqure(target));
    }
    inline T GetDotProduct(const VectorN<T, N_ELEM> &rhs) const{
        T dot = 0;
        for(int i=0; i<N_ELEM; i++){ dot+=data_[i]*rhs[i]; }
        return dot;
    }

    // Mutator ----------------------------------------------------
    inline void set_data(T value, int index){ data_[index] = value; }
    inline void Normalize(T norm_min, T norm_max)
    {
        double max_value = static_cast<double>(wsp::GetMax(data_, N_ELEM));
        double min_value = static_cast<double>(wsp::GetMin(data_, N_ELEM));
        double range = max_value - min_value;
        double norm_range = static_cast<double>(norm_max) - static_cast<double>(norm_min);
        for(u32 i=0; i<N_ELEM; i++)
        {
            double tmp_value = (static_cast<double>(data_[i]) - min_value) * norm_range/range - static_cast<double>(norm_min);
            data_[i] = static_cast<T>(tmp_value);
        }
    }

    inline void NormalizeLength(T normalized_length = 1)
    {
        T *ptr = data_;
        T *end_ptr = data_+N_ELEM;
        double sum = 0.0;
        for(;ptr!=end_ptr; ++ptr)
        {
            sum += (*ptr)*(*ptr);
        }
        double length = sqrt(sum);
        if (length == 0.0)
        {
            return;
        }

        double mult = (double)normalized_length / length;
        ptr = data_;
        for(;ptr!=end_ptr; ++ptr)
        {
            *ptr = static_cast<T>(*ptr * mult);
        }
    }

    inline void SetRand(T min, T max){
        double range = static_cast<double>(max) - static_cast<double>(min);
        for(int i=0; i<N_ELEM; i++){
            data_[i] = static_cast<T>((rand()/static_cast<double>(RAND_MAX))*range + static_cast<double>(min));
        }
    }
    inline void SetRand(T max){ SetRand(0, max); }

    // Operators --------------------
    inline VectorN<T, N_ELEM>& operator=(const VectorN<T, N_ELEM>& other);
    inline VectorN<T, N_ELEM>& operator=(int rhs);
#ifdef USE_QT
    inline VectorN<T, N_ELEM>& operator=(const QRgb& in_data);
#endif
    inline T& operator[](int index);
    inline const T& operator[](int index) const;
    inline bool operator==(const VectorN<T, N_ELEM>& other) const;
    inline bool operator!=(const VectorN<T, N_ELEM>& other) const;
    inline bool operator>(const VectorN<T, N_ELEM>& rhs) const;
    inline bool operator<(const VectorN<T, N_ELEM>& rhs) const;
    inline bool operator>=(const VectorN<T, N_ELEM>& rhs) const;
    inline bool operator<=(const VectorN<T, N_ELEM>& rhs) const;

    inline VectorN<T, N_ELEM> operator+ (const VectorN<T, N_ELEM> &p) const;
    inline VectorN<T, N_ELEM>& operator+= (const VectorN<T, N_ELEM> &p);

    inline VectorN<T, N_ELEM> operator- (const VectorN<T, N_ELEM> &p) const;
    inline VectorN<T, N_ELEM>& operator-= (const VectorN<T, N_ELEM> &p);

    inline VectorN<T, N_ELEM> operator* (const VectorN<T, N_ELEM> &p) const;
    inline VectorN<T, N_ELEM>& operator*= (const VectorN<T, N_ELEM> &p);
    inline VectorN<T, N_ELEM> operator* (double value) const;
    inline VectorN<T, N_ELEM>& operator*= (double value);

    inline VectorN<T, N_ELEM> operator/ (const VectorN<T, N_ELEM> &p) const;
    inline VectorN<T, N_ELEM>& operator/ (const VectorN<T, N_ELEM> &p);
    inline VectorN<T, N_ELEM>& operator/= (const VectorN<T, N_ELEM> &p);

protected:
    T data_[N_ELEM];

};


END_WSP_CORE_NAMESPACE

#include "tpl_cl-vector_n.inl"

#endif