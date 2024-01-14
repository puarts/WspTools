/**
 * @file tpl_cl-compositionlayer.inl
 * @author Jun-ichi Nishikata
 */

#ifndef __WSP_MULTIMEDIA_TEMPLATE_CLASS_COMPOSITION_LAYER_INL__
#define __WSP_MULTIMEDIA_TEMPLATE_CLASS_COMPOSITION_LAYER_INL__

#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#include <wsp/common/tpl_cl-vector.h>
#endif
#include <wsp/common/_cl-std_param.h>
#include <wsp/image/_tpl_fn-imgproc.hpp>
#include <wsp/image/gpl_tpl_fn-imgproc.h>

#include "tpl_cl-compositionlayer.h"

#define DEBUG_WSP_COMPOSITION_LAYER 0

namespace {
    void PrintParams( const wsp::Vector<wsp::StdParam>* params, const wsp::StdParamDefList* def_list )
    {
        const wsp::StdParam* param_ptr = params->data();
        const wsp::StdParam* end_ptr = param_ptr + params->length();
        const wsp::StdParamDefinition* def_ptr = def_list->data();
        WSP_COMMON_LOG("PrintParams( params, def_list )\n");
        for( ; param_ptr!=end_ptr; ++param_ptr, ++def_ptr )
        {
            WSP_COMMON_LOG( "    " );
            wsp::PrintStdParam( param_ptr, def_ptr );
        }
    }

    template<typename T>
    wsp::Result GetOutputImageMain( 
        const wsp::Image<T>*                     in_image, 
        wsp::Image<T>*                           o_image,
        const wsp::Vector<wsp::ImageFilter<T>*>* in_effect_list,
        const wsp::CompLayerParam*               in_params
    )
    {
        WSP_ASSERT_NOT_NULL( in_image );
        WSP_ASSERT_NOT_NULL( o_image );
        WSP_ASSERT_EQUAL( in_image->num_of_channels(), o_image->num_of_channels() );

        int src_width  = in_image->width();
        int src_height = in_image->height();
        int nc         = in_image->num_of_channels();

        int dest_width  = o_image->width();
        int dest_height = o_image->height();

        /* transform */
        {
            wsp::img::AffineParam affine_param;
            affine_param.anchor_x     = in_params->anchor_pt_x;
            affine_param.anchor_y     = in_params->anchor_pt_y;
            affine_param.rotate_angle = in_params->rotate_x;
            affine_param.scale_x      = in_params->scale_x;
            affine_param.scale_y      = in_params->scale_y;
            affine_param.translate_x  = in_params->translate_x;
            affine_param.translate_y  = in_params->translate_y;

            wsp::img::AffineTransform<T>( 
                in_image->data(), src_width,  src_height,  nc,
                o_image->data(),  dest_width, dest_height, nc, affine_param );

            wsp::img::AffineTransform<u8>( 
                in_image->alpha_data(), src_width,  src_height,  1,
                o_image->alpha_data(),  dest_width, dest_height, 1, affine_param );

        }

        /* apply effect */
        s32 num_effects = in_effect_list->length();
        for( s32 i=0; i<num_effects; ++i )
        {
            wsp::ImageFilterPluginInterface<T>* filter_interface = (*in_effect_list)[i]->image_filter_interface();
            wsp::StdParamDefList*               param_def_list   = filter_interface->param_def_list();
            wsp::Vector<wsp::StdParam>*         params           = (*in_effect_list)[i]->image_filter_params();

            PrintParams( params, param_def_list );

            filter_interface->SetImageFrom( o_image );
            wsp::Result result = filter_interface->ApplyFilter( *params );
            if( result.IsFailure() )
            { 
                return result; 
            }
        }

        /* apply opacity */
        {
            u8* ptr = o_image->alpha_data();
            u8* end_ptr = ptr + o_image->width()*o_image->height();
            for( ; ptr!=end_ptr; ++ptr )
            {
                *ptr = (u8)((float)(*ptr) * in_params->opacity);
            }
        }

        return wsp::ResultSuccess();
    }

}

#define INIT_WSP_COMPOSITION_LAYER( id ) \
    id_(id)\
    , name_("uninitialized")\
    , params_( WSP_DEFAULT_COMP_LAYER_PARAM )\
    , data_type_(wsp::mm::MEDIATYPE_UNKNOWN)\
    , has_own_data_(true)

// Constructor, Destructor -----------------

template<typename T>
wsp::mm::CompositionLayer<T>::CompositionLayer(void)
    : INIT_WSP_COMPOSITION_LAYER(0)
{}

template<typename T>
wsp::mm::CompositionLayer<T>::CompositionLayer( s32 id )
    : INIT_WSP_COMPOSITION_LAYER(id)
{}

template<typename T>
wsp::mm::CompositionLayer<T>::CompositionLayer(  wsp::ImageSeq<T> *data_to_set, s32 id )
    : INIT_WSP_COMPOSITION_LAYER(id)
{
    SetExternalImageSeq( data_to_set );
}

template<typename T>
wsp::mm::CompositionLayer<T>::CompositionLayer( wsp::Image<T> *data_to_set, s32 id )
    : INIT_WSP_COMPOSITION_LAYER(id)
{ 
    SetExternalImage( data_to_set );
}

template<typename T>
wsp::mm::CompositionLayer<T>::~CompositionLayer()
{
    // do not need to free data_ because data_ is not own data.
    int num_effects = effect_list_.length();
    for( int i=0; i<num_effects; ++i )
    {
        delete effect_list_[i];
    }
}

// getter -------------------------------------


template<typename T>
wsp::Image<T>*    wsp::mm::CompositionLayer<T>::GetExternalImage   ()
{
    if( has_own_data_ )
    {
        WSP_COMMON_WARNING_LOG("current data is own data, NULL is returned\n");
        return NULL;
    }
    return data_.as_image_ptr;
}

template<typename T>
wsp::ImageSeq<T>* wsp::mm::CompositionLayer<T>::GetExternalImageSeq()
{
    if( has_own_data_ )
    {
        WSP_COMMON_WARNING_LOG("current data is own data, NULL is returned\n");
        return NULL;
    }
    return data_.as_image_seq_ptr;
}

template<typename T>
wsp::Sound<T>*    wsp::mm::CompositionLayer<T>::GetExternalSound   ()
{
    if( has_own_data_ )
    {
        WSP_COMMON_WARNING_LOG("current data is own data, NULL is returned\n");
        return NULL;
    }
    return data_.as_sound_ptr;
}

template<typename T>
wsp::sound::Midi*    wsp::mm::CompositionLayer<T>::GetExternalMidi()
{
    if( has_own_data_ )
    {
        WSP_COMMON_WARNING_LOG("current data is own data, NULL is returned\n");
        return NULL;
    }
    return data_.as_midi_ptr;
}


template<typename T>
wsp::Result wsp::mm::CompositionLayer<T>::GetOutputImage( wsp::Image<T>* o_image ) const
{
    WSP_ASSERT_EQUAL( data_type_, wsp::mm::MEDIATYPE_IMAGE );
    return GetOutputImageMain( data_.as_image_ptr, o_image, &effect_list_, &params_ );
}


template<typename T>
wsp::Result wsp::mm::CompositionLayer<T>::GetOutputImageSeqFrame( wsp::Image<T>* o_image, s32 frame ) const
{
    if( data_type_!=wsp::mm::MEDIATYPE_IMAGE_SEQUENCE )
    {
        return wsp::ResultInvalidParameter();
    }
    if( frame < 0 || data_.as_image_seq_ptr->SeqSize() <= frame )
    {
        return wsp::ResultInvalidParameter();
    }

    wsp::Image<T>* src_image = &(*data_.as_image_seq_ptr)[frame];
    return GetOutputImageMain( src_image, o_image, &effect_list_, &params_ );
}



// setter -------------------------------------
template<typename T>
void wsp::mm::CompositionLayer<T>::SetExternalImage(wsp::Image<T> *image_data)
{
    if(has_own_data_==true)
    {
        ReleaseDataMemory();
    }
    data_.as_image_ptr = image_data;
    data_type_ = wsp::mm::MEDIATYPE_IMAGE;
    has_own_data_ = false;
}
template<typename T>
void wsp::mm::CompositionLayer<T>::SetExternalImageSeq(wsp::ImageSeq<T> *image_seq_data)
{
    if(has_own_data_==true)
    {
        ReleaseDataMemory();
    }
    data_.as_image_seq_ptr = image_seq_data;
    data_type_ = wsp::mm::MEDIATYPE_IMAGE_SEQUENCE;
    has_own_data_ = false;
}
template<typename T>
void wsp::mm::CompositionLayer<T>::SetExternalSound(wsp::Sound<T> *sound_data)
{
    if(has_own_data_==true)
    {
        ReleaseDataMemory();
    }
    data_.as_sound_ptr = sound_data;
    data_type_ = wsp::mm::MEDIATYPE_SOUND;
    has_own_data_ = false;
}
template<typename T>
void wsp::mm::CompositionLayer<T>::SetExternalMidi( wsp::sound::Midi *midi_data )
{
    if(has_own_data_==true)
    {
        ReleaseDataMemory();
    }
    data_.as_midi_ptr = midi_data;
    data_type_ = wsp::mm::MEDIATYPE_MIDI;
    has_own_data_ = false;
}

template<typename T>
void wsp::mm::CompositionLayer<T>::AddImageFilter( wsp::ImageFilterPluginInterface<T>* image_filter_plugin  )
{
    wsp::ImageFilter<T>* effect = new wsp::ImageFilter<T>( image_filter_plugin );
    effect_list_.Append( effect );
}

template<typename T>
s32 wsp::mm::CompositionLayer<T>::TakeImageFilter( 
    const wsp::ImageFilterPluginInterface<T>* target_filter, s32 num_take 
)
{
    wsp::ImageFilter<T>** ptr     = effect_list_.data();
    wsp::ImageFilter<T>** end_ptr = effect_list_.data() + effect_list_.length();
    wsp::ImageFilter<T>* store_elem = *ptr;
    s32 num_found_elems = 0;
    s32 index = 0;
    s32 found_index = -1;
    for( ; ptr!=end_ptr; ++ptr, ++index )
    {
        if( ( (*ptr)->image_filter_interface() == target_filter )
            && ( num_found_elems < num_take ) )
        {
            ++num_found_elems;
            found_index = index;
            continue;
        }
        *(ptr-num_found_elems) = *(ptr);
    }
    if( num_found_elems == 0 ){ return -1; }

    effect_list_.SetLength( effect_list_.length() - num_found_elems );
    return found_index;
}

template<typename T>
void wsp::mm::CompositionLayer<T>::ReleaseDataMemory(void)
{
    WSP_ASSERT( has_own_data_==true, "current data is external data" );

    switch(data_type_){
    case wsp::mm::MEDIATYPE_IMAGE:         { delete data_.as_image_ptr;    } break;
    case wsp::mm::MEDIATYPE_IMAGE_SEQUENCE:{ delete data_.as_image_seq_ptr;} break;
    case wsp::mm::MEDIATYPE_SOUND:         { delete data_.as_sound_ptr;    } break;
    case wsp::mm::MEDIATYPE_UNKNOWN:       break;
    default: WSP_COMMON_ERROR_LOG("invalid media type %d\n", data_type_); return;
    }
    data_.as_byte_ptr = NULL;
}

template<typename T>
void wsp::mm::CompositionLayer<T>::Reset()
{
    effect_list_.Clear();
    params_ = WSP_DEFAULT_COMP_LAYER_PARAM;

    if(has_own_data_==false)
    {
        data_type_ = wsp::mm::MEDIATYPE_UNKNOWN;
        memset(&data_, 0, sizeof(data_));
        has_own_data_ = true;
        return;
    }
    ReleaseDataMemory();
}


// I/O -------------------------------------
template<typename T>
wsp::Result wsp::mm::CompositionLayer<T>::Save( const char *filepath )
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
    default:
        WSP_COMMON_ERROR_LOG("unsupported media type\n"); return WSP_ResultStatusError();
    }
    return WSP_ResultSuccess();
}

#endif
