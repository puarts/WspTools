/**
 * @file tpl_cl-painter.inl
 * 
 */

#ifndef _WSP_IMAGE_TEMPLATE_CLASS_PAINTER_INL_
#define _WSP_IMAGE_TEMPLATE_CLASS_PAINTER_INL_

#include <math.h>

#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#include <wsp/common/tpl_cl-vector.h>
#endif
#include "tpl_cl-image.h"
#include "tpl_cl-painter.h"


template<typename T>
wsp::Painter<T>::Painter(wsp::Image<T> *in_image)
    : image_(NULL)
    , px_(0.0f), py_(0.0f), pz_(0.0f)
    , edge_width_(1)
    , opacity_(1.0f)
    , edge_color_( 0, 0, 0 )
    , fill_color_( 128, 128, 128 )
    , background_color_( 0xff, 0xff, 0xff )
{ 
    image_ = in_image; 
}

template<typename _Type>
void wsp::Painter<_Type>::ClearCanvas()
{
    image_->ResetElems( background_color_.data(), 3 );
}

template<typename T>
wsp::Result wsp::Painter<T>::DrawLine(s32 start_x, s32 start_y, s32 end_x, s32 end_y)
{
    f32 x, y;
    s32 c, i;
    s32 width = image_->width();
    s32 height = image_->height();
    s32 nc= image_->num_of_channels();
    s32 diff_x = end_x - start_x;
    s32 diff_y = end_y - start_y;

    if(diff_x==0 && diff_y==0)
    {
        WSP_COMMON_DEBUG_LOG("is dot\n");
        i = (start_y*width+start_x)*nc;
        for(c=0; c<nc; ++c){
            (*image_)[i + c] = edge_color_[c];
        }
    }
    else if(diff_x==0)
    {
        WSP_COMMON_DEBUG_LOG("is vertical line\n");
        x = start_x;
        for( y=start_y; y<=end_y; ++y )
        {
            i = (y*width+x)*nc;
            for(c=0; c<nc; ++c){
                (*image_)[i + c] = edge_color_[c];
            }
        }
    }
    else
    {
        f32 step = 1.0f;
        if(diff_y!=0.0f)
        {
            step = fabs(diff_x/(float)diff_y);
            if( step > 1.0f )
            {
                step = 1.0f;
            }

        }
        WSP_COMMON_LOG("step=%f\n", step);

        f32 slope = (start_y - end_y)/static_cast<f32>(start_x - end_x);
        f32 intercept = start_y - slope * start_x;

        f32 draw_start = start_x<end_x?start_x:end_x;
        f32 draw_end   = start_x<end_x?end_x:start_x;
        for( x=draw_start; x<draw_end; x+=step )
        {
            WSP_COMMON_LOG("x=%f\n", x);
            y = slope * x + intercept;
            s32 px = x;
            s32 py = y;
            bool is_out_of_image = (px<0 || width<=px) || (py<0 || height<=py);
            if( is_out_of_image  ){ continue; }

            i = (py*width+px) * nc;
            for(c=0; c<nc; ++c)
            {
                (*image_)[i + c] = edge_color_[c];
            }
        }
    }
    return wsp::ResultSuccess();
}

template<typename T> 
wsp::Result wsp::Painter<T>::DrawEclipse(s32 center_x, s32 center_y, s32 hor_rad, s32 ver_rad)
{
    if(image_==NULL){ 
        WSP_COMMON_WARNING_LOG("current image is NULL\n"); 
        return wsp::ResultInvalidArgument(); 
    }

    s32 width  = static_cast<s32>(image_->width());
    s32 height = static_cast<s32>(image_->height());
    s32 nc     = static_cast<s32>(image_->num_of_channels());
    f32 actual_center_x = px_ + center_x;
    f32 actual_center_y = py_ + center_y;
    {
        s32 x, y, i, c, px, py;
        //for( y=-b; y<=b; ++y )
        //{
        //    for( x=-a; x<=a; ++x )
        //    {
        //        px = center_x + x;
        //        py = center_y + y;
        //        bool is_out_of_image = (px<0 && width<=px) || (py<0 && height<=py);
        //        bool is_out_of_ellipse = (x*x/(f32)a*a + y*y/(f32)b*b ) > 1.0f;

        //        if( is_out_of_image && is_out_of_ellipse )
        //        { continue; }

        //        i = py*width + px;
        //        for(c=0; c<nc; ++c)
        //        {
        //            (*image_)[i*nc+c] = color_[c];
        //        }
        //    }
        //}
    }

    /* draw edge */
    {
        f32 x, y;
        s32 i, c, px, py;
        bool is_out_of_image;
        for( x=-hor_rad; x<=hor_rad; ++x )
        {
            y = ver_rad * sqrt( 1 - (x*x)/(hor_rad*hor_rad) );
            px = actual_center_x + x;
            py = actual_center_y + y;

            is_out_of_image = (px<0 || width<=px) || (py<0 || height<=py);
            if( is_out_of_image == false )
            {
                i = (py*width + px)*nc;
                for(c=0; c<nc; ++c)
                {
                    f32 bg_val = (*image_)[i+c];
                    (*image_)[i+c] =  bg_val         * ( 1.0f - opacity_ ) 
                                    + edge_color_[c] * ( opacity_        );
                }
            }

            py = actual_center_y - y;

            is_out_of_image = (px<0 || width<=px) || (py<0 || height<=py);
            if( is_out_of_image == false )
            {
                i = (py*width + px)*nc;
                for(c=0; c<nc; ++c)
                {
                    f32 bg_val = (*image_)[i+c];
                    (*image_)[i+c] =  bg_val         * ( 1.0f - opacity_ ) 
                                    + edge_color_[c] * ( opacity_        );
                }
            }
        }
    }
    return wsp::ResultSuccess();
}


template<typename T> 
wsp::Result wsp::Painter<T>::DrawCircle(s32 center_x, s32 center_y, f32 radius )
{
    if(image_==NULL)
    { 
        return wsp::ResultInvalidParameter(); 
    }
    if( radius <= 0.0f || opacity_ < 0 || 1.0f < opacity_ )
    {
        return wsp::ResultInvalidArgument();
    }

    
    s32 width  = static_cast<s32>(image_->width());
    s32 height = static_cast<s32>(image_->height());
    s32 nc     = static_cast<s32>(image_->num_of_channels());
    f32 rad_seq = radius*radius;
    f32 half_edge_width = edge_width_ * 0.5f;
    f32 antialias_offset = 0.0f;

    f32 actual_center_x = px_ + center_x;
    f32 actual_center_y = py_ + center_y;

    s32 draw_width = radius + antialias_offset;

    /* fill circle */
    {
        s32 x, y, i, c, px, py;

        for( y=-draw_width; y<=draw_width; ++y )
        {
            for( x=-draw_width; x<=draw_width; ++x )
            {
                px = actual_center_x + x;
                py = actual_center_y + y;
                bool is_out_of_image = (px<0 || width<=px) || (py<0 || height<=py);
                if( is_out_of_image  ){ continue; }

                f32 dist_from_center = sqrt( (f32)(x*x + y*y) );
                f32 diff_dist = radius - dist_from_center;
                bool is_out_of_circle =  diff_dist + antialias_offset < 0.0f;
                if( is_out_of_circle ){ continue; }

                i = (py*width + px)*nc;

                bool is_antialias_area = diff_dist < 0.0f;
                if( is_antialias_area )
                {
                //    /* calculate cross point between circle edge and linear line from center */
                //    f32 gradients  = (py-actual_center_y)/(px-actual_center_x);
                //    f32 intercepts = py - px*gradients;
                //    // radius*radius = x*x + (gradients*x+intercepts)*(gradients*x+intercepts)
                //    // x*x + gradients*gradients*x*x + 2*gradients*intercepts*x + intercepts*intercepts - radius*radius = 0
                //    // (1 + gradients*gradietns)*x*x + 2*gradients*intercepts*x + intercepts*intercepts - radius*radius = 0
                //    f32 a = (1 + gradients*gradients);
                //    f32 b = 2*gradients*intercepts;
                //    f32 c = intercepts*intercepts - radius*radius;
                //    f32 bb_4ac = b*b-4*a*c;
                //    f32 actual_x = (-b + sqrt(bb_4ac)) / (2*a); 
                //    f32 actual_y = sqrt( radius*radius - actual_x*actual_x );

                //    WSP_COMMON_LOG("gradients = %f, intercepts = %f\n", gradients, intercepts );
                //    WSP_COMMON_LOG("b*b = %f, -4*a*c = %f\n", b*b, -4*a*c );
                //    WSP_COMMON_LOG("a = %f, b = %f, bb-4ac = %f\n", a, b, bb_4ac );
                //    WSP_COMMON_LOG("actual_x = %f, px = %f\n", actual_x, px );
                //    WSP_COMMON_LOG("actual_y = %f, py = %f\n", actual_y, py );
                //    
                    for( c=0; c<nc; ++c )
                    {
                        f32 bg_val = (*image_)[i+c];
                        f32 antialias_opacity_ = opacity_*0.5f;
                        (*image_)[i+c] =  bg_val         * ( 1.0f - antialias_opacity_ ) 
                                        + fill_color_[c] * ( antialias_opacity_ );
                    }
                }
                else
                {
                    for(c=0; c<nc; ++c)
                    {
                        f32 bg_val = (*image_)[i+c];
                        (*image_)[i+c] =  bg_val         * ( 1.0f - opacity_ ) 
                                        + fill_color_[c] * ( opacity_        );
                    }
                }
            }
        }
    }

    /* draw edge */
    {
        f32 x, y;
        s32 i, c, px, py;
        bool is_out_of_image;
        f32 x_step = 0.01f;
        for( x=-radius; x<=radius; x+=x_step )
        {
            y = sqrt( radius*radius - x*x );
            px = actual_center_x + x;
            py = actual_center_y + y;

            is_out_of_image = (px<0 || width<=px) || (py<0 || height<=py);
            if( is_out_of_image == false )
            {
                i = (py*width + px)*nc;
                for(c=0; c<nc; ++c)
                {
                    f32 bg_val = (*image_)[i+c];
                    (*image_)[i+c] =  bg_val         * ( 1.0f - opacity_ ) 
                                    + edge_color_[c] * ( opacity_        );
                }
            }

            py = actual_center_y - y;

            is_out_of_image = (px<0 || width<=px) || (py<0 || height<=py);
            if( is_out_of_image == false )
            {
                i = (py*width + px)*nc;
                for(c=0; c<nc; ++c)
                {
                    f32 bg_val = (*image_)[i+c];
                    (*image_)[i+c] =  bg_val         * ( 1.0f - opacity_ ) 
                                    + edge_color_[c] * ( opacity_        );
                }
            }
        }    


        //for( y=-draw_width; y<=draw_width; ++y )
        //{
        //    for( x=-draw_width; x<=draw_width; ++x )
        //    {
        //        px = px_ + center_x + x;
        //        py = py_ + center_y + y;
        //        bool is_out_of_image = (px<0 || width<=px) || (py<0 || height<=py);
        //        if( is_out_of_image  ){ continue; }

        //        f32 dist_from_center = sqrt( (f32)(x*x + y*y) );
        //        f32 diff_dist = radius - dist_from_center;
        //        f32 edge_offset = antialias_offset + half_edge_width;
        //        /* 
        //         * out of circle: diff_dist <= -antialias_offset - half_edge_width
        //         * on line:       -1.0f-half_edge_width < diff_dist < 1.0f+half_edge_width 
        //         * in circle:     1.0f+edge_width_ <= diff_dist
        //         */
        //        bool is_out_of_circle =  diff_dist < -edge_offset;  
        //        if( is_out_of_circle ){ continue; }

        //        bool is_in_circle = edge_offset <= diff_dist;
        //        if( is_in_circle ==false )
        //        {
        //            //f32 antialias_mult = 1.0f;
        //            //if( half_edge_width < fabs(diff_dist) && fabs(diff_dist) < half_edge_width + antialias_offset )
        //            //{
        //            //    antialias_mult -= fabs(diff_dist);
        //            //}
        //            //i = (py*width + px)*nc;
        //            //for(c=0; c<nc; ++c)
        //            //{
        //            //    f32 bg_val = (*image_)[i+c];
        //            //    (*image_)[i+c] = (  bg_val         * ( 1.0f - opacity_ + 1.0f - antialias_mult ) 
        //            //                      + edge_color_[c] * ( opacity_        + antialias_mult        ) ) * 0.5f;
        //            //}
        //        }
        //    }
        //}
    }
    return wsp::ResultSuccess();
}


#endif
