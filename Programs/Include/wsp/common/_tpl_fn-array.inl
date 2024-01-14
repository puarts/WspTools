/**
 * @file _tpl_fn-array.inl
 * 
 */


#ifndef _WSP_TEMPLATE_BASE_FN_ARRAY_INL_
#define _WSP_TEMPLATE_BASE_FN_ARRAY_INL_

#include "_tpl_fn-array.hpp"
#include "_tpl_fn-mem_manag.hpp"
#include "ScopedFileOpen.h"

#include <iostream>
//#include <typeinfo.h>
#include <stdarg.h>
#include <math.h>

// Getter Functions ----------------------------------
template<typename T, typename U> inline U wsp::GetSum(const T *in_ary, int length){
    T sum = in_ary[0];
    const T *ptr = in_ary+1;
    const T *end_ptr = in_ary + length;
    for(; ptr!=end_ptr; ++ptr){ sum+=*ptr; }
    return static_cast<U>(sum);
}
template<typename InType, typename OutType> inline OutType wsp::GetAbsSum(const InType *in_ary, int length){
    InType sum = in_ary[0];
    const InType *ptr = in_ary+1;
    const InType *end_ptr = in_ary + length;
    for(; ptr!=end_ptr; ++ptr){
        if(*ptr<0){ sum+=-*ptr; continue; }
        sum += *ptr;
    }
    return static_cast<OutType>(sum);
}
template<typename InType, typename OutType> inline OutType wsp::GetAverage(const InType *ary, int length){
    if(length==0){ return static_cast<OutType>(0); }
    return static_cast<OutType>(wsp::GetSum<InType, double>( ary, length )/static_cast<double>(length));
}
template<typename T> inline T wsp::GetMax(const T *in_ary, int length)
{
    T maxv = in_ary[0];
    const T *ptr = in_ary + 1;
    const T *end_ptr = in_ary + length;
    for(; ptr!=end_ptr; ++ptr)
    {
        if(maxv < *ptr)
        {
            maxv = *ptr;
        }
    }

    return maxv;
}

template<typename T> 
inline void wsp::GetMax(T* o_max_value, int* o_max_index, const T* in_ary, int length)
{
    *o_max_index = 0;
    *o_max_value = in_ary[0];
    const T *ptr = in_ary + 1;
    const T *end_ptr = in_ary + length;
    for (int index = 0; ptr != end_ptr; ++ptr, ++index)
    {
        if (*o_max_value < *ptr)
        {
            *o_max_value = *ptr;
            *o_max_index = index;
        }
    }
}
template<typename T> inline T wsp::GetMin(const T *in_ary, int length)
{
    T minv = in_ary[0];
    const T *ptr = in_ary + 1;
    const T *end_ptr = in_ary + length;
    for(; ptr != end_ptr; ++ptr)
    {
        if(minv > *ptr)
        {
            minv = *ptr;
        }
    }
    return minv;
}
template<typename T> inline void wsp::GetMinMax(const T* in_ary, T &min, T &max, int length)
{
    min = in_ary[0];
    max = in_ary[0];
    const T *ptr = in_ary + 1;
    const T *end_ptr = in_ary + length;
    for(; ptr!=end_ptr; ++ptr)
    {
        if(min>*ptr){ min=*ptr; continue; }
        if(max<*ptr){ max=*ptr; }
    }
}

template<typename T> inline int wsp::GetMaxIndex(const T *in_ary, int length)
{
    T maxv = in_ary[0];
    const T *ptr = in_ary + 1;
    const T *end_ptr = in_ary + length;
    int maxIndex=0;
    for(int i=1; ptr!=end_ptr; ++ptr, ++i){
        if(maxv<*ptr){ maxv=*ptr; maxIndex=i; }
    }
    return maxIndex;
}
template<typename T> inline int wsp::GetMinIndex(const T *in_ary, int length)
{
    T minv = in_ary[0];
    const T *ptr = in_ary+1;
    const T *end_ptr = in_ary + length;
    int minIndex=0;
    for(int i=1; ptr!=end_ptr; ++ptr, ++i){
        if(minv>*ptr){ minv=*ptr; minIndex=i; }
    }
    return minIndex;
}
template<typename T> inline double wsp::GetMeanVariance(const T* ary, int length)
{
    if(length==0){ return 0.0; }

    double mean = wsp::GetAverage(ary, length);
    double meanVar=0.0;
    for(u32 i=0; i<length; i++){
        meanVar+=((double)ary[i] - mean)*((double)ary[i] - mean);
    }
    return meanVar/(double)length;
}
template<typename T> inline double wsp::GetMeanStdDev(const T*ary, int length)
{
    if(length==0){ return 0.0; }

    double mean = wsp::GetAverage(ary, length);
    double meanDev=0.0;
    for(u32 i=0; i<length; i++){
        meanDev += fabs((double)ary[i] - mean);
    }
    return meanDev/(double)length;
}
template<typename T> bool wsp::IsEqual(const T *in_ary_a, const T *in_ary_b, int length)
{
    const T *ptr_a = in_ary_a;
    const T *ptr_b = in_ary_b;
    const T *end_ptr = ptr_a + length;
    for(; ptr_a!=end_ptr; ++ptr_a, ++ptr_b){
        if(*ptr_a != *ptr_b){
            return false;
        }
    }
    return true;
}
template<typename T> int wsp::Find(const T *in_ary, T target_elem, int length)
{
    const T *ptr = in_ary;
    for(int i=0; i<length; ++i, ++in_ary){
        if(*ptr==target_elem){
            return i;
        }
    }
    return -1;
}

// Setter Functions ----------------------------------
template<typename InType, typename OutType> 
inline void wsp::AddArray(const InType *in_ary_a, const InType *in_ary_b, OutType *o_ary, int length)
{
    const InType *ptr_a = in_ary_a;
    const InType *ptr_b = in_ary_b;
    OutType *o_ptr = o_ary;
    OutType *end_ptr = o_ptr+length;

    for(; o_ptr!=end_ptr; ++o_ptr, ++ptr_a, ++ptr_b){
        *o_ptr = (OutType)( *ptr_a + *ptr_b );
    }
}

template<typename T> 
inline void wsp::AddArray(T *io_ary, const T *in_add_ary, int length)
{
    T *ptr = io_ary;
    const T *add_ptr = in_add_ary;
    T *end_ptr = io_ary+length;
    for(; ptr!=end_ptr; ++ptr, ++add_ptr){
        *ptr += *add_ptr;
    }
}

template<typename InType, typename OutType> 
inline void wsp::SustractArray(const InType *in_ary_a, const InType *in_ary_b, OutType *o_ary, int length)
{
    const InType *ptr_a = in_ary_a;
    const InType *ptr_b = in_ary_b;
    OutType *o_ptr = o_ary;
    OutType *end_ptr = o_ptr+length;

    for(; o_ptr!=end_ptr; ++o_ptr, ++ptr_a, ++ptr_b){
        *o_ptr = (OutType)(*ptr_a - *ptr_b);
    }
}

template<typename T> 
inline void wsp::SustractArray(T *io_ary, const T *in_subst_ary, int length)
{
    T *ptr = io_ary;
    T *end_ptr = io_ary+length;
    const T *subst_ptr = in_subst_ary;
    for(; ptr!=end_ptr; ++ptr, ++subst_ptr){
        *ptr -= *subst_ptr;
    }
}


template<typename T> 
inline void wsp::CutOff(T *io_ary, int length, T thre_min, T fill_min, T thre_max, T fill_max)
{
    T *ptr=io_ary;
    T *end_ptr=io_ary+length;
    for(; ptr!=end_ptr; ++ptr){
        if(*ptr<thre_min){ *ptr=fill_min; }
        if(*ptr>thre_max){ *ptr=fill_max; }
    }
}

template<typename T> 
inline void wsp::NormalizeArray(T *io_ary, int length, T norm_min, T norm_max, T min, T max)
{
    WSP_REQUIRES(min <= max, "Invaild argument");
    WSP_REQUIRES(norm_min <= norm_max, "Invaild argument");

    if(min==0 && max==0)
    {
        min = wsp::GetMin(io_ary, length);
        max = wsp::GetMax(io_ary, length);
    }

    T *ptr = io_ary;
    T *end_ptr = io_ary + length;
    T range = max - min;
    T norm_range = norm_max - norm_min;
    for( ;ptr!=end_ptr; ++ptr)
    {
        *ptr = (*ptr - min) * norm_range / range + norm_min;
    }
}
template<typename T>
inline void wsp::NormalizeArray_MaxIsOne(T *io_ary, int length, T max_inType)
{
    T value;
    T max_inAry = wsp::GetMax<T>(io_ary, length);
    T min_inAry = wsp::GetMin<T>(io_ary, length);
    if(max_inAry == min_inAry)
    {
        return;
    }

    T mult = max_inType / (max_inAry - min_inAry);
    for(u32 i=0; i<length; i++)
    {
        io_ary[i] = (io_ary[i] - min_inAry) * mult;
    }
}
template<typename T> inline void wsp::NormalizeArray_SumIsOne(T *io_ary, int length){
    T value;
    T max_inAry = wsp::GetMax<T>(io_ary, length);
    T min_inAry = wsp::GetMin<T>(io_ary, length);
    T sum = wsp::GetAbsSum(io_ary, length);
    if(sum==0)
    {
        return;
    }

    for(u32 i=0; i<length; i++)
    {
        value = (io_ary[i] - min_inAry) / sum;
        io_ary[i] = value;
    }
}
template<typename T> inline void wsp::Reset(T *io_ary, int length, T resetValue)
{
    for(u32 i=0; i<length; i++){ io_ary[i] = resetValue; }
}
template<typename T> inline void wsp::Flip2dVer(T *io_ary, int width, int height, int spp)
{
    int x, y, i, length=width*height*spp;
    T *tmp_ary = WSP_NEW T[length];
    wsp::CopyArray(tmp_ary, io_ary, length);
    for(y=0; y<height; y++){
        for(x=0; x<width; x++){
            for(i=0; i<spp; i++){
                io_ary[(y*width+x)*spp+i]=tmp_ary[((height-y-1)*width+x)*spp+i];
            }
        }
    }
    delete[] tmp_ary;
}
template<typename T> inline void wsp::Flip2dHor(T *io_ary, int width, int height, int spp)
{
    int x, y, i, length=width*height*spp;
    T *tmp_ary = WSP_NEW T[length];
    wsp::CopyArray(tmp_ary, io_ary, length);
    for(y=0; y<height; y++){
        for(x=0; x<width; x++){
            for(i=0; i<spp; i++){
                io_ary[(y*width + x)*spp+i] = tmp_ary[(y*width+width-x-1)*spp+i];
            }
        }
    }
    delete[] tmp_ary;
}
template<typename T> inline void wsp::Flip2dHorVer(T *io_ary, int width, int height, int spp)
{
    wsp::Flip2dHor(io_ary, width, height, spp);
    wsp::Flip2dVer(io_ary, width, height, spp);

    //u32 x, y, i, length=width*height;
    //T *tmp_ary = WSP_NEW T[length];
    //wsp::CopyArray(tmp_ary, io_ary, length);
    //for(y=0; y<height; y++){
    //    for(x=0; x<width; x++){
    //        for(i=0; i<spp; i++){
    //            io_ary[(y*width + x)*spp+i] = tmp_ary[((height-y-1)*width+width-x-1)*spp+i];
    //        }
    //    }
    //}
}
template<typename T> inline void wsp::CopyArray(T *o_dst, const T *in_src, int length){
    const T *src_ptr = in_src;
    const T *end_ptr = in_src+length;
    T *dst_ptr = o_dst;
    for(; src_ptr!=end_ptr; ++src_ptr, ++dst_ptr){
        *dst_ptr=*src_ptr;
    }
}
template<typename T, typename U> inline void wsp::CopyArray(T *o_dst, const U *in_src, int length){
    const U *src_ptr = in_src;
    const U *end_ptr = in_src+length;
    T *dst_ptr = o_dst;
    for(; src_ptr!=end_ptr; ++src_ptr, ++dst_ptr){
        *dst_ptr=static_cast<T>(*src_ptr);
    }
}

template<typename T> inline void wsp::ConvertNumOfChannels(const T *in_src, T *o_dst, int length, int dst_num_channels, int src_num_channels)
{
    // copy into 3 channels temporary array and save it
    if(src_num_channels!=dst_num_channels){
        int dst_size = length*dst_num_channels;
        int c;
        memset(o_dst, 0, dst_size);
        T *dst_ptr = o_dst;
        T *src_ptr = const_cast<T*>(in_src);
        T *end_ptr = dst_ptr + dst_size;
        int diff_channel = abs(dst_num_channels - src_num_channels);
        if(src_num_channels<dst_num_channels){
            for(c=0;dst_ptr!=end_ptr; ++dst_ptr, ++src_ptr){
                *dst_ptr = *src_ptr;
                ++c;
                if(c==src_num_channels){
                    dst_ptr+=diff_channel;
                    c=0;
                }
            }
        }else{
            for(c=0;dst_ptr!=end_ptr; ++dst_ptr, ++src_ptr){
                *dst_ptr = *src_ptr;
                ++c;
                if(c==dst_num_channels){
                    src_ptr+=diff_channel;
                    c=0;
                }
            }
        }
    }
}


//! Copy 2d array with swapping like dimension a[5][2] to b[2][5]
template<typename T> inline void wsp::CopyArray2dSwappingDimension(T **out_a, T **in_b, u32 len1d, u32 len2d)
{
    for(u32 i=0; i<len1d; i++){
        for(u32 j=0; j<len2d; j++){
            out_a[j][i] = in_b[i][j];
        }
    }
}
template<typename T> inline void wsp::CopyArray2dSwappingDimension(T out_a[MAX_BUFFER][MAX_BUFFER], T **in_b, u32 len1d, u32 len2d)
{
    for(u32 i=0; i<len1d; i++){
        for(u32 j=0; j<len2d; j++){
            out_a[j][i] = in_b[i][j];
        }
    }
}
template<typename T> inline wsp::State wsp::CopyArray1dAsImage(T *o_img, int width_o, int height_o, const T *in_img, int width_in, int height_in)
{
    if(height_o > height_in){
        WSP_COMMON_ERROR_LOG("in_img height must be equal or larger than o_img height.\n");
        return WSP_STATE_FAILURE;
    }
    for(u32 y=0; y<height_o; y++){
        for(u32 x=0; x<width_o; x++){
            if(x < width_in){
                o_img[y*width_o + x] = in_img[y*width_in + x];
            }else{
                o_img[y*width_o + x] = 0;
            }
        }
    }
    return WSP_STATE_SUCCESS;
}
template<typename T> wsp::State wsp::ToU8Array(uchar *o_ary, const T *in_ary, int length){
    T max, min, value, range;
    int i;
    max = min = in_ary[0];
    for(i=1; i<length; i++){
        if(max<in_ary[i]){ max=in_ary[i]; }
        if(min>in_ary[i]){ min=in_ary[i]; }
    }
    range = max - min;
    if(range==0){
        memset(o_ary, 0, length); return WSP_STATE_SUCCESS;
    }
    //cout<<"ToU8Array: max="<<max<<", min="<<min<<", range="<<range<<endl;

    for(i=0; i<length; i++){
        value = static_cast<T>(static_cast<double>(in_ary[i]-min) * (256.0 / static_cast<double>(range)));
        //cout<<i<<": "<<value<<endl;
        if(0.0<=value && value<=255.0){
            o_ary[i] = static_cast<uchar>(value);
        }else if(value<0.0){
            o_ary[i] = 0;
        }else{
            o_ary[i] = 255;
        }
    }
    return WSP_STATE_SUCCESS;
}

// I/O ------------------------------------------------------------
inline void wsp::PrintArrayAsHex(const uchar *ary, int length){
    const uchar *ptr = ary;
    const uchar *end_ptr = ptr + length;
    for (; ptr!=end_ptr; ++ptr){
        printf("%2.2x ", *ptr);
    }
}
template<typename T> inline void wsp::PrintArray(const T* ary, int length){
    for (u32 itr_i=0; itr_i<length; itr_i++) {
        std::cout<<ary[itr_i]<<" ";
    }
}

template<typename T> 
wsp::State wsp::SaveRawArray(
    const T *in_data, 
    size_t length,
    const char *in_fileNameFormat, ...
)
{
    char filename[MAX_LENGTH_OF_NAME];
    va_list ap;
    va_start(ap, in_fileNameFormat);
    vsprintf(filename, in_fileNameFormat, ap);
    va_end(ap);

    FILE *fp;
    fp = fopen(filename, "wb");
    if (fp==NULL){
        WSP_COMMON_ERROR_LOG("Opening %s failed\n", filename);
        return WSP_STATE_FAILURE;
    }
    fprintf(fp, "%zu %zu\n", sizeof(T), length );
    if(in_data!=NULL){
        size_t size = fwrite(in_data, sizeof(T), length, fp);
        if (size != length) { fclose(fp); WSP_COMMON_ERROR_LOG("fwrite failed\n"); return WSP_STATE_FAILURE; }
    }
   
    fclose(fp);
    printf("%s successfully saved as raw data\n", filename);
    return WSP_STATE_SUCCESS;
}

template<typename T>
wsp::State wsp::LoadRawArray(
    T **o_data, 
    size_t &o_length, 
    const char *fileName
)
{
    const size_t NUM_OF_HEADER_PARAM = 2;
    const size_t LINE_BUF = 70;
    char line[LINE_BUF];
    int num_read = 0;
    const int MAX_NUM = 6;
    int num[MAX_NUM]; //! parameter container
    wsp::common::ScopedOpenFile scoped_open(fileName, "rb");
    FILE *fp = scoped_open.GetOpenedFilePointer();
    if(fp==NULL)
    {
        WSP_COMMON_ERROR_LOG("Opening %s failed\n", fileName);
        return WSP_STATE_FAILURE;
    }

    //! initialize nums
    for(int i=0; i<MAX_NUM; ++i)
    {
        num[i] = -1;
    }

    fgets((char *)line, sizeof(line), fp);

    char *line_ptr = line;
    char *end_ptr  = line_ptr + LINE_BUF;
    for (int i=0; line_ptr<end_ptr; i++, ++line_ptr )
    {
        char c = *line_ptr;
        if (c == '\n'){ break; }
        if (isdigit(c))
        {
            if (num_read >= MAX_NUM)
            {
                return WSP_STATE_FAILURE;
            }

            num[num_read] = c - '0';
            for (; c=line[i+1]; i++)
            {
                if (!isdigit(c))
                {
                    break;
                }

                num[num_read] = 10*num[num_read] + c - '0';
            }
            ++num_read;
        }
    }
    if(num_read != NUM_OF_HEADER_PARAM)
    {
        WSP_COMMON_ERROR_LOG("Number of params in header is wrong: %d\n", num_read); 
        return WSP_STATE_FAILURE; 
    }

    if(num[0]!=sizeof(T))
    {
        WSP_COMMON_ERROR_LOG("Element size is wrong: file data size=%d, current data size=%zu\n", num[0], sizeof(T)); 
        return WSP_STATE_FAILURE;
    }
    o_length = num[1];

    printf("elem size %zu, length %zu\n", sizeof(T), o_length);

    if(o_data!=NULL)
    {
        size_t cnt;
        wsp::ReallocArrayMem<T>(o_data, o_length);
        cnt = fread(*o_data, sizeof(T), o_length, fp);
        if( cnt != o_length)
        {
            WSP_COMMON_ERROR_LOG("Reading data failed %zu!=%zu\n", cnt, o_length);
            return WSP_STATE_FAILURE;
        }
        //printf("data is loaded, ");
    }
    printf("%s successfully loaded\n", fileName);
    return WSP_STATE_SUCCESS;
}

template<typename T> inline void wsp::LoadArray(T *out_ary, size_t length, const char *in_filename)
{
    FILE *fp = fopen(in_filename, "rb");
    size_t cnt = fread(out_ary, sizeof(T), length, fp);
    fclose(fp);
    if( cnt != length){
        WSP_COMMON_ERROR_LOG("wsp::LoadArray failed");
        return;
    }
}
template<typename T> inline wsp::State wsp::Save1dArrayAsText(const T *array_src, int length, int num_channels, char *const filename)
{
    FILE *fp=fopen(filename, "w");
    if(fp == NULL){ return WSP_STATE_FAILURE; }

    const T *ptr = array_src;
    const T *end_ptr = ptr + length*num_channels;
    const T *end_c_ptr;
    if(typeid(T)==typeid(float)
    || typeid(T)==typeid(double)){
        for(; ptr!=end_ptr; ){
            end_c_ptr = ptr + num_channels;
            for(; ptr!=end_c_ptr; ++ptr){
                fprintf(fp, "%f ", *ptr); 
            }
            fprintf(fp, "\n"); 
        }
    }else{
        for(; ptr!=end_ptr; ){
            end_c_ptr = ptr + num_channels;
            for(; ptr!=end_c_ptr; ++ptr){
                fprintf(fp, "%d ", *ptr); 
            }
            fprintf(fp, "\n"); 
        }
    }
    fclose(fp);

    return WSP_STATE_FAILURE;
}
template<typename T> inline wsp::State wsp::Save1dArrayAsPGM(const T *in_src, int length, const char *in_fileNameFormat,...)
{
    char filename[MAX_LENGTH_OF_NAME];
    va_list ap;
    va_start(ap, in_fileNameFormat);
    vsprintf(filename, in_fileNameFormat, ap);
    va_end(ap);

    int size;
    uchar *u8ary;
    FILE *fp;
    uchar max;

    fp = fopen(filename, "wb");
    if (fp==NULL){ return WSP_STATE_FAILURE; }

    u8ary = WSP_NEW uchar[length];
    wsp::ToU8Array<T>(u8ary, in_src, length);

    std::cout<<"Saving as Portable Image.."<<std::endl;

    max = wsp::GetMax<uchar>(u8ary, length);

    fprintf(fp, "P5\n%d %d\n%d\n", length, 1, max);

    size = fwrite(u8ary, sizeof(uchar), length, fp);
    if (size != length) { fclose(fp); delete[] u8ary; return WSP_STATE_FAILURE; }

    fclose(fp);
    delete[] u8ary;
    return WSP_STATE_SUCCESS;    
}
template<typename T> inline wsp::State wsp::Save2dArrayAsPNM(const T *in_src, int width, int height,
                                                            const char *filename, wsp::PNMFormat format)
{
    u32 size_write, size, spp;
    uchar *u8ary;
    FILE *fp;
    uchar max;

    spp =  format==0? 1:3;
    size = width*height*spp;

    fp = fopen(filename, "wb");
    if (fp==NULL){
        WSP_COMMON_ERROR_LOG("Opening %s failed\n", filename);
        return WSP_STATE_FAILURE;
    }

    printf("Saving 2D Array as %s %s\n", format==0?"PGM":"PPM", filename);

    u8ary = WSP_NEW uchar[size];
    if(sizeof(T)!=1){
        wsp::ToU8Array<T>(u8ary, in_src, size);
    }else{
        wsp::CopyArray<uchar, T>(u8ary, in_src, size);
    }

    max = wsp::GetMax<uchar>(u8ary, size);
    if(max==0){ max=1; }

    switch(format){
    case WSP_PNM_FORMAT_GRAYMAP:
        fprintf(fp, "P5\n%d %d\n%d\n", width, height, max);
        break;
    case WSP_PNM_FORMAT_PIXMAP:
        fprintf(fp, "P6\n%d %d\n%d\n", width, height, max);
        break;
    default:
        fclose(fp); delete[] u8ary;
        WSP_COMMON_ERROR_LOG("Unrecognized portable format: %d\n", format);
        return WSP_STATE_FAILURE;
    }
    size_write = fwrite(u8ary, sizeof(uchar), size, fp);
    if (size_write != size) {
        WSP_COMMON_ERROR_LOG("Writing file failed\n");
        fclose(fp); delete[] u8ary; return WSP_STATE_FAILURE;
    }
    printf("Saving as Portable Image successfully done: max=%d\n", max);

    fclose(fp);
    delete[] u8ary;
    return WSP_STATE_SUCCESS;    
}
template<typename T> inline wsp::State wsp::Save2dArrayAsPGM(const T *in_src, int width, int height, const char *in_fileNameFormat,...)
{
    char filename[MAX_LENGTH_OF_NAME];
    va_list ap;
    va_start(ap, in_fileNameFormat);
    vsprintf(filename, in_fileNameFormat, ap);
    va_end(ap);
    return wsp::Save2dArrayAsPNM(in_src, width, height, filename, WSP_PNM_FORMAT_GRAYMAP);
}
template<typename T> inline wsp::State wsp::Save2dArrayAsPPM(const T *in_src, int width, int height, int num_channels, const char *in_fileNameFormat,...)
{
    char filename[MAX_LENGTH_OF_NAME];
    va_list ap;
    va_start(ap, in_fileNameFormat);
    vsprintf(filename, in_fileNameFormat, ap);
    va_end(ap);

    // copy into 3 channels temporary array and save it
    if(num_channels!=3){
        int len = width*height*3;
        T *data = WSP_NEW T[len];
        wsp::ConvertNumOfChannels(in_src, data, width*height, 3, num_channels);
        wsp::State state = wsp::Save2dArrayAsPNM(data, width, height, filename, WSP_PNM_FORMAT_PIXMAP);
        delete[] data;
        return state;
    }

    return wsp::Save2dArrayAsPNM(in_src, width, height, filename, WSP_PNM_FORMAT_PIXMAP);
}



#endif
