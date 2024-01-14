/**
 * @file tpl_fn-mediaio.h
 * @author Jun-ichi Nishikata
 */

#ifndef __WSP_TEMPLATE_FUNCTION_MEDIA_IO_H__
#define __WSP_TEMPLATE_FUNCTION_MEDIA_IO_H__

#include "_define_multimedia.h"
#include "tpl_cl-mediaobject.h"

namespace wsp{ namespace mm{
    /**
     * @brief Load MediaObject from storage.
     */
    template<typename T>
    wsp::State LoadMediaObject(
                    wsp::mm::MediaObject<T> &o_mediaobj, 
                    const char *filename,
                    bool force_image_as_image_seq=false,
                    bool image_as_alpha=false
                #ifdef USE_QT
                    ,QProgressBar *progress_bar=NULL
                #endif
                );
}}


template<typename T>
wsp::State wsp::mm::LoadMediaObject(
    wsp::mm::MediaObject<T> &o_mediaobj, 
    const char *filename,
    bool force_image_as_image_seq,
    bool image_as_alpha
#ifdef USE_QT
    ,QProgressBar *progress_bar
#endif
){
    char extension[MAX_LENGTH_OF_NAME];
    wsp::mm::MediaType media_type = wsp::mm::GetMediaTypeFromExtension(filename);

    printf("Opening multimedia...\n");
    wsp::StartTimer();
    switch(media_type){
    case wsp::mm::MEDIATYPE_IMAGE:
        if(force_image_as_image_seq==false){
            if(o_mediaobj.has_own_data()==true){
                o_mediaobj.CreateImage();
            }
            if(o_mediaobj.data_type()!=wsp::mm::MEDIATYPE_IMAGE){
                WSP_COMMON_ERROR_LOG("media type is wrong\n"); return WSP_STATE_FAILURE;
            }
            o_mediaobj.image_data()->Load(filename, image_as_alpha);
            break;
        }
    case wsp::mm::MEDIATYPE_IMAGE_SEQUENCE:
        if(o_mediaobj.has_own_data()==true){
            o_mediaobj.CreateImageSeq();
        }
        if(o_mediaobj.data_type()!=wsp::mm::MEDIATYPE_IMAGE_SEQUENCE){
            WSP_COMMON_ERROR_LOG("media type is wrong\n"); return WSP_STATE_FAILURE;
        }
        o_mediaobj.image_seq_data()->LoadSeq(filename, 0, -1, image_as_alpha
            #ifdef USE_QT
            , progress_bar
            #endif
        );
        //wsp::GetExtension(extension, filename);
        //if(as_alpha){
        //    if(strcmp(extension, "adri")==0){
        //        o_mediaobj.image_seq_data()->LoadADRISeq(NULL, filename, 0U, -1, false, true
        //            #ifdef USE_QT
        //            , progress_bar
        //            #endif
        //        );
        //    }else{
        //        o_mediaobj.image_seq_data()->LoadImageSeqAsAlpha(filename, -1
        //            #ifdef USE_QT
        //            , progress_bar
        //            #endif
        //        );
        //    }
        //}else{
        //    if(strcmp(extension, "adri")==0){
        //        o_mediaobj.image_seq_data()->LoadADRISeq(filename, NULL, 0U, -1, false, false 
        //            #ifdef USE_QT
        //            , progress_bar
        //            #endif
        //        );
        //    }else if(strcmp(extension, "raw")==0 || strcmp(extension, "bin")==0){
        //        o_mediaobj.image_seq_data()->LoadRawDataSeq(filename, -1
        //            #ifdef USE_QT
        //            , progress_bar
        //            #endif
        //        );
        //    }else{
        //        o_mediaobj.image_seq_data()->LoadImageSeq(filename, 0U, -1 
        //            #ifdef USE_QT
        //            , progress_bar
        //            #endif
        //        );
        //    }
        //}
        //if(o_mediaobj.image_seq_data()->SeqSize()==0){ 
        //    WSP_COMMON_ERROR_LOG("No image sequence loaded\n"); wsp::StopTimer(); return WSP_STATE_FAILURE; 
        //}
        break;
    case wsp::mm::MEDIATYPE_VIDEO:
        wsp::video::avLoadVideo(*(o_mediaobj.image_seq_data()), filename);
        break;
    case wsp::mm::MEDIATYPE_SOUND:
        WSP_COMMON_ERROR_LOG("sound data type has not been supported yet\n");
        break;
    default:
        WSP_COMMON_ERROR_LOG("unsupported media type\n");
        return WSP_STATE_INVALID_PARAMETER;
    }
    wsp::StopTimer();

    return WSP_STATE_SUCCESS;
}

#endif