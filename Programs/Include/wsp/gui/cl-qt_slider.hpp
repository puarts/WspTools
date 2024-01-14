/**
 * @file cl-qt_slider.hpp
 * @author Junichi Nishikata
 */

#ifndef __WSP_GUI_CLASS_QT_SLIDER_H_
#define __WSP_GUI_CLASS_QT_SLIDER_H_

#include "_define_gui.h"

#include <QtGui/QSlider>

namespace wsp{
    class QtSlider
        : public QSlider
    {
        Q_OBJECT
    private slots:
        inline void slot_setValue(const QString &num);
        inline void slot_setSliderPosition(const QString &num);
    };
}

inline void wsp::QtSlider::slot_setValue(const QString &num)
{
    //printf("setValue %d", atoi(num.toLatin1().data()));
    setValue(atoi(num.toLatin1().data()));
}

inline void wsp::QtSlider::slot_setSliderPosition(const QString &num)
{
    //printf("setSliderPosition %d", atoi(num.toLatin1().data()));
    setSliderPosition(atoi(num.toLatin1().data()));
}

#endif