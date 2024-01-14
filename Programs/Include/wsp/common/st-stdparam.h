/**
* @file st-stdparam.h
* 
*/

#ifndef __WSP_COMMONCORE_STRUCT_STDPARAM_H__
#define __WSP_COMMONCORE_STRUCT_STDPARAM_H__

#ifdef __cplusplus
#include <stdio.h>
#include <string.h>
#endif

#ifndef __WSP_COMMONCORE_CTYPE_H__
#include "_ctypes.h"
#endif

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include "_define_commoncore.h"
#endif

typedef enum WSP_StdParamType{
    WSP_STD_PARAM_UNDEFINED,
    WSP_STD_PARAM_INTEGER,
    WSP_STD_PARAM_FLOAT,
    WSP_STD_PARAM_BOOLEAN,
    WSP_STD_PARAM_INPUT_FILEPATH,
    WSP_STD_PARAM_OUTPUT_FILEPATH,
    WSP_STD_PARAM_POINTER_LIST
}WSP_StdParamType;

#ifdef __cplusplus
extern "C"{
#endif
WSP_DLL_EXPORT const char* WSP_GetStdParamTypeAsString( WSP_StdParamType type );
#ifdef __cplusplus
}
#endif

// param data struct
#define INIT_STD_PARAM \
    int_value(0)

struct WSP_StdParam
{
    char   name[MAX_LENGTH_OF_NAME];
    int    id;
    WSP_StdParamType type;
    char   file_path[MAX_LENGTH_OF_PATH];
    union{
        double double_value;
        int    int_value;
        bool   bool_value;
    };
        
#ifdef __cplusplus
    inline WSP_StdParam()
        :INIT_STD_PARAM
    {
        memset(file_path, 0, MAX_LENGTH_OF_PATH);
    }
    inline explicit WSP_StdParam(int value)   
        :INIT_STD_PARAM
    { 
        type = WSP_STD_PARAM_INTEGER;
        int_value    = value; 
    }
    inline explicit WSP_StdParam(double value)
        :INIT_STD_PARAM
    { 
        type = WSP_STD_PARAM_FLOAT;
        double_value = value; 
    }
    inline explicit WSP_StdParam(bool value)  
        :INIT_STD_PARAM
    { 
        type = WSP_STD_PARAM_BOOLEAN; 
        bool_value   = value; 
    }
    inline explicit WSP_StdParam(const char *in_file_path)
        :INIT_STD_PARAM
    { 
        type = WSP_STD_PARAM_INPUT_FILEPATH; 
        sprintf(file_path, in_file_path); 
    }
#endif
};

#ifdef __cplusplus
namespace wsp{
#endif
    typedef enum   WSP_StdParamType StdParamType;
    typedef struct WSP_StdParam     StdParam;
#ifdef __cplusplus
}
#endif


#endif
