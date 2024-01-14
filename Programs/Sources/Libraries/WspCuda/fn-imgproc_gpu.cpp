// @file fn-imgprocgpu.cpp
// @author Junichi Nishikata

#include "fn-imgproc_gpu-gpl.h"

#ifdef USE_CUDA
Ptr<FilterEngine_GPU> cv::gpu::createGaussianFilter_GPU(int type, Size ksize, double sigma1, double sigma2, int rowBorderType, int columnBorderType)
{
    int depth = CV_MAT_DEPTH(type);

    if (sigma2 <= 0)
        sigma2 = sigma1;

    // automatic detection of kernel size from sigma
    if (ksize.width <= 0 && sigma1 > 0)
        ksize.width = cvRound(sigma1 * (depth == CV_8U ? 3 : 4)*2 + 1) | 1;
    if (ksize.height <= 0 && sigma2 > 0)
        ksize.height = cvRound(sigma2 * (depth == CV_8U ? 3 : 4)*2 + 1) | 1;

    CV_Assert( ksize.width > 0 && ksize.width % 2 == 1 && ksize.height > 0 && ksize.height % 2 == 1 );

    sigma1 = std::max(sigma1, 0.0);
    sigma2 = std::max(sigma2, 0.0);

    Mat kx = getGaussianKernel( ksize.width, sigma1, std::max(depth, CV_32F) );
    Mat ky;
    if( ksize.height == ksize.width && std::abs(sigma1 - sigma2) < DBL_EPSILON )
        ky = kx;
    else
        ky = getGaussianKernel( ksize.height, sigma2, std::max(depth, CV_32F) );

    return createSeparableLinearFilter_GPU(type, type, kx, ky, Point(-1,-1), rowBorderType, columnBorderType);
}

wsp::State wsp::img::CudaGaussianBlur(
    double *o_dst, 
    const double *in_src, 
    u32 width, u32 height, u32 kernel_width, u32 kernel_height, 
    double sigma1, 
    double sigma2, 
    int rowBorderType, 
    int columnBorderType, 
    Stream& stream
)
{
    if (kernel_width == 1 && kernel_height == 1){
        
        return WspStd::Failure;
    }

    
    Ptr<FilterEngine_GPU> f = createGaussianFilter_GPU(src.type(), ksize, sigma1, sigma2, rowBorderType, columnBorderType);
    f->apply(src, dst, Rect(0, 0, src.cols, src.rows), stream);
    return WSP_STATE_SUCCESS;
}

#endif