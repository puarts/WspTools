/**
 * @file cl-qt_glview.cpp
 * @author Junichi Nishikata
 */

#include "cl-qt_glview.hpp"


wsp::QtGLView::QtGLView()
    : gl_widget_( new wsp::QtGLWidget())
    , mdi_area_( new QMdiArea())
    //main_layout_(new QHBoxLayout())
{
    /* set up control dialog */
    {
        wsp::StdParamDefList param_list;
        param_list.Append( wsp::StdParamDefinition(0, "Env Map Ratio", 0.0, 1.0, 0.5, "Reflection ratio of cube map", 0));
        ctrl_dlg_ = new wsp::QtParamDialog(NULL, &param_list, "3D View Setting");
        ctrl_dlg_->HideApplyButton();
        ctrl_dlg_->HideCancelButton();

        this->connect(
            ctrl_dlg_->param_slider(0), SIGNAL( valueChanged(int) ), 
            gl_widget_, SLOT( slot_setEnvmapRatio(int) )
        );
    }

    this->setCentralWidget(mdi_area_);
    gl_widget_->setParent(mdi_area_);
    mdi_area_->addSubWindow(gl_widget_);
    mdi_area_->addSubWindow(ctrl_dlg_);
    mdi_area_->resize(800, 700);
    resize(800, 700);
}

wsp::QtGLView::~QtGLView(){
    delete gl_widget_;
    delete ctrl_dlg_;
    delete mdi_area_;
    //delete main_layout_;
}




//QSize wsp::QtGLView::minimumSizeHint() const{
//    return QSize(200, 100);
//}
QSize wsp::QtGLView::sizeHint() const{
    return QSize(800, 700);
}

void wsp::QtGLView::keyPressEvent(QKeyEvent *event){
    //WSP_COMMON_DEBUG_LOG("Key %d pressed\n", event->key());
    gl_widget_->keyPressEvent(event);
}
void wsp::QtGLView::keyReleaseEvent(QKeyEvent *event){
    //WSP_COMMON_DEBUG_LOG("Key %d released\n", event->key());
    gl_widget_->keyReleaseEvent(event);
}
