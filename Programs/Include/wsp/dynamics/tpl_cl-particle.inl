/**
 * @file tpl_cl-particle.inl
 * 
 * Template particle class
 */

#pragma once

#include <wsp/dynamics/tpl_cl-particle.hpp>
#include <wsp/common/fn-debug.h>
#include <wsp/common/tpl_cl-vector.h>
#include <wsp/graphics/fn-transform.h>

#include <list>

namespace{
    class ParticleIdCreator{
    public:
        ParticleIdCreator()
            : id_(-1)
        {}

        ~ParticleIdCreator()
        {}

        int GetNewId()
        {
            // 何か落ちるのでコメントアウト
            //if(reassignable_id_list.length() > 0)
            //{
            //    int id = reassignable_id_list.Pop();
            //    return id;
            //}

            ++id_;

            return id_;
        }

        void AddReassignableId( int id )
        {
            // 何か落ちるのでコメントアウト
            //reassignable_id_list.Append( id );
        }

    private:
        ParticleIdCreator( const ParticleIdCreator& );
        ParticleIdCreator& operator=(const ParticleIdCreator& rhs);

    private:
        int id_;
        wsp::Vector<int> reassignable_id_list;
    };

    ParticleIdCreator s_particle_id_creator;

    const int INVALID_ID = -1;

    template<typename _CalcType>
    inline void Rotate(_CalcType* o_x, _CalcType* o_y, _CalcType x, _CalcType y, _CalcType rotate_radian)
    {
        *o_x = cos(rotate_radian) * x - sin(rotate_radian) * y;
        *o_y = sin(rotate_radian) * x + cos(rotate_radian) * y;
    }

    template<typename _CalcType>
    inline void GetRotatedPosition3d(
        _CalcType *o_pos_x, _CalcType *o_pos_y, _CalcType *o_pos_z,
        _CalcType pivot_world_x, _CalcType pivot_world_y, _CalcType pivot_world_z,
        _CalcType model_pos_x, _CalcType model_pos_y, _CalcType model_pos_z,
        _CalcType rotate_x, _CalcType rotate_y, _CalcType rotate_z,
        const _CalcType *rotate_mat4x4)
    {
        Rotate(&model_pos_y, &model_pos_z, model_pos_y, model_pos_z, rotate_x);
        Rotate(&model_pos_z, &model_pos_x, model_pos_z, model_pos_x, rotate_y);
        Rotate(&model_pos_x, &model_pos_y, model_pos_x, model_pos_y, rotate_z);

        float rotated_pos_x = model_pos_x * rotate_mat4x4[0] + model_pos_y * rotate_mat4x4[1] + model_pos_z * rotate_mat4x4[2];
        float rotated_pos_y = model_pos_x * rotate_mat4x4[4] + model_pos_y * rotate_mat4x4[5] + model_pos_z * rotate_mat4x4[6];
        float rotated_pos_z = model_pos_x * rotate_mat4x4[8] + model_pos_y * rotate_mat4x4[9] + model_pos_z * rotate_mat4x4[10];

        *o_pos_x = pivot_world_x + rotated_pos_x;
        *o_pos_y = pivot_world_y + rotated_pos_y;
        *o_pos_z = pivot_world_z + rotated_pos_z;
    }

    template<typename _CalcType>
    inline void GetRotatedPosition2d(
        _CalcType *o_pos_x, _CalcType *o_pos_y,
        _CalcType pivot_world_x, _CalcType pivot_world_y,
        _CalcType model_pos_x, _CalcType model_pos_y,
        _CalcType rotate_z)
    {
        Rotate(&model_pos_x, &model_pos_y, model_pos_x, model_pos_y, rotate_z);
        *o_pos_x = pivot_world_x + model_pos_x;
        *o_pos_y = pivot_world_y + model_pos_y;
    }
}

template<typename _CalcType>
wsp::dyn::Particle<_CalcType>::Particle()
    : global_frame_(NULL)
    , scale_x_(1.0f), scale_y_(1.0f), scale_z_(1.0f)
    , position_x_(0.0), position_y_(0.0), position_z_(0.0)
    , rotate_x_(0.0), rotate_y_(0.0), rotate_z_(0.0)
    , velocity_x_(0.0), velocity_y_(0.0), velocity_z_(0.0)
    , angular_velocity_x_(0.0), angular_velocity_y_(0.0), angular_velocity_z_(0.0)
    , acceleration_x_(0.0), acceleration_y_(0.0), acceleration_z_(0.0)
    , color_r_(1.0f), color_g_(1.0f), color_b_(1.0f), color_a_(1.0f)
    , visibility_(true)
    , id_(INVALID_ID)
    , lifespan_(wsp::dyn::INFINATE_LIFE_SPAN)
    , birth_frame_(wsp::dyn::INFINATE_LIFE_SPAN)
    , radius_(0.0f)
    , current_radius_(0.0f)
{
    wsp::graphic::SetToIdentity(rotate_matrix4x4_);
}

template<typename _CalcType>
wsp::dyn::Particle<_CalcType>::Particle( const int *global_frame )
    : global_frame_( global_frame )
    , scale_x_(1.0f), scale_y_(1.0f), scale_z_(1.0f)
    , position_x_(0.0), position_y_(0.0), position_z_(0.0)
    , rotate_x_(0.0), rotate_y_(0.0), rotate_z_(0.0)
    , velocity_x_(0.0), velocity_y_(0.0), velocity_z_(0.0)
    , angular_velocity_x_(0.0), angular_velocity_y_(0.0), angular_velocity_z_(0.0)
    , acceleration_x_(0.0), acceleration_y_(0.0), acceleration_z_(0.0)
    , color_r_(1.0f), color_g_(1.0f), color_b_(1.0f), color_a_(1.0f)
    , visibility_(true)
    , id_(INVALID_ID)
    , lifespan_(wsp::dyn::INFINATE_LIFE_SPAN)
    , birth_frame_(wsp::dyn::INFINATE_LIFE_SPAN)
    , radius_(0.0f)
    , current_radius_(0.0f)
{
    wsp::graphic::SetToIdentity(rotate_matrix4x4_);
}

template<typename _CalcType>
wsp::dyn::Particle<_CalcType>::~Particle()
{
}

template<typename _CalcType>
bool wsp::dyn::Particle<_CalcType>::IsLiving() const 
{ 
    return id_!=INVALID_ID; 
}

template<typename _CalcType>
int wsp::dyn::Particle<_CalcType>::GetAge() const
{
    if (!IsLiving())
    {
        return 0;
    }

    int age = (*global_frame_) - birth_frame_;
    if (age < 0)
    {
        return 0;
    }
    else
    {
        return age;
    }
}

template<typename _CalcType>
void wsp::dyn::Particle<_CalcType>::SetAgePercentage(float percentage)
{
    birth_frame_ = (*global_frame_) - static_cast<int>(lifespan_ * percentage);
}

template<typename _CalcType>
float wsp::dyn::Particle<_CalcType>::GetAgePercentage() const
{
    if (!IsLiving())
    {
        return 1.0f;
    }
    if (lifespan_ <= 0)
    {
        return 0.0f;
    }

    return GetAge() / static_cast<float>(lifespan_);
}

template<typename _CalcType>
void wsp::dyn::Particle<_CalcType>::Birth()
{
    id_ = s_particle_id_creator.GetNewId();
    birth_frame_ = *global_frame_;
}

template<typename _CalcType>
void wsp::dyn::Particle<_CalcType>::Poll()
{
    if( IsLiving()==false )
    {
        return;
    }

    //printf("global_frame_ = %d, age %d > life %d\n", *global_frame_, GetAge(), lifespan_);
    if( GetAge() > lifespan_ )
    {
        Kill();
    }
}

template<typename _CalcType>
void wsp::dyn::Particle<_CalcType>::Kill()
{
    // 消えるIDを再アサイン可能なものとしてIdCreatorに登録
    s_particle_id_creator.AddReassignableId( id_ );
    id_ = INVALID_ID;
    radius_ = 0.0;
}

template<typename _CalcType>
void wsp::dyn::Particle<_CalcType>::GetTransformedModelVertexPositions(
    _CalcType* o_transformed_model_vertex_positions,
    const _CalcType* model_vertex_positions,
    int output_dimension,
    int input_dimension,
    int num_vertices) const
{
    WSP_REQUIRES(1 < output_dimension && output_dimension < 5, "invalid argument");
    WSP_REQUIRES(1 < input_dimension && input_dimension < 5, "invalid argument");

    {
        _CalcType* ptr = o_transformed_model_vertex_positions;
        _CalcType* end = ptr + num_vertices * output_dimension;
        const _CalcType* billboard_vtx_pos = model_vertex_positions;
        _CalcType x, y, z;
        switch (output_dimension)
        {
        case 4:
            {
                for (; ptr != end; ptr += output_dimension, billboard_vtx_pos += input_dimension)
                {
                    x = billboard_vtx_pos[0];
                    y = billboard_vtx_pos[1];
                    z = billboard_vtx_pos[2];
                    GetRotatedPosition3d(
                        &ptr[0], &ptr[1], &ptr[2],
                        position_x_, position_y_, position_z_,
                        x * current_scale_x_, y * current_scale_y_, z * current_scale_z_,
                        rotate_x_, rotate_y_, rotate_z_, rotate_matrix4x4_);
                    ptr[3] = 1.0;
                }
            }
            return;
        case 3:
            {
                for (; ptr != end; ptr += output_dimension, billboard_vtx_pos += input_dimension)
                {
                    x = billboard_vtx_pos[0];
                    y = billboard_vtx_pos[1];
                    z = billboard_vtx_pos[2];
                    GetRotatedPosition3d(
                        &ptr[0], &ptr[1], &ptr[2],
                        position_x_, position_y_, position_z_,
                        x * current_scale_x_, y * current_scale_y_, z * current_scale_z_,
                        rotate_x_, rotate_y_, rotate_z_, rotate_matrix4x4_);
                }
            }
            return;
        case 2:
            {
                for (; ptr != end; ptr += output_dimension, billboard_vtx_pos += input_dimension)
                {
                    x = billboard_vtx_pos[0];
                    y = billboard_vtx_pos[1];
                    GetRotatedPosition2d(
                        &ptr[0], &ptr[1],
                        position_x_, position_y_,
                        x * current_scale_x_, y * current_scale_y_,
                        rotate_z_);
                }
            }
            return;
        default:
            {
                WSP_FATAL("invalid dimension");
            }
            return;
        }
    }
}
