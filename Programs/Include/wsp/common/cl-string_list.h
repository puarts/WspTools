// @file cl-string_list.h
// 


#ifndef __WspStringList_H__
#define __WspStringList_H__

#include "cl-string.h"
#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#include "tpl_cl-vector.h"
#endif
BEGIN_WSP_CORE_NAMESPACE

class WSP_DLL_EXPORT StringList
    :public wsp::Vector<wsp::String>
{
private:
    //u32 length_;
    //wsp::String *data_;

public:
    StringList();
    StringList(const wsp::StringList &list);
    StringList(u32 length);
    ~StringList();

    // Getter
    //inline u32 length() const;
    //inline wsp::String GetElem(u32 index) const;
    //inline wsp::String* data();
    inline wsp::State GetOriginalName(const char *in_target_name, char *o_dst){
        int default_cnt = 0;
        for(int i=0; i<length_; ++i){
            if(strcmp(data_[i].data(), in_target_name)==0){
            }
        }
        return WSP_STATE_SUCCESS;
    }

    // Setter
    //void SetLength(u32 length);
    //inline void SetData(const char *str, u32 index);

    // Operators
    //inline wsp::StringList& operator=(const wsp::StringList& other);
    //inline bool operator==(const wsp::StringList& other) const;
    //inline bool operator!=(const wsp::StringList &other) const;
    //inline wsp::String& operator[](u32 index);
    //inline wsp::String& operator[](int  index);
    //inline const wsp::String operator[](u32 index) const;
    //inline const wsp::String operator[](int index) const;
};
END_WSP_CORE_NAMESPACE

//inline std::ostream& operator<<(std::ostream& lhs, const wsp::StringList& rhs)
//{
//    for(u32 itr_i=0; itr_i<rhs.length(); itr_i++){
//        lhs<<rhs[itr_i]<<endl;
//    }
//    return lhs;
//}

// Getter -------------------------------------------------
//inline u32 wsp::StringList::length() const{ return length_; }
//inline wsp::String* wsp::StringList::data(){ return data_; }
//inline wsp::String wsp::StringList::GetElem(u32 index) const{
//    wsp::String result("");
//    if(index>=length_){ return result; }
//    result = data_[index];
//    return result;
//}
// Setter -------------------------------------------------
//inline void wsp::StringList::SetData(const char *str, u32 index){
//    if(index>=length_){ return; }
//    data_[index].CopyFrom(str);
//}
//
// Operators --------------------------------------------
//inline wsp::StringList& wsp::StringList::operator=(const wsp::StringList& other){
//    if(*this == other){
//        return *this;
//    }
//    delete[] data_;
//    length_ = other.length();
//    if(length_==0){
//        data_=NULL;
//        return *this;
//    }
//    data_ = WSP_NEW wsp::String[length_];
//    for(u32 i=0; i<length_; i++){
//        data_[i]=other.GetElem(i);
//    }
//    return *this;
//}
//inline bool wsp::StringList::operator==(const wsp::StringList &other) const{
//    if(other.length()==0 && length_==0){ return true; }
//    if(other.length() != length_){ return false; }
//    for(u32 itr_i=0; itr_i<length_; itr_i++){
//        if(data_[itr_i] != other.GetElem(itr_i)){
//            return false;
//        }
//    }
//    return true;
//}
//inline bool wsp::StringList::operator!=(const wsp::StringList &other) const{ return !(*this==other); }
//inline wsp::String& wsp::StringList::operator[](u32 index){
//    if(length_==0){ return data_[0]; }
//    if(index<length_){ return data_[index]; }
//    return data_[length_-1];
//}
//inline wsp::String& wsp::StringList::operator[](int  index){ return this->operator[]((u32)index); }
//inline const wsp::String wsp::StringList::operator[](u32 index) const{ return GetElem(index); }
//inline const wsp::String wsp::StringList::operator[](int  index) const{ return GetElem(index); }


#endif