//! WspQtGraphViewer.h
// 

#ifndef __WspQtGraphViewer_H__
#define __WspQtGraphViewer_H__

#include <wsp/common/_define_qt.h>
#include "cl-function.h"

#ifdef USE_QT
class WspQtGraphViewer
    : public wsp::Vector<wsp::Function>
    , public QDialog
{
protected:
    //! Data
    QString m_msg;

    //! GUI
    QPushButton *closeButton;
    QLabel *renderArea;
    QLabel *infoArea;
    QPixmap resultImage;

public:
    u32 width, height;
    double y_max, y_min;
    double x_max, x_min;
    int padding;

protected:
    WSP_DLL_EXPORT void Init();
    WSP_DLL_EXPORT void paintEvent(QPaintEvent*);

public:
    WSP_DLL_EXPORT WspQtGraphViewer();
    //WspQtGraphViewer(const WspFunction &in_func);
    WSP_DLL_EXPORT ~WspQtGraphViewer();

    WSP_DLL_EXPORT void ShowGraph(const QString& msg=tr(""));

};

#endif

#endif