/**
 * @file cl-camera.h
 * 
 */

#ifndef WSP_GRAPHIC_CLASS_CAMERA_H_
#define WSP_GRAPHIC_CLASS_CAMERA_H_

#include "_define_graphic.h"
#include <wsp/common/tpl_cl-vector3.h>
#include <wsp/common/tpl_cl-vector4.h>
#include <wsp/graphics/tpl_cl-transform_matrix.h>
#include <wsp/graphics/fn-transform.h>

namespace wsp{
    namespace graphic{
        enum CameraType
        {
            CAMERA_TYPE_ORTHOGRAPHIC,
            CAMERA_TYPE_PERSPECTIVE,
            NUM_CAMERA_TYPE,
        };

        class WSP_DLL_EXPORT Camera
        {
        public:
            Camera(int viewport_width, int viewport_height);

            // Accessor
            int viewport_width() const
            {
                return viewport_width_;
            }
            int viewport_height() const
            {
                return viewport_height_;
            }
            float               field_of_view_vertical() const{ return field_of_view_vertical_; }
            float               aspect_ratio() const{ return viewport_width_ / (float)viewport_height_; }
            float               near_clip() const{ return near_clip_; }
            float               far_clip() const{ return far_clip_; }
            wsp::Point<float>   position() const{ return position_; }
            wsp::Point<float>   aim_position() const{ return wsp::Point<float>(wsp::Vector3<float>(aim_position_x_, aim_position_y_, aim_position_z_)); }
            wsp::Point<float>   up_vector() const{ return wsp::Point<float>(wsp::Vector3<float>(up_vector_x_, up_vector_y_, up_vector_z_)); }
            float   position_x() const{ return position_[0]; }
            float   position_y() const{ return position_[1]; }
            float   position_z() const{ return position_[2]; }
            float rotation_x() const{ return rotation_[0]; }
            float rotation_y() const{ return rotation_[1]; }
            float rotation_z() const{ return rotation_[2]; }
            wsp::Vector3<float> rotation() const { return rotation_; }
            wsp::Vector3<float> axis_x() const{ return axis_x_; }
            wsp::Vector3<float> axis_y() const{ return axis_y_; }
            wsp::Vector3<float> axis_z() const{ return axis_z_; }
            float aim_position_x() const {
                return aim_position_x_;
            }
            float aim_position_y() const {
                return aim_position_y_;
            }
            float aim_position_z() const {
                return aim_position_z_;
            }

            float up_vector_x() const {
                return up_vector_x_;
            }
            float up_vector_y() const {
                return up_vector_y_;
            }
            float up_vector_z() const {
                return up_vector_z_;
            }

            // mutator

            void set_field_of_view_vertical(float field_of_view_vertical_as_radian)
            {
                field_of_view_vertical_ = field_of_view_vertical_as_radian <= 0.0f ? 0.0f : field_of_view_vertical_as_radian;
            }
            void set_viewport_width(int width);
            void set_viewport_height(int height);
            void set_near_clip(float near_clip){ near_clip_ = near_clip; }
            void set_far_clip(float far_clip){ far_clip_ = far_clip; }
            void set_position(wsp::Point<float> &position){ position_ = position; }
            void set_position(float x, float y, float z)
            {
                position_[0] = (x);
                position_[1] = (y);
                position_[2] = (z);
            }
            void set_rotation(wsp::Vector3<float> &rotation){ rotation_ = rotation; }
            void set_rotation_x(float value) { rotation_.set_x(value); }
            void set_rotation_y(float value) { rotation_.set_y(value); }
            void set_rotation_z(float value) { rotation_.set_z(value); }

            void set_axis_x(wsp::Vector3<float> &axis_x){ axis_x_ = axis_x; }
            void set_axis_y(wsp::Vector3<float> &axis_y){ axis_y_ = axis_y; }
            void set_axis_z(wsp::Vector3<float> &axis_z){ axis_z_ = axis_z; }
            void set_position_x(float x) {
                position_[0] = (x);
            }
            void set_position_y(float y) {
                position_[1] = (y);
            }
            void set_position_z(float z) {
                position_[2] = (z);
            }

            void set_aim_position(float x, float y, float z)
            {
                aim_position_x_ = x;
                aim_position_y_ = y;
                aim_position_z_ = z;
            }
            void set_aim_position_x(float x){
                aim_position_x_ = x;
            }
            void set_aim_position_y(float y){
                aim_position_y_ = y;
            }
            void set_aim_position_z(float z){
                aim_position_z_ = z;
            }

            void set_up_vector_x(float x){
                up_vector_x_ = x;
            }
            void set_up_vector_y(float y){
                up_vector_y_ = y;
            }
            void set_up_vector_z(float z){
                up_vector_z_ = z;
            }

            CameraType camera_type() const
            {
                return camera_type_;
            }

            void set_camera_type(CameraType camera_type)
            {
                camera_type_ = camera_type;
            }

            wsp::Vector4<float> GetWorldCoordinate(
                const wsp::Vector4<float>& view_coordinate)
            {
                wsp::Vector4<float> world_coordinate;
                wsp::graphic::Camera& camera = *this;
                bool success;

                switch (camera.camera_type())
                {
                case wsp::graphic::CAMERA_TYPE_ORTHOGRAPHIC:
                    {
                        wsp::graphic::TransformMatrixF view_mat;
                        wsp::graphic::GetViewMatrix(view_mat, camera.position(), camera.aim_position(), camera.up_vector());
                        wsp::graphic::TransformMatrixF inverse_view_mat;
                        success = view_mat.GetInverseMatrix(&inverse_view_mat);
                        WSP_ASSERT(success, "failed to get inverse matrix");
                        world_coordinate = view_coordinate * inverse_view_mat;
                    }
                    break;
                case wsp::graphic::CAMERA_TYPE_PERSPECTIVE:
                    {
                        wsp::graphic::TransformMatrixF proj_mat;
                        wsp::graphic::TransformMatrixF inverse_proj_mat;

                        wsp::graphic::GetProjectionMatrix(
                            proj_mat, camera.near_clip(), camera.far_clip(), camera.field_of_view_vertical(), camera.aspect_ratio());
                        success = proj_mat.GetInverseMatrix(&inverse_proj_mat);
                        WSP_ASSERT(success, "failed to get inverse matrix");

                        wsp::graphic::TransformMatrixF view_mat;
                        wsp::graphic::GetViewMatrix(
                            view_mat,
                            camera.position(),
                            camera.aim_position(),
                            camera.up_vector());

                        wsp::graphic::TransformMatrixF transform_mat = view_mat * proj_mat;
                        wsp::graphic::TransformMatrixF inverse_mat;
                        bool success = transform_mat.GetInverseMatrix(&inverse_mat);
                        WSP_ASSERT(success, "failed to get inverse matrix");
                        world_coordinate = view_coordinate * inverse_mat;

                        //world_coordinate = view_coordinate * inverse_proj_mat;

                        //wsp::graphic::TransformMatrixF inverse_view_mat;
                        //success = view_mat.GetInverseMatrix(&inverse_view_mat);

                        //world_coordinate[3] = 1.0f; // TODO: Ç±ÇÍÇì¸ÇÍÇΩÇÁÇ‹ÇµÇ…Ç»Ç¡ÇΩÇ™ÅAç™ñ{âåàÇ≈ÇÕÇ»Ç¢ÇÃÇ≈âΩÇ∆Ç©ÇµÇΩÇ¢
                        //world_coordinate = world_coordinate * inverse_view_mat;
                    }
                    break;
                default:
                    break;
                }

                return world_coordinate;
            }

            wsp::Vector4<float> GetViewCoordinate(
                const wsp::Vector4<float>& world_coordinate)
            {
                wsp::Vector4<float> view_coordinate;
                wsp::graphic::Camera& camera = *this;
                wsp::graphic::TransformMatrixF view_mat;
                wsp::graphic::GetViewMatrix(view_mat, camera.position(), camera.aim_position(), camera.up_vector());

                switch (camera.camera_type())
                {
                case wsp::graphic::CAMERA_TYPE_ORTHOGRAPHIC:
                    {
                        view_coordinate = world_coordinate * view_mat;
                    }
                    break;
                case wsp::graphic::CAMERA_TYPE_PERSPECTIVE:
                    {
                        wsp::graphic::TransformMatrixF proj_mat;
                        wsp::graphic::GetProjectionMatrix(
                            proj_mat, camera.near_clip(), camera.far_clip(), camera.field_of_view_vertical(), camera.aspect_ratio());
                        view_coordinate = world_coordinate * view_mat * proj_mat;
                    }
                    break;
                default:
                    break;
                }

                return view_coordinate;
            }

        private:
            float field_of_view_vertical_;
            int viewport_width_;
            int viewport_height_;
            float near_clip_;
            float far_clip_;
            wsp::Point<float> position_;
            wsp::Vector3<float> rotation_;
            wsp::Vector3<float> axis_x_, axis_y_, axis_z_;
            float aim_position_x_;
            float aim_position_y_;
            float aim_position_z_;
            float up_vector_x_;
            float up_vector_y_;
            float up_vector_z_;
            CameraType camera_type_;
        };
    }
}

#endif


