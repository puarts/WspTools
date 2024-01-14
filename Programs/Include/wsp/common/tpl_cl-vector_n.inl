/** 
 * @file tpl_cl-vector_n.inl
 * 
 */


#ifndef __WSP_VECTOR_N_INL__
#define __WSP_VECTOR_N_INL__

#include "tpl_cl-vector_n.h"

#include <math.h>
#include <iostream>

#include "_tpl_cl-stackarray.h"
#include "_tpl_fn-array.hpp"
#ifdef USE_QT
    #include <QtGui/QtGui>
#endif

// Constructor, Destructor ----------------------------------
template<class T, int N_ELEM>
inline wsp::VectorN<T, N_ELEM>::VectorN(){
    memset(data_, 0, N_ELEM*sizeof(T));
}
//template<class T, int N_ELEM>
//inline wsp::VectorN<T, N_ELEM>::VectorN(int value){
//    data_[0] = static_cast<T>(value), data_[1] = static_cast<T>(value), data_[2] = static_cast<T>(value);
//}
template<class T, int N_ELEM>
inline wsp::VectorN<T, N_ELEM>::VectorN(const wsp::VectorN<T, N_ELEM> &vec)
{
    operator=(vec);
}
#ifdef USE_QT
template<class T, int N_ELEM>
inline wsp::VectorN<T, N_ELEM>::VectorN(const QRgb &in_data){
    data_[0] = static_cast<T>(qRed(in_data)), data_[1] = static_cast<T>(qGreen(in_data)), data_[2] = static_cast<T>(qBlue(in_data));
}
#endif


// Operators -------------------------------------------------
template<class T, int N_ELEM>
inline  wsp::VectorN<T, N_ELEM>& wsp::VectorN<T, N_ELEM>::operator=(const wsp::VectorN<T, N_ELEM>& other){
    memcpy(data_, other.data(), sizeof(T)*N_ELEM);
    //if(this != &other){
    //    for(int i=0; i<N_ELEM; i++){ data_[i]=other[i]; }
    //}
    return *this;
}
template<class T, int N_ELEM>
inline  wsp::VectorN<T, N_ELEM>& wsp::VectorN<T, N_ELEM>::operator=(int rhs){
    for(int i=0; i<N_ELEM; i++){ data_[i]=static_cast<T>(rhs); }
    return *this;
}
#ifdef USE_QT
template<class T, int N_ELEM>
inline  wsp::VectorN<T, N_ELEM>& wsp::VectorN<T, N_ELEM>::operator=(const QRgb& in_data){
    data_[0] = static_cast<T>(qRed(in_data)), data_[1] = static_cast<T>(qGreen(in_data)), data_[2] = static_cast<T>(qBlue(in_data));
    return *this;
}
#endif
template<class T, int N_ELEM>
inline T& wsp::VectorN<T, N_ELEM>::operator[](int index){
    return data_[index];
}
template<class T, int N_ELEM>
inline const T& wsp::VectorN<T, N_ELEM>::operator[](int index) const{
    return data_[index];
}

template<class T, int N_ELEM>
inline bool wsp::VectorN<T, N_ELEM>::operator==(const wsp::VectorN<T, N_ELEM> &other) const{
    for(int i=0; i<N_ELEM; i++){
        if( data_[i]!=other[i]){ return false; }
    }
    return true;
}
template<class T, int N_ELEM>
inline bool wsp::VectorN<T, N_ELEM>::operator!=(const wsp::VectorN<T, N_ELEM> &rhs) const{
    return !(*this==rhs);
}

template<class T, int N_ELEM>
inline bool wsp::VectorN<T, N_ELEM>::operator>(const wsp::VectorN<T, N_ELEM>& rhs) const{
    T av_l = wsp::GetSum(data_, N_ELEM);
    T av_r = wsp::GetSum(&rhs[0], N_ELEM);
    if(av_l>av_r){ return true; }
    return false;
}
template<class T, int N_ELEM>
inline bool wsp::VectorN<T, N_ELEM>::operator<(const wsp::VectorN<T, N_ELEM>& rhs) const{
    T av_l = wsp::GetSum(data_, N_ELEM);
    T av_r = wsp::GetSum(&rhs[0], N_ELEM);
    if(av_l<av_r){ return true; }
    return false;
}
template<class T, int N_ELEM>
inline bool wsp::VectorN<T, N_ELEM>::operator>=(const wsp::VectorN<T, N_ELEM>& rhs) const{
    T av_l = wsp::GetSum(data_, N_ELEM);
    T av_r = wsp::GetSum(&rhs[0], N_ELEM);
    if(av_l>=av_r){    return true; }
    return false;
}
template<class T, int N_ELEM>
inline bool wsp::VectorN<T, N_ELEM>::operator<=(const wsp::VectorN<T, N_ELEM>& rhs) const{
    T av_l = wsp::GetSum(data_, N_ELEM);
    T av_r = wsp::GetSum(&rhs[0], N_ELEM);
    if(av_l<=av_r){    return true; }
    return false;
}
template<class T, int N_ELEM>
inline wsp::VectorN<T, N_ELEM> wsp::VectorN<T, N_ELEM>::operator+(const wsp::VectorN<T, N_ELEM>& data) const
{
    VectorN<T, N_ELEM> res(*this);
    for(int i=0; i<N_ELEM; i++)
    {
        res[i]+=data[i];
    }
    return res;
}
template<class T, int N_ELEM>
inline  wsp::VectorN<T, N_ELEM>& wsp::VectorN<T, N_ELEM>::operator+=(const wsp::VectorN<T, N_ELEM>& data){
    for(int i=0; i<N_ELEM; i++){ data_[i]+=data[i]; }
    return *this;
}
template<class T, int N_ELEM>
inline wsp::VectorN<T, N_ELEM> wsp::VectorN<T, N_ELEM>::operator-(const wsp::VectorN<T, N_ELEM>& data) const
{
    VectorN<T, N_ELEM> res(*this);
    for(int i=0; i<N_ELEM; i++){ res[i]-=data[i]; }
    return res;
}
template<class T, int N_ELEM>
inline  wsp::VectorN<T, N_ELEM>& wsp::VectorN<T, N_ELEM>::operator-=(const wsp::VectorN<T, N_ELEM>& data)
{
    for(int i=0; i<N_ELEM; i++){ data_[i]-=data[i]; }
    return *this;
}
template<class T, int N_ELEM>
inline wsp::VectorN<T, N_ELEM> wsp::VectorN<T, N_ELEM>::operator*(const wsp::VectorN<T, N_ELEM>& data) const{
    VectorN<T, N_ELEM> res(*this);
    for(int i=0; i<N_ELEM; i++){ res[i]*=data[i]; }
    return res;
}
template<class T, int N_ELEM>
inline  wsp::VectorN<T, N_ELEM>& wsp::VectorN<T, N_ELEM>::operator*=(const wsp::VectorN<T, N_ELEM>& data){
    for(int i=0; i<N_ELEM; i++){ data_[i]*=data[i]; }
    return *this;
}
template<class T, int N_ELEM>
inline wsp::VectorN<T, N_ELEM> wsp::VectorN<T, N_ELEM>::operator*(double value) const{
    VectorN<T, N_ELEM> res(*this);
    T v = static_cast<T>(value);
    for(int i=0; i<N_ELEM; i++){ res[i]*=v; }
    return res;
}
template<class T, int N_ELEM>
inline  wsp::VectorN<T, N_ELEM>& wsp::VectorN<T, N_ELEM>::operator*=(double value){
    T v = static_cast<T>(value);
    for(int i=0; i<N_ELEM; i++){ data_[i]*=v; }
    return *this;
}
template<class T, int N_ELEM>
inline wsp::VectorN<T, N_ELEM> wsp::VectorN<T, N_ELEM>::operator/(const wsp::VectorN<T, N_ELEM>& data) const{
    VectorN<T, N_ELEM> res(*this);
    for(int i=0; i<N_ELEM; i++){ 
        if(data[i]==0){ WSP_COMMON_WARNING_LOG("division by 0\n"); continue; }
        res[i]/=data[i]; 
    }
    return res;
}
template<class T, int N_ELEM>
inline wsp::VectorN<T, N_ELEM>& wsp::VectorN<T, N_ELEM>::operator/(const wsp::VectorN<T, N_ELEM>& data){
    for(int i=0; i<N_ELEM; i++){ 
        if(data[i]==0){ WSP_COMMON_WARNING_LOG("division by 0\n"); continue; }
        data_[i]/=data[i]; 
    }
    return *this;
}
template<class T, int N_ELEM>
inline  wsp::VectorN<T, N_ELEM>& wsp::VectorN<T, N_ELEM>::operator/=(const wsp::VectorN<T, N_ELEM>& data){
    for(int i=0; i<N_ELEM; i++){ 
        if(data[i]==0){ WSP_COMMON_WARNING_LOG("division by 0\n"); continue; }
        data_[i]/=data[i]; 
    }
    return *this;
}



template<class T, int N_ELEM>
inline const wsp::VectorN<T, N_ELEM> operator -(const wsp::VectorN<T, N_ELEM> &rhs){
    wsp::VectorN<T, N_ELEM> res;
    for(int i=0; i<N_ELEM; i++){ 
        res[i] = -rhs[i]; 
    }
    return res;
}

// I/O --------------------------------------------------------

template<class T, int N_ELEM>
inline std::ostream& operator<<(std::ostream& lhs, const wsp::VectorN<T, N_ELEM> &rhs)
{
    int i;
    for(i=0; i<N_ELEM-1; i++){ lhs<<rhs[i]<<", "; }
    lhs<<rhs[i];
    return lhs;
}
template<class T, int N_ELEM>
inline std::ostream& operator<<(std::ostream& lhs, const wsp::VectorN<T, N_ELEM> *rhs)
{
    lhs<<*rhs;
    return lhs;
}

#endif