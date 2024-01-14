
#include <WspGpu/WspGpuDefine.h>



// RGBA version
// reads from 32-bit u32 array holding 8-bit RGBA

// convert floating point rgba color to 32-bit integer
inline __device__ u32 WspFnGpu::RgbaFloatToInt(float4 rgba)
{
    rgba.x = __saturatef(rgba.x);   // clamp to [0.0, 1.0]
    rgba.y = __saturatef(rgba.y);
    rgba.z = __saturatef(rgba.z);
    rgba.w = __saturatef(rgba.w);
    return (u32(rgba.w*255)<<24) | (u32(rgba.z*255)<<16) | (u32(rgba.y*255)<<8) | u32(rgba.x*255);
}

// convert from 32-bit int to float4
inline __device__ float4 WspFnGpu::RgbaIntToFloat(u32 c)
{
    float4 rgba;
    rgba.x = (c & 0xff) / 255.0f;
    rgba.y = ((c>>8) & 0xff) / 255.0f;
    rgba.z = ((c>>16) & 0xff) / 255.0f;
    rgba.w = ((c>>24) & 0xff) / 255.0f;
    return rgba;
}