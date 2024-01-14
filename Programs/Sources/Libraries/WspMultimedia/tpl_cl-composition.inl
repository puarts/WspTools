/**
 * @file tpl_cl-composition.inl
 * @author Jun-ichi Nishikata
 */

#ifndef __WSP_MULTIMEDIA_TEMPLATE_CLASS_COMPOSITION_INL__
#define __WSP_MULTIMEDIA_TEMPLATE_CLASS_COMPOSITION_INL__

#ifndef __WSP_COMMONCORE_RESULT_H__
#include <wsp/common/result.h>
#endif
#ifndef __WSP_COMMONUTIL_FN_UTIL_H__
#include <wsp/common/fn-util.h>
#endif
#include <wsp/image/_tpl_fn-imgblend.hpp>

#include "fn-mediautil.h"
#include "tpl_cl-composition.h"
#include "fn-multimedia_log.h"


#define DEBUG_WSP_COMPOSITION 0


// Constructor, Destructor -----------------

template<typename T>
wsp::mm::Composition<T>::Composition()
    : layer_list_(new wsp::Vector<wsp::mm::CompositionLayer<T>*>())
    , param_( WSP_DEFAULT_COMPOSITION_PARAM )
    , bg_color_( wsp::Color<T>(0, 0, 255) )
#ifdef USE_QT
    , progress_bar_( NULL )
#endif
{ 
}

template<typename T>
wsp::mm::Composition<T>::Composition(wsp::mm::CompositionParam &params, wsp::Color<T> &bg_color)
    : wsp::mm::Composition()
{ 
    param_ = params; 
    bg_color_ = bg_color;
}

template<typename T>
wsp::mm::Composition<T>::~Composition()
{
    delete layer_list_; 
}

namespace{
    const f32 PI = 3.14159265358979323846;
    inline bool IsOnLayer( int x, int y, int width, int height, const wsp::CompLayerParam &param )
    {
        float centered_x, centered_y;
        int src_x, src_y;

        float rotate_rad = param.rotate_x * PI / 180.0f;
        float sx_mult    = 1.0f / param.scale_x;
        float sy_mult    = 1.0f / param.scale_y;
        float tx         = param.translate_x;
        float ty         = param.translate_y;
        float ax         = param.anchor_pt_x + tx;
        float ay         = param.anchor_pt_y + ty;

        centered_x = x - ax;
        centered_y = y - ay;

        src_x = (int)( (centered_x * cos(rotate_rad) - centered_y * sin(rotate_rad)) * sx_mult - tx + ax );
        src_y = (int)( (centered_x * sin(rotate_rad) + centered_y * cos(rotate_rad)) * sy_mult - ty + ay );

        return 0<=src_x && src_x<width && 0<=src_y && src_y<height;
    }
}

//#define DEBUG_GetTopLayerOnPoint
template<typename T>
wsp::mm::CompositionLayer<T>* wsp::mm::Composition<T>::GetTopLayerOnPoint( int x, int y )
{
    int num_layers = layer_list_->length();
    for( int i=num_layers-1; i>=0; --i )
    {
        wsp::mm::CompositionLayer<T> *layer = (*layer_list_)[i];
        wsp::CompLayerParam* param = layer->param_data();

        
        switch( layer->data_type() )
        {
        case wsp::mm::MEDIATYPE_IMAGE:
            {
                const wsp::Image<T>* image = layer->image_data();
                if( IsOnLayer(x, y, image->width(), image->height(), *param ) )
                {
#ifdef DEBUG_GetTopLayerOnPoint
                    WSP_COMMON_DEBUG_LOG("%d: IsOnLayer() = true\n", i);
#endif
                    return layer;
                }
            }
            break;
        case wsp::mm::MEDIATYPE_IMAGE_SEQUENCE:
            {
                const wsp::ImageSeq<T>* image_seq = layer->image_seq_data();
                if( image_seq->SeqSize()!=0 )
                {
                    if( IsOnLayer(x, y, image_seq->width(), image_seq->height(), *param ) )
                    {
                        return layer;
                    }
                }
            }
            break;
        }
#ifdef DEBUG_GetTopLayerOnPoint
        WSP_COMMON_DEBUG_LOG("%d: IsOnLayer() = false\n", i);
#endif
    }
    return NULL;
}


/* accessor */
template<typename T>
wsp::Result wsp::mm::Composition<T>::GetCombinedImage(wsp::Image<T> *o_dst_img, int frame) const
{
    o_dst_img->SetSize(param_.width, param_.height, param_.num_channels);

    int num_channels = 3;
    o_dst_img->ResetElems( bg_color_.data(), num_channels*sizeof(T) );

    {
        wsp::mm::CompositionLayer<T> **layer_ptr = layer_list_->data();
        wsp::mm::CompositionLayer<T> **layer_end_ptr = layer_ptr + layer_list_->length();

        /* ------------------------------------
                    Iterate layers
           ------------------------------------*/
        for(; layer_ptr!=layer_end_ptr; ++layer_ptr)
        {
            if( (*layer_ptr)->visibility() == false )
            {
                continue;
            }

            wsp::Image<T>* src_img;
            wsp::Image<T>  effect_img;
            wsp::Result result;

            s32 start_frame = (*layer_ptr)->start_frame();
            wsp::mm::MediaType data_type = (*layer_ptr)->data_type();

            effect_img.SetSize( param_.width, param_.height, param_.num_channels );
            switch(data_type)
            {
            case wsp::mm::MEDIATYPE_IMAGE:
                {
                    result = (*layer_ptr)->GetOutputImage( &effect_img );
                    if ( result != wsp::ResultSuccess() )
                    {
                        return result;
                    }
                }
                break;
            case wsp::mm::MEDIATYPE_IMAGE_SEQUENCE:
                {
                    s32 seq_frame = frame - start_frame;
                    result = (*layer_ptr)->GetOutputImageSeqFrame( &effect_img, seq_frame );
                    if ( result != wsp::ResultSuccess() )
                    {
                        return result;
                    }
                }
                break;
            case wsp::mm::MEDIATYPE_VIDEO:
                WSP_COMMON_WARNING_LOG("video type has yet to be implemented, the layer skipped\n");
                continue;
            case wsp::mm::MEDIATYPE_SOUND:
                WSP_COMMON_WARNING_LOG("sound type has yet to be implemented, the layer skipped\n");
                continue;
            case wsp::mm::MEDIATYPE_COMPOSITION:
                WSP_COMMON_WARNING_LOG("composition type has yet to be implemented, the layer skipped\n");
                continue;
            default:
                WSP_COMMON_WARNING_LOG("Unsupported media data type %d, the layer skipped\n", data_type);
                continue;
            }

            float src_ap_x, src_ap_y;
            float src_tx, src_ty;
            float offset_x, offset_y;

            int src_w = effect_img.width();
            int src_h = effect_img.height();
            int src_nc= effect_img.num_of_channels();

            int dst_w = param_.width;
            int dst_h = param_.height;
            int dst_nc= param_.num_channels;

            if(dst_nc!=src_nc)
            {
                WSP_COMMON_WARNING_LOG("Number of channels for source and destination image must be the same, skipped\n");
                continue;
            }
            u32 w  = src_w;
            u32 h  = src_h;
            u32 nc = src_nc;


#define USE_OPTIMIZED_CODE_GET_COMBINED_IMAGE
#ifndef USE_OPTIMIZED_CODE_GET_COMBINED_IMAGE
            /* Unoptimized code */
            {
                float transmittance;
                int x, y, c, i;
                int src_x, src_y, src_i, alpha_i;

                T *dst_ptr, *dst_end_ptr;
                T *src_ptr;
                src_ptr = effect_img.data();
                uchar *alpha_ptr;
                dst_ptr   = o_dst_img->data();
                alpha_ptr = effect_img.alpha_data();

                for( y=0, src_y=0; y<dst_h; ++y, ++src_y )
                {
                    for( x=0, src_x=0; x<dst_w; ++x, ++src_x )
                    {
                        /* Continue if current point is out of image */
                        if( src_y<0 || src_y>=src_h 
                         || src_x<0 || src_x>=src_w
                        ){
                            continue;
                        }

                        i             = (y*dst_w + x)*dst_nc;
                        src_i         = (src_y*src_w + src_x)*src_nc;
                        alpha_i       = src_y*src_w + src_x;
                        transmittance = alpha_ptr[alpha_i]/255.0f;
                        for( c=0; c<param_.num_channels; ++c )
                        {
                            dst_ptr[i+c] = dst_ptr[i+c] * (1.0f-transmittance) + src_ptr[src_i+c] * transmittance;
                        }
                    }
                }
            }
#else
            /* Optimized code */
            {
                float transmittance;
                T *dst_ptr, *dst_end_ptr;
                T *src_ptr;
                uchar *alpha_ptr;
                src_ptr = effect_img.data();
                dst_ptr   = o_dst_img->data();
                alpha_ptr = effect_img.alpha_data();

                dst_ptr = o_dst_img->data();
                dst_end_ptr = dst_ptr + o_dst_img->width()*o_dst_img->height()*o_dst_img->num_of_channels();
                
                T *c_ep;

                int nc = dst_nc ;
                switch( (*layer_ptr)->mode() )
                {
                case WSP_IMAGE_BLEND_MODE_NORMAL:     wsp::img::BlendNormal    ( o_dst_img->data(), effect_img.data(), effect_img.alpha_data(), w*nc, h, nc ); break;
                case WSP_IMAGE_BLEND_MODE_DARKEN:     wsp::img::BlendDarken    ( o_dst_img->data(), effect_img.data(), effect_img.alpha_data(), w*nc, h, nc ); break;
                case WSP_IMAGE_BLEND_MODE_MULTIPLY:   wsp::img::BlendMultiply  ( o_dst_img->data(), effect_img.data(), effect_img.alpha_data(), w*nc, h, nc ); break;
                case WSP_IMAGE_BLEND_MODE_BURN:       wsp::img::BlendBurn      ( o_dst_img->data(), effect_img.data(), effect_img.alpha_data(), w*nc, h, nc ); break;
                case WSP_IMAGE_BLEND_MODE_LIGHTEN:    wsp::img::BlendLighten   ( o_dst_img->data(), effect_img.data(), effect_img.alpha_data(), w*nc, h, nc ); break;
                case WSP_IMAGE_BLEND_MODE_ADD:        wsp::img::BlendAdd       ( o_dst_img->data(), effect_img.data(), effect_img.alpha_data(), w*nc, h, nc ); break;
                case WSP_IMAGE_BLEND_MODE_SCREEN:     wsp::img::BlendScreen    ( o_dst_img->data(), effect_img.data(), effect_img.alpha_data(), w*nc, h, nc ); break;
                case WSP_IMAGE_BLEND_MODE_DODGE:      wsp::img::BlendDodge     ( o_dst_img->data(), effect_img.data(), effect_img.alpha_data(), w*nc, h, nc ); break;
                case WSP_IMAGE_BLEND_MODE_OVERLAY:    wsp::img::BlendOverlay   ( o_dst_img->data(), effect_img.data(), effect_img.alpha_data(), w*nc, h, nc ); break;
                case WSP_IMAGE_BLEND_MODE_SOFTLIGHT:  wsp::img::BlendSoftLight ( o_dst_img->data(), effect_img.data(), effect_img.alpha_data(), w*nc, h, nc ); break;
                case WSP_IMAGE_BLEND_MODE_HARDLIGHT:  wsp::img::BlendHardLight ( o_dst_img->data(), effect_img.data(), effect_img.alpha_data(), w*nc, h, nc ); break;
                case WSP_IMAGE_BLEND_MODE_SUBTRACT:   wsp::img::BlendSubtract  ( o_dst_img->data(), effect_img.data(), effect_img.alpha_data(), w*nc, h, nc ); break;
                case WSP_IMAGE_BLEND_MODE_DIFFERENCE: wsp::img::BlendDifference( o_dst_img->data(), effect_img.data(), effect_img.alpha_data(), w*nc, h, nc ); break;
                case WSP_IMAGE_BLEND_MODE_DIVIDE:     wsp::img::BlendDivide    ( o_dst_img->data(), effect_img.data(), effect_img.alpha_data(), w*nc, h, nc ); break;
                case WSP_IMAGE_BLEND_MODE_EXCLUSION:  wsp::img::BlendExclusion ( o_dst_img->data(), effect_img.data(), effect_img.alpha_data(), w*nc, h, nc ); break;
                default: continue;
                }
            }
#endif
        }
    }

    return wsp::ResultSuccess();
}

// Mutators --------------------------------
template<typename T>
wsp::Result wsp::mm::Composition<T>::set_name( const char* name )
{
    if(strlen(name)>=MAX_LENGTH_OF_NAME)
    { 
        return wsp::ResultInvalidArgument(); 
    } 
    strcpy( param_.name, name );
    return wsp::ResultSuccess();
}

template<typename T>
void wsp::mm::Composition<T>::CopyLayerListFrom( const wsp::Vector<wsp::mm::CompositionLayer<T>*> &layer_list )
{
    *layer_list_ = layer_list;
}

// I/O -------------------------------------

template<typename T>
wsp::Result wsp::mm::Composition<T>::Render( const RenderParam &render_param )
{
    WSP_COMMON_DEBUG_LOG(
        "    render_path:  %s\n"
        "    width:        %u\n"
        "    height:       %u\n"
        "    num_channels: %u\n"
        "    num_frames:   %u\n"
        "    fps:          %f\n"
        , render_param.render_path
        , render_param.width
        , render_param.height
        , render_param.num_channels
        , render_param.num_frames
        , render_param.fps
    );

    wsp::State state = SaveAsImageSequence( render_param.render_path );
    if( state != WSP_STATE_SUCCESS )
    {
        return wsp::ResultStatusError();
    }

    return wsp::ResultSuccess();
}

template<typename T>
wsp::State wsp::mm::Composition<T>::SaveFrameAsImage( s32 frame, const char *filename )
{
    wsp::Image<T> tmp_img;
    char tmp_path[MAX_LENGTH_OF_PATH];
    char tmp_dir[MAX_LENGTH_OF_PATH];
    char tmp_name[MAX_LENGTH_OF_NAME];
    char tmp_ext[MAX_LENGTH_OF_NAME];

    wsp::GetDir(tmp_dir, filename);
    wsp::GetName(tmp_name, filename);
    wsp::GetExtension(tmp_ext, filename);

    GetCombinedImage(&tmp_img, frame);
    sprintf(tmp_path, "%s/%s%d.%s", tmp_dir, tmp_name, frame, tmp_ext);
    tmp_img.Save(tmp_path);

    return WSP_STATE_SUCCESS;
}

template<typename T>
wsp::State wsp::mm::Composition<T>::SaveAsImageSequence( const char *filename )
{
    WSP_COMMON_DEBUG_LOG("\n");
    int progress = 0;
#ifdef USE_QT
    if( progress_bar_!=NULL )
    {
        progress_bar_->setMaximum( param_.num_frames );
        progress_bar_->setValue( 0 );
    }
#endif
    wsp::StartTimer();
    int num_frames = (s32) param_.num_frames;
    {
        wsp::Image<T> tmp_img;
        char tmp_path[MAX_LENGTH_OF_PATH];

        wsp::SeqInfo seqinfo;
        wsp::GetSeqInfo( seqinfo, filename );

        char fmt[32];
        sprintf( fmt, "%%s/%%s%%0%dd.%%s", seqinfo.num_padding );
        /**
         * This loop has no benefit for parallelization.
         */
        for( int frame=0; frame<num_frames; ++frame )
        {
            GetCombinedImage(&tmp_img, frame);

            sprintf(tmp_path, fmt, seqinfo.dir, seqinfo.name, frame, seqinfo.ext);
            tmp_img.Save(tmp_path);

            ++progress;
            WSP_COMMON_LOG("Render Progress: %u/%u (%3d %%)\n", frame, num_frames, progress/(f32)num_frames );
            #ifdef USE_QT
            if( progress_bar_!=NULL )
            {
                progress_bar_->setValue(progress);
            }
            #endif

        }
    }
    f32 render_time = wsp::StopTimer();
    WSP_COMMON_LOG( "    Render Time: %f\n", render_time );

#ifdef USE_QT
    if( progress_bar_!=NULL )
    {
        progress_bar_->setValue( 0 );
    }
#endif

    return WSP_STATE_SUCCESS;
}

/* operators */
template<typename T>
wsp::mm::Composition<T>& wsp::mm::Composition<T>::operator=( const wsp::mm::Composition<T> &rhs )
{
    if( layer_list_ != rhs.layer_list() )
    {
        this->CopyLayerListFrom( *rhs.layer_list() );
    }
    if( bg_color_ != rhs.bg_color() )
    {
        bg_color_ = rhs.bg_color();
    }
    if( param_ != rhs.params() )
    {
        param_ = rhs.params();
    }
    return *this;
}
template<typename T>
bool wsp::mm::Composition<T>::operator==( const wsp::mm::Composition<T> &rhs ) const
{
    return layer_list_ == rhs.layer_list() 
        && bg_color_ == rhs.bg_color()
        && param_ == rhs.params();
}
template<typename T> 
bool wsp::mm::Composition<T>::operator!=( const wsp::mm::Composition<T> &rhs ) const
{
    return !(*this==rhs);
}



#endif
