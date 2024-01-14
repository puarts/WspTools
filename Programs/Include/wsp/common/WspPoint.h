//! WspPoint.h
// 

#ifndef __WspPoint_H__
#define __WspPoint_H__

#ifndef __WSP_COMMONUTIL_DEFINE_COMMONUTIL_H__
#include "_define_commonutil.h"
#endif

struct WspPoint2
{
    double x, y;

    WspPoint2():x(0), y(0){}
    WspPoint2(double in_x, double in_y) { x=in_x; y=in_y; }
    WspPoint2(int in_x, int in_y) { x=(double)in_x; y=(double)in_y; }

    WspPoint2 operator- ()        { return WspPoint2(-x, -y); }
    WspPoint2 operator+ (WspPoint2 pt) { return WspPoint2(x+pt.x, y+pt.y); }
    WspPoint2 operator- (WspPoint2 pt) { return WspPoint2(x-pt.x, y-pt.y); }
    bool  operator< (WspPoint2 pt) { return (x <  pt.x) && (y <  pt.y); }
    bool  operator<=(WspPoint2 pt) { return (x <= pt.x) && (y <= pt.y); }
    bool  operator> (WspPoint2 pt) { return (x >  pt.x) && (y >  pt.y); }
    bool  operator>=(WspPoint2 pt) { return (x >= pt.x) && (y >= pt.y); }
    bool  operator==(WspPoint2 pt) { return (x == pt.x) && (y == pt.y); }
    bool  operator!=(WspPoint2 pt) { return (x != pt.x) || (y != pt.y); }
};

struct WspPoint3
{
    double x, y, z;

    WspPoint3():x(0), y(0), z(0){}
    WspPoint3(double in_x, double in_y, double in_z) { x=in_x; y=in_y; z=in_z; }
    WspPoint3(int in_x, int in_y, int in_z) { x=(double)in_x; y=(double)in_y; z=(double)in_z; }

    WspPoint3 operator- ()        { return WspPoint3(-x, -y, -z); }
    WspPoint3 operator+ (WspPoint3 pt) { return WspPoint3(x+pt.x, y+pt.y, z+pt.z); }
    WspPoint3 operator- (WspPoint3 pt) { return WspPoint3(x-pt.x, y-pt.y, z-pt.z); }
    bool  operator< (WspPoint3 pt) { return (x <  pt.x) && (y <  pt.y) && (z <  pt.z); }
    bool  operator<=(WspPoint3 pt) { return (x <= pt.x) && (y <= pt.y) && (z <= pt.z); }
    bool  operator> (WspPoint3 pt) { return (x >  pt.x) && (y >  pt.y) && (z >  pt.z); }
    bool  operator>=(WspPoint3 pt) { return (x >= pt.x) && (y >= pt.y) && (z >= pt.z); }
    bool  operator==(WspPoint3 pt) { return (x == pt.x) && (y == pt.y) && (z == pt.z); }
    bool  operator!=(WspPoint3 pt) { return (x != pt.x) || (y != pt.y) || (z != pt.z); }
};


#endif