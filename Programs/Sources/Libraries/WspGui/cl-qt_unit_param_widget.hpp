/**
 * @file cl-qt_unit_param_widget.hpp
 * @author Junichi Nishikata
 */

#ifndef __WSP_GUI_CLASS_QT_UNIT_PARAM_WIDGET_H_
#define __WSP_GUI_CLASS_QT_UNIT_PARAM_WIDGET_H_

#include <QtGui/QtGui>
#include <wsp/common/_cl-std_param.h>

namespace wsp{
    class QtTextNumber  ;
    class QtSlider      ;
    class QtFilepathLine;
}

namespace wsp{
    class WSP_DLL_EXPORT QtUnitParamWidget
        : public QWidget
    {
        Q_OBJECT
    public:
        QtUnitParamWidget();
        ~QtUnitParamWidget();

        /* getter */
        int                  parent_process_id() const{ return parent_process_id_; }
        int                  id               () const{ return id_; }
        wsp::StdParamType    param_type       () const{ return param_type_; }
        QLabel*              param_label      ()      { return param_label_; }
        const wsp::QtSlider* param_slider     () const{ return param_slider_; }
        QCheckBox*           param_checkbox   ()      { return param_checkbox_; }
        wsp::QtTextNumber*   param_text_num   ()      { return param_text_num_; }
        wsp::QtFilepathLine* param_text_edit  ()      { return param_text_edit_; }
        QComboBox*           param_combobox   ()      { return param_combobox_; }

        /* setter */
        void set_parent_process_id( int process_id ){ parent_process_id_ = process_id; }
        void set_param_type( wsp::StdParamType param_type ){ param_type_ = param_type; }
        void setParamDefinition( const wsp::StdParamDefinition *param_def, bool with_param_name=true );
        void setValue( int value );
        void setFixedHeight( int height );
        void SetParamSliderValue( int value );

    public slots:
        void slot_emitValueChanged( int value );
        void slot_emitTextChanged();
        void slot_emitComboBoxChanged( int index );

    signals:
        void signal_valueChanged( int value, int param_id, int parent_process_id );
        void signal_textChanged( QString text, int param_id, int parent_process_id );
        void signal_comboBoxChanged( int index, int param_id, int parent_process_id );

    protected:
        int                 parent_process_id_;
        int                 id_;
        wsp::StdParamType   param_type_;
        QLabel              *param_label_;
        QLabel              *description_label_;
        wsp::QtTextNumber   *param_text_num_;
        wsp::QtSlider       *param_slider_;
        wsp::QtFilepathLine *param_text_edit_;
        QCheckBox           *param_checkbox_;
        QComboBox           *param_combobox_;

        QHBoxLayout         *param_hblayout_;
        QVBoxLayout         *param_vblayout_;
    };
}


#endif