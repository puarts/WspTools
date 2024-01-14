// @file fn-encryption.h
// 


#ifndef WspFnEncryption_H_
#define WspFnEncryption_H_

#include <vector>
#include <string>

#include "_define_commoncore.h"
#include "_ctypes.h"

namespace wsp{
    class StringList;
}

#define LICENSE_LEN (4)

namespace wsp{
    WSP_DLL_EXPORT wsp::State GetMacAddressFromLicenseFile(uchar *o_macAddr, const char *in_licensePath, const char *in_selfPath);
    WSP_DLL_EXPORT void GenerateKey(const char *in_licensePath, const char *in_binPath);
    WSP_DLL_EXPORT void XorMsg(char *io_msg, const char *in_key, u32 length);
    WSP_DLL_EXPORT void ShiftMsg(char *io_msg, int shiftByte, u32 length);
    WSP_DLL_EXPORT void EncryptByXorShift(char *io_msg, const wsp::StringList &in_keys, u32 length, int shiftByte);
    WSP_DLL_EXPORT void DecryptByXorShift(char *io_msg, const wsp::StringList &in_keys, u32 length, int shiftByte);

    //! Encryption by sum of neighbors
    WSP_DLL_EXPORT std::string EncodeSumOfNeighbors(const std::string &in_data);
    WSP_DLL_EXPORT std::vector<std::string> DecodeSubOfNeighbors(const std::string &in_data);
}

#endif