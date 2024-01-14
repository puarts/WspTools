/**
* @file tpl_cl-image.inl
* 
* @brief Base class for any type of image class
*/

#ifndef _WSP_IMAGE_TEMPLATE_CLASS_IMAGE_INL_
#define _WSP_IMAGE_TEMPLATE_CLASS_IMAGE_INL_

#include "tpl_cl-image.h"

#ifndef __WSP_COMMONUTIL_FN_UTIL_H__
#include <wsp/common/fn-util.h>
#endif
#include "_fn-imgio.h"
#include "_tpl_fn-imgio.h"
#include "_tpl_fn-imgproc.hpp"
#include "fn-imgio.h"
#include <wsp/common/fn-debug.h>
#include <wsp/math/_tpl_fn-math.h>
#include <wsp/devil/tpl_fn-imgio-gpl.h>
#include <wsp/libpng/fn-png_io.h>

// Constructor, Destructor -------------------------------------------------------------------

#define INIT_WSP_IMAGE \
    alpha_data_(NULL),\
    state_data_(NULL),\
    width_(0),\
    height_(0)

template<typename T, int MemAllocStep> wsp::Image<T, MemAllocStep>::Image()
    :INIT_WSP_IMAGE, wsp::Vector<T>()
{
    SetSize(1, 1);
}
template<typename T, int MemAllocStep> wsp::Image<T, MemAllocStep>::Image(int width, int height, int nChannels)
    :INIT_WSP_IMAGE, wsp::Vector<T>()//, wsp::Vector<T>(width*height*nChannels)
{
    SetSize(width, height, nChannels);
}

template<typename T, int MemAllocStep> template<typename U>
wsp::Image<T, MemAllocStep>::Image(const wsp::Image<U> &in_img, int num_of_channels_dst)
    :INIT_WSP_IMAGE, wsp::Vector<T>()
    //wsp::Image<T, MemAllocStep>(in_img.data(), in_img.width(), in_img.height(), in_img.num_of_channels(), dst_num_of_channels)
{
    //wsp::Vector<T, MemAllocStep>::has_own_data_=true;
    in_img.template CopyIntoAnyImage<T>(*this, num_of_channels_dst);
}
template<typename T, int MemAllocStep> 
template<typename U>
wsp::Image<T, MemAllocStep>::Image(const U *in_data, const uchar *in_alpha, const uchar *in_state, int width, int height, int nChannels)
    :INIT_WSP_IMAGE, wsp::Vector<T>()
{
    CopyFrom(in_data, in_alpha, in_state, width, height, nChannels);
}

//! Copy Constructor
template<typename T, int MemAllocStep> wsp::Image<T, MemAllocStep>::Image(const wsp::Image<T, MemAllocStep>& im)
    :INIT_WSP_IMAGE, wsp::Vector<T>()
{
    if(im.data() != NULL && im.length() != 0){
        CopyFrom(im);
    }
}
template<typename T, int MemAllocStep> wsp::Image<T, MemAllocStep>::Image(int length)
    :INIT_WSP_IMAGE, wsp::Vector<T>()
{
    if(length<=0){ Clear(); return; }
    SetSize(length, 1);
}
//! Destructor
template<typename T, int MemAllocStep> wsp::Image<T, MemAllocStep>::~Image()
{
    wsp::img::FreeImageData<T>(NULL, &alpha_data_, &state_data_);
}
// end Constructor, Destructor ---------------------------------------------------------------


// Accessors: Getter --------------------------------------------------------------------------
template<typename T, int MemAllocStep> int wsp::Image<T, MemAllocStep>::width() const{ return width_; }
template<typename T, int MemAllocStep> int wsp::Image<T, MemAllocStep>::height() const{ return height_; }

template<typename T, int MemAllocStep> int wsp::Image<T, MemAllocStep>::GetImageSize() const{ return height_*width_; }
template<typename T, int MemAllocStep> int wsp::Image<T, MemAllocStep>::GetDataSize() const{ return GetImageSize() * sizeof(T); }
//template<typename T, int MemAllocStep> T* wsp::Image<T, MemAllocStep>::data(){ return wsp::Vector<T, MemAllocStep>::data_; }
template<typename T, int MemAllocStep> uchar* wsp::Image<T, MemAllocStep>::alpha_data(){ return alpha_data_; }
template<typename T, int MemAllocStep> uchar* wsp::Image<T, MemAllocStep>::state_data(){ return state_data_; }
template<typename T, int MemAllocStep> const uchar* wsp::Image<T, MemAllocStep>::alpha_data()const{ return alpha_data_; }
template<typename T, int MemAllocStep> const uchar* wsp::Image<T, MemAllocStep>::state_data()const{ return state_data_; }

template<typename T, int MemAllocStep> T wsp::Image<T, MemAllocStep>::GetElem(int x, int y, int c) const{
    //int index = (y*width_ + x)*wsp::Vector<T, MemAllocStep>::num_of_channels_ + c;
    //if(index<width_*height_){ return wsp::Vector<T, MemAllocStep>::data_[index]; }
    //return wsp::Vector<T, MemAllocStep>::data_[width_*height_-1];
    return wsp::Vector<T, MemAllocStep>::data_[(y*width_+x)*this->wsp::Vector<T, MemAllocStep>::num_of_channels_ + c];
}
//template<typename T, int MemAllocStep> T wsp::Image<T, MemAllocStep>::GetElem(int x, int y, int c) const{
//    return wsp::Image<T, MemAllocStep>::GetElem((int)x, (int)y, (int)c);
//}
template<typename T, int MemAllocStep> T wsp::Image<T, MemAllocStep>::GetElem(int index) const{
    //    return wsp::Image<T, MemAllocStep>::operator[](index);
    return wsp::Vector<T, MemAllocStep>::data_[index];
}
//template<typename T, int MemAllocStep> T wsp::Image<T, MemAllocStep>::GetElem(int index) const{
//    return wsp::Image<T, MemAllocStep>::operator[](static_cast<int>(index));
//}
template<typename T, int MemAllocStep> uchar wsp::Image<T, MemAllocStep>::GetAlpha(int index) const{
    //if(alpha_data_==NULL){ return 0; }
    //if(index<wsp::Vector<T, MemAllocStep>::length_){ return alpha_wsp::Vector<T, MemAllocStep>::data_[index]; }
    //return alpha_data_[width_*height_-1];
    return alpha_data_[index];
}
//template<typename T, int MemAllocStep> uchar wsp::Image<T, MemAllocStep>::GetAlpha(int index) const{
//    return wsp::Image<T, MemAllocStep>::GetAlpha(static_cast<int>(index));
//}
template<typename T, int MemAllocStep> uchar wsp::Image<T, MemAllocStep>::GetState(int index) const{
    //if(state_data_==NULL){ return 0; }
    //if(index<width_*height_){    return state_data_[index]; }
    //return state_data_[width_*height_-1];
    return state_data_[index];
}
//template<typename T, int MemAllocStep> uchar wsp::Image<T, MemAllocStep>::GetState(int index) const{
//    return wsp::Image<T, MemAllocStep>::GetState(static_cast<int>(index));
//}
template<typename T, int MemAllocStep> uchar wsp::Image<T, MemAllocStep>::GetAlpha(int x, int y) const{
    //if(alpha_data_==NULL){ return 0; }
    //int index = y*width_+x;
    //return GetAlpha(index);
    return alpha_data_[y*width_+x];
}
template<typename T, int MemAllocStep> uchar wsp::Image<T, MemAllocStep>::GetState(int x, int y) const{
    if(x>=width_ || x<0 || y>=height_ || y<0){
        WSP_COMMON_ERROR_LOG("The coordinate(%d, %d) is out of range: (w, h)=(%d, %d)\n", x, y, width_, height_);
        return 0;
    }
    //if(state_data_==NULL){ return 0; }
    //int index = y*width_+x;
    //return GetState(index);
    return state_data_[y*width_+x];
}
//template<typename T, int MemAllocStep> uchar wsp::Image<T, MemAllocStep>::GetAlpha(int x, int y) const{
//    return GetAlpha((int)x, (int)y);
//}
//template<typename T, int MemAllocStep> uchar wsp::Image<T, MemAllocStep>::GetState(int x, int y) const{
//    return GetState((int)x, (int)y);
//}
template<typename T, int MemAllocStep> void wsp::Image<T, MemAllocStep>::CopyAlphaInto(wsp::Image<uchar> &o_img) const{
    o_img.SetSize(width_, height_, 1);
    o_img.Reset(255);
    for(int i=0; i<wsp::Vector<T, MemAllocStep>::length_; i++){
        if(alpha_data_!=NULL){
            o_img[i] = alpha_data_[i];
        }
    }
}
template<typename T, int MemAllocStep> void wsp::Image<T, MemAllocStep>::CopyStateInto(wsp::Image<uchar> &o_img) const{
    o_img.SetSize(width_, height_, 1);
    o_img.Reset(WSP_PIXEL_STATE_NORMAL);
    for(int i=0; i<wsp::Vector<T, MemAllocStep>::length_; i++){
        if(state_data_!=NULL){
            o_img[i] = state_data_[i];
        }
    }
}

template<typename T, int MemAllocStep> bool wsp::Image<T, MemAllocStep>::IsOutOfBounds(int x, int y, int c) const{
    return (x>=static_cast<int>(width_) || y>=static_cast<int>(height_) || x<0 || y<0 || c>=static_cast<int>(wsp::Vector<T, MemAllocStep>::num_of_channels_) || c<0);
}

template<typename T, int MemAllocStep> T wsp::Image<T, MemAllocStep>::GetMin(uchar alpha_thre) const{
    if(wsp::Vector<T, MemAllocStep>::length_==0 || wsp::Vector<T, MemAllocStep>::data_==NULL){ return (T)0; }
    T min = wsp::Vector<T, MemAllocStep>::data_[0];
    int itr_i, itr_c, index;
    for(itr_i=0; itr_i<wsp::Vector<T, MemAllocStep>::length_; itr_i++)
    {
        for(itr_c=0; itr_c<wsp::Vector<T, MemAllocStep>::num_of_channels_; itr_c++)
        {
            index = itr_i*wsp::Vector<T, MemAllocStep>::num_of_channels_+itr_c;
            if(wsp::Vector<T, MemAllocStep>::data_[index] < min && alpha_data_[itr_i]>=alpha_thre){
                min = wsp::Vector<T, MemAllocStep>::data_[index];
            }
        }
    }
    return min;
}
template<typename T, int MemAllocStep> T wsp::Image<T, MemAllocStep>::GetMax(uchar alpha_thre) const{
    if(wsp::Vector<T, MemAllocStep>::length_==0 || wsp::Vector<T, MemAllocStep>::data_==NULL){ return (T)0; }
    T max = wsp::Vector<T, MemAllocStep>::data_[0];
    int itr_i, itr_c, index;
    for(itr_i=0; itr_i<wsp::Vector<T, MemAllocStep>::length_; itr_i++){
        for(itr_c=0; itr_c<wsp::Vector<T, MemAllocStep>::num_of_channels_; itr_c++){
            index = itr_i*wsp::Vector<T, MemAllocStep>::num_of_channels_+itr_c;
            if(wsp::Vector<T, MemAllocStep>::data_[index] > max && alpha_data_[itr_i]>=alpha_thre){
                max = wsp::Vector<T, MemAllocStep>::data_[index];
            }
        }
    }
    return max;
}
template<typename T, int MemAllocStep> void wsp::Image<T, MemAllocStep>::GetMaxLoc(int *o_x, int *o_y, T *o_value) const{
    if(width_*height_==0 || wsp::Vector<T, MemAllocStep>::data_==NULL){ return; }
    int maxX=0, maxY=0;
    T maxVal = wsp::Vector<T, MemAllocStep>::data_[0];
    for(int itr_y=0; itr_y<height_; itr_y++){
        for(int itr_x=0; itr_x<width_; itr_x++){
            if(GetElem((int)itr_x, (int)itr_y) > maxVal){
                maxVal = GetElem((int)itr_x, (int)itr_y);
                maxX = itr_x;
                maxY = itr_y;
            }
        }
    }
    if(o_value != NULL){ *o_value = maxVal; }
    if(o_x != NULL){ *o_x = maxX; }
    if(o_y != NULL){ *o_y = maxY; }
}
template<typename T, int MemAllocStep> void wsp::Image<T, MemAllocStep>::GetMinLoc(int *o_x, int *o_y, T *o_value) const{
    if(width_*height_==0 || wsp::Vector<T, MemAllocStep>::data_==NULL){ return; }
    int minX=0, minY=0;
    T minVal = wsp::Vector<T, MemAllocStep>::data_[0];
    for(int itr_y=0; itr_y<height_; itr_y++){
        for(int itr_x=0; itr_x<width_; itr_x++){
            if(GetElem((int)itr_x, (int)itr_y) < minVal){
                minVal = GetElem((int)itr_x, (int)itr_y);
                minX = itr_x;
                minY = itr_y;
            }
        }
    }
    if(o_value!=NULL){ *o_value = minVal; }
    if(o_x!=NULL){ *o_x = minX; }
    if(o_y!=NULL){ *o_y = minY; }
}
template<typename T, int MemAllocStep> void wsp::Image<T, MemAllocStep>::GetMaxLoc(double *o_x, double *o_y, T *o_value) const{
    int px, py;
    GetMaxLoc(&px, &py, o_value);
    if(o_x!=NULL){ *o_x = (double)px; }
    if(o_y!=NULL){ *o_y = (double)py; }
}
template<typename T, int MemAllocStep> void wsp::Image<T, MemAllocStep>::GetMinLoc(double *o_x, double *o_y, T *o_value) const{
    int px, py;
    GetMinLoc(&px, &py, o_value);
    if(o_x!=NULL){ *o_x = (double)px; }
    if(o_y!=NULL){ *o_y = (double)py; }
}
template<typename T, int MemAllocStep> void wsp::Image<T, MemAllocStep>::GetMaxIndex(int *o_index, T *o_value) const{
    double px, py;
    GetMaxLoc(&px, &py, o_value);
    if(o_index!=NULL){ *o_index = (int)(py*width_ + px); }
}

template<typename T, int MemAllocStep> template<typename U>
void wsp::Image<T, MemAllocStep>::CopyIntoAnyImage(wsp::Image<U> &o_dst, int num_of_channels_dst) const
{
    int i, c, nc = static_cast<int>(num_of_channels_dst);
    int len = static_cast<int>(wsp::Vector<T, MemAllocStep>::length_);

    if(num_of_channels_dst==0){ nc = wsp::Vector<T, MemAllocStep>::num_of_channels_; }

    o_dst.SetSize(width_, height_, nc);
    wsp::Image<T, MemAllocStep> normImg(*this);

    //#ifdef _OPENMP
    //#pragma omp parallel for private(i, c)
    //#endif
    for(i=0; i<len; i++){
        for(c=0; c<nc; c++){
            if(c<wsp::Vector<T, MemAllocStep>::num_of_channels_){
                o_dst[i*nc+c] = static_cast<U>(wsp::Vector<T, MemAllocStep>::data_[i*wsp::Vector<T, MemAllocStep>::num_of_channels_+c]);
                continue;
            }
            o_dst[i*nc+c] = static_cast<U>(wsp::Vector<T, MemAllocStep>::data_[i*wsp::Vector<T, MemAllocStep>::num_of_channels_+wsp::Vector<T, MemAllocStep>::num_of_channels_-1]);
        }
        o_dst.SetAlpha(alpha_data_[i], i);
        o_dst.SetState(state_data_[i], i);
    }
}
template<typename T, int MemAllocStep> template<typename U> 
void wsp::Image<T, MemAllocStep>::CopyIntoAnyImageAsU8(wsp::Image<U> &o_dst) const
{
    int i, c, spp;
    int len = static_cast<int>(wsp::Vector<T, MemAllocStep>::length_);

    spp = static_cast<int>(wsp::Vector<T, MemAllocStep>::num_of_channels_*sizeof(T));
    o_dst.SetSize(width_, height_, spp);
    uchar *data = (uchar*)wsp::Vector<T, MemAllocStep>::data_;
    //#ifdef _OPENMP
    //#pragma omp parallel for private(i, c)
    //#endif
    for(i=0; i<len; i++){
        for(c=0; c<spp; c++){
            o_dst[i*wsp::Vector<T, MemAllocStep>::num_of_channels_+c] = (U)wsp::Vector<T, MemAllocStep>::data_[i*spp+c];
        }
        o_dst.SetAlpha(alpha_data_[i], i);
        o_dst.SetState(state_data_[i], i);
    }
}
template<typename T, int MemAllocStep> template<typename U> 
bool wsp::Image<T, MemAllocStep>::IsTheSameSize(const wsp::Image<U> &in_img) const
{
    return (width_==in_img.width() && height_==in_img.height() && wsp::Vector<T, MemAllocStep>::num_of_channels_==in_img.num_of_channels());
}
template<typename T, int MemAllocStep>
int wsp::Image<T, MemAllocStep>::GetNumOfIndex() const{
    wsp::Vector<int> indexList;
    wsp::Image<T, MemAllocStep>::GetIndexList(indexList);
    return (int)indexList.length();
}

template<typename T, int MemAllocStep>
wsp::State wsp::Image<T, MemAllocStep>::GetIndexList(wsp::Vector<int> &o_indexList) const
{
    // êÆêîå^ÇÃÇ∆Ç´ÇµÇ©égÇ¶Ç»Ç¢ä÷êî
    int index, i, c;
    o_indexList.SetLength(0);
    for(i=0; i<wsp::Vector<T, MemAllocStep>::length_; ++i)
    {
        index=0;
        for(c = 0; c < wsp::Vector<T, MemAllocStep>::num_of_channels_; ++c)
        {
            index += static_cast<int>(wsp::Vector<T, MemAllocStep>::data_[i]) << (c * 8);
        }
        if(o_indexList.HasElem(index)==false)
        {
            o_indexList.Append(index);
        }
    }
    return WSP_STATE_SUCCESS;    
}


// end Accessors: Getter ----------------------------------------------------------------------

// Accessors: Setter --------------------------------------------------------------------------

template<typename T, int MemAllocStep>
void wsp::Image<T, MemAllocStep>::CopyFrom(
    const void* data, 
    const uchar* alpha, 
    const uchar* state, 
    wsp::DataType type, int width, int height, int num_of_channels
){
    int num_elem = width*height*num_of_channels;
    this->SetSize(width, height, num_of_channels);
    T *dst_ptr = wsp::Vector<T, MemAllocStep>::data_;
    T *end_ptr = dst_ptr + num_elem;
    switch(type){
    case wsp::DATATYPE_8U:{
        const uchar *src_ptr=reinterpret_cast<const uchar*>(data);
        for(; dst_ptr!=end_ptr; ++src_ptr, ++dst_ptr){ *dst_ptr = static_cast<T>(*src_ptr); }
                            }
                            break;
    case wsp::DATATYPE_16S:{
        const short *src_ptr=reinterpret_cast<const short*>(data);
        for(; dst_ptr!=end_ptr; ++src_ptr, ++dst_ptr){ *dst_ptr = static_cast<T>(*src_ptr); }
                            }
                            break;
    case wsp::DATATYPE_64F:{
        const double *src_ptr=reinterpret_cast<const double*>(data);
        for(; dst_ptr!=end_ptr; ++src_ptr, ++dst_ptr){ *dst_ptr = static_cast<T>(*src_ptr); }
                            }
                            break;
    }
}

template<typename T, int MemAllocStep>
void wsp::Image<T, MemAllocStep>::NormalizeAlpha(uchar min_norm, uchar max_norm)
{
    if(alpha_data_==NULL)
    { 
        WSP_COMMON_WARNING_LOG("alpha is NULL\n"); 
        return; 
    }

    //#ifdef _OPENMP
    //#pragma omp parallel
    //#endif
    {
        //#ifdef _OPENMP
        //#pragma omp for
        //#endif
        uchar *ptr_alpha = alpha_data_;
        uchar *end_ptr = alpha_data_ + wsp::Vector<T, MemAllocStep>::length_;
        uchar min=alpha_data_[0], max=alpha_data_[0];
        for(; ptr_alpha!=end_ptr; ptr_alpha++){
            if(min>*ptr_alpha){ min=*ptr_alpha; }
            if(max<*ptr_alpha){ max=*ptr_alpha; }
        }
        printf("Normalize Alpha %d-%d: min=%d, max=%d\n", min_norm, max_norm, min, max);
        if(min==max){ WSP_COMMON_WARNING_LOG("min and max is the same value\n"); return; }
        uchar range=max-min, range_norm=max_norm-min_norm;
        //#ifdef _OPENMP
        //#pragma omp for
        //#endif
        ptr_alpha = alpha_data_;
        for(; ptr_alpha!=end_ptr; ptr_alpha++){
            *ptr_alpha = (*ptr_alpha-min)*range_norm/range + min_norm;
        }
    }
}

template<typename T, int MemAllocStep>
wsp::Result wsp::Image<T, MemAllocStep>::Shift( s32 x, s32 y )
{
    wsp::img::ShiftImage( wsp::Vector<T, MemAllocStep>::data_, width_, height_, wsp::Vector<T, MemAllocStep>::num_of_channels_, x, y );
    return wsp::ResultSuccess();
}


template<typename T, int MemAllocStep>
wsp::Result wsp::Image<T, MemAllocStep>::BlendImage( 
    const wsp::Image<T> &in_image, 
    WSP_ImageBlendMode blend_mode  
)
{
    if( wsp::Vector<T, MemAllocStep>::num_of_channels_ != in_image.num_of_channels() )
    {
        WSP_IMAGE_ERROR_LOG("wsp::Vector<T, MemAllocStep>::num_of_channels_ = %d, in_image.num_of_channels() = %d", wsp::Vector<T, MemAllocStep>::num_of_channels_, in_image.num_of_channels());
        return wsp::ResultInvalidArgument();
    }

    wsp::img::BlendImage( 
        wsp::Vector<T, MemAllocStep>::data_, in_image.data(), in_image.alpha_data(), 
        GetWidthStep(), height_, wsp::Vector<T, MemAllocStep>::num_of_channels_, blend_mode );

    //s32 src_width = in_image.width();
    //s32 src_height = in_image.height();
    //s32 src_num_channels = in_image.num_of_channels();
    //for( s32 y=0; (y<height_) && (y<src_height); ++y )
    //{
    //    for( s32 x=0; (x<width_) && (x<src_width); ++x )
    //    {
    //        s32 i = (y * width_ + x) * wsp::Vector<T, MemAllocStep>::num_of_channels_;
    //        s32 src_i = (y * src_width + x) * src_num_channels;
    //        f32 src_ratio = (f32)in_image.GetAlpha( src_i ) / 255.0f;
    //        for( s32 c=0; c<wsp::Vector<T, MemAllocStep>::num_of_channels_; ++c )
    //        {
    //            wsp::Vector<T, MemAllocStep>::data_[i + c] = wsp::Vector<T, MemAllocStep>::data_[i + c] * (1.0f-src_ratio) + in_image[src_i + c] * src_ratio;
    //        }
    //    }
    //}

    //s32 num_channels = in_image.num_of_channels()
    //s32 size = in_image.width() * in_image.height() * in_image.num_of_channels();
    //const u8* alpha_ptr = in_image.alpha_data();
    //const U* iter_ptr = &in_image[0];
    //const U* end_ptr  = &in_image[0] + size;
    //const U* end_c_ptr;
    //T* out_ptr = wsp::Vector<T, MemAllocStep>::data_;
    //T* 
    //for( ; iter_ptr<end_ptr; ++alpha_ptr )
    //{
    //    end_c_ptr = iter_ptr + num_channels;
    //    for( ; iter_ptr<end_c_ptr; ++iter_ptr)
    //    {
    //    }
    //}
    return wsp::ResultSuccess();
}

template<typename T, int MemAllocStep> 
template<typename U>
wsp::State wsp::Image<T, MemAllocStep>::CopyFrom(const wsp::Image<U>& img, int dst_num_of_channels)
{
    //WSP_COMMON_DEBUG_LOG("data = %08x, alpha_data = %08x, state_data = %08x", img.data(), img.alpha_data(), img.state_data());

    return CopyFrom<U>(
        img.data(), img.alpha_data(), img.state_data(), 
        img.width(), img.height(), img.num_of_channels(), dst_num_of_channels);
}
template<typename T, int MemAllocStep> 
template<typename U>
wsp::State wsp::Image<T, MemAllocStep>::CopyFrom(
    const U* data, const uchar* alpha, const uchar* state, 
    int width, int height, int src_num_of_channels, int dst_num_of_channels
)
{
    if(wsp::Vector<T, MemAllocStep>::has_own_data_==false)
    { 
        WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); 
        return WSP_STATE_FAILURE; 
    }

    if( (data==NULL&&alpha==NULL&&state==NULL) 
     || height*width*src_num_of_channels==0)
    {
        WSP_COMMON_ERROR_LOG("Invalid parameter\n"); 
        return WSP_STATE_INVALID_PARAMETER;
    }

    int i, c, nc = static_cast<int>(src_num_of_channels);
    if(dst_num_of_channels != 0)
    {
        nc = static_cast<int>(dst_num_of_channels);
    }

    SetSize(width, height, nc);

    int len = static_cast<int>(wsp::Vector<T, MemAllocStep>::length_);

    for(i=0; i<len; i++)
    {
        for(c=0; c<nc; c++)
        {
            if(c<src_num_of_channels)
            {
                wsp::Vector<T, MemAllocStep>::data_[i*wsp::Vector<T, MemAllocStep>::num_of_channels_+c] = static_cast<T>(data[i*src_num_of_channels+c]);
                continue;
            }
            wsp::Vector<T, MemAllocStep>::data_[i*wsp::Vector<T, MemAllocStep>::num_of_channels_+c] = static_cast<T>(data[i*src_num_of_channels+src_num_of_channels-1]);
        }
    }

    if(alpha!=NULL)
    {
        memcpy(alpha_data_, alpha, wsp::Vector<T, MemAllocStep>::length_*sizeof(uchar));
    }

    if(state!=NULL)
    {
        memcpy(state_data_, state, wsp::Vector<T, MemAllocStep>::length_*sizeof(uchar));
    }

    return WSP_STATE_SUCCESS;
}
template<typename T, int MemAllocStep>
void wsp::Image<T, MemAllocStep>::SetDataFrom(T *data, int width, int height, int num_of_channels){
    SetDataFrom(data, NULL, NULL, width, height, num_of_channels);
    //delete[] wsp::Vector<T, MemAllocStep>::data_;
    //wsp::Vector<T, MemAllocStep>::data_=data;
    //width_ = width;
    //height_ = height;
    //wsp::Vector<T, MemAllocStep>::length_ = width_*height_;
    //wsp::Vector<T, MemAllocStep>::num_of_channels_ = num_of_channels;
    //wsp::Vector<T, MemAllocStep>::has_own_data_=false;
}
template<typename T, int MemAllocStep>
void wsp::Image<T, MemAllocStep>::SetDataFrom(T *data, uchar *alpha, uchar *state, int width, int height, int num_of_channels){
    //delete[] wsp::Vector<T, MemAllocStep>::data_; delete[] alpha_data_; delete[] state_data_;
    //free(wsp::Vector<T, MemAllocStep>::data_); free(alpha_data_); free(state_data_);
    Clear();
    wsp::Vector<T, MemAllocStep>::data_=data; alpha_data_=alpha; state_data_=state;
    width_ = width;
    height_ = height;
    wsp::Vector<T, MemAllocStep>::length_ = width_*height_;
    wsp::Vector<T, MemAllocStep>::num_of_channels_ = num_of_channels;
    wsp::Vector<T, MemAllocStep>::has_own_data_=false;
}
template<typename T, int MemAllocStep>
void wsp::Image<T, MemAllocStep>::SetDataInto(T *o_data, int *o_width, int *o_height, int *o_num_of_channels)
{
    SetDataInto(
        &o_data,
        NULL,
        NULL,
        o_width,
        o_height,
        o_num_of_channels);
    //o_data = wsp::Vector<T, MemAllocStep>::data_;
    //if(o_width!=NULL){ *o_width=width_; }
    //if(o_height!=NULL){ *o_height=height_; }
    //if(o_num_of_channels!=NULL){ *o_num_of_channels=wsp::Vector<T, MemAllocStep>::num_of_channels_; }
    //wsp::Vector<T, MemAllocStep>::data_=NULL;
    //width_=0; height_=0; wsp::Vector<T, MemAllocStep>::num_of_channels_=0; wsp::Vector<T, MemAllocStep>::length_=0;
    //wsp::Vector<T, MemAllocStep>::has_own_data_=true;

    //delete[] alpha_data_; delete[] state_data_;
    //alpha_data_=NULL; state_data_=NULL;
}
template<typename T, int MemAllocStep>
void wsp::Image<T, MemAllocStep>::SetDataInto(T **o_data, uchar **o_alpha, uchar **o_state, int *o_width, int *o_height, int *o_num_of_channels){
    *o_data = wsp::Vector<T, MemAllocStep>::data_;
    wsp::Vector<T, MemAllocStep>::data_=NULL;
    if(o_alpha!=NULL){
        *o_alpha = alpha_data_;
        alpha_data_=NULL; 
    }
    if(o_state!=NULL){ 
        *o_state = state_data_; 
        state_data_=NULL;
    }
    if(o_width!=NULL){ *o_width=width_; }
    if(o_height!=NULL){ *o_height=height_; }
    if(o_num_of_channels!=NULL){ *o_num_of_channels=wsp::Vector<T, MemAllocStep>::num_of_channels_; }
    width_=0; height_=0; wsp::Vector<T, MemAllocStep>::num_of_channels_=0; wsp::Vector<T, MemAllocStep>::length_=0;
    wsp::Vector<T, MemAllocStep>::has_own_data_=true;
}
//template<typename T, int MemAllocStep> void wsp::Image<T, MemAllocStep>::SetAlpha(const uchar* alpha, int width, int height){
//    if(alpha==NULL || height*width==0){
//        return;
//    }
//    delete[] alpha_data_;
//    width_ = width;
//    height_ = height;
//    wsp::Vector<T, MemAllocStep>::length_ = width*height;
//    alpha_data_ = WSP_NEW uchar[width_*height_ + RESERVE_MEM];
//    for(int y=0; y<height; y++){
//        for(int x=0; x<width; x++){
//            alpha_data_[y*width + x] = alpha[y*width + x];
//        }
//    }
//}
//template<typename T, int MemAllocStep> void wsp::Image<T, MemAllocStep>::SetState(const uchar* state, int width, int height){
//    if(state==NULL || height*width==0){
//        return;
//    }
//    delete[] state_data_;
//    width_ = width;
//    height_ = height;
//    wsp::Vector<T, MemAllocStep>::length_ = width*height;
//    state_data_ = WSP_NEW uchar[width_*height_ + RESERVE_MEM];
//    for(int y=0; y<height; y++){
//        for(int x=0; x<width; x++){
//            state_data_[y*width + x] = state[y*width + x];
//        }
//    }
//}
template<typename T, int MemAllocStep> void wsp::Image<T, MemAllocStep>::SetElem(const T& element, int x, int y, int channel){
    int index = (y*width_+x)*wsp::Vector<T, MemAllocStep>::num_of_channels_+channel;
    wsp::Image<T, MemAllocStep>::SetElem(element, index);
}
template<typename T, int MemAllocStep> void wsp::Image<T, MemAllocStep>::SetElem(const T& element, int index){
    if(wsp::Vector<T, MemAllocStep>::data_==NULL || width_*height_==0){
        return;
    }
    if(index<width_*height_*wsp::Vector<T, MemAllocStep>::num_of_channels_){
        wsp::Vector<T, MemAllocStep>::data_[index]=element;
    }
}
template<typename T, int MemAllocStep> void wsp::Image<T, MemAllocStep>::SetAlpha(uchar element, int x, int y){
    int index = y*width_+x;
    wsp::Image<T, MemAllocStep>::SetAlpha(element, index);
}
template<typename T, int MemAllocStep> void wsp::Image<T, MemAllocStep>::SetAlpha(uchar element, int index){
    if(alpha_data_==NULL || width_*height_==0){
        return;
    }
    if(index<width_*height_){
        alpha_data_[index]=element;
    }
}
template<typename T, int MemAllocStep> void wsp::Image<T, MemAllocStep>::AddElem(const T& element, int x, int y, int channel){
    if(wsp::Vector<T, MemAllocStep>::data_==NULL || width_*height_*wsp::Vector<T, MemAllocStep>::num_of_channels_==0){
        return;
    }
    int index = (y*width_+x)*wsp::Vector<T, MemAllocStep>::num_of_channels_+channel;
    if(index<wsp::Vector<T, MemAllocStep>::length_*wsp::Vector<T, MemAllocStep>::num_of_channels_){
        wsp::Vector<T, MemAllocStep>::data_[index]+=element;
    }
}
template<typename T, int MemAllocStep> 
void wsp::Image<T, MemAllocStep>::AddAlpha(uchar element, int x, int y)
{
    if(alpha_data_==NULL || width_*height_==0)
    {
        return;
    }
    int index = y*width_+x;
    if(index<width_*height_){
        if((int)alpha_data_[index]+(int)element >= 255){
            alpha_data_[index] = 255;
        }else{
            alpha_data_[index] += element;
        }
    }
}
template<typename T, int MemAllocStep> void wsp::Image<T, MemAllocStep>::SetState(uchar element, int x, int y){
    wsp::Image<T, MemAllocStep>::SetState(element, y*width_+x);
}
template<typename T, int MemAllocStep> 
void wsp::Image<T, MemAllocStep>::SetState(uchar element, int index)
{
    if(index>=wsp::Vector<T, MemAllocStep>::length_ || index<0){
        WSP_COMMON_WARNING_LOG("Given index is out of range\n"); return;
    }
    state_data_[index]=element;
}

//template<typename T, int MemAllocStep> wsp::State wsp::Image<T, MemAllocStep>::SetSize(int width, int height, int nChannels){
//    if(width<0 || height<0 || nChannels<0){
//        WSP_COMMON_ERROR_LOG("Arguments must be more than zero\n"); return WSP_STATE_FAILURE;
//    }
//    return SetSize(static_cast<int>(width), static_cast<int>(height), static_cast<int>(nChannels));
//}

template<typename T, int MemAllocStep> 
wsp::State wsp::Image<T, MemAllocStep>::SetSize(
    int width, int height, int nChannels, bool set_new_channels_to_zero
)
{
    if(width_==width && height_==height && wsp::Vector<T, MemAllocStep>::num_of_channels_==nChannels)
    { 
        return WSP_STATE_SUCCESS; 
    }

    if(wsp::Vector<T, MemAllocStep>::has_own_data_==false )
    { 
        WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); 
        return WSP_STATE_FAILURE; 
    }

    if(width==0||height==0)
    { 
        Clear(); 
        return WSP_STATE_SUCCESS; 
    }

    wsp::State state;
    int length_old, nc_old, num_of_elem_old;
    //wsp::Image<T, MemAllocStep> tmpImg(*this);

    length_old = width_*height_;
    nc_old = wsp::Vector<T, MemAllocStep>::num_of_channels_;
    num_of_elem_old = length_old*nc_old;

    width_ = width;
    height_ = height;
    if(nChannels>0){ wsp::Vector<T, MemAllocStep>::num_of_channels_ = nChannels; }
    wsp::Vector<T, MemAllocStep>::length_ = width_*height_;
    wsp::Vector<T, MemAllocStep>::num_of_elem_ = wsp::Vector<T, MemAllocStep>::length_*wsp::Vector<T, MemAllocStep>::num_of_channels_;

    //int mem_size_old = static_cast<int>(ceil(static_cast<float>(wsp::Vector<T, MemAllocStep>::num_of_elem_old)/static_cast<float>(MemAllocStep))*MemAllocStep);
    int mem_size_new = static_cast<int>(ceil(static_cast<float>(wsp::Vector<T, MemAllocStep>::num_of_elem_)/static_cast<float>(MemAllocStep)))*MemAllocStep;
    if(mem_size_new==wsp::Vector<T, MemAllocStep>::mem_size_){ return WSP_STATE_SUCCESS; }

    state = wsp::img::ReallocImageData(
        &this->wsp::Vector<T, MemAllocStep>::data_,
        &alpha_data_, 
        &state_data_, 
        mem_size_new, 
        wsp::Vector<T, MemAllocStep>::length_, 
        wsp::Vector<T, MemAllocStep>::num_of_channels_, 
        length_old,
        nc_old, 
        set_new_channels_to_zero
        );
    if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Memory allocation has failed\n"); return state; }

    wsp::Vector<T, MemAllocStep>::mem_size_ = mem_size_new;

    return WSP_STATE_SUCCESS;
}

template<typename T, int MemAllocStep> 
wsp::Result wsp::Image<T, MemAllocStep>::ScaleImage( u32 width, u32 height )
{
    wsp::Image<T> tmp_img( width, height, wsp::Vector<T, MemAllocStep>::num_of_channels_ );
    wsp::img::InterpolationAlgorithm algorithm = WSP_INTERPOLATION_NEAREST_NEIGHBOR;
    wsp::img::ScaleImage( 
        wsp::Vector<T, MemAllocStep>::data_, width_, height_, wsp::Vector<T, MemAllocStep>::num_of_channels_, 
        tmp_img.data(), tmp_img.width(), tmp_img.height(), algorithm );
    *this = tmp_img;
    return wsp::ResultSuccess();
}


template<typename T, int MemAllocStep> 
wsp::State wsp::Image<T, MemAllocStep>::ResizeCanvas(int width, int height, u8 expand_alpha_val )
{
    wsp::Image<T, MemAllocStep> tmp_img(*this);
    wsp::State state;
    int w_old = width_, h_old = height_;

    state = SetSize(width, height);
    if(state!=WSP_STATE_SUCCESS){
        WSP_COMMON_ERROR_LOG("Setting size failed\n");
        return state;
    }

    {
        // unoptimized code -----------------------
        int x, y, c, i, i_old;
        int w_min = width<w_old? width : w_old;
        int h_min = height<h_old? height : h_old;
        for(y=0; y<h_min; ++y)
        {
            for(x=0; x<w_min; ++x)
            {
                i = (y*width + x)*wsp::Vector<T, MemAllocStep>::num_of_channels_;
                i_old = (y*w_old + x)*wsp::Vector<T, MemAllocStep>::num_of_channels_;
                for(c=0; c<wsp::Vector<T, MemAllocStep>::num_of_channels_; ++c)
                {
                    wsp::Vector<T, MemAllocStep>::data_[i+c] = tmp_img[i_old+c];
                }
            }

            i = (y*width + w_min);
#define FILL_BG_WITH_BLACK
#ifdef FILL_BG_WITH_BLACK
            memset( &this->wsp::Vector<T, MemAllocStep>::data_[i*wsp::Vector<T, MemAllocStep>::num_of_channels_], 0, sizeof(T)*(width-w_min)*wsp::Vector<T, MemAllocStep>::num_of_channels_);
#else
            // store any color in expanded area
            for(; x<width; ++x)
            {
                i = (y*width + x)*wsp::Vector<T, MemAllocStep>::num_of_channels_;
                for(c=0; c<wsp::Vector<T, MemAllocStep>::num_of_channels_; ++c)
                {
                    wsp::Vector<T, MemAllocStep>::data_[i+c] = 0;
                }
            }
#endif
            memset( &alpha_data_[i], expand_alpha_val, (width-w_min) );
            memset( &state_data_[i], 0x00, (width-w_min) );
        }
#ifdef FILL_BG_WITH_BLACK
        i = h_min*width;
        memset( &this->wsp::Vector<T, MemAllocStep>::data_[i*wsp::Vector<T, MemAllocStep>::num_of_channels_], 0, sizeof(T)*(height-h_min)*(width)*wsp::Vector<T, MemAllocStep>::num_of_channels_);
#else
        for(; y<height; ++y)
        {
            for( x=0; x<width; ++x)
            {
                i = (y*width + x)*wsp::Vector<T, MemAllocStep>::num_of_channels_;
                for(c=0; c<wsp::Vector<T, MemAllocStep>::num_of_channels_; ++c)
                {
                    wsp::Vector<T, MemAllocStep>::data_[i+c] = 0;
                }
            }
        }
#endif
        memset( &alpha_data_[i], expand_alpha_val, (height-h_min)*(width) );
        memset( &state_data_[i], 0x00, (height-h_min)*(width) );
    }
    return WSP_STATE_SUCCESS;
}

template<typename T, int MemAllocStep> 
wsp::State wsp::Image<T, MemAllocStep>::SetSizeToPowerOfTwo(bool set_also_height)
{
    int w_size = wsp::math::GetNearestPowerOfTwo(width_);
    int h_size = height_;
    if(set_also_height){
        h_size = wsp::math::GetNearestPowerOfTwo(height_);
    }
    return SetSize(w_size, h_size);
}

template<typename T, int MemAllocStep> 
wsp::State wsp::Image<T, MemAllocStep>::Clear()
{
    if(wsp::Vector<T, MemAllocStep>::has_own_data_==false)
    { 
        WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); 
        return WSP_STATE_FAILURE; 
    }
    //delete[] wsp::Vector<T, MemAllocStep>::data_; delete[] alpha_data_; delete[] state_data_;
    wsp::img::FreeImageData(&this->wsp::Vector<T, MemAllocStep>::data_, &alpha_data_, &state_data_);
    width_ = 0;
    height_ = 0;
    wsp::Vector<T, MemAllocStep>::length_ = 0;
    wsp::Vector<T, MemAllocStep>::num_of_elem_ = 0;
    wsp::Vector<T, MemAllocStep>::mem_size_ = 0;
    return WSP_STATE_SUCCESS;
}
template<typename T, int MemAllocStep> 
void wsp::Image<T, MemAllocStep>::Invert()
{
    //if(sizeof(T)>4){ return; }
    T pix, dmax = this->GetMax();
    int i;
    int len=wsp::Vector<T, MemAllocStep>::length_*wsp::Vector<T, MemAllocStep>::num_of_channels_;
    switch(sizeof(T)){
    case 8:
        //! if image is double image
        for(i=0; i<len; i++){
            wsp::Vector<T, MemAllocStep>::data_[i]*=-1;
        }
        //cout<<dmax<<endl;
        break;
    default:
        for(i=0; i<len; i++){
            pix = dmax - wsp::Vector<T, MemAllocStep>::data_[i];
            wsp::Vector<T, MemAllocStep>::data_[i] = pix;
        }
    }
}

template<typename T, int MemAllocStep> 
void wsp::Image<T, MemAllocStep>::ResetAlpha(const uchar alpha)
{
    if(alpha_data_==NULL){ return; }
    for(int i=0; i<wsp::Vector<T, MemAllocStep>::length_; i++){
        alpha_data_[i] = alpha;
    }
}
template<typename T, int MemAllocStep> 
void wsp::Image<T, MemAllocStep>::ResetState(const wsp::img::PixelState pixStat)
{
    if(state_data_==NULL)
    { 
        return; 
    }
    for(int i=0; i<wsp::Vector<T, MemAllocStep>::length_; i++){
        state_data_[i] = pixStat;
    }
}

template<typename T, int MemAllocStep> 
void wsp::Image<T, MemAllocStep>::ResetTargetChannel( T value, s32 target_channel_index )
{
    T *ptr = wsp::Vector<T, MemAllocStep>::data_ + target_channel_index;
    T *end_ptr = wsp::Vector<T, MemAllocStep>::data_ + width_*height_*wsp::Vector<T, MemAllocStep>::num_of_channels_;
    for( ; ptr<end_ptr; ptr+=wsp::Vector<T, MemAllocStep>::num_of_channels_ )
    {
        *ptr = value;
    }
}


template<typename T, int MemAllocStep> 
void wsp::Image<T, MemAllocStep>::SetTransparentStateByAlpha(uchar thre_alpha)
{
    for(int i=0; i<wsp::Vector<T, MemAllocStep>::length_; i++)
    {
        if(alpha_data_[i]<thre_alpha){
            state_data_[i]=WSP_PIXEL_STATE_TRANSPARENT;
            continue;
        }
        state_data_[i]=WSP_PIXEL_STATE_NORMAL;
    }
}
template<typename T, int MemAllocStep> void wsp::Image<T, MemAllocStep>::SetAlphaAtTargetStatePix(uchar alpha, wsp::img::PixelState target_state){
    for(int i=0; i<wsp::Vector<T, MemAllocStep>::length_; i++){
        if(state_data_[i]==target_state){
            alpha_data_[i]=alpha;
        }
    }
}
template<typename T, int MemAllocStep>
void wsp::Image<T, MemAllocStep>::SetValueAtTargetStatePixel(const T *in_fill_pixel_data, wsp::img::PixelState target_state)
{
    int i, c;
    for(i=0; i<wsp::Vector<T, MemAllocStep>::length_; i++){
        if(state_data_[i]==target_state){
            //printf("target state detected!\n");
            for(c=0; c<wsp::Vector<T, MemAllocStep>::num_of_channels_; c++){
                wsp::Vector<T, MemAllocStep>::data_[i*wsp::Vector<T, MemAllocStep>::num_of_channels_+c] = in_fill_pixel_data[c];
            }
        }
    }

}
template<typename T, int MemAllocStep> 
void wsp::Image<T, MemAllocStep>::Normalize(
    T min_norm, T max_norm, u8 thre_alpha, T min_src, T max_src)
{
    T min, max;
    if(min_src!=max_src){
        min = min_src;
        max = max_src;
    }else{
        min = this->GetMin(thre_alpha);
        max = this->GetMax(thre_alpha);
    }
    
    T range=max-min, range_norm=max_norm-min_norm;
    if(range==0 || range_norm==0){
        return;
    }

    int len = static_cast<int>(wsp::Vector<T, MemAllocStep>::length_*wsp::Vector<T, MemAllocStep>::num_of_channels_);
    printf("Normalize: \n"
        "    min=%f,      max=%f,      range=%f\n"
        "    min_norm=%f, max_norm=%f, range_norm=%f\n", 
        (double)min, (double)max, (double)range,
        (double)min_norm, (double)max_norm, (double)range_norm
        );
    
    for (int i = 0; i<len; ++i)
    {
        wsp::Vector<T, MemAllocStep>::data_[i] = range_norm*(wsp::Vector<T, MemAllocStep>::data_[i]-min)/range + min_norm;
    }
}

template<typename T, int MemAllocStep> 
void wsp::Image<T, MemAllocStep>::Transpose()
{
    wsp::Image<T, MemAllocStep> tmp_img(*this);
    this->SetSize(height_, width_, wsp::Vector<T, MemAllocStep>::num_of_channels_);

    s32 x, y, c, i_src, i_dst;
    for (y = 0; y<height_; ++y)
    {
        for (x = 0; x<width_; ++x)
        {
            i_src = x*height_ + y;
            i_dst = y*width_ + x;
            for (c = 0; c<wsp::Vector<T, MemAllocStep>::num_of_channels_; c++)
            {
                wsp::Vector<T, MemAllocStep>::data_[i_dst*wsp::Vector<T, MemAllocStep>::num_of_channels_ + c] = tmp_img[i_src*wsp::Vector<T, MemAllocStep>::num_of_channels_ + c];
            }
        }
    }
}

// end Accessors ------------------------------------------------------------------------------

//! IO -----------------------------------------------------------------------------------------

template<typename T, int MemAllocStep>
wsp::State wsp::Image<T, MemAllocStep>::Load(const char *filename, bool as_alpha)
{
    char ext[MAX_LENGTH_OF_NAME];
    wsp::GetExtension(ext, filename);
    if(strcmp(ext, "bin")==0 || strcmp(ext, "raw")==0){ 
        //printf("file type is raw or bin\n");
        return LoadRawData(filename); 
    }else if(strcmp(ext, "adri")==0){ 
        //printf("file type is adri\n");
        if(as_alpha){
            return LoadADRI(NULL, filename);
        }
        return LoadADRI(filename); 
    }
    if(as_alpha){
        return LoadImageFileAsAlpha(filename);
    }
    return LoadImageFile(filename);
}

template<typename T, int MemAllocStep>
wsp::State wsp::Image<T, MemAllocStep>::Save(const char *filename) const
{
    char ext[MAX_LENGTH_OF_NAME];
    wsp::GetExtension(ext, filename);
    if(strcmp(ext, "bin")==0 || strcmp(ext, "raw")==0){ 
        //printf("file type is raw or bin\n");
        return SaveRawData(filename); 
    }else if(strcmp(ext, "adri")==0){ 
        //printf("file type is adri\n");
        return SaveAsADRI(filename); 
    }
    return SaveImage(filename);
}

template<typename T, int MemAllocStep> 
wsp::State wsp::Image<T, MemAllocStep>::LoadADRI(const char *filename, const char *filename_alpha, int nChannels)
{
    if(wsp::Vector<T, MemAllocStep>::has_own_data_==false)
    { 
        WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); 
        return WSP_STATE_FAILURE; 
    }
    int i, c;
    int tmp_w, tmp_h, tmp_nc;
    int alpha_w, alpha_h, alpha_nc;
    wsp::State stat;
    T *tmp_data=NULL;
    uchar *alpha_data=NULL;

    if(filename!=NULL){
        stat=wsp::img::LoadADRI(&tmp_data, tmp_w, tmp_h, tmp_nc, filename);
        if(stat!=WSP_STATE_SUCCESS){
            WSP_COMMON_ERROR_LOG("Loading image failed\n");
            wsp::FreeArrayMem(&tmp_data); wsp::FreeArrayMem(&alpha_data); Clear(); return stat;
        }
    }
    if(filename_alpha!=NULL){
        stat=wsp::img::LoadADRI<uchar>(&alpha_data, alpha_w, alpha_h, alpha_nc, filename_alpha);
        if(stat!=WSP_STATE_SUCCESS){
            wsp::FreeArrayMem(&tmp_data); wsp::FreeArrayMem(&alpha_data); Clear();
            WSP_COMMON_ERROR_LOG("Loading alpha data failed\n"); return stat;
        }
        if(filename!=NULL){
            if(tmp_w!=alpha_w || tmp_h!=alpha_h){
                wsp::FreeArrayMem(&tmp_data); wsp::FreeArrayMem(&alpha_data); Clear();
                WSP_COMMON_ERROR_LOG("The alpha data size is different from data size\n"); return WSP_STATE_INVALID_PARAMETER;
            }
        }else{
            tmp_w = alpha_w;
            tmp_h = alpha_h;
            tmp_nc = static_cast<int>(wsp::Vector<T, MemAllocStep>::num_of_channels_);
        }
    }
    if(nChannels!=0){
        SetSize(tmp_w, tmp_h, nChannels);
    }else{
        SetSize(tmp_w, tmp_h, tmp_nc);
    }
    //! Copying image data
    if(tmp_data!=NULL){
        for(i=0; i<wsp::Vector<T, MemAllocStep>::length_; i++){
            for(c=0; c<wsp::Vector<T, MemAllocStep>::num_of_channels_; c++){
                if(c<tmp_nc){
                    wsp::Vector<T, MemAllocStep>::data_[i*wsp::Vector<T, MemAllocStep>::num_of_channels_+c] = static_cast<T>(tmp_data[i*tmp_nc+c]);
                    //data[i*spp+j]=(T)tmp_data[i*nChannel+j];
                    continue;
                }
                wsp::Vector<T, MemAllocStep>::data_[i*wsp::Vector<T, MemAllocStep>::num_of_channels_+c] = (T)tmp_data[i*tmp_nc+tmp_nc-1];
                //data[i*spp+j]=(T)tmp_data[i*nChannel+nChannel-1];
            }
        }
    }
    //wsp::Vector<T, MemAllocStep>::data_ = (T*)data;

    printf("width=%d, height=%d, nChannels=%d\n", width_, height_, wsp::Vector<T, MemAllocStep>::num_of_channels_);

    //! set alpha channel to default if it is NULL
    if(alpha_data!=NULL){
        for(i=0; i<wsp::Vector<T, MemAllocStep>::length_; i++){
            alpha_data_[i] = alpha_data[i];
        }
    }

    memset(state_data_, WSP_PIXEL_STATE_NORMAL, wsp::Vector<T, MemAllocStep>::length_);

    wsp::FreeArrayMem(&tmp_data);
    wsp::FreeArrayMem(&alpha_data);
    return WSP_STATE_SUCCESS;
}

template<typename T, int MemAllocStep> wsp::State wsp::Image<T, MemAllocStep>::SaveAsADRI(const char *filename, const char *filename_alpha) const
{
    wsp::State state;
    if(filename!=NULL){
        state = wsp::img::SaveAsADRI(wsp::Vector<T, MemAllocStep>::data_, width_, height_, wsp::Vector<T, MemAllocStep>::num_of_channels_, filename);
        if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Saving %s failed\n", filename); return state; }
    }
    if(filename_alpha!=NULL){
        state = wsp::img::SaveAsADRI(alpha_data_, width_, height_, 1, filename_alpha);
        if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Saving %s failed\n", filename_alpha); return state; }
    }
    return WSP_STATE_SUCCESS;
}


template<typename T, int MemAllocStep> 
wsp::State wsp::Image<T, MemAllocStep>::LoadImageFileAsAlpha(const char *filename)
{
    if(wsp::Vector<T, MemAllocStep>::has_own_data_==false)
    { 
        WSP_IMAGE_ERROR_LOG("You can not reallocate image data because current data is external data\n");
        return WSP_STATE_FAILURE; 
    }
#ifdef USE_DEVIL
    wsp::State state;
    uchar *tmp_data=NULL;
    int w, h, nc;
    //stat=wsp::img::LoadImageFileAsU8(&tmp_data, NULL, w, h, nChannel, filename);
    state = wsp::devil::ilLoadRGBAImage(&tmp_data, w, h, nc, filename, 0, 0, MemAllocStep);
    if(state!=WSP_STATE_SUCCESS)
    {
        WSP_IMAGE_ERROR_LOG("Loading image failed\n");
        return state;
    }
    SetSize(w, h);
    for(int i=0; i<wsp::Vector<T, MemAllocStep>::length_; i++){
        alpha_data_[i]=tmp_data[i*nc];
    }
    wsp::FreeArrayMem(&tmp_data);
#else
    WSP_FATAL("Not implemented");
#endif
    return WSP_STATE_SUCCESS;
}

template<typename T, int MemAllocStep> 
wsp::State wsp::Image<T, MemAllocStep>::LoadImageFile(const char *filename, int nChannels)
{
    if(wsp::Vector<T, MemAllocStep>::has_own_data_==false)
    { 
        WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); 
        return WSP_STATE_FAILURE; 
    }
#ifdef USE_DEVIL

    wsp::State state;
    int old_len = wsp::Vector<T, MemAllocStep>::length_;

    this->Clear();
    if( nChannels == 4 )
    {
        state = wsp::devil::ilLoadRGBAImage(
            &this->wsp::Vector<T, MemAllocStep>::data_,
            width_,
            height_,
            wsp::Vector<T, MemAllocStep>::num_of_channels_,
            filename, 
            wsp::Vector<T, MemAllocStep>::length_,
            wsp::Vector<T, MemAllocStep>::num_of_channels_,
            MemAllocStep );

        wsp::ReallocArrayMemByStep(
            &alpha_data_,
            wsp::Vector<T, MemAllocStep>::length_,
            0,
            0xff,
            MemAllocStep);

        if (num_of_channels_ == 3)
        {
            SetSize(width_, height_, nChannels);
            this->ResetTargetChannel(0xff, 3);
        }
    }
    else
    {
        state = wsp::devil::ilLoadRGBImageAndAlpha(
            &this->wsp::Vector<T, MemAllocStep>::data_, &alpha_data_, width_, height_, wsp::Vector<T, MemAllocStep>::num_of_channels_, filename, 
            wsp::Vector<T, MemAllocStep>::length_, wsp::Vector<T, MemAllocStep>::num_of_channels_, MemAllocStep);
    }

    if(state!=WSP_STATE_SUCCESS){
        WSP_COMMON_ERROR_LOG("Loading %s failed\n", filename); 
        Clear(); 
        return state;
    }
    wsp::Vector<T, MemAllocStep>::length_ = width_*height_;
    wsp::Vector<T, MemAllocStep>::num_of_elem_ = wsp::Vector<T, MemAllocStep>::length_*wsp::Vector<T, MemAllocStep>::num_of_channels_;
    mem_size_ = wsp::GetMemSize(wsp::Vector<T, MemAllocStep>::num_of_elem_, MemAllocStep);
    wsp::ReallocArrayMem(&state_data_, GetMemSize(wsp::Vector<T, MemAllocStep>::length_, MemAllocStep), old_len, WSP_PIXEL_STATE_NORMAL);
#else
    if(sizeof(T)!=1 && sizeof(T)!=3){
        WSP_COMMON_ERROR_LOG("Number of channels %zu is invalid for image format\n", sizeof(T));
        return WSP_STATE_FAILURE;
    }

    wsp::State stat;
    uchar *tmp_data=NULL, *data=NULL;
    int width, height, num_channels, width_step;
    SeqInfo seq_info;
    wsp::GetSeqInfo(seq_info, filename);
    if (stricmp("png", seq_info.ext) == 0)
    {
        wsp::libpng::LibpngResult result = wsp::libpng::LoadPng(&tmp_data, &width, &height, &num_channels, &width_step, filename);
        if (result != wsp::libpng::LIBPNG_RESULT_NO_ERROR)
        {
            WSP_IMAGE_ERROR_LOG("Loading image failed with error code LibpngResult %d\n", result);
            return WSP_STATE_FAILURE;
        }

        //WSP_ASSERT(width == width_step, "width step is not supported\n");

        CopyFrom(tmp_data, NULL, NULL, width, height, num_channels, 4);

        wsp::libpng::FreePng(&tmp_data);
    }
    else if (stricmp("bmp", seq_info.ext) == 0)
    {
        stat = wsp::img::LoadBitmapImage(&tmp_data, &width, &height, &num_channels, filename);
        if (stat != WSP_STATE_SUCCESS)
        {
            WSP_IMAGE_ERROR_LOG("Loading image failed with error code wsp::State %d\n", stat);
            return WSP_STATE_FAILURE;
        }

        CopyFrom(tmp_data, NULL, NULL, width, height, num_channels, num_channels);

        wsp::img::FreeBitmapImage(&tmp_data);
    }
    else if (stricmp("pnm", seq_info.ext) == 0)
    {
        stat = wsp::img::LoadPNM(&tmp_data, &width, &height, filename);
        if (stat != WSP_STATE_SUCCESS)
        {
            WSP_IMAGE_ERROR_LOG("Loading image failed\n");
            width_ = 0; height_ = 0; wsp::Vector<T, MemAllocStep>::length_ = 0;
            wsp::Vector<T, MemAllocStep>::data_ = NULL; alpha_data_ = NULL; state_data_ = 0;
            return stat;
        }

        num_channels = 3;

        CopyFrom(tmp_data, NULL, NULL, width, height, num_channels);
        wsp::img::FreePNM(&tmp_data);
    }
    else
    {
        return WSP_STATE_UNSUPPORTED_TYPE;
    }

#endif

    return WSP_STATE_SUCCESS;
}
template<typename T, int MemAllocStep> 
wsp::State wsp::Image<T, MemAllocStep>::SaveImage(const char *filename, wsp::img::PixelState red_state) const
{
#ifdef USE_DEVIL
    wsp::State state;

    // fill up target pixels with red and save
    if(red_state!=WSP_PIXEL_STATE_NORMAL){
        wsp::Image<T, MemAllocStep> tmp_img(*this, 3);
        uchar *state_ptr=state_data_;
        uchar *end_ptr=state_data_+wsp::Vector<T, MemAllocStep>::length_;
        T *data_ptr =tmp_img.data();
        for(; state_ptr!=end_ptr; ++state_ptr, data_ptr+=3){
            if(*state_ptr==red_state){
                *data_ptr=255; *(data_ptr+1)=0; *(data_ptr+2)=0;
            }
        }
        state = tmp_img.SaveImage(filename);
    }else{
        state = wsp::devil::ilSaveAs8BitImage(wsp::Vector<T, MemAllocStep>::data_, alpha_data_, width_, height_, wsp::Vector<T, MemAllocStep>::num_of_channels_, filename);
    }
    if(state==WSP_STATE_SUCCESS){
        printf("%s successfully saved\n", filename);
    }
#else
    SeqInfo seq_info;
    wsp::GetSeqInfo(seq_info, filename);
    if (stricmp("png", seq_info.ext) == 0)
    {
        wsp::Image<u8, MemAllocStep> tmp_img(*this, 4);
        if (this->num_of_channels() == 3)
        {
            tmp_img.ResetTargetChannel(0xff, 3);
        }
        wsp::libpng::LibpngResult result = wsp::libpng::SavePng(tmp_img.data(), tmp_img.width(), tmp_img.height(), tmp_img.num_of_channels(), 4 * tmp_img.width(), filename);
        if (result != wsp::libpng::LIBPNG_RESULT_NO_ERROR)
        {
            WSP_IMAGE_ERROR_LOG("Saving image failed with error code LibpngResult %d\n", result);
            return WSP_STATE_FAILURE;
        }
    }
    else if ((stricmp("pgm", seq_info.ext) == 0) || (stricmp("ppm", seq_info.ext) == 0))
    {
        if (sizeof(T) != 1 && sizeof(T) != 3) {
            WSP_COMMON_ERROR_LOG("Number of channels %zd is invalid for image format\n", sizeof(T));
            return WSP_STATE_FAILURE;
        }
        WSP_ImageState state;
        uchar *data = (uchar*)wsp::Vector<T, MemAllocStep>::data_;
        int spp = sizeof(T);
        if (red_state != WSP_PIXEL_STATE_NORMAL) {
            uchar *tmp_data = WSP_NEW uchar[3 * wsp::Vector<T, MemAllocStep>::length_];
            //! fill up occluded pixel in red and save as RGB image
            for (int i = 0; i < wsp::Vector<T, MemAllocStep>::length_; i++) {
                if (state_data_[i] == red_state) {
                    tmp_data[i * 3] = 255; tmp_data[i * 3 + 1] = 0; tmp_data[i * 3 + 2] = 0;
                    continue;
                }
                for (int j = 0; j < 3; j++) {
                    if (j < spp) {
                        tmp_data[i * 3 + j] = data[i*spp + j];
                        continue;
                    }
                    tmp_data[i * 3 + j] = data[i*spp + spp - 1];
                }
            }
            state = WSP_SaveU24AsPPM(tmp_data, width_, height_, filename);
            delete[] tmp_data;
        }
        else {
            switch (sizeof(T)) {
            case 1:
                state = WSP_SaveU8AsPGM(data, width_, height_, filename);
                break;
            case 3:
                state = WSP_SaveU24AsPPM(data, width_, height_, filename);
                break;
            default:
                WSP_COMMON_ERROR_LOG("Number of channels %zu is invalid for image format\n", sizeof(T));
                return WSP_STATE_FAILURE;
            }
        }
        if (state != WSP_IMAGE_STATE_SUCCESS) { WSP_COMMON_ERROR_LOG("Saving %s failed\n", filename); return WSP_STATE_FAILURE; }
    }
    else
    {
        WSP_COMMON_ERROR_LOG("Invalid extension %s\n", seq_info.ext);
        return WSP_STATE_FAILURE;
    }
#endif
    return WSP_STATE_SUCCESS;
}

template<typename T, int MemAllocStep> wsp::State wsp::Image<T, MemAllocStep>::SaveAlpha(const char *filename) const{
    return wsp::devil::ilSaveAs8BitImage(alpha_data_, NULL, width_, height_, 1, filename);
}
template<typename T, int MemAllocStep> wsp::State wsp::Image<T, MemAllocStep>::SaveState(const char *filename) const{
    return wsp::devil::ilSaveAs8BitImage(state_data_, NULL, width_, height_, 1, filename);
}
template<typename T, int MemAllocStep> 
wsp::State wsp::Image<T, MemAllocStep>::SaveState(const char *filename, wsp::img::PixelState state) const
{
    wsp::Image<uchar> tmp_img(width_, height_, 1);
    tmp_img.Reset(0);
    for(int i=0; i<wsp::Vector<T, MemAllocStep>::length_; i++){
        if(state_data_[i]==state){ tmp_img[i]=255; }
    }
    return tmp_img.SaveImage(filename);
}

template<typename T, int MemAllocStep> wsp::State wsp::Image<T, MemAllocStep>::SaveRawData(const char *in_fileNameFormat,...) const{
    char filename[MAX_LENGTH_OF_NAME];
    va_list ap;
    va_start(ap, in_fileNameFormat);
    vsprintf(filename, in_fileNameFormat, ap);
    va_end(ap);
    return wsp::img::SaveRawData<T>(wsp::Vector<T, MemAllocStep>::data_, alpha_data_, state_data_, width_, height_, wsp::Vector<T, MemAllocStep>::num_of_channels_, filename);
}
template<typename T, int MemAllocStep> wsp::State wsp::Image<T, MemAllocStep>::LoadRawData(const char *fileName){
    wsp::State stat;
    stat=wsp::img::LoadRawData<T>(&this->wsp::Vector<T, MemAllocStep>::data_, &alpha_data_, &state_data_, width_, height_, wsp::Vector<T, MemAllocStep>::num_of_channels_, fileName);
    wsp::Vector<T, MemAllocStep>::length_ = width_*height_;
    return stat;
}

// Operators ---------------------------------------------------------------------------------
template<typename T, int MemAllocStep> 
wsp::Image<T, MemAllocStep>& wsp::Image<T, MemAllocStep>::operator=(const wsp::Image<T, MemAllocStep>& rhs)
{
    if( *this != rhs)
    {
        this->CopyFrom(rhs);
    }

    return *this;
}

template<typename T, int MemAllocStep> 
bool wsp::Image<T, MemAllocStep>::operator==(const wsp::Image<T, MemAllocStep> &rhs) const
{
    if (width_ != rhs.width() ||
        height_ != rhs.height() ||
        wsp::Vector<T, MemAllocStep>::num_of_channels_ != rhs.num_of_channels())
    {
        return false;
    }

    if (rhs.data() == wsp::Vector<T, MemAllocStep>::data_)
    {
        return true;
    }

    if (wsp::Vector<T>::operator != (rhs))
    {
        return false;
    }

    s32 length = width_ * height_ * wsp::Vector<T, MemAllocStep>::num_of_channels_;
    for (s32 itr_i = 0; itr_i<length; ++itr_i)
    {
        if ( alpha_data_[itr_i] != rhs.GetAlpha(itr_i) ||
             state_data_[itr_i] != rhs.GetState(itr_i) )
        {
            return false;
        }
    }

    return true;
}

template<typename T, int MemAllocStep> 
bool wsp::Image<T, MemAllocStep>::operator!=(const wsp::Image<T, MemAllocStep> &rhs) const
{
    return !(*this==rhs);
}

#endif
