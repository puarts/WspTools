/**
 * @file _tpl_fn-core.hpp
 * 
 * @brief Base definition of core template functions.
 */

#ifndef WSP_CORE_BASE_TEMPLATE_FUNC_CORE_HPP__
#define WSP_CORE_BASE_TEMPLATE_FUNC_CORE_HPP__

#include <typeinfo>
#ifndef __WSP_COMMONUTIL_DEFINE_COMMONUTIL_H__
#include "_define_commonutil.h"
#endif

namespace wsp{
    template<typename T> inline void PrintAsBit(T value){
        int len = sizeof(T)*8;
        for(int i=len-1; i>=0; --i)
        {
            int extracted_bit, mask;
            if((i+1)%8==0 && i!=len-1)
            {
                printf("-");
            }
            mask = 1 << i;
            extracted_bit = value & mask;
            extracted_bit>>=i;
            printf("%x", extracted_bit);
        }
    }

    template<typename T> inline wsp::DataType GetDataType()
    {
        const std::type_info& info = typeid(T);
        if(info==typeid(uchar) ){ return wsp::DATATYPE_8U; }
        if(info==typeid(short) ){ return wsp::DATATYPE_16S; }
        if(info==typeid(int)   ){ return wsp::DATATYPE_32U; }
        if(info==typeid(double)){ return wsp::DATATYPE_64F; }
        if(info==typeid(float) ){ return wsp::DATATYPE_32F; }
        if(info==typeid(char)  ){ return wsp::DATATYPE_8S; }
        if(info==typeid(u32)   ){ return wsp::DATATYPE_32U; }
        if(info==typeid(ushort)){ return wsp::DATATYPE_16U; }
        return wsp::DATATYPE_UNKNOWN;
    }

    template<typename T> inline bool IsFundamentalDataType()
    {
        if(sizeof(T)>sizeof(long double)){ return false; }
        return typeid(T)==typeid(char)   || typeid(T)==typeid(unsigned char)
            || typeid(T)==typeid(short)  || typeid(T)==typeid(unsigned short)
            || typeid(T)==typeid(int)    || typeid(T)==typeid(unsigned int)
            || typeid(T)==typeid(float)  || typeid(T)==typeid(double) || typeid(T)==typeid(long double);
    }
    
    template<typename T> inline T Max(T x, T y, T z){
        T max = x;
        if(max<y){ max=y; }
        if(max<z){ max=z; }
        return max;
    }
    template<typename T> inline T Min(T x, T y, T z){
        T min = x;
        if(min>y){ min=y; }
        if(min>z){ min=z; }
        return min;
    }

}


#endif
