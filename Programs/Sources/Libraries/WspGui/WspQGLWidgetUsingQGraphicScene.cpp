//** WspQtGLWidgetUsingQGraphicScene.cpp
//** Author: Jun-ichi Nishikata

#include <iostream>
#include <wsp/gui/WspQGLWidgetUsingQGraphicScene.h>

using namespace std;

wsp::QtGLWidgetUsingQGraphicScene::QtGLWidgetUsingQGraphicScene(QWidget *parent, const char *name)
    : QGLWidget(parent)
{ this->setWindowTitle(QString(name)); }

void wsp::QtGLWidgetUsingQGraphicScene::initializeGL()
{
    glClearColor(0.9, 0.9, 0.9, 1.0);
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void wsp::QtGLWidgetUsingQGraphicScene::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void wsp::QtGLWidgetUsingQGraphicScene::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //** write graphics code from here ----------------

    //**----------------------------------------

    glColor3d( 0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2d(0.1, 0.1);
    glVertex2d(0.9, 0.1);
    glVertex2d(0.9, 0.9);
    glVertex2d(0.1, 0.9);
    glEnd();

}



