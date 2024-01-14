/**
* @file st-stdparam.c
* 
*/

#include "st-stdparam.h"

const char* WSP_GetStdParamTypeAsString( WSP_StdParamType type )
{
    switch(type)
    {
    case WSP_STD_PARAM_UNDEFINED      : return "WSP_STD_PARAM_UNDEFINED";
    case WSP_STD_PARAM_INTEGER        : return "WSP_STD_PARAM_INTEGER";
    case WSP_STD_PARAM_FLOAT          : return "WSP_STD_PARAM_FLOAT";
    case WSP_STD_PARAM_BOOLEAN        : return "WSP_STD_PARAM_BOOLEAN";
    case WSP_STD_PARAM_INPUT_FILEPATH : return "WSP_STD_PARAM_INPUT_FILEPATH";
    case WSP_STD_PARAM_OUTPUT_FILEPATH: return "WSP_STD_PARAM_OUTPUT_FILEPATH";
    case WSP_STD_PARAM_POINTER_LIST   : return "WSP_STD_PARAM_POINTER_LIST";
    default:  return "UNKNOWN";
    }
}
