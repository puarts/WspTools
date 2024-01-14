/**
 * @file fn-mediautil.h
 * @author Jun-ichi Nishikata
 * @brief Utility functions for multimedia module.
 */

#ifndef _WSP_MULTIMEDIA_FN_MEDIAUTIL_H_
#define _WSP_MULTIMEDIA_FN_MEDIAUTIL_H_

#include <string>
#include <map>

#include "_define_multimedia.h"

#ifndef __WSP_COMMONUTIL_FN_UTIL_H__
#include <wsp/common/fn-util.h>
#endif

namespace wsp{ namespace mm{

    /**
     * @brief Get wsp::mm::MediaType from string
     */
    inline wsp::mm::MediaType GetMediaTypeFromString( const char *in_type_str )
    {
        std::map<std::string, wsp::mm::MediaType> relation_map;
        relation_map["composition"]              = wsp::mm::MEDIATYPE_COMPOSITION;
        relation_map["image"]                    = wsp::mm::MEDIATYPE_IMAGE;
        relation_map["image sequence"]           = wsp::mm::MEDIATYPE_IMAGE_SEQUENCE;
        relation_map["sound"]                    = wsp::mm::MEDIATYPE_SOUND;
        relation_map["video"]                    = wsp::mm::MEDIATYPE_VIDEO;
        relation_map["midi"]                     = wsp::mm::MEDIATYPE_MIDI;
        relation_map["MEDIATYPE_COMPOSITION"]    = wsp::mm::MEDIATYPE_COMPOSITION;
        relation_map["MEDIATYPE_IMAGE"]          = wsp::mm::MEDIATYPE_IMAGE;
        relation_map["MEDIATYPE_IMAGE_SEQUENCE"] = wsp::mm::MEDIATYPE_IMAGE_SEQUENCE;
        relation_map["MEDIATYPE_SOUND"]          = wsp::mm::MEDIATYPE_SOUND;
        relation_map["MEDIATYPE_VIDEO"]          = wsp::mm::MEDIATYPE_VIDEO;
        relation_map["MEDIATYPE_MIDI"]           = wsp::mm::MEDIATYPE_MIDI;

        std::map<std::string, wsp::mm::MediaType>::iterator iter = relation_map.find(in_type_str);
        if( iter!=relation_map.end() )
        {
            return iter->second;
        }
        return wsp::mm::MEDIATYPE_UNKNOWN;
    }

    /**
     * @brief Get wsp::mm::MediaType as string
     */
    inline const char* GetMediaTypeAsString( wsp::mm::MediaType media_type )
    {
        switch( media_type )
        {
        case wsp::mm::MEDIATYPE_IMAGE:          return "MEDIATYPE_IMAGE";
        case wsp::mm::MEDIATYPE_IMAGE_SEQUENCE: return "MEDIATYPE_IMAGE_SEQUENCE";
        case wsp::mm::MEDIATYPE_VIDEO:          return "MEDIATYPE_VIDEO";
        case wsp::mm::MEDIATYPE_SOUND:          return "MEDIATYPE_SOUND";
        case wsp::mm::MEDIATYPE_COMPOSITION:    return "MEDIATYPE_COMPOSITION";
        case wsp::mm::MEDIATYPE_MIDI:           return "MEDIATYPE_MIDI";
        case wsp::mm::MEDIATYPE_UNKNOWN:        return "MEDIATYPE_UNKNOWN";
        }
        return "Undefined media type";
    }

    inline const char* GetMediaTypeAsSimpleString( wsp::mm::MediaType media_type )
    {
        switch( media_type )
        {
        case wsp::mm::MEDIATYPE_IMAGE:          return "image";
        case wsp::mm::MEDIATYPE_IMAGE_SEQUENCE: return "image sequence";
        case wsp::mm::MEDIATYPE_VIDEO:          return "video";
        case wsp::mm::MEDIATYPE_SOUND:          return "sound";
        case wsp::mm::MEDIATYPE_COMPOSITION:    return "composition";
        case wsp::mm::MEDIATYPE_MIDI:           return "midi";
        case wsp::mm::MEDIATYPE_UNKNOWN:        return "unknown";
        }
        return "undefined";
    }

    /** @brief  Determin whether extension is image type extension. */
    inline bool IsImageExtension(const char *ext){
        return !(
              strcmp(ext, "jpg")
            & strcmp(ext, "png")
            & strcmp(ext, "bmp") 
            & strcmp(ext, "gif")
            & strcmp(ext, "tif")
            & strcmp(ext, "pgm")
            & strcmp(ext, "ppm")
            & strcmp(ext, "xbm")
            & strcmp(ext, "xpm")
            & strcmp(ext, "adri")
        );
    }
    /** @brief  Determin whether extension is video type extension. */
    inline bool IsVideoExtension(const char *ext){
        return !(
              strcmp(ext, "avi")
            & strcmp(ext, "mp4")
            & strcmp(ext, "mov") 
            & strcmp(ext, "flv")
            & strcmp(ext, "swf")
            & strcmp(ext, "mkv")
            & strcmp(ext, "wmv")
            & strcmp(ext, "rm")
            & strcmp(ext, "mpg")
            & strcmp(ext, "mpeg")
            & strcmp(ext, "3gp")
            & strcmp(ext, "m4v")
        );
    }
    /** @brief  Determin whether extension is audio type extension. */
    inline bool IsSoundExtension(const char *ext){
        return !(
              strcmp(ext, "mp3")
            & strcmp(ext, "wav")
            & strcmp(ext, "wma") 
            & strcmp(ext, "aif")
        );
    }

    /** @brief  Determin whether extension is MIDI type extension. */
    inline bool IsMidiExtension(const char *ext){
        return !(
              strcmp(ext, "mid")
        );
    }

    inline bool ContinuousSeqFileExists( const char *in_filename )
    {
        char num_str[32], filepath[MAX_LENGTH_OF_PATH];
        wsp::GetNumber(num_str, in_filename);

        wsp::SeqInfo seq_info;
        wsp::State state = wsp::GetSeqInfo( seq_info, in_filename );
        if( state!=WSP_STATE_SUCCESS ){ return false; }

        char *end_ptr = num_str+strlen(num_str);
        s32 frame = std::strtol(num_str, &end_ptr, 10);
        wsp::GetSeqPath( filepath, seq_info, frame-1 );
        bool prev_frame_file_exists = WSP_Exists( filepath );
        if( prev_frame_file_exists ){ return true; }

        wsp::GetSeqPath( filepath, seq_info, frame+1 );
        bool next_frame_file_exists = WSP_Exists( filepath );
        if( next_frame_file_exists ){ return true; }

        return false;
    }

    inline bool IsImageSequence(const char *in_filename)
    {
        char ext[MAX_LENGTH_OF_NAME], num_str[32];

        wsp::GetExtension(ext, in_filename);
        bool is_image_ext = wsp::mm::IsImageExtension(ext);
        if( is_image_ext==false ){ return false; }

        wsp::GetNumber(num_str, in_filename);
        bool filename_has_number_at_tail = (strlen(num_str) != 0);
        if( filename_has_number_at_tail==false ){ return false; }

        bool continuous_seq_file_exists = wsp::mm::ContinuousSeqFileExists( in_filename );
        if( continuous_seq_file_exists==false ){ return false; }

        return true;
    }

    /** @brief  Determine media type from extension. */
    inline wsp::mm::MediaType GetMediaTypeFromExtension(const char *filename)
    {
        char ext[MAX_LENGTH_OF_NAME];
        wsp::GetExtension(ext, filename);
        if(wsp::mm::IsVideoExtension(ext)){ return wsp::mm::MEDIATYPE_VIDEO; }
        if(wsp::mm::IsSoundExtension(ext)){ return wsp::mm::MEDIATYPE_SOUND; }
        if(wsp::mm::IsMidiExtension(ext)) { return wsp::mm::MEDIATYPE_MIDI; }
        if(wsp::mm::IsImageSequence(filename)){ return wsp::mm::MEDIATYPE_IMAGE_SEQUENCE; }
        return wsp::mm::MEDIATYPE_IMAGE;
    }
}}

#endif