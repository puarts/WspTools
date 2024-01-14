/**
 * @file _ctypes.h
 * 
 * @brief C type definition.
 */

#ifndef __WSP_COMMONCORE_CTYPE_H__
#define __WSP_COMMONCORE_CTYPE_H__



#ifdef __cplusplus
    #ifndef _BOOL
        #define _BOOL
    #endif
#else
    #ifndef _BOOL
        typedef unsigned char bool;
        #if !defined(true) && !defined(false)
            enum {false, true};
        #endif
    #endif /* _BOOL */
#endif

typedef unsigned int uint;
typedef unsigned int u32;
typedef unsigned char uchar;
//typedef unsigned char byte;
typedef unsigned short ushort;
typedef short wchar;

/** @brief 8 bit signed */
typedef signed char int8;

/** @brief 8 bit signed */
typedef signed char s8;

/** @brief 8 bit unsigned */
typedef unsigned char uint8;

/** @brief 8 bit unsigned */
typedef unsigned char u8;

/** @brief 16 bit signed */
typedef short int16; 

/** @brief 16 bit signed */
typedef short s16;

/** @brief 16 bit unsigned */
typedef unsigned short uint16;

/** @brief 16 bit unsigned */
typedef unsigned short u16;

/** @brief 32 bit signed */
typedef int int32; 

/** @brief 32 bit signed */
typedef int s32;

/** @brief 32 bit unsigned */
typedef unsigned int uint32;

/** @brief 32 bit unsigned */
typedef unsigned int u32;

typedef float f32;

typedef double f64;

#if defined(_WIN32)
/** @brief 64 bit signed */
typedef __int64 int64;

/** @brief 64 bit signed */
typedef __int64 s64;

/** @brief 64 bit unsigned */
typedef unsigned __int64 uint64;

/** @brief 64 bit unsigned */
typedef unsigned __int64 u64;
#else
/** @brief 64 bit signed */
typedef long long int64;

/** @brief 64 bit signed */
typedef long long s64;

/** @brief 64 bit unsigned */
typedef unsigned long long uint64;

/** @brief 64 bit unsigned */
typedef unsigned long long u64; 
#endif
/** @brief 32 bit complex number */
typedef float complex32[2];

/** @brief 64 bit complex number */
typedef double complex64[2];

typedef float mat4x4[16];
typedef float mat3x3[9];

#endif
