//! WspFloat4.h
// 


#ifndef __WspFloat4_H__
#define __WspFloat4_H__

#include <iostream>
#ifndef __WSP_COMMONUTIL_DEFINE_COMMONUTIL_H__
#include "_define_commonutil.h"
#endif

class WspFloat4{
public:
    float x, y, z, w;

public:
    inline WspFloat4();
    inline WspFloat4(float in_x, float in_y, float in_z, float in_w);
    inline WspFloat4(uchar in_x, uchar in_y, uchar in_z, uchar in_w);
    inline WspFloat4(u32 in_x, u32 in_y, u32 in_z, u32 in_w);
    inline WspFloat4(int in_x, int in_y, int in_z, int in_w);
    inline explicit WspFloat4(float value);
    inline explicit WspFloat4(u32 rgba);
    inline WspFloat4(const WspFloat4& data);

    // Getter -----------------------
    inline float GetMax() const;
    inline u32 ToRgba() const;
    inline u32 ToAbgr() const;
    inline WspFloat4 GetNormalizedFloat4() const;

    // Setter -----------------------
    inline void Normalize();
    inline void FromRgba(u32 rgba);
    inline void FromAbgr(u32 abgr);

    // Operators --------------------
    inline WspFloat4& operator=(const WspFloat4& other);
    inline WspFloat4& operator=(int rhs);

    inline bool operator==(const WspFloat4& other) const;
    inline bool operator>(const WspFloat4& rhs) const;
    inline bool operator<(const WspFloat4& rhs) const;
    inline bool operator>=(const WspFloat4& rhs) const;
    inline bool operator<=(const WspFloat4& rhs) const;

    inline WspFloat4 operator+ (const WspFloat4 &p) const;
    inline WspFloat4& operator+= (const WspFloat4 &p);

    inline WspFloat4 operator- (const WspFloat4 &p) const;
    inline WspFloat4& operator-= (const WspFloat4 &p);

    inline WspFloat4 operator* (const WspFloat4 &p) const;
    inline WspFloat4& operator*= (const WspFloat4 &p);
    inline WspFloat4 operator* (float value) const;
    inline WspFloat4& operator*= (float value);

    inline WspFloat4 operator/ (const WspFloat4 &p) const;
    inline WspFloat4& operator/= (const WspFloat4 &p);
};

// Constructor, Destructor ----------------------------------
inline WspFloat4::WspFloat4()
    : x(0)
    , y(0)
    , z(0)
    , w(0)
{
}
inline WspFloat4::WspFloat4(float in_x, float in_y, float in_z, float in_w){
    x = (float)in_x, y = (float)in_y, z = (float)in_z, w = (float)in_w;
}
inline WspFloat4::WspFloat4(uchar in_x, uchar in_y, uchar in_z, uchar in_w){
    x = (float)in_x, y = (float)in_y, z = (float)in_z, w = (float)in_w;
}
inline WspFloat4::WspFloat4(u32 in_x, u32 in_y, u32 in_z, u32 in_w){
    x = (float)in_x, y = (float)in_y, z = (float)in_z, w = (float)in_w;
}
inline WspFloat4::WspFloat4(int in_x, int in_y, int in_z, int in_w){
    x = (float)in_x, y = (float)in_y, z = (float)in_z, w = (float)in_w;
}
inline WspFloat4::WspFloat4(float value)
    : x(value)
    , y(value)
    , z(value)
    , w(value)
{
}
inline WspFloat4::WspFloat4(u32 rgba){
    w = (rgba & 0xff) / 255.0f;
    z = ((rgba>>8) & 0xff) / 255.0f;
    y = ((rgba>>16) & 0xff) / 255.0f;
    x = ((rgba>>24) & 0xff) / 255.0f;
}
inline WspFloat4::WspFloat4(const WspFloat4 &data)
    : x(data.x)
    , y(data.y)
    , z(data.z)
    , w(data.w)
{
}

// Getter ----------------------------------------------------
inline float WspFloat4::GetMax() const{
    float max = x;
    if(max<y){ max=y; } if(max<z){ max=z; } if(max<w){ max=w; }
    return max;
}
inline WspFloat4 WspFloat4::GetNormalizedFloat4() const{
    WspFloat4 res(*this);
    res.Normalize();
    return res;
}
inline u32 WspFloat4::ToRgba() const{
    WspFloat4 f = GetNormalizedFloat4();
    return (u32(f.x*255)<<24) | (u32(f.y*255)<<16) | (u32(f.z*255)<<8) | u32(f.w*255);
}
inline u32 WspFloat4::ToAbgr() const{
    WspFloat4 f = GetNormalizedFloat4();
    return (u32(f.w*255)<<24) | (u32(f.z*255)<<16) | (u32(f.y*255)<<8) | u32(f.x*255);
}

// Setter ----------------------------------------------------
inline void WspFloat4::Normalize(){
    float max = GetMax();
    x/=max; y/=max; z/=max; w/=max;
}
inline void WspFloat4::FromRgba(u32 rgba){
    w = (rgba & 0xff) / 255.0f;
    z = ((rgba>>8) & 0xff) / 255.0f;
    y = ((rgba>>16) & 0xff) / 255.0f;
    x = ((rgba>>24) & 0xff) / 255.0f;
}
inline void WspFloat4::FromAbgr(u32 abgr){
    x = (abgr & 0xff) / 255.0f;
    y = ((abgr>>8) & 0xff) / 255.0f;
    z = ((abgr>>16) & 0xff) / 255.0f;
    w = ((abgr>>24) & 0xff) / 255.0f;
}


// Operators -------------------------------------------------
inline WspFloat4& WspFloat4::operator=(const WspFloat4& other){
    if(this != &other){
        x = other.x; y = other.y; z = other.z; w = other.w;
    }
    return *this;
}
inline WspFloat4& WspFloat4::operator=(int rhs){
    x=(float)rhs, x=(float)rhs, z=(float)rhs, w=(float)rhs;
    return *this;
}


inline bool WspFloat4::operator==(const WspFloat4 &other) const{
    if( x==other.x && x==other.y && z==other.z && w==other.w){
        return true;
    }
    return false;
}
inline bool WspFloat4::operator>(const WspFloat4& rhs) const{
    float av_l = (x+y+z+w)/4.0;
    float av_r = (rhs.x + rhs.y + rhs.z + rhs.w)/4.0;
    if(av_l>av_r){
        return true;
    }
    return false;
}
inline bool WspFloat4::operator<(const WspFloat4& rhs) const{
    float av_l = (x+y+z+w)/4.0;
    float av_r = (rhs.x + rhs.y + rhs.z + rhs.w)/4.0;
    if(av_l<av_r){
        return true;
    }
    return false;
}
inline bool WspFloat4::operator>=(const WspFloat4& rhs) const{
    float av_l = (x+y+z+w)/4.0;
    float av_r = (rhs.x + rhs.y + rhs.z + rhs.w)/4.0;
    if(av_l>=av_r){
        return true;
    }
    return false;
}
inline bool WspFloat4::operator<=(const WspFloat4& rhs) const{
    float av_l = (x+y+z+w)/4.0;
    float av_r = (rhs.x + rhs.y + rhs.z + rhs.w)/4.0;
    if(av_l<=av_r){
        return true;
    }
    return false;
}
inline WspFloat4 WspFloat4::operator+(const WspFloat4& data) const{
    WspFloat4 res(*this);
    res.x+=data.x; res.y+=data.y; res.z+=data.z; res.w+=data.w;
    return res;
}
inline WspFloat4& WspFloat4::operator+=(const WspFloat4& data){
    x+=data.x; y+=data.y; z+=data.z;
    return *this;
}
inline WspFloat4 WspFloat4::operator-(const WspFloat4& data) const{
    WspFloat4 res(*this);
    res.x-=data.x; res.y-=data.y; res.z-=data.z; res.w-=data.w;
    return res;
}
inline WspFloat4& WspFloat4::operator-=(const WspFloat4& data){
    x-=data.x; y-=data.y; z-=data.z; w-=data.w;
    return *this;
}
inline WspFloat4 WspFloat4::operator*(const WspFloat4& data) const{
    WspFloat4 res(*this);
    res.x*=data.x; res.y*=data.y; res.z*=data.z; res.w*=data.w;
    return res;
}
inline WspFloat4& WspFloat4::operator*=(const WspFloat4& data){
    x*=data.x; y*=data.y; z*=data.z; w*=data.w;
    return *this;
}
inline WspFloat4 WspFloat4::operator*(float value) const{
    WspFloat4 res(*this);
    res.x*=(float)value; res.y*=(float)value; res.z*=(float)value; res.w*=(float)value;
    return res;
}
inline WspFloat4& WspFloat4::operator*=(float value){
    x*=(float)value; y*=(float)value; z*=(float)value; w*=(float)value;
    return *this;
}
inline WspFloat4 WspFloat4::operator/(const WspFloat4& data) const{
    WspFloat4 res(*this);
    if(data.x!=0){ res.x/=data.x; }
    if(data.y!=0){ res.y/=data.y; }
    if(data.z!=0){ res.z/=data.z; }
    if(data.w!=0){ res.w/=data.w; }
    return res;
}
inline WspFloat4& WspFloat4::operator/=(const WspFloat4& data){
    if(data.x!=0){ x/=data.x; }
    if(data.y!=0){ y/=data.y; }
    if(data.z!=0){ z/=data.z; }
    if(data.w!=0){ w/=data.w; }
    return *this;
}

// I/O --------------------------------------------------------

inline std::ostream& operator<<(std::ostream& lhs, const WspFloat4 &rhs)
{
    lhs<<rhs.x<<","<<rhs.y<<","<<rhs.z<<","<<rhs.w;
    return lhs;
}
inline std::ostream& operator<<(std::ostream& lhs, const WspFloat4 *rhs)
{
    lhs<<*rhs;
    return lhs;
}
#endif