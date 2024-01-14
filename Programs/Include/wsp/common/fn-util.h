/**
 * @file fn-util.h
 * 
 */

#ifndef __WSP_COMMONUTIL_FN_UTIL_H__
#define __WSP_COMMONUTIL_FN_UTIL_H__

#include "common_types.h"
#include "_define_commoncore.h"

#ifndef __WSP_COMMONUTIL_FUNCTION_STRING_H__
#include "fn-string.h"
#endif

#include <sys/timeb.h>
#include <time.h>
#include <iostream>

struct WSP_SeqInfo
{
    int  start_frame, end_frame;
    int  num_padding;
    char fullpath[MAX_LENGTH_OF_PATH];
    char dir[MAX_LENGTH_OF_PATH];
    char name[MAX_LENGTH_OF_NAME];
    char ext[MAX_LENGTH_OF_NAME];

    WSP_SeqInfo()
        : start_frame(0)
        , end_frame(0)
        , num_padding(0)
    {
    }
    WSP_SeqInfo(const WSP_SeqInfo& rhs)
    {
        *this = rhs;
    }
    inline WSP_SeqInfo& operator=( const WSP_SeqInfo &rhs )
    {
        memcpy( this, &rhs, sizeof( WSP_SeqInfo ) );
        return *this;
    }
    inline bool operator==( const WSP_SeqInfo &rhs ){ return memcmp( this, &rhs, sizeof( WSP_SeqInfo ) )==0; }
    inline bool operator!=( const WSP_SeqInfo &rhs ){ return !( *this == rhs ); }

};

namespace wsp{
    typedef struct WSP_SeqInfo SeqInfo;

    WSP_DLL_EXPORT wsp::State GetSeqInfo(wsp::SeqInfo &o_info, const char *in_filename);

    // File name operations ------------------------------------------

    WSP_DLL_EXPORT wsp::State GetDir              ( char *o_dir, const char *in_filepath, const char *in_separator="/");
    WSP_DLL_EXPORT wsp::State GetFilename         ( char *o_filename, const char *in_filepath);
    WSP_DLL_EXPORT wsp::State GetName             ( char *o_name, const char *in_filename);
    WSP_DLL_EXPORT wsp::State GetNumber           ( char *o_num, const char *in_filename);
    inline wsp::State GetExtension        ( char *o_ext, const char *in_filename);
    WSP_DLL_EXPORT int        GetNumber           ( const char *in_filename);
    inline         void       GetSeqName          ( char *o_dest, const SeqInfo &info, int frame=-1);
    inline         void       GetSeqPath          ( char *o_dest, const SeqInfo &info, int frame=-1);
    WSP_DLL_EXPORT wsp::State RemoveExtension     ( char *o_result, const char *in_filename);
    WSP_DLL_EXPORT void       AddToStringNum      ( char *io_num, int add);
    WSP_DLL_EXPORT void       ReplaceAll          ( char* io_str, char in_oldc, char in_newc);
    WSP_DLL_EXPORT void       GetLeftRightViewPath( const char *in_view_path, char *o_another_view_path );

    WSP_DLL_EXPORT void       CopyArgs( const char *in_argv[], char *o_argv[], int argc );
    WSP_DLL_EXPORT char**     AllocArgv( int argc );
    WSP_DLL_EXPORT void       FreeArgv( char **o_argv, int argc );


    // Time operations ----------------------------------------------
    #ifdef _WIN32
        static struct _timeb g_tstruct;    /* time */
    #else
        static struct timeb g_tstruct; /* time */
    #endif
    inline void StartTimer(bool show_message=false);
    //! return time as second
    inline float StopTimer(bool show_message=false);
    
    // Other --------------------------------------------------------
    WSP_DLL_EXPORT wsp::State GetMacAddress(uchar *o_addr);
    WSP_DLL_EXPORT wsp::State MakeDir(const char *destination);
    WSP_DLL_EXPORT wsp::State MakeOneDir(const char* dir);
    WSP_DLL_EXPORT void PrintPathEnvVar();

    WSP_DLL_EXPORT void GetCurrentDir(char* o_current_dir, size_t buffer_size);
}

WSP_DLL_EXPORT std::ostream& operator<<(std::ostream& lhs, const wsp::SeqInfo &rhs);
WSP_DLL_EXPORT std::ostream& operator<<(std::ostream& lhs, const wsp::SeqInfo* rhs);

//! Filename operations --------------------------------------------
inline wsp::State wsp::GetExtension(char *o_ext, const char *in_filename)
{
    char tokens[MAXSIZE_TOKEN];
    int num = wsp::Tokenize(tokens, in_filename, ".");
    if(o_ext==NULL){ return WSP_STATE_FAILURE;}
    strcpy(o_ext, &(tokens[wsp::GetTokenCharIndex(num-1)]));
    return WSP_STATE_SUCCESS;
}

inline void wsp::GetSeqName(char *o_dest, const wsp::SeqInfo &in_info, int frame)
{
    char num[MAX_LENGTH_OF_NAME];
    if(frame<0){
        frame = in_info.start_frame;
    }
    wsp::GetNumAsString(num, frame, in_info.num_padding);
    sprintf(o_dest,"%s%s.%s", in_info.name, num, in_info.ext);
}

inline void wsp::GetSeqPath(char *o_dest, const wsp::SeqInfo &in_info, int frame)
{
    char num[MAX_LENGTH_OF_NAME];
    if(frame<0){
        frame = in_info.start_frame;
    }
    wsp::GetNumAsString(num, frame, in_info.num_padding);
    sprintf(o_dest,"%s/%s%s.%s", in_info.dir, in_info.name, num, in_info.ext);
}

//! Time operations -----------------------------------------------
#ifdef _WIN32
inline void wsp::StartTimer(bool show_message)
{
    if(show_message){ printf("Timer is started"); }
    _ftime(&wsp::g_tstruct);
}

inline float wsp::StopTimer(bool show_message)
{
    struct _timeb tstruct_new;
    _ftime(&tstruct_new);

    float millisec;
    float second;
    millisec = 1000.0f * (float)(tstruct_new.time - wsp::g_tstruct.time) + (tstruct_new.millitm - wsp::g_tstruct.millitm);
    second = millisec/1000.0f;
    if(show_message){ printf("Timer is stopped: %.3f sec", (float)second); }
    return second;
}
#else
inline void wsp::StartTimer(bool show_message)
{
    ftime(&wsp::g_tstruct);
}

inline float wsp::StopTimer(bool show_message)
{
    struct timeb tstruct_new;
    time_t t;

    ftime(&tstruct_new);
    t = 1000*(tstruct_new.time - wsp::g_tstruct.time) + (tstruct_new.millitm - wsp::g_tstruct.millitm);
    float second = (float)t/1000.0;
    printf("%.3f sec\n\n", second);
    return second;
}
#endif

#endif
