// @file cl-text_stream.h
// 


#ifndef _WspTextStream_H_
#define _WspTextStream_H_

#include "cl-string.h"

#define WSP_TEXTSTREAM_MAX_BUFFER 4096

namespace wsp{
    class WSP_DLL_EXPORT TextStream
    {
    public:
        TextStream(const char *input_text);
        inline TextStream(const wsp::TextStream &ts){
            sprintf(data_, ts.data());
            size_ = ts.size();
            pointer_ = ts.pointer();
        }

        // Getter ---------------------------
        int pointer() const{ return pointer_; }
        int size() const{ return size_; }
        inline bool AtEnd() const;
        inline char* data(){ return data_; }
        inline const char* data()const{ return data_; }

        // Operators ------------------------
        // extract one string by space deliminator
        inline wsp::TextStream& operator>>(wsp::String &o_text);
        // extract float value from text stream by space deliminator
        inline wsp::TextStream& operator>>(float &o_value);

    private:
        void Init();    

    private:
        char data_[WSP_TEXTSTREAM_MAX_BUFFER];
        int size_;
        int pointer_;

    };

}

// Getter ---------------------------
inline bool wsp::TextStream::AtEnd() const{
    if(pointer_>=size_){ return true; }
    int tmp_pointer = pointer_;
    while(isspace(data_[tmp_pointer])){
        tmp_pointer++;
        if(size_<=tmp_pointer){ return true; }
    }
    return false;
}

// Operators ------------------------
// extract one string by space deliminator
inline wsp::TextStream& wsp::TextStream::operator>>(wsp::String &o_text){
    o_text.Clear();
    while(isspace(data_[pointer_])){
        ++pointer_;
        if(size_<=pointer_){ o_text[0]='\0'; return *this; }
    }
    int i=0;
    while(!isspace(data_[pointer_])){
        o_text[i] = data_[pointer_];
        ++pointer_; ++i;
        if(i>=WSP_STRING_MAX_BUFFER){
            WSP_COMMON_ERROR_LOG("Result exceeds max buffer of wsp::String: %d>=%d", i, WSP_STRING_MAX_BUFFER); return *this;
        }
        if(size_<=pointer_){ break; }
    }
    o_text[i]='\0';
    return *this;
}
// extract float value from text stream by space deliminator
inline wsp::TextStream& wsp::TextStream::operator>>(float &o_value){
    wsp::String text;
    *this>>text;
    o_value = atof(text.data());
    return *this;
}


#endif