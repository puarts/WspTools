/**
 * @file cl-string.h
 * 
 */

#ifndef __WSP_CORE_CLASS_STRING_H__
#define __WSP_CORE_CLASS_STRING_H__

#include <iostream>
#include <string.h>
#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#include "tpl_cl-vector.h"
#endif
#include "_tpl_cl-stackarray.h"

#define WSP_STRING_MAX_BUFFER 1024

namespace wsp{
    //! \ingroup WspCommonUtil
    //! \brief String munipulator
    /*!
        wsp::String is a class which provides to access some of munipulation of
        string functionality.
    */
    class WSP_DLL_EXPORT String
        : public wsp::StackArray<char, WSP_STRING_MAX_BUFFER>
    {
    public:
        inline String();
        inline String(const char *str);
        inline String(const String &str);
        inline String(double value){ sprintf(data_, "%f", value); }
        inline String(int value){ sprintf(data_, "%d", value); }
        inline ~String();

        // Getter
        inline int length()const { return strlen(data_); }
        inline char GetElem(int index) const;
        inline void CopyInto(char *o_dest, int length=0) const;
        //inline char* data(){ return data_; }
        //inline const char* data() const{ return data_; }
        inline bool IsEmpty() const{
            for(int i=0; i<length(); i++){
                if(!isspace(data_[i])){ return false; }
            }
            return true;
        }


        // Setter
        inline void CopyFrom(const char *in_str, int length=0);
        inline void Sprintf(char *fmt, ...);
        inline void SprintfAdd(char *fmt, ...);
        inline void Clear(){ 
            memset(data_, 0, WSP_STRING_MAX_BUFFER);
            length_=0;
            //memset(data_, 0, WSP_STRING_MAX_BUFFER-1); 
        }
        //! this is faster because of no dynamic memory allocation
        wsp::String Split(const char deliminator, int fetch_index) const;
        wsp::Vector<wsp::String> Split(const char deliminator) const;

        /* Operators */
        inline wsp::String& operator=(int rhs);
        inline wsp::String& operator=(const wsp::String &rhs);
        inline wsp::String operator=(double rhs) const{ return wsp::String(rhs); }
        inline wsp::String& operator=(double rhs){ sprintf(data_, "%f", rhs); return *this; }
        inline bool operator==(const wsp::String &rhs) const;
        inline bool operator!=(const wsp::String &rhs) const{ return !(*this==rhs); }

        inline bool operator==(const char *rhs) const{ return (strcmp(data_, rhs)==0); }
        inline bool operator!=(const char *rhs) const{ return !(*this==rhs); }
        inline bool operator<(const wsp::String &rhs) const{ return strlen(data_)<rhs.length(); }
        inline bool operator>(const wsp::String &rhs) const{ return strlen(data_)>rhs.length(); }
        inline bool operator<=(const wsp::String &rhs) const{ return strlen(data_)<=rhs.length(); }
        inline bool operator>=(const wsp::String &rhs) const{ return strlen(data_)>=rhs.length(); }

        inline wsp::String operator+ (const wsp::String &rhs) const;
        inline wsp::String& operator+= (const wsp::String &rhs){
            *this = *this + rhs; return *this;
        }
        inline wsp::String operator+(const char *right) const{
            return wsp::String(*this)+wsp::String(right);
        }
        inline wsp::String& operator+=(const char *right){
            *this+=wsp::String(right); return *this;
        }

        inline char operator[](int index) const{ 
            if(index>=length() || index<0){ WSP_COMMON_ERROR_LOG("wrong index %d\n", index); return static_cast<char>(0); }
            return data_[index]; 
        }
        inline char& operator[](int index){ 
            if(index<0 || index>WSP_STRING_MAX_BUFFER){ WSP_COMMON_ERROR_LOG("wrong index %d\n", index); return data_[0]; }
            if(length_>=index){ length_=index+1; }
            return data_[index]; 
        }

        /* casting operators */
        inline operator double() const{ return atof(data_); }
        //inline operator int() const{ return atoi(data_); }
    };

}



#include "cl-string.inl"


#endif
