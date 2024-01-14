// @file _tpl_cl-stackarray.h
// 


#ifndef _WSP_CORE_BASE_TEMPLATE_CLASS_STACK_ARRAY_H_
#define _WSP_CORE_BASE_TEMPLATE_CLASS_STACK_ARRAY_H_

#ifndef __WSP_COMMONUTIL_DEFINE_COMMONUTIL_H__
#include "_define_commonutil.h"
#endif


namespace wsp{

    #define INIT_StackArray length_(0)

    template<class T, int PreallocLength=256>
    class StackArray
    {
    protected:
        int length_;
        T data_[PreallocLength];

    public:
        // Constructor, destructor ---------------
        inline StackArray():INIT_StackArray{}
        inline explicit StackArray(int length):INIT_StackArray{ set_length(length); }

        // Accessors -----------------------------
        inline int length()const{ return length_; }
        inline T* data(){ return &data_[0]; }
        inline const T* data()const{ return &data_[0]; }

        // Mutators ------------------------------
        wsp::State set_length(int length);
        inline wsp::State Append(T elem){
            if(length_+1>PreallocLength){ WSP_COMMON_ERROR_LOG("stack memory is full\n"); return WSP_STATE_FAILURE; }
            data_[length_] = elem;
            length_++;
            return WSP_STATE_SUCCESS;
        }
        //inline void Clear(){
        //    memset(this, 0, sizeof(*this));
        //}

        // Operators -----------------------------
        inline T operator[](int index) const{ 
            if(index>=length_ || index<0){ WSP_COMMON_ERROR_LOG("wrong index %d\n", index); return static_cast<T>(0); }
            return data_[index]; }
        inline T& operator[](int index){ 
            if(index<0 || index>PreallocLength){ WSP_COMMON_ERROR_LOG("wrong index %d\n", index); return data_[0]; }
            if(index>=length_){ WSP_COMMON_WARNING_LOG("index exceeds current length %d %d\n", length_, index); length_=index+1; }
            return data_[index]; 
        }
        inline bool operator==(const wsp::StackArray<T, PreallocLength> &rhs) const{
            if(rhs.length()!=length_){ return false; }
            for(int itr_i=0; itr_i<length_; itr_i++){
                if(data_[itr_i] != rhs[itr_i]){ return false; }
            }
            return true;
        }
        inline bool operator!=(const wsp::StackArray<T, PreallocLength> &other) const{ return !(*this==other); }
    };


}


template<class T, int PreallocLength>
wsp::State wsp::StackArray<T, PreallocLength>::set_length(int length){
    if(length<0){ WSP_COMMON_ERROR_LOG("the length must be positive\n"); return WSP_STATE_FAILURE; }
    if(length>PreallocLength){ WSP_COMMON_ERROR_LOG("the length exceeds max buffer\n"); return WSP_STATE_FAILURE; }
    length_ = length;
    return WSP_STATE_SUCCESS;
}




#endif