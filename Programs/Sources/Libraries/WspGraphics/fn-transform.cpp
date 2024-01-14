// @file fn-transform.cpp
// 

#include "fn-transform.h"

namespace
{
    double inversesqrt(double const & x)
    {
        return 1.0 / sqrt(x);
    }

    wsp::Vector3<float> normalize(wsp::Vector3<float> const & x)
    {
        float sqr = x[0] * x[0] + x[1] * x[1] + x[2] * x[2];
        return wsp::Vector3<float>(x * inversesqrt(sqr));
    }

    wsp::Vector3<float> cross
        (
        wsp::Vector3<float> const & x,
        wsp::Vector3<float> const & y
        )
    {
        return wsp::Vector3<float>(
            x[1] * y[2] - y[1] * x[2],
            x[2] * y[0] - y[2] * x[0],
            x[0] * y[1] - y[0] * x[1]);
    }
}

inline void NormalizeVectorFloat3(float *pvector)
{
    float normalizingConstant = 1.0 / sqrtf(pvector[0] * pvector[0] + pvector[1] * pvector[1] + pvector[2] * pvector[2]);
    pvector[0] *= normalizingConstant;
    pvector[1] *= normalizingConstant;
    pvector[2] *= normalizingConstant;
}

inline void ComputeNormalOfPlane(float *normal, const float *pvector1, const float *pvector2)
{
    normal[0] = (pvector1[1] * pvector2[2]) - (pvector1[2] * pvector2[1]);
    normal[1] = (pvector1[2] * pvector2[0]) - (pvector1[0] * pvector2[2]);
    normal[2] = (pvector1[0] * pvector2[1]) - (pvector1[1] * pvector2[0]);
}

void glhTranslatef2(float *matrix, float x, float y, float z)
{
    matrix[12] = matrix[0] * x + matrix[4] * y + matrix[8] * z + matrix[12];
    matrix[13] = matrix[1] * x + matrix[5] * y + matrix[9] * z + matrix[13];
    matrix[14] = matrix[2] * x + matrix[6] * y + matrix[10] * z + matrix[14];
    matrix[15] = matrix[3] * x + matrix[7] * y + matrix[11] * z + matrix[15];
}


//PURPOSE:	For square matrices. This is column major for OpenGL
inline void MultiplyMatrices4by4OpenGL_FLOAT(float *result, float *matrix1, float *matrix2)
{
    result[0] = matrix1[0] * matrix2[0] +
        matrix1[4] * matrix2[1] +
        matrix1[8] * matrix2[2] +
        matrix1[12] * matrix2[3];
    result[4] = matrix1[0] * matrix2[4] +
        matrix1[4] * matrix2[5] +
        matrix1[8] * matrix2[6] +
        matrix1[12] * matrix2[7];
    result[8] = matrix1[0] * matrix2[8] +
        matrix1[4] * matrix2[9] +
        matrix1[8] * matrix2[10] +
        matrix1[12] * matrix2[11];
    result[12] = matrix1[0] * matrix2[12] +
        matrix1[4] * matrix2[13] +
        matrix1[8] * matrix2[14] +
        matrix1[12] * matrix2[15];

    result[1] = matrix1[1] * matrix2[0] +
        matrix1[5] * matrix2[1] +
        matrix1[9] * matrix2[2] +
        matrix1[13] * matrix2[3];
    result[5] = matrix1[1] * matrix2[4] +
        matrix1[5] * matrix2[5] +
        matrix1[9] * matrix2[6] +
        matrix1[13] * matrix2[7];
    result[9] = matrix1[1] * matrix2[8] +
        matrix1[5] * matrix2[9] +
        matrix1[9] * matrix2[10] +
        matrix1[13] * matrix2[11];
    result[13] = matrix1[1] * matrix2[12] +
        matrix1[5] * matrix2[13] +
        matrix1[9] * matrix2[14] +
        matrix1[13] * matrix2[15];

    result[2] = matrix1[2] * matrix2[0] +
        matrix1[6] * matrix2[1] +
        matrix1[10] * matrix2[2] +
        matrix1[14] * matrix2[3];
    result[6] = matrix1[2] * matrix2[4] +
        matrix1[6] * matrix2[5] +
        matrix1[10] * matrix2[6] +
        matrix1[14] * matrix2[7];
    result[10] = matrix1[2] * matrix2[8] +
        matrix1[6] * matrix2[9] +
        matrix1[10] * matrix2[10] +
        matrix1[14] * matrix2[11];
    result[14] = matrix1[2] * matrix2[12] +
        matrix1[6] * matrix2[13] +
        matrix1[10] * matrix2[14] +
        matrix1[14] * matrix2[15];

    result[3] = matrix1[3] * matrix2[0] +
        matrix1[7] * matrix2[1] +
        matrix1[11] * matrix2[2] +
        matrix1[15] * matrix2[3];
    result[7] = matrix1[3] * matrix2[4] +
        matrix1[7] * matrix2[5] +
        matrix1[11] * matrix2[6] +
        matrix1[15] * matrix2[7];
    result[11] = matrix1[3] * matrix2[8] +
        matrix1[7] * matrix2[9] +
        matrix1[11] * matrix2[10] +
        matrix1[15] * matrix2[11];
    result[15] = matrix1[3] * matrix2[12] +
        matrix1[7] * matrix2[13] +
        matrix1[11] * matrix2[14] +
        matrix1[15] * matrix2[15];
}

void glhLookAtf2(float *matrix,
    const float *eyePosition3D,
    const float *center3D, const float *upVector3D)
{
    float forward[3], side[3], up[3];
    float matrix2[16], resultMatrix[16];
    //------------------
    forward[0] = center3D[0] - eyePosition3D[0];
    forward[1] = center3D[1] - eyePosition3D[1];
    forward[2] = center3D[2] - eyePosition3D[2];
    NormalizeVectorFloat3(forward);
    //------------------
    //Side = forward x up
    ComputeNormalOfPlane(side, forward, upVector3D);
    NormalizeVectorFloat3(side);
    //------------------
    //Recompute up as: up = side x forward
    ComputeNormalOfPlane(up, side, forward);
    //------------------
    matrix2[0] = side[0];
    matrix2[4] = side[1];
    matrix2[8] = side[2];
    matrix2[12] = 0.0;
    //------------------
    matrix2[1] = up[0];
    matrix2[5] = up[1];
    matrix2[9] = up[2];
    matrix2[13] = 0.0;
    //------------------
    matrix2[2] = -forward[0];
    matrix2[6] = -forward[1];
    matrix2[10] = -forward[2];
    matrix2[14] = 0.0;
    //------------------
    matrix2[3] = matrix2[7] = matrix2[11] = 0.0;
    matrix2[15] = 1.0;
    //------------------
    MultiplyMatrices4by4OpenGL_FLOAT(resultMatrix, matrix, matrix2);
    glhTranslatef2(resultMatrix,
        -eyePosition3D[0], -eyePosition3D[1], -eyePosition3D[2]);
    //------------------
    memcpy(matrix, resultMatrix, 16 * sizeof(float));
}

void wsp::graphic::LookAt(
    float *o_matrix4x4,
    float eye_x, float eye_y, float eye_z,
    float target_x, float target_y, float target_z,
    float up_x, float up_y, float up_z)
{
    //float eye_vec[] = {
    //    eye_x, eye_y, eye_z
    //};
    //float target_vec[] = {
    //    target_x, target_y, target_z
    //};
    //float up_vec[] = {
    //    up_x, up_y, up_z
    //};
    //glhLookAtf2(
    //    o_matrix4x4,
    //    eye_vec,
    //    target_vec,
    //    up_vec);



    /* z é≤ = e - t */
    target_x = eye_x - target_x;
    target_y = eye_y - target_y;
    target_z = eye_z - target_z;
    float length = sqrtf(target_x * target_x + target_y * target_y + target_z * target_z); /* Ç±ÇÃ length Ç∆, */
    o_matrix4x4[2] = target_x / length;
    o_matrix4x4[6] = target_y / length;
    o_matrix4x4[10] = target_z / length;

    /* x é≤ = up_p x z é≤ */
    target_x = up_y * o_matrix4x4[10] - up_z * o_matrix4x4[6];
    target_y = up_z * o_matrix4x4[2] - up_x * o_matrix4x4[10];
    target_z = up_x * o_matrix4x4[6] - up_y * o_matrix4x4[2];
    length = sqrtf(target_x * target_x + target_y * target_y + target_z * target_z); /* Ç±ÇÃ length. */
    o_matrix4x4[0] = target_x / length;
    o_matrix4x4[4] = target_y / length;
    o_matrix4x4[8] = target_z / length;

    /* y é≤ = z é≤ x x é≤ */
    o_matrix4x4[1] = o_matrix4x4[6] * o_matrix4x4[8] - o_matrix4x4[10] * o_matrix4x4[4];
    o_matrix4x4[5] = o_matrix4x4[10] * o_matrix4x4[0] - o_matrix4x4[2] * o_matrix4x4[8];
    o_matrix4x4[9] = o_matrix4x4[2] * o_matrix4x4[4] - o_matrix4x4[6] * o_matrix4x4[0];

    /* ïΩçsà⁄ìÆ */
    o_matrix4x4[12] = -(eye_x * o_matrix4x4[0] + eye_y * o_matrix4x4[4] + eye_z * o_matrix4x4[8]);
    o_matrix4x4[13] = -(eye_x * o_matrix4x4[1] + eye_y * o_matrix4x4[5] + eye_z * o_matrix4x4[9]);
    o_matrix4x4[14] = -(eye_x * o_matrix4x4[2] + eye_y * o_matrix4x4[6] + eye_z * o_matrix4x4[10]);

    /* écÇË */
    o_matrix4x4[3] = o_matrix4x4[7] = o_matrix4x4[11] = 0.0f;
    o_matrix4x4[15] = 1.0f;
}

void wsp::graphic::GetViewMatrix(
    wsp::graphic::TransformMatrix<float> &o_mat,
    const wsp::Point<float> &in_eye_pos, 
    const wsp::Point<float> &in_target_pos,
    const wsp::Vector3<float> &in_cam_up_vec)
{
    LookAt(
        o_mat.data(),
        in_eye_pos.x(), in_eye_pos.y(), in_eye_pos.z(),
        in_target_pos.x(), in_target_pos.y(), in_target_pos.z(),
        in_cam_up_vec[0], in_cam_up_vec[1], in_cam_up_vec[2]);
}


void wsp::graphic::GetProjectionMatrix(
    wsp::graphic::TransformMatrix<float> &o_mat,
    float near_plane,
    float far_plane,
    float vertical_field_of_view_as_radian,
    float aspect_ratio)
{
    //float h_scale = 1.0f / tan(vertical_field_of_view_as_radian * 0.5f);
    float horizontal_field_of_view = vertical_field_of_view_as_radian * aspect_ratio;
    //float w_scale = 1.0f / tan(horizontal_field_of_view * 0.5f);
    //float d = far_plane / (far_plane - near_plane);

    //o_mat[0] = 1.0f / tan(horizontal_field_of_view * 0.5f);
    //o_mat[4] = 0.0f;   
    //o_mat[8] = 0.0f;   
    //o_mat[12]=0.0f;    

    //o_mat[1] = 0.0f;
    //o_mat[5] = 1.0f / tan(vertical_field_of_view_as_radian * 0.5f);
    //o_mat[9] = 0.0f;
    //o_mat[13]= 0.0f;

    //o_mat[2] = 0.0f;
    //o_mat[6] = 0.0f;
    //o_mat[10] = far_plane / (far_plane - near_plane);
    //o_mat[14] = -near_plane * far_plane / (far_plane - near_plane);

    //o_mat[3] = 0.0f;
    //o_mat[7] = 0.0f;
    //o_mat[11]= 1.0f;
    //o_mat[15]= 0.0f;

    float dx = 2.0f * near_plane * tan(horizontal_field_of_view * 0.5f);
    float dy = 2.0f * near_plane * tan(vertical_field_of_view_as_radian * 0.5f);
    float dz = far_plane - near_plane;

    o_mat[0] = 2.0f * near_plane / dx;
    o_mat[4] = 0.0f;
    o_mat[8] = 0.0f;
    o_mat[12] = 0.0f;

    o_mat[1] = 0.0f;
    o_mat[5] = 2.0f * near_plane / dy;
    o_mat[9] = 0.0f;
    o_mat[13] = 0.0f;

    o_mat[2] = 0.0f;
    o_mat[6] = 0.0f;
    o_mat[10] = -(far_plane + near_plane) / dz;
    o_mat[14] = -2.0f * far_plane * near_plane / dz;

    o_mat[3] = 0.0f;
    o_mat[7] = 0.0f;
    o_mat[11] = -1.0f;
    o_mat[15] = 0.0f;
}

void wsp::graphic::GetViewportMatrix(wsp::graphic::TransformMatrix<float> &o_mat, float width, float height, float depth)
{
    float sx = width * 0.5f;
    float sy = height* 0.5f;
    float sz = depth;

    o_mat[0] =sx;   o_mat[1] =0.0f; o_mat[2] =0.0f; o_mat[3] =0.0f;
    o_mat[4] =0.0f; o_mat[5] =-sy;  o_mat[6] =0.0f; o_mat[7] =0.0f;
    o_mat[8] =0.0f; o_mat[9] =0.0f; o_mat[10]=sz;   o_mat[11]=0.0f;
    o_mat[12]=sx;   o_mat[13]=sy;   o_mat[14]=0.0f; o_mat[15]=1.0f;
}

void wsp::graphic::GetOrthoMatrix(
    wsp::graphic::TransformMatrix<float> &o_mat,
    float left,
    float right,
    float bottom,
    float top,
    float zNear,
    float zFar)
{
    o_mat.SetToIdentity();
    o_mat[0 * 4 + 0] = static_cast<float>(2) / (right - left);
    o_mat[1 * 4 + 1] = static_cast<float>(2) / (top - bottom);
    o_mat[2 * 4 + 2] = -static_cast<float>(2) / (zFar - zNear);
    o_mat[3 * 4 + 0] = -(right + left) / (right - left);
    o_mat[3 * 4 + 1] = -(top + bottom) / (top - bottom);
    o_mat[3 * 4 + 2] = -(zFar + zNear) / (zFar - zNear);
}
