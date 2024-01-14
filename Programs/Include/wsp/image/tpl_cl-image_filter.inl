/**
 * @file cl-image_filter.inl
 * 
 */

#ifndef WSP_IMAGE_TEMPLATE_CLASS_IMAGE_FILTER_INL_
#define WSP_IMAGE_TEMPLATE_CLASS_IMAGE_FILTER_INL_

#include <wsp/common/_cl-std_param.h>
#include "tpl_cl-image_filter.hpp"

template<typename T>
wsp::ImageFilter<T>::ImageFilter( wsp::ImageFilterPluginInterface<T>* image_filter_interface )
    : enable_filter_( true )
    , image_filter_interface_( image_filter_interface )
{
    wsp::StdParamDefList* param_def_list = image_filter_interface_->param_def_list();
    u32 num_params                       = (u32)param_def_list->length();
    image_filter_params_.SetLength( (int)num_params );
    for( u32 i=0; i<num_params; ++i )
    {
        wsp::StdParam *param = &image_filter_params_[i];
        wsp::SetDefaultParamFromDefinition( param, &(*param_def_list)[i] );
    }
}

template<typename T>
wsp::ImageFilter<T>::ImageFilter( wsp::ImageFilter<T> &image_filter )
    : ImageFilter( image_filter.image_filter_interface() )
    , enable_filter_( image_filter.enable_filter() )
{
}

template<typename T>
void wsp::ImageFilter<T>::set_image_filter_interface( wsp::ImageFilterPluginInterface<T>* image_filter_interface )
{ 
    image_filter_interface_ = image_filter_interface; 
    image_filter_interface_->GetDefaultParams( &image_filter_params_ );
}

#endif
