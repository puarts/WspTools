/**
 * @file _math_types.h
 * 
 */

#ifndef __WSP_MATHCORE_MATH_TYPES_H__
#define __WSP_MATHCORE_MATH_TYPES_H__

#ifndef __WSP_COMMONCORE_CTYPE_H__
#include <wsp/common/_ctypes.h>
#endif

typedef struct{
    float x, y, z;
}WSP_Vector3f;

typedef enum WSP_MathState{
    WSP_MATH_STATE_SUCCESS,
    WSP_MATH_STATE_FAILURE,
    WSP_MATH_STATE_INVALID_PARAMETER,
    WSP_MATH_STATE_PARALLEL
} WSP_MathState;


typedef enum WSP_RotationOrder{
    WSP_ROTATION_ORDER_XYZ, 
    WSP_ROTATION_ORDER_YZX, 
    WSP_ROTATION_ORDER_ZXY, 
    WSP_ROTATION_ORDER_XZY, 
    WSP_ROTATION_ORDER_YXZ, 
    WSP_ROTATION_ORDER_ZYX 
}WSP_RotationOrder;

typedef enum WSP_AxisType{
    WSP_AXIS_X,   WSP_AXIS_Y,  WSP_AXIS_Z,
    WSP_AXIS_XY,  WSP_AXIS_XZ, WSP_AXIS_YZ,
    WSP_AXIS_XYZ, WSP_AXIS_NONE
}WSP_AxisType;

typedef enum WSP_FittingFunction{
    WSP_FITTING_FUNCTION_EQUIANGULAR_LINE,
    WSP_FITTING_FUNCTION_PARABOLA,
    WSP_FITTING_FUNCTION_UNDEFINED
}WSP_FittingFunction;

typedef enum WSP_CorrelationFunction{
    /** Sum of Absolute Difference */
    WSP_CORRELATION_FUNCTION_SAD, 
    /** Sum of Squared Difference */
    WSP_CORRELATION_FUNCTION_SSD,
    /** Cross Correlation */
    WSP_CORRELATION_FUNCTION_CC, 
    /** Normalized Cross Correlation */
    WSP_CORRELATION_FUNCTION_NCC, 
    /** Zero-Mean Normalized Cross Correlation */
    WSP_CORRELATION_FUNCTION_ZNCC 
}WSP_CorrelationFunction;


#ifdef __cplusplus
namespace wsp{ namespace math{
    typedef WSP_Vector3f            Vector3f;
    typedef WSP_MathState           State;
    typedef WSP_RotationOrder       RotationOrder;
    typedef WSP_AxisType            AxisType;
    typedef WSP_FittingFunction     FittingFunction;
    typedef WSP_CorrelationFunction CorrelationFunction;
}}
#endif

#endif
