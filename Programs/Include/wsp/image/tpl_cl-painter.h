/**
 * @file tpl_cl-painter.h
 * 
 */

#ifndef _WSP_IMAGE_TEMPLATE_CLASS_PAINTER_H_
#define _WSP_IMAGE_TEMPLATE_CLASS_PAINTER_H_

#ifndef __WSP_COMMONCORE_RESULT_H__
#include <wsp/common/result.h>
#endif
#include "tpl_cl-image.h"
#include <wsp/common/tpl_cl-vector3.h>

#define INIT_WSP_PAINTER image_(NULL)

namespace wsp{

    /**
     * @brief Painter class provides functionality for painting.
     */
    template<typename _Type> 
    class Painter
    {
    public:
        Painter(wsp::Image<_Type> *in_image);

        /*----------------
           Accessors
          ----------------*/
        const wsp::Image<_Type>* image()           const{ return image_; }
        wsp::Color<_Type>        fill_color()      const{ return fill_color_; }
        wsp::Color<_Type>        edge_color()      const{ return edge_color_; }
        wsp::Color<_Type>        background_color()const{ return background_color_; }
        u32                      edge_width()      const{ return edge_width_; }
        f32                      opacity   ()      const{ return opacity_; }

        /*----------------
           Mutators
          ----------------*/
        void set_image           ( wsp::Image<_Type> *image           ){ if(image==NULL){ return; } image_ = image; }
        void set_fill_color      ( wsp::Color<_Type> color            ){ fill_color_ = color; }
        void set_edge_color      ( wsp::Color<_Type> color            ){ edge_color_ = color; }
        void set_background_color( wsp::Color<_Type> background_color ){ background_color_ = background_color_; }
        void set_edge_width      ( u32 edge_width                     ){ edge_width_ = edge_width; }
        void set_opacity         ( f32 opacity                        ){ if( 0.0f<=opacity && opacity<=1.0f ){ opacity_ = opacity; } }

        void Translate( f32 x, f32 y, f32 z ){ px_+=x; py_+=y; pz_+=z; }
        void ClearCanvas   ();

        wsp::Result DrawLine   ( s32 start_x,  s32 start_y,  s32 end_x, s32 end_y);
        wsp::Result DrawEclipse( s32 center_x, s32 center_y, s32 eclipse_width, s32 eclipse_height);
        wsp::Result DrawCircle ( s32 center_x, s32 center_y, f32 radius );

    private:
        f32                px_, py_, pz_;
        u32                edge_width_;
        f32                opacity_;
        wsp::Image<_Type>  *image_;
        wsp::Color<_Type>  fill_color_;
        wsp::Color<_Type>  edge_color_;
        wsp::Color<_Type>  background_color_;

    };
}

#include "tpl_cl-painter.inl"

#endif

