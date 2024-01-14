/**
 * @file cl-qt_depth_to_mesh_view.hpp
 * @author Junichi Nishikata
 */

#ifndef __WSP_GUI_CLASS_QT_DEPTH_TO_MESH_VIEW_HPP_
#define __WSP_GUI_CLASS_QT_DEPTH_TO_MESH_VIEW_HPP_

#include "cl-qt_glview.hpp"

namespace wsp{
    class QtParamDialog;
}

namespace wsp{
    class WSP_DLL_EXPORT QtDepthToMeshView
        : public wsp::QtGLView
    {
        Q_OBJECT
    public:
        QtDepthToMeshView();

        /* ----------------------------------
               Getter
           ----------------------------------*/
        int bg_segm_id() const;


        /* ----------------------------------
               Setter
           ----------------------------------*/
        void set_depth_list(wsp::Vector<wsp::Image<SIGNED_IMG_DATA_TYPE>>* depth_list);
        void set_current_depth(wsp::Image<SIGNED_IMG_DATA_TYPE> *current_depth);
        void set_depth_min(double min);
        void set_depth_max(double max);
        void set_bg_segm_id(int segm_id);
        void set_scale(float scale);

        void CreateDepth();


    private slots:
        void slot_setFov(int value);
        void slot_setNearClip(int value);
        void slot_setScale(int value);
        void slot_setFaceDepthThre(int value);
        void slot_setAlphaThre(int value);
        void slot_setSamplingStep(int value);
        void slot_setInvert(int value);

    protected:
        //void keyPressEvent(QKeyEvent *event){
        //    WSP_COMMON_DEBUG_LOG("Key %d pressed\n", event->key());
        //    gl_widget_->keyPressEvent(event);
        //}
        //void keyReleaseEvent(QKeyEvent *event){
        //    WSP_COMMON_DEBUG_LOG("Key %d released\n", event->key());
        //    gl_widget_->keyReleaseEvent(event);
        //}

    private:
        // data
        wsp::Image<SIGNED_IMG_DATA_TYPE> *current_depth_;
        wsp::Vector<wsp::Image<SIGNED_IMG_DATA_TYPE>> *depth_list_;
        //wsp::Vector<wsp::Image<uchar>> *tex_list_;
        int bg_segm_id_;
        double depth_min_, depth_max_;

        // depth mesh param
        float field_of_view_;
        float near_clip_ratio_;
        float scale_;
        const float kScaleMin, kScaleMax;
        float face_depth_thre_;
        float alpha_thre_;
        float sampling_step_;
        bool invert_depth_;


        // gui data
        wsp::QtParamDialog *ctrl_dlg_;
        //wsp::QtSlider *param_slider_;
        //wsp::QtTextNumber *param_text_num_;


    };
}

#endif
