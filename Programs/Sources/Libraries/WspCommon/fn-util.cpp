
#include "fn-debug.h"

#include <cassert>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <ctype.h>
//#include <io.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#ifndef __WSP_COMMONUTIL_FN_UTIL_H__
#include "fn-util.h"
#endif

#ifdef _WIN32

#include <Windows.h>
#include <IPHlpApi.h>
#pragma comment(lib, "iphlpapi.lib")

#include <direct.h>
#include <rpc.h>
#include <rpcdce.h>
#pragma comment(lib, "rpcrt4.lib")

#elif defined __linux__

#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/if.h>

#elif defined __APPLE__

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <net/if_dl.h>
#include <ifaddrs.h>
#define IFT_ETHER 0x6

#endif


using namespace std;

#define ARGV_ALLOC_MEM 128


void wsp::CopyArgs( const char *in_argv[], char *o_argv[], int argc )
{
    const char **src_ptr = in_argv;
    char **dst_ptr = o_argv;
    char **end_ptr = dst_ptr + argc;
    for( ; dst_ptr!=end_ptr; ++dst_ptr )
    {
        strcpy( *dst_ptr, *src_ptr );
    }
}
char** wsp::AllocArgv( int argc )
{
    char **argv = (char**)malloc( argc * sizeof(char**) );

    char **ptr = argv;
    char **end_ptr = ptr + argc;
    for( ; ptr != end_ptr; ++ptr )
    {
        *ptr = (char*)malloc( ARGV_ALLOC_MEM * sizeof(char*) );
    }
    return argv;
}

void wsp::FreeArgv( char **io_argv, int argc )
{
    char **ptr = io_argv;
    char **end_ptr = ptr + argc;
    for( ; ptr != end_ptr; ++ptr )
    {
        free(*ptr);
    }
    free(io_argv);
}

#ifdef _WIN32
bool GetMacAddressList(vector<string> *o_macAddrListHex, vector<string> *o_macAddrListStr)
{
    // Get the buffer length required for IP_ADAPTER_INFO.
    ULONG BufferLength = 0;
    BYTE* pBuffer = 0;
    if( ERROR_BUFFER_OVERFLOW == GetAdaptersInfo( 0, &BufferLength )){
        // Now the BufferLength contain the required buffer length.
        // Allocate necessary buffer.
        pBuffer = WSP_NEW BYTE[ BufferLength ];
    }else{
        // Error occurred. handle it accordingly.
        return false;
    }

    if(o_macAddrListHex!=NULL){ o_macAddrListHex->clear(); }
    if(o_macAddrListStr!=NULL){ o_macAddrListStr->clear(); }
    // Get the Adapter Information.
    PIP_ADAPTER_INFO pAdapterInfo = reinterpret_cast<PIP_ADAPTER_INFO>(pBuffer);
    GetAdaptersInfo( pAdapterInfo, &BufferLength );

    // Iterate the network adapters and print their MAC address.
    while( pAdapterInfo )
    {
        string macAddrHex, macAddrStr;
        macAddrHex.resize(6);
        macAddrStr.resize(12);
        // Assuming pAdapterInfo->AddressLength is 6.
        sprintf(&macAddrStr[0], "%02X%02X%02X%02X%02X%02X"
                ,pAdapterInfo->Address[0], pAdapterInfo->Address[1]
                ,pAdapterInfo->Address[2], pAdapterInfo->Address[3]
                ,pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
        cout<<"Adapter Name :"<<pAdapterInfo->AdapterName<<" ";
        cout<<"MAC :"<<macAddrStr<<endl;
        for(int i=0; i<6; i++){
            macAddrHex[i]=pAdapterInfo->Address[i];
        }
        if(o_macAddrListHex!=NULL){ o_macAddrListHex->push_back(macAddrHex); }
        if(o_macAddrListStr!=NULL){ o_macAddrListStr->push_back(macAddrStr); }

        // Get next adapter info.
        pAdapterInfo = pAdapterInfo->Next;
    }

    // deallocate the buffer.
    delete[] pBuffer;

    return true;
}

bool GetMacAddress_UUID(uchar *o_addr){
    UUID uuid;
    UuidCreateSequential( &uuid ); //! Ask OS to create UUID

    for (int i=2; i<8; i++){
        //! Bytes 2 through 7 inclusive are MAC address
        o_addr[i - 2] = uuid.Data4[i];
    }
    return true;
}
#elif defined __APPLE__
void GetMacAddress_Apple(char* macAddress, const char* ifName) {
    
    int  success;
    struct ifaddrs * addrs;
    struct ifaddrs * cursor;
    const struct sockaddr_dl * dlAddr;
    const unsigned char* base;
    int i;
    
    success = getifaddrs(&addrs) == 0;
    if (success) {
        cursor = addrs;
        while (cursor != 0) {
            if ( (cursor->ifa_addr->sa_family == AF_LINK)
                && (((const struct sockaddr_dl *) cursor->ifa_addr)->sdl_type == IFT_ETHER) && strcmp(ifName,  cursor->ifa_name)==0 ) {
                dlAddr = (const struct sockaddr_dl *) cursor->ifa_addr;
                base = (const unsigned char*) &dlAddr->sdl_data[dlAddr->sdl_nlen];
                strcpy(macAddress, "");
                for (i = 0; i < dlAddr->sdl_alen; i++) {
                    if (i != 0) {
                        strcat(macAddress, ":");
                    }
                    char partialAddr[3];
                    sprintf(partialAddr, "%02X", base[i]);
                    strcat(macAddress, partialAddr);
                    
                }
            }
            cursor = cursor->ifa_next;
        }
        
        freeifaddrs(addrs);
    }    
}
#endif


wsp::State wsp::GetMacAddress(uchar *o_addr)
{
#ifdef _WIN32
    bool stat;
    //return GetMacAddress_UUID(o_addr)? WSP_STATE_SUCCESS : WSP_STATE_FAILURE;
    // -------------------------------------------------------------------
    vector<string> macAddressList;
    stat=GetMacAddressList(&macAddressList, NULL);
    if(stat==false){
        return WSP_STATE_FAILURE;
    }
    for (int i=0; i<6; i++){
        o_addr[i] = macAddressList[0][i];
    }
    // -------------------------------------------------------------------

    return WSP_STATE_SUCCESS;
#elif defined __linux__
    struct ifreq ifr;
    struct ifreq *IFR;
    struct ifconf ifc;
    char buf[1024];
    int s, i;
    int ok = 0;

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s==-1) { return -1; }

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    ioctl(s, SIOCGIFCONF, &ifc);

    IFR = ifc.ifc_req;
    for (i = ifc.ifc_len / sizeof(struct ifreq); --i >= 0; IFR++) {
        strcpy(ifr.ifr_name, IFR->ifr_name);
        if (ioctl(s, SIOCGIFFLAGS, &ifr) == 0) {
            if (! (ifr.ifr_flags & IFF_LOOPBACK)) {
                if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0) {
                    ok = 1;
                    break;
                }
            }
        }
    }

    close(s);
    if (ok) {
        bCopy( ifr.ifr_hwaddr.sa_data, o_addr, 6);
        return WSP_STATE_SUCCESS;
    }
#elif defined __APPLE__
    char* macAddressString= (char*)malloc(18);
    GetMacAddress_Apple(macAddressString, "en0");
    
    WSP_FATAL("not implemented");
    
    free(macAddressString);
#endif

    return WSP_STATE_FAILURE;
}

void wsp::PrintPathEnvVar()
{
    const char *env_path_list = getenv( "PATH" );
    const char *env_ptr = env_path_list;
    char path[MAX_LENGTH_OF_PATH];
    char *path_ptr = path;

    WSP_COMMON_DEBUG_LOG( "PATH:\n"  );
    for( ; *env_ptr!='\0'; ++env_ptr)
    {
        if( *env_ptr==';' )
        {
            *path_ptr = '\0';
            printf("%s\n", path);
            path_ptr = path;
            continue;
        }
        *path_ptr = *env_ptr;
        ++path_ptr;
    }
}




wsp::State wsp::GetSeqInfo(SeqInfo &o_info, const char *in_filepath)
{
    int end;
    struct stat st;
    char filename[MAX_LENGTH_OF_PATH], numc[MAX_LENGTH_OF_NAME], numc_first[MAX_LENGTH_OF_NAME];

    if(strlen(in_filepath)>MAX_LENGTH_OF_PATH)
    {
        return WSP_STATE_FAILURE;
    }

    strcpy(o_info.fullpath, in_filepath);
    wsp::GetDir(o_info.dir, in_filepath);
    wsp::GetFilename(filename, in_filepath);
    wsp::GetName(o_info.name, filename);
    wsp::GetExtension(o_info.ext, filename);
    wsp::GetNumber(numc_first, filename);

    if(strlen(numc_first) == 0){
        o_info.start_frame=0;
        o_info.end_frame=0;
        o_info.num_padding=0;
        return WSP_STATE_SUCCESS;
    }

    o_info.start_frame = end = atoi(numc_first);

    strcpy(numc, numc_first);

    sprintf(filename,"%s/%s%s.%s", o_info.dir, o_info.name, numc, o_info.ext);
    while(stat(filename, &st)==0){
        wsp::AddToStringNum(numc,1);
        sprintf(filename,"%s/%s%s.%s", o_info.dir, o_info.name, numc, o_info.ext);
        ++end;
    }

    o_info.end_frame = end-1;
    o_info.num_padding = strlen(numc);

    // TODO: うまく動かず、面倒なので放置
    //if (o_info.start_frame == o_info.end_frame && strlen(numc_first) != 0)
    //{
    //    // 名前に数値があり、かつ、一枚しかない場合は数値を切り離さない
    //    char temp_filename[MAX_LENGTH_OF_NAME];
    //    char format[32];
    //    sprintf(format, "%%s%%0%dd", o_info.num_padding);
    //    sprintf(temp_filename, format, o_info.name, o_info.start_frame);
    //    strcpy(o_info.name, temp_filename);
    //}


    return WSP_STATE_SUCCESS;
}


wsp::State wsp::RemoveExtension(char *result,const char *filename){
    char tokens[MAXSIZE_TOKEN], tmp[MAXLEN_TOKEN];

    u32 size=wsp::Tokenize(tokens, filename, ".");
    if(size==0){ return WSP_STATE_FAILURE; }
    strcpy(result, &(tokens[0]));

    for(u32 itr_i=1; itr_i<size-1; itr_i++){
        strcpy(tmp, result);
        sprintf(result, "%s.%s", tmp, &tokens[wsp::GetTokenCharIndex(itr_i)]);
    }
    return WSP_STATE_SUCCESS;
}

int wsp::GetNumber(const char *in_filename){
    char num[MAXLEN_TOKEN];
    if( wsp::GetNumber(num, in_filename) == WSP_STATE_FAILURE){
        return -1;
    }
    return atoi(num);
}


wsp::State wsp::GetNumber(char *o_dest, const char *in_filename){
    char name[MAXLEN_TOKEN], num[MAXLEN_TOKEN];
    int i=0;

    wsp::RemoveExtension(name, in_filename);

    strcpy(o_dest, "");
    while(name[i]!='\0'){
        if(isdigit(name[i])){
            strcpy(num, o_dest);
            sprintf(o_dest, "%s%c", num, name[i]);
        }else{
            strcpy(o_dest, "");
        }
        i++;
    }
    if(strlen(o_dest)==0){ return WSP_STATE_FAILURE; }
    return WSP_STATE_SUCCESS;
}


wsp::State wsp::GetName(char *name, const char *filename)
{
    int filename_len=0;
    const char *end_ptr = filename + strlen(filename);

    // 拡張子まで戻る
    while(*end_ptr!='.' && end_ptr!=filename )
    {
        --end_ptr; 
        ++filename_len; 
    }

    if(end_ptr != filename)
    {
        // ファイル名先頭を辿る
        if(isdigit(*(end_ptr-1)))
        {
            --end_ptr;
            while(end_ptr!=filename && isdigit(*end_ptr))
            { 
                --end_ptr; 
            }
            if(end_ptr!=filename)
            {
                ++end_ptr;
            }
        }
    }
    else
    {
        strcpy(name , filename);
        return WSP_STATE_SUCCESS;
    }


    const char *top_ptr = end_ptr;
    while(top_ptr!=filename && *top_ptr!='/' && *top_ptr!='\\')
    { 
        --top_ptr; 
        ++filename_len; 
    }

    if( *top_ptr == '/' || *top_ptr == '\\' )
    {
        ++top_ptr;
    }

    WSP_UNUSED(filename_len);
    //if(strlen(name)<filename_len){ WSP_COMMON_ERROR_LOG("length of name is too small\n"); return WSP_STATE_FAILURE; }

    char *dst_ptr = name;
    const char *ptr = top_ptr;
    for(;ptr!=end_ptr; ++ptr, ++dst_ptr)
    {
        *dst_ptr = *ptr;
    }
    *dst_ptr='\0';

    return WSP_STATE_SUCCESS;
}


void wsp::AddToStringNum(char *result, int add){
    int size1,size2,itr_i,numi;
    char numc[MAXLEN_TOKEN], numc2[MAXLEN_TOKEN];

    size1=0;
    size2=0;


    strcpy(numc,result);

    while(numc[size1]!='\0'){ size1++; }

    numi=atoi(numc);
    numi += add;

    sprintf(numc2,"%d",numi);

    while(numc2[size2]!='\0'){ size2++; }

    //printf("size1=%d,size2=%d\n",size1,size2);

    for(itr_i=0; itr_i<size1-size2; itr_i++){
        numc[itr_i]='0';
    }
    for(;itr_i<size1; itr_i++){
        numc[itr_i]=numc2[itr_i-size1+size2];
    }

    strcpy(result,numc);
    return;
}


wsp::State wsp::GetFilename(char *o_filename, const char *in_filepath){
    int size;
    char tokens[MAXSIZE_TOKEN];
    char replaced_path[MAX_LENGTH_OF_PATH];

    strcpy( replaced_path, in_filepath );
    wsp::ReplaceAll( replaced_path, '\\', '/' );
    WSP_COMMON_DEBUG_LOG( "replaced_path = %s\n", replaced_path );
    size = wsp::Tokenize(tokens, in_filepath, "/" );

    if(size==0){
        fprintf(stderr, "Error in getDir: filepath was not given\n");
        return WSP_STATE_FAILURE;
    }
    strcpy(o_filename, &(tokens[wsp::GetTokenCharIndex(size-1)]));

    return WSP_STATE_SUCCESS;
}

wsp::State wsp::GetDir(char *o_dir, const char *filepath, const char *in_separator){
    u32 size;
    char dirpath[MAXLEN_TOKEN];
    char tokens[MAXSIZE_TOKEN];

    size = wsp::Tokenize(tokens, filepath, in_separator);
    if(size==0){
        fprintf(stderr, "Error in getDir: filepath was not given\n");
        return WSP_STATE_FAILURE;
    }

    strcpy(dirpath, ".");

    //! removing filename from filepath
    if(size > 1){
        char tmp[MAXLEN_TOKEN];
        strcpy(dirpath, &(tokens[0]));
        for(u32 itr_i=1; itr_i<size-1; itr_i++){
            strcpy(tmp, dirpath);
            sprintf(dirpath,"%s%s%s", tmp, in_separator, &(tokens[wsp::GetTokenCharIndex(itr_i)]));
        }
    }

    strcpy(o_dir, dirpath);

    return WSP_STATE_SUCCESS;
}

void wsp::GetLeftRightViewPath( const char *in_view_path, char *o_another_view_path )
{
    char dir[MAX_LENGTH_OF_PATH];
    char filename[MAX_LENGTH_OF_NAME];
    char name[MAX_LENGTH_OF_NAME];
    char ext[MAX_LENGTH_OF_NAME];

    wsp::GetDir( dir, in_view_path );
    wsp::GetFilename( filename, in_view_path );
    wsp::GetName( name, filename );
    wsp::GetExtension( ext, filename );
    WSP_COMMON_DEBUG_LOG(
        "path: %s\n"
        "@file %s\n"
        "name: %s\n"
        "ext: %s\n"
        , in_view_path!=NULL? in_view_path : "(NULL)"
        , filename
        , name
        , ext
    );

    const char *kLeftRightConvertList[] = {
        "l",    "r",
        "L",    "R",
        "left", "right",
        "Left", "Right",
        "LEFT", "RIGHT",
    };
    s32 list_size = sizeof(kLeftRightConvertList) / sizeof(char*);
    const char **list_ptr = kLeftRightConvertList;
    const char **end_list_ptr = list_ptr + list_size;
    for( ; list_ptr!=end_list_ptr; list_ptr+=2)
    {
        const char *src_str_ptr;
        const char *conv_str_ptr;
        src_str_ptr  = *(list_ptr);
        conv_str_ptr = *(list_ptr + 1);
        char *name_tail_str_ptr = (name+strlen(name)-strlen(src_str_ptr));
        WSP_COMMON_DEBUG_LOG("comparing \"%s\" and \"%s\"\n", name_tail_str_ptr, src_str_ptr);
        if( strcmp( name_tail_str_ptr, src_str_ptr )==0 )
        {
            assert( strlen(name)-strlen(src_str_ptr)+strlen(conv_str_ptr) < (MAX_LENGTH_OF_NAME-1) );
            strcpy( name_tail_str_ptr, conv_str_ptr );
            sprintf( o_another_view_path, "%s/%s.%s", dir, name, ext );
            return;
        }
        src_str_ptr  = *(list_ptr + 1);
        conv_str_ptr = *(list_ptr);
        name_tail_str_ptr = (name+strlen(name)-strlen(src_str_ptr));
        WSP_COMMON_DEBUG_LOG("comparing \"%s\" and \"%s\"\n", name_tail_str_ptr, src_str_ptr);
        if( strcmp( name_tail_str_ptr, src_str_ptr )==0 )
        {
            assert( strlen(name)-strlen(src_str_ptr)+strlen(conv_str_ptr) < (MAX_LENGTH_OF_NAME-1) );
            strcpy( name_tail_str_ptr, conv_str_ptr );
            sprintf( o_another_view_path, "%s/%s.%s", dir, name, ext );
            return;
        }
    }
}


void wsp::ReplaceAll(char* io_str, char in_oldc, char in_newc)
{
    int length = strlen(io_str);
    for(int itr_i=0; itr_i<length; itr_i++)
    {
        char *cp;
        cp = strstr(io_str, &in_oldc);
        if(cp != NULL){
            *cp = in_newc;
        }
    }
}

wsp::State wsp::MakeOneDir(const char* dir)
{
    struct stat st;
    if (stat(dir, &st) != 0){
#if defined UNIX
        if (mkdir(cpath, mode) != 0)
#elif defined WIN32
        if (_mkdir(dir) != 0)
#endif
        {
            WSP_COMMON_ERROR_LOG("couldn't make %s!", dir);
            return WSP_STATE_FAILURE;
        }
    }

    return WSP_STATE_SUCCESS;
}

wsp::State wsp::MakeDir(const char *destination)
{
    u32 itr_i, dir_num;
    char cpath[MAX_LENGTH_OF_PATH], tmpdir[MAX_LENGTH_OF_PATH], tokens[MAXSIZE_TOKEN];
    #ifdef UNIX
    mode_t mode = S_IRUSR | S_IRGRP | S_IXUSR | S_IXGRP | S_IWUSR | S_IWGRP;
    #endif

    dir_num=wsp::Tokenize(tokens, destination, SPRTR);
    if (dir_num == 0)
    {
        return MakeOneDir(cpath);
    }

    strcpy(cpath, &(tokens[0]));
    // making all directories step by step
    for (itr_i = 1; itr_i < dir_num; itr_i++){
        strcpy(tmpdir, cpath);
        //printf("tmpdir=%s,tokens=%s\n",tmpdir,tokens[itr_i]);
        sprintf(cpath, "%s%s%s", tmpdir, SPRTR, &(tokens[wsp::GetTokenCharIndex(itr_i)]));
        wsp::State state = MakeOneDir(cpath);
        if (state != WSP_STATE_SUCCESS)
        {
            return state;
        }
    }

    return WSP_STATE_SUCCESS;
}

void wsp::GetCurrentDir(char* o_current_dir, size_t buffer_size)
{
#ifdef WIN32
    DWORD write_size = GetCurrentDirectory(buffer_size, o_current_dir);
    WSP_ASSERT(write_size > 0, "get current directory failed");
#elif defined __APPLE__
    getcwd(o_current_dir, buffer_size);
#else
    WSP_FATAL_NOT_IMPLEMENTED;
#endif
}

// Operators -------------------------------------------------------
ostream& operator<<(ostream& lhs, const wsp::SeqInfo &rhs){
    cout<<"Sequence Informations -----------"<<endl;
    cout<<"full path: "<<rhs.fullpath<<endl;
    cout<<"directory: "<<rhs.dir<<endl;
    cout<<"name: "<<rhs.name<<endl;
    cout<<"extension: "<<rhs.ext<<endl;
    cout<<"frame padding: "<<rhs.num_padding<<endl;
    cout<<"start frame: "<<rhs.start_frame<<endl;
    cout<<"end frame: "<<rhs.end_frame<<endl;
    cout<<"---------------------------------"<<endl;
    return lhs;
}
ostream& operator<<(ostream& lhs, const wsp::SeqInfo* rhs){
    lhs<<*rhs;
    return lhs;
}

