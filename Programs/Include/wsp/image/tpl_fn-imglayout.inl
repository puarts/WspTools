/**
 * @file _tpl_fn-imglayout.inl
 * 
 */

#ifndef _WSP_TEMPLATE_FN_IMG_LAYOUT_INL_
#define _WSP_TEMPLATE_FN_IMG_LAYOUT_INL_

#ifndef __WSP_COMMONCORE_RESULT_H__
#include <wsp/common/result.h>
#endif
#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#include <wsp/common/tpl_cl-vector.h>
#endif
#include "tpl_cl-image.h"
#include "_tpl_fn-imgproc.hpp"

#include "tpl_fn-imglayout.hpp"

namespace {

}

template<typename _Type>
void wsp::img::LayoutImagesAnyColumns(
    wsp::Image<_Type> &o_image,
    const wsp::Vector<wsp::Image<_Type>> &in_imagelist,
    u32 layout_unit_width, 
    u32 layout_unit_height,
    u32 num_columns
)
{
    int num_images = in_imagelist.length();
    if( num_images==0 ){ return; }

    wsp::Vector<wsp::Image<_Type>> tmp_imagelist;
    tmp_imagelist.SetLength( num_images );

    int num_rows = (num_images+1) / num_columns;
    int width    = (int)layout_unit_width * num_columns;
    int height   = (int)layout_unit_height * num_rows;
    int nc       = (int)in_imagelist[0].num_of_channels();
    o_image.SetSize( width, height, nc );
    o_image.Reset( 0 );

    char tmp_name[MAX_LENGTH_OF_PATH];
    for( int row=0; row<num_rows; ++row )
    {
        for( int col=0; col<num_columns; ++col )
        {
            int img_index = row * num_columns + col;
            wsp::Image<_Type>* current_image = &tmp_imagelist[img_index];
            current_image->SetSize( layout_unit_width, layout_unit_height, nc );

            //sprintf( tmp_name, "layouttest_src_%2d.jpg", img_index );
            //in_imagelist[img_index].Save( tmp_name );

            //sprintf( tmp_name, "layouttest_tmp_%2d.jpg", img_index );
            //current_image->Save( tmp_name );

            int src_w = in_imagelist[img_index].width();
            int src_h = in_imagelist[img_index].height();
            wsp::img::ScaleImageNearestNeighbor(
                in_imagelist[img_index].data(), 
                src_w, src_h, nc,
                current_image->data(), 
                layout_unit_width, layout_unit_height
            );
            //sprintf( tmp_name, "layouttest_tmp_resize_%2d.jpg", img_index );
            //current_image->Save( tmp_name );

            int dst_offset_x = col * layout_unit_width;
            int dst_offset_y = row * layout_unit_height;
            for( int y=0, dst_y=dst_offset_y; y<layout_unit_height; ++y, ++dst_y )
            {
                for( int x=0, dst_x=dst_offset_x; x<layout_unit_width; ++x, ++dst_x )
                {
                    int i     = (y * layout_unit_width + x) * nc;
                    int dst_i = (dst_y * width + dst_x) * nc;
                    for( int c=0; c<nc; ++c )
                    {
                        o_image[ dst_i + c ] = (*current_image)[ i + c ];
                    }
                }
            }
        }
    }
}


#endif
