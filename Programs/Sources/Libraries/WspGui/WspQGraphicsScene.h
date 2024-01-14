// File Name: WspQGraphicsScene.h
// Author: Junichi Nishikata


#ifndef _WspOGraphicsScene_H_
#define _WspOGraphicsScene_H_

#include <wsp/common/tpl_cl-vector3.h>

#include <QGraphicsScene>
#include <GL/freeglut.h>

#include <QLabel>
#include <QTime>

#ifndef QT_NO_CONCURRENT
#include <QFutureWatcher>
#endif



namespace wsp{
        class PolyModel;
}
class Model;

class WspQGraphicsScene
    : public QGraphicsScene
{
    Q_OBJECT
private:
    //** data ------------------------------
    bool m_wireframeEnabled;
    bool m_normalsEnabled;
    QColor m_modelColor;
    QColor m_backgroundColor;
    wsp::PolyModel *m_model;
    //Model *m_model;
    QTime m_time;
    int m_lastTime;
    int m_mouseEventTime;

    float m_distance;
    wsp::Point<float> m_rotation;
    wsp::Point<float> m_angularMomentum;
    wsp::Point<float> m_accumulatedMomentum;

    /** camera exponential distance */
    //float cam_exp_dist_;
    float camera_hor_move_, camera_ver_move_, camera_distance_;
    float camera_pitch_, camera_yaw_;

    GLuint texture_;


    //** gui data --------------------------
    QLabel *m_labels[4];
    QPushButton *m_modelButton;
    QPushButton *m_depthMapButton;
    QComboBox *display_type_combobox_;

    QGraphicsRectItem *m_lightItem;

    bool camera_move_switch_;

public:
    WspQGraphicsScene();
    ~WspQGraphicsScene();

    void drawBackground(QPainter *painter, const QRectF &rect);

public slots:
    void enableWireframe(bool enabled);
    void enableNormals(bool enabled);
    void setModelColor();
    void setBackgroundColor();
    //void loadModel();
    //void loadModel(const QString &filePath);
    //void modelLoaded();
    void slot_changeDisplayType(int type);
    void slot_loadModel();
    void slot_loadDepthMap();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent * wheelEvent);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    void setDisplayType();
    QDialog *createDialog(const QString &windowTitle) const;
    //void setModel(wsp::PolyModel *model);
    //void setModel(Model *model);

//#ifndef QT_NO_CONCURRENT
//    QFutureWatcher<wsp::PolyModel *> m_modelLoader;
//    //QFutureWatcher<Model *> m_modelLoader;
//#endif
};

#endif
