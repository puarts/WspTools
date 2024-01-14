/**
 * @file result.h
 * 
 */

#ifndef __WSP_COMMONCORE_RESULT_H__
#define __WSP_COMMONCORE_RESULT_H__

#include <string>

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include "_define_commoncore.h"
#endif
#ifndef __WSP_COMMONCORE_CTYPE_H__
#include "_ctypes.h"
#endif

/* ======================================
            struct definition
   ======================================*/

enum WSP_ResultLevel{
    WSP_RESULT_LEVEL_SUCCESS,
    WSP_RESULT_LEVEL_USAGE,
    WSP_RESULT_LEVEL_STATUS,
    WSP_RESULT_LEVEL_FATAL
};

#ifdef __cplusplus
inline const char* WSP_GetResultLevelAsString( WSP_ResultLevel level )
{
    switch( level )
    {
    case WSP_RESULT_LEVEL_SUCCESS: return "SUCCESS";
    case WSP_RESULT_LEVEL_USAGE: return "USAGE";
    case WSP_RESULT_LEVEL_STATUS: return "STATUS";
    case WSP_RESULT_LEVEL_FATAL: return "FATAL";
    }
    return "UNDEFINED LEVEL";
}
#endif

struct WSP_Result{
    u32  range_min_bits;
    u32  range_max_bits;
    char result_string[32];
    u8   level;

#ifdef __cplusplus
    inline bool IsSuccess(){ return level==WSP_RESULT_LEVEL_SUCCESS; }
    inline bool IsFailure(){ return !IsSuccess(); }
    inline bool operator<=(const struct WSP_Result& rhs);
    inline bool operator==(const struct WSP_Result& rhs);
    inline bool operator!=(const struct WSP_Result& rhs);
#endif
};

typedef struct WSP_Result WSP_Result;

#ifdef __cplusplus
namespace wsp{
    typedef struct WSP_Result Result;
}
#endif



/* ======================================
     instance declaration and definition
   ======================================*/

/* C result macro */
#define WSP_DECLARE_C_RESULT(in_result_name) \
    WSP_DLL_EXPORT struct WSP_Result in_result_name();

#define WSP_DEFINE_C_RESULT_RANGE( in_result_name, in_min_bits, in_max_bits, in_level )  \
    struct WSP_Result in_result_name (){                         \
        struct WSP_Result result;                                        \
        result.range_min_bits = in_min_bits;                             \
        result.range_max_bits = in_max_bits;                             \
        result.level          = in_level;                              \
        strncpy( result.result_string, #in_result_name, 32 );          \
        return result;                                                   \
    }                                                                    

#define WSP_DEFINE_C_RESULT( in_result_name, in_bits, in_level ) \
    WSP_DEFINE_C_RESULT_RANGE( in_result_name, in_bits, in_bits, in_level )

/* C++ result macro */
#ifdef __cplusplus
    #define WSP_CPP_RESULT_FROM_C_RESULT( in_wrapper_name, in_result_name ) \
        inline struct WSP_Result in_wrapper_name ()         \
        {                                                    \
            return in_result_name ();                \
        }

    #define WSP_DEFINE_CPP_RESULT_RANGE( in_result_name, in_min_bits, in_max_bits, in_level ) \
        inline WSP_DEFINE_C_RESULT_RANGE( in_result_name, in_min_bits, in_max_bits, in_level )

    #define WSP_DEFINE_CPP_RESULT( in_result_name, in_bits, in_level ) \
        inline WSP_DEFINE_C_RESULT( in_result_name, in_bits, in_level )

#endif


/* ======================================
     declare C result
   ======================================*/

#ifdef __cplusplus
extern "C"{
#endif
    WSP_DLL_EXPORT bool WSP_ResultInclude( struct WSP_Result lhs, struct WSP_Result rhs );
    WSP_DLL_EXPORT bool WSP_ResultEqual( struct WSP_Result lhs, struct WSP_Result rhs );


    WSP_DECLARE_C_RESULT( WSP_ResultSuccess );

#ifdef __cplusplus
}
#endif


/* ======================================
     define C++ wrapper for C result
   ======================================*/
#ifdef __cplusplus
inline bool WSP_Result::operator<=(const struct WSP_Result& rhs )
{
    return WSP_ResultInclude( *this, rhs );
}

inline bool WSP_Result::operator==(const struct WSP_Result& rhs )
{
    return WSP_ResultEqual( *this, rhs );
}

inline bool WSP_Result::operator!=(const struct WSP_Result& rhs )
{
    return !WSP_ResultEqual( *this, rhs );
}

namespace wsp{

    inline bool ResultInclude( const struct WSP_Result& lhs, const struct WSP_Result& rhs )
    {
        return WSP_ResultInclude( lhs, rhs );
    }
    inline bool ResultEqual( const struct WSP_Result& lhs, const struct WSP_Result& rhs )
    {
        return WSP_ResultEqual( lhs, rhs );
    }

    WSP_CPP_RESULT_FROM_C_RESULT( ResultSuccess, WSP_ResultSuccess );

}
#endif

#define WSP_PRINT_RESULT(result) \
    WSP_Printf( \
        "Result failed - %s\n"\
        "    (%08x) %s: %s\n"\
        , #result\
        , result.range_min_bits\
        , result.result_string\
        , WSP_GetResultLevelAsString((WSP_ResultLevel)result.level) );\

#define WSP_THROW_RESULT(result) return result

#define WSP_RETURN_IF_FAILED( result ) \
    do{\
        WSP_Result WSP_RETURN_IF_FAILED_result = result;\
        if( WSP_RETURN_IF_FAILED_result.level!=WSP_RESULT_LEVEL_SUCCESS ){ \
            WSP_PRINT_RESULT(WSP_RETURN_IF_FAILED_result);\
            WSP_THROW_RESULT( result ); \
        }\
    }while(0)


#endif
