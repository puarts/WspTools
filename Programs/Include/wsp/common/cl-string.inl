/**
 * @file cl-string.inl
 * 
 */

#ifndef __WSP_CORE_CLASS_STRING_INL__
#define __WSP_CORE_CLASS_STRING_INL__

#include <stdarg.h>

inline std::ostream& operator<<(std::ostream& lhs, const wsp::String& rhs)
{
    for(int itr_i=0; itr_i<rhs.length(); itr_i++){
        lhs<<rhs[itr_i];
    }
    return lhs;
}
inline wsp::String operator-(wsp::String lhs, wsp::String rhs){
    return static_cast<wsp::String>(atof(lhs.data())-atof(rhs.data()));
}


// Constructor, Destructor -----------------------------------------
inline wsp::String::String(){}
inline wsp::String::String(const char *string){
    strcpy(data_, string);
    length_ = strlen(string);
}
inline wsp::String::String(const wsp::String &str){
    length_ = str.length();
    memset(data_, 0, WSP_STRING_MAX_BUFFER);
    memcpy(data_, str.data(), length_*sizeof(char));
    //for(int i=0; i<length_; i++){
    //    data_[i]=str[i];
    //}
    //data_[i]='\0';
}
inline wsp::String::~String(){}


// Getter ----------------------------------------------
inline void wsp::String::CopyInto(char *o_dest, int length) const{
    if(length==0){
        strcpy(o_dest, data_);
    }
    for(int i=0; i<length; i++){
        o_dest[i] = data_[i];
    }
}

inline char wsp::String::GetElem(int index) const{
    if(strlen(data_)==0){ return '\0'; }
    if(index<strlen(data_)){    return data_[index]; }
    return data_[strlen(data_)-1];
}

// Setter ----------------------------------------------
inline void wsp::String::CopyFrom(const char *in_str, int length){
    if(length>WSP_STRING_MAX_BUFFER){ return; }
    //memset(data_, '\0', WSP_STRING_MAX_BUFFER);
    length_ = length;
    if(length_==0){ data_[0]='\0'; return; }
    //memcpy(data_, in_str, length_*sizeof(char));
    for(int i=0; i<length_; i++){
        data_[i] = in_str[i];
    }
}
inline void wsp::String::Sprintf(char *fmt, ...)
{
    char str[MAX_BUFFER];
    va_list ap;
    va_start(ap, fmt);
    vsprintf(str, fmt, ap);
    operator=(wsp::String(str));
    va_end(ap);
}
inline void wsp::String::SprintfAdd(char *fmt, ...)
{
    char str[MAX_BUFFER];
    va_list ap;
    va_start(ap, fmt);
    vsprintf(str, fmt, ap);
    operator+=(wsp::String(str));
    va_end(ap);
}


// Operators -------------------------------------------
inline wsp::String& wsp::String::operator=(int rhs){
    char tmp_str[WSP_STRING_MAX_BUFFER];
    sprintf(tmp_str, "%d", rhs);
    memset(data_, '\0', WSP_STRING_MAX_BUFFER);
    strcpy(data_, tmp_str);
    length_=strlen(data_);
    return *this;
}
inline wsp::String& wsp::String::operator=(const wsp::String& other){
    if(*this == other){ return *this; }
    int length = other.length();
    if(length!=0){
        memset(data_, '\0', WSP_STRING_MAX_BUFFER);
        for(int i=0; i<length; i++){
            data_[i]=other[i];
        }
        return *this;
    }
    length_=strlen(data_);
    return *this;
}

inline wsp::String wsp::String::operator+ (const wsp::String &rhs) const{
    int i, len = (int)rhs.length(), clen=(int)length();
    char tmp_str[WSP_STRING_MAX_BUFFER];
    strcpy(tmp_str, data());
    for(i=0; i<len; i++){
        tmp_str[i+clen]=rhs[i];
    }
    tmp_str[i+clen]='\0';
    wsp::String str(tmp_str);
    return str;
}

inline bool wsp::String::operator==(const wsp::String &rhs) const{
    if(rhs.length()!=length_){ return false; }
    for(int itr_i=0; itr_i<length_; itr_i++){
        if(data_[itr_i] != rhs[itr_i]){ return false; }
    }
    return true;
    //return !strcmp(data_, rhs.data());
}

#endif
