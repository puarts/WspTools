/**
 * @file cl-qt_image_proc_param_dialog.cpp
 * @author Jun-ichi Nishikata
 */

#include "_define_gui.h"

#include <wsp/image/gpl_tpl_fn-imgproc.h>
#include <wsp/common/cl-std_param_def_list.hpp>

#include "cl-qt_image_proc_param_dialog.hpp"


#define INIT_WSP_IMGPROC_PARAM_DIALOG \
            plugin_interface_(NULL), \
            preview_checkbox_(NULL)


wsp::QtImageProcParamDialog::QtImageProcParamDialog(
        QWidget*              parent, 
        wsp::StdParamDefList* paramList, 
        const char*           title, 
        wsp::ImageFilterPluginInterface<SIGNED_IMG_DATA_TYPE> *plugin_interface
)
: INIT_WSP_IMGPROC_PARAM_DIALOG
, wsp::QtParamDialog(parent, paramList, title)
{
    if(plugin_interface==NULL){ return; }
    plugin_interface_ = plugin_interface;


    /* Create GUI data */
    if(plugin_interface_->preview_filter()==false){ return; }

    preview_checkbox_ = new QCheckBox(QString("Preview"));
    extra_hblayout_->addWidget(preview_checkbox_);

    for(int i=0; i<num_of_params_; i++)
    {
        connect(
                param_widgets_[i].param_slider(), SIGNAL(valueChanged(int)), 
                this, SLOT(slot_preview(int))
            );
        connect(
                param_widgets_[i].param_checkbox(), SIGNAL(stateChanged(int)), 
                this, SLOT(slot_preview(int))
            );
        connect(
                param_widgets_[i].param_text_edit()->path_line(), SIGNAL(textChanged( const QString& )), 
                this, SLOT(slot_preview(const QString&))
            );
    }
    connect(preview_checkbox_, SIGNAL(stateChanged(int)), this, SLOT(slot_preview(int)));
}

wsp::QtImageProcParamDialog::~QtImageProcParamDialog()
{
    delete preview_checkbox_;
}

void wsp::QtImageProcParamDialog::slot_preview(const QString &text)
{
    wsp::QtImageProcParamDialog::slot_preview(0);
}

void wsp::QtImageProcParamDialog::slot_preview(int)
{
    WSP_COMMON_DEBUG_LOG("slot_preview called\n");
    if(plugin_interface_==NULL){ return; }
    if(preview_checkbox_->isChecked()==false)
    {
        if(plugin_interface_->img_seq()==NULL){
            WSP_COMMON_WARNING_LOG("Image sequence is NULL.\n"); return;
        }
        if(plugin_interface_->img_seq()->SeqSize()==0){
            WSP_COMMON_WARNING_LOG("The size of image sequence is zero.\n"); return;
        }
        if((*plugin_interface_->img_seq())[0]==NULL){
            WSP_COMMON_WARNING_LOG("Image is NULL.\n"); return; 
        }
        wsp::img::qtWspRgbImageToQLabel((*plugin_interface_->img_seq())[0], *plugin_interface_->image_label(), true, false);
        return;
    }

    wsp::Vector<wsp::StdParam> params;
    for(int i=0; i<num_of_params_; ++i){
        params.Append(GetParam(i));
    }
    plugin_interface_->PreviewFilter(params);
}

void wsp::QtImageProcParamDialog::slot_estimateParams()
{
    if(plugin_interface_==NULL){ return; }

    plugin_interface_->SetDefaultByCurrentResources();
    setToDefault();
}

