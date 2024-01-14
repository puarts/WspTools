//! wsplicensinghandler.cpp
// 

//#include <ctype.h>
//#include <io.h>
#include <sys/stat.h>
//#include <sys/types.h>
//#include <errno.h>

#ifndef __WSP_COMMONUTIL_FN_UTIL_H__
#include "fn-util.h"
#endif
#include "fn-encryption.h"
#include "_tpl_fn-array.hpp"
#include "cl-licensing_handler.h"

using namespace std;


// Constructor, Destructor -----------------------------------------
#define INIT_WspLicensingHandler m_licenseDataLen(512)
WspLicensingHandler::WspLicensingHandler()
    :INIT_WspLicensingHandler
{}
WspLicensingHandler::~WspLicensingHandler()
{}

// Setter ----------------------------------------------------------
void WspLicensingHandler::AppendKey(const char *key)
{
    m_keys.Append(wsp::String(key));
    //u32 i, length;
    //char *tmp_str;
    //wsp::StringList tmp_keys(m_keys);
    //tmp_str = WSP_NEW char[m_licenseDataLen+10];
    //memset(tmp_str, 0, m_licenseDataLen+10);
    //length = m_keys.length()+1;
    //m_keys.SetLength(length);
    ////! Set data to fix length ignoring '\0'
    //for(i=0; i<length-1; i++){
    //    //memcpy(m_keys[i].data(), tmp_keys[i].data(), m_licenseDataLen);
    //    tmp_keys[i].CopyFrom(tmp_str);
    //    m_keys[i].CopyFrom(tmp_keys[i].data(), m_licenseDataLen);
    //}
    //m_keys[length-1].CopyFrom(key, m_licenseDataLen);
    //delete[] tmp_str;
}

//! Main ------------------------------------------------------------
wsp::State WspLicensingHandler::GenerateLicenseFile(const char *in_destPath, const uchar* in_macAddr) const
{
    uchar macAddr[6];
    int status, shiftByte = m_licenseDataLen/5;
    FILE *fp;
    uchar result[WSP_STRING_MAX_BUFFER];

    //! Get MAC Address
    if(in_macAddr==NULL){
        status = wsp::GetMacAddress(macAddr);
        if (status == -1) { WSP_COMMON_ERROR_LOG("Can't get MAC Address"); return WSP_STATE_FAILURE; }
    }else{
        wsp::CopyArray<uchar>(macAddr, in_macAddr, 6);
    }

    //! debug output
    cout<<"DEBUG MAC address in hex: ";  wsp::PrintArrayAsHex(macAddr, 6); cout<<endl;

    //! Encryption ---------------------------------
    //! init result array
    memset(result, 11, m_licenseDataLen); 
    result[m_licenseDataLen] = '\0';
    wsp::CopyArray<uchar>(result, macAddr, 6);

    //! encrypt by xor and shift
    //cout<<"keys-------------"<<endl;
    //wsp::PrintArrayAsHex(reinterpret_cast<const uchar*>(m_keys[0].data()), m_licenseDataLen);
    //cout<<endl;
    wsp::EncryptByXorShift((char*)result, m_keys, m_licenseDataLen, shiftByte);
    //cout<<"keys-------------"<<endl;
    //wsp::PrintArrayAsHex(reinterpret_cast<const uchar*>(m_keys[0].data()), m_licenseDataLen);
    //cout<<endl;


    //! debug output
    cout<<"Encrypted: "<< endl; wsp::PrintArrayAsHex(result, m_licenseDataLen); cout<<endl;
    //cout<<"keys-----------\n";
    //PrintKeysAsHex();

    //! Write ----------------------------------------
    fp = fopen(in_destPath, "wb");
    if (fp==NULL){ WSP_COMMON_ERROR_LOG("Opening file failed: %s", in_destPath); return WSP_STATE_FAILURE; }
    fwrite(result, sizeof(uchar), m_licenseDataLen, fp);
    fclose(fp);

    //! debug decrypted output
    wsp::DecryptByXorShift((char*)result, m_keys, m_licenseDataLen, shiftByte);
    cout<<"DEBUG Decrypted: "; wsp::PrintArrayAsHex(result, 6); cout<<endl;
    //cout<<"keys-----------\n";
    //PrintKeysAsHex();

    return WSP_STATE_SUCCESS;
}

wsp::State WspLicensingHandler::Licensing() const
{
    u32 i;
    uchar addr_sys[6], addr_file[6];
    wsp::State state;

    //! Get MAC Address from system and license file
    state = wsp::GetMacAddress(addr_sys);
    if (state == WSP_STATE_FAILURE) {
        WSP_COMMON_ERROR_LOG("can't get MAC address from system\n"); return WSP_STATE_FAILURE;
    }
    state = GetMacAddressFromLicenseFile(addr_file);
    if (state == WSP_STATE_FAILURE) {
        WSP_COMMON_ERROR_LOG("can't get MAC address from license file\n"); return WSP_STATE_FAILURE;
    }

#ifdef USE_QT
    //char addr_sysx[64]="", addr_filex[64]="";
    //for (i=0; i<6; i++) {
    //    sprintf(addr_sysx, "%s%2.2x ", addr_sysx, addr_sys[i]);
    //    sprintf(addr_filex, "%s%2.2x ", addr_filex, addr_file[i]);
    //}
    //QMessageBox::information(0,QString("DEBUG MAC address"), QString(addr_sysx)+QString("\n")+QString(addr_filex));
#endif

    //! Compare them
    for (i=0; i<6; i++) {
        if(addr_sys[i]!=addr_file[i]){ return WSP_STATE_FAILURE; }
        printf("%2.2x ", addr_file[i]);
    }
    printf("\n");

    return WSP_STATE_SUCCESS;
}

wsp::State WspLicensingHandler::GetMacAddressFromLicenseFile(uchar *o_macAddr) const
{
    uchar macAddr[6];
    u32 readLen;
    int status, shiftByte = m_licenseDataLen/5;
    FILE *fp;
    uchar result[WSP_STRING_MAX_BUFFER];

    //! Read file ----------------------------------------
    fp = fopen(m_licensePath, "rb");
    if (!fp){ WSP_COMMON_ERROR_LOG("Can't find license file %s\n", m_licensePath); return WSP_STATE_FAILURE; }
    readLen = fread(result, sizeof(uchar), m_licenseDataLen, fp);
    if (readLen != m_licenseDataLen){ WSP_COMMON_ERROR_LOG("reading license file failed\n"); fclose(fp); return WSP_STATE_FAILURE; }
    //! debug output
    cout<<"DEBUG data: "; wsp::PrintArrayAsHex(result, m_licenseDataLen); cout<<endl;
    //cout<<"keys-----------\n";
    //PrintKeysAsHex();

    //! Decryption ---------------------------------
    wsp::DecryptByXorShift((char*)result, m_keys, m_licenseDataLen, shiftByte);

    //! debug output
    cout<<"DEBUG Decrypted: "; wsp::PrintArrayAsHex(result, 6); cout<<endl;
    //cout<<"keys-----------\n";
    //PrintKeysAsHex();

    for(u32 itr_i=0; itr_i<6; itr_i++){
        o_macAddr[itr_i]=result[itr_i];
    }

    return WSP_STATE_SUCCESS;
}