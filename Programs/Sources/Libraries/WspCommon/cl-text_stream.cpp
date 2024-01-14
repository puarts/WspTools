// WspTextStream.cpp
// 

#include "cl-text_stream.h"




// Constructor, Destructor ---------------------------------
#define INIT_TextStream size_(0), pointer_(0)
void wsp::TextStream::Init(){
    memset(data_, 0, WSP_TEXTSTREAM_MAX_BUFFER);
}

wsp::TextStream::TextStream(const char *input_text)
    :INIT_TextStream
{
    Init();
    if(input_text==NULL){ WSP_COMMON_ERROR_LOG("input text is NULL\n"); return; }
    if(strlen(input_text)>WSP_TEXTSTREAM_MAX_BUFFER){
        WSP_COMMON_ERROR_LOG("The size of input text exceeds max buffer size\n"); return;
    }
    size_ = strlen(input_text);
    strcpy(data_, input_text);
}


