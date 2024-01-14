/**
 * @file QtGLWidget.cpp
 * @author Jun-ichi Nishikata
 */


#include <iostream>

#include "_define_gui.h"

#include "cl-qt_glwidget.hpp"

#include <wsp/common/_define_global.h>
#include <wsp/image/tpl_cl-image.h>
#include <wsp/opengl/wrap_fn-gl.h>
#include <wsp/graphics/st-depth_to_mesh_param.h>


#include "fn-gui_io.h"

using namespace std;

namespace{
    /*
    ** 光源
    */
    static const GLfloat lightpos[] = { 0.0, 0.0, 1.0, 0.0 }; /* 位置　　　 */
    static const GLfloat lightcol[] = { 1.0, 1.0, 1.0, 1.0 }; /* 直接光強度 */
    static const GLfloat lightamb[] = { 0.1, 0.1, 0.1, 1.0 }; /* 環境光強度 */

    static QPointF last_pos = QPointF(0,0);

    static GLint texture, cubemap;

    static float rotAngle = 0.0;
    int	giAntiAlias		= 0;
    int	giMultiSample	= 0;

    int	giWindowWidth = 800;
    int giWindowHeight = 600;
    int	giHalfWindowWidth = giWindowWidth / 2;
    int	giHalfWindowHeight = giWindowHeight / 2;


    QDialog *CreateQDialog(const QString &windowTitle)
    {
        QDialog *dialog = new QDialog(0, Qt::CustomizeWindowHint | Qt::WindowTitleHint);

        dialog->setWindowOpacity(0.8);
        dialog->setWindowTitle(windowTitle);
        dialog->setLayout(new QVBoxLayout);

        return dialog;
    }

    static int   giTextPos = 0;
    static int   giTextPosDelta = 30;
    static float gfTextScale = 0.15;
    //
    // Draw text to screen
    //
    void DrawText(char *strText)
    {   
        char *pChar = NULL;
 
        glPushMatrix();
        glTranslatef(10.0, giTextPos, 0.0);
        glScalef(gfTextScale, gfTextScale, gfTextScale);

        //glEnable(GL_LINE_SMOOTH);  
        for(pChar = strText; *pChar; pChar++) {
	        glutStrokeCharacter(GLUT_STROKE_ROMAN, *pChar); 
        }
        //glDisable(GL_LINE_SMOOTH);  
        glPopMatrix();

        // Update text position
        giTextPos -= giTextPosDelta;
    }
}

class wsp::QtGLWidget::Impl
{
public:
    enum TestObject{
       TEST_OBJECT_NONE,
       TEST_OBJECT_SPHERE,
       TEST_OBJECT_TEAPOT,
    };
public:
    Impl(QWidget *parent)
        :
#ifdef _DEBUG
          test_obj_to_draw_(TEST_OBJECT_TEAPOT)
#else
          test_obj_to_draw_(TEST_OBJECT_NONE)
#endif
        , drawAxis_  (true)
        , current_shader_id_(0)

        /* init menus */
        , pop_menu_              (new QMenu(QString("Popup Menu"), parent))
        , file_menu_             (new QMenu(QString("File"), parent))
        , disp_type_menu_        (new QMenu(QString("Display Type"), parent))
        , shading_menu_          (new QMenu(QString("Shading"), parent ))
        , shading_envmap_menu_   (new QMenu(QString("Environment Map"), parent ))
        , test_menu_             (new QMenu(QString("Test"), parent ))

        /* init actions */
        , open_depth_act_        (new QAction(QString("&Open Depth Map"), parent))
        , open_model_act_        (new QAction(QString("&Open 3D Model"), parent))
        , open_texture_act_      (new QAction(QString("Open Image as Texture"), parent))
        , open_shader_act_       (new QAction(QString("Open Shader"), parent))
        , disp_type_triangle_    (new QAction(QString("Triangles"), parent))
        , disp_type_vertex_      (new QAction(QString("Verteces"), parent))
        , disp_type_wireframe_   (new QAction(QString("Wireframe"), parent))
        , shading_envmap_off_    (new QAction(QString("Off"), parent))
        , shading_envmap_cubemap_(new QAction(QString("Cube Map"), parent))
        , test_draw_off_         (new QAction(QString("Off"), parent))
        , test_draw_sphere_      (new QAction(QString("Draw Sphere"), parent))
        , test_draw_teapot_      (new QAction(QString("Draw Teapot"), parent))

        /* init GUI */
        , polymodel_btn_        (new QPushButton(QString("Load model")))
        , controls              (CreateQDialog(QString("Controls")))
        , statistics            (CreateQDialog(QString("Model info")))
        , instructions          (CreateQDialog(QString("Instructions")))
        , depth_map_btn_        (new QPushButton(QString("Load Depth Map")))
        , display_type_combobox_(new QComboBox())
        , wireframe             (new QCheckBox(QString("Render as wireframe")))
        , normals               (new QCheckBox(QString("Display normals vectors")))
        , colorButton           (new QPushButton(QString("Choose model color")))
        , backgroundButton      (new QPushButton(QString("Choose background color")))
        , instr_label0_         (new QLabel(QString("Use mouse wheel to zoom model, and click and drag to rotate model")))
        , instr_label1_         (new QLabel(QString("Move the sun around to change the light position")))
        , m_lightItem           (new QGraphicsRectItem(0, 0, 80, 80))
    {
        parent->connect(polymodel_btn_, SIGNAL(clicked()), parent, SLOT(slot_loadModel()));
        parent->connect(depth_map_btn_, SIGNAL(clicked()), parent, SLOT(slot_loadDepthMap()));


        display_type_combobox_->addItem(QString("Points"));
        display_type_combobox_->addItem(QString("Wireframe"));
        display_type_combobox_->addItem(QString("Triangle"));
        parent->connect(display_type_combobox_, SIGNAL(currentIndexChanged(int)), parent, SLOT(slot_changeDisplayType(int)));



        parent->connect(wireframe,        SIGNAL(toggled(bool)), parent, SLOT(enableWireframe(bool)));
        parent->connect(normals,          SIGNAL(toggled(bool)), parent, SLOT(enableNormals(bool)));
        parent->connect(colorButton,      SIGNAL(clicked()),     parent, SLOT(setModelColor()));
        parent->connect(backgroundButton, SIGNAL(clicked()),     parent, SLOT(setBackgroundColor()));

    
        statistics->layout()->setMargin(20);

        controls->layout()->addWidget(polymodel_btn_);
        controls->layout()->addWidget(depth_map_btn_);
        controls->layout()->addWidget(display_type_combobox_);
        controls->layout()->addWidget(wireframe);
        controls->layout()->addWidget(normals);
        controls->layout()->addWidget(colorButton);
        controls->layout()->addWidget(backgroundButton);



        for (int i = 0; i < NUM_OF_LABELS; ++i) {
            m_labels[i] = new QLabel;
            statistics->layout()->addWidget(m_labels[i]);
        }

        instructions->layout()->addWidget(instr_label0_);
        instructions->layout()->addWidget(instr_label1_);

        QWidget *widgets[] = { instructions, controls, statistics };

    
        //QGraphicsProxyWidget *proxy_list[3];
        //for (int i = 0; i < 3; ++i) {
        //    //proxy_list[i] = new QGraphicsProxyWidget(0, Qt::Dialog);
        //    //proxy_list[i]->setWidget(widgets[i]);
        //    main_layout_->addWidget(widgets[i]);
        //}

        //QPointF pos(10, 10);
        //for(int i=0; i<3; ++i) {
        //    QGraphicsItem *item = proxy_list[i];
        //    item->setFlag(QGraphicsItem::ItemIsMovable);
        //    item->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

        //    const QRectF rect = item->boundingRect();
        //    item->setPos(pos.x() - rect.x(), pos.y() - rect.y());
        //    pos += QPointF(0, 10 + rect.height());
        //}

        QRadialGradient gradient(40, 40, 40, 40, 40);
        gradient.setColorAt(0.2, Qt::yellow);
        gradient.setColorAt(1, Qt::transparent);

        m_lightItem->setPen(Qt::NoPen);
        m_lightItem->setBrush(gradient);
        m_lightItem->setFlag(QGraphicsItem::ItemIsMovable);
        m_lightItem->setPos(700, 440);
        //addItem(m_lightItem);

        //loadModel(QLatin1String("qt.obj"));


        //polymodel_->LoadObjFile("qt.obj");
        //polymodel_->CreatePrimitive(wsp::graphic::POLY_PRIMITIVE_TAURUS);
        //polymodel_->LoadDepthMap("G:/__Images_DepthSet/asian_woman_tex1_depth_low.tif");
        /* set up actions */
        {
            // File Menu --------------------------------------------------
            // open
            open_depth_act_->setShortcut(QString("Ctrl+D"));
            parent->connect(this->open_depth_act_,   SIGNAL(triggered()), parent, SLOT(slot_loadDepthMap()));
            parent->connect(this->open_model_act_,   SIGNAL(triggered()), parent, SLOT(slot_loadModel()));
            parent->connect(this->open_texture_act_, SIGNAL(triggered()), parent, SLOT(slot_loadTexture()));
            parent->connect(this->open_shader_act_,  SIGNAL(triggered()), parent, SLOT(slot_loadShader()));

            // display type
            disp_type_triangle_->setCheckable(true);
            disp_type_vertex_->setCheckable(true);
            disp_type_triangle_->setChecked(true);
            disp_type_wireframe_->setCheckable(true);
            parent->connect(this->disp_type_triangle_,  SIGNAL(triggered()), parent, SLOT(slot_triangleDisp()));
            parent->connect(this->disp_type_vertex_,    SIGNAL(triggered()), parent, SLOT(slot_vertexDisp()));
            parent->connect(this->disp_type_wireframe_, SIGNAL(triggered()), parent, SLOT(slot_wireframeDisp()));

            // shading menu
            shading_envmap_off_->setCheckable(true);
            shading_envmap_cubemap_->setCheckable(true);
            shading_envmap_off_->setChecked(true);
            parent->connect(this->shading_envmap_off_,     SIGNAL(triggered()), parent, SLOT(slot_disableEnvmap()));
            parent->connect(this->shading_envmap_cubemap_, SIGNAL(triggered()), parent, SLOT(slot_enableEnvmapCubemap()));

            // test menu
            test_draw_off_->setCheckable(true);
            test_draw_sphere_->setCheckable(true);
            test_draw_teapot_->setCheckable(true);
            parent->connect(this->test_draw_off_,    SIGNAL(triggered()), parent, SLOT(slot_testDrawOff()));
            parent->connect(this->test_draw_sphere_, SIGNAL(triggered()), parent, SLOT(slot_testDrawSphere()));
            parent->connect(this->test_draw_teapot_, SIGNAL(triggered()), parent, SLOT(slot_testDrawTeapot()));
#ifdef _DEBUG
            test_draw_teapot_->setChecked(true);
#else
            test_draw_off_->setChecked(true);
#endif
        }

        /* set up menu */
        {
            file_menu_->addAction(open_depth_act_);
            file_menu_->addAction(open_model_act_);
            file_menu_->addAction(open_depth_act_);
            file_menu_->addAction(open_texture_act_);
            file_menu_->addAction(open_shader_act_);

            disp_type_menu_->addAction(disp_type_vertex_);
            disp_type_menu_->addAction(disp_type_wireframe_);
            disp_type_menu_->addAction(disp_type_triangle_);

            shading_envmap_menu_->addAction( shading_envmap_off_);
            shading_envmap_menu_->addAction( shading_envmap_cubemap_);

            test_menu_->addAction( test_draw_off_ );
            test_menu_->addAction( test_draw_sphere_ );
            test_menu_->addAction( test_draw_teapot_ );

            shading_menu_->addMenu( shading_envmap_menu_ );
            pop_menu_->addMenu( file_menu_);
            pop_menu_->addSeparator();
            pop_menu_->addMenu( disp_type_menu_);
            pop_menu_->addMenu( shading_menu_ );
            pop_menu_->addMenu( test_menu_ );
        }
    }
    ~Impl()
    {
        /* free actions */
        {
            /* actions */
            delete disp_type_menu_;
            delete open_depth_act_;
            delete open_model_act_;
            delete open_texture_act_;
            delete open_shader_act_;
            delete disp_type_triangle_;
            delete disp_type_vertex_;
            delete disp_type_wireframe_;
            delete shading_envmap_off_;
            delete shading_envmap_cubemap_;

            /* menus */
            delete test_menu_;
            delete shading_envmap_menu_;
            delete shading_menu_;
            delete file_menu_;
            delete pop_menu_;
        }

        for (int i = 0; i < NUM_OF_LABELS; ++i) {
            delete m_labels[i];
        }

        /* free GUI */
        {
            delete polymodel_btn_;
            delete depth_map_btn_;
            delete display_type_combobox_;
            delete wireframe;
            delete normals;
            delete colorButton;
            delete backgroundButton;
            delete instr_label0_;
            delete instr_label1_;
            delete m_lightItem;
        }

        /* free GUI widgets */
        {
            delete (QVBoxLayout*)(controls->layout());
            delete controls;
            delete (QVBoxLayout*)(instructions->layout());
            delete instructions;
            delete (QVBoxLayout*)(statistics->layout());
            delete statistics;
        }
    }


    void uncheckAllDisplayType();
    void uncheckAllEnvmapType();
    void uncheckAllTestDrawObject();
    void loadShader(const char *vsh_file, const char *fsh_file);


public:
    /* ----------------------------
                  data 
       ---------------------------- */
    bool drawAxis_;
    TestObject test_obj_to_draw_;

    /* ----------------------------
              shader data 
       ---------------------------- */
    /* currently loaded shader program ID */
    GLint current_shader_id_; 
        
    GLuint texture_;
    GLint uni_tex_loc_[MAX_TEXTURE_NUM];
    GLint uni_time_loc_;
    GLint uni_proj_mtx_loc_;
    GLint uni_view_mtx_loc_;
    GLint uni_model_mtx_loc_;
    GLint in_pos_loc_;
    GLint in_col_loc_, o_col_loc_;
    float proj_mtx_[16];
    float view_mtx_[16];

    /* ----------------------------
               GUI data 
       ---------------------------- */
    QLabel *m_labels[NUM_OF_LABELS];
    QPushButton *polymodel_btn_;
    QPushButton *depth_map_btn_;
    QComboBox *display_type_combobox_;

    QCheckBox *wireframe;
    QCheckBox *normals;
    QPushButton *colorButton;
    QPushButton *backgroundButton;
    QLabel *instr_label0_, *instr_label1_;

    QGraphicsRectItem *m_lightItem;

    // layout
    QVBoxLayout *vbox_layout_;

    // dialog
    QWidget *statistics;
    QWidget *controls;
    QWidget *instructions;
    

    // Menus and Actions ----------------------
    // context menu
    QMenu *pop_menu_;

    // File
    QMenu *file_menu_;
    QAction *open_depth_act_;
    QAction *open_model_act_;
    QAction *open_texture_act_; 
    QAction *open_shader_act_;

    QMenu *disp_type_menu_;
    QAction *disp_type_triangle_;
    QAction *disp_type_vertex_;
    QAction *disp_type_wireframe_;

    QMenu *shading_menu_;
    QMenu *shading_envmap_menu_;
    QAction *shading_envmap_off_;
    QAction *shading_envmap_cubemap_;

    QMenu *test_menu_;
    QAction *test_draw_off_;
    QAction *test_draw_teapot_;
    QAction *test_draw_sphere_;

};

void wsp::QtGLWidget::Impl::uncheckAllDisplayType()
{
    disp_type_triangle_->setChecked(false);
    disp_type_vertex_->setChecked(false);
    disp_type_wireframe_->setChecked(false);
}

void wsp::QtGLWidget::Impl::uncheckAllEnvmapType()
{
    shading_envmap_off_->setChecked(false);
    shading_envmap_cubemap_->setChecked(false);
    glDisable(GL_TEXTURE_CUBE_MAP);
}

void wsp::QtGLWidget::Impl::uncheckAllTestDrawObject()
{
    this->test_draw_off_->setChecked(false);
    this->test_draw_sphere_->setChecked(false);
    this->test_draw_teapot_->setChecked(false);
}

void wsp::QtGLWidget::Impl::loadShader(const char *vsh_file, const char *fsh_file)
{
    current_shader_id_ = wsp::opengl::GlLoadShaderFile(vsh_file, fsh_file);

    /* uniform 変数の位置を取り出す */
    texture            = glGetUniformLocation( current_shader_id_, "texture");
    cubemap            = glGetUniformLocation( current_shader_id_, "cubemap");
    texture_           = glGetUniformLocation( current_shader_id_, "texture0"); 
    uni_tex_loc_[0]    = glGetUniformLocation( current_shader_id_, "u_texture0");
    uni_tex_loc_[1]    = glGetUniformLocation( current_shader_id_, "u_texture1");
    uni_time_loc_      = glGetUniformLocation( current_shader_id_, "u_time");
    uni_time_loc_      = glGetUniformLocation( current_shader_id_, "u_time");
    uni_proj_mtx_loc_  = glGetUniformLocation( current_shader_id_, "u_proj_mtx");
    uni_view_mtx_loc_  = glGetUniformLocation( current_shader_id_, "u_view_mtx");
    uni_model_mtx_loc_ = glGetUniformLocation( current_shader_id_, "u_model_mtx");
    {
        GLint location = glGetUniformLocation( current_shader_id_, "u_envmap_reflect_ratio");
        if( location!=-1 )
        {
            glUniform1f(location, 0.5f);
        }
    }

    in_pos_loc_ = glGetAttribLocation( current_shader_id_, "in_position");
    in_col_loc_ = glGetAttribLocation( current_shader_id_, "in_color");
    o_col_loc_  = glGetAttribLocation( current_shader_id_, "o_color");
}




QSize wsp::QtGLWidget::minimumSizeHint() const
{
    return QSize(200, 100);
}

QSize wsp::QtGLWidget::sizeHint() const
{
    return QSize(800, 700);
}



void wsp::QtGLWidget::slot_vertexDisp()
{
    int num_model = polymodel_list_->length();
    for(int i=0; i<num_model; ++i){
        (*polymodel_list_)[i].set_display_type(wsp::graphic::POLY_DISPLAY_POINTS);
    }
    pImpl_->uncheckAllDisplayType();
    pImpl_->disp_type_vertex_->setChecked(true);
}

void wsp::QtGLWidget::slot_wireframeDisp()
{
    int num_model = polymodel_list_->length();
    for(int i=0; i<num_model; ++i){
        (*polymodel_list_)[i].set_display_type(wsp::graphic::POLY_DISPLAY_WIREFRAME);
    }
    pImpl_->uncheckAllDisplayType();
    pImpl_->disp_type_wireframe_->setChecked(true);
}

void wsp::QtGLWidget::slot_triangleDisp()
{
    int num_model = polymodel_list_->length();
    for(int i=0; i<num_model; ++i){
        (*polymodel_list_)[i].set_display_type(wsp::graphic::POLY_DISPLAY_TRIANGLES);
    }
    pImpl_->uncheckAllDisplayType();
    pImpl_->disp_type_triangle_->setChecked(true);
}

void wsp::QtGLWidget::slot_disableEnvmap()
{
    pImpl_->uncheckAllEnvmapType();
    pImpl_->shading_envmap_off_->setChecked(true);
}

void wsp::QtGLWidget::slot_enableEnvmapCubemap()
{
    pImpl_->uncheckAllEnvmapType();
    pImpl_->shading_envmap_cubemap_->setChecked(true);
    glEnable(GL_TEXTURE_CUBE_MAP);
}

void wsp::QtGLWidget::assignTexture( const wsp::Image<uchar> &in_image )
{
    glDeleteTextures(MAX_TEXTURE_NUM, tex_obj_);

    glGenTextures(MAX_TEXTURE_NUM, tex_obj_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_obj_[0]);

    GLint level_of_detail = 0;
    GLint border = 0;
    switch(in_image.num_of_channels())
    {
    case 1:
        WSP_GUI_DEBUG_LOG("Texture type: Luminance\n");
        {
            glTexImage2D(
                GL_TEXTURE_2D, level_of_detail, 
                GL_LUMINANCE, 
                in_image.width(), in_image.height(), border, 
                GL_LUMINANCE, 
                GL_UNSIGNED_BYTE, 
                in_image.data()
            );
        }
        break;
    case 3:
        WSP_GUI_DEBUG_LOG("Texture type: RGB\n");
        {
            glTexImage2D(
                GL_TEXTURE_2D, level_of_detail, 
                GL_RGB, 
                in_image.width(), in_image.height(), border, 
                GL_RGB, 
                GL_UNSIGNED_BYTE, 
                in_image.data()
            );
        }
        break;
    case 4:
        WSP_GUI_DEBUG_LOG("Texture type: RGBA\n");
        {
            glTexImage2D(
                GL_TEXTURE_2D, level_of_detail, 
                GL_RGBA, 
                in_image.width(), in_image.height(), border, 
                GL_RGBA, 
                GL_UNSIGNED_BYTE, 
                in_image.data()
            );
        }
        break;
    }

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void wsp::QtGLWidget::slot_loadTexture()
{
    // Create texture -------------------------------
    wsp::Image<uchar> image;
    wsp::State state = image.LoadImageFileDialog(3, "Open Image as Texture");
    if(state!=WSP_STATE_SUCCESS){ return; }

    assignTexture(image);
}

void wsp::QtGLWidget::slot_setEnvmapRatio(int value)
{
    GLint location = glGetUniformLocation( pImpl_->current_shader_id_, "u_envmap_reflect_ratio");
    if( location != -1 )
    {
        glUseProgram( pImpl_->current_shader_id_ );
        GLfloat set_value = value/10000.0f;
        glUniform1f(location, set_value);
    }
}

void wsp::QtGLWidget::assignTextures( const wsp::Vector<wsp::Image<uchar>> &in_image_list )
{
    int num_tex = in_image_list.length();
    if( num_tex >= GL_MAX_TEXTURE_UNITS-2 ){
        WSP_COMMON_ERROR_LOG("Too many textures are given\n");
        return;
    }
    glDeleteTextures(num_tex, tex_obj_list_.data());
    glGenTextures(num_tex, tex_obj_list_.data());

    GLint level_of_detail = 0;
    GLint border = 0;

    for( int i=0; i<num_tex; ++i )
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, tex_obj_list_[i]);
        switch(in_image_list[i].num_of_channels())
        {
        case 1:
            WSP_GUI_DEBUG_LOG("Texture type: Luminance\n");
            {
                glTexImage2D(
                    GL_TEXTURE_2D, level_of_detail, 
                    GL_LUMINANCE, 
                    in_image_list[i].width(), in_image_list[i].height(), border, 
                    GL_LUMINANCE, 
                    GL_UNSIGNED_BYTE, 
                    in_image_list[i].data()
                );
            }
            break;
        case 3:
            WSP_GUI_DEBUG_LOG("Texture type: RGB\n");
            {
                glTexImage2D(
                    GL_TEXTURE_2D, level_of_detail, 
                    GL_RGB, 
                    in_image_list[i].width(), in_image_list[i].height(), border, 
                    GL_RGB, 
                    GL_UNSIGNED_BYTE, 
                    in_image_list[i].data()
                );
            }
            break;
        case 4:
            WSP_GUI_DEBUG_LOG("Texture type: RGBA\n");
            {
                glTexImage2D(
                    GL_TEXTURE_2D, level_of_detail, 
                    GL_RGBA, 
                    in_image_list[i].width(), in_image_list[i].height(), border, 
                    GL_RGBA, 
                    GL_UNSIGNED_BYTE, 
                    in_image_list[i].data()
                );
            }
            break;
        }
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    }

}

void wsp::QtGLWidget::slot_loadShader()
{
    //WSP_COMMON_DEBUG_LOG("Opening shader dialog\n");
    //// Create shader -----------------------------------
    //pImpl_->current_shader_id_ = wsp::graphic::GlLoadShaderFileDialog();

    char currentDir[MAX_LENGTH_OF_PATH];
    char fsh_file[MAX_LENGTH_OF_PATH], vsh_file[MAX_LENGTH_OF_PATH];

    strcpy(vsh_file, QFileDialog::getOpenFileName(NULL, QString("Open Vertex Shader File"), QDir::currentPath(), QString("GLSL Vertex Shader File(*.vs *.vsh);;All files (*)")).toLatin1().data());
    wsp::GetDir(currentDir, vsh_file);
    if(QDir::setCurrent(QString(currentDir))==false){ 
        WSP_COMMON_ERROR_LOG("setCurrent failed\n"); return;
    }

    char filename[MAX_LENGTH_OF_PATH];
    wsp::RemoveExtension(filename, vsh_file);
    sprintf(fsh_file, "%s.fs", filename);
    FILE *fp = fopen(fsh_file,"r");
    if(fp==NULL)
    {
        WSP_COMMON_DEBUG_LOG("%s does not exist\n", fsh_file);
        strcpy(fsh_file, QFileDialog::getOpenFileName(NULL, QString("Open Fragment Shader File"), QDir::currentPath(), QString("GLSL Fragment Shader File(*.fs *.fsh);;All files (*)")).toLatin1().data());
        wsp::GetDir(currentDir, fsh_file);
        if(QDir::setCurrent(QString(currentDir))==false){ 
            WSP_COMMON_ERROR_LOG("setCurrent failed\n"); return; 
        }
    }
    fclose(fp);
    pImpl_->loadShader(vsh_file, fsh_file);
}


void wsp::QtGLWidget::initializeGL()
{
    WSP_GUI_DEBUG_LOG("Initialize GL\n");
    int width = 640;
    int height = 480;
    float aspect = (float)width / (float)height;
    int major, minor;

    //* OpenGL version check */
    char *version = (char *)glGetString(GL_VERSION);

    sscanf(version, "%d.%d", &major, &minor);
    WSP_GUI_DEBUG_LOG("OpenGL version %s\n", version);
    if (major < 2) {
        WSP_GUI_ERROR_LOG("Need OpenGL 2.0 or later.\n");
        return;
    }



    // //////////////////////////////////////////////
    //             INIT Environment
    // //////////////////////////////////////////////
    //* Setting for viewport, camera */
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //*
    //    GL.glOrtho(left , right , bottom , top , near , far）
    //    引数はすべてDouble型
    //    正射影で描画範囲を指定する。3つのコマンドで、唯一nearにマイナスを使える。視点を移動しても、大きさは反映されない（大きさが視点からの距離に依存しないため）。視点を移動する場合、nearとfarの値は+にしたほうが混乱しにくいかもしれない。
    //    left    描画範囲の左端。視線を0として+-で入力。
    //    right    描画範囲の右端。視線を0として+-で入力。
    //    bottom    描画範囲の上端。視線を0として+-で入力。
    //    top    描画範囲の下端。視線を0として+-で入力。
    //    near    視点からどれだけ離れた位置から表示するか。+-で入力。
    //    farよりも大きな値やマイナスでもエラーにはならないが、混乱しやすいので注意（farとの大小が逆転すると、描画方向も変わる）。
    //    far    視点からどれだけ離れた位置まで表示するか。+-で入力。
    //*/
    glOrtho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // background color
    glClearColor( 0.4f, 0.4f, 0.4f, 0.0f );

    // Enable depth test
    glEnable( GL_DEPTH_TEST );
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

    // Enable culling
    {
        //glCullFace( GL_BACK );
        //glEnable( GL_CULL_FACE );
        //glDisable( GL_CULL_FACE );
    }

    /* anti-aliasing */
    {
        glEnable(GL_MULTISAMPLE);
    }

    // Set up material shading by glColor
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Be enable material color info
    glEnable(GL_COLOR_MATERIAL);

    /* 光源の初期設定 */
    {
        // Enable lighting
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        // Create light
        float  light0_position[] = { 10.0f, 10.0f, 10.0f, 1.0f };
        float  light0_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
        float  light0_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        float  light0_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
        glLightfv( GL_LIGHT0, GL_POSITION, light0_position );
        glLightfv( GL_LIGHT0, GL_DIFFUSE, light0_diffuse );
        glLightfv( GL_LIGHT0, GL_SPECULAR, light0_specular );
        glLightfv( GL_LIGHT0, GL_AMBIENT, light0_ambient );
    }


#ifndef QT_OPENGL_ES_2
    glEnable(GL_TEXTURE_2D);
#endif

    //bindTexture(QPixmap(QString("side1.png")), GL_TEXTURE_2D);
    //int num_model = polymodel_list_->length();
    //for(int i=0; i<num_model; ++i){
    //    (*polymodel_list_)[i].LoadDepthMap("test_depth.png");
    //}


    // Create default shader -----------------------------------
    {
        std::string shader_dir = std::string(WSP_EXE_DIR_PATH) + "shaders/";
        std::string vsh_file = shader_dir + std::string("default.vs");
        std::string fsh_file = shader_dir + std::string("default.fs");

        pImpl_->loadShader(vsh_file.c_str(), fsh_file.c_str());
    }



    /* copyied from cube map sample */
    {
        std::string resource_dir = std::string(WSP_EXE_DIR_PATH) + "resource/";
        /*
        ** テクスチャ
        */
        const int TEXWIDTH  = 128; /* テクスチャの幅　　　 */
        const int TEXHEIGHT = 128; /* テクスチャの高さ　　 */


        /* テクスチャ画像はバイト単位に詰め込まれている */
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  
        const char *cubemap_textures[] = { /* テクスチャファイル名 */
            "room2nx.raw",
            "room2ny.raw",
            "room2nz.raw",
            "room2px.raw",
            "room2py.raw",
            "room2pz.raw",
        };
        const int cubemap_targets[] = { /* テクスチャのターゲット名 */
            GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        };
        for (int i = 0; i < 6; ++i) 
        {
            /* テクスチャの読み込みに使う配列 */
            GLubyte texture[TEXHEIGHT][TEXWIDTH][4];
            FILE *fp;
  
            /* テクスチャ画像の読み込み */
            std::string image_path = resource_dir + cubemap_textures[i];
            if ((fp = fopen(image_path.c_str(), "rb")) != NULL) {
                fread(texture, sizeof texture, 1, fp);
                fclose(fp);
            } else {
                perror(image_path.c_str());
            }
  
            /* テクスチャの割り当て */
            glTexImage2D(cubemap_targets[i], 0, GL_RGBA, TEXWIDTH, TEXHEIGHT, 0,
                            GL_RGBA, GL_UNSIGNED_BYTE, texture);
        }
  
        /* テクスチャを拡大・縮小する方法の指定 */
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  
        /* テクスチャの繰り返し方法の指定 */
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
  
        /* テクスチャ環境 */
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  
        /* キューブマッピング用のテクスチャ座標を生成する */
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
        glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

        /* ========================================= 
                        sphere map 
           ========================================= */
        ///* テクスチャの読み込みに使う配列 */
        //GLubyte texture[TEXHEIGHT][TEXWIDTH][4];

        ///* load textures */
        //{
        //    const char spheremap_texture_path[] = "room.raw";

        //    FILE *fp;
  
        //    std::string image_path = resource_dir + spheremap_texture_path;
        //    /* テクスチャ画像の読み込み */
        //    if ((fp = fopen(image_path.c_str(), "rb")) != NULL) {
        //        fread(texture, sizeof texture, 1, fp);
        //        fclose(fp);
        //    } else {
        //        perror(image_path.c_str());
        //    }
        //}
  
        ///* テクスチャ画像はバイト単位に詰め込まれている */
        //glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  
        ///* テクスチャの割り当て */
        //glTexImage2D(GL_TEXTURE_2D , 0, GL_RGBA, TEXWIDTH, TEXHEIGHT, 0,
        //            GL_RGBA, GL_UNSIGNED_BYTE, texture);
  
        ///* テクスチャを拡大・縮小する方法の指定 */
        //glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  
        ///* テクスチャの繰り返し方法の指定 */
        //glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S, GL_CLAMP);
        //glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T, GL_CLAMP);
  
        ///* テクスチャ環境 */
        //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  
        //#if 0
        ///* 混合する色の設定 */
        //static const GLfloat blend[] = { 0.0, 1.0, 0.0, 1.0 };
        //glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, blend);
        //#endif
  
        //#if 0
        ///* 頂点のオブジェクト空間における座標値を使ってマッピングする */
        //glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        //glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        //glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        //glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  
        ///* テクスチャ座標生成関数の設定 */
        //glTexGendv(GL_S, GL_OBJECT_PLANE, genfunc[0]);
        //glTexGendv(GL_T, GL_OBJECT_PLANE, genfunc[1]);
        //glTexGendv(GL_R, GL_OBJECT_PLANE, genfunc[2]);
        //glTexGendv(GL_Q, GL_OBJECT_PLANE, genfunc[3]);
        //#else
        ///* スフィアマッピング用のテクスチャ座標を生成する */
        //glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        //glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        //#endif
    }


    //{
	   // GLfloat values[2];
	   // glGetFloatv (GL_LINE_WIDTH_GRANULARITY, values);
	   // printf ("GL_LINE_WIDTH_GRANULARITY value is %3.1f\n", values[0]);

	   // glGetFloatv (GL_LINE_WIDTH_RANGE, values);
	   // printf ("GL_LINE_WIDTH_RANGE values are %3.1f %3.1f\n",
		  //  values[0], values[1]);

	   // glEnable (GL_LINE_SMOOTH);
	   // glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
	   // glEnable (GL_BLEND);
	   // glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	   // glLineWidth (1.5);

	   // glClearColor(0.0, 0.0, 0.0, 0.0);
    //}
}

void wsp::QtGLWidget::resizeGL(int width, int height)
{
    //WSP_GUI_DEBUG_LOG("Resize GL\n");
    glViewport(0, 0, width, height);  /* viewport setting */

    glMatrixMode(GL_PROJECTION); // カメラ座標系→スクリーン座標系への変換行列を設定
    glPushMatrix();
    glLoadIdentity();

    // perspective settings
    // [focal length of view y], [aspect ratio], [near plane], [far plane]
    gluPerspective(35.0, (double)width/(double)height, 0.1, 1000.0); 

    glMatrixMode(GL_MODELVIEW);
}

void wsp::QtGLWidget::paintGL()
{
    //if (painter->paintEngine()->type() != QPaintEngine::OpenGL
    //    && painter->paintEngine()->type() != QPaintEngine::OpenGL2)
    //{
    //    qWarning("QtGLWidget: drawBackground needs a QtGLWidget to be set as viewport on the graphics view");
    //    return;
    //}
    //WSP_GUI_DEBUG_LOG("Draw GL\n");

    //painter->beginNativePainting();

    //glClearColor(0,0,0,1);
    glClearColor(bg_color_.redF(), bg_color_.greenF(), bg_color_.blueF(), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    {
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluPerspective(70, width() / height(), 0.01, 1000);

        gluLookAt(
            0.0f, 0.0f, camera_distance_, /* camera position */
            tgt_x_, tgt_y_, tgt_z_, /* target position */
            0.0f, 1.0f, 0.0f  /* upper vector */
        );

        glTranslatef( camera_hor_move_,  camera_ver_move_, 0.0f);
        glRotatef( camera_pitch_, 1.0f, 0.0f, 0.0f );
        glRotatef( camera_yaw_, 0.0f, 1.0f, 0.0f );

        const float light_pos[] = { 
            pImpl_->m_lightItem->x() - width() / 2, 
            height() / 2 - pImpl_->m_lightItem->y(), 
            20,
            0 
        };
    }

    /* update rotation */
    {
        const int delta = m_time.elapsed() - m_lastTime;
        m_rotation += m_angularMomentum * (delta / 1000.0);
        m_lastTime += delta;
    }

    glUseProgram( pImpl_->current_shader_id_ );

    /* draw models */
    int num_model = polymodel_list_->length();
    for(int i=0; i<num_model; ++i) 
    {
        glMatrixMode(GL_MODELVIEW);

        // send uniform texture to shader
        //glUniform1i(texture_, 0);
        glUniform1i(pImpl_->uni_tex_loc_[0], i);

        glEnableVertexAttribArray(pImpl_->in_pos_loc_);
        glVertexAttribPointer(pImpl_->in_pos_loc_, 3, GL_FLOAT, GL_FALSE, 0, (*polymodel_list_)[i].points()->data());

        // --------------------------------
        // draw model
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        {
            glLoadIdentity();

            //glEnable(GL_TEXTURE_2D);
            //glBindTexture(GL_TEXTURE_2D, texture_);

            //glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
            glColor4f(model_color_.redF(), model_color_.greenF(), model_color_.blueF(), 1.0f);

            glRotatef(m_rotation.x()+init_rx_, 1, 0, 0);
            glRotatef(m_rotation.y()+init_ry_, 0, 1, 0);
            glRotatef(m_rotation.z()+init_rz_, 0, 0, 1);

            glScalef(1.0f, 1.0f, 1.0f);

            glEnable(GL_MULTISAMPLE);
            {
                (*polymodel_list_)[i].Render(display_normal_);
            }
            glDisable(GL_MULTISAMPLE);
        }
        glPopMatrix();
        // --------------------------------
    }

    /* draw test object */
    if( pImpl_->test_obj_to_draw_ != Impl::TEST_OBJECT_NONE )
    {
        glUniform1i(pImpl_->uni_tex_loc_[0], 0);

        static const GLfloat color[] = { 1.0, 1.0, 1.0, 1.0 };  /* 材質 (色) */
  
        /* 材質の設定 */
        //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  
        /* テクスチャマッピング開始 */
        //glActiveTexture(GL_TEXTURE0);
  
        /* テクスチャ座標の自動生成を有効にする */
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glEnable(GL_TEXTURE_GEN_R);

        glPushMatrix();
        {
            glLoadIdentity();

            glRotatef(m_rotation.x()+init_rx_, 1, 0, 0);
            glRotatef(m_rotation.y()+init_ry_, 0, 1, 0);
            glRotatef(m_rotation.z()+init_rz_, 0, 0, 1);

            switch(pImpl_->test_obj_to_draw_ )
            {
            case Impl::TEST_OBJECT_SPHERE:
                {
                    /* 球を描く */
                    glutSolidSphere(1.0, 32, 16);
                }
                break;
            case Impl::TEST_OBJECT_TEAPOT:
                {
                    /* ティーポットを描く */
                    glutSolidTeapot(1.0);
                }
                break;
            default:
                break;
            }
        }
        glPopMatrix();
  
        /* テクスチャ座標の自動生成を無効にする */
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_GEN_R);
    }


    /* draw axis */
    glUseProgram(0);

    if( pImpl_->drawAxis_ == true )
    {
        // --------------------------------
        // draw axis
        const float axis_pos[] = {0.0f, 0.0f, 0.0f};
        glDisable(GL_TEXTURE_2D);

        glPushMatrix();
        {
            glLoadIdentity();
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
        // --------------------------------

        //glPushMatrix();
        //{
        //    glLoadIdentity();
        //    glTranslatef(light_pos[0], light_pos[1], light_pos[2]);
        //    glBegin(GL_LINE_STRIP);
        //    {
        //        //glVertex3f( 0.0f+light_pos[0], 1.0f+light_pos[1], 0.0f );
        //        //glVertex3f( 1.0f+light_pos[0], 0.0f+light_pos[1], 0.0f );
        //        //glVertex3f(-1.0f+light_pos[0], 0.0f+light_pos[1], 0.0f );
        //        glVertex3f( 0.0f, 1.0f, 0.0f );
        //        glVertex3f( 1.0f, 0.0f, 0.0f );
        //        glVertex3f(-1.0f, 0.0f, 0.0f );
        //    }
        //    glEnd();
        //}
        //glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
    }

    //painter->endNativePainting();

    //{
	   // GLint	iMultiSample	= 0;
	   // GLint	iNumSamples		= 4;
	   // float	fRadius			= 0;
	   // char	strMsg[256];

	   // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   // glColor3f(1.0, 1.0, 1.0);

	   // // Restore font location
	   // giTextPos = giWindowHeight - giTextPosDelta;

	   // // MultiSampling
	   // glGetIntegerv(GL_SAMPLE_BUFFERS, &iMultiSample);
	   // glGetIntegerv(GL_SAMPLES, &iNumSamples);
	   // memset(strMsg, 0, sizeof(strMsg));

	   // if (giMultiSample == 1) {
		  //  glEnable(GL_MULTISAMPLE);
		  //  glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
		  //  sprintf(strMsg, "Multisample Enabled  : Number_of_Samples(%d)", iNumSamples);
		  //  DrawText(strMsg);
	   // } else {
		  //  glDisable(GL_MULTISAMPLE);
		  //  DrawText("Multisample Disabled");
	   // }

	   // if (giAntiAlias == 1) {
		  //  glEnable(GL_LINE_SMOOTH);
		  //  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		  //  glEnable(GL_BLEND);
		  //  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		  //  DrawText("AntiAlias Enabled");
	   // } else {
		  //  glDisable(GL_LINE_SMOOTH);
		  //  glDisable(GL_BLEND);
		  //  DrawText("AntiAlias Disabled");
	   // }

	   // fRadius = giHalfWindowHeight * 0.50;


	   // glColor3f (0.0, 1.0, 0.0);

	   // glPushMatrix();
	   // glTranslatef((float)giHalfWindowWidth, (float)giHalfWindowHeight, 0.0);
	   // glRotatef(-rotAngle, 0.0, 0.0, 0.1);
	   // glBegin (GL_LINES);
	   // glVertex2f (-fRadius, fRadius);
	   // glVertex2f (fRadius, -fRadius);
	   // glEnd ();
	   // glPopMatrix();

	   // glColor3f (0.0, 0.0, 1.0);

	   // glPushMatrix();
	   // glTranslatef((float)giHalfWindowWidth, (float)giHalfWindowHeight, 0.0);
	   // glRotatef(rotAngle, 0.0, 0.0, 0.1);
	   // glBegin (GL_LINES);
	   // glVertex2f (fRadius, fRadius);
	   // glVertex2f (-fRadius, -fRadius);
	   // glEnd ();
	   // glPopMatrix();

	   // glFlush();
	   // //glutSwapBuffers();
    //}

    QTimer::singleShot(20, this, SLOT(update()));

    /* ダブルバッファリング */
    glutSwapBuffers();
}

void wsp::QtGLWidget::slot_testDrawOff()
{
    pImpl_->test_obj_to_draw_ = Impl::TEST_OBJECT_NONE;
    pImpl_->uncheckAllTestDrawObject();
    pImpl_->test_draw_off_->setChecked(true);
}
void wsp::QtGLWidget::slot_testDrawSphere()
{
    pImpl_->test_obj_to_draw_ = Impl::TEST_OBJECT_SPHERE;
    pImpl_->uncheckAllTestDrawObject();
    pImpl_->test_draw_sphere_->setChecked(true);
}
void wsp::QtGLWidget::slot_testDrawTeapot()
{
    pImpl_->test_obj_to_draw_ = Impl::TEST_OBJECT_TEAPOT;
    pImpl_->uncheckAllTestDrawObject();
    pImpl_->test_draw_teapot_->setChecked(true);
}

void wsp::QtGLWidget::setDisplayType(){
    int num_model = polymodel_list_->length();
    for(int i=0; i<num_model; ++i) {
        (*polymodel_list_)[i].set_display_type(wsp::graphic::POLY_DISPLAY_TRIANGLES);
    }
    //switch(display_type_combobox_->currentIndex()){
    //case 0:
    //    polymodel_->set_display_type(wsp::graphic::POLY_DISPLAY_POINTS);
    //    break;
    //case 1:
    //    polymodel_->set_display_type(wsp::graphic::POLY_DISPLAY_WIREFRAME);
    //    break;
    //case 2:
    //    polymodel_->set_display_type(wsp::graphic::POLY_DISPLAY_TRIANGLES);
    //    break;
    //}
}

void wsp::QtGLWidget::slot_loadModel(){
    int num_model = polymodel_list_->length();
    for(int i=0; i<num_model; ++i) {
        (*polymodel_list_)[i].LoadObjFileDialog();
    }
}

void wsp::QtGLWidget::slot_changeDisplayType(int type){
    WSP_GUI_DEBUG_LOG("display type index: %d\n", type);
    setDisplayType();
    update();
}


void wsp::QtGLWidget::slot_loadDepthMap()
{
    wsp::graphic::DepthToMeshParam d2m_param;
    d2m_param = wsp::graphic::DEFAULT_DEPTH_TO_MESH_PARAM;

    d2m_param.invert_depth = true;
    d2m_param.face_depth_thre = 6.0f;

    int num_model = polymodel_list_->length();
    int num_points=0;
    int num_edges=0;
    int num_faces=0;
    for(int i=0; i<num_model; ++i) {
        (*polymodel_list_)[i].LoadDepthMapDialog(&d2m_param, "Open Depth Map");

        num_points += (*polymodel_list_)[i].GetNumPoints();
        num_edges += (*polymodel_list_)[i].GetNumEdges();
        num_faces += (*polymodel_list_)[i].GetNumFaces();
    }

    pImpl_->m_labels[1]->setText(tr("Points: %0").arg(num_points));
    pImpl_->m_labels[2]->setText(tr("Edges: %0").arg(num_edges));
    pImpl_->m_labels[3]->setText(tr("Faces: %0").arg(num_faces));


    setDisplayType();
    update();
}

void wsp::QtGLWidget::enableWireframe(bool enabled)
{
    enable_wireframe_ = enabled;
    update();
}

void wsp::QtGLWidget::enableNormals(bool enabled)
{
    display_normal_ = enabled;
    update();
}

void wsp::QtGLWidget::setModelColor()
{
    const QColor color = QColorDialog::getColor(model_color_);
    if (color.isValid()) {
        model_color_ = color;
        update();
    }
}

void wsp::QtGLWidget::setBackgroundColor()
{
    const QColor color = QColorDialog::getColor(bg_color_);
    if (color.isValid()) {
        bg_color_ = color;
        update();
    }
}



void wsp::QtGLWidget::keyPressEvent(QKeyEvent *event){
    //WSP_COMMON_DEBUG_LOG("Key %d pressed\n", event->key());
    switch(event->key()){
    case Qt::Key_Alt:
        camera_move_switch_ = true;
        //WSP_GUI_DEBUG_LOG("ALT key pressed\n");
        break;
    default:
        break;
    }
}

void wsp::QtGLWidget::keyReleaseEvent(QKeyEvent *event){
    //WSP_COMMON_DEBUG_LOG("Key %d released\n", event->key());
    switch(event->key())
    {
    case Qt::Key_Alt:
        camera_move_switch_ = false;
        //WSP_GUI_DEBUG_LOG("ALT key released\n");
        break;
    default:
        break;
    }
}



void wsp::QtGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    //WSP_COMMON_DEBUG_LOG("mouseMoveEvent\n");
    const QPointF delta = event->posF() - last_pos;
    last_pos = event->posF();
    if(camera_move_switch_==false)
    {
        if (event->buttons() & Qt::LeftButton) 
        {
            const wsp::Point<float> angularImpulse = wsp::Point<float>(-delta.y(), delta.x(), 0) * 0.1;

            m_rotation += angularImpulse;
            m_accumulatedMomentum += angularImpulse;
        }
    }
    else
    {
        if (event->buttons() & Qt::LeftButton) 
        {
            // aiming rotation animation
            camera_yaw_ += delta.x();
            camera_pitch_ -= delta.y();
            //WSP_GUI_DEBUG_LOG("delta: %.2f %.2f\n", delta.x(), delta.y());
        }
        else if(event->buttons() & Qt::MiddleButton)
        {
            // dolly animation
            camera_hor_move_ -= delta.x() * 0.01f;
            camera_ver_move_ -= delta.y() * 0.01f;
            //WSP_GUI_DEBUG_LOG("cam move: %.2f %.2f\n", camera_hor_move_, camera_ver_move_);
        }
        else if(event->buttons() & Qt::RightButton)
        {
            // dolly in, out animation
            camera_distance_ *= pow(1.2f, -(float)(delta.x()+delta.y())*0.02f);
        }
    }


    update();
    event->accept();
}

namespace{
    static bool dragging = false;
}
void wsp::QtGLWidget::mousePressEvent(QMouseEvent *event)
{
    m_mouseEventTime = m_time.elapsed();
    m_angularMomentum = m_accumulatedMomentum = wsp::Point<float>();

    last_pos = event->pos();
    dragging = true;

    if(camera_move_switch_==false && event->buttons()&Qt::RightButton)
    {
        pImpl_->pop_menu_->move(event->globalPos());
        pImpl_->pop_menu_->show();
    }

    event->accept();
}

void wsp::QtGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    const int delta = m_time.elapsed() - m_mouseEventTime;
    m_angularMomentum = m_accumulatedMomentum * (1000.0 / qMax(1, delta));

    dragging = false;

    event->accept();
    update();
}

void wsp::QtGLWidget::wheelEvent(QWheelEvent *event)
{
    //m_distance *= qPow(1.2, -event->delta() / 120);
    if(dragging==false){
        camera_distance_ *= pow(1.2f, -event->delta() / 120.0f);
    }

    event->accept();
    update();
}

/* ============================================== */
/* ============================================== */
/* ============================================== */


wsp::QtGLWidget::QtGLWidget( QWidget *parent, const char *name )
    : ::QGLWidget(parent)
    /* init vars */
    , enable_wireframe_  (false)
    , display_normal_    (false)
    , model_color_       (128, 128, 128)
    , bg_color_          (148, 148, 148)
    , m_lastTime         (0)
    , m_distance         (0.0f)
    , m_angularMomentum  (0, 0, 0)
    , camera_hor_move_   (0.0f)
    , camera_ver_move_   (0.0f)
    , camera_distance_   (3.0f)
    , camera_pitch_      (0.0f)
    , camera_yaw_        (0.0f)
    , camera_move_switch_(false)
    , tgt_x_             (0.0f)
    , tgt_y_             (0.0f)
    , tgt_z_             (0.0f)
    , init_rx_           (0.0f)
    , init_ry_           (0.0f)
    , init_rz_           (0.0f)

    /* init data */
    , polymodel_list_       (new wsp::Vector<wsp::PolyModel>())
    , camera_               (new wsp::Camera())

    , pImpl_ ( new Impl(this) )
{ 
    polymodel_list_->SetLength(1);
    this->setWindowTitle(QString(name));

    tex_obj_list_.SetLength(MAX_TEXTURE_NUM);

    m_time.start();
}

wsp::QtGLWidget::~QtGLWidget()
{
    /* free data */
    {
        /*int num_model = polymodel_list_->length();\
        for(int i=0; i<num_model; ++i){\
            delete (*polymodel_list_)[i];\
        }*/
        delete polymodel_list_;
        delete camera_;
    }

    delete pImpl_;
}