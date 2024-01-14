/**
 * @file _fn-string.h
 * 
 * @brief functions which provide access to string processing functionality.
 */

#ifndef _WSP_COMMONCORE_FUNCTION_STRING_H_
#define _WSP_COMMONCORE_FUNCTION_STRING_H_


#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include "_define_commoncore.h"
#endif
#ifndef __WSP_COMMONCORE_CTYPE_H__
#include "_ctypes.h"
#endif


#ifdef __cplusplus 
extern "C" {
#endif

WSP_DLL_EXPORT int         WSP_GetTokenCharIndex(int token_index);
WSP_DLL_EXPORT const char* WSP_GetToken(const char *in_tokens, int index);
WSP_DLL_EXPORT int         WSP_Tokenize(char *o_tokens, const char *in_src, const char *in_div, int max_tokens);
                           
WSP_DLL_EXPORT int         WSP_StrLen(const char *in_src);
WSP_DLL_EXPORT void        WSP_GetNumAsString(char *o_num, int num, u32 padding);
WSP_DLL_EXPORT int         WSP_EnToEm(char *str);
WSP_DLL_EXPORT int         WSP_EmToEn(char *str);
WSP_DLL_EXPORT int         WSP_StrToHex(const char *src);
WSP_DLL_EXPORT bool        WSP_IsDigitString(const char *str);
WSP_DLL_EXPORT void        WSP_RemoveAnyTypedStrings(char *io_str, int(*isfunc)(int));
                           
WSP_DLL_EXPORT void        WSP_RemoveAlphabet(char *io_str);
WSP_DLL_EXPORT void        WSP_RemoveNumber(char *io_str);
WSP_DLL_EXPORT void        WSP_RemoveSpace(char *io_str);
                           
WSP_DLL_EXPORT void        WSP_ReplaceSpecifiedChar( char *io_str, size_t length, char target_char, char subst_char );
                           
WSP_DLL_EXPORT void        WSP_GetDirectoryPathFromFilePath( char *o_dst, const char *path );
WSP_DLL_EXPORT void        WSP_GetFileNameFromFilePath(char *o_dst, const char *path);


/**
 * @brief Rename only leaf node if global name is given.
 *        For example, "|pSphere1|pSphere2"  to  "|pSphere1|ball2"
 */
WSP_DLL_EXPORT void WSP_RenameLeafNode(
        const char *in_str, 
        char *out_str, 
        void (*rename_func)(char*), 
        const char *denom
    );

/** @brief Get name of leaf node */
WSP_DLL_EXPORT void WSP_GetLeafNodeName(const char *in_global_name, char *o_local_name, const char *denom);
WSP_DLL_EXPORT void WSP_RemoveNamespace(char *io_str, const char *denom);
WSP_DLL_EXPORT void WSP_RemoveMayaNamespace(char *io_str);

#ifdef __cplusplus 
}
#endif

#ifdef __cplusplus 

#include <vector>
#include <string>
#include <sstream>

#include <iostream>
#include <string.h>

#ifndef __WSP_COMMONUTIL_DEFINE_COMMONUTIL_H__
#include "_define_commonutil.h"
#endif

#define MAXNUM_TOKEN (128) //! max number of token
#define MAXLEN_TOKEN (128) //! max size of token represented as character
#define MAXSIZE_TOKEN (MAXNUM_TOKEN*MAXLEN_TOKEN) //! buffer size of token array
#define SPRTR wsp::kSeparator

namespace wsp{
    static const char kSeparator[MAX_LENGTH_OF_NAME] = "/";
    static const char kHexLower[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    static const char kHexUpper[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

    inline int GetTokenCharIndex(int token_index){ return token_index*MAXLEN_TOKEN; }
    WSP_DLL_EXPORT int Tokenize(char *o_tokens, const char *in_src, const char *in_div);
    inline const char* GetToken(const char *in_tokens, int index){ return &in_tokens[wsp::GetTokenCharIndex(index)]; }

    inline int StrLen(const char *in_src);
    inline void GetNumAsString(char *o_num, int num, u32 padding);
    WSP_DLL_EXPORT int EnToEm(char *str);
    WSP_DLL_EXPORT int EmToEn(char *str);
    WSP_DLL_EXPORT int StrToHex(const char *src);

    inline bool IsDigit(const char *str){
        return WSP_IsDigitString(str);
    }
    inline void RemoveAlphabet(char *io_str){ WSP_RemoveAnyTypedStrings(io_str, isalpha); }
    inline void RemoveNumber(char *io_str){ WSP_RemoveAnyTypedStrings(io_str, isdigit); }
    inline void RemoveSpace(char *io_str){ WSP_RemoveAnyTypedStrings(io_str, isspace); }


    /* Rename only leaf node if global name is given.
    * For example, "|pSphere1|pSphere2"  to  "|pSphere1|ball2"
    */
    inline void RenameLeafNode(const char *in_str, char *out_str, void(*rename_func)(char*), const char *denom = "|"){
        char tokens[MAX_LENGTH_OF_PATH], tmp_str[MAX_LENGTH_OF_PATH];
        int i;

        // Split name into local name
        int num_tokens = wsp::Tokenize(tokens, in_str, denom);

        int end_index = num_tokens - 1;
        strcpy(tmp_str, &tokens[wsp::GetTokenCharIndex(end_index)]);
        rename_func(tmp_str);
        char *out_ptr = out_str;

        size_t denom_size = strlen(denom);

        // Merge tokens to get global name
        for (i = 0; i<end_index; ++i){
            strcpy(out_ptr, &tokens[wsp::GetTokenCharIndex(i)]);
            out_ptr += strlen(&tokens[wsp::GetTokenCharIndex(i)]);
            strcpy(out_ptr, denom);
            out_ptr += denom_size;
        }
        strcpy(out_ptr, tmp_str);
    }
    /* Get name of leaf node */
    inline void GetLeafNodeName(const char *in_global_name, char *o_local_name, const char *denom = "|"){
        char tokens[MAX_LENGTH_OF_PATH];
        // Split name into local name
        int num_tokens = wsp::Tokenize(tokens, in_global_name, denom);
        strcpy(o_local_name, &tokens[wsp::GetTokenCharIndex(num_tokens - 1)]);
    }
    inline void RemoveNamespace(char *io_str, const char *denom){
        wsp::GetLeafNodeName(io_str, io_str, denom);
    }
    inline void RemoveMayaNamespace(char *io_str){ wsp::RemoveNamespace(io_str, ":"); }

    inline std::vector<std::string> Split(const std::string &s, char delim)
    {
        std::vector<std::string> elems;
        std::stringstream ss(s);
        std::string item;
        while (getline(ss, item, delim))
        {
            if (!item.empty())
            {
                elems.push_back(item);
            }
        }
        return elems;
    }


#ifdef USE_QT
    //int qstrcpy(unsigned short *dest, const QString &src);
    //int qstrcpy(QString& dest, const unsigned short *src);
#endif
}

inline int wsp::StrLen(const char *in_src){
    int len = 0;
    while (in_src[len] != '\0'){ len++; }
    return len;
}
inline void wsp::GetNumAsString(char *o_dest, int num, u32 padding)
{
    if (padding != 0)
    {
        char format[MAX_LENGTH_OF_NAME];
        sprintf(format, "%%0%ud", padding);
        sprintf(o_dest, format, num);
        return;
    }
    strcpy(o_dest, "");
}
#endif

#endif