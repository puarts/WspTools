// @file wsprgb.h
// 

#ifndef __WspRgb_H__
#define __WspRgb_H__

#ifndef __WSP_COMMONUTIL_DEFINE_COMMONUTIL_H__
#include "_define_commonutil.h"
#endif
#include <iostream>
#include <math.h>

#ifdef USE_QT
    #include <QtGui/QtGui>
#endif

class WspDouble3;

class WspRgb{
public:
    uchar b, g, r;
    //uchar r, g, b;

public:
    inline WspRgb();
    inline WspRgb(int red, int green, int blue);
    inline WspRgb(double x, double y, double z);
    inline WspRgb(uchar red, uchar green, uchar blue);
    inline explicit WspRgb(int value);
    inline explicit WspRgb(double value);
    #ifdef USE_QT
    inline explicit WspRgb(const QRgb& qrgb);
    #endif
    inline WspRgb(const WspRgb& rgb);

    // Operators --------------------
    inline WspRgb& operator=(const WspRgb& other);
    inline WspRgb& operator=(int rhs);
    inline WspRgb& operator=(uchar rhs);
    inline WspRgb& operator=(double rhs);
    #ifdef USE_QT
    inline WspRgb& operator=(const QRgb& qrgb);
    #endif

    inline operator double(){ return (double)(r+g+b)/3.0; }

    inline bool operator==(const WspRgb& other) const;
    inline bool operator>(const WspRgb& rhs) const;
    inline bool operator<(const WspRgb& rhs) const;
    inline bool operator>=(const WspRgb& rhs) const;
    inline bool operator<=(const WspRgb& rhs) const;

    inline WspRgb operator+ (const WspRgb &p) const;
    inline WspRgb& operator+= (const WspRgb &p);

    inline WspRgb operator- (const WspRgb &p) const;
    inline double operator- (double rhs) const;
    inline WspRgb& operator-= (const WspRgb &p);

    inline WspRgb operator* (const WspRgb &p) const;
    inline WspRgb& operator*= (const WspRgb &p);
    inline WspRgb operator* (double value) const;
    inline WspRgb& operator*= (double value);

    inline WspRgb operator/ (const WspRgb &p) const;
    inline WspRgb& operator/= (const WspRgb &p);
};

#define BLANK_COL (WspRgb(0,0,0))

// Constructor, Destructor ----------------------------------
inline WspRgb::WspRgb(){
    r = 0, g = 0, b = 0;
}
inline WspRgb::WspRgb(int red, int green, int blue){
    r = (uchar)red, g = (uchar)green, b = (uchar)blue;
}
inline WspRgb::WspRgb(uchar red, uchar green, uchar blue){
    r = (uchar)red, g = (uchar)green, b = (uchar)blue;
}
inline WspRgb::WspRgb(int value){
    r = (uchar)value, g = (uchar)value, b = (uchar)value;
}
inline WspRgb::WspRgb(double value){ *this=WspRgb(value, value, value); }
inline WspRgb::WspRgb(double x, double y, double z){
    if(0.0<=x && x<=255.0){ r = (uchar)floor(x); }
    else if(x>255.0){ r = 255; }
    else{ r = 0; }
    if(0.0<=y && y<=255.0){ g = (uchar)floor(y); }
    else if(y>255.0){ g = 255; }
    else{ g = 0; }
    if(0.0<=z && z<=255.0){ b = (uchar)floor(z); }
    else if(z>255.0){ b = 255; }
    else{ b = 0; }
}
inline WspRgb::WspRgb(const WspRgb &rgb){
    r = rgb.r, g = rgb.g, b = rgb.b;
}
#ifdef USE_QT
inline WspRgb::WspRgb(const QRgb &qrgb){
    r=(uchar)qRed(qrgb), g=(uchar)qGreen(qrgb), b=(uchar)qBlue(qrgb);
}
#endif

// Operators -------------------------------------------------
inline WspRgb& WspRgb::operator=(const WspRgb& other){
    if(this != &other){
        r = other.r; g = other.g; b = other.b;
    }
    return *this;
}
inline WspRgb& WspRgb::operator=(int rhs){
    r=(uchar)rhs, g=(uchar)rhs, b=(uchar)rhs;
    return *this;
}
inline WspRgb& WspRgb::operator=(uchar rhs){
    r=(uchar)rhs, g=(uchar)rhs, b=(uchar)rhs;
    return *this;
}
inline WspRgb& WspRgb::operator=(double rhs){
    *this=WspRgb(rhs, rhs, rhs);
    return *this;
}

#ifdef USE_QT
inline WspRgb& WspRgb::operator=(const QRgb& qrgb){
    r=(uchar)qRed(qrgb), g=(uchar)qGreen(qrgb), b=(uchar)qBlue(qrgb);
    return *this;
}
#endif

inline bool WspRgb::operator==(const WspRgb &other) const{
    if( r==other.r && g==other.g && b==other.b){
        return true;
    }
    return false;
}
inline bool WspRgb::operator>(const WspRgb& rhs) const{
    double av_l = (r+g+b)/3.0;
    double av_r = (rhs.r + rhs.g + rhs. b)/3.0;
    if(av_l>av_r){
        return true;
    }
    return false;
}
inline bool WspRgb::operator<(const WspRgb& rhs) const{
    double av_l = (r+g+b)/3.0;
    double av_r = (rhs.r + rhs.g + rhs. b)/3.0;
    if(av_l<av_r){
        return true;
    }
    return false;
}
inline bool WspRgb::operator>=(const WspRgb& rhs) const{
    double av_l = (r+g+b)/3.0;
    double av_r = (rhs.r + rhs.g + rhs. b)/3.0;
    if(av_l>=av_r){
        return true;
    }
    return false;
}
inline bool WspRgb::operator<=(const WspRgb& rhs) const{
    double av_l = (r+g+b)/3.0;
    double av_r = (rhs.r + rhs.g + rhs. b)/3.0;
    if(av_l<=av_r){
        return true;
    }
    return false;
}
inline WspRgb WspRgb::operator+(const WspRgb& rgb) const{
    WspRgb res(*this);
    res.r+=rgb.r; res.g+=rgb.g; res.b+=rgb.b;
    return res;
}
inline WspRgb& WspRgb::operator+=(const WspRgb& rgb){
    r+=rgb.r; g+=rgb.g; b+=rgb.b;
    return *this;
}
inline WspRgb WspRgb::operator-(const WspRgb& rgb) const{
    WspRgb res(*this);
    res.r-=rgb.r; res.g-=rgb.g; res.b-=rgb.b;
    return res;
}
inline double WspRgb::operator-(double rhs) const{
    double res;
    res = ((double)(r+g+b)/3.0)-rhs;
    return res;
}
inline WspRgb& WspRgb::operator-=(const WspRgb& rgb){
    r-=rgb.r; g-=rgb.g; b-=rgb.b;
    return *this;
}
inline WspRgb WspRgb::operator*(const WspRgb& rgb) const{
    WspRgb res(*this);
    res.r*=rgb.r; res.g*=rgb.g; res.b*=rgb.b;
    return res;
}
inline WspRgb& WspRgb::operator*=(const WspRgb& rgb){
    r*=rgb.r; g*=rgb.g; b*=rgb.b;
    return *this;
}
inline WspRgb WspRgb::operator*(double value) const{
    double x=(double)r * value;
    double y=(double)g * value;
    double z=(double)b * value;
    WspRgb res(x, y, z);
    return res;
}
inline WspRgb& WspRgb::operator*=(double value){
    *this = *this * value;
    return *this;
}
inline WspRgb WspRgb::operator/(const WspRgb& rgb) const{
    WspRgb res(*this);
    if(rgb.r!=0){ res.r/=rgb.r; }
    if(rgb.g!=0){ res.g/=rgb.g; }
    if(rgb.b!=0){ res.b/=rgb.b; }
    return res;
}
inline WspRgb& WspRgb::operator/=(const WspRgb& rgb){
    if(rgb.r!=0){ r/=rgb.r; }
    if(rgb.g!=0){ g/=rgb.g; }
    if(rgb.b!=0){ b/=rgb.b; }
    return *this;
}

// I/O --------------------------------------------------------

inline std::ostream& operator<<(std::ostream& lhs, const WspRgb &rhs)
{
    lhs<<(int)rhs.r<<","<<(int)rhs.g<<","<<(int)rhs.b;
    return lhs;
}
inline std::ostream& operator<<(std::ostream& lhs, const WspRgb *rhs)
{
    lhs<<*rhs;
    return lhs;
}

#endif