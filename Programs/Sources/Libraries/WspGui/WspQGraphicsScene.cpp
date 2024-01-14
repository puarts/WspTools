// File Name: WspQGraphicsScene.cpp
// Author: Jun-ichi Nishikata

#include <wsp/graphics/cl-polymodel.h>
#include <wsp/graphics/wrap_fn-gl.h>

#include <QtGui>
#include <QtOpenGL/QtOpenGL>


#include <wsp/image/tpl_cl-image.h>
//#include "model.h"

#include <wsp/gui/WspQGraphicsScene.h>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

QDialog *WspQGraphicsScene::createDialog(const QString &windowTitle) const
{
    QDialog *dialog = new QDialog(0, Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    dialog->setWindowOpacity(0.8);
    dialog->setWindowTitle(windowTitle);
    dialog->setLayout(new QVBoxLayout);

    return dialog;
}


WspQGraphicsScene::WspQGraphicsScene()
    : m_wireframeEnabled(false)
    , m_normalsEnabled(false)
    , m_modelColor(128, 128, 128)
    , m_backgroundColor(148, 148, 148)
    , m_model(0)
    , m_lastTime(0)
    , m_distance(0.0f)
    , m_angularMomentum(0, 40, 0)
    , camera_hor_move_(0.0f), camera_ver_move_(0.0f)
    , camera_distance_(-3.0f)
    , camera_pitch_(0.0f), camera_yaw_(0.0f)
    , camera_move_switch_(false)
{
    m_model = new wsp::PolyModel();



    QWidget *controls = createDialog(tr("Controls"));

    m_modelButton = new QPushButton(tr("Load model"));
    connect(m_modelButton, SIGNAL(clicked()), this, SLOT(slot_loadModel()));
//#ifndef QT_NO_CONCURRENT
//    connect(&m_modelLoader, SIGNAL(finished()), this, SLOT(modelLoaded()));
//#endif

    m_depthMapButton = new QPushButton(tr("Load Depth Map"));
    connect(m_depthMapButton, SIGNAL(clicked()), this, SLOT(slot_loadDepthMap()));


    display_type_combobox_ = new QComboBox();
    display_type_combobox_->addItem(QString("Points"));
    display_type_combobox_->addItem(QString("Wireframe"));
    display_type_combobox_->addItem(QString("Triangle"));
    connect(display_type_combobox_, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_changeDisplayType(int)));



    QCheckBox *wireframe = new QCheckBox(tr("Render as wireframe"));
    connect(wireframe, SIGNAL(toggled(bool)), this, SLOT(enableWireframe(bool)));

    QCheckBox *normals = new QCheckBox(tr("Display normals vectors"));
    connect(normals, SIGNAL(toggled(bool)), this, SLOT(enableNormals(bool)));

    QPushButton *colorButton = new QPushButton(tr("Choose model color"));
    connect(colorButton, SIGNAL(clicked()), this, SLOT(setModelColor()));

    QPushButton *backgroundButton = new QPushButton(tr("Choose background color"));
    connect(backgroundButton, SIGNAL(clicked()), this, SLOT(setBackgroundColor()));

    QWidget *statistics = createDialog(tr("Model info"));
    statistics->layout()->setMargin(20);


    controls->layout()->addWidget(m_modelButton);
    controls->layout()->addWidget(m_depthMapButton);
    controls->layout()->addWidget(display_type_combobox_);
    controls->layout()->addWidget(wireframe);
    controls->layout()->addWidget(normals);
    controls->layout()->addWidget(colorButton);
    controls->layout()->addWidget(backgroundButton);


    for (int i = 0; i < 4; ++i) {
        m_labels[i] = new QLabel;
        statistics->layout()->addWidget(m_labels[i]);
    }

    QWidget *instructions = createDialog(tr("Instructions"));
    instructions->layout()->addWidget(new QLabel(tr("Use mouse wheel to zoom model, and click and drag to rotate model")));
    instructions->layout()->addWidget(new QLabel(tr("Move the sun around to change the light position")));

    QWidget *widgets[] = { instructions, controls, statistics };

    for (u32 i = 0; i < sizeof(widgets) / sizeof(*widgets); ++i) {
        QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget(0, Qt::Dialog);
        proxy->setWidget(widgets[i]);
        addItem(proxy);
    }

    QPointF pos(10, 10);
    foreach (QGraphicsItem *item, items()) {
        item->setFlag(QGraphicsItem::ItemIsMovable);
        item->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

        const QRectF rect = item->boundingRect();
        item->setPos(pos.x() - rect.x(), pos.y() - rect.y());
        pos += QPointF(0, 10 + rect.height());
    }

    QRadialGradient gradient(40, 40, 40, 40, 40);
    gradient.setColorAt(0.2, Qt::yellow);
    gradient.setColorAt(1, Qt::transparent);

    m_lightItem = new QGraphicsRectItem(0, 0, 80, 80);
    m_lightItem->setPen(Qt::NoPen);
    m_lightItem->setBrush(gradient);
    m_lightItem->setFlag(QGraphicsItem::ItemIsMovable);
    m_lightItem->setPos(800, 200);
    addItem(m_lightItem);

    //loadModel(QLatin1String("qt.obj"));


    //m_model->LoadObjFile("qt.obj");
    //m_model->CreatePrimitive(wsp::graphic::POLY_PRIMITIVE_TAURUS);
    //m_model->LoadDepthMap("G:/__Images_DepthSet/asian_woman_tex1_depth_low.tif");

    //bindTexture(QPixmap(QString("side1.png")), GL_TEXTURE_2D);
    m_model->LoadDepthMap("test_depth.png");

    m_time.start();
}

WspQGraphicsScene::~WspQGraphicsScene(){
    //delete m_modelButton;
    //delete m_depthMapButton;
}



void WspQGraphicsScene::setDisplayType(){
    switch(display_type_combobox_->currentIndex()){
    case 0:
        m_model->set_display_type(wsp::graphic::POLY_DISPLAY_POINTS);
        break;
    case 1:
        m_model->set_display_type(wsp::graphic::POLY_DISPLAY_WIREFRAME);
        break;
    case 2:
        m_model->set_display_type(wsp::graphic::POLY_DISPLAY_TRIANGLES);
        break;
    }
}

void WspQGraphicsScene::slot_loadModel(){
    m_model->LoadObjFileDialog();
}

void WspQGraphicsScene::slot_changeDisplayType(int type){
    printf("display type index: %d\n", type);
    setDisplayType();
    update();
}


void WspQGraphicsScene::drawBackground(QPainter *painter, const QRectF &)
{
    if (painter->paintEngine()->type() != QPaintEngine::OpenGL
        && painter->paintEngine()->type() != QPaintEngine::OpenGL2)
    {
        qWarning("WspQGraphicsScene: drawBackground needs a QGLWidget to be set as viewport on the graphics view");
        return;
    }

    painter->beginNativePainting();

    glClearColor(m_backgroundColor.redF(), m_backgroundColor.greenF(), m_backgroundColor.blueF(), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_model) {
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluPerspective(70, width() / height(), 0.01, 1000);

        gluLookAt(
            0.0f, 0.0f, camera_distance_, /* camera position */
            0.0f, 0.0f, 0.0f, /* target position */
            0.0f, 1.0f, 0.0f  /* upper vector */
        );

        glTranslatef( camera_hor_move_,  camera_ver_move_, 0.0f);
        glRotatef( camera_pitch_, 1.0f, 0.0f, 0.0f );
        glRotatef( camera_yaw_, 0.0f, 1.0f, 0.0f );


        glMatrixMode(GL_MODELVIEW);


        //** --------------------------------
        //** draw model
        glPushMatrix();
        {
            glLoadIdentity();

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture_);

            const float light_pos[] = { m_lightItem->x() - width() / 2, height() / 2 - m_lightItem->y(), 1, 0 };
            glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
            glColor4f(m_modelColor.redF(), m_modelColor.greenF(), m_modelColor.blueF(), 1.0f);

            const int delta = m_time.elapsed() - m_lastTime;
            m_rotation += m_angularMomentum * (delta / 1000.0);
            m_lastTime += delta;

            glTranslatef(0, 0, -m_distance);
            glRotatef(m_rotation.x(), 1, 0, 0);
            glRotatef(m_rotation.y(), 0, 1, 0);
            glRotatef(m_rotation.z(), 0, 0, 1);

            glEnable(GL_MULTISAMPLE);
            {
                m_model->Render(m_normalsEnabled);
            }
            glDisable(GL_MULTISAMPLE);
        }
        glPopMatrix();
        //** --------------------------------


        //** --------------------------------
        //** draw axis
        static float axis_pos[] = {0.0f, 0.0f, 0.0f};
        glPushMatrix();
        {
            glTranslatef(axis_pos[0], axis_pos[1], axis_pos[2]);
            glBegin(GL_LINES);
            {
                glColor3f ( 1.0f, 0.0f, 0.0f );
                glVertex3f( 1.0f, 0.0f, 0.0f );
                glVertex3f( 0.0f, 0.0f, 0.0f );

                glColor3f ( 0.0f, 1.0f, 0.0f );
                glVertex3f( 0.0f, 1.0f, 0.0f );
                glVertex3f( 0.0f, 0.0f, 0.0f );

                glColor3f ( 0.0f, 0.0f, 1.0f );
                glVertex3f( 0.0f, 0.0f, 1.0f );
                glVertex3f( 0.0f, 0.0f, 0.0f );
            }
            glEnd();
        }
        glPopMatrix();
        //** --------------------------------

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
    }

    painter->endNativePainting();

    QTimer::singleShot(20, this, SLOT(update()));
}
//
//static wsp::PolyModel *loadModel(const QString &filePath)
////static Model *loadModel(const QString &filePath)
//{
//    return new wsp::PolyModel(filePath.toLatin1().data());
//    //return new Model(filePath);
//}

void WspQGraphicsScene::slot_loadDepthMap()
{
    wsp::graphic::DepthToMeshParam d2m_param;
    memcpy(&d2m_param, &wsp::graphic::DEFAULT_DEPTH_TO_MESH_PARAM, sizeof(wsp::graphic::DepthToMeshParam));

    d2m_param.invert_depth = true;
    d2m_param.face_depth_thre = 6.0f;

    m_model->LoadDepthMapDialog(&d2m_param, "Open Depth Map");

    m_labels[1]->setText(tr("Points: %0").arg(m_model->GetNumPoints()));
    m_labels[2]->setText(tr("Edges: %0").arg(m_model->GetNumEdges()));
    m_labels[3]->setText(tr("Faces: %0").arg(m_model->GetNumFaces()));

    setDisplayType();
    update();
}

//void WspQGraphicsScene::loadModel(){
//    m_model->LoadObjFileDialog();
//    //loadModel(QFileDialog::getOpenFileName(0, tr("Choose model"), QString(), QLatin1String("*.obj")));
//}
//
//void WspQGraphicsScene::loadModel(const QString &filePath)
//{
//    if (filePath.isEmpty()){
//        return;
//    }
//
//    m_modelButton->setEnabled(false);
//    QApplication::setOverrideCursor(Qt::BusyCursor);
//    m_model->LoadObjFile(filePath.toLatin1().data());
////#ifndef QT_NO_CONCURRENT
////    m_modelLoader.setFuture(QtConcurrent::run(::loadModel, filePath));
////#else
////    setModel(::loadModel(filePath));
//    modelLoaded();
////#endif
//}

//void WspQGraphicsScene::modelLoaded()
//{
////#ifndef QT_NO_CONCURRENT
////    setModel(m_modelLoader.result());
////#endif
//    m_modelButton->setEnabled(true);
//    QApplication::restoreOverrideCursor();
//}

//void WspQGraphicsScene::setModel(wsp::PolyModel *model)
////void WspQGraphicsScene::setModel(Model *model)
//{
//    delete m_model;
//    m_model = model;
//
//    ////m_labels[0]->setText(tr("File: %0").arg(m_model->fileName()));
//    m_labels[1]->setText(tr("Points: %0").arg(m_model->GetNumPoints()));
//    m_labels[2]->setText(tr("Edges: %0").arg(m_model->GetNumEdges()));
//    m_labels[3]->setText(tr("Faces: %0").arg(m_model->GetNumFaces()));
//    //m_labels[1]->setText(tr("Points: %0").arg(m_model->points()));
//    //m_labels[2]->setText(tr("Edges: %0").arg(m_model->edges()));
//    //m_labels[3]->setText(tr("Faces: %0").arg(m_model->faces()));
//    setDisplayType();
//
//    update();
//}

void WspQGraphicsScene::enableWireframe(bool enabled)
{
    m_wireframeEnabled = enabled;
    update();
}

void WspQGraphicsScene::enableNormals(bool enabled)
{
    m_normalsEnabled = enabled;
    update();
}

void WspQGraphicsScene::setModelColor()
{
    const QColor color = QColorDialog::getColor(m_modelColor);
    if (color.isValid()) {
        m_modelColor = color;
        update();
    }
}

void WspQGraphicsScene::setBackgroundColor()
{
    const QColor color = QColorDialog::getColor(m_backgroundColor);
    if (color.isValid()) {
        m_backgroundColor = color;
        update();
    }
}

void WspQGraphicsScene::keyPressEvent(QKeyEvent *event){
    //printf("Key %d pressed\n", event->key());
    switch(event->key()){
    case Qt::Key_Alt:
        camera_move_switch_ = true;
        //printf("ALT key pressed\n");
        break;
    default:
        break;
    }
}
void WspQGraphicsScene::keyReleaseEvent(QKeyEvent *event){
    //printf("Key %d released\n", event->key());
    switch(event->key()){
    case Qt::Key_Alt:
        camera_move_switch_ = false;
        //printf("ALT key released\n");
        break;
    default:
        break;
    }
}


void WspQGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
    if (event->isAccepted()){
        return;
    }

    const QPointF delta = event->scenePos() - event->lastScenePos();
    if(camera_move_switch_==false){
        if (event->buttons() & Qt::LeftButton) {
            const wsp::Point<float> angularImpulse = wsp::Point<float>(-delta.y(), delta.x(), 0) * 0.1;

            m_rotation += angularImpulse;
            m_accumulatedMomentum += angularImpulse;
        }
    }else{
        if (event->buttons() & Qt::LeftButton) {
            //** aiming rotation animation
            camera_yaw_ += delta.x();
            camera_pitch_ -= delta.y();
            //printf("delta: %.2f %.2f\n", delta.x(), delta.y());
        }else if(event->buttons() & Qt::MiddleButton){
            //** dolly animation
            camera_hor_move_ -= delta.x() * 0.01f;
            camera_ver_move_ -= delta.y() * 0.01f;
            //printf("cam move: %.2f %.2f\n", camera_hor_move_, camera_ver_move_);
        }else if(event->buttons() & Qt::RightButton){
            //** dolly in, out animation
            camera_distance_ *= pow(1.2f, -(float)(delta.x()+delta.y())*0.02f);
        }
    }

    
    update();
    event->accept();
}

static bool dragging = false;
void WspQGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    if (event->isAccepted()){
        return;
    }

    m_mouseEventTime = m_time.elapsed();
    m_angularMomentum = m_accumulatedMomentum = wsp::Point<float>();

    dragging = true;

    event->accept();
}

void WspQGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
    if (event->isAccepted()){
        return;
    }

    const int delta = m_time.elapsed() - m_mouseEventTime;
    m_angularMomentum = m_accumulatedMomentum * (1000.0 / qMax(1, delta));
    
    dragging = false;

    event->accept();
    update();
}

void WspQGraphicsScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    QGraphicsScene::wheelEvent(event);
    if (event->isAccepted())
        return;

    //m_distance *= qPow(1.2, -event->delta() / 120);
    if(dragging==false){
        camera_distance_ *= pow(1.2f, -event->delta() / 120.0f);
    }

    event->accept();
    update();
}
