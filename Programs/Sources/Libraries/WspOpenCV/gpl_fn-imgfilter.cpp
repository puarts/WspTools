/**
 * @file gpl_fn-imgfilter.cpp
 * @author Junichi Nishikata
 */


#include "gpl_fn-imgfilter.h"

#ifdef USE_OPENCV
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

cv::Mat wsp::img::cvMakeGaborKernel(
    int kernel_size, 
    double sig, 
    double th, 
    double lm, 
    double ps
)
{
    int half_kernel_size = (kernel_size-1)/2;
    double theta = th * M_PI / 180;
    double psi = ps * M_PI / 180;
    double del = 2.0/(kernel_size-1);
    double lmbd = lm;
    double sigma = sig/kernel_size;
    double x_theta;
    double y_theta;
    cv::Mat kernel(kernel_size, kernel_size, CV_32F);
    for (int y=-half_kernel_size; y<=half_kernel_size; y++)
    {
        for (int x=-half_kernel_size; x<=half_kernel_size; x++)
        {
            x_theta = x*del*cos(theta)+y*del*sin(theta);
            y_theta = -x*del*sin(theta)+y*del*cos(theta);
            double gauss = (float)exp(-0.5*(pow(x_theta,2) + pow(y_theta,2))/pow(sigma,2));
            kernel.at<float>( half_kernel_size + y, half_kernel_size + x ) 
                = gauss * cos(2*CV_PI*x_theta/lmbd + psi);
            printf("%d gauss: %f\n", y*kernel_size + x, gauss);
        }
    }
    return kernel;
}
#endif