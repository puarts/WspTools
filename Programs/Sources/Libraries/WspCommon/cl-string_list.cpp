// wsp::StringList.cpp
// 

#include "cl-string_list.h"

using namespace std;

// Constructor, Destructor -----------------------------------------
//#define INIT_StringList data_(NULL), length_(0)
wsp::StringList::StringList()
    //:INIT_StringList
    : wsp::Vector<wsp::String>()
{}
wsp::StringList::StringList(const wsp::StringList &list)
    //:INIT_StringList
    : wsp::Vector<wsp::String>(list)
{
    //*this = list;
}
wsp::StringList::StringList(u32 length)
    //:INIT_StringList
    : wsp::Vector<wsp::String>(length)
{
    //if(length!=0){
    //    length_=length;
    //    wsp::AllocArrayMem(&data_, length_);
    //    //data_ = new wsp::String[length];
    //    for(u32 i=0; i<length; i++){
    //        data_[i][0] = '\0';
    //    }
    //}
}
wsp::StringList::~StringList()
{
    //delete[] data_;
    //wsp::FreeArrayMem(&data_);
}
//
// Setter -------------------------------------------------
//void wsp::StringList::SetLength(u32 length){
//    if(length_ == length){
//        return;
//    }
//    if(length==0){
//        delete[] data_;
//        data_ = NULL;
//        length_ = 0;
//        return;
//    }
//    u32 itr_i, length_old;
//    //wsp::StringList tmp(*this);
//
//    length_old = length_;
//    length_ = length;
//    wsp::ReallocArrayMem(&data_, length_, length_old);
//
//    //delete[] data_;
//    //data_ = new wsp::String[length_];
//
//    //for(itr_i=0; itr_i<length_; itr_i++){
//    //    if(itr_i < length_old){
//    //        data_[itr_i] = tmp[itr_i];
//    //        continue;
//    //    }
//    //}
//
//}
//
