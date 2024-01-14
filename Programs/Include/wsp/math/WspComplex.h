//! WspComplex.h
// 

#ifndef __WspComplex_H__
#define __WspComplex_H__

#include <wsp/common/_define_commoncore.h>

struct WspComplexD
{
    double real, imag;

    WspComplexD():real(0), imag(0){}
    WspComplexD(double in_real, double in_imag) { real=in_real; imag=in_imag; }
    WspComplexD(int in_real, int in_imag) { real=(double)in_real; imag=(double)in_imag; }

    WspComplexD operator- ()        { return WspComplexD(-real, -imag); }
    WspComplexD operator+ (WspComplexD in_complex) { return WspComplexD(real+in_complex.real, imag+in_complex.imag ); }
    WspComplexD operator- (WspComplexD in_complex) { return WspComplexD(real-in_complex.real, imag-in_complex.imag ); }
    WspComplexD operator* (WspComplexD in_complex) { return WspComplexD(real*in_complex.real-imag*in_complex.imag, 
                                                                        real*in_complex.imag-imag*in_complex.real ); }
    WspComplexD operator* (double scale) { return WspComplexD(real*scale, imag*scale); }
    bool  operator< (WspComplexD in_complex) { return (real <  in_complex.real) && (imag <  in_complex.imag ); }
    bool  operator<=(WspComplexD in_complex) { return (real <= in_complex.real) && (imag <= in_complex.imag ); }
    bool  operator> (WspComplexD in_complex) { return (real >  in_complex.real) && (imag >  in_complex.imag ); }
    bool  operator>=(WspComplexD in_complex) { return (real >= in_complex.real) && (imag >= in_complex.imag ); }
    bool  operator==(WspComplexD in_complex) { return (real == in_complex.real) && (imag == in_complex.imag ); }
    bool  operator!=(WspComplexD in_complex) { return (real != in_complex.real) || (imag != in_complex.imag ); }
};

struct WspComplexF
{
    float real, imag;

    WspComplexF():real(0), imag(0){}
    WspComplexF(double in_real, double in_imag) { real=in_real; imag=in_imag; }
    WspComplexF(int in_real, int in_imag) { real=(double)in_real; imag=(double)in_imag; }

    WspComplexF operator- ()        { return WspComplexF(-real, -imag); }
    WspComplexF operator+ (WspComplexF in_complex) { return WspComplexF(real+in_complex.real, imag+in_complex.imag ); }
    WspComplexF operator- (WspComplexF in_complex) { return WspComplexF(real-in_complex.real, imag-in_complex.imag ); }
    WspComplexF operator* (WspComplexF in_complex) { return WspComplexF(real*in_complex.real-imag*in_complex.imag, 
                                                                        real*in_complex.imag-imag*in_complex.real ); }
    WspComplexF operator* (float scale) { return WspComplexF(real*scale, imag*scale); }
    bool  operator< (WspComplexF in_complex) { return (real <  in_complex.real) && (imag <  in_complex.imag ); }
    bool  operator<=(WspComplexF in_complex) { return (real <= in_complex.real) && (imag <= in_complex.imag ); }
    bool  operator> (WspComplexF in_complex) { return (real >  in_complex.real) && (imag >  in_complex.imag ); }
    bool  operator>=(WspComplexF in_complex) { return (real >= in_complex.real) && (imag >= in_complex.imag ); }
    bool  operator==(WspComplexF in_complex) { return (real == in_complex.real) && (imag == in_complex.imag ); }
    bool  operator!=(WspComplexF in_complex) { return (real != in_complex.real) || (imag != in_complex.imag ); }
};

#endif