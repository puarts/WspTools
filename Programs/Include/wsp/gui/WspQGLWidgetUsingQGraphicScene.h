/**
 * @file WspQGLWidgetUsingQGraphicScene.h
 * Author: Jun-ichi Nishikata
 */

#ifndef WSP_GUI_CLASS_QT_GLWIDGET_H
#define WSP_GUI_CLASS_QT_GLWIDGET_H

#include <QtOpenGL/qgl.h>

namespace wsp{
    class QtGLWidgetUsingQGraphicScene 
        : public QGLWidget
    {
        Q_OBJECT

    public:
        QtGLWidgetUsingQGraphicScene( QWidget *parent = 0 , const char *name = 0 );

    protected:
        void initializeGL();
        void resizeGL(int width, int height);
        void paintGL();

    };
}

#endif

