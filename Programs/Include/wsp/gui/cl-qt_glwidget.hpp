/**
 * @file cl-qt_glwidget.hpp
 * @author Jun-ichi Nishikata
 */

#ifndef _WSP_GUI_CLASS_QT_GLWIDGET_HPP_
#define _WSP_GUI_CLASS_QT_GLWIDGET_HPP_

#include "_define_gui.h"

#include <QtGui/QtGui>

#include <wsp/opengl/cl-polymodel.h>
#include <wsp/graphics/cl-camera.hpp>
#include <QtOpenGL/qgl.h>


#define NUM_OF_LABELS 4
#define MAX_TEXTURE_NUM 30

namespace wsp{
    class QtGLView;

    class WSP_DLL_EXPORT QtMaterialView
        : public QWidget
    {
        Q_OBJECT
    public:

    };

    class WSP_DLL_EXPORT QtGLWidget 
        : public ::QGLWidget
    {
        friend class wsp::QtGLView;

        Q_OBJECT

    public:
        QtGLWidget( QWidget *parent = 0 , const char *window_title="3D View" );
        ~QtGLWidget();

        // accessors
        inline wsp::Vector<wsp::PolyModel>& polymodel_list(){ return *polymodel_list_; }
        void loadShader(const char *vsh_file, const char *fsh_file);
        void assignTexture(const wsp::Image<uchar> &in_image );
        void assignTextures(const wsp::Vector<wsp::Image<uchar>> &in_image_list );

        virtual void keyPressEvent(QKeyEvent *event);
        virtual void keyReleaseEvent(QKeyEvent *event);

        // mutators
        void set_init_rx(float rx){ init_rx_ = rx; }
        void set_init_ry(float ry){ init_ry_ = ry; }
        void set_init_rz(float rz){ init_rz_ = rz; }

        QSize minimumSizeHint() const;
        QSize sizeHint() const;


    public slots:
        void enableWireframe(bool enabled);
        void enableNormals(bool enabled);
        void setModelColor();
        void setBackgroundColor();
        //void loadModel();
        //void loadModel(const QString &filePath);
        //void modelLoaded();
        void slot_changeDisplayType(int type);

        /* file menu */
        void slot_loadModel();
        void slot_loadDepthMap();
        void slot_loadTexture();
        void slot_loadShader();

        /* display menu */
        void slot_vertexDisp();
        void slot_wireframeDisp();
        void slot_triangleDisp();

        /* shading > environment map menu */
        void slot_disableEnvmap();
        void slot_enableEnvmapCubemap();

        /* test menu */
        void slot_testDrawOff();
        void slot_testDrawSphere();
        void slot_testDrawTeapot();

        /* slots for parameter setting dialog */
        void slot_setEnvmapRatio(int);


    protected:
        virtual void initializeGL();
        virtual void resizeGL(int width, int height);
        virtual void paintGL();
        virtual void mousePressEvent(QMouseEvent *event);
        virtual void mouseReleaseEvent(QMouseEvent *event);
        virtual void mouseMoveEvent(QMouseEvent *event);
        virtual void wheelEvent(QWheelEvent * wheelEvent);


    private:
        void setDisplayType();
        

    private:
        class Impl;
        Impl *pImpl_;

        // data ------------------------------
        bool enable_wireframe_;
        bool display_normal_;
        QColor model_color_;
        QColor bg_color_;

        wsp::Vector<wsp::PolyModel> *polymodel_list_;
        wsp::Camera *camera_;

        wsp::Vector<GLuint> tex_obj_list_;
        GLuint tex_obj_[MAX_TEXTURE_NUM];
        QTime m_time;
        int m_lastTime;
        int m_mouseEventTime;

        float m_distance;
        wsp::Point<float> m_rotation;
        wsp::Point<float> m_angularMomentum;
        wsp::Point<float> m_accumulatedMomentum;

        float init_rx_, init_ry_, init_rz_;

        /** @brief  camera exponential distance */
        //float cam_exp_dist_;
        float camera_hor_move_, camera_ver_move_, camera_distance_;
        float camera_pitch_, camera_yaw_;
        float tgt_x_, tgt_y_, tgt_z_;

        // gui related data
        bool camera_move_switch_;
    };
}

#endif

