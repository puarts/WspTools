/**
 * @file cl-qt_depth_to_mesh_view.hpp
 * @author Junichi Nishikata
 */


#include "_define_gui.h"

#include "cl-qt_depth_to_mesh_view.hpp"

#include <wsp/common/cl-std_param_def_list.hpp>
#include "cl-qt_param_dialog.hpp"

wsp::QtDepthToMeshView::QtDepthToMeshView()
    : kScaleMin(0.01f)
    , kScaleMax(10.0f)
    , field_of_view_(50.0f)
    , near_clip_ratio_(0.6)
    , scale_(5.0f)
    , face_depth_thre_(20.0f)
    , alpha_thre_(0)
    , sampling_step_(1.0f)
    , invert_depth_(false)

{
    gl_widget_->set_init_rx(180);
    gl_widget_->set_init_ry(180);

    wsp::StdParamDefList param_list;
    param_list.Append(wsp::StdParamDefinition(0,"field_of_view", 1.0, 80.0, field_of_view_, "Field of view"));
    param_list.Append(wsp::StdParamDefinition(1,"near clip ratio", 0.01, 1.0, near_clip_ratio_, "Near clip ratio"));
    param_list.Append(wsp::StdParamDefinition(2,"scale", kScaleMin, kScaleMax, scale_, "Scale"));
    param_list.Append(wsp::StdParamDefinition(3,"alpha threshold", 0, 255, (int)alpha_thre_, "Alpha threshold"));
    param_list.Append(wsp::StdParamDefinition(4,"face depth threshold", 1.0, 255.0, face_depth_thre_, "Face depth threshold"));
    param_list.Append(wsp::StdParamDefinition(5,"sampling step", 1, 10, (int)sampling_step_, "Sampling step"));
    param_list.Append(wsp::StdParamDefinition(6,"invert", invert_depth_, "Invert depth."));
    //int num_of_params = param_list.length();

    field_of_view_ *= M_PI/180.0f;

    //param_slider_ = new wsp::QtSlider[num_of_params];
    //param_text_num_ = new wsp::QtTextNumber[num_of_params_];

    ctrl_dlg_ = new wsp::QtParamDialog(
                                NULL, 
                                &param_list, 
                                "Depth to Mesh Control"
                            );
    ctrl_dlg_->setFixedSize(400, 600);

    //for(int i=0; i<num_of_params; ++i){
    //    param_text_num_[i].setText(QString().setNum((*param_list)[i].default_double()));
    //    param_text_num_[i].is_double_value = true;
    //    param_slider_[i].setRange((*param_list)[i].min_double()*double_param_mult, (*param_list)[i].max_double()*double_param_mult);
    //    param_slider_[i].setValue((*param_list)[i].default_double()*double_param_mult);

    //    param_slider_[i].setOrientation(Qt::Horizontal);
    //    param_slider_[i].setSingleStep(1);

    //    connect(&param_slider_[i], SIGNAL(valueChanged(int)), &param_text_num_[i], SLOT(slot_setText(int)));
    //}

    connect(
        ctrl_dlg_->param_slider(0), SIGNAL( valueChanged(int) ), 
        this, SLOT( slot_setFov(int) )
    );
    connect(
        ctrl_dlg_->param_slider(1), SIGNAL( valueChanged(int) ), 
        this, SLOT( slot_setNearClip(int) )
    );
    connect(
        ctrl_dlg_->param_slider(2), SIGNAL( valueChanged(int) ), 
        this, SLOT( slot_setScale(int) )
    );
    connect(
        ctrl_dlg_->param_slider(3), SIGNAL( valueChanged(int) ), 
        this, SLOT( slot_setAlphaThre(int) )
    );
    connect(
        ctrl_dlg_->param_slider(4), SIGNAL( valueChanged(int) ), 
        this, SLOT( slot_setFaceDepthThre(int) )
    );
    connect(
        ctrl_dlg_->param_slider(5), SIGNAL( valueChanged(int) ), 
        this, SLOT( slot_setSamplingStep(int) )
    );
    connect(
        ctrl_dlg_->param_checkbox(6), SIGNAL( stateChanged(int) ), 
        this, SLOT( slot_setInvert(int) )
    );

    mdi_area_->addSubWindow(ctrl_dlg_);
    //ctrl_dlg_->show();
    //mdi_area_->resize(900, 600);
    //resize(900, 600);
    setGeometry(50, 50, 1280, 700);
}


void wsp::QtDepthToMeshView::CreateDepth()
{
    if(depth_list_==NULL){
        WSP_COMMON_ERROR_LOG("Current depth is NULL\n");
        return;
    }

    int num_model = gl_widget_->polymodel_list().length();
    int num_depth = depth_list_->length();
    if( num_model!=num_depth ){
        WSP_COMMON_ERROR_LOG("number of model and number of depth must be the same: %d==%d\n", num_model, num_depth);
        return;
    }

    printf("bg_segm_id_=%d\n", bg_segm_id_);
    double range = depth_max_ - depth_min_;
    //num_model = 1;

    WSP_COMMON_DEBUG_LOG(
        "field_of_view_ = %f\n"
        "near_clip_ratio_ = %f\n"
        "scale_ = %f\n"
        "face_depth_thre_ = %f\n"
        "alpha_thre_ = %f\n"
        "sampling_step_ = %f\n"
        "invert_depth_ = %s\n" 
        ,field_of_view_,
        near_clip_ratio_,
        scale_,
        face_depth_thre_,
        alpha_thre_,
        sampling_step_,
        invert_depth_==false? "false" : "true"
    );

    for(int i=0; i<num_model; ++i)
    {
        //if(i!=bg_segm_id_){
        //    continue;
        //}
        gl_widget_->polymodel_list()[i].CreateDepthMesh(
            (*depth_list_)[i],
            field_of_view_,
            near_clip_ratio_,
            scale_,
            face_depth_thre_,
            alpha_thre_,
            sampling_step_,
            invert_depth_, 
            0.0f, 0.0f, -scale_*0.8f,
            depth_min_, depth_max_
        );
    }
}

int wsp::QtDepthToMeshView::bg_segm_id() const{ return bg_segm_id_; }


void wsp::QtDepthToMeshView::set_current_depth(wsp::Image<SIGNED_IMG_DATA_TYPE>* current_depth)
{
    current_depth_ = current_depth;
}
void wsp::QtDepthToMeshView::set_depth_list(wsp::Vector<wsp::Image<SIGNED_IMG_DATA_TYPE>>* depth_list)
{
    depth_list_ = depth_list;
}
void wsp::QtDepthToMeshView::set_depth_min(double min){ depth_min_ = min; }
void wsp::QtDepthToMeshView::set_depth_max(double max){ depth_max_ = max; }
void wsp::QtDepthToMeshView::set_bg_segm_id(int segm_id){ bg_segm_id_ = segm_id; }
void wsp::QtDepthToMeshView::set_scale(float scale){ 
    slot_setScale((int)(scale * 10000.0f));
    int slider_min = ctrl_dlg_->param_slider(2)->minimum();
    int slider_max = ctrl_dlg_->param_slider(2)->maximum();
    float slider_step = (kScaleMax - kScaleMin) / ( slider_max - slider_min );
    int slider_value = (int)(scale / slider_step);

    WSP_COMMON_DEBUG_LOG(
        "slider_min = %d\n"
        "slider_max = %d\n"
        "slider_step = %f\n"
        "slider_value = %d\n"
        , slider_min
        , slider_max
        , slider_step
        , slider_value
    );

    ctrl_dlg_->SetParamSliderValue( 2, slider_value );
};



/* --------------------------------------
           Slots
   --------------------------------------*/
void wsp::QtDepthToMeshView::slot_setFov(int value){
    float fvalue = value * 0.0001f;
    fvalue = M_PI*fvalue/180.0f;
    WSP_COMMON_DEBUG_LOG("%f\n", fvalue);
    field_of_view_ = fvalue; 
    CreateDepth();
}
void wsp::QtDepthToMeshView::slot_setNearClip(int value){
    float fvalue = value * 0.0001f;
    WSP_COMMON_DEBUG_LOG("%f\n", fvalue);
    near_clip_ratio_ = fvalue; 
    CreateDepth();
}
void wsp::QtDepthToMeshView::slot_setScale(int value){
    float fvalue = value * 0.0001f;
    WSP_COMMON_DEBUG_LOG("%f\n", fvalue);
    scale_ = fvalue; 
    CreateDepth();
}
void wsp::QtDepthToMeshView::slot_setFaceDepthThre(int value){
    float fvalue = value * 0.0001f;
    WSP_COMMON_DEBUG_LOG("%f\n", fvalue);
    face_depth_thre_ = fvalue; 
    CreateDepth();
}
void wsp::QtDepthToMeshView::slot_setAlphaThre(int value){
    float fvalue = value;
    WSP_COMMON_DEBUG_LOG("%f\n", fvalue);
    if(fvalue<0 || fvalue>255){ return; }
    alpha_thre_ = fvalue; 
    CreateDepth();
}
void wsp::QtDepthToMeshView::slot_setSamplingStep(int value){
    float fvalue = value;
    WSP_COMMON_DEBUG_LOG("%f\n", fvalue);
    if(fvalue<1 || fvalue>10){ return; }
    sampling_step_ = fvalue; 
    CreateDepth();
}
void wsp::QtDepthToMeshView::slot_setInvert(int value){
    invert_depth_ = !invert_depth_; 
    CreateDepth();
}
