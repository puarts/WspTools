// @file fn-transform.h
// 

#ifndef _WSP_FN_TRANSFORM_H_
#define _WSP_FN_TRANSFORM_H_

#include <wsp/common/tpl_cl-vector3.h>
#include <wsp/math/_tpl_fn-math.h>
#include <wsp/math/_math_types.h>
#include "tpl_cl-transform_matrix.h"

namespace wsp{ namespace graphic{

    WSP_DLL_EXPORT void LookAt(
        float *o_matrix4x4,
        float eye_x, float eye_y, float eye_z,
        float aim_target_x, float aim_target_y, float aim_target_z,
        float up_x, float up_y, float up_z);

    inline void SetToIdentity(float *o_matrix4x4)
    {
        o_matrix4x4[0] = 1.0f;
        o_matrix4x4[1] = 0.0f;
        o_matrix4x4[2] = 0.0f;
        o_matrix4x4[3] = 0.0f;

        o_matrix4x4[4] = 0.0f;
        o_matrix4x4[5] = 1.0f;
        o_matrix4x4[6] = 0.0f;
        o_matrix4x4[7] = 0.0f;

        o_matrix4x4[8] = 0.0f;
        o_matrix4x4[9] = 0.0f;
        o_matrix4x4[10] = 1.0f;
        o_matrix4x4[11] = 0.0f;

        o_matrix4x4[12] = 0.0f;
        o_matrix4x4[13] = 0.0f;
        o_matrix4x4[14] = 0.0f;
        o_matrix4x4[15] = 1.0f;
    }

    /**
     * @param o_view_mat output
     * @param in_eye_pos camera position
     * @param in_target_pos target position
     * @param in_cam_up_vec camera up vector
     */
    WSP_DLL_EXPORT void GetViewMatrix(
        wsp::graphic::TransformMatrix<float> &o_view_mat,
        const wsp::Point<float> &in_eye_pos, 
        const wsp::Point<float> &in_target_pos,
        const wsp::Vector3<float> &in_cam_up_vec
    );

    /**
     * @param near_clip_plane near clip
     * @param far_clip_plane far clip
     * @param horizontal_field_of_view_as_radian vertical field of view as radian
     * @param aspect_ratio aspect ratio (width / height)
     */
    WSP_DLL_EXPORT void GetProjectionMatrix(
        wsp::graphic::TransformMatrix<float> &o_mat,
        float near_clip_plane,
        float far_clip_plane,
        float vertical_field_of_view_as_radian,
        float aspect_ratio);
    
    WSP_DLL_EXPORT void GetViewportMatrix(
        wsp::graphic::TransformMatrix<float> &o_mat,
        float width,
        float height,
        float depth);

    WSP_DLL_EXPORT void GetOrthoMatrix(
        wsp::graphic::TransformMatrix<float> &o_mat,
        float left,
        float right,
        float bottom,
        float top,
        float zNear,
        float zFar);

    inline void GetNormalMatrix(
        wsp::graphic::TransformMatrix<float>* o_normal_mat,
        const wsp::graphic::TransformMatrix<float> &in_model_mat,
        const wsp::graphic::TransformMatrix<float> &in_view_mat)
    {
        wsp::graphic::TransformMatrixF model_view_mat = in_model_mat * in_view_mat;
        wsp::graphic::TransformMatrixF model_view_inverse_mat;
        model_view_mat.GetInverseMatrix(&model_view_inverse_mat);
        *o_normal_mat = model_view_inverse_mat.Transpose();
    }

    inline void RotateAxes(
        float move_axis[3], 
        float move_dir_axis[3], 
        float rot_axis[3], 
        float move_rate
    )
    {
        // add X direction value to Z axis
        move_axis[0] += move_dir_axis[0] * move_rate;
        move_axis[1] += move_dir_axis[1] * move_rate;
        move_axis[2] += move_dir_axis[2] * move_rate;


        // normalize Y
        wsp::math::NormalizeLengthVec3(move_axis);
        // normalize Z
        wsp::math::NormalizeLengthVec3(rot_axis);

        // X = Y x Z, and normalize X
        wsp::math::GetCrossProduct(rot_axis, move_axis, move_dir_axis);
        wsp::math::NormalizeLengthVec3(move_dir_axis);

        // Y = Z x X, and normalize Y
        wsp::math::GetCrossProduct(move_axis, move_dir_axis, rot_axis);
        wsp::math::NormalizeLengthVec3(rot_axis);
    }

    //inline void AimConstraint_FrontMinusZ_UpY(
    //    wsp::Vec3F *io_ax, wsp::Vec3F *io_ay, wsp::Vec3F *io_az,
    //    wsp::Vec3F *self_pos, wsp::Vec3F *target_pos
    //){
    //    wsp::Vec3F aim_vec = *self_pos - *target_pos;
    //    *io_az = aim_vec;
    //    OrthoNormalizeZ(io_ax, io_ay, io_az);
    //}


    template<typename _CalcType>
    wsp::graphic::TransformMatrix<_CalcType> GetXAxisRotationMatrixRowMajor(float rotation_as_radian);

    template<typename _CalcType>
    wsp::graphic::TransformMatrix<_CalcType> GetYAxisRotationMatrixRowMajor(float rotation_as_radian);

    template<typename _CalcType>
    wsp::graphic::TransformMatrix<_CalcType> GetZAxisRotationMatrixRowMajor(float rotation_as_radian);

    template<typename _CalcType>
    wsp::graphic::TransformMatrix<_CalcType> GetRotationMatrixRowMajor(float rx, float ry, float rz, wsp::math::RotationOrder order);

    template<typename _CalcType>
    wsp::graphic::TransformMatrix<_CalcType> GetScaleMatrix(float sx, float sy, float sz)
    {
        wsp::graphic::TransformMatrix<_CalcType> matrix;
        matrix.SetToIdentity();
        matrix[0 * 4 + 0] = sx;
        matrix[1 * 4 + 1] = sy;
        matrix[2 * 4 + 2] = sz;
        return matrix;
    }

    template<typename _CalcType>
    wsp::graphic::TransformMatrix<_CalcType> GetTranslateMatrixColumnMajor(float tx, float ty, float tz)
    {
        wsp::graphic::TransformMatrix<_CalcType> matrix;
        matrix.SetToIdentity();
        matrix[0 * 4 + 3] = tx;
        matrix[1 * 4 + 3] = ty;
        matrix[2 * 4 + 3] = tz;
        return matrix;
    }

    template<typename _CalcType>
    wsp::graphic::TransformMatrix<_CalcType> GetTranslateMatrixRowMajor(float tx, float ty, float tz)
    {
        wsp::graphic::TransformMatrix<_CalcType> matrix;
        matrix.SetToIdentity();
        matrix[3 * 4 + 0] = tx;
        matrix[3 * 4 + 1] = ty;
        matrix[3 * 4 + 2] = tz;
        return matrix;
    }

    template<typename _CalcType>
    wsp::graphic::TransformMatrix<_CalcType> GetTransformMatrixRowMajor(
        float tx, float ty, float tz,
        float rx, float ry, float rz,
        float sx, float sy, float sz,
        wsp::math::RotationOrder rotation_order);
}}

template<typename _CalcType>
wsp::graphic::TransformMatrix<_CalcType> wsp::graphic::GetXAxisRotationMatrixRowMajor(float theta)
{
    wsp::graphic::TransformMatrix<_CalcType> matrix;
    matrix.SetToIdentity();
    int ColumnCount = 4;
    matrix[1 * ColumnCount + 1] = cos(theta);
    matrix[1 * ColumnCount + 2] = sin(theta);
    matrix[2 * ColumnCount + 1] = -sin(theta);
    matrix[2 * ColumnCount + 2] = cos(theta);
    return matrix;
}

template<typename _CalcType>
wsp::graphic::TransformMatrix<_CalcType> wsp::graphic::GetYAxisRotationMatrixRowMajor(float theta)
{
    wsp::graphic::TransformMatrix<_CalcType> matrix;
    matrix.SetToIdentity();
    int ColumnCount = 4;
    matrix[0 * ColumnCount + 0] = cos(theta);
    matrix[0 * ColumnCount + 2] = -sin(theta);
    matrix[2 * ColumnCount + 0] = sin(theta);
    matrix[2 * ColumnCount + 2] = cos(theta);
    return matrix;
}

template<typename _CalcType>
wsp::graphic::TransformMatrix<_CalcType> wsp::graphic::GetZAxisRotationMatrixRowMajor(float theta)
{
    wsp::graphic::TransformMatrix<_CalcType> matrix;
    matrix.SetToIdentity();
    int ColumnCount = 4;
    matrix[0 * ColumnCount + 0] = cos(theta);
    matrix[0 * ColumnCount + 1] = sin(theta);
    matrix[1 * ColumnCount + 0] = -sin(theta);
    matrix[1 * ColumnCount + 1] = cos(theta);
    return matrix;
}

template<typename _CalcType>
wsp::graphic::TransformMatrix<_CalcType> wsp::graphic::GetRotationMatrixRowMajor(float rx, float ry, float rz, wsp::math::RotationOrder rotation_order)
{
    switch (rotation_order)
    {
    case WSP_ROTATION_ORDER_XYZ:
        return GetXAxisRotationMatrixRowMajor<_CalcType>(rx) * GetYAxisRotationMatrixRowMajor<_CalcType>(ry) * GetZAxisRotationMatrixRowMajor<_CalcType>(rz);
    case WSP_ROTATION_ORDER_XZY:
        return GetXAxisRotationMatrixRowMajor<_CalcType>(rx) * GetZAxisRotationMatrixRowMajor<_CalcType>(rz) * GetYAxisRotationMatrixRowMajor<_CalcType>(ry);
    case WSP_ROTATION_ORDER_YXZ:
        return GetYAxisRotationMatrixRowMajor<_CalcType>(ry) * GetXAxisRotationMatrixRowMajor<_CalcType>(rx) * GetZAxisRotationMatrixRowMajor<_CalcType>(rz);
    case WSP_ROTATION_ORDER_YZX:
        return GetYAxisRotationMatrixRowMajor<_CalcType>(ry) * GetZAxisRotationMatrixRowMajor<_CalcType>(rz) * GetXAxisRotationMatrixRowMajor<_CalcType>(rx);
    case WSP_ROTATION_ORDER_ZXY:
        return GetZAxisRotationMatrixRowMajor<_CalcType>(rz) * GetXAxisRotationMatrixRowMajor<_CalcType>(rx) * GetYAxisRotationMatrixRowMajor<_CalcType>(ry);
    case WSP_ROTATION_ORDER_ZYX:
        return GetZAxisRotationMatrixRowMajor<_CalcType>(rz) * GetYAxisRotationMatrixRowMajor<_CalcType>(ry) * GetXAxisRotationMatrixRowMajor<_CalcType>(rx);
    default:
        WSP_FATAL_UNEXPECTED_DEFAULT;
    }

    return wsp::graphic::TransformMatrix<_CalcType>();
}

template<typename _CalcType>
wsp::graphic::TransformMatrix<_CalcType> wsp::graphic::GetTransformMatrixRowMajor(
    float tx, float ty, float tz,
    float rx, float ry, float rz,
    float sx, float sy, float sz,
    wsp::math::RotationOrder rotation_order)
{
    return GetScaleMatrix<_CalcType>(sx, sy, sz) * GetRotationMatrixRowMajor<_CalcType>(rx, ry, rz, rotation_order) * GetTranslateMatrixRowMajor<_CalcType>(tx, ty, tz);
}

#endif
