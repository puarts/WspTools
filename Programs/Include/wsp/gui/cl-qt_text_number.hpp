/**
 * @file cl-qt_text_number.hpp
 * @author Junichi Nishikata
 */

#ifndef __WSP_GUI_CLASS_QT_TEXTNUMBER_H__
#define __WSP_GUI_CLASS_QT_TEXTNUMBER_H__

#include <QtGui/QtGui>

namespace wsp{
    class QtTextNumber
        : public QLineEdit
    {
        Q_OBJECT
    public:
        inline QtTextNumber():is_double_value(false){
            connect( this, SIGNAL(editingFinished()), this, SLOT(slot_convertText()) );
        }

    public:
        bool is_double_value;

    private slots:
        void slot_setText(int);
        //void slot_convertText(const QString&);
        void slot_convertText();

    signals:
        void signal_textConverted(const QString&);

    };
}

#endif