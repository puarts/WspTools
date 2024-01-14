/**
 * @file tpl_cl-particle.h
 * 
 * Template particle class
 */

#pragma once

#include <wsp/common/_ctypes.h>
#include <wsp/graphics/tpl_cl-transform_matrix.h>

#define _USE_MATH_DEFINES
#include <math.h>

namespace wsp{ namespace dyn{

    enum { INFINATE_LIFE_SPAN = 0x7fffffff };
    enum LifeType
    {
        LIFE_TYPE_INFINITY,
        LIFE_TYPE_AGE,
    };

    /**
     * @brief Particle class.
     * @tparam _CalcType Floating point type for simulation.
     */
    template<typename _CalcType>
    class Particle
    {
    public:
        Particle();
        explicit Particle( const int *global_frame );
        virtual ~Particle();

        int GetAge() const;
        float GetAgePercentage() const;
        bool IsLiving()   const;
        _CalcType scale_x()     const { return scale_x_; }
        _CalcType scale_y()     const { return scale_y_; }
        _CalcType scale_z()     const { return scale_z_; }
        _CalcType current_scale_x()     const { return current_scale_x_; }
        _CalcType current_scale_y()     const { return current_scale_y_; }
        _CalcType current_scale_z()     const { return current_scale_z_; }
        _CalcType rotate_x() const { return rotate_x_; }
        _CalcType rotate_y() const { return rotate_y_; }
        _CalcType rotate_z() const { return rotate_z_; }
        _CalcType position_x() const { return position_x_; }
        _CalcType position_y() const { return position_y_; }
        _CalcType position_z() const { return position_z_; }
        _CalcType angular_velocity_x() const { return angular_velocity_x_; }
        _CalcType angular_velocity_y() const { return angular_velocity_y_; }
        _CalcType angular_velocity_z() const { return angular_velocity_z_; }
        _CalcType velocity_x() const { return velocity_x_; }
        _CalcType velocity_y() const { return velocity_y_; }
        _CalcType velocity_z() const { return velocity_z_; }
        _CalcType acceleration_x() const { return acceleration_x_; }
        _CalcType acceleration_y() const { return acceleration_y_; }
        _CalcType acceleration_z() const { return acceleration_z_; }
        _CalcType color_r()     const { return color_r_; }
        _CalcType color_g()     const { return color_g_; }
        _CalcType color_b()     const { return color_b_; }
        _CalcType color_a()     const { return color_a_; }
        void GetTransformedModelVertexPositions(
            _CalcType* o_transformed_model_vertex_positions,
            const _CalcType* model_vertex_positions,
            int output_dimension,
            int input_dimension,
            int num_vertices) const;

        bool visibility() const { return visibility_; }
        int lifespan()    const { return lifespan_; }
        _CalcType radius()      const { return radius_; }
        _CalcType current_radius() const { return current_radius_; }
        void* user_ptr() { return user_ptr_; }
        template<typename TPtr>
        TPtr* user_ptr() { return static_cast<TPtr*>(user_ptr_); }

        void set_global_frame(int* global_frame)
        {
            global_frame_ = global_frame;
        }

        void set_scale(_CalcType sx, _CalcType sy, _CalcType sz)
        {
            scale_x_ = sx;
            scale_y_ = sy;
            scale_z_ = sz;
        }

        void set_scale(_CalcType scale)
        {
            scale_x_ = scale;
            scale_y_ = scale;
            scale_z_ = scale;
        }

        void set_current_scale(_CalcType scale)
        {
            current_scale_x_ = scale;
            current_scale_y_ = scale;
            current_scale_z_ = scale;
        }

        void set_current_scale(_CalcType sx, _CalcType sy, _CalcType sz)
        {
            current_scale_x_ = sx;
            current_scale_y_ = sy;
            current_scale_z_ = sz;
        }

        void set_current_scale_x(_CalcType sx)
        {
            current_scale_x_ = sx;
        }

        void set_current_scale_y(_CalcType sy)
        {
            current_scale_y_ = sy;
        }

        void set_current_scale_z(_CalcType sz)
        {
            current_scale_z_ = sz;
        }

        void set_scale_x(_CalcType sx)
        {
            scale_x_ = sx;
        }

        void set_scale_y(_CalcType sy)
        {
            scale_y_ = sy;
        }

        void set_scale_z(_CalcType sz)
        {
            scale_z_ = sz;
        }

        void set_rotate_x(_CalcType rx)
        {
            rotate_x_ = rx;
        }

        void set_rotate_y(_CalcType ry)
        {
            rotate_y_ = ry;
        }

        void set_rotate_z(_CalcType rz)
        {
            rotate_z_ = rz;
        }

        void set_position_x(_CalcType position_x)
        {
            position_x_ = position_x;
        }

        void set_position_y(_CalcType position_y)
        {
            position_y_ = position_y;
        }

        void set_position_z(_CalcType position_z)
        {
            position_z_ = position_z;
        }

        void set_position(_CalcType x, _CalcType y, _CalcType z)
        {
            set_position_x(x);
            set_position_y(y);
            set_position_z(z);
        }

        void set_angular_velocity_x(_CalcType angular_velocity_x){ angular_velocity_x_ = angular_velocity_x; }
        void set_angular_velocity_y(_CalcType angular_velocity_y){ angular_velocity_y_ = angular_velocity_y; }
        void set_angular_velocity_z(_CalcType angular_velocity_z){ angular_velocity_z_ = angular_velocity_z; }
        void set_velocity_x(_CalcType velocity_x){ velocity_x_ = velocity_x; }
        void set_velocity_y(_CalcType velocity_y){ velocity_y_ = velocity_y; }
        void set_velocity_z(_CalcType velocity_z){ velocity_z_ = velocity_z; }
        void set_acceleration_x(_CalcType acceleration_x){ acceleration_x_ = acceleration_x; }
        void set_acceleration_y(_CalcType acceleration_y){ acceleration_y_ = acceleration_y; }
        void set_acceleration_z(_CalcType acceleration_z){ acceleration_z_ = acceleration_z; }
        void set_color_r( _CalcType r ){ color_r_ = r; }
        void set_color_g( _CalcType g ){ color_g_ = g; }
        void set_color_b( _CalcType b ){ color_b_ = b; }
        void set_color_a( _CalcType a ){ color_a_ = a; }
        void set_color(_CalcType r, _CalcType g, _CalcType b, _CalcType a)
        {
            color_r_ = r;
            color_g_ = g;
            color_b_ = b;
            color_a_ = a;
        }
        void set_visibility( bool visibility ){ visibility_ = visibility; }
        void set_radius( _CalcType radius ){ radius_ = radius; }
        void set_current_radius(_CalcType radius) { current_radius_ = radius; }
        void set_lifespan( int lifespan ){ lifespan_ = lifespan; }
        void set_user_ptr(void* user_ptr) { user_ptr_ = user_ptr; }

        void Birth();
        void Poll();
        void Kill();

        void SetAgePercentage(float percentage);

        void SetRotateMatrix4x4(const _CalcType* in_mat)
        {
            memcpy(rotate_matrix4x4_, in_mat, 16 * sizeof(_CalcType));
        }

    private:
        int id_;
        int birth_frame_;
        int lifespan_;
        _CalcType position_x_, position_y_, position_z_;
        _CalcType angular_velocity_x_, angular_velocity_y_, angular_velocity_z_;
        _CalcType velocity_x_, velocity_y_, velocity_z_;
        _CalcType acceleration_x_, acceleration_y_, acceleration_z_;
        _CalcType color_r_, color_g_, color_b_, color_a_;
        _CalcType radius_, current_radius_;
        const int *global_frame_;
        bool visibility_;

        // ビルボード、モデルインスタンス用
        _CalcType rotate_x_, rotate_y_, rotate_z_;
        _CalcType scale_x_, scale_y_, scale_z_;
        _CalcType current_scale_x_, current_scale_y_, current_scale_z_;
        _CalcType rotate_matrix4x4_[16];
        void* user_ptr_;
    };
}}

#include "tpl_cl-particle.inl"
