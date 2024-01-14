/**
 * @file _fn-string.c
 * 
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "fn-string.h"
#include "fn-debug.h"

//! max number of token
#define MAXNUM_TOKEN (128) 
//! max size of token represented as character
#define MAXLEN_TOKEN (128) 
//! buffer size of token array
#define MAXSIZE_TOKEN (MAXNUM_TOKEN*MAXLEN_TOKEN) 

const char   _g_kDelimiterList[][MAX_LENGTH_OF_NAME] = {"/", "\\"};
const size_t _g_kDelimiterListSize = sizeof(_g_kDelimiterList)/MAX_LENGTH_OF_NAME;
const char   _g_kHexLower[16]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
const char   _g_kHexUpper[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};


void WSP_ReplaceSpecifiedChar( char *io_str, size_t length, char target_char, char subst_char )
{
    char *ptr = io_str;
    char *end_ptr = ptr + length;
    for( ; ptr!=end_ptr; ++ptr )
    {
        if( *ptr==target_char ){
            *ptr = subst_char;
        }
    }
}

void WSP_RemoveAlphabet(char *io_str)
{ 
    WSP_RemoveAnyTypedStrings(io_str, isalpha); 
}
void WSP_RemoveNumber(char *io_str)
{ 
    WSP_RemoveAnyTypedStrings(io_str, isdigit); 
}
void WSP_RemoveSpace(char *io_str)
{ 
    WSP_RemoveAnyTypedStrings(io_str, isspace); 
}

int  WSP_GetTokenCharIndex(int token_index);
const char* WSP_GetToken(const char *in_tokens, int index);


int WSP_GetTokenCharIndex(int token_index)
{ 
    return token_index*MAXLEN_TOKEN; 
}
const char* WSP_GetToken(const char *in_tokens, int index)
{ 
    return &in_tokens[WSP_GetTokenCharIndex(index)]; 
}

void WSP_RenameLeafNode(const char *in_str, char *out_str, void (*rename_func)(char*), const char *denom)
{
    char tokens[MAXSIZE_TOKEN], tmp_str[MAX_LENGTH_OF_PATH];
    int i;
    // Split name into local name
    int num_tokens = WSP_Tokenize(tokens, in_str, denom, MAXNUM_TOKEN);
    WSP_ASSERT(num_tokens != -1, "number of tokens exceeds maximum number");
    int end_index = num_tokens - 1;
    char *out_ptr;
    size_t denom_size;

    strcpy(tmp_str, &tokens[WSP_GetTokenCharIndex(end_index)]);
    rename_func(tmp_str);
    out_ptr = out_str;

    denom_size = strlen(denom);

    // Merge tokens to get global name
    for(i=0; i<end_index; ++i){
        strcpy(out_ptr, &tokens[WSP_GetTokenCharIndex(i)]);
        out_ptr+=strlen(&tokens[WSP_GetTokenCharIndex(i)]);
        strcpy(out_ptr, denom);
        out_ptr+=denom_size;
    }
    strcpy(out_ptr, tmp_str);
}

void _WSP_FindDelimiterFromPath( char *o_delimiter, const char *path )
{
    int i;
    char comp_target_buf[MAX_LENGTH_OF_NAME];
    for( i=0; i<_g_kDelimiterListSize; ++i )
    {
        const char* delimiter = _g_kDelimiterList[i];
        size_t delimiter_size = strlen( delimiter );
        const char *ptr = path;
        for( ; *ptr!='\0'; ++ptr)
        {
            strncpy( comp_target_buf, ptr, delimiter_size );
            comp_target_buf[delimiter_size] = '\0';
            if( strcmp( comp_target_buf, delimiter ) == 0 )
            {
                strcpy( o_delimiter, delimiter );
                return;
            }
        }
    }
}

void WSP_GetDirectoryPathFromFilePath(char *o_dst, const char *path)
{
    char tokens[MAXSIZE_TOKEN];
    char delimiter[MAX_LENGTH_OF_NAME];
    char *path_ptr;
    int num_tokens;
    int i, itr_end;
    size_t delim_size;

    _WSP_FindDelimiterFromPath(delimiter, path);
    num_tokens = WSP_Tokenize(tokens, path, delimiter, MAXNUM_TOKEN);
    WSP_ASSERT(num_tokens != -1, "number of tokens exceeds maximum number");

    delim_size = strlen(delimiter);
    itr_end = num_tokens - 1;
    path_ptr = o_dst;
    for (i = 0; i < itr_end; ++i, path_ptr += delim_size)
    {
        char* token = &tokens[WSP_GetTokenCharIndex(i)];
        strcpy(path_ptr, token);
        path_ptr += strlen(token);
        strcpy(path_ptr, delimiter);
    }

    // 末尾のデリミタを取り除く
    o_dst[strlen(o_dst) - 1] = '\0';
}

void WSP_GetFileNameFromFilePath(char *o_dst, const char *path)
{
    char tokens[MAXSIZE_TOKEN];
    char delimiter[MAX_LENGTH_OF_NAME];
    char *path_ptr;
    int num_tokens;
    int i, itr_end;
    size_t delim_size;

    _WSP_FindDelimiterFromPath(delimiter, path);
    num_tokens = WSP_Tokenize(tokens, path, delimiter, MAXNUM_TOKEN);
    WSP_ASSERT(num_tokens != -1, "number of tokens exceeds maximum number");

    delim_size = strlen(delimiter);
    itr_end = num_tokens - 1;
    path_ptr = o_dst;

    char* last_token = &tokens[WSP_GetTokenCharIndex(num_tokens - 1)];
    strcpy(o_dst, last_token);
}


/* Get name of leaf node */
void WSP_GetLeafNodeName(const char *in_global_name, char *o_local_name, const char *denom)
{
    char tokens[MAXSIZE_TOKEN];
    // Split name into local name
    int num_tokens = WSP_Tokenize(tokens, in_global_name, denom, MAXNUM_TOKEN);
    WSP_ASSERT(num_tokens != -1, "number of tokens exceeds maximum number");
    strcpy(o_local_name, &tokens[WSP_GetTokenCharIndex(num_tokens-1)]);
}
void WSP_RemoveNamespace(char *io_str, const char *denom)
{
    WSP_GetLeafNodeName(io_str, io_str, denom);
}
void WSP_RemoveMayaNamespace(char *io_str)
{ 
    WSP_RemoveNamespace(io_str, ":"); 
}


int WSP_StrLen(const char *in_src)
{
    int len=0;
    while(in_src[len]!='\0'){ len++; }
    return len;
}

void WSP_GetNumAsString(char *o_dest, int num, u32 padding)
{
    if(padding!=0){
        char format[MAX_LENGTH_OF_NAME];
        sprintf(format, "%%0%ud", padding);
        sprintf(o_dest, format, num);
        return;
    }
    strcpy(o_dest, "");
}


bool WSP_IsDigitString(const char *str)
{
    size_t len = strlen(str);
    const char *ptr = str;
    const char *end_ptr = ptr + len;

    if(len==0){ return false; }

    if(*ptr=='-'){ ++ptr; }

    for(; ptr!=end_ptr; ++ptr){
        if(!isdigit(*ptr)){ return false; }
    }
    return true;
}

void WSP_RemoveAnyTypedStrings(char *io_str, int(*isfunc)(int))
{
    char res_str[MAX_LENGTH_OF_PATH];
    char *src_ptr = io_str;
    char *res_ptr = res_str;
    while(*src_ptr!='\0'){
        if(!isfunc(*src_ptr)){
            *res_ptr = *src_ptr;
            ++res_ptr;
        }
        ++src_ptr;
    }
    *res_ptr = '\0';
    strcpy(io_str, res_str);
}

int WSP_Tokenize(char *tokens, const char *src, const char *div, int max_tokens)
{
    char srcbuf[MAXSIZE_TOKEN];
    int numOfToken=0;
    char *cp;
    if(strlen(src) > MAXSIZE_TOKEN + 1)
    {
        return 0;
    }

    strcpy( srcbuf, src );

    cp = (char*)strtok( srcbuf, div );
    while(cp != NULL)
    {
        strcpy(&(tokens[WSP_GetTokenCharIndex(numOfToken)]), cp);
        cp = (char*)strtok(NULL, div);
        ++numOfToken;
        if (numOfToken == max_tokens && cp != NULL)
        {
            // トークン数がマックスでまだトークンがあればエラー扱い
            return -1;
        }
    }

    tokens[WSP_GetTokenCharIndex(numOfToken)]='\0';

    return numOfToken;
}

int WSP_StrToHex(const char *src)
{
    int iter_j, itr_i=0, result=0;
    while(src[itr_i] != '\0')
    {
        char ctmp;
        ctmp = src[itr_i];
        if(isxdigit(ctmp)==0){ return 0;}

        for(iter_j=0; iter_j<16; iter_j++)
        {
            if(tolower(ctmp) == _g_kHexLower[iter_j]){
                result += iter_j<<(4*itr_i);
                break;
            }
        }
        ++itr_i;
    }
    return result;
}

int WSP_EnToEm(char *str)
{
    char *buf,*p,*ptr;

    buf=(char *)calloc(strlen(str)*2+1,sizeof(char));

    for(ptr=str,p=buf;*ptr!='\0';*ptr++){
        switch((int)*ptr){
            case    ' ': strcpy(p,"　");p+=2;break;
            case    '!': strcpy(p,"！");p+=2;break;
            case    '"': strcpy(p,"”");p+=2;break;
            case    '#': strcpy(p,"＃");p+=2;break;
            case    '$': strcpy(p,"＄");p+=2;break;
            case    '%': strcpy(p,"％");p+=2;break;
            case    '&': strcpy(p,"＆");p+=2;break;
            case    '\'': strcpy(p,"’");p+=2;break;
            case    '(': strcpy(p,"（");p+=2;break;
            case    ')': strcpy(p,"）");p+=2;break;
            case    '*': strcpy(p,"＊");p+=2;break;
            case    '+': strcpy(p,"＋");p+=2;break;
            case    ',': strcpy(p,"，");p+=2;break;
            case    '-': strcpy(p,"−");p+=2;break;
            case    '.': strcpy(p,"．");p+=2;break;
            case    '/': strcpy(p,"／");p+=2;break;
            case    '0': strcpy(p,"０");p+=2;break;
            case    '1': strcpy(p,"１");p+=2;break;
            case    '2': strcpy(p,"２");p+=2;break;
            case    '3': strcpy(p,"３");p+=2;break;
            case    '4': strcpy(p,"４");p+=2;break;
            case    '5': strcpy(p,"５");p+=2;break;
            case    '6': strcpy(p,"６");p+=2;break;
            case    '7': strcpy(p,"７");p+=2;break;
            case    '8': strcpy(p,"８");p+=2;break;
            case    '9': strcpy(p,"９");p+=2;break;
            case    ':': strcpy(p,"：");p+=2;break;
            case    ';': strcpy(p,"；");p+=2;break;
            case    '<': strcpy(p,"＜");p+=2;break;
            case    '=': strcpy(p,"＝");p+=2;break;
            case    '>': strcpy(p,"＞");p+=2;break;
            case    '?': strcpy(p,"？");p+=2;break;
            case    '@': strcpy(p,"＠");p+=2;break;
            case    'A': strcpy(p,"Ａ");p+=2;break;
            case    'B': strcpy(p,"Ｂ");p+=2;break;
            case    'C': strcpy(p,"Ｃ");p+=2;break;
            case    'D': strcpy(p,"Ｄ");p+=2;break;
            case    'E': strcpy(p,"Ｅ");p+=2;break;
            case    'F': strcpy(p,"Ｆ");p+=2;break;
            case    'G': strcpy(p,"Ｇ");p+=2;break;
            case    'H': strcpy(p,"Ｈ");p+=2;break;
            case    'I': strcpy(p,"Ｉ");p+=2;break;
            case    'J': strcpy(p,"Ｊ");p+=2;break;
            case    'K': strcpy(p,"Ｋ");p+=2;break;
            case    'L': strcpy(p,"Ｌ");p+=2;break;
            case    'M': strcpy(p,"Ｍ");p+=2;break;
            case    'N': strcpy(p,"Ｎ");p+=2;break;
            case    'O': strcpy(p,"Ｏ");p+=2;break;
            case    'P': strcpy(p,"Ｐ");p+=2;break;
            case    'Q': strcpy(p,"Ｑ");p+=2;break;
            case    'R': strcpy(p,"Ｒ");p+=2;break;
            case    'S': strcpy(p,"Ｓ");p+=2;break;
            case    'T': strcpy(p,"Ｔ");p+=2;break;
            case    'U': strcpy(p,"Ｕ");p+=2;break;
            case    'V': strcpy(p,"Ｖ");p+=2;break;
            case    'W': strcpy(p,"Ｗ");p+=2;break;
            case    'X': strcpy(p,"Ｘ");p+=2;break;
            case    'Y': strcpy(p,"Ｙ");p+=2;break;
            case    'Z': strcpy(p,"Ｚ");p+=2;break;
            case    '[': strcpy(p,"［");p+=2;break;
            case    '\\': strcpy(p,"￥");p+=2;break;
            case    ']': strcpy(p,"］");p+=2;break;
            case    '^': strcpy(p,"＾");p+=2;break;
            case    '_': strcpy(p,"＿");p+=2;break;
            case    '`': strcpy(p,"‘");p+=2;break;
            case    'a': strcpy(p,"ａ");p+=2;break;
            case    'b': strcpy(p,"ｂ");p+=2;break;
            case    'c': strcpy(p,"ｃ");p+=2;break;
            case    'd': strcpy(p,"ｄ");p+=2;break;
            case    'e': strcpy(p,"ｅ");p+=2;break;
            case    'f': strcpy(p,"ｆ");p+=2;break;
            case    'g': strcpy(p,"ｇ");p+=2;break;
            case    'h': strcpy(p,"ｈ");p+=2;break;
            case    'i': strcpy(p,"ｉ");p+=2;break;
            case    'j': strcpy(p,"ｊ");p+=2;break;
            case    'k': strcpy(p,"ｋ");p+=2;break;
            case    'l': strcpy(p,"ｌ");p+=2;break;
            case    'm': strcpy(p,"ｍ");p+=2;break;
            case    'n': strcpy(p,"ｎ");p+=2;break;
            case    'o': strcpy(p,"ｏ");p+=2;break;
            case    'p': strcpy(p,"ｐ");p+=2;break;
            case    'q': strcpy(p,"ｑ");p+=2;break;
            case    'r': strcpy(p,"ｒ");p+=2;break;
            case    's': strcpy(p,"ｓ");p+=2;break;
            case    't': strcpy(p,"ｔ");p+=2;break;
            case    'u': strcpy(p,"ｕ");p+=2;break;
            case    'v': strcpy(p,"ｖ");p+=2;break;
            case    'w': strcpy(p,"ｗ");p+=2;break;
            case    'x': strcpy(p,"ｘ");p+=2;break;
            case    'y': strcpy(p,"ｙ");p+=2;break;
            case    'z': strcpy(p,"ｚ");p+=2;break;
            case    '{': strcpy(p,"｛");p+=2;break;
            case    '|': strcpy(p,"｜");p+=2;break;
            case    '}': strcpy(p,"｝");p+=2;break;
            default:
                *p=*ptr;
                p++;
                *p='\0';
                break;
        }
    }
    strcpy(str,buf);
    free(buf);

    return(0);
}

int WSP_EmToEn(char *str)
{
    char *buf,*p,*ptr;

    buf=(char *)calloc(strlen(str)+1,sizeof(char));

    for(ptr=str,p=buf;*ptr!='\0';*ptr++){
        if(strncmp(ptr,"　",2)==0){*p=' ';p++;ptr++;}
        else if(strncmp(ptr,"！",2)==0){*p='!';p++;ptr++;}
        else if(strncmp(ptr,"”",2)==0){*p='"';p++;ptr++;}
        else if(strncmp(ptr,"＃",2)==0){*p='#';p++;ptr++;}
        else if(strncmp(ptr,"＄",2)==0){*p='$';p++;ptr++;}
        else if(strncmp(ptr,"％",2)==0){*p='%';p++;ptr++;}
        else if(strncmp(ptr,"＆",2)==0){*p='&';p++;ptr++;}
        else if(strncmp(ptr,"’",2)==0){*p='\'';p++;ptr++;}
        else if(strncmp(ptr,"（",2)==0){*p='(';p++;ptr++;}
        else if(strncmp(ptr,"）",2)==0){*p=')';p++;ptr++;}
        else if(strncmp(ptr,"＊",2)==0){*p='*';p++;ptr++;}
        else if(strncmp(ptr,"＋",2)==0){*p='+';p++;ptr++;}
        else if(strncmp(ptr,"，",2)==0){*p=',';p++;ptr++;}
        else if(strncmp(ptr,"−",2)==0){*p='-';p++;ptr++;}
        else if(strncmp(ptr,"．",2)==0){*p='.';p++;ptr++;}
        else if(strncmp(ptr,"／",2)==0){*p='/';p++;ptr++;}
        else if(strncmp(ptr,"０",2)==0){*p='0';p++;ptr++;}
        else if(strncmp(ptr,"１",2)==0){*p='1';p++;ptr++;}
        else if(strncmp(ptr,"２",2)==0){*p='2';p++;ptr++;}
        else if(strncmp(ptr,"３",2)==0){*p='3';p++;ptr++;}
        else if(strncmp(ptr,"４",2)==0){*p='4';p++;ptr++;}
        else if(strncmp(ptr,"５",2)==0){*p='5';p++;ptr++;}
        else if(strncmp(ptr,"６",2)==0){*p='6';p++;ptr++;}
        else if(strncmp(ptr,"７",2)==0){*p='7';p++;ptr++;}
        else if(strncmp(ptr,"８",2)==0){*p='8';p++;ptr++;}
        else if(strncmp(ptr,"９",2)==0){*p='9';p++;ptr++;}
        else if(strncmp(ptr,"：",2)==0){*p=':';p++;ptr++;}
        else if(strncmp(ptr,"；",2)==0){*p=';';p++;ptr++;}
        else if(strncmp(ptr,"＜",2)==0){*p='<';p++;ptr++;}
        else if(strncmp(ptr,"＝",2)==0){*p='=';p++;ptr++;}
        else if(strncmp(ptr,"＞",2)==0){*p='>';p++;ptr++;}
        else if(strncmp(ptr,"？",2)==0){*p='?';p++;ptr++;}
        else if(strncmp(ptr,"＠",2)==0){*p='@';p++;ptr++;}
        else if(strncmp(ptr,"Ａ",2)==0){*p='A';p++;ptr++;}
        else if(strncmp(ptr,"Ｂ",2)==0){*p='B';p++;ptr++;}
        else if(strncmp(ptr,"Ｃ",2)==0){*p='C';p++;ptr++;}
        else if(strncmp(ptr,"Ｄ",2)==0){*p='D';p++;ptr++;}
        else if(strncmp(ptr,"Ｅ",2)==0){*p='E';p++;ptr++;}
        else if(strncmp(ptr,"Ｆ",2)==0){*p='F';p++;ptr++;}
        else if(strncmp(ptr,"Ｇ",2)==0){*p='G';p++;ptr++;}
        else if(strncmp(ptr,"Ｈ",2)==0){*p='H';p++;ptr++;}
        else if(strncmp(ptr,"Ｉ",2)==0){*p='I';p++;ptr++;}
        else if(strncmp(ptr,"Ｊ",2)==0){*p='J';p++;ptr++;}
        else if(strncmp(ptr,"Ｋ",2)==0){*p='K';p++;ptr++;}
        else if(strncmp(ptr,"Ｌ",2)==0){*p='L';p++;ptr++;}
        else if(strncmp(ptr,"Ｍ",2)==0){*p='M';p++;ptr++;}
        else if(strncmp(ptr,"Ｎ",2)==0){*p='N';p++;ptr++;}
        else if(strncmp(ptr,"Ｏ",2)==0){*p='O';p++;ptr++;}
        else if(strncmp(ptr,"Ｐ",2)==0){*p='P';p++;ptr++;}
        else if(strncmp(ptr,"Ｑ",2)==0){*p='Q';p++;ptr++;}
        else if(strncmp(ptr,"Ｒ",2)==0){*p='R';p++;ptr++;}
        else if(strncmp(ptr,"Ｓ",2)==0){*p='S';p++;ptr++;}
        else if(strncmp(ptr,"Ｔ",2)==0){*p='T';p++;ptr++;}
        else if(strncmp(ptr,"Ｕ",2)==0){*p='U';p++;ptr++;}
        else if(strncmp(ptr,"Ｖ",2)==0){*p='V';p++;ptr++;}
        else if(strncmp(ptr,"Ｗ",2)==0){*p='W';p++;ptr++;}
        else if(strncmp(ptr,"Ｘ",2)==0){*p='X';p++;ptr++;}
        else if(strncmp(ptr,"Ｙ",2)==0){*p='Y';p++;ptr++;}
        else if(strncmp(ptr,"Ｚ",2)==0){*p='Z';p++;ptr++;}
        else if(strncmp(ptr,"［",2)==0){*p='[';p++;ptr++;}
        else if(strncmp(ptr,"￥",2)==0){*p='\\';p++;ptr++;}
        else if(strncmp(ptr,"］",2)==0){*p=']';p++;ptr++;}
        else if(strncmp(ptr,"＾",2)==0){*p='^';p++;ptr++;}
        else if(strncmp(ptr,"＿",2)==0){*p='_';p++;ptr++;}
        else if(strncmp(ptr,"‘",2)==0){*p='`';p++;ptr++;}
        else if(strncmp(ptr,"ａ",2)==0){*p='a';p++;ptr++;}
        else if(strncmp(ptr,"ｂ",2)==0){*p='b';p++;ptr++;}
        else if(strncmp(ptr,"ｃ",2)==0){*p='c';p++;ptr++;}
        else if(strncmp(ptr,"ｄ",2)==0){*p='d';p++;ptr++;}
        else if(strncmp(ptr,"ｅ",2)==0){*p='e';p++;ptr++;}
        else if(strncmp(ptr,"ｆ",2)==0){*p='f';p++;ptr++;}
        else if(strncmp(ptr,"ｇ",2)==0){*p='g';p++;ptr++;}
        else if(strncmp(ptr,"ｈ",2)==0){*p='h';p++;ptr++;}
        else if(strncmp(ptr,"ｉ",2)==0){*p='i';p++;ptr++;}
        else if(strncmp(ptr,"ｊ",2)==0){*p='j';p++;ptr++;}
        else if(strncmp(ptr,"ｋ",2)==0){*p='k';p++;ptr++;}
        else if(strncmp(ptr,"ｌ",2)==0){*p='l';p++;ptr++;}
        else if(strncmp(ptr,"ｍ",2)==0){*p='m';p++;ptr++;}
        else if(strncmp(ptr,"ｎ",2)==0){*p='n';p++;ptr++;}
        else if(strncmp(ptr,"ｏ",2)==0){*p='o';p++;ptr++;}
        else if(strncmp(ptr,"ｐ",2)==0){*p='p';p++;ptr++;}
        else if(strncmp(ptr,"ｑ",2)==0){*p='q';p++;ptr++;}
        else if(strncmp(ptr,"ｒ",2)==0){*p='r';p++;ptr++;}
        else if(strncmp(ptr,"ｓ",2)==0){*p='s';p++;ptr++;}
        else if(strncmp(ptr,"ｔ",2)==0){*p='t';p++;ptr++;}
        else if(strncmp(ptr,"ｕ",2)==0){*p='u';p++;ptr++;}
        else if(strncmp(ptr,"ｖ",2)==0){*p='v';p++;ptr++;}
        else if(strncmp(ptr,"ｗ",2)==0){*p='w';p++;ptr++;}
        else if(strncmp(ptr,"ｘ",2)==0){*p='x';p++;ptr++;}
        else if(strncmp(ptr,"ｙ",2)==0){*p='y';p++;ptr++;}
        else if(strncmp(ptr,"ｚ",2)==0){*p='z';p++;ptr++;}
        else if(strncmp(ptr,"｛",2)==0){*p='{';p++;ptr++;}
        else if(strncmp(ptr,"｜",2)==0){*p='|';p++;ptr++;}
        else if(strncmp(ptr,"｝",2)==0){*p='}';p++;ptr++;}
        else{ *p=*ptr; p++; }
    }
    strcpy(str,buf);
    free(buf);

    return(0);
}