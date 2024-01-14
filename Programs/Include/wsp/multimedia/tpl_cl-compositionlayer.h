/**
 * @file tpl_cl-compositionlayer.h
 * @author Junichi Nishikata
 * @brief Declaration of wsp::CompostionLayer class.
 */

#ifndef __WSP_MULTIMEDIA_TEMPLATE_CLASS_COMPOSITION_LAYER_H__
#define __WSP_MULTIMEDIA_TEMPLATE_CLASS_COMPOSITION_LAYER_H__

#include <wsp/image/tpl_cl-image.h>
#include <wsp/image/tpl_cl-imageseq.h>
#include <wsp/image/tpl_cl-image_filter_plugin_interface.hpp>
#include <wsp/image/tpl_cl-image_filter.hpp>
#include <wsp/image/_tpl_fn-imgblend.hpp>
#include <wsp/sound/tpl_cl-sound.h>
#include <wsp/sound/cl-midi.hpp>

#include "_define_multimedia.h"

/** @brief Composition layer parameter. */
struct WSP_CompLayerParam{
    f32  translate_x;
    f32  translate_y;
    f32  anchor_pt_x;
    f32  anchor_pt_y;
    f32  scale_x;
    f32  scale_y;
    f32  rotate_x;
    f32  rotate_y;
    f32  opacity;
    s32  start_frame;
    f32  time_strech;
    bool visibility;
    wsp::img::ImageBlendMode mode;
};

/** @brief Print WSP_CompLayerParam. */
inline void WSP_PrintCompLayerParam( const WSP_CompLayerParam* param )
{
    WSP_COMMON_DEBUG_LOG(
        "\n"
        "WSP_CompLayerParam (%08x)\n"
        "  translate_x = %f\n"
        "  translate_y = %f\n"
        "  anchor_pt_x = %f\n"
        "  anchor_pt_y = %f\n"
        "  scale_x     = %f\n"
        "  scale_y     = %f\n"
        "  rotate_x    = %f\n"
        "  rotate_y    = %f\n"
        "  opacity     = %f\n"
        "  start_frame = %d\n"
        "  time_strech = %f\n"
        "  mode        = %d\n"
        "  visibility  = %s\n"
        , (intptr_t)param
        , param->translate_x
        , param->translate_y
        , param->anchor_pt_x
        , param->anchor_pt_y
        , param->scale_x    
        , param->scale_y    
        , param->rotate_x   
        , param->rotate_y   
        , param->opacity    
        , param->start_frame
        , param->time_strech
        , param->mode
        , param->visibility?"true":"false"
    );
}


#ifdef __cplusplus
namespace wsp{
    inline void Print( const WSP_CompLayerParam &param )
    {
        WSP_PrintCompLayerParam( &param );
    }
#endif
    typedef struct WSP_CompLayerParam CompLayerParam;

#ifdef __cplusplus
}
#endif

static struct WSP_CompLayerParam WSP_DEFAULT_COMP_LAYER_PARAM = {
                            0.0f, 0.0f, /* translate */
                            0.0f, 0.0f, /* anchor */
                            1.0f, 1.0f, /* scale */
                            0.0f, 0.0f, /* rotate */
                            1.0f, /* opacity */
                            0, /* start_frame */
                            1.0f, /* time_strech */
                            true, /* visibility */
                            WSP_IMAGE_BLEND_MODE_NORMAL /* blend_mode */
                        };


namespace wsp{ namespace mm{
    /**
     * @brief       template class for unit composition layer.
     * @description wsp::mm::CompositionLayer is a template class which provides 
     *              access to some of composition layer functionality. 
     * @group       WspMultimedia
     */
    template<typename T>
    class CompositionLayer
    {
    public:
        CompositionLayer(void);
        explicit CompositionLayer( s32 id );
        CompositionLayer( wsp::ImageSeq<T> *data_to_set, s32 id );
        CompositionLayer( wsp::Image<T>    *data_to_set, s32 id );
        ~CompositionLayer(void);

        // Accessors ------------------------
        wsp::CompLayerParam  params          (void) const{ return params_; }
        wsp::CompLayerParam* param_data      (void)      { return &params_; }
        /** @brief Get layer ID. */
        s32                  id              (void) const{ return id_; }
        s32                  src_material_id (void) const{ return src_material_id_; }
        const char*          name            (void) const{ return &name_[0]; }
        f32                  translate_x     (void) const{ return params_.translate_x; }
        f32                  translate_y     (void) const{ return params_.translate_y; }
        f32                  anchor_pt_x     (void) const{ return params_.anchor_pt_x; }
        f32                  anchor_pt_y     (void) const{ return params_.anchor_pt_y; }
        f32                  scale_x         (void) const{ return params_.scale_x; }
        f32                  scale_y         (void) const{ return params_.scale_y; }
        f32                  rotate_x        (void) const{ return params_.rotate_x; }
        f32                  rotate_y        (void) const{ return params_.rotate_y; }
        f32                  opacity         (void) const{ return params_.opacity;}
        s32                  start_frame     (void) const{ return params_.start_frame; }
        f32                  time_strech     (void) const{ return params_.time_strech; }
        bool                 visibility      (void) const{ return params_.visibility; }
        wsp::img::ImageBlendMode  mode       (void) const{ return params_.mode; }

        wsp::mm::MediaType data_type      (void) const{ return data_type_; }
        bool                       has_own_data   (void) const{ return has_own_data_; }
        const wsp::Image<T>*       image_data     (void) const{ return data_.as_image_ptr; }
        const wsp::ImageSeq<T>*    image_seq_data (void) const{ return data_.as_image_seq_ptr; }
        const wsp::Sound<T>*       sound_data     (void) const{ return data_.as_sound_ptr; }
        const wsp::sound::Midi*    midi_data      (void) const{ return data_.as_midi_ptr; }

        /** @brief Get effect list applied on layer */
        wsp::Vector<wsp::ImageFilter<T>*>* effect_list(void){ return &effect_list_; }

        wsp::Result          GetOutputImage              ( wsp::Image<T>* o_image            ) const;
        wsp::Result          GetOutputImageSeqFrame      ( wsp::Image<T>* o_image, s32 frame ) const;
        wsp::Image<T>*       GetExternalImage     (void);
        wsp::ImageSeq<T>*    GetExternalImageSeq  (void);
        wsp::Sound<T>*       GetExternalSound     (void);
        wsp::sound::Midi*    GetExternalMidi      (void);


        // Mutators -------------------------
        void set_id              ( s32 id            ){ id_   = id; }
        void set_src_material_id ( s32 id            ){ src_material_id_ = id; }
        void set_name            ( const char *name  ){ name_ = std::string( name ); }
        void set_translate_x     ( float translate_x ){ params_.translate_x = translate_x; }
        void set_translate_y     ( float translate_y ){ params_.translate_y = translate_y; }
        void set_anchor_pt_x     ( float anchor_pt_x ){ params_.anchor_pt_x = anchor_pt_x; }
        void set_anchor_pt_y     ( float anchor_pt_y ){ params_.anchor_pt_y = anchor_pt_y; }
        void set_scale_x         ( float scale_x     ){ params_.scale_x     = scale_x; }
        void set_scale_y         ( float scale_y     ){ params_.scale_y     = scale_y; }
        void set_rotate_x        ( float rotate_x    ){ params_.rotate_x    = rotate_x; }
        void set_rotate_y        ( float rotate_y    ){ params_.rotate_y    = rotate_y; }
        void set_opacity         ( float opacity     ){ params_.opacity     = opacity; }
        void set_start_frame     ( int   start_frame ){ params_.start_frame = start_frame; }
        void set_time_strech     ( float time_strech ){ params_.time_strech = time_strech; }
        void set_visibility      ( bool visibility   ){ params_.visibility  = visibility; }
        void set_mode            ( wsp::img::ImageBlendMode  mode ){ params_.mode = mode; }
        void set_params          ( wsp::CompLayerParam &params ){ params_ = params; }


        /** @brief Translate layer */
        void Translate( float x, float y ){ params_.translate_x+=x; params_.translate_y+=y; }

        /** @brief Set compostion parameter */
        void SetCompLayerParam( wsp::CompLayerParam &param );

        // functions for setting external data
        void SetExternalImage   (wsp::Image<T>    *image_data );
        void SetExternalImageSeq(wsp::ImageSeq<T> *image_seq_data );
        void SetExternalSound   (wsp::Sound<T>    *sound_data );
        void SetExternalMidi    (wsp::sound::Midi *midi_data );

        void AddImageFilter( wsp::ImageFilterPluginInterface<T>* image_filter_plugin );
        /** 
         * @brief Just take a target filter, do not delete. 
         * @param take_num The number of filters to take.
         */
        s32 TakeImageFilter( const wsp::ImageFilterPluginInterface<T>* target_filter, s32 take_num = 1 );

        /* @brief Release allocated data memory when current data is own data. */
        void ReleaseDataMemory(void);
        void Reset(void);

        /* I/O */
        /**
         * @brief Save layer data into storage. 
         *        The file format is automatically decided from extentsion of input file name. 
         */
        wsp::Result Save( const char *filepath );

    private:
        CompositionLayer( const wsp::mm::CompositionLayer<T> &rhs ){}
        wsp::mm::CompositionLayer<T>& operator=( const wsp::mm::CompositionLayer<T> &rhs ){ return *this; }

    private:
        /** Layer ID. */
        s32                                      id_;
        s32                                      src_material_id_;
        std::string                              name_;
        wsp::CompLayerParam                      params_;
        wsp::Vector<wsp::ImageFilter<T>*>        effect_list_;

        /** current data type. */
        wsp::mm::MediaType               data_type_;

        /** media data itself. */
        union {
            u8               *as_byte_ptr;
            wsp::Image<T>    *as_image_ptr;
            wsp::ImageSeq<T> *as_image_seq_ptr;
            wsp::Sound<T>    *as_sound_ptr;
            wsp::sound::Midi *as_midi_ptr;
        } data_;

        bool has_own_data_;


    };
}}


#include "tpl_cl-compositionlayer.inl"

#endif
