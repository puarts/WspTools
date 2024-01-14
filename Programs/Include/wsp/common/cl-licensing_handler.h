// @file cl-licensing_handler.h
// 


#ifndef __WspLicensingHandler_H__
#define __WspLicensingHandler_H__

#include "_tpl_fn-array.hpp"
#include "cl-string_list.h"

class WspLicensingHandler
{
private:
    u32 m_licenseDataLen;
    wsp::StringList m_keys;
    char m_licensePath[MAX_LENGTH_OF_PATH];
    //char m_binaryPath[MAX_LENGTH_OF_PATH];

private:
    // Getter --------------------
    WSP_DLL_EXPORT wsp::State GetMacAddressFromLicenseFile(uchar *o_macAddr) const;

public:
    WSP_DLL_EXPORT WspLicensingHandler();
    WSP_DLL_EXPORT ~WspLicensingHandler();

    // Getter --------------------
    //inline void GetBinaryPath(char *o_dest) const;
    inline void GetLicensePath(char *o_dest) const;
    //inline const char* GetBinaryPathPtr() const;
    inline const char* GetLicensePathPtr() const;
    inline u32 GetLicenseDataLength() const;
    inline wsp::StringList GetKeys() const;

    // Setter --------------------
    WSP_DLL_EXPORT void AppendKey(const char *key);
    inline void ResetKeys();
    inline void SetLicenseDataSize(u32 size);
    inline void SetLicensePath(const char *in_licensePath);
    //inline void SetBinaryPath(const char *in_binaryPath);

    //! Main ----------------------
    WSP_DLL_EXPORT wsp::State GenerateLicenseFile(const char *in_destPath, const uchar* in_macAddr=NULL) const;
    WSP_DLL_EXPORT wsp::State Licensing() const;

    //! Print ---------------------
    inline void PrintKeys() const;
    inline void PrintKeysAsHex() const;
};

// Getter ----------------------------------------------------------
//inline void WspLicensingHandler::GetBinaryPath(char *o_dest) const{ strcpy(o_dest, m_binaryPath); }
inline void WspLicensingHandler::GetLicensePath(char *o_dest) const{ strcpy(o_dest, m_licensePath); }
//inline const char* WspLicensingHandler::GetBinaryPathPtr() const{ return m_binaryPath; }
inline const char* WspLicensingHandler::GetLicensePathPtr() const{ return m_licensePath; }
inline u32 WspLicensingHandler::GetLicenseDataLength() const{ return m_licenseDataLen; }
inline wsp::StringList WspLicensingHandler::GetKeys() const{
    wsp::StringList ret(m_keys);
    return ret;
}


// Setter ----------------------------------------------------------
inline void WspLicensingHandler::ResetKeys(){ m_keys.SetLength(0); }
//inline void WspLicensingHandler::SetBinaryPath(const char *in_binaryPath){ strcpy(m_binaryPath, in_binaryPath); }
inline void WspLicensingHandler::SetLicensePath(const char *in_licensePath){ strcpy(m_licensePath, in_licensePath); }
inline void WspLicensingHandler::SetLicenseDataSize(u32 size){
    if(size>=WSP_STRING_MAX_BUFFER){
        m_licenseDataLen=WSP_STRING_MAX_BUFFER;
        return;
    }
    m_licenseDataLen=size;
}

//! Print -----------------------------------------------------------
inline void WspLicensingHandler::PrintKeys() const{
    for(u32 i=0; i<m_keys.length(); i++){
        std::cout<<m_keys[i];
        std::cout<<std::endl;
    }
}
inline void WspLicensingHandler::PrintKeysAsHex() const{
    for(u32 i=0; i<m_keys.length(); i++){
        wsp::PrintArrayAsHex((uchar*)m_keys[i].data(), m_licenseDataLen);
        std::cout<<std::endl;
    }
}

#endif