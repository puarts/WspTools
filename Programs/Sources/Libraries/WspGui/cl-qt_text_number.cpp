/**
 * @file cl-qt_text_number.cpp
 * @author Junichi Nishikata
 */

#include <wsp/common/_cl-std_param.h>

#include "cl-qt_text_number.hpp"

//int wsp::QtParamDialog::kDoubleParamPrecision = 5;

void wsp::QtTextNumber::slot_setText(int num)
{
    //WSP_COMMON_LOG("setText %d\n", num);
    if(is_double_value){
        double set_num = wsp::ConvertIntegerValueToDoubleParam( num );
        setText(QString().setNum( set_num ));
        return;
    }
    setText(QString().setNum(num));
}

void wsp::QtTextNumber::slot_convertText()
{
    QString num = this->text();
    int value;
    if(is_double_value){
        value = static_cast<int>(atof(num.toLatin1().data()) * static_cast<double>(pow(10.0, wsp::StdParamDefinition::kDoubleParamPrecision)));
    }else{
        value = atoi(num.toLatin1().data());
    }
    emit signal_textConverted(QString().setNum(value));
}