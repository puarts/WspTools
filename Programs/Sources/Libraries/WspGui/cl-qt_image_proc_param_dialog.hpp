/**
 * @file cl-qt_image_proc_param_dialog.hpp
 * @author Jun-ichi Nishikata
 */

#ifndef WSP_GUI_CLASS_IMG_PROC_PARAM_DIALOG_HPP__
#define WSP_GUI_CLASS_IMG_PROC_PARAM_DIALOG_HPP__

#include "_define_gui.h"
#include <wsp/image/tpl_cl-image_filter_plugin_interface.hpp>
#include "cl-qt_param_dialog.hpp"
#include "cl-qt_abstract_mdisubwin.hpp"

namespace wsp{
    class WSP_DLL_EXPORT QtImageProcParamDialog
        : public wsp::QtParamDialog
    {
        Q_OBJECT
    public:
        // Constructor, destructor
        QtImageProcParamDialog(
            QWidget *parent=0, 
            wsp::StdParamDefList *paramList=NULL, 
            const char *title="paramSettingDialog", 
            wsp::ImageFilterPluginInterface<SIGNED_IMG_DATA_TYPE> *plugin_interface=NULL
        );
        ~QtImageProcParamDialog();

        // Mutators
        void set_plugin_interface( wsp::ImageFilterPluginInterface<SIGNED_IMG_DATA_TYPE> *plugin_interface )
        {
            plugin_interface_ = plugin_interface;
        }

    private slots:
        void slot_preview(int);
        void slot_preview(const QString &text);
        virtual void slot_estimateParams();

    private:
        //! data
        wsp::ImageFilterPluginInterface<SIGNED_IMG_DATA_TYPE> *plugin_interface_;

        //! gui
        QCheckBox *preview_checkbox_;

    };

    DEFINE_MDI_SUB_WIN_CLASS(QtImageProcParamDialog );
}

#endif
