/**
 * @file cl-qt_param_dialog.hpp
 * @author Junichi Nishikata
 */

#ifndef __WSP_GUI_CLASS_QT_PARAM_DIALOG_H_
#define __WSP_GUI_CLASS_QT_PARAM_DIALOG_H_

#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>

#include "_define_gui.h"

#include <wsp/common/_define_qt.h>
#include <wsp/image/tpl_cl-image.h>
#include <wsp/image/tpl_fn-imgproc.hpp>
#include "cl-qt_filepath_line.hpp"
#include "cl-qt_text_number.hpp"
#include "cl-qt_slider.hpp"
#include "cl-qt_unit_param_widget.hpp"
#include <wsp/common/cl-std_param_def_list.hpp>

//class WspRgb;
//class WspDoubleImg;

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;

class QGridLayout;
class QVBoxLayout;



//namespace wsp{
//    enum StdParamType;
//    class StdParamDefinition;
//    struct StdParam;
//}


namespace wsp{

    class WSP_DLL_EXPORT QtParamDialog
        : public QDialog
    {
        Q_OBJECT
        // Methods -------------------------------------------
    public:
        QtParamDialog(
            QWidget *parent=0, 
            wsp::StdParamDefList *param_list=NULL, 
            const char *title="Param Setting"
        );
        ~QtParamDialog();

        /* Getter */
        wsp::StdParam  GetParam      (int index);
        const wsp::QtSlider* param_slider  (int index){ return param_widgets_[index].param_slider(); }
        QCheckBox*     param_checkbox(int index){ return param_widgets_[index].param_checkbox(); }

        /* Setter */
        void set_param_def_list( wsp::StdParamDefList* param_def_list ){ param_def_list_ = param_def_list; }

        //QSize minimumSizeHint() const;
        //QSize sizeHint() const;

        /* Setter */
        void setToDefault();
        void SetParamSliderValue( int index, int value );

        void HideApplyButton();
        void HideCancelButton();


    protected slots:
        virtual void slot_start();
        virtual void slot_loadXml();
        virtual void slot_saveXml();
        virtual void slot_estimateParams(){ WSP_COMMON_DEBUG_LOG("wsp::QtParamDialog::slot_estimateParams()\n"); };

    protected:
        virtual void closeEvent(QCloseEvent *e);


        // Variables -------------------------------------------
    protected:
        int num_of_params_;
        QHBoxLayout *extra_hblayout_;

        wsp::QtUnitParamWidget* param_widgets_;

    private:
        // data
        wsp::StdParamDefList *param_def_list_;
        //int *id_list_;

        // main GUI
        QWidget *scroll_area_widget_list_;
        QGridLayout *main_glayout_;
        QScrollArea *scroll_area_; // unused now

        QTabWidget *tab_widget_;
        QScrollArea *tab_scroll_area_list_;
        QGridLayout *tab_glayout_list_;

        // Footer button elements
        QHBoxLayout *btn_hlayout_;
        QPushButton *start_btn;
        QPushButton *cancel_btn;
        QPushButton *load_btn;
        QPushButton *save_btn;
        QPushButton *auto_param_estimate_btn_;

        // Parameter setting GUI elements

        //QHBoxLayout *param_hblayout;
        //QVBoxLayout *param_vblayout;
        //QLabel *param_label_, *description_label_;
        //QCheckBox *param_checkbox_;
        //wsp::QtTextNumber *param_text_num_;
        //wsp::QtSlider *param_slider_;
        //wsp::QtFilepathLine *param_text_edit_;
        //wsp::StdParamType *param_types_;

        QFrame *separators_;

    };
}


#endif