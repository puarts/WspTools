/**
 * @file cl-mediaobject.inl
 * @author Jun-ichi Nishikata
 */

#ifndef __WSP_MULTIMEDIA_TEMPLATE_CLASS_MEDIA_OBJECT_INL__
#define __WSP_MULTIMEDIA_TEMPLATE_CLASS_MEDIA_OBJECT_INL__

#include <wsp/video/tpl_fn-videoio-gpl.h>
#include <wsp/sound/fn-midi.h>
#include "fn-mediautil.h"

#include "multimedia_result.h"
#ifndef __WSP_COMMONCORE_COMMON_RESULT_H__
#include <wsp/common/common_result.h>
#endif

#include "tpl_cl-mediaobject.h"

/* ---------------------------------
       Constructor, Destructor
   ---------------------------------*/


template<typename T>
wsp::mm::MediaObject<T>::MediaObject( s32 id )
    : data_type_(wsp::mm::MEDIATYPE_UNKNOWN)
    , has_own_data_(true)
    , id_(id)
{ 
    data_.as_image_ptr = NULL;
}

template<typename T>
wsp::mm::MediaObject<T>::MediaObject( const wsp::mm::MediaObject<T> &copy_src )
{
    (*this) = copy_src;
}

template<typename T>
wsp::mm::MediaObject<T>::MediaObject( wsp::mm::MediaType type, s32 id )
    : id_(id)
{
    switch(type)
    {
    case wsp::mm::MEDIATYPE_IMAGE:
        data_.as_image_ptr = new wsp::Image<T>();
        break;
    case wsp::mm::MEDIATYPE_IMAGE_SEQUENCE:
        data_.as_image_seq_ptr = new wsp::ImageSeq<T>();
        break;
    case wsp::mm::MEDIATYPE_SOUND:
        data_.as_sound_ptr = new wsp::Sound<T>();
        break;
    case wsp::mm::MEDIATYPE_COMPOSITION:
        data_.as_composition_ptr = new wsp::mm::Composition<T>();
        break;
    case wsp::mm::MEDIATYPE_MIDI:
        data_.as_midi_ptr = new wsp::sound::Midi();
        break;
    default:
        WSP_COMMON_ERROR_LOG("Unsupported file type\n");
        return;
    }
    data_type_ = type;
}

template<typename T>
wsp::mm::MediaObject<T>::~MediaObject()
{
    if(has_own_data_==false){ return; }
    Clear();
}

/* ---------------------------------
             Getter
   ---------------------------------*/

template<typename T>
wsp::Image<T>*    wsp::mm::MediaObject<T>::GetExternalImage   ()
{
    if( has_own_data_ )
    {
        WSP_COMMON_WARNING_LOG("current data is own data, NULL is returned\n");
        return NULL;
    }
    return data_.as_image_ptr;
}

template<typename T>
wsp::ImageSeq<T>* wsp::mm::MediaObject<T>::GetExternalImageSeq()
{
    if( has_own_data_ )
    {
        WSP_COMMON_WARNING_LOG("current data is own data, NULL is returned\n");
        return NULL;
    }
    return data_.as_image_seq_ptr;
}

template<typename T>
wsp::Sound<T>*    wsp::mm::MediaObject<T>::GetExternalSound   ()
{
    if( has_own_data_ )
    {
        WSP_COMMON_WARNING_LOG("current data is own data, NULL is returned\n");
        return NULL;
    }
    return data_.as_sound_ptr;
}

template<typename T>
wsp::mm::Composition<T>*    wsp::mm::MediaObject<T>::GetExternalComposition   ()
{
    if( has_own_data_ )
    {
        WSP_COMMON_WARNING_LOG("current data is own data, NULL is returned\n");
        return NULL;
    }
    return data_.as_composition_ptr;
}


template<typename T>
const wsp::Image<T>*    wsp::mm::MediaObject<T>::GetExternalImage   ()const 
{
    if( has_own_data_ )
    {
        WSP_COMMON_WARNING_LOG("current data is own data, NULL is returned\n");
        return NULL;
    }
    return data_.as_image_ptr;
}

template<typename T>
const wsp::ImageSeq<T>* wsp::mm::MediaObject<T>::GetExternalImageSeq()const 
{
    if( has_own_data_ )
    {
        WSP_COMMON_WARNING_LOG("current data is own data, NULL is returned\n");
        return NULL;
    }
    return data_.as_image_seq_ptr;
}

template<typename T>
const wsp::Sound<T>*    wsp::mm::MediaObject<T>::GetExternalSound   ()const 
{
    if( has_own_data_ )
    {
        WSP_COMMON_WARNING_LOG("current data is own data, NULL is returned\n");
        return NULL;
    }
    return data_.as_sound_ptr;
}

template<typename T>
const wsp::mm::Composition<T>*    wsp::mm::MediaObject<T>::GetExternalComposition   ()const 
{
    if( has_own_data_ )
    {
        WSP_COMMON_WARNING_LOG("current data is own data, NULL is returned\n");
        return NULL;
    }
    return data_.as_composition_ptr;
}

/* ---------------------------------
             Setter
   ---------------------------------*/

/* free data and set data type as unknown data */
template<typename T>
wsp::Result wsp::mm::MediaObject<T>::Clear()
{
    if(has_own_data_==false){
        data_type_ = wsp::mm::MEDIATYPE_UNKNOWN;
        memset(&data_, 0, sizeof(data_));
        has_own_data_ = true;
        return wsp::ResultSuccess();
    }
    switch(data_type_){
    case wsp::mm::MEDIATYPE_IMAGE:         { delete data_.as_image_ptr;          } break;
    case wsp::mm::MEDIATYPE_IMAGE_SEQUENCE:{ delete data_.as_image_seq_ptr;      } break;
    case wsp::mm::MEDIATYPE_SOUND:         { delete data_.as_sound_ptr;          } break;
    case wsp::mm::MEDIATYPE_COMPOSITION:   { delete data_.as_composition_ptr;    } break;
    case wsp::mm::MEDIATYPE_MIDI:          { delete data_.as_midi_ptr;           } break;
    case wsp::mm::MEDIATYPE_UNKNOWN:       break;
    default: 
        WSP_COMMON_ERROR_LOG("invalid media type %d\n", data_type_); 
        return wsp::ResultUnsupported();
    }
    data_.as_byte_ptr = NULL;

    return wsp::ResultSuccess();
}
/* functions for creating new data */
template<typename T>
wsp::Result wsp::mm::MediaObject<T>::CreateData(wsp::mm::MediaType type)
{
    WSP_RETURN_IF_FAILED( Clear() );

    switch(type){
    case wsp::mm::MEDIATYPE_IMAGE:
        data_.as_image_ptr = new wsp::Image<T>();
        data_type_ = wsp::mm::MEDIATYPE_IMAGE;
        break;
    case wsp::mm::MEDIATYPE_IMAGE_SEQUENCE:
        data_.as_image_seq_ptr = new wsp::ImageSeq<T>();
        data_type_ = wsp::mm::MEDIATYPE_IMAGE_SEQUENCE;
        break;
    case wsp::mm::MEDIATYPE_SOUND:
        data_.as_sound_ptr = new wsp::Sound<T>();
        data_type_ = wsp::mm::MEDIATYPE_SOUND;
        break;
    case wsp::mm::MEDIATYPE_COMPOSITION:
        data_.as_composition_ptr = new wsp::mm::Composition<T>();
        data_type_ = wsp::mm::MEDIATYPE_COMPOSITION;
        break;
    case wsp::mm::MEDIATYPE_MIDI:
        data_.as_midi_ptr = new wsp::sound::Midi();
        data_type_ = wsp::mm::MEDIATYPE_MIDI;
        break;
    default:
        WSP_COMMON_ERROR_LOG("unknown media type\n"); 
        return wsp::mm::ResultUnsupportedType();
    }
    return wsp::ResultSuccess();
}
template<typename T>
void wsp::mm::MediaObject<T>::SetExternalImage(wsp::Image<T> *image_data)
{
    Clear();
    data_.as_image_ptr = image_data;
    data_type_ = wsp::mm::MEDIATYPE_IMAGE;
    has_own_data_ = false;
}
template<typename T>
void wsp::mm::MediaObject<T>::SetExternalImageSeq(wsp::ImageSeq<T> *image_seq_data)
{
    Clear();
    data_.as_image_seq_ptr = image_seq_data;
    data_type_ = wsp::mm::MEDIATYPE_IMAGE_SEQUENCE;
    has_own_data_ = false;
}
template<typename T>
void wsp::mm::MediaObject<T>::SetExternalSound(wsp::Sound<T> *sound_data)
{
    Clear();
    data_.as_sound_ptr = sound_data;
    data_type_ = wsp::mm::MEDIATYPE_SOUND;
    has_own_data_ = false;
}

template<typename T>
void wsp::mm::MediaObject<T>::SetExternalComposition ( wsp::mm::Composition<T> *data )
{
    Clear();
    data_.as_composition_ptr = data;
    data_type_ = wsp::mm::MEDIATYPE_COMPOSITION;
    has_own_data_ = false;
}

template<typename T>
wsp::Result wsp::mm::MediaObject<T>::TransferExternalData( wsp::mm::MediaObject<T> &o_dest )
{
    if( has_own_data_ )
    {
        WSP_COMMON_WARNING_LOG("current data is own data, data transfer is skipped\n");
        return wsp::mm::ResultIsExternalData();
    }
    switch(data_type_){
    case wsp::mm::MEDIATYPE_IMAGE:
        o_dest.SetExternalImage( data_.as_image_ptr );
        break;
    case wsp::mm::MEDIATYPE_IMAGE_SEQUENCE:
        o_dest.SetExternalImageSeq( data_.as_image_seq_ptr );
        break;
    case wsp::mm::MEDIATYPE_SOUND:
        o_dest.SetExternalSound( data_.as_sound_ptr );
        break;
    case wsp::mm::MEDIATYPE_COMPOSITION:
        o_dest.SetExternalComposition( data_.as_composition_ptr );
        break;
    default:
        WSP_COMMON_ERROR_LOG("unsupported media type\n"); 
        return wsp::mm::ResultUnsupportedType();
    }
    return Clear();
}

/* ---------------------------------
            I/O
   ---------------------------------*/

template<typename T>
wsp::Result wsp::mm::MediaObject<T>::Load(
    const char *filename,
    bool force_image_as_image_seq,
    bool image_as_alpha
#ifdef USE_QT
    ,QProgressBar *progress_bar
#endif
){
    char extension[MAX_LENGTH_OF_NAME];
    wsp::mm::MediaType media_type = wsp::mm::GetMediaTypeFromExtension(filename);

    printf("Loading a media content...\n");

    wsp::StartTimer();

    // if current data is own data, overwrite the data with new data
    if(has_own_data_==true)
    { 
        WSP_RETURN_IF_FAILED( CreateData(media_type) ); 
    }
    else if(data_type()!=media_type)
    {
        WSP_COMMON_WARNING_LOG("media type is different, %d %d\n", media_type, data_type_);
        return wsp::mm::ResultUnmatchedMediaType();
    }

    switch(media_type)
    {
    case wsp::mm::MEDIATYPE_IMAGE:
        {
            if(force_image_as_image_seq==false)
            {
                data_.as_image_ptr->Load(filename, image_as_alpha);
            }
            else
            {
                if(has_own_data_==true)
                { 
                    WSP_RETURN_IF_FAILED( CreateData(wsp::mm::MEDIATYPE_IMAGE_SEQUENCE) ); 
                }
                data_.as_image_seq_ptr->LoadSeq(filename, 0, -1, image_as_alpha
                    #ifdef USE_QT
                    , progress_bar
                    #endif
                );
            }
        }
        break;
    case wsp::mm::MEDIATYPE_IMAGE_SEQUENCE:
        {
            data_.as_image_seq_ptr->LoadSeq(filename, 0, -1, image_as_alpha
                #ifdef USE_QT
                , progress_bar
                #endif
            );
        }
        break;
    case wsp::mm::MEDIATYPE_VIDEO:
        {
            #ifdef USE_LIBAV
            wsp::video::avLoadVideo(*(data_.as_image_seq_ptr), filename);
            #else
            WSP_COMMON_WARNING_LOG("Video I/O is not supported yet.\n");
            #endif
        }
        break;
    case wsp::mm::MEDIATYPE_SOUND:
        {
            WSP_COMMON_ERROR_LOG("sound data type has not been supported yet\n");
            return wsp::ResultNotImplemented();
        }
        break;
    case wsp::mm::MEDIATYPE_MIDI:
        {
            WSP_RETURN_IF_FAILED( data_.as_midi_ptr->Load( filename ) );
        }
        break;
    case wsp::mm::MEDIATYPE_COMPOSITION:
        {
            WSP_COMMON_ERROR_LOG("composition data type has not been supported yet\n");
            return wsp::ResultNotImplemented();
        }
        break;
    default:
        {
            WSP_COMMON_ERROR_LOG("unsupported media type\n");
        }
        return wsp::mm::ResultUnsupportedType();
    }

    wsp::StopTimer();

    return wsp::ResultSuccess();
}

template<typename T> 
wsp::Result wsp::mm::MediaObject<T>::Save( const char *filepath )
{
    switch(data_type_){
    case wsp::mm::MEDIATYPE_IMAGE:
        {
            data_.as_image_ptr->Save( filepath );
        }
        break;
    case wsp::mm::MEDIATYPE_IMAGE_SEQUENCE:
        {
            data_.as_image_seq_ptr->SaveImageSeq( filepath );
        }
        break;
    case wsp::mm::MEDIATYPE_SOUND:
        {
            data_.as_sound_ptr->Save( filepath );
        }
        break;
    case wsp::mm::MEDIATYPE_COMPOSITION:
        {
            data_.as_composition_ptr->SaveFrameAsImage( 0, filepath );
        }
        break;
    case wsp::mm::MEDIATYPE_MIDI:
        {
            WSP_RETURN_IF_FAILED( data_.as_midi_ptr->Save( filename ) );
        }
        break;
    default:
        WSP_COMMON_ERROR_LOG("unsupported media type\n"); 
        return WSP_ResultStatusError();
    }
    return WSP_ResultSuccess();
}

template<typename T> 
wsp::State wsp::mm::MediaObject<T>::SaveAsImage(const char *filename)
{
    if( data_type_!=wsp::mm::MEDIATYPE_IMAGE ){
        WSP_COMMON_ERROR_LOG("current data type is not image type\n");
        return WSP_STATE_FAILURE;
    }
    return data_.as_image_ptr->SaveImage( filename );
}

#ifdef USE_QT
template<typename T> 
wsp::State wsp::mm::MediaObject<T>::LoadDialog(
    bool force_image_as_image_seq,
    bool image_as_alpha
    , QProgressBar *progress_bar
    , const char *title
)
{
    char currentDir[MAX_LENGTH_OF_PATH];
    QString qfilename = QFileDialog::getOpenFileName(
                            NULL, 
                            QString(title), 
                            QDir::currentPath(), 
                            wsp::formatFilterMultimedia
                        );
    if( qfilename.isNull() ){
        return WSP_STATE_CANCELED;
    }
    const char *path_ptr = qfilename.toLatin1().data();
    if( WSP_Exists(path_ptr)==false ){
        WSP_COMMON_ERROR_LOG("%s does not exist.\n", path_ptr);
        return WSP_STATE_NOT_FOUND;
    }
    wsp::GetDir(currentDir, path_ptr);
    if(QDir::setCurrent(QString(currentDir))==false){ 
        WSP_COMMON_ERROR_LOG("setCurrent failed.\n"); 
        return WSP_STATE_FAILURE;
    }
    return Load(
            path_ptr,
            force_image_as_image_seq,
            image_as_alpha
            #ifdef USE_QT
            , progress_bar
            #endif
        );
}
#endif


#endif


/* ---------------------------------
            Operator
   ---------------------------------*/
template<typename T> 
wsp::mm::MediaObject<T>& wsp::mm::MediaObject<T>::operator=( const wsp::mm::MediaObject<T> &rhs )
{
    if( *this != rhs )
    {
        id_ = rhs.id();
        if( rhs.has_own_data()==false )
        {
            data_type_ = rhs.data_type();
            switch(rhs.data_type())
            {
            case wsp::mm::MEDIATYPE_IMAGE:
                SetExternalImage( const_cast<wsp::Image<T>*>(rhs.GetExternalImage()) );
                break;
            case wsp::mm::MEDIATYPE_IMAGE_SEQUENCE:
                SetExternalImageSeq( const_cast<wsp::ImageSeq<T>*>(rhs.GetExternalImageSeq()) );
                break;
            case wsp::mm::MEDIATYPE_SOUND:
                SetExternalSound( const_cast<wsp::Sound<T>*>(rhs.GetExternalSound()) );
                break;
            case wsp::mm::MEDIATYPE_COMPOSITION:
                SetExternalComposition( const_cast<wsp::mm::Composition<T>*>(rhs.GetExternalComposition()) );
                break;
            case wsp::mm::MEDIATYPE_MIDI:
            default:
                WSP_COMMON_ERROR_LOG("unknown media type\n"); 
                return (*this);
            }
        }
        else
        {
            this->Clear();
            this->CreateData( rhs.data_type() );
            switch(rhs.data_type())
            {
            case wsp::mm::MEDIATYPE_IMAGE:
                (*data_.as_image_ptr) = (*rhs.image_data());
                break;
            case wsp::mm::MEDIATYPE_IMAGE_SEQUENCE:
                (*data_.as_image_seq_ptr) = (*rhs.image_seq_data());
                break;
            case wsp::mm::MEDIATYPE_SOUND:
                (*data_.as_sound_ptr) = (*rhs.sound_data());
                break;
            case wsp::mm::MEDIATYPE_COMPOSITION:
                (*data_.as_composition_ptr) = (*rhs.composition_data());
                break;
            case wsp::mm::MEDIATYPE_MIDI:
                (*data_.as_midi_ptr) = (*rhs.midi_data());
                break;
            default:
                WSP_COMMON_ERROR_LOG("unknown media type\n"); 
                return (*this);
            }
        }
    }
    return (*this);
}

template<typename T> 
bool wsp::mm::MediaObject<T>::operator==( const wsp::mm::MediaObject<T> &rhs ) const
{
    if( id_ != rhs.id() ){ return false; } 
    if( data_type_ != rhs.data_type() ){ return false; }

    if( rhs.has_own_data()==false )
    {
        switch(rhs.data_type())
        {
        case wsp::mm::MEDIATYPE_IMAGE:
            return data_.as_image_ptr == rhs.GetExternalImage();
        case wsp::mm::MEDIATYPE_IMAGE_SEQUENCE:
            return data_.as_image_seq_ptr == rhs.GetExternalImageSeq();
        case wsp::mm::MEDIATYPE_SOUND:
            return data_.as_sound_ptr == rhs.GetExternalSound();
        case wsp::mm::MEDIATYPE_COMPOSITION:
            return data_.as_composition_ptr == rhs.GetExternalComposition();
        case wsp::mm::MEDIATYPE_MIDI:
        default:
            WSP_COMMON_ERROR_LOG("unknown media type\n"); return false;
        }
    }
    else
    {
        switch(rhs.data_type())
        {
        case wsp::mm::MEDIATYPE_IMAGE:
            return (*data_.as_image_ptr) == (*rhs.image_data());
        case wsp::mm::MEDIATYPE_IMAGE_SEQUENCE:
            return (*data_.as_image_seq_ptr) == (*rhs.image_seq_data());
        case wsp::mm::MEDIATYPE_SOUND:
            return (*data_.as_sound_ptr) == (*rhs.sound_data());
        case wsp::mm::MEDIATYPE_COMPOSITION:
            return (*data_.as_composition_ptr) == (*rhs.composition_data());
        case wsp::mm::MEDIATYPE_MIDI:
            return (*data_.as_midi_ptr) == (*rhs.midi_data());
        default:
            WSP_COMMON_ERROR_LOG("unknown media type\n"); return false;
        }
    }
    return false;
}

template<typename T> 
bool wsp::mm::MediaObject<T>::operator!=( const wsp::mm::MediaObject<T> &rhs ) const
{
    return !(*this==rhs);
}
