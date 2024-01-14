/**
 * @file tpl_cl-vector.inl
 * @author Junichi Nishikata
 * @brief Base class for vector type class
 */

#ifndef __WSP_CORE_TEMPLATE_CLASS_VECTOR_TEMPORARY_INL__
#define __WSP_CORE_TEMPLATE_CLASS_VECTOR_TEMPORARY_INL__

#include "tpl_cl-vector_temporary.h"
#include "fn-debug.h"

#ifndef __WSP_COMMONCORE_COMMON_RESULT_H__
#include "common_result.h"
#endif

#include <stdarg.h>

// Constructor, Destructor -------------------------------------------------------------------

template<class T, int MemAllocStep>
wsp::VectorTemp<T, MemAllocStep>::VectorTemp()
    : data_(NULL),
    length_(0),
    num_of_channels_(1),
    num_of_elem_(0),
    mem_size_(0),
    has_own_data_(true)
{}
template<class T, int MemAllocStep>
wsp::VectorTemp<T, MemAllocStep>::VectorTemp(int length, int num_of_channels)
    : VectorTemp()
{
    SetLength(length, num_of_channels);
    //if(length!=0 && num_of_channels!=0){
    //    //length_ = length;
    //    //num_of_channels_ = num_of_channels;
    //    //num_of_elem_ = length_*num_of_channels_;
    //    //data_ = new T[length_*num_of_channels_+RESERVE_MEM];
    //    //data_ = (T*)calloc(length_*num_of_channels_, sizeof(T));
    //    //wsp::AllocArrayMem(&data_, num_of_elem_);
    //    //Reset((T)0);
    //    SetLength(length, num_of_channels);
    //}
}
template<class T, int MemAllocStep>
wsp::VectorTemp<T, MemAllocStep>::VectorTemp(const T* ary, int length, int num_of_channels)
    : VectorTemp()
{
    this->CopyFrom(ary, length, num_of_channels, 0);
    //if(ary != NULL && length != 0){
    //    SetLength(length, num_of_channels);
    //    int i, c;
    //    for(int i=0; i<length_; i++){
    //        for(c=0; c<num_of_channels_; c++){
    //            data_[i*num_of_channels_+c] = ary[i*num_of_channels_+c];
    //        }
    //    }
    //}
}

//! Operator= Constructor
template<class T, int MemAllocStep>
wsp::VectorTemp<T, MemAllocStep>::VectorTemp(const wsp::VectorTemp<T, MemAllocStep>& vec)
    : VectorTemp()
{
    *this = vec;
}
//! Destructor
template<class T, int MemAllocStep>
wsp::VectorTemp<T, MemAllocStep>::~VectorTemp()
{
    //printf("wsp::VectorTemp Destructor\n");
    if(has_own_data_){
        //delete[] data_;
        //free(data_);
        wsp::FreeArrayMem(&data_);
        //printf("wsp::VectorTemp destroyed\n");
    }
}
// end Constructor, Destructor ---------------------------------------------------------------


// Accessors: Getter --------------------------------------------------------------------------
template<class T, int MemAllocStep> 
inline int wsp::VectorTemp<T, MemAllocStep>::length() const
{ 
    return length_; 
}

template<class T, int MemAllocStep> 
inline int wsp::VectorTemp<T, MemAllocStep>::num_of_channels() const
{ 
    return num_of_channels_; 
}

template<class T, int MemAllocStep> 
inline T* wsp::VectorTemp<T, MemAllocStep>::data()
{ 
    return data_; 
}

template<class T, int MemAllocStep> 
inline const T* wsp::VectorTemp<T, MemAllocStep>::data() const
{ 
    return data_; 
}

template<class T, int MemAllocStep> 
inline T wsp::VectorTemp<T, MemAllocStep>::GetElem(int index) const
{
    //if(length_==0 || num_of_channels_==0){ return (T)0; }
    //if(index<length_*num_of_channels_){ return data_[index]; }
    //return data_[length_*num_of_channels_-1];
    return data_[index];
}
//template<class T, int MemAllocStep> inline T wsp::VectorTemp<T, MemAllocStep>::GetElem(int index) const{
//    if(index < 0){ return T(0); }
//    return GetElem(static_cast<int>(index));
//}
template<class T, int MemAllocStep> 
inline T wsp::VectorTemp<T, MemAllocStep>::GetMax(int channel) const
{
    if(length_==0 || data_==NULL){ return (T)0; }
    T max = data_[0];
    T *ptr, *end_ptr;
    int len = length_*num_of_channels_;
    if(channel==-1){
        ptr = data_;
        end_ptr = ptr + len;
        for(; ptr!=end_ptr; ++ptr){ 
            if(*ptr>max){ max=*ptr; } 
        }
    }else{
        ptr = data_ + channel;
        end_ptr = ptr + len;
        for(; ptr!=end_ptr; ptr+=num_of_channels_){
            if(*ptr>max){ max=*ptr; }
        }
    }
    return max;
}
template<class T, int MemAllocStep> inline T wsp::VectorTemp<T, MemAllocStep>::GetMin(int channel) const{
    if(length_==0 || data_==NULL){ return (T)0; }
    T min = data_[0];
    T *ptr, *end_ptr;
    int len = length_*num_of_channels_;
    if(channel==-1){
        ptr = data_;
        end_ptr = ptr + len;
        for(; ptr!=end_ptr; ++ptr){
            if(*ptr<min){ min=*ptr; }
        }
    }else{
        ptr = data_ + channel;
        end_ptr = ptr + len;
        for(; ptr!=end_ptr; ptr+=num_of_channels_){
            if(*ptr<min){ min=*ptr; }
        }
    }
    return min;
}
template<class T, int MemAllocStep> inline T wsp::VectorTemp<T, MemAllocStep>::GetSum(int channel) const{
    if(length_==0 || data_==NULL){ return (T)0; }
    T sum=data_[0];
    T *ptr, *end_ptr;
    int len = length_*num_of_channels_;
    if(channel==-1){
        ptr = data_;
        end_ptr = ptr + len;
        for(; ptr!=end_ptr; ++ptr){
            sum+=*ptr;
        }
    }else{
        ptr = data_ + channel;
        end_ptr = ptr + len;
        for(; ptr!=end_ptr; ptr+=num_of_channels_){
            sum+=*ptr;
        }
    }
    return sum;
}
template<class T, int MemAllocStep> inline T wsp::VectorTemp<T, MemAllocStep>::GetAverage(int channel) const{
    if(num_of_elem_==0){ 
        WSP_COMMON_WARNING_LOG("Number of elements is zero\n");
        return static_cast<T>(0); 
    }

    double div = static_cast<double>(num_of_elem_);
    if(channel!=-1){ div /= num_of_channels_; }
    double sum = static_cast<double>(GetSum(channel));
    return static_cast<T>(sum/div);
}
template<class T, int MemAllocStep> inline bool wsp::VectorTemp<T, MemAllocStep>::HasElem(T elem) const{
    int len = length_*num_of_channels_;
    for(int i=0; i<len; i++){
        if(data_[i]==elem){ return true; }
    }
    return false;
}
template<class T, int MemAllocStep> inline int wsp::VectorTemp<T, MemAllocStep>::GetIndex(T elem) const{
    int len = static_cast<int>(length_*num_of_channels_);
    for(int i=0; i<len; i++){
        if(data_[i]==elem){ return i; }
    }
    return -1;
}


// end Accessors: Getter ----------------------------------------------------------------------

// Accessors: Setter --------------------------------------------------------------------------

template<class T, int MemAllocStep> 
void wsp::VectorTemp<T, MemAllocStep>::SetDataFrom(T* data, int length, int num_of_channels){
    Clear();
    //wsp::FreeArrayMem(&data_);
    //free(data_);
    //delete[] data_;
    data_ = data;
    length_ = length;
    num_of_channels_ = num_of_channels;
    num_of_elem_ = length_*num_of_channels_;
    has_own_data_ = false;
}
template<class T, int MemAllocStep> 
void wsp::VectorTemp<T, MemAllocStep>::SetDataInto(T** o_data, int *o_length, int *o_num_of_channels){
    *o_data = data_;
    if(o_length!=NULL){ *o_length = length_; }
    if(o_num_of_channels!=NULL){ *o_num_of_channels = num_of_channels_; }
    has_own_data_ = false;
    data_=NULL;
    InitMemberVariables();
}

template<class T, int MemAllocStep> template<typename U>
wsp::State wsp::VectorTemp<T, MemAllocStep>::CopyFrom(const U* ary, int length, int num_of_channels, int skipStep){
    //wsp::FreeArrayMem(&data_);
    //free(data_);
    //delete[] data_;
    if(ary==NULL || length==0){
        Clear();
        //wsp::FreeArrayMem(&data_);
        //length_ = 0; num_of_channels_ = 0; num_of_elem_ = 0; 
        return WSP_STATE_FAILURE;
    }
    int len, step, itr_i, c, index;
    step = 1+skipStep;
    //if(length%step == 0){
    //    len = length/step;
    //}else{
    //    len = length/step + 1;
    //}
    len = (length%step)==0? length/step : length/step+1;
    //num_of_channels_=num_of_channels;
    //num_of_elem_ = length_*num_of_channels_;
    //data_ = new T[length_*num_of_channels_+RESERVE_MEM];
    //data_ = (T*)calloc(length_*num_of_channels_, sizeof(T));
    //wsp::AllocArrayMem(&data_, num_of_elem_);
    wsp::State state = SetLength(len, num_of_channels);
    if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Setting length failed\n"); return state; }

    if(skipStep==0){
        memcpy(data_, ary, num_of_elem_*sizeof(T));
    }else{
        for(itr_i=0, index=0; itr_i<length_; itr_i++, index+=step){
            for(c=0; c<num_of_channels_; c++){
                data_[itr_i*num_of_channels_+c] = static_cast<T>(ary[index*num_of_channels_+c]);
            }
        }
    }
    return WSP_STATE_FAILURE;
}
template<class T, int MemAllocStep> void wsp::VectorTemp<T, MemAllocStep>::SetElem(const T& element, int index, int channel){
    if(data_==NULL || length_==0 || num_of_channels_==0){
        return;
    }
    if(index<length_){
        if(channel<num_of_channels_){
            data_[index*num_of_channels_+channel]=element;
        }
    }
}
template<class T, int MemAllocStep> 
wsp::State wsp::VectorTemp<T, MemAllocStep>::SetLength(int length, int num_of_channels)
{
    if(length_==length && num_of_channels_==num_of_channels)
    { 
        return WSP_STATE_SUCCESS; 
    }
    if(has_own_data_==false)
    { 
        WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); 
        return WSP_STATE_FAILURE; 
    }
    if(length==0 || num_of_channels==0)
    {
        Clear();
        return WSP_STATE_SUCCESS;
    }
    int nc_old, length_old, num_of_elem_old;
    //wsp::VectorTemp<T, MemAllocStep> tmp(*this);

    length_old = length_;
    nc_old = num_of_channels_;
    num_of_elem_old = length_old*nc_old;
    length_ = length;
    num_of_channels_ = num_of_channels;
    num_of_elem_ = length_*num_of_channels;

    //int mem_size_old = static_cast<int>(ceil(static_cast<float>(num_of_elem_old)/static_cast<float>(MemAllocStep))*MemAllocStep);
    int mem_size_new = static_cast<int>(ceil(static_cast<float>(num_of_elem_)/static_cast<float>(MemAllocStep)))*MemAllocStep;
    if(mem_size_new==mem_size_){ return WSP_STATE_SUCCESS; }

    //! c realloc version allocation -----------------------------
    //wsp::ReallocArrayMem(&data_, num_of_elem_, num_of_elem_old);
    wsp::ReallocArrayMem(&data_, mem_size_new, mem_size_);
    //if(data_==NULL){ data_ = (T*)malloc(length_*sizeof(T)); }
    //else{ realloc(data_, length_*sizeof(T)); }
    if(data_==NULL){ WSP_COMMON_ERROR_LOG("memory allocation failed\n"); return WSP_STATE_FAILURE; }
    mem_size_ = mem_size_new;

    return WSP_STATE_SUCCESS;
}

template<class T, int MemAllocStep> 
void wsp::VectorTemp<T, MemAllocStep>::Append(const T& element, int length){
    if(has_own_data_==false)
    { 
        WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); 
        return; 
    }
    if(length==0){ return; }
    SetLength(length_+length);
    int c, i;
    for(i=(int)length_-(int)length; i<(int)length_; i++){
        for(c=0; c<num_of_channels_; c++){
            data_[i*num_of_channels_+c] = element;
        }
    }
}

template<class T, int MemAllocStep> 
T wsp::VectorTemp<T, MemAllocStep>::Pop()
{
    T popped_elem = data_[length_-1];
    this->SetLength( length_-1 );
    return popped_elem;
}

template<class T, int MemAllocStep> 
wsp::State wsp::VectorTemp<T, MemAllocStep>::Reset(int startIndex, int endIndex, const T& value)
{
    if(startIndex>=length_ || endIndex>=length_){
        WSP_COMMON_ERROR_LOG("startIndex and endIndex are exceeding length of array:startIndex=%d, endIndex=%d, length=%d\n"
            , startIndex, endIndex, length_); 
        return WSP_STATE_FAILURE;
    }
    if(startIndex>endIndex){ WSP_COMMON_ERROR_LOG("start index must be smaller than end index\n"); return WSP_STATE_FAILURE; }
    //if(sizeof(T)==1){
    //    memset(data_+startIndex, value, (endIndex-startIndex));
    //}else{
        int itr_i, itr_c, i;
        for(itr_i=startIndex; itr_i<=endIndex; itr_i++){
            i = itr_i*num_of_channels_;
            for(itr_c=0; itr_c<num_of_channels_; itr_c++){
                data_[i+itr_c] = value;
            }
        }
    //}
    return WSP_STATE_SUCCESS;
}

template<class T, int MemAllocStep> 
wsp::State wsp::VectorTemp<T, MemAllocStep>::Reset(const T& value){
    return Reset(0, length_-1, value);
}

template<class T, int MemAllocStep> 
void wsp::VectorTemp<T, MemAllocStep>::ResetElems(const T *src_elem, size_t elem_size)
{
    {
        T *ptr = data_;
        T *end_ptr = ptr + num_of_elem_;
        for(; ptr!=end_ptr; ptr+=num_of_channels_){
            memcpy(ptr, src_elem, elem_size);
        }
    }
}

template<class T, int MemAllocStep> inline wsp::State wsp::VectorTemp<T, MemAllocStep>::Clear(){
    if(has_own_data_==false)
    { WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); return WSP_STATE_FAILURE; }
    wsp::FreeArrayMem(&data_);
    InitMemberVariables();
    return WSP_STATE_SUCCESS;
}
template<class T, int MemAllocStep> void wsp::VectorTemp<T, MemAllocStep>::SortAscending(){
    qsort(data_, num_of_elem_, sizeof(T), wsp::CompA<T>);
}
template<class T, int MemAllocStep> void wsp::VectorTemp<T, MemAllocStep>::SortDescending(){
    qsort(data_, num_of_elem_, sizeof(T), wsp::CompB<T>);
}
template<class T, int MemAllocStep> void wsp::VectorTemp<T, MemAllocStep>::Reverse()
{
    int L=0, R=length_-1, c;
    while(L<R){
        for(c=0; c<num_of_channels_; c++){
            T tmp=data_[L*num_of_channels_+c];
            data_[L*num_of_channels_+c]=data_[R*num_of_channels_+c];
            data_[R*num_of_channels_+c]=tmp;
            L++;  R--;
        }
    }
}
template<class T, int MemAllocStep> void wsp::VectorTemp<T, MemAllocStep>::ShiftElements(double shiftSubpix)
{
    if(shiftSubpix==0){ return; }
    int index, c;
    double w1, w2;
    int shiftPix = (int)floor(shiftSubpix);
    w1 = shiftSubpix - (double)shiftPix;
    w2 = 1.0 - w1;
    for(int i=0; i<length_; i++)
    {
        index = i+shiftPix;
        if(0<=index && index<length_-1)
        {
            for(c=0; c<num_of_channels_; ++c)
            {
                data_[i*num_of_channels_+c] = 
                    static_cast<T>(
                        static_cast<double>(data_[index*num_of_channels_+c])*w2
                            + static_cast<double>(data_[(index+1)*num_of_channels_+c])*w1);
            }
        }
        else if(index==length_-1)
        {
            for(c=0; c<num_of_channels_; c++)
            {
                data_[i*num_of_channels_+c] = data_[index*num_of_channels_+c];
            }
        }
        else
        {
            for(c=0; c<num_of_channels_; c++)
            {
                data_[i*num_of_channels_+c] = (T)0;
            }
        }
    }
}
template<class T, int MemAllocStep> void wsp::VectorTemp<T, MemAllocStep>::SetRandNoise(double min, double max){
    double range = max - min;
    int len = length_*num_of_channels_;
    for(int i=0; i<len; i++)
    {
        T randVal = (T)(floor(range * (double)rand()/32767.0) + min);
        data_[i] = randVal;
    }
}
template<class T, int MemAllocStep> void wsp::VectorTemp<T, MemAllocStep>::Abs(){
    int len = length_*num_of_channels_;
    for(int i=0; i<len; i++)
    {
        if(data_[i] < static_cast<T>(0)){ data_[i] = data_[i]*(-1); }
    }
}
template<class T, int MemAllocStep> void wsp::VectorTemp<T, MemAllocStep>::Normalize(T min_norm, T max_norm){
    T min=GetMin(), max=GetMax();
    T range=max-min, range_norm=max_norm-min_norm;
    int len = static_cast<int>(num_of_elem_);
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for(int i=0; i<len; i++)
    {
        data_[i] = (data_[i]-min)*range_norm/range + min_norm;
    }
}
template<class T, int MemAllocStep> void wsp::VectorTemp<T, MemAllocStep>::CombineChannelsByFunc(T(*pre_func)(T), T(*post_func)(T)){
    if(num_of_channels_<=1){ return; }
    int i, c;
    double sum;
    wsp::VectorTemp<T, MemAllocStep> tmp_vec(length_, 1);
    for(i=0; i<length_; ++i){
        sum = 0;
        for(c=0; c<num_of_channels_; c++){
            sum += static_cast<double>(pre_func(data_[i*num_of_channels_+c]));
        }
        tmp_vec[i] = static_cast<T>(post_func(static_cast<T>(sum)));
    }
    *this=tmp_vec;
}
template<class T, int MemAllocStep> void wsp::VectorTemp<T, MemAllocStep>::CombineChannelsByNorm()
{
    return wsp::VectorTemp<T, MemAllocStep>::CombineChannelsByFunc(wsp::SquareOperation, wsp::SquareRootOperation);
}
template<class T, int MemAllocStep> void wsp::VectorTemp<T, MemAllocStep>::CombineChannelsBySum()
{
    return wsp::VectorTemp<T, MemAllocStep>::CombineChannelsByFunc(wsp::NoOperation);
}
template<typename T, int MemAllocStep> 
wsp::State wsp::VectorTemp<T, MemAllocStep>::SetNumOfChannels(int num_of_channels)
{
    if( num_of_channels == num_of_channels_ )
    {
        return WSP_STATE_SUCCESS;
    }
    wsp::VectorTemp<T, MemAllocStep> tmp_vec(*this);

    wsp::State state = SetLength(length_, num_of_channels);
    if(state!=WSP_STATE_SUCCESS){ return WSP_STATE_FAILURE; }

    {
        int src_nc = tmp_vec.num_of_channels();
        T *src_ptr = tmp_vec.data();
        T *end_ptr = src_ptr + tmp_vec.num_of_elem();
        T *dest_ptr = data_;
        T *end_nc;
        if( src_nc < num_of_channels_ )
        {
            int diff_nc = num_of_channels_ - src_nc;
            for( ; src_ptr!=end_ptr; )
            {
                end_nc = src_ptr + src_nc;
                for( ; src_ptr!=end_nc; ++src_ptr, ++dest_ptr )
                {
                    *dest_ptr = *src_ptr;
                }
                dest_ptr+=diff_nc;
            }
        }
        else
        {
            int diff_nc = src_nc - num_of_channels_;
            for( ; src_ptr!=end_ptr; )
            {
                end_nc = dest_ptr + num_of_channels_;
                for( ; dest_ptr!=end_nc; ++src_ptr, ++dest_ptr )
                {
                    *dest_ptr = *src_ptr;
                }
                src_ptr+=diff_nc;
            }
        }
    }    
    return WSP_STATE_SUCCESS; 
}
template<typename T, int MemAllocStep> 
wsp::State wsp::VectorTemp<T, MemAllocStep>::ExtendsNumOfChannels( int extended_num_of_channels )
{
    if( extended_num_of_channels == num_of_channels_ )
    {
        return WSP_STATE_SUCCESS;
    }
    if( extended_num_of_channels < num_of_channels_ )
    {
        WSP_COMMON_ERROR_LOG("given number of channels is smaller than current\n");
        return WSP_STATE_FAILURE;
    }

    SetNumOfChannels( extended_num_of_channels );

    /*
     * Just copy first channel to other channels
     */
    {
        T *ptr = data_;
        T *end_ptr = ptr + num_of_elem_;
        T *end_nc;
        T copy_value;
        for( ; ptr!=end_ptr; )
        {
            end_nc = ptr + num_of_channels_;
            copy_value = *ptr;
            ++ptr;
            for( ; ptr!=end_nc; ++ptr )
            {
                *ptr = copy_value;
            }
        }
    }
    return WSP_STATE_SUCCESS;
}

template<typename T, int MemAllocStep> 
wsp::State wsp::VectorTemp<T, MemAllocStep>::LeaveOneChannel(int channel_index) 
{
    WSP_REQUIRES(
        channel_index < this->num_of_channels_, 
        "Too larget channel index %d < %d", channel_index, this->num_of_channels_);

    wsp::VectorTemp<T, MemAllocStep> tmp_vec(*this);
    wsp::State state = SetNumOfChannels(1);
    if(state!=WSP_STATE_SUCCESS){ return state; }

    int nc = tmp_vec.num_of_channels();
    for(int i=0; i<length_; ++i){
        data_[i] = tmp_vec[i*nc + channel_index];
    }

    return WSP_STATE_SUCCESS;
}

template<typename T, int MemAllocStep> 
wsp::State wsp::VectorTemp<T, MemAllocStep>::RemoveExceptOneChannel(int channel_index )
{
    for(int i=0; i<length_; ++i)
    {
        for( int j=0; j<num_of_channels_; ++j )
        {
            if( j==channel_index ){
                continue;
            }
            data_[i * num_of_channels_ + j] = 0;
        }
    }
    return WSP_STATE_SUCCESS;
}

namespace {
    //template<typename T> 
    //inline s32 TakeAnyMatchedElems( T* data, s32 length, T target_elem, u32 num_take )
    //{
    //    T* ptr     = data;
    //    T* end_ptr = ptr + length;
    //    T store_elem = *ptr;
    //    s32 num_found_elems = 0;
    //    for( ; ptr!=end_ptr; ++ptr )
    //    {
    //        if( ( *ptr == target_elem )
    //         && ( num_found_elems < num_take ) )
    //        {
    //            ++num_found_elems;
    //            continue;
    //        }
    //        *(ptr-num_found_elems) = *(ptr);
    //    }
    //    return num_found_elems;
    //}
}

template<typename T, int MemAllocStep> 
s32 wsp::VectorTemp<T, MemAllocStep>::TakeAllMatchedElems( T target_elem )
{
    T* ptr     = data_;
    T* end_ptr = ptr + length_;
    T store_elem = *ptr;
    s32 num_found_elems = 0;
    for( ; ptr!=end_ptr; ++ptr )
    {
        if( ( *ptr == target_elem ) )
        {
            ++num_found_elems;
            continue;
        }
        *(ptr-num_found_elems) = *(ptr);
    }
    if( num_found_elems == 0 ){ return 0; }

    this->SetLength( length_ - num_found_elems );

    return num_found_elems;
}
template<typename T, int MemAllocStep> 
s32 wsp::VectorTemp<T, MemAllocStep>::TakeFirstMatchedElem( T target_elem )
{
    T* ptr     = data_;
    T* end_ptr = ptr + length_;
    T store_elem = *ptr;
    s32 num_found_elems = 0;
    s32 index = 0;
    s32 found_index = -1;
    for( ; ptr!=end_ptr; ++ptr, ++index )
    {
        if( ( *ptr == target_elem )
            && ( num_found_elems < 1 ) )
        {
            ++num_found_elems;
            found_index = index;
            continue;
        }
        *(ptr-num_found_elems) = *(ptr);
    }
    if( num_found_elems == 0 ){ return -1; }

    this->SetLength( length_ - num_found_elems );
    return found_index;
}
template<typename T, int MemAllocStep> 
wsp::Result wsp::VectorTemp<T, MemAllocStep>::InsertElem( s32 index, T elem )
{
    if( index<0 || index>length_ )
    {
        return wsp::ResultInvalidArgument();
    }

    s32 old_length = length_;
    this->SetLength( length_ + 1 );

    T* ptr = data_ + length_;
    T* end_ptr =  data_ + index;
    for( ; ptr>end_ptr; --ptr )
    {
        *ptr = *(ptr-1);
    }
    *ptr = elem;
    return wsp::ResultSuccess();
}

// end Accessors ------------------------------------------------------------------------------

// I/O ---------------------------------------------------------------------------------------
template<class T, int MemAllocStep> wsp::State wsp::VectorTemp<T, MemAllocStep>::SaveRawData(const char *in_fileNameFormat,...) const{
    char filename[MAX_LENGTH_OF_NAME];
    va_list ap;
    va_start(ap, in_fileNameFormat);
    vsprintf(filename, in_fileNameFormat, ap);
    va_end(ap);
    return wsp::SaveRawArray<T>( data_, length_, filename );
}
template<class T, int MemAllocStep> wsp::State wsp::VectorTemp<T, MemAllocStep>::LoadRawData(const char *fileName){
    if(has_own_data_==false)
    { 
        WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); 
        return WSP_STATE_FAILURE; 
    }

    wsp::State stat;
    size_t len;
    stat = wsp::LoadRawArray<T>( &data_, len, fileName );
    length_ = static_cast<s32>(len);
    num_of_channels_ = 1;
    return stat;
}
//
//#ifdef USE_QT
//template<class T, int MemAllocStep> inline wsp::State wsp::VectorTemp<T, MemAllocStep>::SaveRawDataDialog(const char *title) const{
//    QString selectedFilter;
//    QString qfilename = QFileDialog::getSaveFileName(NULL, QString(title),
//                            QDir::currentPath(), wsp::formatFilterWspLibData, &selectedFilter);
//    char currentDir[MAX_LENGTH_OF_PATH];
//    wsp::GetDir(currentDir, qfilename.toLatin1().data());
//    if(QDir::setCurrent(QString(currentDir))==false){ WSP_COMMON_ERROR_LOG("setCurrent failed\n"); }
//
//    return SaveRawData(qfilename.toLatin1().data());
//}
//
//template<class T, int MemAllocStep> inline wsp::State wsp::VectorTemp<T, MemAllocStep>::LoadRawDataDialog(const char *title){
//    QString qfilename = QFileDialog::getOpenFileName(NULL, QString(title), QDir::currentPath(), wsp::formatFilterWspLibData);
//    char currentDir[MAX_LENGTH_OF_PATH];
//    wsp::GetDir(currentDir, qfilename.toLatin1().data());
//    if(QDir::setCurrent(QString(currentDir))==false){ WSP_COMMON_ERROR_LOG("setCurrent failed\n"); }
//    return LoadRawData(qfilename.toLatin1().data());
//}
//#endif
//! --------------------------------------------------------------------------------------- end I/O

// Operators ---------------------------------------------------------------------------------
template<class T, int MemAllocStep> inline wsp::VectorTemp<T, MemAllocStep>& wsp::VectorTemp<T, MemAllocStep>::operator=(const wsp::VectorTemp<T, MemAllocStep>& other){
    if(has_own_data_==false)
    { 
        WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); 
        return *this; 
    }
    if(this != &other){
        CopyFrom(other.data(), other.length(), other.num_of_channels());
    }
    return *this;
}

template<class T, int MemAllocStep> inline bool wsp::VectorTemp<T, MemAllocStep>::operator==(const wsp::VectorTemp<T, MemAllocStep> &other) const{
    if(other.data()==NULL && data_==NULL && length_==other.length() && num_of_channels_==other.num_of_channels()){
        return true;
    }
    if(other.length()!=length_ || other.num_of_channels()!=num_of_channels_ || data_==NULL || other.data()==NULL){
        return false;
    }
    return memcmp(data_, other.data(), num_of_elem_*sizeof(T))==0? true : false;
    //int len = length_*num_of_channels_;
    //int itr_i, c;
    //for(itr_i=0; itr_i<len; itr_i++){
    //    for(c=0; c<num_of_channels_; c++){
    //        if(data_[itr_i*num_of_channels_+c]!=other[itr_i*num_of_channels_+c]){
    //            return false;
    //        }
    //    }
    //}
    //return true;
}
template<class T, int MemAllocStep> 
inline bool wsp::VectorTemp<T, MemAllocStep>::operator!=(const wsp::VectorTemp<T, MemAllocStep> &other) const
{
    return !(*this==other);
}

template<class T, int MemAllocStep> 
inline T& wsp::VectorTemp<T, MemAllocStep>::operator[](int index)
{ 
    return data_[index]; 
}
//template<class T, int MemAllocStep> inline T& wsp::VectorTemp<T, MemAllocStep>::operator[](int index){
//    if(index<0){ return data_[0]; }
//    return operator[](static_cast<int>(index));
//}
template<class T, int MemAllocStep> 
inline const T wsp::VectorTemp<T, MemAllocStep>::operator[](int index) const
{
    if(index>=num_of_elem_)
    { 
        WSP_COMMON_WARNING_LOG("index exceeds size of vector, %d>=%d\n", index, num_of_elem_); 
        return (T)0;
    }
    return data_[index];
}
//template<class T, int MemAllocStep> inline const T wsp::VectorTemp<T, MemAllocStep>::operator[](int index) const{
//    if(index<0){ return data_[0]; }
//    return operator[](static_cast<int>(index));
//}
template<class T, int MemAllocStep> 
inline wsp::VectorTemp<T, MemAllocStep>& wsp::VectorTemp<T, MemAllocStep>::operator+=(const T& right){
    operator=(operation(right, wsp::add));
    return *this;
}
template<class T, int MemAllocStep> 
inline wsp::VectorTemp<T, MemAllocStep>& wsp::VectorTemp<T, MemAllocStep>::operator-=(const T& right){
    operator=(operation(right, wsp::sub));
    return *this;
}
template<class T, int MemAllocStep> 
inline wsp::VectorTemp<T, MemAllocStep>& wsp::VectorTemp<T, MemAllocStep>::operator*=(const T& right){
    operator=(operation(right, wsp::mult));
    return *this;
}
template<class T, int MemAllocStep> 
inline wsp::VectorTemp<T, MemAllocStep>& wsp::VectorTemp<T, MemAllocStep>::operator/=(const T& right){
    operator=(division(right));
    return *this;
}
template<class T, int MemAllocStep> 
inline wsp::VectorTemp<T, MemAllocStep>& wsp::VectorTemp<T, MemAllocStep>::operator+=(const wsp::VectorTemp<T, MemAllocStep>& vec){
    operator=(operation(vec, wsp::add));
    return *this;
}
template<class T, int MemAllocStep> 
inline wsp::VectorTemp<T, MemAllocStep>& wsp::VectorTemp<T, MemAllocStep>::operator-=(const wsp::VectorTemp<T, MemAllocStep>& vec){
    operator=(operation(vec, wsp::sub));
    return *this;
}
template<class T, int MemAllocStep> 
inline wsp::VectorTemp<T, MemAllocStep>& wsp::VectorTemp<T, MemAllocStep>::operator*=(const wsp::VectorTemp<T, MemAllocStep>& vec){
    operator=(operation(vec, wsp::mult));
    return *this;
}
template<class T, int MemAllocStep> 
inline wsp::VectorTemp<T, MemAllocStep> wsp::VectorTemp<T, MemAllocStep>::operator+(const T& right) const
{
    return operation(right, wsp::add);
}
template<class T, int MemAllocStep> 
inline wsp::VectorTemp<T, MemAllocStep> wsp::VectorTemp<T, MemAllocStep>::operator-(const T& right) const{
    return operation(right, wsp::sub);
}
template<class T, int MemAllocStep> 
inline wsp::VectorTemp<T, MemAllocStep> wsp::VectorTemp<T, MemAllocStep>::operator*(const T& right) const{
    return operation(right, wsp::mult);
}
template<class T, int MemAllocStep> 
inline wsp::VectorTemp<T, MemAllocStep> wsp::VectorTemp<T, MemAllocStep>::operator/(const T& right) const{
    return division(right);
}
template<class T, int MemAllocStep> 
inline wsp::VectorTemp<T, MemAllocStep> wsp::VectorTemp<T, MemAllocStep>::operator+(const wsp::VectorTemp<T, MemAllocStep>& vec) const{
    return operation(vec, wsp::add);
}
template<class T, int MemAllocStep> 
inline wsp::VectorTemp<T, MemAllocStep> wsp::VectorTemp<T, MemAllocStep>::operator-(const wsp::VectorTemp<T, MemAllocStep>& vec) const{
    return operation(vec, wsp::sub);
}
template<class T, int MemAllocStep> 
inline wsp::VectorTemp<T, MemAllocStep> wsp::VectorTemp<T, MemAllocStep>::operator*(const wsp::VectorTemp<T, MemAllocStep>& vec) const{
    return operation(vec, wsp::mult);
}
template<class T, int MemAllocStep> 
inline wsp::VectorTemp<T, MemAllocStep> wsp::VectorTemp<T, MemAllocStep>::operator/(const wsp::VectorTemp<T, MemAllocStep>& vec) const{
    return division(vec);
}
template<class T, int MemAllocStep> 
inline wsp::VectorTemp<T, MemAllocStep>& wsp::VectorTemp<T, MemAllocStep>::operator/=(const wsp::VectorTemp<T, MemAllocStep>& vec){
    division(vec);
    return *this;
}

template<class T, int MemAllocStep> wsp::VectorTemp<T, MemAllocStep> wsp::VectorTemp<T, MemAllocStep>::operation(const T& value, T(*op)(const T& left, const T& right)) const{
    wsp::VectorTemp<T, MemAllocStep> vec(length_);
    int len = length_*num_of_channels_;
    T *data_ptr = vec.data();
    T *end_ptr = vec.data()+vec.length()*vec.num_of_channels();
    for(; data_ptr!=end_ptr; ++data_ptr){
        *data_ptr = op(*data_ptr, value);
    }
    return vec;
}
template<class T, int MemAllocStep> wsp::VectorTemp<T, MemAllocStep> wsp::VectorTemp<T, MemAllocStep>::operation(const wsp::VectorTemp<T, MemAllocStep>& vec, T(*op)(const T& left, const T& right)) const{
    wsp::VectorTemp<T, MemAllocStep> vec_dst(*this);
    int len_s = vec.length();
    int nc_vec = vec.num_of_channels();
    if(vec.data()==NULL || len_s==0){
        WSP_COMMON_WARNING_LOG("right hand side vector is zero length array.\n");
        return vec_dst;
    }

    if(len_s > length_){
        len_s = length_;
    }
    //for(int itr_i=0; itr_i<len_s; itr_i++){
    //    for(c=0; c<num_of_channels_; c++){
    //        if(c<nc_vec){
    //            data[itr_i*num_of_channels_+c] = op(data_[itr_i*num_of_channels_+c], vec[itr_i*nc_vec+c]);
    //            continue;
    //        }
    //        data[itr_i*num_of_channels_+c] = op(data_[itr_i*num_of_channels_+c], vec[itr_i*nc_vec+nc_vec-1]);
    //    }
    //}
    int dst_nc = vec_dst.num_of_channels();
    int src_nc = vec.num_of_channels();
    int nc_diff = vec_dst.num_of_channels()-vec.num_of_channels();
    T *dst_ptr = vec_dst.data();
    T *end_ptr = vec_dst.data()+vec_dst.length()*dst_nc;
    T *src1_ptr = data_;
    const T *src2_ptr = vec.data();
    int cnt=0;
    if(nc_diff==0){
        // same number of channels
        for(; dst_ptr!=end_ptr; ++dst_ptr, ++src1_ptr, ++src2_ptr){
            *dst_ptr = op(*src1_ptr, *src2_ptr);
        }
    }else if(nc_diff>0){
        for(; dst_ptr!=end_ptr; ++dst_ptr, ++src1_ptr, ++src2_ptr){
            *dst_ptr = op(*src1_ptr, *src2_ptr);
            cnt++;
            if(cnt==dst_nc){
                src2_ptr+=nc_diff;
                cnt=0;
            }
        }
    }else{
        nc_diff = vec.num_of_channels()-vec_dst.num_of_channels();
        for(; dst_ptr!=end_ptr; ++dst_ptr, ++src1_ptr, ++src2_ptr){
            *dst_ptr = op(*src1_ptr, *src2_ptr);
            cnt++;
            if(cnt==src_nc){
                dst_ptr+=nc_diff;
                src1_ptr+=nc_diff;
                cnt=0;
            }
        }
    }
    return vec_dst;
}
template<class T, int MemAllocStep> wsp::VectorTemp<T, MemAllocStep> wsp::VectorTemp<T, MemAllocStep>::division(const T& value) const{
    wsp::VectorTemp<T, MemAllocStep> data(*this);
    if(static_cast<double>(value)!=0){
        int len=length_*num_of_channels_;
        for(int itr_i=0; itr_i<len; itr_i++){
            data[itr_i] = static_cast<T>(static_cast<double>(data_[itr_i]) / static_cast<double>(value));
        }
    }
    return data;
}
template<class T, int MemAllocStep> 
wsp::VectorTemp<T, MemAllocStep> wsp::VectorTemp<T, MemAllocStep>::division(const wsp::VectorTemp<T, MemAllocStep>& vec) const{
    wsp::VectorTemp<T, MemAllocStep> data(*this);
    int len_s = vec.length();
    if(vec.data()==NULL || len_s==0){
        return data;
    }

    if(len_s > length_){
        len_s = length_;
    }
    int itr_i, c;
    int nc_vec=vec.num_of_channels();
    for(itr_i=0; itr_i<len_s; itr_i++){
        for(c=0; c<num_of_channels_; c++){
            if(c<nc_vec){
                if(static_cast<double>(vec[itr_i*nc_vec+c])!=0){
                    data[itr_i*num_of_channels_+c] = static_cast<T>(static_cast<double>(data_[itr_i*num_of_channels_+c]) 
                                                                    / static_cast<double>(vec[itr_i*nc_vec+c]));
                    continue;
                }
                data[itr_i*num_of_channels_+c] = (T)0;
            }
            if(static_cast<double>(vec[itr_i*nc_vec+c])!=0){
                data[itr_i*num_of_channels_+c] = static_cast<T>(static_cast<double>(data_[itr_i*num_of_channels_+c])
                                                                / static_cast<double>(vec[itr_i*nc_vec+nc_vec-1]));
            }
            data[itr_i*num_of_channels_+c] = (T)0;
        }
    }
    return data;
}


template<class T, int MemAllocStep> 
std::ostream& operator<<(std::ostream& lhs, const wsp::VectorTemp<T, MemAllocStep>& rhs)
{
    const T *ptr = rhs.data();
    const T *end_ptr = ptr + rhs.length();
    for(int itr_i=0; ptr!=end_ptr; ++itr_i, ++ptr)
    {
        lhs<<std::setw(PRINT_COLW)<<std::setprecision(PRINT_PREC);
        //lhs<<scientific;
        lhs<<PRINT_TYPE;
        lhs<<"["<<itr_i<<"]="<<*ptr;
        //lhs<<" ";
        //lhs<<'\t';
    }
    lhs<<std::endl;
    return lhs;
}
template<class T, int MemAllocStep> 
std::ostream& operator<<(std::ostream& lhs, const wsp::VectorTemp<T, MemAllocStep>* rhs)
{
    lhs<<*rhs;
    return lhs;
}

#endif