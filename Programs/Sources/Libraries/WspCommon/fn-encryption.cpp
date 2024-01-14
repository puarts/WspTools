// wspfnencryption.cpp
// 


#include "_tpl_fn-sort.hpp"
#include "_tpl_fn-array.hpp"
#include "cl-string_list.h"
#ifndef __WSP_COMMONUTIL_FN_UTIL_H__
#include "fn-util.h"
#endif
#include "fn-encryption.h"

#define KEY_NUM 3


// ----------------------------------------------------------------

std::vector<std::string> wsp::DecodeSubOfNeighbors(const std::string &in_msg)
{
    std::vector<std::string> result;
    std::string res0, res1;
    int i, p[3];
    int len = (int)in_msg.length();

    //! if first bit is 0
    for(i=0; i<len; i++){
        if(i-1<0){ p[0] = 0; }
        if(i+1>=len){ p[2] = 0; }
        //p[1] = ;

    }

    //! if first bit is 1

    return result;
}

void wsp::XorMsg(char *io_msg, const char *in_key, u32 length)
{
    if(length > WSP_STRING_MAX_BUFFER){ WSP_COMMON_ERROR_LOG("length is too large\n"); return; }

    char tmp_msg[WSP_STRING_MAX_BUFFER];
    memcpy(tmp_msg, io_msg, length);
    for(u32 itr_i=0; itr_i<length; itr_i++){
        io_msg[itr_i] = tmp_msg[itr_i]^in_key[itr_i];
    }
    io_msg[length]='\0';
}

void wsp::ShiftMsg(char *io_msg, int shiftByte, u32 length)
{
    if(length > WSP_STRING_MAX_BUFFER){ WSP_COMMON_ERROR_LOG("length is too large\n"); return; }
    if(shiftByte > (int)length){ WSP_COMMON_ERROR_LOG("shiftByte is too large\n"); return; } 

    int pos;
    char tmp_msg[WSP_STRING_MAX_BUFFER];
    memcpy(tmp_msg, io_msg, length);
    for(int itr_i=0; itr_i<length; itr_i++){
        pos=itr_i - shiftByte;
        if(pos>=0 && pos<length){
            io_msg[pos] = tmp_msg[itr_i];
        }else if(pos<0){
            io_msg[(int)length+pos] = tmp_msg[itr_i];
        }else{
            io_msg[pos-(int)length] = tmp_msg[itr_i];
        }
    }
    io_msg[length]='\0';
}

void wsp::EncryptByXorShift(char *io_msg, const wsp::StringList &in_keys, u32 length, int shiftByte)
{
    int keyLen = static_cast<int>(in_keys.length());
    for(int i=0; i<keyLen; i++){
        wsp::XorMsg(io_msg, in_keys[i].data(), length);
        //if(wsp::dbgmode>=WSP_DEBUGMODE_LEVEL2){
        //    cout<<i<<"-xor: "; wsp::PrintArrayAsHex(reinterpret_cast<const uchar*>(io_msg), length); cout<<endl;
        //    cout<<i<<"-key: "; wsp::PrintArrayAsHex(reinterpret_cast<const uchar*>(in_keys[i].data()), length); cout<<endl<<endl;
        //}
        wsp::ShiftMsg(io_msg, shiftByte, length);
        //if(wsp::dbgmode>=WSP_DEBUGMODE_LEVEL2){
        //    cout<<i<<"-shift: "; wsp::PrintArrayAsHex(reinterpret_cast<const uchar*>(io_msg), length); cout<<endl<<endl;
        //}
    }
}
void wsp::DecryptByXorShift(char *io_msg, const wsp::StringList &in_keys, u32 length, int shiftByte)
{
    int keyLen = (int)in_keys.length();
    for(int i=keyLen-1; i>=0; i--){
        wsp::ShiftMsg(io_msg, -shiftByte, length);
        //if(wsp::dbgmode>=WSP_DEBUGMODE_LEVEL2){
        //    cout<<i<<"-shift: "; wsp::PrintArrayAsHex(reinterpret_cast<const uchar*>(io_msg), length); cout<<endl<<endl;
        //}
        wsp::XorMsg(io_msg, in_keys[i].data(), length);
        //if(wsp::dbgmode>=WSP_DEBUGMODE_LEVEL2){
        //    cout<<i<<"-xor: "; wsp::PrintArrayAsHex(reinterpret_cast<const uchar*>(io_msg), length); cout<<endl;
        //    cout<<i<<"-key: "; wsp::PrintArrayAsHex(reinterpret_cast<const uchar*>(in_keys[i].data()), length); cout<<endl<<endl;
        //}
    }
}

