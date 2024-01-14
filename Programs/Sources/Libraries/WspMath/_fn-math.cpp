/**
 * @file _fn-math.cpp
 * 
 */

#ifndef M_PI
#define _USE_MATH_DEFINES
#endif
#include <math.h>

#include <wsp/common/_tpl_fn-array.hpp>
#include "_tpl_fn-math.h"

#include "_fn-math.h"

//
//
//void wsp::math::GetLinearFunctionByLeastSquaresMethod( 
//    double &o_gradient, 
//    double &o_intercept,
//    const double *in_posArrayX,
//    const double *in_posArrayY, 
//    u32 length
//){
//    u32 i;
//    double sum_x, sum_y, sum_xy, sum_xx;
//    double n = (double)length;
//    sum_x = sum_y = sum_xy = sum_xx = 0.0;
//    for(i=0; i<length; i++){
//        sum_x += in_posArrayX[i];
//        sum_y += in_posArrayY[i];
//        sum_xy += in_posArrayX[i] * in_posArrayY[i];
//        sum_xx += in_posArrayX[i] * in_posArrayY[i];
//    }
//    o_gradient = (n*sum_xy - sum_x*sum_y)/(n*sum_xx - sum_x*sum_x);
//    o_intercept = (sum_xx*sum_y - sum_xy*sum_x)/(n*sum_xx - sum_x*sum_x);
//}
//
//// ------------------------------------------------------------------
////! Rotation Conversion 
//// ------------------------------------------------------------------
//void wsp::math::EulerToMatrix(double *o_mat, const double in_eulerRot[3], wsp::math::RotationOrder order)
//{
//    double heading, attitude, bank;
//    double cosh, sinh, cosa, sina, cosb, sinb;
//    switch(order){
//    case wsp::math::ROTATION_ORDER_XYZ:
//        heading = in_eulerRot[0]; attitude = in_eulerRot[1]; bank = in_eulerRot[2];
//        break;
//    case wsp::math::ROTATION_ORDER_YZX:
//        heading = in_eulerRot[1]; attitude = in_eulerRot[2]; bank = in_eulerRot[0];
//        break;
//    case wsp::math::ROTATION_ORDER_ZXY:
//        heading = in_eulerRot[2]; attitude = in_eulerRot[0]; bank = in_eulerRot[1];
//        break;
//    case wsp::math::ROTATION_ORDER_XZY:
//        heading = in_eulerRot[0]; attitude = in_eulerRot[2]; bank = in_eulerRot[1];
//        break;
//    case wsp::math::ROTATION_ORDER_YXZ:
//        heading = in_eulerRot[1]; attitude = in_eulerRot[0]; bank = in_eulerRot[2];
//        break;
//    case wsp::math::ROTATION_ORDER_ZYX:
//        heading = in_eulerRot[2]; attitude = in_eulerRot[1]; bank = in_eulerRot[0];
//        break;
//    }
//
//    cosh = cos(heading);    sinh = sin(heading);
//    cosa = cos(attitude);   sina = sin(attitude);
//    cosb = cos(bank);       sinb = sin(bank);
//
//    o_mat[0]=cosh * cosa;   o_mat[1]=sinh*sinb-cosh*sina*cosb;   o_mat[2]=cosh*sina*sinb+sinh*cosb;
//    o_mat[3]=sina;          o_mat[4]=cosa*cosb;                  o_mat[5]=-cosa*sinb;
//    o_mat[6]=-sinh*cosa;    o_mat[7]=sinh*sina*cosb+cosh*sinb;   o_mat[8]=-sinh*sina*sinb+cosh*cosb;
//}
//
////! in_mat is 3x3 matrix represented by double[9] array
//void wsp::math::MatrixToEuler(double o_eulerRot[3], const double *in_mat, wsp::math::RotationOrder order)
//{
//    double heading, attitude, bank;
//    if(in_mat[3] > 0.998){
//        //! singularity at north pole
//        heading = atan2(in_mat[2], in_mat[8]);
//        attitude = M_PI/2.0;
//        bank = 0;
//    }else if(in_mat[3] < -0.998){
//        //! singularity at south pole
//        heading = atan2(in_mat[2], in_mat[8]);
//        attitude = -M_PI/2.0;
//        bank = 0;
//    }else{
//        heading = atan2(-in_mat[6], in_mat[0]);
//        bank = atan2(-in_mat[5], in_mat[4]);
//        attitude = asin(in_mat[3]);
//    }
//    // if it is in yzx rotation order, 
//    // Heading = rotation about y axis
//    // Attitude = rotation about z axis
//    // Bank = rotation about x axis
//    switch(order){
//    case wsp::math::ROTATION_ORDER_XYZ:
//        o_eulerRot[0] = heading; o_eulerRot[1] = attitude; o_eulerRot[2] = bank;
//        break;
//    case wsp::math::ROTATION_ORDER_YZX:
//        o_eulerRot[1] = heading; o_eulerRot[2] = attitude; o_eulerRot[0] = bank;
//        break;
//    case wsp::math::ROTATION_ORDER_ZXY:
//        o_eulerRot[2] = heading; o_eulerRot[0] = attitude; o_eulerRot[1] = bank;
//        break;
//    case wsp::math::ROTATION_ORDER_XZY:
//        o_eulerRot[0] = heading; o_eulerRot[2] = attitude; o_eulerRot[1] = bank;
//        break;
//    case wsp::math::ROTATION_ORDER_YXZ:
//        o_eulerRot[1] = heading; o_eulerRot[0] = attitude; o_eulerRot[2] = bank;
//        break;
//    case wsp::math::ROTATION_ORDER_ZYX:
//        o_eulerRot[2] = heading; o_eulerRot[1] = attitude; o_eulerRot[0] = bank;
//        break;
//    }
//}
//void wsp::math::InvertRotationMatrix(double *io_matrix, wsp::math::RotationOrder order, wsp::math::AxisType axis)
//{
//    double euler[3];
//    wsp::math::MatrixToEuler(euler, io_matrix, order);
//    if(axis==wsp::math::AXIS_X || axis==wsp::math::AXIS_XY || axis==wsp::math::AXIS_XZ || axis==wsp::math::AXIS_XYZ){
//        euler[0] = -euler[0];
//    }
//    if(axis==wsp::math::AXIS_Y || axis==wsp::math::AXIS_XY || axis==wsp::math::AXIS_YZ || axis==wsp::math::AXIS_XYZ){
//        euler[1] = -euler[1];
//    }
//    if(axis==wsp::math::AXIS_Z || axis==wsp::math::AXIS_XZ || axis==wsp::math::AXIS_YZ || axis==wsp::math::AXIS_XYZ){
//        euler[2] = -euler[2];
//    }
//    wsp::math::EulerToMatrix(io_matrix, euler, order);
//}
//
////! multiply 3x3 matrix
//void wsp::math::MultiplyMatrix(double *o_mat, double *in_mat1, double *in_mat2)
//{
//    double tmp_mat[9];
//    tmp_mat[0] = in_mat1[0]*in_mat2[0] + in_mat1[1]*in_mat2[3] + in_mat1[2]*in_mat2[6]; 
//    tmp_mat[1] = in_mat1[0]*in_mat2[1] + in_mat1[1]*in_mat2[4] + in_mat1[2]*in_mat2[7]; 
//    tmp_mat[2] = in_mat1[0]*in_mat2[2] + in_mat1[1]*in_mat2[5] + in_mat1[2]*in_mat2[8]; 
//
//    tmp_mat[3] = in_mat1[3]*in_mat2[0] + in_mat1[4]*in_mat2[3] + in_mat1[5]*in_mat2[6]; 
//    tmp_mat[4] = in_mat1[3]*in_mat2[1] + in_mat1[4]*in_mat2[4] + in_mat1[5]*in_mat2[7]; 
//    tmp_mat[5] = in_mat1[3]*in_mat2[2] + in_mat1[4]*in_mat2[5] + in_mat1[5]*in_mat2[8]; 
//
//    tmp_mat[6] = in_mat1[6]*in_mat2[0] + in_mat1[7]*in_mat2[3] + in_mat1[8]*in_mat2[6]; 
//    tmp_mat[7] = in_mat1[6]*in_mat2[1] + in_mat1[7]*in_mat2[4] + in_mat1[8]*in_mat2[7]; 
//    tmp_mat[8] = in_mat1[6]*in_mat2[2] + in_mat1[7]*in_mat2[5] + in_mat1[8]*in_mat2[8];
//
//    for(int i=0; i<9; i++){
//        o_mat[i] = tmp_mat[i];
//    }
//}
//
//
//void wsp::math::ApplyHannWindow_Complex(complex64 *io_data, int length)
//{
//    double x, n, M;
//    M = (double)floor(((double)length-1)/2.0);
//    for(int i=0; i<length; i++){
//        //x = (double)i/(double)length;
//        //io_data[i] *= 0.5 - 0.5 * cos(2*M_PI*x);
//        n = (double)i-M;
//        io_data[i][0] *= (1+cos(M_PI*n/M))/2.0;
//        io_data[i][1] *= (1+cos(M_PI*n/M))/2.0;
//    }
//}
//void wsp::math::ApplyHannWindowHor(complex64 *io_data, int width, int height)
//{
//    {
//        complex64 *data_ptr = io_data;
//        complex64 *end_ptr = data_ptr + width*height;
//        for(; data_ptr!=end_ptr; data_ptr+=width){
//            wsp::math::ApplyHannWindow_Complex(data_ptr, width);
//        }
//    }
//}
//void wsp::math::SwapQuadrantHor_Complex(complex64 *io_img, int width, int height)
//{
//    int x, y, i, j;
//    int w = (int)width;
//    int h = (int)height;
//    int length = w*h;
//    int hw = w/2;
//    int hh = h/2;
//    complex64 *tmp_img = (complex64*)malloc(sizeof( complex64 ) * length);
//    // copy
//    for(i=0; i<length; i++){
//        tmp_img[i][0] = io_img[i][0];
//        tmp_img[i][1] = io_img[i][1];
//    }
//    // swap
//    for(y=0; y<h; y++){
//        for(x=0; x<w; x++){
//            j = y*w + x;
//            if(y<hh && x<hw){
//                i = y*w + x + hw;
//            }else if(y<hh && x>=hw){
//                i = y*w + x - hw;
//            }else if(y>=hh && x<hw){
//                i = y*w + x + hw;
//            }else{
//                i = y*w + x - hw;
//            }
//            io_img[j][0] = tmp_img[i][0];
//            io_img[j][1] = tmp_img[i][1];
//        }
//    }
//    free(tmp_img);
//}
//void wsp::math::SwapQuadrantVer_Complex(complex64 *io_img, int width, int height)
//{
//    int x, y, i, j;
//    int w = (int)width;
//    int h = (int)height;
//    int length = w*h;
//    int hw = w/2;
//    int hh = h/2;
//    complex64 *tmp_img = (complex64*)malloc(sizeof( complex64 ) * length);
//    // copy
//    for(i=0; i<length; i++){
//        tmp_img[i][0] = io_img[i][0];
//        tmp_img[i][1] = io_img[i][1];
//    }
//    // swap
//    for(y=0; y<h; y++){
//        for(x=0; x<w; x++){
//            j = y*w + x;
//            if(y<hh && x<hw){
//                i = (y+hh)*w + x;
//            }else if(y<hh && x>=hw){
//                i = (y+hh)*w + x;
//            }else if(y>=hh && x<hw){
//                i = (y-hh)*w + x;
//            }else{
//                i = (y-hh)*w + x;
//            }
//            io_img[j][0] = tmp_img[i][0];
//            io_img[j][1] = tmp_img[i][1];
//        }
//    }
//    free(tmp_img);
//}
//void wsp::math::SwapQuadrantHV_Complex(complex64 *io_img, int width, int height)
//{
//    wsp::math::SwapQuadrantHor_Complex(io_img, width, height);
//    wsp::math::SwapQuadrantVer_Complex(io_img, width, height);
//}
//void wsp::math::Transpose_Complex(complex64 *io_img, int width, int height)
//{
//    int x, y, i, j;
//    int w = (int)width;
//    int h = (int)height;
//    int length = w*h;
//    complex64 *tmp_img = ( complex64* )malloc( sizeof( complex64 ) * length);
//    // copy
//    for(i=0; i<length; i++){
//        tmp_img[i][0] = io_img[i][0];
//        tmp_img[i][1] = io_img[i][1];
//    }
//    for(y=0; y<h; y++){
//        for(x=0; x<w; x++){
//            i = y*w + x;
//            j = x*h + y;
//            io_img[j][0] = tmp_img[i][0];
//            io_img[j][1] = tmp_img[i][1];
//        }
//    }
//    free(tmp_img);
//}
//
//
//// ------------------------------------------------------------------
//// Fourier Transform
//// ------------------------------------------------------------------
//wsp::State wsp::math::DFT1d(
//    double *dest_r, 
//    double *dest_i, 
//    const double *src_r, 
//    const double *src_i, 
//    int sampleSize, 
//    bool inverse
//){
//    double sum_re, sum_im, pi, arg, freqSample;
//    int sample;
//    int freq;
//
//    if(dest_r == NULL || dest_i == NULL || src_r == NULL || src_i == NULL || sampleSize <= 1){
//        return WSP_STATE_FAILURE;
//    }
//
//    // initializing a destination array
//    for( sample=0; sample<sampleSize; sample++){
//        dest_r[sample] = 0.0;
//        dest_i[sample] = 0.0;
//    }
//
//    pi = (inverse == false)? M_PI: -M_PI;
//
//    // main loop
//    for( freq=0; freq<sampleSize; freq++)
//    {
//        sum_re = 0.0;
//        sum_im = 0.0;
//        arg = ((double)freq/(double)sampleSize) * (2*pi);
//        for( sample=0; sample<sampleSize; sample++)
//        {
//            freqSample = sample * arg;
//
//            sum_re += src_r[sample] * cos( freqSample ) + src_i[sample] * sin( freqSample );
//            sum_im += src_i[sample] * cos( freqSample ) - src_r[sample] * sin( freqSample );
//
//            //! Explanation for equation ----------------------------------
//            //!  image = REAL(image) + IMAGINARY(image);
//            //!  We define that an image has imaginary number, and it's 0;
//            //!  First, we get correlation between one sin function and a x line of the image.
//            //!  It will be spectrum.
//            //!
//            //! Pseudo-code
//            //!  F(freq) = sum(REAL(image, point) * cos(freq, point) - i * REAL(image, point) * sin(freq, point))
//            //!          + i * sum( IMAGINARY(image, point) * sin(freq, point)) - i * IMAGINARY(image, point) * sin(freq, point) );
//            //!          = sum(REAL(image, point) * cos(freq, point)) - i * sum(REAL(image, point) * sin(freq, point))
//            //!          + i * sum( IMAGINARY(image, point) * sin(freq, point)) + sum(IMAGINARY(image, point) * sin(freq, point));
//            //!  sum_re = REAL(srcData, iter) * cos( samplingPoint ) 
//            //!        + IMAGINARY(srcData, iter) * sin( samplingPoint );
//            //!  sum_im = IMAGINARY(srcData, iter) * cos( samplingPoint )
//            //!        - REAL(srcData, iter) * sin( samplingPoint );
//            //!  We have defined imaginary number of this image is 0,
//            //!  so IMAGINARY(...) is 0;
//            //!  sum_re = REAL(srcData, iter) * cos( samplingPoint );
//            //!  sum_im = - REAL(srcData, iter) * sin( samplingPoint );
//            //!------------------------------------------------------------
//
//        }
//        if(inverse){
//            dest_r[freq] = sum_re;
//            dest_i[freq] = sum_im;
//        }else{
//            dest_r[freq] = sum_re/(double)sampleSize;
//            dest_i[freq] = sum_im/(double)sampleSize;
//        }
//    }
//
//    return WSP_STATE_SUCCESS;
//}
//
///*-------------------------------------------------------------------------
//   This computes an in-place complex-to-complex FFT
//   io_real and io_imag are the real and imaginary arrays of 2^m points.
//   ifft = false gives forward transform
//   ifft = true gives reverse transform
//
//     Formula: forward
//                  N-1
//                  ---
//              1   \          - j k 2 pi n / N
//      X(n) = ---   >   x(k) e                    = forward transform
//              N   /                                n=0..N-1
//                  ---
//                  k=0
//
//      Formula: reverse
//                  N-1
//                  ---
//                  \          j k 2 pi n / N
//      X(n) =       >   x(k) e                    = forward transform
//                  /                                n=0..N-1
//                  ---
//                  k=0
//*/
//wsp::State wsp::math::FFT1d(
//    double *io_real, 
//    double *io_imag, 
//    int size, int m, bool ifft
//){
//    long i,i1,j,k,i2,l,l1,l2;
//    double c1,c2,tx,ty,t1,t2,u1,u2,z;
//    // argument check ---------------------------------------
//    if(size < 2){
//        WSP_COMMON_ERROR_LOG("size is less than 2\n"); return WSP_STATE_INVALID_PARAMETER;
//    }
//    if(m<=0)
//    {
//        int i, j;
//        for(m=0, i=size; (i=i/2)!=0; ++m){}
//        for(i=0, j=1; i<m; ++i, j*=2){}
//        if(size != j){
//            WSP_COMMON_ERROR_LOG("size is not power of 2, %d != %d\n", size, j); 
//            return WSP_STATE_INVALID_PARAMETER;
//        }
//    }
//    // --------------------------------------------------------
//    //printf("size: %d, m: %d\n", size, m);
//
//    // Do the bit reversal
//    i2 = size >> 1;
//    j = 0;
//    int len = size-1;
//    for (i=0;i<len; ++i) {
//        if (i < j) {
//            tx = io_real[i];
//            ty = io_imag[i];
//            io_real[i] = io_real[j];
//            io_imag[i] = io_imag[j];
//            io_real[j] = tx;
//            io_imag[j] = ty;
//        }
//        k = i2;
//        while (k <= j) {
//            j -= k;
//            k >>= 1;
//        }
//        j += k;
//    }
//
//    // Compute the FFT
//    c1 = -1.0; 
//    c2 = 0.0;
//    l2 = 1;
//    for (l=0; l<m; ++l) {
//        l1 = l2;
//        l2 <<= 1;
//        u1 = 1.0; 
//        u2 = 0.0;
//        for (j=0; j<l1; ++j) {
//            for (i=j; i<size; i+=l2) {
//                i1 = i + l1;
//                t1 = u1 * io_real[i1] - u2 * io_imag[i1];
//                t2 = u1 * io_imag[i1] + u2 * io_real[i1];
//                io_real[i1] = io_real[i] - t1; 
//                io_imag[i1] = io_imag[i] - t2;
//                io_real[i] += t1;
//                io_imag[i] += t2;
//            }
//            z =  u1 * c1 - u2 * c2;
//            u2 = u1 * c2 + u2 * c1;
//            u1 = z;
//        }
//        c2 = sqrt((1.0 - c1) / 2.0);
//        if (ifft==false){ c2 = -c2; }
//        c1 = sqrt((1.0 + c1) / 2.0);
//    }
//
//    // Scaling for forward transform
//    if (ifft==false) {
//        for (i=0;i<size;i++) {
//            io_real[i] /= size;
//            io_imag[i] /= size;
//        }
//    }
//
//    return WSP_STATE_SUCCESS;
//}
//wsp::State wsp::math::FFT1d(complex64 *io_complex, int size, int exponent, bool ifft)
//{
//    int i2;
//    double c1,c2,tx,ty,t1,t2,u1,u2,z;
//    int complex_size = size * 2;
//    
//    // argument check ---------------------------------------
//    if(size < 2){
//        WSP_COMMON_ERROR_LOG("size is less than 2\n"); return WSP_STATE_INVALID_PARAMETER;
//    }
//    if(exponent<=0)
//    {
//        int i, j;
//        for(exponent=0, i=size; (i=i/2)!=0; ++exponent){}
//        for(i=0, j=1; i<exponent; ++i, j*=2){}
//        if(size != j){
//            WSP_COMMON_ERROR_LOG("size is not power of 2, %d != %d\n", size, j); 
//            return WSP_STATE_INVALID_PARAMETER;
//        }
//    }
//    // --------------------------------------------------------
//    //printf("size: %d, exponent: %d\n", size, exponent);
//
//    // reverse bit
//    i2 = size >> 1;
//    {
//        double *im_ptr, *re_ptr_j, *im_ptr_j;
//        int i, j, k;
//        double *src_ptr = reinterpret_cast<double*>(io_complex);
//        double *re_ptr = src_ptr;
//        double *end_ptr = re_ptr + complex_size - 2;
//        for (j=0; re_ptr!=end_ptr; re_ptr+=2) {
//            re_ptr_j = src_ptr + j*2;
//            if (re_ptr < re_ptr_j){
//                im_ptr = re_ptr + 1;
//                im_ptr_j = re_ptr_j + 1;
//                // swap complex[i] and complex[j]
//                tx = *re_ptr;
//                ty = *im_ptr;
//                *re_ptr = *re_ptr_j;
//                *im_ptr = *im_ptr_j;
//                *re_ptr_j = tx;
//                *im_ptr_j = ty;
//            }
//            
//            for(k = i2; k<=j; k>>=1){ j-=k; }
//            j+=k;
//        }
//    }
//
//    // Compute FFT
//    {
//        int l,l1,l2;
//        double *real_ptr, *imag_ptr, *end_ptr;
//        double *real_ptr_2, *imag_ptr_2, *end_ptr_2, *real_ptr_i1, *imag_ptr_i1;
//        int i, j, k, l2_step, l1_step;
//        c1 = -1.0; 
//        c2 = 0.0;
//        l2 = 1;
//        for (l=0; l<exponent; ++l) {
//            l1 = l2;
//            l2 <<= 1;
//            u1 = 1.0; 
//            u2 = 0.0;
//            l2_step = l2 * 2;
//            l1_step = l1 * 2;
//            real_ptr = reinterpret_cast<double*>(io_complex);
//            end_ptr = real_ptr + l1_step;
//            end_ptr_2 = reinterpret_cast<double*>(io_complex) + complex_size;
//            for(; real_ptr!=end_ptr; real_ptr+=2) {
//                real_ptr_2 = real_ptr;
//                for(; real_ptr_2<end_ptr_2; real_ptr_2+=l2_step){
//                    imag_ptr_2 = real_ptr_2 + 1;
//                    real_ptr_i1 = real_ptr_2 + l1_step;
//                    imag_ptr_i1 = real_ptr_i1 + 1;
//                    t1 = u1 * (*real_ptr_i1) - u2 * (*imag_ptr_i1);
//                    t2 = u1 * (*imag_ptr_i1) + u2 * (*real_ptr_i1);
//                    *real_ptr_i1 = *real_ptr_2 - t1; 
//                    *imag_ptr_i1 = *imag_ptr_2 - t2;
//                    *real_ptr_2 += t1;
//                    *imag_ptr_2 += t2;
//                }
//                z =  u1 * c1 - u2 * c2;
//                u2 = u1 * c2 + u2 * c1;
//                u1 = z;
//            }
//
//            c2 = sqrt((1.0 - c1) / 2.0);
//            if (ifft==true){ c2 = -c2; }
//            c1 = sqrt((1.0 + c1) / 2.0);
//        }
//    }
//    // Scale array size for forward fft
//    if(ifft==true) {
//        double *ptr = reinterpret_cast<double*>(io_complex);
//        double *end_ptr = ptr + complex_size;
//        for (; ptr!=end_ptr; ptr+=2) {
//            *ptr /= size;
//            *(ptr+1) /= size;
//        }
//    }
//    return WSP_STATE_SUCCESS;
//}
//
//
//wsp::State wsp::math::FFT1d_Opt(double *io_real, double *io_imag, int size, int m, bool ifft)
//{
//    int i2;
//    double c1,c2,tx,ty,t1,t2,u1,u2,z;
//    
//    // argument check ---------------------------------------
//    if(size < 2){
//        WSP_COMMON_ERROR_LOG("size is less than 2\n"); return WSP_STATE_INVALID_PARAMETER;
//    }
//    if(m<=0)
//    {
//        int i, j;
//        for(m=0, i=size; (i=i/2)!=0; ++m){}
//        for(i=0, j=1; i<m; ++i, j*=2){}
//        if(size != j){
//            WSP_COMMON_ERROR_LOG("size is not power of 2, %d != %d\n", size, j); 
//            return WSP_STATE_INVALID_PARAMETER;
//        }
//    }
//    // --------------------------------------------------------
//    //printf("size: %d, m: %d\n", size, m);
//
//    // Do the bit reversal
//    i2 = size >> 1;
//    {
//        double *real_ptr, *imag_ptr, *end_ptr;
//        double *real_ptr_j, *imag_ptr_j;
//        int i, j, k;
//        real_ptr = io_real;
//        imag_ptr = io_imag;
//        end_ptr = io_real + size - 1;
//        for (j=0; real_ptr!=end_ptr; ++real_ptr, ++imag_ptr) {
//            real_ptr_j = io_real + j;
//            if (real_ptr < real_ptr_j){
//                imag_ptr_j = io_imag + j;
//                // swap complex[i] and complex[j]
//                tx = *real_ptr;
//                ty = *imag_ptr;
//                *real_ptr = *real_ptr_j;
//                *imag_ptr = *imag_ptr_j;
//                *real_ptr_j = tx;
//                *imag_ptr_j = ty;
//            }
//            
//            for(k = i2; k<=j; k>>=1){ j-=k; }
//            j+=k;
//        }
//    }
//
//    // Compute FFT
//    {
//        int l,l1,l2;
//        double *real_ptr, *imag_ptr, *end_ptr;
//        double *real_ptr_2, *imag_ptr_2, *end_ptr_2, *real_ptr_i1, *imag_ptr_i1;
//        int i, j, k;
//        c1 = -1.0; 
//        c2 = 0.0;
//        l2 = 1;
//        for (l=0; l<m; ++l) {
//            l1 = l2;
//            l2 <<= 1;
//            u1 = 1.0; 
//            u2 = 0.0;
//            real_ptr = io_real;
//            imag_ptr = io_imag;
//            end_ptr = io_real + l1;
//            end_ptr_2 = io_real + size;
//            for(; real_ptr!=end_ptr; ++real_ptr, ++imag_ptr) {
//                real_ptr_2 = real_ptr;
//                imag_ptr_2 = imag_ptr;
//                for(; real_ptr_2<end_ptr_2; real_ptr_2+=l2, imag_ptr_2+=l2){
//                    real_ptr_i1 = real_ptr_2 + l1;
//                    imag_ptr_i1 = imag_ptr_2 + l1;
//                    t1 = u1 * (*real_ptr_i1) - u2 * (*imag_ptr_i1);
//                    t2 = u1 * (*imag_ptr_i1) + u2 * (*real_ptr_i1);
//                    *real_ptr_i1 = *real_ptr_2 - t1; 
//                    *imag_ptr_i1 = *imag_ptr_2 - t2;
//                    *real_ptr_2 += t1;
//                    *imag_ptr_2 += t2;
//                }
//                z =  u1 * c1 - u2 * c2;
//                u2 = u1 * c2 + u2 * c1;
//                u1 = z;
//            }
//
//            c2 = sqrt((1.0 - c1) / 2.0);
//            if (ifft==false){ c2 = -c2; }
//            c1 = sqrt((1.0 + c1) / 2.0);
//        }
//    }
//    // Scaling size for forward transoform
//    if(ifft==false) {
//        double *real_ptr, *imag_ptr, *end_ptr;
//        real_ptr = io_real;
//        imag_ptr = io_imag;
//        end_ptr = real_ptr + size;
//        for (; real_ptr!=end_ptr; ++real_ptr, ++imag_ptr) {
//            *real_ptr /= size;
//            *imag_ptr /= size;
//        }
//    }
//    return WSP_STATE_SUCCESS;
//}
//
///*-------------------------------------------------------------------------
//   Perform a 2D FFT inplace given a complex 2D array
//   The direction dir, 1 for forward, -1 for reverse
//   The size of the array (nx,ny)
//   Return false if there are memory problems or
//      the dimensions are not powers of 2
//*/
//wsp::State wsp::math::FFT2d(double *io_imag, double *io_real, int nx, int ny, int exp_x, int exp_y, bool ifft)
//{
//    int mx;
//
//    // argument check ---------------------------------------
//    if(nx < 2 || ny < 2){
//        WSP_COMMON_ERROR_LOG("size is less than 2\n"); return WSP_STATE_INVALID_PARAMETER;
//    }
//    if(exp_x<=0)
//    {
//        int i, j;
//        for(exp_x=0, i=nx; (i=i/2)!=0; ++exp_x){}
//        for(i=0, j=1; i<exp_x; ++i, j*=2){}
//        if(nx != j){
//            WSP_COMMON_ERROR_LOG("size is not power of 2, %d != %d\n", nx, j); 
//            return WSP_STATE_INVALID_PARAMETER;
//        }
//    }
//    if(exp_y<=0)
//    {
//        int i, j;
//        for(exp_y=0, i=ny; (i=i/2)!=0; ++exp_y){}
//        for(i=0, j=1; i<exp_y; ++i, j*=2){}
//        if(ny != j){
//            WSP_COMMON_ERROR_LOG("size is not power of 2, %d != %d\n", ny, j); 
//            return WSP_STATE_INVALID_PARAMETER;
//        }
//    }
//    // --------------------------------------------------------
//
//    //// Transform the rows
//    //{
//    //    int i, j, index;
//    //    double *real = (double *)malloc(nx * sizeof(double));
//    //    double *imag = (double *)malloc(nx * sizeof(double));
//    //    if (real == NULL || imag == NULL){
//    //        return WSP_STATE_INSUFFICIENT_MEMORY;
//    //    }
//    //    for (j=0;j<ny;j++) {
//    //        for (i=0;i<nx;i++) {
//    //            index = j*ny+i;
//    //            real[i] = io_real[index];
//    //            imag[i] = io_imag[index];
//    //        }
//    //        FFT1d(real,imag, nx, m, false);
//    //        for (i=0;i<nx;i++) {
//    //            c[i][j].real = real[i];
//    //            c[i][j].imag = imag[i];
//    //        }
//    //    }
//    //    free(real);
//    //    free(imag);
//    //}
//
//    //// Transform the columns
//    //{
//    //    double *real = (double *)malloc(ny * sizeof(double));
//    //    double *imag = (double *)malloc(ny * sizeof(double));
//    //    if (real == NULL || imag == NULL){
//    //        return WSP_STATE_INSUFFICIENT_MEMORY;
//    //    }
//    //    for (i=0;i<nx;i++) {
//    //        for (j=0;j<ny;j++) {
//    //            real[j] = io_real[i][j];
//    //            imag[j] = io_imag[i][j];
//    //        }
//    //        FFT(dir,m,real,imag);
//    //        for (j=0;j<ny;j++) {
//    //            io_real[i][j] = real[j];
//    //            io_imag[i][j] = imag[j];
//    //        }
//    //    }
//    //    free(real);
//    //    free(imag);
//    //}
//
//    return WSP_STATE_SUCCESS;
//}
//
//wsp::State wsp::math::FFT1dPerLine(double *io_real, double *io_imag, int width, int height, int exponent, bool ifft)
//{
//    if(exponent<=0)
//    {
//        int i, j;
//        for(exponent=0, i=width; (i=i/2)!=0; ++exponent){}
//        for(i=0, j=1; i<exponent; ++i, j*=2){}
//        if(width != j){
//            WSP_COMMON_ERROR_LOG("size is not power of 2, %d != %d\n", width, j); 
//            return WSP_STATE_INVALID_PARAMETER;
//        }
//    }
//    // Apply 1D FFT per line
//    {
//        double *real_ptr = io_real;
//        double *imag_ptr = io_imag;
//        double *end_ptr = io_real + width*height;
//        for(; real_ptr!=end_ptr; real_ptr+=width, imag_ptr+=width){
//            wsp::math::FFT1d_Opt(real_ptr, imag_ptr, width, exponent, ifft);
//        }
//    }
//    return WSP_STATE_SUCCESS;
//}
//
//wsp::State wsp::math::FFT1dPerLine(complex64 *io_complex, int width, int height, int exponent, bool ifft)
//{
//    if(exponent<=0)
//    {
//        int i, j;
//        for(exponent=0, i=width; (i=i/2)!=0; ++exponent){}
//        for(i=0, j=1; i<exponent; ++i, j*=2){}
//        if(width != j){
//            WSP_COMMON_ERROR_LOG("size is not power of 2, %d != %d\n", width, j); 
//            return WSP_STATE_INVALID_PARAMETER;
//        }
//    }
//    // Apply 1D FFT per line
//    {
//        complex64 *ptr = io_complex;
//        complex64 *end_ptr = ptr + width*height;
//        for(; ptr!=end_ptr; ptr+=width){
//            wsp::math::FFT1d(ptr, width, exponent, ifft);
//        }
//    }
//    return WSP_STATE_SUCCESS;
//}
//
//
//wsp::State wsp::math::FFT2dPerLine(double *io_real, double *io_imag, int width, int height, int exp_x, int exp_y, bool ifft)
//{
//    // argument check ---------------------------------------
//    if(width < 2 || height < 2){
//        WSP_COMMON_ERROR_LOG("size is less than 2\n"); return WSP_STATE_INVALID_PARAMETER;
//    }
//    if(exp_x<=0)
//    {
//        int i, j;
//        for(exp_x=0, i=width; (i=i/2)!=0; ++exp_x){}
//        for(i=0, j=1; i<exp_x; ++i, j*=2){}
//        if(width != j){
//            WSP_COMMON_ERROR_LOG("size is not power of 2, %d != %d\n", width, j); 
//            return WSP_STATE_INVALID_PARAMETER;
//        }
//    }
//    if(exp_y<=0)
//    {
//        int i, j;
//        for(exp_y=0, i=height; (i=i/2)!=0; ++exp_y){}
//        for(i=0, j=1; i<exp_y; ++i, j*=2){}
//        if(height != j){
//            WSP_COMMON_ERROR_LOG("size is not power of 2, %d != %d\n", height, j); 
//            return WSP_STATE_INVALID_PARAMETER;
//        }
//    }
//    // -----------------------------------------------------
//    int len = width*height;
//    int back_pix = len - 1;
//    // horizontal FFT
//    {
//        double *real_ptr = io_real, *imag_ptr = io_imag;
//        double *end_ptr = io_real + len;
//        for(; real_ptr!=end_ptr; real_ptr+=width, imag_ptr+=width){
//            wsp::math::FFT1d_Opt(real_ptr, imag_ptr, width, exp_x, ifft);
//        }
//    }
//    double *tmp_r = WSP_NEW double[len];
//    double *tmp_i = WSP_NEW double[len];
//    //// copy data
//    //{
//    //    double *dst_r = tmp_r, *dst_i = tmp_i;
//    //    double *src_r = io_real, *src_i = io_imag;
//    //    double *end_ptr = dst_ptr + len;
//    //    for(;dst_ptr!=end_ptr; ++dst_r, ++src_r, ++src_r, ++src_i){
//    //        *dst_r = *src_r;
//    //        *dst_i = *src_i;
//    //    }
//    //}
//    // transpose
//    {
//        double *src_r = io_real, *src_i = io_imag;
//        double *end_src = io_real + len - 1;
//        double *dst_r = tmp_r, *dst_i = tmp_i;
//        double *end_dst = tmp_r + len;
//        for(; dst_r!=end_dst; ++dst_r, ++dst_i, src_r+=width, src_i+=width){
//            if(src_r>end_src){
//                src_r -= back_pix;
//                src_i -= back_pix;
//            }
//            *dst_r = *src_r;
//            *dst_i = *src_i;
//        }
//    }
//    // vertical FFT
//    {
//        double *real_ptr = tmp_r, *imag_ptr = tmp_i;
//        double *end_ptr = io_real + len;
//        for(; real_ptr!=end_ptr; real_ptr+=height, imag_ptr+=height){
//            wsp::math::FFT1d_Opt(real_ptr, imag_ptr, height, exp_y, ifft);
//        }
//    }
//    // transpose
//    {
//        double *src_r = tmp_r, *src_i = tmp_i;
//        double *end_src = tmp_r + len - 1;
//        double *dst_r = io_real, *dst_i = io_imag;
//        double *end_dst = io_real + len;
//        for(; dst_r!=end_dst; ++dst_r, ++dst_i, src_r+=height, src_i+=height){
//            if(src_r>end_src){
//                src_r -= back_pix;
//                src_i -= back_pix;
//            }
//            *dst_r = *src_r;
//            *dst_i = *src_i;
//        }
//    }
//    delete[] tmp_r;
//    delete[] tmp_i;
//    return WSP_STATE_SUCCESS;
//}
//
//wsp::State wsp::math::FFT2dPerLine(
//    complex64 *io_complex, 
//    int width, int height, 
//    int exp_x, int exp_y, 
//    bool ifft, 
//    bool apply_hann_win, 
//    bool do_last_transpose,
//    complex64 *tmp_complex
//){
//    // argument check ---------------------------------------
//    if(width < 2 || height < 2){
//        WSP_COMMON_ERROR_LOG("size is less than 2\n"); return WSP_STATE_INVALID_PARAMETER;
//    }
//    if(exp_x<=0)
//    {
//        int i, j;
//        for(exp_x=0, i=width; (i=i/2)!=0; ++exp_x){}
//        for(i=0, j=1; i<exp_x; ++i, j*=2){}
//        if(width != j){
//            WSP_COMMON_ERROR_LOG("size is not power of 2, %d != %d\n", width, j); 
//            return WSP_STATE_INVALID_PARAMETER;
//        }
//    }
//    if(exp_y<=0)
//    {
//        int i, j;
//        for(exp_y=0, i=height; (i=i/2)!=0; ++exp_y){}
//        for(i=0, j=1; i<exp_y; ++i, j*=2){}
//        if(height != j){
//            WSP_COMMON_ERROR_LOG("size is not power of 2, %d != %d\n", height, j); 
//            return WSP_STATE_INVALID_PARAMETER;
//        }
//    }
//    // -----------------------------------------------------
//    int len = width*height;
//    int len2 = len*2;
//    int w2 = width*2;
//    int h2 = height*2;
//    if(apply_hann_win){
//        wsp::math::ApplyHannWindowHor(io_complex, width, height);
//    }
//    // horizontal FFT
//    {
//        complex64 *ptr = io_complex;
//        complex64 *end_ptr = ptr + len;
//        for(; ptr!=end_ptr; ptr+=width){
//            wsp::math::FFT1d(ptr, width, exp_x, ifft);
//        }
//    }
//    //// copy data
//    //{
//    //    double *dst_r = tmp_r, *dst_i = tmp_i;
//    //    double *src_r = io_real, *src_i = io_imag;
//    //    double *end_ptr = dst_ptr + len;
//    //    for(;dst_ptr!=end_ptr; ++dst_r, ++src_r, ++src_r, ++src_i){
//    //        *dst_r = *src_r;
//    //        *dst_i = *src_i;
//    //    }
//    //}
//    double *tmp_data;
//    if(tmp_complex==NULL){
//        tmp_data = new double[len2];
//    }else{
//        tmp_data = reinterpret_cast<double*>(tmp_complex);
//    }
//    // transpose
//    int v_step = w2-1;
//    int back_pix = len2 - 2;
//    {
//        double *src_r = reinterpret_cast<double*>(io_complex);
//        double *end_src = src_r + len2 - 2;
//        double *dst_r = tmp_data;
//        double *end_dst = tmp_data + len2;
//        for(; dst_r!=end_dst; ++dst_r, src_r+=v_step){
//            if(src_r>end_src){
//                src_r -= back_pix;
//            }
//            *dst_r = *src_r;
//            ++dst_r;
//            ++src_r;
//            *dst_r = *src_r;
//        }
//    }
//    //int v_step = w2;
//    //int back_pix = len2 - 2;
//    //{
//    //    double *src_r = io_complex;
//    //    double *end_src = io_complex + len2 - 2;
//    //    double *dst_r = tmp_data;
//    //    double *end_dst = tmp_data + len2;
//    //    for(; dst_r!=end_dst; dst_r+=2, src_r+=v_step){
//    //        if(src_r>end_src){
//    //            src_r -= back_pix;
//    //        }
//    //        *dst_r = *src_r;
//    //        *(dst_r+1) = *(src_r+1);
//    //    }
//    //}
//    if(apply_hann_win){
//        wsp::math::ApplyHannWindowHor(io_complex, width, height);
//    }
//    // vertical FFT
//    {
//        complex64 *ptr = reinterpret_cast<complex64*>(tmp_data);
//        complex64 *end_ptr = ptr + len;
//        for(; ptr!=end_ptr; ptr+=height){
//            wsp::math::FFT1d(ptr, height, exp_y, ifft);
//        }
//    }
//    if(do_last_transpose){
//        // transpose
//        v_step = h2-1;
//        double *src_r = tmp_data;
//        double *end_src = tmp_data + len2 - 2;
//        double *dst_r = reinterpret_cast<double*>(io_complex);
//        double *end_dst = dst_r + len2;
//        for(; dst_r!=end_dst; ++dst_r, src_r+=v_step){
//            if(src_r>end_src){
//                src_r -= back_pix;
//            }
//            *dst_r = *src_r;
//            ++dst_r;
//            ++src_r;
//            *dst_r = *src_r;
//        }
//    }else{
//        // do just copy only
//        double *src_r = tmp_data;
//        double *dst_r = reinterpret_cast<double*>(io_complex);
//        double *end_dst = dst_r + len2;
//        for(; dst_r!=end_dst; ++dst_r, ++src_r){
//            *dst_r = *src_r;
//        }
//    }
//    //v_step = h2;
//    //{
//    //    double *src_r = tmp_data;
//    //    double *end_src = tmp_data + len2 - 2;
//    //    double *dst_r = io_complex;
//    //    double *end_dst = io_complex + len2;
//    //    for(; dst_r!=end_dst; dst_r+=2, src_r+=v_step){
//    //        if(src_r>end_src){
//    //            src_r -= back_pix;
//    //        }
//    //        *dst_r = *src_r;
//    //        *(dst_r+1) = *(src_r+1);
//    //    }
//    //}
//    //{
//    //    for(int i=0; i<len2; ++i){
//    //        io_complex[i] = tmp_data[i];
//    //    }
//    //}
//    if(tmp_complex==NULL){
//        delete[] tmp_data;
//    }
//    return WSP_STATE_SUCCESS;
//}
//
//
//
//wsp::State wsp::math::Get1dFftPoc(double *o_poc_real, double *o_poc_imag,
//                                  double *o_pocav_real, double *o_pocav_imag, 
//                                  double *io_ref_real, double *io_ref_imag, 
//                                  double *io_tpl_real, double *io_tpl_imag, 
//                                  int width, int height, 
//                                  double low_pass_ratio)
//{
//    if(o_poc_real==NULL || o_poc_imag==NULL){
//        WSP_COMMON_ERROR_LOG("o_poc_real and o_poc_imag must not be NULL\n");
//        return WSP_STATE_INVALID_PARAMETER;
//    }
//    double crossSpectrum;
//    int i, j, x, y; //! iterators
//    int w = width;
//    int h = height;
//    int length = w*h;
//    int hw = w/2;
//    int hh = h/2;
//    //wsp::Image<double> imgRe;
//    //fftw_plan ifft_res;
//
//    //imgRe.SetSize(w, h);
//    //imgRe.Reset(0);
//
//
//    // load image data to FFTW input
//    //for(i=0; i<length; i++){
//    //    img1[i][0] = in_ref[i];
//    //    img2[i][0] = in_tpl[i];
//    //    img1[i][1] = 0.0;
//    //    img2[i][1] = 0.0;
//    //}
//
//    //! apply hann window
//    //for(i=0; i<length; i+=w){
//    //    wsp::math::ApplyHannWindow(&io_ref_real[i], w);
//    //    wsp::math::ApplyHannWindow(&io_tpl_real[i], w);
//    //}
//
//    //wsp::Save2dArrayAsPGM(io_ref_real, width, height, "io_ref_real_hann.pgm");
//    //wsp::Save2dArrayAsPGM(io_tpl_real, width, height, "io_tpl_real_hann.pgm");    // swapping quadrant horizontal
//
//    double *tmp_ref_r = new double[length];
//    double *tmp_tpl_r = new double[length];
//    double *tmp_ref_i = new double[length];
//    double *tmp_tpl_i = new double[length];
//    //double *ptr = tmp_buffer, *src_ptr = io_ref_real, *end_ptr = tmp_buffer+length;
//    //for(; ptr!=end_ptr; ++ptr){ *ptr = *src_ptr; }
//    //wsp::math::SwapQuadrantHor<double>(io_ref_real, tmp_ref_r, width, height);
//    //wsp::math::SwapQuadrantHor<double>(io_tpl_real, tmp_tpl_r, width, height);
//    
//
//    //wsp::math::FFT1dPerLine(io_ref_real, io_ref_imag, width, height, -1, false);
//    //wsp::math::FFT1dPerLine(io_tpl_real, io_tpl_imag, width, height, -1, false);
//    wsp::math::FFT1dPerLine(tmp_ref_r, tmp_ref_i, width, height, -1, false);
//    wsp::math::FFT1dPerLine(tmp_tpl_r, tmp_tpl_i, width, height, -1, false);
//
//    // swapping quadrant 1 and 3, 2 and 4
//    //wsp::math::SwapQuadrantVer<double>(io_ref_real, tmp_ref_r, width, height);
//    //wsp::math::SwapQuadrantVer<double>(io_tpl_real, tmp_tpl_r, width, height);
//
//    //wsp::Save2dArrayAsPGM(io_ref_real, width, height, "io_ref_real.pgm");
//    //wsp::Save2dArrayAsPGM(io_tpl_real, width, height, "io_tpl_real.pgm");
//    //wsp::Save2dArrayAsPGM(tmp_ref_r, width, height, "io_ref_real.pgm");
//    //wsp::Save2dArrayAsPGM(tmp_tpl_r, width, height, "io_tpl_real.pgm");
//
//
//    // obtain the cross power spectrum
//    for(y=0; y<h; y++){
//        for(x=0; x<w; x++){
//            i = y*w + x;
//            // remove high freq(low pass filter) because it include many noise
//            if(abs((int)x-hw) < (int)(hw*(1.0-low_pass_ratio))){
//                o_poc_real[i] = 0;
//                o_poc_imag[i] = 0;
//                //imgRe[i] = 0;
//                continue;
//            }
//            // corr can be obtained by F(k1, k2)*conj(G(k1, k2))
//            //                            = (f0 - f1*i)*(g0 + g1*i)
//            //                            = (f0*g0 + f1*g1) + (f0*g1 - f1*g0)*i
//            o_poc_real[i] = (tmp_ref_r[i] * tmp_tpl_r[i]) + (tmp_ref_i[i] * tmp_tpl_i[i]);
//            o_poc_imag[i] = (tmp_ref_r[i] * tmp_tpl_i[i]) - (tmp_ref_i[i] * tmp_tpl_r[i]);
//            //o_poc_real[i] = (io_ref_real[i] * io_tpl_real[i]) + (io_ref_imag[i] * io_tpl_imag[i]);
//            //o_poc_imag[i] = (io_ref_real[i] * io_tpl_imag[i]) - (io_ref_imag[i] * io_tpl_real[i]);
//
//            //tmp = sqrt( pow( o_poc_real[i], 2.0 ) + pow( o_poc_imag[i], 2.0 ) );
//            crossSpectrum = sqrt(o_poc_real[i]*o_poc_real[i] + o_poc_imag[i]*o_poc_imag[i]);
//
//            if(crossSpectrum!=0){
//                o_poc_real[i] /= crossSpectrum;
//                o_poc_imag[i] /= crossSpectrum;
//            }
//
//            // debug
//            //o_poc_real[i] = io_ref_real[i][0];
//            //o_poc_imag[i] = io_ref_real[i][1];
//            //o_poc_real[i] /= 256*N;
//            //o_poc_imag[i] /= 256*N;
//            //imgRe[i] = log(sqrt(io_ref_real[i][0]*io_ref_real[i][0] + io_ref_real[i][1]*io_ref_real[i][1]));
//            //imgRe[i] = log(sqrt(o_poc_real[i]*o_poc_real[i] + o_poc_imag[i]*o_poc_imag[i]));
//        }
//    }
//    //wsp::Save2dArrayAsPGM(o_poc_real, width, height, "io_cross_spectrum.pgm");
//    //imgRe.SaveAsTiff("fft_spectr_io_ref_real.tif");
//
//    if(o_pocav_real!=NULL && o_pocav_imag!=NULL){
//        // get average of correlation
//        for(x=0; x<w; x++){
//            o_pocav_real[x] = 0;
//            o_pocav_imag[x] = 0;
//            for(y=0; y<h; y++){
//                o_pocav_real[x] += o_poc_real[y*w+x];
//                o_pocav_imag[x] += o_poc_imag[y*w+x];
//                //res_av[x][0] *= res[y*w+x][0];
//                //res_av[x][1] *= res[y*w+x][1];
//            }
//            o_pocav_real[x]/=w;
//            o_pocav_imag[x]/=w;
//        }
//
//        //wsp::math::SwapQuadrantHor_fftw(res, (int)w, (int)h);
//
//        wsp::math::FFT1dPerLine(o_pocav_real, o_pocav_imag, width, height, -1, true);
//
//        for(x=0; x<w; x++){
//            o_pocav_real[x] = o_pocav_real[x]/(double)w;
//        }
//    }
//
//    //! horizontal ifft
//    if(o_poc_real!=NULL){
//        wsp::math::FFT1dPerLine(o_poc_real, o_poc_imag, width, height, -1, true);
//        //wsp::math::SwapQuadrantHV_fftw(res, (int)N, (int)N);
//
//        //! normalize and copy to result image
//        for(i=0; i<length; i++){
//            o_poc_real[i] = o_poc_real[i]/(double)(length);
//        }
//    }
//
//    //! deallocate FFTW arrays and plans
//    //fftw_free(img1);
//    //fftw_free(img2);
//    //fftw_free(res);
//    //fftw_free(res_av);
//    return WSP_STATE_SUCCESS;
//}
//
//void wsp::math::GetCrossPowerSpectrum(complex64 *io_ref, complex64 *io_tpl, complex64 *o_poc, int length){
//    // obtain the cross power spectrum
//    {
//        // optimized code ---------------------------------------------
//        double cross_spectrum;
//        double *ref_ptr = reinterpret_cast<double*>(io_ref);
//        double *tpl_ptr = reinterpret_cast<double*>(io_tpl);
//        double *poc_ptr = reinterpret_cast<double*>(o_poc);
//        double *end_ptr = ref_ptr + length*2;
//        double *poc_img_ptr, *tpl_img_ptr, *ref_img_ptr;
//        for(; ref_ptr!=end_ptr; ref_ptr+=2, tpl_ptr+=2, poc_ptr+=2){
//            ref_img_ptr = ref_ptr + 1;
//            tpl_img_ptr = tpl_ptr + 1;
//            poc_img_ptr = poc_ptr + 1;
//            // --------------------------------------------------------------
//            // corr can be obtained by F(k1, k2)*conj(G(k1, k2))
//            //                            = (f0 - f1*i)*(g0 + g1*i)
//            //                            = (f0*g0 + f1*g1) + (f0*g1 - f1*g0)*i
//            *poc_ptr = ((*ref_ptr) * (*tpl_ptr)) + ((*ref_img_ptr) * (*tpl_img_ptr));
//            *poc_img_ptr = ((*ref_ptr) * (*tpl_img_ptr)) - ((*ref_img_ptr) * (*tpl_ptr));
//            // --------------------------------------------------------------
//
//            //tmp = sqrt( pow( res[i][0], 2.0 ) + pow( res[i][1], 2.0 ) );
//            cross_spectrum = sqrt((*poc_ptr)*(*poc_ptr) + (*poc_img_ptr)*(*poc_img_ptr));
//
//            *poc_ptr /= cross_spectrum;
//            *poc_img_ptr /= cross_spectrum;
//        }
//        // un-optimized code ------------------------------------------
//        //int i, j, x, y; // iterators
//        //for(y=0; y<height; y++){
//        //    for(x=0; x<width; x++){
//        //        i = y*width + x;
//        //        // remove high freq(low pass filter) because it include many noise
//        //        if(abs((int)x-hw) > (int)(hw*low_pass_ratio) 
//        //            || abs((int)y-hh) > (int)(hh*low_pass_ratio)){
//        //            res[i][0] = 0;
//        //            res[i][1] = 0;
//        //            continue;
//        //        }
//        //        // corr can be obtained by F(k1, k2)*conj(G(k1, k2))
//        //        //                            = (f0 - f1*i)*(g0 + g1*i)
//        //        //                            = (f0*g0 + f1*g1) + (f0*g1 - f1*g0)*i
//        //        res[i][0] = (img1[i][0] * img2[i][0]) + (img1[i][1] * img2[i][1]);
//        //        res[i][1] = (img1[i][0] * img2[i][1]) - (img1[i][1] * img2[i][0]);
//        //        //tmp = sqrt( pow( res[i][0], 2.0 ) + pow( res[i][1], 2.0 ) );
//        //        crossSpectrum = sqrt(res[i][0]*res[i][0] + res[i][1]*res[i][1]);
//        //        res[i][0] /= crossSpectrum;
//        //        res[i][1] /= crossSpectrum;
//        //    }
//        //}
//
//    }
//}
////! remove high freq(low pass filter) because it include many noise
//void wsp::math::RemoveCentricVerticalHorizontalArea(complex64 *io_data, int width, int height, double remove_ratio)
//{
//    {
//        int x, y;
//        int hw = width/2, hh = height/2;
//        int thre_pix_w = hw * remove_ratio;
//        int thre_pix_h = hh * remove_ratio;
//        int abs_y_hh = abs(hh);
//        // optimized code ---------------------
//        complex64 *data_ptr = io_data;
//        complex64 *end_ptr = data_ptr + width*height;
//        complex64 *end_w = data_ptr + width;
//        for(x=0, y=0; data_ptr!=end_ptr; ++data_ptr, ++x){
//            if(abs(x-hw) > thre_pix_w 
//                || abs_y_hh > hh * thre_pix_h){
//                (*data_ptr)[0] = 0;
//                (*data_ptr)[1] = 0;
//            }
//            if(data_ptr==end_w){
//                end_w = data_ptr + width;
//                ++y; x=0;
//                abs_y_hh = abs(y-hh);
//            }
//        }
//        // un-optimized code -------------------
//        //int i;
//        //for(y=0; y<height; ++y){
//        //    abs_y_hh = abs(y-hh);
//        //    for(x=0; x<width; ++x){
//        //        i = y*width + x;
//        //        if(  abs(x-hw)> thre_pix_w) 
//        //          || abs_y_hh > thre_pix_h){
//        //            io_data[i][0] = 0;
//        //            io_data[i][1] = 0;
//        //        }
//        //    }
//        //}
//    }
//}
//
//void wsp::math::RemoveCentricVerticalArea(complex64 *io_data, int width, int height, double remove_ratio)
//{
//    {
//        int x, y;
//        int hw = width/2;
//        int thre_pix_w = hw * remove_ratio;
//        // optimized code ---------------------
//        complex64 *data_ptr = io_data;
//        complex64 *end_ptr = data_ptr + width*height;
//        complex64 *end_w = data_ptr + width;
//        for(x=0, y=0; data_ptr!=end_ptr; ++data_ptr, ++x){
//            if(abs(x-hw) > thre_pix_w){
//                (*data_ptr)[0] = 0;
//                (*data_ptr)[1] = 0;
//            }
//            if(data_ptr==end_w){
//                end_w = data_ptr + width;
//                ++y; x=0;
//            }
//        }
//        // un-optimized code -------------------
//        //int i;
//        //for(y=0; y<height; ++y){
//        //    for(x=0; x<width; ++x){
//        //        i = y*width + x;
//        //        if(abs(x-hw)> thre_pix_w){
//        //            io_data[i][0] = 0;
//        //            io_data[i][1] = 0;
//        //        }
//        //    }
//        //}
//    }
//}
//
//wsp::State wsp::math::Get1dFftPoc(
//    complex64 *io_ref, 
//    complex64 *io_tpl, 
//    complex64 *o_poc, 
//    complex64 *o_average_poc, 
//    int width, 
//    int height, 
//    int exponent_x,
//    bool apply_hann_win,
//    double low_pass_ratio
//){
//    if(!wsp::math::IsPowerOfTwo(width)){
//        WSP_COMMON_ERROR_LOG("width must be power of two: width=%d\n", width);
//        return WSP_STATE_FAILURE;
//    }
//    int length = width*height;
//
//    // apply hann window
//    if(apply_hann_win){
//        wsp::math::ApplyHannWindowHor(io_ref, width, height);
//        wsp::math::ApplyHannWindowHor(io_tpl, width, height);
//    }
//
//    wsp::math::FFT1dPerLine(io_ref, width, height, exponent_x, false);
//    wsp::math::FFT1dPerLine(io_tpl, width, height, exponent_x, false);
//
//    //wsp::math::GetCrossPowerSpectrum(io_ref, io_tpl, o_poc, length);
//    //if(low_pass_ratio<1.0){
//    //    wsp::math::RemoveCentricVerticalArea(o_poc, width, height, low_pass_ratio);
//    //    int x, y, i;
//    //    int hw = width/2;
//    //    for(y=0; y<height; y++){
//    //        for(x=0; x<width; x++){
//    //            i = y*width + x;
//    //            // remove high freq(low pass filter) because it include many noise
//    //            if(abs(x-hw) < (int)(hw*(1.0-low_pass_ratio))){
//    //                o_poc[i][0] = 0;
//    //                o_poc[i][1] = 0;
//    //            }
//    //        }
//    //    }
//    //}
//    // obtain the cross power spectrum
//    {
//        int x, y, i;
//        int hw = width/2;
//        for(y=0; y<height; ++y){
//            for(x=0; x<width; ++x){
//                i = y*width + x;
//                //! remove high freq(low pass filter) because it include many noise
//                if(abs(x-hw) < (int)(hw*(1-low_pass_ratio))){
//                    o_poc[i][0] = 0;
//                    o_poc[i][1] = 0;
//                    continue;
//                }
//                // corr can be obtained by F(k1, k2)*conj(G(k1, k2))
//                //                            = (f0 - f1*i)*(g0 + g1*i)
//                //                            = (f0*g0 + f1*g1) + (f0*g1 - f1*g0)*i
//                o_poc[i][0] = (io_ref[i][0] * io_tpl[i][0]) + (io_ref[i][1] * io_tpl[i][1]);
//                o_poc[i][1] = (io_ref[i][0] * io_tpl[i][1]) - (io_ref[i][1] * io_tpl[i][0]);
//
//                //tmp = sqrt( pow( res[i][0], 2.0 ) + pow( o_poc[i][1], 2.0 ) );
//                double cross_spectrum = sqrt(o_poc[i][0]*o_poc[i][0] + o_poc[i][1]*o_poc[i][1]);
//
//                if(cross_spectrum!=0){
//                    o_poc[i][0] /= cross_spectrum;
//                    o_poc[i][1] /= cross_spectrum;
//                }
//            }
//            //SpectralWeighting_RectangularWin(complex64 *io_data, int length, double low_pass_ratio=0.75);
//        }
//    }
//
//    wsp::math::FFT1dPerLine(o_poc, width, height, exponent_x, true);
//
//    if(o_average_poc!=NULL){
//        // get average of poc
//        complex64 *src_ptr = o_poc;
//        complex64 *end_ptr = src_ptr + length;
//        complex64 *end_w = src_ptr + width;
//        complex64 *dst_ptr = o_average_poc;
//        for(; src_ptr!=end_w; ++dst_ptr, ++src_ptr){
//            (*dst_ptr)[0] = *src_ptr[0];
//            (*dst_ptr)[1] = *src_ptr[1];
//        }
//        for(; src_ptr!=end_ptr; ){
//            dst_ptr = o_average_poc;
//            end_w = src_ptr + width;
//            for(; src_ptr!=end_w; ++dst_ptr, ++src_ptr){
//                (*dst_ptr)[0] += *src_ptr[0];
//                (*dst_ptr)[1] += *src_ptr[1];
//            }
//        }
//        dst_ptr = o_average_poc;
//        complex64 *end_d_ptr = dst_ptr + width;
//        for(; dst_ptr!=end_d_ptr; ++dst_ptr){
//            (*dst_ptr)[0]/=static_cast<double>(height);
//            (*dst_ptr)[1]/=static_cast<double>(height);
//        }
//    }
//
//    return WSP_STATE_SUCCESS;
//}
//
//
//wsp::State wsp::math::Get2dPocWithFft(
//        complex64 *io_ref, 
//        complex64 *io_tpl, 
//        complex64 *o_poc, 
//        int width, 
//        int height, 
//        int exponent_x,
//        int exponent_y,
//        double low_pass_ratio,
//        bool apply_hann_win,
//        complex64 *tmp_data
//){
//    //if(!wsp::math::IsPowerOfTwo<int>(width)){
//    //    WSP_COMMON_ERROR_LOG("width must be power of two: width=%d\n", width);
//    //    return WSP_STATE_INVALID_PARAMETER;
//    //}
//    int length = width*height;
//
//    wsp::math::FFT2dPerLine(io_ref, width, height, exponent_x, exponent_y, false, apply_hann_win, false, tmp_data);
//    wsp::math::FFT2dPerLine(io_tpl, width, height, exponent_x, exponent_y, false, apply_hann_win, false, tmp_data);
//
//    wsp::math::GetCrossPowerSpectrum(io_ref, io_tpl, o_poc, length);
//    if(low_pass_ratio<1.0){
//        wsp::math::RemoveCentricVerticalHorizontalArea(o_poc, width, height, low_pass_ratio);
//    }
//
//    wsp::math::FFT2dPerLine(o_poc, width, height, exponent_x, exponent_y, true, false, false, tmp_data);
//
//    return WSP_STATE_SUCCESS;
//}
