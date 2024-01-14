/**
 * @file cl-qt_glview.hpp
 * @author Jun-ichi Nishikata
 */

#ifndef _WSP_GUI_QT_GLVIEW_HPP_
#define _WSP_GUI_QT_GLVIEW_HPP_

#include "cl-qt_param_dialog.hpp"
#include "cl-qt_glwidget.hpp"

namespace wsp{
    class WSP_DLL_EXPORT QtGLView
        : public QMainWindow
    {
        //Q_OBJECT
    public:
        QtGLView();
        virtual ~QtGLView();

        // accessors
        inline wsp::QtGLWidget* gl_widget(){ return gl_widget_; }

    protected:
        void keyPressEvent(QKeyEvent *event);
        void keyReleaseEvent(QKeyEvent *event);
        //QSize wsp::QtGLView::minimumSizeHint() const;
        QSize sizeHint() const;

    protected:
        QMdiArea           *mdi_area_;
        wsp::QtGLWidget    *gl_widget_;
        wsp::QtParamDialog *ctrl_dlg_;
        QHBoxLayout        *main_layout_;
    };
}

#endif