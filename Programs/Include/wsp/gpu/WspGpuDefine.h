//! WspGpuDefine.h
// @author Jun-ichi Nishikata

#ifndef _WspGpuDefine_H_
#define _WspGpuDefine_H_

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include <wsp/commoncore/_define_commoncore.h>
#endif

#include <cutil_inline.h>    // includes cuda.h and cuda_runtime_api.h

#ifdef _USRDLL
    #ifdef _DEBUG
        #pragma comment(lib, "WspCommonUtil-d.lib")
    #else
        #pragma comment(lib, "WspCommonUtil.lib")
    #endif
#endif

#define BLOCK_DIM 16
#define CLAMP_TO_EDGE 1

namespace wsp{
namespace gpu{
}
}


#endif