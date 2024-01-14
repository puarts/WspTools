//! fn-imgproc_gpu-gpl.h
// @author Junichi Nishikata

#ifndef _WspFnImgProcGpu_H_
#define _WspFnImgProcGpu_H_

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include <wsp/common/_define_commoncore.h>
#endif

#ifdef USE_CUDA

namespace wsp{ namespace img{
    wsp::State CudaGaussianBlur(
        double *o_dst, 
        const double *in_src, 
        u32 width, u32 height, u32 kernel_width, u32 kernel_height, 
        double sigma1, 
        double sigma2, 
        int rowBorderType, 
        int columnBorderType, 
        Stream& stream
    );
}

#endif

#endif