/**
 * @file tpl_fn-maya_convert.inl
 * @author Jun-ichi Nishikata
 * @brief Conversion between OpenMaya and other libraries.
 */

#ifndef WSP_MAYA_TEMPLATE_FUNCTION_MAYA_CONVERT_INL__
#define WSP_MAYA_TEMPLATE_FUNCTION_MAYA_CONVERT_INL__

#include <maya/MIntArray.h>
#include <maya/MPointArray.h>
#include <maya/MFloatPointArray.h>

#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#include <wsp/common/tpl_cl-vector.h>
#endif

template<typename T, int MemAllocStep>
void wsp::maya::Convert(const MIntArray &marray, wsp::Vector<T, MemAllocStep> &warray)
{
    int len = (int)marray.length();
    warray.SetLength(len);

    T *ptr = warray.data();
    for(int i=0; i<len; ++i, ++ptr){
        *ptr = static_cast<T>(marray[i]);
    }
}

template<typename T, int MemAllocStep>
void wsp::maya::Convert(const MPointArray &marray, wsp::Vector<T, MemAllocStep> &warray)
{
    int len = (int)marray.length();
    warray.SetLength(len, 4);

    T *ptr = warray.data();
    int i, j;
    for(i=0; i<len; ++i){
        for(j=0; j<4; ++j, ++ptr){
            *ptr = static_cast<T>(marray[i][j]);
        }
    }
}

template<typename T, int MemAllocStep>
void wsp::maya::Convert(const MFloatPointArray &marray, wsp::Vector<T, MemAllocStep> &warray)
{
    int len = (int)marray.length();
    warray.SetLength(len, 4);

    T *ptr = warray.data();
    int i, j;
    for(i=0; i<len; ++i){
        for(j=0; j<4; ++j, ++ptr){
            *ptr = static_cast<T>(marray[i][j]);
        }
    }
}

template<typename T, int MemAllocStep>
void wsp::maya::Convert(const wsp::Vector<T, MemAllocStep> &in_warray, MIntArray &o_marray)
{
    int len = in_warray.length();
    o_marray.setLength((unsigned int)len);

    const T *ptr = in_warray.data();
    for(int i=0; i<len; ++i, ++ptr){
        o_marray[i] = static_cast<int>(*ptr);
    }
}

template<typename T> 
inline void wsp::maya::Convert(const T *in_ary, MIntArray &o_marray, int length){
    int len = length;
    o_marray.setLength((unsigned int)len);

    const T *ptr = in_ary;
    for(int i=0; i<len; ++i, ++ptr){
        o_marray[i] = static_cast<int>(*ptr);
    }
}

template<typename T, int MemAllocStep>
void wsp::maya::Convert(const wsp::Vector<T, MemAllocStep> &in_warray, MPointArray &o_marray)
{
    int len = in_warray.length();
    int src_nc = in_warray.num_of_channels();
    o_marray.setLength((unsigned int)len);

    const T *ptr = in_warray.data();
    int i, j;
    int dst_nc = 4;
    if( src_nc == dst_nc )
    {
        for(i=0; i<len; ++i){
            for(j=0; j<dst_nc; ++j, ++ptr){
                o_marray[i][j] = static_cast<double>(*ptr);
            }
        }
    }
    else if( src_nc < dst_nc )
    {
        int diff_nc = src_nc-dst_nc;
        for(i=0; i<len; ++i){
            for(j=0; j<dst_nc; ++j, ++ptr){
                o_marray[i][j] = static_cast<double>(*ptr);
            }
            ptr+=diff_nc;
        }
    }
    else
    {
        for(i=0; i<len; ++i){
            for(j=0; j<src_nc; ++j, ++ptr){
                o_marray[i][j] = static_cast<double>(*ptr);
            }
            for(; j<dst_nc; ++j){
                o_marray[i][j] = static_cast<double>(*ptr);
            }
        }
    }
}

template<typename T, int MemAllocStep>
void wsp::maya::Convert(const wsp::Vector<T, MemAllocStep> &in_warray, MFloatPointArray &o_marray)
{
    int len = in_warray.length();
    int src_nc = in_warray.num_of_channels();
    o_marray.setLength((unsigned int)len);

    const T *ptr = in_warray.data();
    int i, j;
    int dst_nc = 4;
    if( src_nc == dst_nc )
    {
        for(i=0; i<len; ++i){
            for(j=0; j<dst_nc; ++j, ++ptr){
                o_marray[i][j] = static_cast<float>(*ptr);
            }
        }
    }
    else if( src_nc < dst_nc )
    {
        int diff_nc = src_nc-dst_nc;
        for(i=0; i<len; ++i){
            for(j=0; j<dst_nc; ++j, ++ptr){
                o_marray[i][j] = static_cast<float>(*ptr);
            }
            ptr+=diff_nc;
        }
    }
    else
    {
        for(i=0; i<len; ++i){
            for(j=0; j<src_nc; ++j, ++ptr){
                o_marray[i][j] = static_cast<float>(*ptr);
            }
            for(; j<dst_nc; ++j){
                o_marray[i][j] = static_cast<float>(*ptr);
            }
        }
    }
}



template<typename T> 
inline void wsp::maya::Convert(const T *in_ary, MFloatPointArray &o_marray, int length, int num_chennels)
{
    int len = length;
    int src_nc = num_chennels;
    o_marray.setLength((unsigned int)len);

    const T *ptr = in_ary;
    int i, j;
    int dst_nc = 4;
    if( src_nc == dst_nc )
    {
        for(i=0; i<len; ++i){
            for(j=0; j<dst_nc; ++j, ++ptr){
                o_marray[i][j] = static_cast<float>(*ptr);
            }
        }
    }
    else if( src_nc < dst_nc )
    {
        int diff_nc = src_nc-dst_nc;
        for(i=0; i<len; ++i){
            for(j=0; j<dst_nc; ++j, ++ptr){
                o_marray[i][j] = static_cast<float>(*ptr);
            }
            ptr+=diff_nc;
        }
    }
    else
    {
        for(i=0; i<len; ++i){
            for(j=0; j<src_nc; ++j, ++ptr){
                o_marray[i][j] = static_cast<float>(*ptr);
            }
            for(; j<dst_nc; ++j){
                o_marray[i][j] = static_cast<float>(*ptr);
            }
        }
    }
}

#endif
