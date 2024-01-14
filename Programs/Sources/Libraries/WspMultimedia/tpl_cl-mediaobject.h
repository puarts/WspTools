/**
 * @file tpl_cl-mediaobject.h
 * @author Jun-ichi Nishikata
 * @brief Declaration of wsp::mm::MediaObject class.
 */

#ifndef __WSP_MULTIMEDIA_TEMPLATE_CLASS_MEDIA_OBJECT_H__
#define __WSP_MULTIMEDIA_TEMPLATE_CLASS_MEDIA_OBJECT_H__

#include "_define_multimedia.h"

#ifndef __WSP_COMMONCORE_RESULT_H__
#include <wsp/common/result.h>
#endif
#include <wsp/image/tpl_cl-image.h>
#include <wsp/image/tpl_cl-imageseq.h>
#include <wsp/sound/tpl_cl-sound.h>
#include <wsp/sound/cl-midi.hpp>

#include "tpl_cl-composition.h"

namespace wsp{ namespace mm{
    /** 
     * @brief template class for multimedia object
     */
    template<typename T=u8>
    class MediaObject
    {
    public:
        explicit MediaObject( s32 id=0 );
        explicit MediaObject( const wsp::mm::MediaObject<T> &rhs );
        explicit MediaObject( wsp::mm::MediaType type, s32 id );
        ~MediaObject();

        /* --------------------
               getter 
           --------------------*/
        s32                        id           () const{ return id_; }
        wsp::mm::MediaType data_type    () const{ return data_type_; }
        bool                       has_own_data () const{ return has_own_data_; }

        wsp::Image<T>*           image_data         () { return data_.as_image_ptr; }
        wsp::ImageSeq<T>*        image_seq_data     () { return data_.as_image_seq_ptr; }
        wsp::Sound<T>*           sound_data         () { return data_.as_sound_ptr; }
        wsp::mm::Composition<T>* composition_data   () { return data_.as_composition_ptr; }
        wsp::sound::Midi*        midi_data          () { return data_.as_midi_ptr; }

        const wsp::Image<T>*           image_data         () const{ return data_.as_image_ptr; }
        const wsp::ImageSeq<T>*        image_seq_data     () const{ return data_.as_image_seq_ptr; }
        const wsp::Sound<T>*           sound_data         () const{ return data_.as_sound_ptr; }
        const wsp::mm::Composition<T>* composition_data   () const{ return data_.as_composition_ptr; }
        const wsp::sound::Midi*        midi_data          () const{ return data_.as_midi_ptr; }

        wsp::Image<T>*           GetExternalImage       ();
        wsp::ImageSeq<T>*        GetExternalImageSeq    ();
        wsp::Sound<T>*           GetExternalSound       ();
        wsp::mm::Composition<T>* GetExternalComposition ();

        const wsp::Image<T>*           GetExternalImage       ()const;
        const wsp::ImageSeq<T>*        GetExternalImageSeq    ()const;
        const wsp::Sound<T>*           GetExternalSound       ()const;
        const wsp::mm::Composition<T>* GetExternalComposition ()const;

        /* --------------------
               setter 
           --------------------*/
        void set_id( s32 id ){ id_ = id; }
        /** @brief  free data and set data type as unknown data */
        wsp::Result Clear();

        //** @brief function for creating new data
        wsp::Result CreateData(wsp::mm::MediaType type);

        // functions for setting external data
        void SetExternalImage       ( wsp::Image<T>       *data );
        void SetExternalImageSeq    ( wsp::ImageSeq<T>    *data );
        void SetExternalSound       ( wsp::Sound<T>       *data );
        void SetExternalComposition ( wsp::mm::Composition<T> *data );

        /** @brief  set data of destination to self data and clear myself */
        wsp::Result TransferExternalData( wsp::mm::MediaObject<T> &o_dest );

        /* --------------------
               I/O
           --------------------*/
        /**
         * @brief Load a media content.
         * @param force_image_as_image_seq Whether force to load image as image sequence.
         * @param image_as_alpha Load image file as alpha channel if image_as_alpha is true.
         */
        wsp::Result Load( 
            const char *filename,
            bool force_image_as_image_seq=false,
            bool image_as_alpha=false
            #ifdef USE_QT
            ,QProgressBar *progress_bar=NULL
            #endif
        );
        
        wsp::Result Save( const char *filepath );
        wsp::State  SaveAsImage(const char *filename);

        #ifdef USE_QT
        wsp::State LoadDialog(
            bool force_image_as_image_seq=false,
            bool image_as_alpha=false
            , QProgressBar *progress_bar=NULL
            , const char *title="Open"
        );
        #endif

        /* --------------------
               operator
           --------------------*/
        wsp::mm::MediaObject<T>& operator= ( const wsp::mm::MediaObject<T> &rhs );
        bool                     operator==( const wsp::mm::MediaObject<T> &rhs ) const;
        bool                     operator!=( const wsp::mm::MediaObject<T> &rhs ) const;

    private:
        /* --------------------
               data
           --------------------*/

        s32                        id_;
        /** @brief  current data type */
        wsp::mm::MediaType data_type_;

        /** @brief  media data itself */
        union {
            u8                  *as_byte_ptr;
            wsp::Image<T>       *as_image_ptr;
            wsp::ImageSeq<T>    *as_image_seq_ptr;
            wsp::Sound<T>       *as_sound_ptr;
            wsp::mm::Composition<T> *as_composition_ptr;
            wsp::sound::Midi    *as_midi_ptr;
        } data_;

        /** @brief  whether current data is own data or external data */
        bool has_own_data_;
    };
}}

#include "tpl_cl-mediaobject.inl"

#endif
