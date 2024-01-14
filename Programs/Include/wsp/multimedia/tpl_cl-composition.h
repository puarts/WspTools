/**
 * @file tpl_cl-composition.h
 * @author Jun-ichi Nishikata
 * @brief Declaration of wsp::Compostion class.
 */

#ifndef __WSP_MULTIMEDIA_TEMPLATE_CLASS_COMPOSITION_H__
#define __WSP_MULTIMEDIA_TEMPLATE_CLASS_COMPOSITION_H__

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include <wsp/common/_define_commoncore.h>
#endif

#ifdef USE_QT
#include <QtGui/QtGui>
#endif

#include <wsp/common/tpl_cl-vector3.h>
#include "tpl_cl-compositionlayer.h"

/** @brief  composition parameter struct */
struct WSP_CompositionParam{
    char name[MAX_LENGTH_OF_NAME];
    u32 width;
    u32 height;
    u32 num_channels;
    u32 num_frames;
    f32 fps;
#ifdef __cplusplus
    inline WSP_CompositionParam& operator= ( const WSP_CompositionParam &rhs );
    inline bool                  operator==( const WSP_CompositionParam &rhs )const;
    inline bool                  operator!=( const WSP_CompositionParam &rhs )const;
#endif
};

static const WSP_CompositionParam WSP_DEFAULT_COMPOSITION_PARAM = {
                                                        "null", 
                                                        1, 1, 
                                                        3, 1, 
                                                        24.0
                                                    };


inline void WSP_CopyCompositionParam( 
    struct WSP_CompositionParam &lhs, const struct WSP_CompositionParam &rhs 
){
    memcpy( &lhs, &rhs, sizeof( struct WSP_CompositionParam ) );
}
inline bool WSP_IsEqualToCompositionParam( 
    const struct WSP_CompositionParam &lhs, const struct WSP_CompositionParam &rhs 
){
    return memcmp( &lhs, &rhs, sizeof(WSP_CompositionParam) )==0;
}

#ifdef __cplusplus
WSP_CompositionParam& WSP_CompositionParam::operator= ( const WSP_CompositionParam &rhs )
{
    WSP_CopyCompositionParam( *this, rhs );
    return *this;
}
bool WSP_CompositionParam::operator==( const WSP_CompositionParam &rhs )const
{
    return WSP_IsEqualToCompositionParam( *this, rhs );
}
bool WSP_CompositionParam::operator!=( const WSP_CompositionParam &rhs )const
{
    return !( *this == rhs );
}
#endif


/** @brief  Rendering parameter struct */
struct WSP_RenderParam{
    char render_path[MAX_LENGTH_OF_PATH];
    u32  width;
    u32  height;
    u32  num_channels;
    u32  num_frames;
    f32  fps;
};

#ifdef __cplusplus
namespace wsp{ namespace mm{

    typedef struct WSP_CompositionParam CompositionParam;
    typedef struct WSP_RenderParam RenderParam;

}}
#endif

namespace wsp{ namespace mm{
    /**
     * @brief template class for composition.
     */
    template<typename T=u8>
    class Composition
    {
    public:
        Composition();
        Composition( const wsp::mm::Composition<T> &rhs ){ *this = rhs }
        explicit Composition( wsp::mm::CompositionParam &param, wsp::Color<T> &bg_color );
        ~Composition();

        /*--------------------------- 
           Accessors 
          ---------------------------*/
        wsp::Vector<wsp::mm::CompositionLayer<T>*>* layer_list(){ return layer_list_; }
        const wsp::Vector<wsp::mm::CompositionLayer<T>*>* layer_list()const{ return layer_list_; }
        wsp::mm::CompositionParam& params()     { return param_; }
        const wsp::mm::CompositionParam& params()const{ return param_; }

        const char*   name             ()const{ return param_.name; }
        int           width            ()const{ return param_.width; }
        int           height           ()const{ return param_.height; }
        int           num_channels     ()const{ return param_.num_channels; }
        int           num_frames       ()const{ return param_.num_frames; }
        int           num_layers       ()const{ return layer_list_->length(); }
        float         fps              ()const{ return param_.fps; }
        wsp::Color<T> bg_color         ()const{ return bg_color_; }
        float         GetDurationAsSec ()const{ return param_.num_frames/param_.fps; }

        wsp::Result               GetCombinedImage(wsp::Image<T> *o_dst_image, int frame) const;
        wsp::mm::CompositionLayer<T>* GetTopLayerOnPoint( int x, int y );

        /*--------------------------- 
           Mutators
          ---------------------------*/
        wsp::Result set_name        ( const char* name );
        void        set_width       ( u32 width                  ){ param_.width = width; }
        void        set_height      ( u32 height                 ){ param_.height = height; }
        void        set_num_channels( u32 num_channels           ){ param_.num_channels = num_channels; }
        void        set_fps         ( f32 fps                    ){ param_.fps = fps; }
        void        set_num_frames  ( u32 num_frames             ){ param_.num_frames = num_frames; }
        void        set_bg_color    ( wsp::Color<T> bg_color     ){ bg_color_ = bg_color; }
        void        set_param       (wsp::mm::CompositionParam &param){ param_ = param; }
        void        CopyLayerListFrom( const wsp::Vector<wsp::mm::CompositionLayer<T>*> &layer_list );

#ifdef USE_QT
        void set_progress_bar    ( QProgressBar *progress_bar ){ progress_bar_ = progress_bar; }
#endif


        void AddLayer   ( wsp::mm::CompositionLayer<T> *layer ){ layer_list_->Append(layer); }
        void ClearLayers()                                 { layer_list_->Clear(); }

        /*--------------------------- 
           I/O
          ---------------------------*/
        wsp::Result Render             ( const RenderParam &render_param );

        /** @brief  Save composition as image sequence */
        wsp::State  SaveAsImageSequence( const char *filename );

        /** @brief  Save a frame as image */
        wsp::State  SaveFrameAsImage   ( s32 frame, const char *filename );

        /*--------------------------- 
           Operators
          ---------------------------*/
        wsp::mm::Composition<T>& operator= ( const wsp::mm::Composition<T> &rhs );
        bool                 operator==( const wsp::mm::Composition<T> &rhs )const;
        bool                 operator!=( const wsp::mm::Composition<T> &rhs )const;

    private:
        wsp::mm::CompositionParam param_;
        wsp::Color<T> bg_color_;
        wsp::Vector<wsp::mm::CompositionLayer<T>*> *layer_list_;

#ifdef USE_QT
        QProgressBar *progress_bar_;
#endif
    };
}}

#include "tpl_cl-composition.inl"

#endif
