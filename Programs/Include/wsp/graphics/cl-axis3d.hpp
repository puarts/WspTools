/**
 * @file cl-axis.h
 * 
 */

#ifndef WSP_GRAPHIC_CLASS_AXIS3D_H_
#define WSP_GRAPHIC_CLASS_AXIS3D_H_

#include <wsp/common/tpl_cl-vector3.h>
#include "_define_graphic.h"

typedef struct{
    WspVec3F x, y, z;
}WspAxis3;

void WspRotateByAxis(
    float *io_axisx_x, float *io_axisx_y, float *io_axisx_z,
    float *io_axisy_x, float *io_axisy_y, float *io_axisy_z,
    float *io_axisz_x, float *io_axisz_y, float *io_axisz_z
)
{
}


#endif
