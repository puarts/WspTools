/**
 * @file cl-image_filter_plugin_interface.hpp
 * 
 * @brief Declaration of wsp::ImageFilterPluginInterface.
 */

#ifndef WSP_IMAGE_CLASS_IMAGE_FILTER_PLUGIN_INTERFACE_HPP_
#define WSP_IMAGE_CLASS_IMAGE_FILTER_PLUGIN_INTERFACE_HPP_

#ifndef __WSP_COMMONCORE_RESULT_H__
#include <wsp/common/result.h>
#endif
#include "tpl_cl-imageseq.h"
#include <wsp/common/cl-std_param_def_list.hpp>

#ifdef USE_QT
class QLabel;
#endif

namespace wsp{ namespace mm{
    template<typename T> class CompositionLayer;
}}
struct WSP_CompositionParam;



namespace wsp{
    /**
     * @brief Plug-in interface for image filter
     */
    template<class T>
    class ImageFilterPluginInterface
    {
    public:
        ImageFilterPluginInterface(void);
        virtual ~ImageFilterPluginInterface(void){}

        /* ====================================
                      Getter 
           ====================================*/
        inline s32                      id                (void) const{ return id_; }
        inline wsp::StdParamDefList*    param_def_list    (void)      { return &param_def_list_; }
        inline u32                      version           (void) const{ return version_; }
        inline u16                      GetMajorVersion   (void) const;
        inline u8                       GetMinorVersion   (void) const;
        inline u8                       GetMicroVersion   (void) const;
        inline const char*              version_as_str    (void) const{ return version_as_str_; }
        inline const char*              filter_name       (void) const{ return filter_name_; }
        inline const char*              category          (void) const{ return category_; }
        inline bool                     preview_filter    (void) const{ return enable_preview_; }
        inline const char*              src_image_path_ptr(void) const{ return src_img_path_; }
        inline wsp::ImageSeq<T>*        img_seq           (void)      { return img_seq_; }
        inline wsp::Image<T>*           image             (void)      { return image_; }
        inline const wsp::Vector<wsp::mm::CompositionLayer<T>*>* comp_layer_list() const{ return comp_layer_list_; }
        inline const WSP_CompositionParam* composition_param (void) const{ return composition_param_; }
        inline const wsp::Vector<wsp::mm::CompositionLayer<T>*>* const* GetCompLayerListPointer()const{ return &comp_layer_list_; }

        #ifdef USE_QT
        inline QLabel*                  image_label       (void)      { return image_label_; }
        #endif
        inline u32                      GetNumOfParams    (void) const{ return (u32)param_def_list_.length(); }
        inline bool                     IsNullImage       (void) const{ return image_==NULL; }
        inline bool                     IsNullImageSeq    (void) const{ return img_seq_==NULL; }

        wsp::Result              GetDefaultParams      ( wsp::Vector<wsp::StdParam>* o_params );
        wsp::StdParam            GetParamByID          ( const wsp::Vector<wsp::StdParam> &param_list, s32 id);
        wsp::StdParam            GetParamByName        ( const wsp::Vector<wsp::StdParam> &param_list, const char *name);
        wsp::StdParamDefinition* GetParamDefinitionByID( s32 id );

#ifdef USE_QT
        inline s32 GetCurrentFrame() const{ return time_slider_->value(); }
#endif

        /* ====================================
                      Setter 
           ====================================*/
        void SetDefaultParams(wsp::Vector<wsp::StdParam> &in_params);

        inline void        SetImageFrom    ( wsp::Image<T> *image      ){ image_   = image; }
        inline void        SetImageSeqFrom ( wsp::ImageSeq<T> *img_seq ){ img_seq_ = img_seq; }
        inline void        SetImageSeqInto ( wsp::ImageSeq<T> *img_seq ){ img_seq  = img_seq_; }
        inline wsp::Result SetSrcImagePath ( const char *src_path      );
        inline void        set_comp_layer_list( const wsp::Vector<wsp::mm::CompositionLayer<T>*> *comp_layer_list ){ comp_layer_list_ = comp_layer_list; }
        inline void        set_composition_param( const WSP_CompositionParam *composition_param ){ composition_param_ = composition_param; }

        #ifdef USE_QT
        inline void set_time_slider   (const QSlider *time_slider){ time_slider_  = time_slider; }
        inline void SetImageLabelFrom (QLabel *image_label ){ image_label_  = image_label; }
        inline void SetImageLabelInto (QLabel *image_label ){ image_label   = image_label_; }
        inline void SetProgressBarFrom(QProgressBar *bar   ){ progress_bar_ = bar; }
        inline void SetProgressBarInto(QProgressBar *bar   ){ bar           = progress_bar_; }
        #endif


    
        /* ====================================
                   Virtual methods
           ====================================*/
        virtual wsp::Result ApplyFilter                 ( const wsp::Vector<wsp::StdParam> &in_params ){ return wsp::ResultNotImplemented(); };
        virtual wsp::Result ApplySequenceRelatedFilter  ( const wsp::Vector<wsp::StdParam> &in_params ){ return wsp::ResultNotImplemented(); };
        virtual wsp::Result PreviewFilter               ( const wsp::Vector<wsp::StdParam> &in_params ){ return wsp::ResultNotImplemented(); };
        virtual void        SetDefaultByCurrentResources(void){};

    protected:
        inline void set_id            ( s32 id ){ id_ = id; }
        inline void SetVersion        ( u32 version );
        inline void SetVersion        ( u16 major_version, u8 minor_version, u8 micro_version );
        inline void set_filter_name   ( const char *filter_name ){ strcpy( filter_name_, filter_name ); }
        inline void set_category      ( const char *category    ){ strcpy( category_, category ); }
        inline void set_enable_preview( bool enable_preview     ){ enable_preview_ = enable_preview; }
        inline void AppendParamDefinition( const wsp::StdParamDefinition &param_def){ param_def_list_.Append(param_def); }

    private:
        static const int     kAdvancedParam;

        s32                  id_;
        wsp::StdParamDefList param_def_list_;
        u32                  version_;
        char                 version_as_str_[MAX_LENGTH_OF_NAME];
        char                 filter_name_   [MAX_LENGTH_OF_NAME];
        char                 category_      [MAX_LENGTH_OF_NAME];
        char                 src_img_path_  [MAX_LENGTH_OF_PATH];
        wsp::Image<T>*       image_;
        wsp::ImageSeq<T>*    img_seq_;

        #ifdef USE_QT
        const QSlider*       time_slider_;
        QLabel*              image_label_;
        QProgressBar*        progress_bar_;
        #endif

        bool                 enable_preview_;

        const wsp::Vector<wsp::mm::CompositionLayer<T>*> *comp_layer_list_;
        const WSP_CompositionParam *composition_param_;

    private:
        ImageFilterPluginInterface( const wsp::ImageFilterPluginInterface<T> &rhs );
        const wsp::ImageFilterPluginInterface<T>& operator=( const wsp::ImageFilterPluginInterface<T> &rhs );
    };

    template<class T>
    const int ImageFilterPluginInterface<T>::kAdvancedParam = 1;

}

#include "tpl_cl-image_filter_plugin_interface.inl"


#endif
