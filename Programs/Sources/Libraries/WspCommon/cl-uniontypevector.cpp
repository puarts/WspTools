/**
 * @file cl-uniontypevector.cpp
 * 
 * Base class for vector type class
 */

#include "cl-uniontypevector.h"


// mutators
inline wsp::State wsp::UnionTypeVector::SetSize(int length, int num_of_channels, wsp::DataType type){
    if(length_==length && num_of_channels_==num_of_channels && ( type==wsp::DATATYPE_DEFAULT))
    { return WSP_STATE_SUCCESS; }
    if(has_own_data_==false)
    { WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); return WSP_STATE_FAILURE; }
    if(length==0 || num_of_channels==0){
        Clear();
        return WSP_STATE_SUCCESS;
    }
    int nc_old, length_old, num_elem_old;
    //wsp::Vector<T, mem_alloc_step_> tmp(*this);

    length_old = length_;
    nc_old = num_of_channels_;
    num_elem_old = length_old*nc_old;
    length_ = length;
    num_of_channels_ = num_of_channels;
    num_elem_ = length_*num_of_channels;

    //int mem_size_old = static_cast<int>(ceil(static_cast<float>(num_elem_old)/static_cast<float>(mem_alloc_step_))*mem_alloc_step_);
    size_t mem_size_new = static_cast<int>(ceil(static_cast<float>(num_elem_)/static_cast<float>(mem_alloc_step_)))*mem_alloc_step_;
    if(mem_size_new==mem_size_){ return WSP_STATE_SUCCESS; }

    //! c realloc version allocation -----------------------------
    //wsp::ReallocArrayMem(&data_, size_, num_elem_old);
    switch(type){
    case wsp::DATATYPE_8U:
        wsp::ReallocArrayMem<uchar>(&data_, mem_size_new, mem_size_);
        break;
    case wsp::DATATYPE_16S:
        {
            short *ptr = reinterpret_cast<short*>(data_);
            wsp::ReallocArrayMem<short>(&ptr, mem_size_new, mem_size_);
        }
        break;
    case wsp::DATATYPE_32S:
        {
            int *ptr = reinterpret_cast<int*>(data_);
            wsp::ReallocArrayMem<int>(&ptr, mem_size_new, mem_size_);
        }
        break;
    case wsp::DATATYPE_32F:
        {
            float *ptr = reinterpret_cast<float*>(data_);
            wsp::ReallocArrayMem<float>(&ptr, mem_size_new, mem_size_);
        }
        break;
    case wsp::DATATYPE_64F:
        {
            double *ptr = reinterpret_cast<double*>(data_);
            wsp::ReallocArrayMem<double>(&ptr, mem_size_new, mem_size_);
        }
        break;
    }
    //if(data_==NULL){ data_ = (T*)malloc(length_*sizeof(T)); }
    //else{ realloc(data_, length_*sizeof(T)); }
    if(data_==NULL){ WSP_COMMON_ERROR_LOG("memory allocation failed\n"); return WSP_STATE_FAILURE; }
    mem_size_ = mem_size_new;

    return WSP_STATE_SUCCESS;
}
