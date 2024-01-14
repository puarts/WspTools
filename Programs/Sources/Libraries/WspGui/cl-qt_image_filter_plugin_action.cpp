/**
 * @file cl-qt_image_filter_plugin_action.cpp
 * @author Junichi Nishikata
 */

#include <omp.h>

#include "_define_gui.h"
#include "cl-qt_image_filter_plugin_action.hpp"

#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#include <wsp/common/tpl_cl-vector.h>
#endif
#include <wsp/multimedia/tpl_cl-mediaobject.h>
#include <wsp/image/gpl_tpl_fn-imgproc.h>
#include <wsp/common/cl-std_param_def_list.hpp>
#include "cl-qt_image_proc_param_dialog.hpp"
#ifndef __WSP_COMMONUTIL_FN_UTIL_H__
#include <wsp/common/fn-util.h>
#endif


#define INIT_WspQPuginAction plugin_interface_(NULL), img_seq_(NULL),    img_label_(NULL), index_(0)

wsp::QtImageFilterPluginAction::QtImageFilterPluginAction(QObject *parent)
    : INIT_WspQPuginAction, QAction(parent)
{}
wsp::QtImageFilterPluginAction::QtImageFilterPluginAction(const QString &text, QObject *parent)
    : INIT_WspQPuginAction, QAction(text, parent)
{}
wsp::QtImageFilterPluginAction::~QtImageFilterPluginAction()
{ 
    delete plugin_interface_; 
}

/*==========================================*/
/*==========================================*/
/*==========================================*/

void wsp::QtImageFilterPluginAction::slot_emitIndex(){
    //WSP_COMMON_DEBUG_LOG("slot_emitIndex\n");
    emit signal_emitIndex(index_);
}


//void wsp::QtImageFilterPluginAction::slot_applyFilter(wsp::ImageSeq<uchar> &io_img, QLabel &io_img_label)
void wsp::QtImageFilterPluginAction::slot_applyFilter()
{
    WSP_COMMON_DEBUG_LOG("Filter: %s %s\n", plugin_interface_->filter_name(),  plugin_interface_->version());
    if(plugin_interface_->img_seq()->SeqSize()==0){ WSP_COMMON_ERROR_LOG("input image is NULL"); return; }

    #ifdef _OPENMP
    //omp_set_num_threads(8);
    WSP_COMMON_LOG(
            "The number of processors is %u\n"
            "OpenMP : Enabled (Max # of threads = %u)\n"
            , omp_get_num_procs()
            , omp_get_max_threads()
        );
    #endif

    /* set params */
    wsp::Vector<wsp::StdParam> params;
    {
        wsp::QtImageProcParamDialog *dlg = new wsp::QtImageProcParamDialog(
                                                        NULL, 
                                                        plugin_interface_->param_def_list(), 
                                                        plugin_interface_->filter_name(), 
                                                        plugin_interface_
                                                    );
        int state = dlg->exec(); //! display, get status
        if(state!=QDialog::Accepted){ WSP_COMMON_DEBUG_LOG("Canceled\n"); delete dlg; return; }

        WSP_COMMON_LOG("length of param_def_list: %d\n", plugin_interface_->param_def_list()->length());
        for(int i=0; i<plugin_interface_->param_def_list()->length(); ++i)
        {
            params.Append(dlg->GetParam(i));
        }
        delete dlg; //! delete widget

        plugin_interface_->SetDefaultParams(params);
    }

    plugin_interface_->SetProgressBarFrom(progress_bar_);

    wsp::StartTimer();
    plugin_interface_->ApplySequenceRelatedFilter(params);
    wsp::StopTimer();

    /* copy result image */
    {
        wsp::State wstat;
        int w = static_cast<int>(plugin_interface_->img_seq()->width());
        int h = static_cast<int>(plugin_interface_->img_seq()->height());
        QImage qimg(static_cast<int>(w), static_cast<int>(h), QImage::Format_ARGB32);
        wsp::Image<SIGNED_IMG_DATA_TYPE> tmp_img;
        tmp_img = (*plugin_interface_->img_seq())[0];

        wstat = wsp::img::qtWspImageToQImage<SIGNED_IMG_DATA_TYPE>(qimg, NULL, tmp_img);
        if(wstat!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Converting failed\n"); return; }

        plugin_interface_->image_label()->setPixmap( QPixmap::fromImage(qimg) );
    }

    WSP_COMMON_LOG( "\n%s finished\n", plugin_interface_->filter_name() );
}
