/**
* @file common_types.h
* 
*/

#ifndef __WSP_COMMONCORE_COMMON_TYPES_H__
#define __WSP_COMMONCORE_COMMON_TYPES_H__

#ifndef __WSP_COMMONCORE_CTYPE_H__
#include "_ctypes.h"
#endif

/** @brief simple vector struct */
struct WspVec2F{
    float x, y;
    WspVec2F()
        : x(0.0f), y(0.0f)
    {
    }
    WspVec2F(float input_x, float input_y)
        : x(input_x), y(input_y)
    {
    }
};

struct WspVec3F{
    float x, y, z;
    WspVec3F()
        : x(0.0f), y(0.0f), z(0.0f)
    {
    }
    WspVec3F(float input_x, float input_y, float input_z)
        : x(input_x), y(input_y), z(input_z)
    {
    }
};

struct WspVec4F{
    float x, y, z, w;
    WspVec4F()
        : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
    {
    }
    WspVec4F(float input_x, float input_y, float input_z, float input_w)
        : x(input_x), y(input_y), z(input_z), w(input_w)
    {
    }
};

struct WspIntVec4 {
    int x, y, z, w;
    WspIntVec4()
        : x(0), y(0), z(0), w(0)
    {
    }
    WspIntVec4(int input_x, int input_y, int input_z, int input_w)
        : x(input_x), y(input_y), z(input_z), w(input_w)
    {
    }
};

/** @brief simple color struct */
struct WspColor3F{
    float r, g, b;
    WspColor3F()
        : r(0.0f), g(0.0f), b(0.0f)
    {
    }
    WspColor3F(float input_r, float input_g, float input_b)
        : r(input_r), g(input_g), b(input_b)
    {
    }
};

struct WspColor4F {
    float r, g, b, a;
    WspColor4F()
        : r(0.0f), g(0.0f), b(0.0f), a(0.0f)
    {
    }
    WspColor4F(float input_r, float input_g, float input_b, float input_a)
        : r(input_r), g(input_g), b(input_b), a(input_a)
    {
    }
};

struct PairLabel{ int label[2]; };
struct PairIndex{ int index1, index2; };


typedef enum WSP_State{
    WSP_STATE_SUCCESS,
    WSP_STATE_FAILURE,
    WSP_STATE_CANCELED,
    WSP_STATE_INSUFFICIENT_MEMORY,
    WSP_STATE_INVALID_ARGUMENT,
    WSP_STATE_INVALID_PARAMETER,
    WSP_STATE_NOT_FOUND,
    WSP_STATE_NOT_IMPLEMENTED,
    WSP_STATE_UNSUPPORTED_TYPE,
    WSP_STATE_UNKNOWN_PARAMETER,
    WSP_STATE_END_OF_FILE,
    WSP_STATE_NULL_DATA
} WSP_State;


enum WSP_DebugMode{
    WSP_DEBUGMODE_OFF = 0, 
    WSP_DEBUGMODE_LEVEL1 = 1, //! output data for debug
    WSP_DEBUGMODE_LEVEL2 = 2 //! print variables
};

enum WSP_PNMFormat{
    WSP_PNM_FORMAT_BITMAP,
    WSP_PNM_FORMAT_GRAYMAP,
    WSP_PNM_FORMAT_PIXMAP
};

#ifdef __cplusplus
inline bool operator==(const WspVec4F& a, const WspVec4F& b)
{
    return a.x == b.x &&
        a.y == b.y &&
        a.z == b.z &&
        a.w == b.w;
}

inline bool operator==(const WspIntVec4& a, const WspIntVec4& b)
{
    return a.x == b.x &&
        a.y == b.y &&
        a.z == b.z &&
        a.w == b.w;
}

inline bool operator==(const WspVec3F& a, const WspVec3F& b)
{
    return a.x == b.x &&
        a.y == b.y &&
        a.z == b.z;
}

inline bool operator==(const WspVec2F& a, const WspVec2F& b)
{
    return a.x == b.x &&
        a.y == b.y;
}

inline bool operator==(const WspColor3F& a, const WspColor3F& b)
{
    return a.r == b.r &&
        a.g == b.g &&
        a.b == b.b;
}

inline bool operator==(const WspColor4F& a, const WspColor4F& b)
{
    return a.r == b.r &&
        a.g == b.g &&
        a.b == b.b &&
        a.a == b.a;
}
#endif

// type define
#ifdef __cplusplus
namespace wsp{
    typedef WSP_State State;

    typedef enum WSP_State State;
    typedef enum WSP_DebugMode DebugMode;
    typedef enum WSP_PNMFormat PNMFormat;
#endif

    

#ifdef __cplusplus
    extern "C" {
#endif
        static const long long DBL_INF  = 0x7ff0000000000000;
        static const long long DBL_MINF = 0xfff0000000000000;
        static const long long DBL_NAN  = 0xfff8000000000000;

        static const int FLT_INF  = 0x7f800000;
        static const int FLT_MINF = 0xff800000;
        static const int FLT_NAN  = 0xffc00000;
#ifdef __cplusplus
    }
#endif


    static const uchar kRed[]  = {255, 0, 0};
    static const uchar kGreen[]= {0, 255, 0};
    static const uchar kBlue[] = {0, 0, 255};

    static const wchar kRedW[]  = {255, 0, 0};
    static const wchar kGreenW[]= {0, 255, 0};
    static const wchar kBlueW[] = {0, 0, 255};

#ifdef __cplusplus
}
#endif

#endif
