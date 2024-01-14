// File Name: WspQGraphicsView.h
// Author: Junichi Nishikata


#include "WspQGraphicsScene.h"


class WspQGraphicsView
    : public QGraphicsView
{
public:
    WspQGraphicsView()
    {
        setWindowTitle(tr("3D Model Viewer"));
    }

protected:
    void resizeEvent(QResizeEvent *event) {
        if (scene()!=NULL){
            scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
        }
        QGraphicsView::resizeEvent(event);
    }
};

