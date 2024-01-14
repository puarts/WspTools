/**
 * @file _result.c
 * 
 */

#include <string.h>

#ifndef __WSP_COMMONCORE_RESULT_H__
#include "result.h"
#endif

WSP_DEFINE_C_RESULT( WSP_ResultSuccess,   0x00000000, WSP_RESULT_LEVEL_SUCCESS );

bool WSP_ResultInclude( struct WSP_Result lhs, struct WSP_Result rhs )
{
    return rhs.range_min_bits <= lhs.range_min_bits 
        && lhs.range_max_bits <= rhs.range_max_bits;
}


bool WSP_ResultEqual( struct WSP_Result lhs, struct WSP_Result rhs )
{
    return lhs.range_min_bits == rhs.range_min_bits 
        && rhs.range_max_bits == lhs.range_max_bits;
}

