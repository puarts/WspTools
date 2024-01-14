/**
 * @file fn-debug.h
 * 
 * @brief functions which provide access to debug functionality.
 */

#ifndef __WSP_COMMONCORE_FUNCTION_DEBUG_H__
#define __WSP_COMMONCORE_FUNCTION_DEBUG_H__

#include "fn-io.h"
#include "result.h"

#include <stdio.h>
#include <stdlib.h>

#define WSP_UNUSED(var) (void)(var)

#ifdef __cplusplus
extern "C"{
#endif
    WSP_DLL_EXPORT void WSP_PrintCallStack(void);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#define WSP_THROW( exception, message_fmt, ... ) \
do{\
    char message[MAX_LENGTH_OF_DISCRIPTION]; \
    sprintf(message, message_fmt "(%s, line:%d, %s)\n", __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__); \
    throw exception(message); \
} while (false)

#define WSP_REQUIRES_IF_THEN_THROW( condition, exception, message_fmt, ... ) \
do{\
if ((condition) == false)\
{\
    WSP_THROW(exception, message_fmt, __VA_ARGS__); \
}\
} while (false)
#endif

#define WSP_TERMINATE abort()

#ifdef __cplusplus
#ifdef _DEBUG
//        #include <assert.h>
//        #define WSP_TERMINATE_IF_FAILED(condition) \
//            do{ assert(condition); }while(false)
#define WSP_TERMINATE_IF_FAILED(condition) do{ if( (condition) == false ) { abort(); } }while(false)
#else
#define WSP_TERMINATE_IF_FAILED(condition) do{ if( (condition) == false ) { abort(); } }while(false)
#endif
#else
#define WSP_TERMINATE_IF_FAILED(condition) do{ if( (condition) == false ) { abort(); } }while(false)
#endif

/* =========================================== */
/*           FATAL declaration                 */
/* =========================================== */
#define WSP_FATAL( ... )\
    WSP_Printf("FATAL: ");\
    WSP_Printf(__VA_ARGS__); \
    WSP_Printf("\n");\
    WSP_PrintCallStack(); \
    WSP_Printf("\nThe process aborted\n"); \
    WSP_TERMINATE

#define WSP_FATAL_IF_FALSE( condition, ... )\
    do{\
        if (condition == false)\
        {\
            WSP_Printf(#condition ", "); WSP_FATAL(__VA_ARGS__); \
        }\
    } while (false)

#define WSP_FATAL_IF_RESULT_FAILED( result, message_fmt, ... )\
    do{\
        WSP_Result WSP_FATAL_IF_RESULT_FAILED_result = result; \
        if (WSP_FATAL_IF_RESULT_FAILED_result.level != WSP_RESULT_LEVEL_SUCCESS)\
        {\
            WSP_PRINT_RESULT(WSP_FATAL_IF_RESULT_FAILED_result); \
            WSP_FATAL(message_fmt, __VA_ARGS__); \
        }\
    } while (false)

#define WSP_FATAL_NOT_IMPLEMENTED WSP_FATAL( "Not implemented" )

#define WSP_FATAL_UNEXPECTED_DEFAULT WSP_FATAL( "Reached to unexpected default" )

/* =========================================== */
/*           ASSERT declaration                */
/* =========================================== */
#define WSP_REQUIRES( condition, message_fmt, ... ) \
    do{\
        if ((condition) == false)\
        {\
            char message[MAX_LENGTH_OF_DISCRIPTION]; \
            sprintf(message, "Precondition failed: " #condition ", " message_fmt, ##__VA_ARGS__); \
            WSP_Printf(message); \
            WSP_PrintCallStack(); \
            WSP_Printf("\nThe process aborted\n"); \
            WSP_TERMINATE_IF_FAILED(condition); \
        }\
    } while (false)

#define WSP_ENSURES( condition, message_fmt, ... ) \
    do{\
        if ((condition) == false)\
        {\
            char message[MAX_LENGTH_OF_DISCRIPTION]; \
            sprintf(message, "Postcondition failed: " #condition ", " message_fmt, ##__VA_ARGS__); \
            WSP_Printf(message); \
            WSP_PrintCallStack(); \
            WSP_Printf("\nThe process aborted\n"); \
            WSP_TERMINATE_IF_FAILED(condition); \
        }\
    } while (false)

#define WSP_REQUIRES_ARGUMENT(condition) WSP_REQUIRES(condition, "Invalid argument")
#define WSP_REQUIRES_NOT_NULL( ptr ) WSP_REQUIRES(ptr != nullptr, "pointer must not be null")
#define WSP_REQUIRES_IS_NULL( ptr ) WSP_REQUIRES(ptr == nullptr, "pointer must be null")

#ifdef _DEBUG

#define WSP_ASSERT( condition, ... ) \
    do{\
        if( (condition) == false )\
        {\
            char message[MAX_LENGTH_OF_DISCRIPTION]; \
            sprintf(message, "Assertion failed (%s, line:%d, %s): " #condition ", ", __FILE__, __LINE__, __FUNCTION__); \
            WSP_Printf(message); \
            WSP_Printf(__VA_ARGS__); \
            WSP_PrintCallStack();\
            WSP_Printf ( "\nThe process aborted\n" );\
            WSP_TERMINATE_IF_FAILED(condition); \
        }\
    }while(false)

#define WSP_ASSERT_NOT_NULL( ptr ) WSP_ASSERT( ptr!=NULL, #ptr " must not be NULL" )
#define WSP_ASSERT_IS_NULL( ptr ) WSP_ASSERT( ptr==NULL, #ptr " must be NULL" )
#define WSP_ASSERT_NOT_NEGATIVE_INT( value ) WSP_ASSERT( value>=0, #value " must not be negative: " #value " = %d", value )
#define WSP_ASSERT_NOT_NEGATIVE_FLOAT( value ) WSP_ASSERT( value>=0, #value " must not be negative: " #value " = %f", value )
#define WSP_ASSERT_POSITIVE_INT( value ) WSP_ASSERT( value>=0, #value " must be positive: " #value " = %d", value )
#define WSP_ASSERT_POSITIVE_FLOAT( value ) WSP_ASSERT( value>=0, #value " must be positive: " #value " = %f", value )
#define WSP_ASSERT_EQUAL( v1, v2 ) WSP_ASSERT( v1==v2, #v1 " must be equal to " #v2 ": " #v1 " = %d, " #v2" = %d", v1, v2 )
#define WSP_ASSERT_NOT_EQUAL( v1, v2 ) WSP_ASSERT( v1!=v2, #v1 " must not be equal to " #v2 ": " #v1 " = %d, " #v2" = %d", v1, v2 )
#define WSP_ASSERT_LARGER_THAN( v1, v2 ) WSP_ASSERT( v1>v2, #v1 " must larger than " #v2 ": " #v1 " = %d, " #v2" = %d", v1, v2 )
#define WSP_ASSERT_SMALLER_THAN( v1, v2 ) WSP_ASSERT( v1<v2, #v1 " must smaller than " #v2 ": " #v1 " = %d, " #v2" = %d", v1, v2 )
#define WSP_ASSERT_NOT_LARGER_THAN( v1, v2 ) WSP_ASSERT( v1<=v2, #v1 " must not larger than " #v2 ": " #v1 " = %d, " #v2" = %d", v1, v2 )
#define WSP_ASSERT_NOT_SMALLER_THAN( v1, v2 ) WSP_ASSERT( v1>=v2, #v1 " must not smaller than " #v2 ": " #v1 " = %d, " #v2" = %d", v1, v2 )
#define WSP_ASSERT_EXISTS( path ) WSP_ASSERT( ::WSP_Exists( path ), #path " is not found" )
#define WSP_ASSERT_ARRAY_INDEX_RANGE(value, min, count) WSP_ASSERT(min <= value && value < count, "index out of range");

#else

#define WSP_ASSERT( condition, ... ) WSP_UNUSED(condition)
#define WSP_ASSERT_NOT_NULL( ptr )                WSP_UNUSED(ptr)
#define WSP_ASSERT_NOT_NEGATIVE_INT( value )      WSP_UNUSED(value)
#define WSP_ASSERT_NOT_NEGATIVE_FLOAT( value )    WSP_UNUSED(value)
#define WSP_ASSERT_POSITIVE_INT( value )          WSP_UNUSED(value)
#define WSP_ASSERT_POSITIVE_FLOAT( value )        WSP_UNUSED(value)
#define WSP_ASSERT_EQUAL( v1, v2 )                (0)
#define WSP_ASSERT_NOT_EQUAL( v1, v2 )            (0)
#define WSP_ASSERT_LARGER_THAN( v1, v2 )          (0)
#define WSP_ASSERT_SMALLER_THAN( v1, v2 )         (0)
#define WSP_ASSERT_NOT_LARGER_THAN( v1, v2 )      (0)
#define WSP_ASSERT_NOT_SMALLER_THAN( v1, v2 )     (0)
#define WSP_ASSERT_EXISTS( path )                 WSP_UNUSED(path)
#define WSP_ASSERT_ARRAY_INDEX_RANGE(value, min, count) (0)

#endif

#endif
