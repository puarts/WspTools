/**
 * @file cl-qt_unit_param_widget.cpp
 * @author Junichi Nishikata
 */


#include "cl-qt_text_number.hpp"
#include "cl-qt_slider.hpp"
#include "cl-qt_filepath_line.hpp"

#include "cl-qt_unit_param_widget.hpp"

#include "fn-gui_io.h"

wsp::QtUnitParamWidget::QtUnitParamWidget()
    : param_hblayout_    ( new QHBoxLayout         () )
    , param_vblayout_    ( new QVBoxLayout         () )
    , param_label_       ( new QLabel              () )
    , description_label_ ( new QLabel              () )
    , param_text_num_    ( new wsp::QtTextNumber   () )
    , param_slider_      ( new wsp::QtSlider       () )
    , param_text_edit_   ( new wsp::QtFilepathLine () )
    , param_checkbox_    ( new QCheckBox           () )
    , param_combobox_    ( new QComboBox           () )
    , param_type_        ( WSP_STD_PARAM_UNDEFINED )
    , id_                ( -1 )
{
}
wsp::QtUnitParamWidget::~QtUnitParamWidget()
{
    delete param_label_;
    delete description_label_;
    delete param_text_num_   ;
    delete param_slider_     ;
    delete param_text_edit_  ;
    delete param_checkbox_   ;
    delete param_combobox_;

    delete param_hblayout_;
    delete param_vblayout_;
}

void wsp::QtUnitParamWidget::setParamDefinition( 
    const wsp::StdParamDefinition *param_def,
    bool with_param_name
)
{
    const double double_param_mult =  static_cast<double>(pow(10.0, wsp::StdParamDefinition::kDoubleParamPrecision));

    param_type_ = param_def->param_type();
    id_ = param_def->param_id();

    if ( with_param_name )
    {
        param_label_->setText(param_def->name());//+QString("\t"));
        param_label_->setFixedWidth(110);
        param_hblayout_->addWidget(param_label_);
    }

    switch(param_type_)
    {
    case WSP_STD_PARAM_INTEGER:
        {
            param_text_num_->setText(QString().setNum(param_def->default_int()));
            param_text_num_->is_double_value = false;

            param_slider_->setRange(param_def->min_int(), param_def->max_int());
            param_slider_->setValue(param_def->default_int());
            WSP_GUI_LOG("%s: %d %d\n", param_def->name(), param_def->min_int(), param_def->max_int());
        }
        break;
    case WSP_STD_PARAM_FLOAT:
        {
            param_text_num_->setText(QString().setNum(param_def->default_double()));
            param_text_num_->is_double_value = true;

            param_slider_->setRange(
                    param_def->min_double() * double_param_mult, 
                    param_def->max_double() * double_param_mult
                );
            param_slider_->setValue(param_def->default_double() * double_param_mult);
        }
        break;
    case WSP_STD_PARAM_POINTER_LIST:
        {
            WSP_GUI_LOG("setting up combobox\n");
            if( param_def->ptr_list() )
            {
                int num_elems = param_def->ptr_list()->length();
                for( int i=0; i<num_elems; ++i )
                {
                    param_combobox_->addItem( QString("Item ") + QString().setNum(i) );
                }
            }
        }
        break;
    case WSP_STD_PARAM_BOOLEAN:
    case WSP_STD_PARAM_INPUT_FILEPATH:
        break;
    default:
        WSP_COMMON_WARNING_LOG("Unknown parameter type: %d\n", param_type_);
        return;
    }

    switch(param_type_)
    {
    case WSP_STD_PARAM_FLOAT:
    case WSP_STD_PARAM_INTEGER:
        {
            //param_text_num_->setFixedSize(50, 40);
            param_text_num_->setFixedWidth(50);
            param_slider_->setOrientation(Qt::Horizontal);
            param_slider_->setSingleStep(1);
            //param_slider_->setFixedSize(100, 10);
            connect(
                param_slider_,   SIGNAL(valueChanged(int)), 
                param_text_num_, SLOT  (slot_setText(int))
            );
            connect( 
                param_slider_, SIGNAL(valueChanged(int)),
                this, SLOT(slot_emitValueChanged(int))
            );
            connect(
                param_text_num_, SIGNAL(signal_textConverted(const QString&)), 
                param_slider_,   SLOT  (slot_setValue(const QString&))
            );
            connect(
                param_text_num_, SIGNAL(signal_textConverted(const QString&)), 
                param_slider_,   SLOT  (slot_setSliderPosition(const QString&))
            );

            // set layout
            param_hblayout_->addWidget(param_text_num_);
            param_hblayout_->addWidget(param_slider_);
            //param_hblayout_->setAlignment( param_text_num_, Qt::AlignTop );
            //param_hblayout_->setAlignment( param_slider_, Qt::AlignTop );
        }
        break;
    case WSP_STD_PARAM_BOOLEAN:
        {
            param_checkbox_->setChecked(param_def->default_bool());
            connect( 
                param_checkbox_, SIGNAL(stateChanged(int)),
                this,            SLOT(slot_emitValueChanged(int))
            );

            param_hblayout_->addWidget(param_checkbox_);
        }
        break;
    case WSP_STD_PARAM_POINTER_LIST:
        connect( 
            param_combobox_,  SIGNAL( currentIndexChanged(int) ),
            this,             SLOT(slot_emitComboBoxChanged(int))
        );
        param_hblayout_->addWidget( param_combobox_ );
        break;
    case WSP_STD_PARAM_INPUT_FILEPATH:
    case WSP_STD_PARAM_OUTPUT_FILEPATH:
        connect( 
            param_text_edit_, SIGNAL( textChanged() ),
            this,             SLOT(slot_textChanged())
        );
        param_hblayout_->addWidget(param_text_edit_);
        break;
    default:
        WSP_COMMON_WARNING_LOG("Unknown parameter type: %d\n", param_type_);
        return;
    }
    //param_hblayout_->setAlignment( Qt::AlignTop );
    //param_vblayout_->setAlignment( Qt::AlignTop );
    param_vblayout_->addLayout(param_hblayout_);
    if(param_def->description()[0]!='\0')
    {
        description_label_->setText(QString(param_def->description()));
        param_vblayout_->addWidget(description_label_);
        //param_vblayout_->addWidget(&description_label_);
    }
    this->setLayout( param_vblayout_ );
}

void wsp::QtUnitParamWidget::slot_emitValueChanged( int value )
{
    emit signal_valueChanged( value, id_, parent_process_id_ );
}

void wsp::QtUnitParamWidget::slot_emitTextChanged()
{
    emit signal_textChanged( param_text_edit_->text(), id_, parent_process_id_ );
}

void wsp::QtUnitParamWidget::slot_emitComboBoxChanged( int index )
{
    emit signal_comboBoxChanged( index, id_, parent_process_id_ );
}

void wsp::QtUnitParamWidget::setValue( int value )
{
    param_slider_->setValue( value );
    param_text_num_->setText( QString().setNum( value ) );
    param_combobox_->setCurrentIndex( value );
}

void wsp::QtUnitParamWidget::setFixedHeight( int height )
{
    param_text_num_->setFixedHeight( height );
    param_slider_->setFixedHeight( height );
}

void wsp::QtUnitParamWidget::SetParamSliderValue( int value )
{
    param_slider_->setValue( value );
}
