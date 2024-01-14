// @file tpl_cl-uniontypevector.h
// 
// Description: Class for union type vector


#ifndef __WSP_CORE_CLASS_UNION_TYPE_VECTOR_H__
#define __WSP_CORE_CLASS_UNION_TYPE_VECTOR_H__

#include "_tpl_fn-mem_manag.hpp"

namespace wsp{

    class WSP_DLL_EXPORT UnionTypeVector
    {
    public:
        // constructors
        inline UnionTypeVector();
        inline UnionTypeVector(void* data, int length, int num_channels, size_t size);
        inline ~UnionTypeVector();

        // accessors
        inline int length(){ return length_; }
        inline int num_of_channels(){ return num_of_channels_; }
        inline uchar* data(){ return data_; }

        // mutators
        inline wsp::State Clear()
        {
            if(has_own_data_==false)
            { 
                WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); 
                return WSP_STATE_FAILURE; 
            }
            wsp::FreeArrayMem(&data_);
            length_ = 0;
            num_elem_ = 0;
            return WSP_STATE_SUCCESS;
        }
        wsp::State SetSize(int length, int num_of_channels, wsp::DataType type=wsp::DATATYPE_DEFAULT);

        // operators
        inline uchar& operator[](int index){ return data_[index]; }
        inline const uchar operator[](int index) const{    return data_[index]; }
    private:
        int mem_alloc_step_;
        int length_, num_of_channels_, num_elem_, mem_size_;
        size_t unit_size_;
        bool has_own_data_;

        uchar* data_;

    };
}


#define INIT_WSP_UNION_TYPE_VECTOR \
    mem_alloc_step_(50)\
    , length_(0)\
    , num_of_channels_(1)\
    , num_elem_(0)\
    , mem_size_(0)\
    , unit_size_(0)\
    , has_own_data_(false)\
    , data_(NULL)

// constructors
inline wsp::UnionTypeVector::UnionTypeVector()
    : INIT_WSP_UNION_TYPE_VECTOR
{}
inline wsp::UnionTypeVector::UnionTypeVector(void* data, int length, int num_channels, size_t size)
    : INIT_WSP_UNION_TYPE_VECTOR
{
    if(data==NULL){ return; }
    length_ = length;
    num_of_channels_ = num_channels;
    num_elem_ = length_*num_channels;
    data_ = (uchar*)data;
    unit_size_ = size;
    has_own_data_ = false;
}
inline wsp::UnionTypeVector::~UnionTypeVector(){
    if(has_own_data_){
        wsp::FreeArrayMem(&data_);
    }
}

#endif