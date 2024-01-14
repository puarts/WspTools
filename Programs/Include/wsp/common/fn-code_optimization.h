/**
 * @file fn-code_optimization.h
 * 
 */

#ifndef _WSP_CORE_BASE_FN_CODE_OPTIMIZATION_H_
#define _WSP_CORE_BASE_FN_CODE_OPTIMIZATION_H_


#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include "_define_commoncore.h"
#endif
#ifndef __WSP_COMMONCORE_RESULT_H__
#include "result.h"
#endif

#ifdef __cplusplus
extern "C"{
#endif

WSP_DLL_EXPORT WSP_Result WSP_RemoveCppComment( const char *src_file_path, const char *dest_file_path );
WSP_DLL_EXPORT WSP_Result WSP_RemoveHtmlComment( 
    const char *src_file_path, 
    const char *dest_file_path 
);
WSP_DLL_EXPORT WSP_Result WSP_RemoveHtmlCommentExceptStyleAndScript( 
    const char *src_file_path, 
    const char *dest_file_path 
);
WSP_DLL_EXPORT WSP_Result WSP_RemoveLineFeed( const char *src_file_path, const char *dest_file_path );
WSP_DLL_EXPORT WSP_Result WSP_RemoveCarriageReturn( const char *src_file_path, const char *dest_file_path );
WSP_DLL_EXPORT WSP_Result WSP_RemoveContinuousSpaces( const char *src_file_path, const char *dest_file_path );
WSP_DLL_EXPORT WSP_Result WSP_ReplaceCharInFile( 
    const char *src_file_path, 
    const char *dest_file_path,
    char target_char, 
    char subst_char
);


#ifdef __cplusplus
}
#endif


#endif
