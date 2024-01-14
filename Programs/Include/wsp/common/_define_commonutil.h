/**
 * @file _define_commonutil.h
 * 
 * @brief Base definition of commonutil module.
 */

#ifndef __WSP_COMMONUTIL_DEFINE_COMMONUTIL_H__
#define __WSP_COMMONUTIL_DEFINE_COMMONUTIL_H__

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include "_define_commoncore.h"
#endif
#ifndef __WSP_COMMONCORE_CTYPE_H__
#include "_ctypes.h"
#endif
#include "common_types.h"

#include "fn-io.h"

#define BEGIN_WSP_CORE_NAMESPACE namespace wsp{
#define END_WSP_CORE_NAMESPACE }

/**
 * @brief Collection of functions and classes for WspLib module
 */
namespace wsp{
    enum DataType{
        DATATYPE_8U=0,
        DATATYPE_8S=1,
        DATATYPE_16U=2,
        DATATYPE_16S=3,
        DATATYPE_32U=4,
        DATATYPE_32S=5,
        DATATYPE_32F=6,
        DATATYPE_64F=7,
        DATATYPE_UNKNOWN,
        DATATYPE_DEFAULT
    };

    static const int kNumOfDataType = 8;
}

#endif
