/**
 * @file _fn-code_optimization.c
 * 
 */

#include <stdio.h>
#include <string.h>

#ifndef __WSP_COMMONCORE_COMMON_RESULT_H__
#include "common_result.h"
#endif
#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include "_define_commoncore.h"
#endif
#include "fn-io.h"

#include "fn-code_optimization.h"

#define MAX_READ_SIZE (4096U)


WSP_Result WSP_ReplaceCharInFile( 
    const char *src_file_path, 
    const char *dest_file_path,
    char target_char, 
    char subst_char
)
{
    FILE *fp_src, *fp_dest;
    char read_buf[MAX_READ_SIZE], write_buf[MAX_READ_SIZE];

    size_t read_size;

    fp_src = fopen( src_file_path, "r" );
    if( fp_src==NULL ){
        WSP_COMMON_ERROR_LOG( "Could not open %s\n", src_file_path );
        return WSP_ResultStatusError();
    }
    fp_dest = fopen( dest_file_path, "w" );
    if( fp_dest==NULL ){
        WSP_COMMON_ERROR_LOG( "Could not open %s\n", dest_file_path );
        fclose( fp_src );
        return WSP_ResultStatusError();
    }

    while( (read_size=fread(read_buf, sizeof(char), MAX_READ_SIZE-1, fp_src)) != 0 )
    {
        char *write_ptr = write_buf;
        char *read_ptr = read_buf;
        char *end_ptr  = read_ptr + read_size;
        while( read_ptr<end_ptr && *read_ptr!='\0' )
        {
            if( *read_ptr==target_char )
            {
                *write_ptr = subst_char;
                ++read_ptr;
                ++write_ptr;
                continue;
            }
            *write_ptr = *read_ptr;
            ++read_ptr;
            ++write_ptr;
        }
        *write_ptr = '\0';
        fwrite( write_buf, 1, (u32)(write_ptr-write_buf), fp_dest); 
        WSP_COMMON_DEBUG_LOG("%s", write_buf);
    }



    fclose( fp_src );
    fclose( fp_dest );
    return WSP_ResultSuccess();
}

#define DEST_BUF_SIZE (MAX_READ_SIZE+256)
WSP_Result WSP_RemoveHtmlCommentExceptStyleAndScript( 
    const char *src_file_path, 
    const char *dest_file_path 
)
{
    const char *comment_start = "<!--";
    const char *comment_end   = "-->";
    const char *style_start   = "<style";
    const char *style_end     = "</style";
    const char *script_start  = "<script";
    const char *script_end    = "</script";

    FILE *fp_src, *fp_dest;
    char read_buf[MAX_READ_SIZE], write_buf[DEST_BUF_SIZE];

    size_t read_size;
    bool inside_comment = false;
    bool inside_double_quotation  = false; /* inside of "" */
    bool inside_single_quotation  = false; /* inside of '' */
    bool inside_script = false;
    bool inside_style  = false;

    u32 num_char_of_comment_start = strlen( comment_start );
    u32 num_char_of_comment_end = strlen( comment_end );
    u32 num_char_of_style_start = strlen( style_start );
    u32 num_char_of_style_end   = strlen( style_end );
    u32 num_char_of_script_start = strlen( script_start );
    u32 num_char_of_script_end   = strlen( script_end );

    const char *com_ptr;
    const char *style_ptr;
    const char *script_ptr;
    bool in_comment = false;
    u32 match_cnt = 0;
    u32 style_match_cnt = 0;
    u32 script_match_cnt = 0;
    u32 share_match_cnt = 0;
    char last_char_in_buf = '\0';
    u32 write_size, res_size;

    fp_src = fopen( src_file_path, "r" );
    if( fp_src==NULL ){
        WSP_COMMON_ERROR_LOG( "Could not open %s\n", src_file_path );
        return WSP_ResultStatusError();
    }
    fp_dest = fopen( dest_file_path, "w" );
    if( fp_dest==NULL ){
        WSP_COMMON_ERROR_LOG( "Could not open %s\n", dest_file_path );
        fclose( fp_src );
        return WSP_ResultStatusError();
    }

    //WSP_COMMON_DEBUG_LOG("num_com_start = %u, num_com_end = %u\n", num_char_of_comment_start, num_char_of_comment_end );

    while( (read_size=fread(read_buf, sizeof(char), MAX_READ_SIZE, fp_src)) != 0 )
    {
        char *write_ptr, *read_ptr, *end_read_ptr;
        write_ptr = write_buf;
        read_ptr = read_buf;
        end_read_ptr  = read_ptr + read_size;
        while( read_ptr<end_read_ptr && *read_ptr!='\0' )
        {
            if( in_comment ){
                com_ptr = comment_end + match_cnt;
                for( ; 
                    match_cnt<num_char_of_comment_end && 
                    read_ptr<end_read_ptr; 
                    ++read_ptr
                )
                {
                    //WSP_COMMON_DEBUG_LOG("%c==%c\n", *read_ptr, *com_ptr);
                    if( *read_ptr != *com_ptr )
                    {
                        printf("%c", *read_ptr);
                        match_cnt = 0;
                        com_ptr = comment_end;
                        continue;
                    }
                    
                    ++match_cnt;
                    ++com_ptr;
                }

                if( read_ptr>=end_read_ptr ){
                    break;
                }
                /* comment end */
                in_comment = false;
                match_cnt = 0;
                printf(
                    "\n[break comment block]\n"
                );
            }
            else if(inside_style)
            {
                style_ptr = style_end + style_match_cnt;
                for(; 
                    style_match_cnt<num_char_of_style_end &&
                    read_ptr<end_read_ptr; 
                    ++read_ptr)
                {
                    *write_ptr = *read_ptr;
                    ++write_ptr;
                    printf("%c", *read_ptr);
                    if( *read_ptr != *style_ptr )
                    {
                        style_match_cnt = 0;
                        style_ptr = style_end;
                        continue;
                    }
                    
                    ++style_match_cnt;
                    ++style_ptr;
                }
                if( read_ptr>=end_read_ptr ){
                    break;
                }
                inside_style = false;
                style_match_cnt = 0;
                printf(
                    "\n[break style]\n"
                );
                continue;
            }
            else if(inside_script)
            {
                script_ptr = script_end + script_match_cnt;
                for(; 
                    script_match_cnt<num_char_of_script_end &&
                    read_ptr<end_read_ptr; 
                    ++read_ptr)
                {
                    *write_ptr = *read_ptr;
                    ++write_ptr;
                    printf("%c", *read_ptr);
                    if( *read_ptr != *script_ptr )
                    {
                        script_match_cnt = 0;
                        script_ptr = script_end;
                        continue;
                    }
                    
                    ++script_match_cnt;
                    ++script_ptr;
                }
                if( read_ptr>=end_read_ptr ){
                    break;
                }
                inside_script = false;
                script_match_cnt = 0;
                printf(
                    "\n[break script]\n"
                );
                continue;
            }
            else
            {
                com_ptr = comment_start + match_cnt;
                style_ptr = style_start + style_match_cnt;
                script_ptr = script_start + script_match_cnt;
                for( ; 
                    match_cnt<num_char_of_comment_start && 
                    style_match_cnt<num_char_of_style_start &&
                    script_match_cnt<num_char_of_script_start &&
                    read_ptr<end_read_ptr; 
                    ++read_ptr
                )
                {
                    if( inside_double_quotation )
                    {
                        /* find double quotation (end of string) */
                        for(; read_ptr<end_read_ptr; ++read_ptr)
                        {
                            *write_ptr = *read_ptr;
                            ++write_ptr;
                            printf("%c", *read_ptr);
                            if( *read_ptr=='\"' )
                            {
                                if( read_ptr!=read_buf)
                                {
                                    if( *(read_ptr-1)=='\\' )
                                    {
                                        continue;
                                    }
                                }
                                else if( last_char_in_buf=='\\' )
                                {
                                    continue;
                                }
                                break;
                            }
                        }
                        if( read_ptr>=end_read_ptr ){
                            break;
                        }
                        inside_double_quotation = false;
                        printf(
                            "\n[break double quotation]\n"
                            );
                        continue;
                    }
                    else if( inside_single_quotation )
                    {
                        /* find single quatation (end of string) */
                        for(; read_ptr<end_read_ptr; ++read_ptr)
                        {
                            *write_ptr = *read_ptr;
                            ++write_ptr;
                            printf("%c", *read_ptr);
                            if( *read_ptr=='\'' )
                            {
                                if( read_ptr!=read_buf)
                                {
                                    if( *(read_ptr-1)=='\\' )
                                    {
                                        continue;
                                    }
                                }
                                else if( last_char_in_buf=='\\' )
                                {
                                    continue;
                                }
                                break;
                            }
                        }
                        if( read_ptr>=end_read_ptr ){
                            break;
                        }
                        inside_single_quotation = false;
                        printf("\n[break single quotation]\n");
                        continue;
                    }
                    

                    if( *read_ptr==*com_ptr || *read_ptr == *style_ptr || *read_ptr == *script_ptr )
                    {
                        if( *read_ptr == *com_ptr )
                        {
                            ++com_ptr;
                            ++match_cnt;
                        }
                        else
                        {
                            match_cnt = 0;
                            com_ptr = comment_start;
                        }

                        if( *read_ptr == *style_ptr )
                        {
                            ++style_match_cnt;
                            ++style_ptr;
                        }
                        else
                        {
                            style_match_cnt = 0;
                            style_ptr = style_start;
                        }

                        if( *read_ptr == *script_ptr )
                        {
                            ++script_match_cnt;
                            ++script_ptr;
                        }
                        else
                        {
                            script_match_cnt = 0;
                            script_ptr = script_start;
                        }
                        ++share_match_cnt;

                        printf("'%c': comment=%u, style=%u, script=%u\n"
                            , *read_ptr, match_cnt, style_match_cnt, script_match_cnt
                        );

                    }
                    else
                    {
                        memcpy( write_ptr, read_ptr - share_match_cnt, share_match_cnt + 1 );
                        write_ptr += share_match_cnt + 1;

                        share_match_cnt = 0;
                        script_match_cnt = 0;
                        style_match_cnt = 0;
                        match_cnt = 0;

                        com_ptr = comment_start;
                        style_ptr = style_start;
                        script_ptr = script_start;

                        if( *read_ptr == '\"' )
                        {
                            printf(
                                "[into double quotation]\n"
                                "  Detected string: %c"
                                , *read_ptr
                            );
                            inside_double_quotation = true;
                        }
                        else if( *read_ptr == '\'' )
                        {
                            printf(
                                "[into single quotation]\n"
                                "  Detected string: %c"
                                , *read_ptr
                            );
                            inside_single_quotation = true;
                        }
                    }
                }
                if( read_ptr>=end_read_ptr ){
                    break;
                }

                if(match_cnt == num_char_of_comment_start )
                {
                    /* comment start */
                    in_comment = true;
                    printf(
                        "\n[into comment block]\n"
                        "  Detected comments: "
                    );
                }
                else if( style_match_cnt == num_char_of_style_start)
                {
                    inside_style = true;
                    printf(
                        "\n[into style block]\n"
                        "  Detected style: \n"
                        "%s", style_start
                    );
                    memcpy( write_ptr, style_start, num_char_of_style_start );
                    write_ptr += num_char_of_style_start;
                }
                else if( script_match_cnt == num_char_of_script_start)
                {
                    inside_script = true;
                    printf(
                        "\n[into script block]\n"
                        "  Detected script: \n"
                        "%s", script_start
                    );
                    memcpy( write_ptr, script_start, num_char_of_script_start );
                    write_ptr += num_char_of_script_start;
                }
                match_cnt = 0;
                style_match_cnt = 0;
                script_match_cnt = 0;
                share_match_cnt = 0;
            }
        }
        last_char_in_buf = *(read_ptr-1);
        write_size = (u32)(write_ptr-write_buf);
        printf(
            "\n\n[buffer end]\n"
            "  in_comment=%s, match_cnt=%u\n"
            "  inside_double_quotation=%s, inside_single_quotation=%s\n"
            "  last_char_in_buf=%c\n"
            "  write_size=%u\n\n"
            , in_comment? "true":"false", match_cnt
            , inside_double_quotation? "true":"false", inside_single_quotation? "true":"false"
            , last_char_in_buf
            , write_size
        );
        if( write_size>DEST_BUF_SIZE){
            WSP_COMMON_ERROR_LOG("write_size exceeds MAX_READ_SIZE: %u>=%u\n", write_size, DEST_BUF_SIZE);
            fclose(fp_src);
            fclose(fp_dest);
            return WSP_ResultStatusError();
        }
        res_size = fwrite( write_buf, 1, write_size, fp_dest); 
        if( write_size !=res_size)
        {
            WSP_COMMON_ERROR_LOG("fwrite failed: %u!=%u\n", write_size, res_size);
            fclose(fp_src);
            fclose(fp_dest);
            return WSP_ResultStatusError();
        }
        printf("buffer data was successfully wrote\n");
    }
    //*write_ptr = '\0';

    fclose( fp_src );
    fclose( fp_dest );

    return WSP_ResultSuccess();
}



WSP_Result WSP_RemoveSpecifiedComment( 
    const char *src_file_path, 
    const char *dest_file_path,
    const char *comment_start,
    const char *comment_end
)
{
    FILE *fp_src, *fp_dest;
    char read_buf[MAX_READ_SIZE], write_buf[MAX_READ_SIZE];

    size_t read_size;
    bool inside_comment = false;
    bool inside_double_quotation  = false; /* inside of "" */
    bool inside_single_quotation  = false; /* inside of '' */

    u32 num_char_of_comment_start = strlen( comment_start );
    u32 num_char_of_comment_end = strlen( comment_end );
    const char *com_ptr;
    bool in_comment = false;
    u32 match_cnt = 0;
    char last_char_in_buf = '\0';

    fp_src = fopen( src_file_path, "r" );
    if( fp_src==NULL ){
        WSP_COMMON_ERROR_LOG( "Could not open %s\n", src_file_path );
        return WSP_ResultStatusError();
    }
    fp_dest = fopen( dest_file_path, "w" );
    if( fp_dest==NULL ){
        WSP_COMMON_ERROR_LOG( "Could not open %s\n", dest_file_path );
        fclose( fp_src );
        return WSP_ResultStatusError();
    }

    //printf("num_com_start = %u, num_com_end = %u\n", num_char_of_comment_start, num_char_of_comment_end );

    char *write_ptr;
    while( (read_size=fread(read_buf, sizeof(char), MAX_READ_SIZE, fp_src)) != 0 )
    {
        char *read_ptr, *end_read_ptr;
        write_ptr = write_buf;
        read_ptr = read_buf;
        end_read_ptr  = read_ptr + read_size;
        while( read_ptr<end_read_ptr && *read_ptr!='\0' )
        {
            if( in_comment )
            {
                com_ptr = comment_end + match_cnt;
                for( ; 
                    match_cnt<num_char_of_comment_end && read_ptr<end_read_ptr; 
                    ++read_ptr
                )
                {
                    printf("%c", *read_ptr);
                    //WSP_COMMON_DEBUG_LOG("%c==%c\n", *read_ptr, *com_ptr);
                    if( *read_ptr != *com_ptr )
                    {
                        match_cnt = 0;
                        com_ptr = comment_end;
                        continue;
                    }
                    ++match_cnt;
                    ++com_ptr;
                }

                if( read_ptr>=end_read_ptr ){
                    break;
                }
                if( *comment_end=='\n'){
                    /* keep line feed */
                    *write_ptr = '\n';
                    ++write_ptr;
                }
                /* comment end */
                in_comment = false;
                match_cnt = 0;
                WSP_COMMON_DEBUG_LOG(
                    "\n[break comment block]\n"
                );
            }
            else
            {
                com_ptr = comment_start + match_cnt;
                for( ; 
                    match_cnt<num_char_of_comment_start && read_ptr<end_read_ptr; 
                    ++read_ptr
                )
                {
                    if( inside_double_quotation )
                    {
                        /* find double quotation (end of string) */
                        for(; read_ptr<end_read_ptr; ++read_ptr)
                        {
                            *write_ptr = *read_ptr;
                            ++write_ptr;
                            WSP_COMMON_DEBUG_LOG("%c", *read_ptr);
                            if( *read_ptr=='\"' )
                            {
                                if( read_ptr!=read_buf)
                                {
                                    if( *(read_ptr-1)=='\\' )
                                    {
                                        continue;
                                    }
                                }
                                else if( last_char_in_buf=='\\' )
                                {
                                    continue;
                                }
                                break;
                            }
                        }
                        if( read_ptr>=end_read_ptr ){
                            break;
                        }
                        inside_double_quotation = false;
                        WSP_COMMON_DEBUG_LOG(
                            "\n[break double quotation]\n"
                            );
                        continue;
                    }
                    else if( inside_single_quotation )
                    {
                        /* find single quatation (end of string) */
                        for(; read_ptr<end_read_ptr; ++read_ptr)
                        {
                            *write_ptr = *read_ptr;
                            ++write_ptr;
                            WSP_COMMON_DEBUG_LOG("%c", *read_ptr);
                            if( *read_ptr=='\'' )
                            {
                                if( read_ptr!=read_buf)
                                {
                                    if( *(read_ptr-1)=='\\' )
                                    {
                                        continue;
                                    }
                                }
                                else if( last_char_in_buf=='\\' )
                                {
                                    continue;
                                }
                                break;
                            }
                        }
                        if( read_ptr>=end_read_ptr ){
                            break;
                        }
                        inside_single_quotation = false;
                        WSP_COMMON_DEBUG_LOG("\n[break single quotation]\n");
                        continue;
                    }
                    else if( *read_ptr != *com_ptr )
                    {
                        memcpy( write_ptr, read_ptr - match_cnt, match_cnt + 1 );
                        write_ptr += match_cnt + 1;
                        match_cnt = 0;
                        com_ptr = comment_start;
                        if( *read_ptr == '\"' )
                        {
                            WSP_COMMON_DEBUG_LOG(
                                "[into double quotation]\n"
                                "  Detected string: %c"
                                , *read_ptr
                            );
                            inside_double_quotation = true;
                        }
                        else if( *read_ptr == '\'' )
                        {
                            WSP_COMMON_DEBUG_LOG(
                                "[into single quotation]\n"
                                "  Detected string: %c"
                                , *read_ptr
                            );
                            inside_single_quotation = true;
                        }
                        continue;
                    }
                    else
                    {
                        ++com_ptr;
                        ++match_cnt;
                    }
                }
                if( read_ptr>=end_read_ptr ){
                    break;
                }
                /* comment start */
                in_comment = true;
                match_cnt = 0;
                WSP_COMMON_DEBUG_LOG(
                    "\n[into comment block]\n"
                    "  Detected comments: "
                );
            }
        }
        last_char_in_buf = *(read_ptr-1);
        WSP_COMMON_DEBUG_LOG(
            "\n\n[buffer end]\n"
            "  in_comment=%s, match_cnt=%u\n"
            "  inside_double_quotation=%s, inside_single_quotation=%s\n"
            "  last_char_in_buf=%c\n\n"
            , in_comment? "true":"false", match_cnt
            , inside_double_quotation? "true":"false", inside_single_quotation? "true":"false"
            , last_char_in_buf
        );
        fwrite( write_buf, 1, (u32)(write_ptr-write_buf), fp_dest); 
    }
    *write_ptr = '\0';

    fclose( fp_src );
    fclose( fp_dest );

    return WSP_ResultSuccess();
}

WSP_Result WSP_RemoveHtmlComment(
    const char *src_file_path, 
    const char *dest_file_path
)
{
    const char *comment_start = "<!--";
    const char *comment_end   = "-->";
    return WSP_RemoveSpecifiedComment( src_file_path, dest_file_path, comment_start, comment_end );
}


WSP_Result WSP_RemoveCppComment( const char *src_file_path, const char *dest_file_path )
{
    WSP_Result result;
    int rm_res;
    const char *c_comment_start   = "/*";
    const char *c_comment_end     = "*/";
    const char *cpp_comment_start = "//";
    const char *cpp_comment_end   = "\n";
    const char *tmp_file = "remove_cpp_comment.tmp.txt";

    result = WSP_RemoveSpecifiedComment( src_file_path, tmp_file, c_comment_start, c_comment_end );
    if( WSP_ResultEqual(result, WSP_ResultSuccess())==false ){
        WSP_COMMON_ERROR_LOG("Removing /* */ comment failed\n");
        return result;
    }

    result = WSP_RemoveSpecifiedComment( tmp_file, dest_file_path, cpp_comment_start, cpp_comment_end );
    if( WSP_ResultEqual(result, WSP_ResultSuccess())==false ){
        WSP_COMMON_ERROR_LOG("Removing // comment failed\n");
        return result;
    }

    rm_res = remove( tmp_file );
    if( rm_res != 0 ){
        WSP_COMMON_ERROR_LOG( "removing %s failed\n", tmp_file );
        return WSP_ResultStatusError();
    }

    return WSP_ResultSuccess();
}



WSP_Result WSP_RemoveContinuousSpaces( const char *src_file_path, const char *dest_file_path )
{
    FILE *fp_src, *fp_dest;
    char read_buf[MAX_READ_SIZE], write_buf[MAX_READ_SIZE];

    size_t read_size;

    fp_src = fopen( src_file_path, "r" );
    if( fp_src==NULL ){
        WSP_COMMON_ERROR_LOG( "Could not open %s\n", src_file_path );
        return WSP_ResultStatusError();
    }
    fp_dest = fopen( dest_file_path, "w" );
    if( fp_dest==NULL ){
        WSP_COMMON_ERROR_LOG( "Could not open %s\n", dest_file_path );
        fclose( fp_src );
        return WSP_ResultStatusError();
    }

    while( (read_size=fread(read_buf, sizeof(char), MAX_READ_SIZE-1, fp_src)) != 0 )
    {
        char *write_ptr, *read_ptr, *end_ptr;
        write_ptr = write_buf;
        read_ptr = read_buf;
        end_ptr  = read_ptr + read_size;
        while( read_ptr<end_ptr && *read_ptr!='\0' )
        {
            if( *read_ptr==' ' )
            {
                ++read_ptr;
                while( *read_ptr==' '
                    && read_ptr<end_ptr 
                    && *read_ptr!='\0'
                )
                {
                    ++read_ptr; 
                }
                *write_ptr = ' ';
                ++write_ptr;
                continue;
            }
            *write_ptr = *read_ptr;
            ++read_ptr;
            ++write_ptr;
        }
        *write_ptr = '\0';
        fwrite( write_buf, 1, (u32)(write_ptr-write_buf), fp_dest); 
        WSP_COMMON_DEBUG_LOG("%s", write_buf);
    }

    fclose( fp_src );
    fclose( fp_dest );
    return WSP_ResultSuccess();
}

WSP_Result WSP_RemoveSpecifiedChar( const char *src_file_path, const char *dest_file_path, char rm_char )
{
    FILE *fp_src, *fp_dest;
    char read_buf[MAX_READ_SIZE], write_buf[MAX_READ_SIZE];

    size_t read_size;

    fp_src = fopen( src_file_path, "r" );
    if( fp_src==NULL ){
        WSP_COMMON_ERROR_LOG( "Could not open %s\n", src_file_path );
        return WSP_ResultStatusError();
    }
    fp_dest = fopen( dest_file_path, "w" );
    if( fp_dest==NULL ){
        WSP_COMMON_ERROR_LOG( "Could not open %s\n", dest_file_path );
        fclose( fp_src );
        return WSP_ResultStatusError();
    }

    while( (read_size=fread(read_buf, sizeof(char), MAX_READ_SIZE-1, fp_src)) != 0 )
    {
        char *write_ptr, *read_ptr, *end_ptr;
        write_ptr = write_buf;
        read_ptr = read_buf;
        end_ptr  = read_ptr + read_size;
        while( read_ptr<end_ptr && *read_ptr!='\0' )
        {
            if( *read_ptr==rm_char )
            {
                ++read_ptr; 
                continue;
            }
            *write_ptr = *read_ptr;
            ++read_ptr;
            ++write_ptr;
        }
        *write_ptr = '\0';
        fwrite( write_buf, 1, (u32)(write_ptr-write_buf), fp_dest); 
        WSP_COMMON_DEBUG_LOG("%s", write_buf);
    }

    fclose( fp_src );
    fclose( fp_dest );

    return WSP_ResultSuccess();
}

WSP_Result WSP_RemoveLineFeed( const char *src_file_path, const char *dest_file_path )
{
    return WSP_RemoveSpecifiedChar( src_file_path, dest_file_path, '\n' );
}

WSP_Result WSP_RemoveCarriageReturn( const char *src_file_path, const char *dest_file_path )
{
    return WSP_RemoveSpecifiedChar( src_file_path, dest_file_path, '\r' );
}
