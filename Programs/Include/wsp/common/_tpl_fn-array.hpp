/**
 * @file _tpl_fn-array.hpp
 * 
 */

#ifndef __WSP_COMMONUTIL_TEMPLATE_BASE_FN_ARRAY_H__
#define __WSP_COMMONUTIL_TEMPLATE_BASE_FN_ARRAY_H__

#ifndef __WSP_COMMONUTIL_DEFINE_COMMONUTIL_H__
#include "_define_commonutil.h"
#endif



namespace wsp{
    // Getter Functions ----------------------------------
    template<typename T, typename U> inline U GetSum(const T* in_ary, int length);
    template<typename T, typename U> inline U GetAbsSum(const T* in_ary, int length);
    template<typename T, typename U> inline U GetAverage(const T* in_ary, int length);
    template<typename T> inline T GetSum(const T* in_ary, int length){ return GetSum<T, T>(in_ary, length); }
    template<typename T> inline T GetAbsSum(const T* in_ary, int length){ return GetAbsSum<T, T>(in_ary, length); }
    template<typename T> inline T GetAverage(const T* in_ary, int length){ return GetAverage<T, T>(in_ary, length); }
    template<typename T> inline T GetMax(const T* in_ary, int length);
    template<typename T> inline T GetMin(const T* in_ary, int length);
    template<typename T> inline void GetMinMax(const T* in_ary, T &min, T &max, int length);
    template<typename T> inline int GetMaxIndex(const T* in_ary, int length);
    template<typename T> inline int GetMinIndex(const T* in_ary, int length);
    template<typename T> inline void GetMax(T* o_max_value, int* o_max_index, const T* in_ary, int length);
    template<typename T> inline double GetMeanVariance(const T* in_ary, int length);
    template<typename T> inline double GetMeanStdDev(const T* in_ary, int length);
    template<typename T> bool IsEqual(const T *in_ary_a, const T *in_ary_b, int length);
    template<typename T> int Find(const T *in_ary, T target_elem, int length);

    // Setter Functions ----------------------------------
    template<typename InType, typename OutType> 
    inline void AddArray(const InType *in_ary_a, const InType *in_ary_b, OutType *o_ary, int length);

    template<typename InType, typename OutType> 
    inline void SustractArray(const InType *in_ary_a, const InType *in_ary_b, OutType *o_ary, int length);

    template<typename T> inline void AddArray(T *io_ary, const T *in_add_ary, int length);
    template<typename T> inline void SustractArray(T *io_ary, const T *in_subst_ary, int length);

    template<typename T> inline void NormalizeArray(T *io_ary, int length, T norm_min = 0, T norm_max = 1, T min = 0, T max = 0);
    template<typename T> inline void NormalizeArray_MaxIsOne(T *io_array, int length, T max_inType);
    template<typename T> inline void NormalizeArray_SumIsOne(T *io_array, int length);
    template<typename T> inline void Flip2dVer(T *io_ary, int width, int height, int spp=1);
    template<typename T> inline void Flip2dHor(T *io_ary, int width, int height, int spp=1);
    template<typename T> inline void Flip2dHorVer(T *io_ary, int width, int height, int spp=1);
    template<typename T> inline void Reset(T *io_ary, int length, T resetValue);
    template<typename T> inline void CutOff(T *io_ary, int length, T thre_min, T fill_min=0, T thre_max=255, T fill_max=0);

    // Copy, Convert -------------------------------------
    template<typename T> inline void CopyArray(T *out_a, const T *in_b, int length);
    template<typename T, typename U> inline void CopyArray(T *out_a, const U *in_b, int length);
    template<typename T> inline void ConvertNumOfChannels(const T *in_src, T *o_dst, int length, int dst_num_channel, int src_num_channel);
    template<typename T> inline wsp::State CopyArray1dAsImage(T *out_img, int width_o, int height_o, const T *in_img, int width_in, int height_in);
    template<typename T> inline void CopyArray2dSwappingDimension(T **out_a, T **in_b, u32 len1d, u32 len2d);
    template<typename T> inline void CopyArray2dSwappingDimension(T out_a[MAX_BUFFER][MAX_BUFFER], T **in_b, u32 len1d, u32 len2d);
    template<typename T> wsp::State ToU8Array(uchar *o_ary, const T *in_ary, int length);

    // I/O ------------------------------------------------

    template<typename T> inline void PrintArray(const T* ary, int length);
    inline void PrintArrayAsHex(const uchar *ary, int length);
    template<typename T> wsp::State SaveRawArray     ( 
                                        const T *in_data, 
                                        size_t length,
                                        const char *in_fileNameFormat, ...
                                    );
    template<typename T> wsp::State LoadRawArray     ( 
                                        T **o_data, 
                                        size_t &o_length, 
                                        const char *fileName
                                    );
    template<typename T> void       LoadArray        ( T *out_ary, size_t length, const char *in_filename);
    template<typename T> wsp::State Save1dArrayAsText( const T *in_data, int length, int num_channels, char *const filename);
    template<typename T> wsp::State Save1dArrayAsPGM ( const T *in_src,  int length, const char* filename, ...);
    template<typename T> wsp::State Save2dArrayAsPNM ( const T *in_src,  int width,  int height, const char *filename, wsp::PNMFormat format); //! format 0 is PGM, 1 is PPM
    template<typename T> wsp::State Save2dArrayAsPGM ( const T *in_src,  int width,  int height, const char* filename, ...);
    template<typename T> wsp::State Save2dArrayAsPPM ( const T *in_src,  int width,  int height, int num_channels, const char* filename, ...);
}

#include "_tpl_fn-array.inl"

#endif
