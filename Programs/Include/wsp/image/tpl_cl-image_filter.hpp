/**
 * @file cl-image_filter.hpp
 * 
 * @brief Declaration of wsp::ImageFilter class template.
 */

#ifndef WSP_IMAGE_TEMPLATE_CLASS_IMAGE_FILTER_HPP_
#define WSP_IMAGE_TEMPLATE_CLASS_IMAGE_FILTER_HPP_

#include "tpl_cl-image_filter_plugin_interface.hpp"

namespace wsp{
    /**
     * @brief Class which provides access to image filter plugin interface.
     */
    template<class T>
    class ImageFilter
    {
    public:
        explicit ImageFilter( wsp::ImageFilterPluginInterface<T>* image_filter_interface );
        explicit ImageFilter( wsp::ImageFilter<T> &image_filter );

        /* getter */
        s32                                 id                    (void) const{ return image_filter_interface_->id(); }
        bool                                enable_filter         (void) const{ return enable_filter_; }
        wsp::ImageFilterPluginInterface<T>* image_filter_interface(void)      { return image_filter_interface_; }
        wsp::StdParamDefList*               param_def_list        (void)      { return image_filter_interface_->param_def_list(); }
        wsp::Vector<wsp::StdParam>*         image_filter_params   (void)      { return &image_filter_params_; }
        const char*                         filter_name           (void) const{ return image_filter_interface_->filter_name(); }
        u32                                 GetNumOfParams        (void) const{ return image_filter_interface_->GetNumOfParams(); }

        /* setter */
        void set_image_filter_interface( wsp::ImageFilterPluginInterface<T>* image_filter_interface );
        void set_enable_filter         ( bool enable ){ enable_filter_ = enable; }

    private:
        /* external data */
        wsp::ImageFilterPluginInterface<T>* image_filter_interface_;

        /* own data */
        wsp::Vector<wsp::StdParam> image_filter_params_;
        bool enable_filter_;
    };
}

#include "tpl_cl-image_filter.inl"

#endif
