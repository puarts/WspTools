
#include "RenderSkeleton.h"

namespace {
    wsp::graphic::TransformMatrixF CalculateBoneMatrix(
        float tx, float ty, float tz,
        float rx, float ry, float rz,
        float sx, float sy, float sz,
        float pre_rx, float pre_ry, float pre_rz,
        float post_rx, float post_ry, float post_rz)
    {
        return
            //wsp::graphic::GetTransformMatrixRowMajor<float>(
            //    tx, ty, tz,
            //    pre_rx + rx, pre_ry + ry, pre_rz + rz,
            //    sx, sy, sz,
            //    wsp::math::RotationOrder::WSP_ROTATION_ORDER_XYZ);

            wsp::graphic::GetTransformMatrixRowMajor<float>(
                0.0f, 0.0f, 0.0f,
                pre_rx, pre_ry, pre_rz,
                1.0f, 1.0f, 1.0f,
                wsp::math::RotationOrder::WSP_ROTATION_ORDER_XYZ)
            *
            wsp::graphic::GetTransformMatrixRowMajor<float>(
                tx, ty, tz,
                rx, ry, rz,
                sx, sy, sz,
                wsp::math::RotationOrder::WSP_ROTATION_ORDER_XYZ);

            //wsp::graphic::GetTransformMatrixRowMajor<float>(
            //    tx, ty, tz,
            //    pre_rx, pre_ry, pre_rz,
            //    1.0f, 1.0f, 1.0f,
            //    wsp::math::RotationOrder::WSP_ROTATION_ORDER_XYZ)
            //*
            //wsp::graphic::GetTransformMatrixRowMajor<float>(
            //    0.0f, 0.0f, 0.0f,
            //    rx, ry, rz,
            //    1.0f, 1.0f, 1.0f,
            //    wsp::math::RotationOrder::WSP_ROTATION_ORDER_XYZ)
            //*
            //wsp::graphic::GetTransformMatrixRowMajor<float>(
            //    0.0f, 0.0f, 0.0f,
            //    post_rx, post_ry, post_rz,
            //    sx, sy, sz,
            //    wsp::math::RotationOrder::WSP_ROTATION_ORDER_XYZ);
    }

    wsp::graphic::TransformMatrixF CalculateInverseBoneMatrix(
        float tx, float ty, float tz,
        float rx, float ry, float rz,
        float sx, float sy, float sz,
        float pre_rx, float pre_ry, float pre_rz,
        float post_rx, float post_ry, float post_rz)
    {
        wsp::graphic::TransformMatrixF inverse_post;
        wsp::graphic::GetTransformMatrixRowMajor<float>(
            0.0f, 0.0f, 0.0f,
            post_rx, post_ry, post_rz,
            sx, sy, sz,
            wsp::math::RotationOrder::WSP_ROTATION_ORDER_XYZ).GetInverseMatrix(&inverse_post);

        wsp::graphic::TransformMatrixF inverse_pre;
        wsp::graphic::GetTransformMatrixRowMajor<float>(
            tx, ty, tz,
            pre_rx, pre_ry, pre_rz,
            1.0f, 1.0f, 1.0f,
            wsp::math::RotationOrder::WSP_ROTATION_ORDER_XYZ).GetInverseMatrix(&inverse_pre);

        wsp::graphic::TransformMatrixF inverse;
        wsp::graphic::GetTransformMatrixRowMajor<float>(
            0.0f, 0.0f, 0.0f,
            rx, ry, rz,
            1.0f, 1.0f, 1.0f,
            wsp::math::RotationOrder::WSP_ROTATION_ORDER_XYZ).GetInverseMatrix(&inverse);

        return inverse_post * inverse * inverse_pre;
    }
}

wsp::graphic::RenderBone::~RenderBone()
{
}

void wsp::graphic::RenderBone::AddChildBone(RenderBone* bone)
{
    child_bones_.Append(bone);
}

void wsp::graphic::RenderBone::CopyFrom(const RenderBone& rhs)
{
    parent_bone_ = rhs.parent_bone_;
    bind_pose_transform_matrix_ = rhs.bind_pose_transform_matrix_;
    bind_pose_normal_transform_matrix_ = rhs.bind_pose_normal_transform_matrix_;
    transform_matrix_ = rhs.transform_matrix_;
    normal_transform_matrix_ = rhs.normal_transform_matrix_;
    current_pose_ = rhs.current_pose_;
    bind_pose_ = rhs.bind_pose_;
    smooth_skinning_matrix_table_index_ = rhs.smooth_skinning_matrix_table_index_;

    set_name(rhs.name());
    set_parent_bone_name(rhs.parent_bone_name());
    for (int index = 0; index < rhs.GetChildBoneCount(); ++index)
    {
        child_bones_.Append(rhs.child_bones_[index]);
    }
}

wsp::graphic::RenderSkeleton::~RenderSkeleton()
{
}

void wsp::graphic::RenderSkeleton::StoreMaricesIntoMatrixPalette()
{
    for (int bone_index = 0, bone_count = GetBoneCount(); bone_index < bone_count; ++bone_index)
    {
        wsp::graphic::RenderBone* bone = GetBone(bone_index);
        WSP_FATAL_IF_FALSE(bone->GetSmoothSkinningMatrixTableIndex() == bone_index, "invalid bone, matrix pallette index must be the same with bone index.");
        {
            wsp::graphic::TransformMatrixF bone_mat = bone->transform_matrix();
            memcpy(transform_matrix_palette_ + bone_index * BoneMatrixElementCount,
                bone_mat.data(),
                sizeof(float) * BoneMatrixElementCount);
        }

        {
            wsp::graphic::TransformMatrixF bone_mat = bone->normal_transform_matrix();
            float* inv_transpose_bone_mat_ptr = normal_transform_matrix_palette_ + bone_index * NormalBoneMatrixElementCount;
            bone_mat.GetDataAsMatrix3x3(inv_transpose_bone_mat_ptr);
        }
    }
}

wsp::graphic::RenderBone* wsp::graphic::RenderSkeleton::FindBone(const char* bone_name)
{
    for (int bone_index = 0; bone_index < bones_.length(); ++bone_index)
    {
        RenderBone* bone = &bones_[bone_index];
        if (strcmp(bone_name, bone->name()) == 0)
        {
            return bone;
        }
    }

    return nullptr;
}

void wsp::graphic::RenderSkeleton::ConvertParentBoneNameToAddress()
{
    for (int current_bone_index = 0; current_bone_index < bones_.length(); ++current_bone_index)
    {
        RenderBone* current_bone = &bones_[current_bone_index];
        RenderBone* parent_bone = FindBone(current_bone->parent_bone_name());
        if (parent_bone != nullptr)
        {
            parent_bone->AddChildBone(current_bone);
        }

        current_bone->set_parent_bone(parent_bone);
    }
}

void wsp::graphic::RenderSkeleton::ConvertBindPoseLocalTransformMatricesToGlobal()
{
    RenderBone* root_bone = FindRootBone();
    if (root_bone == nullptr)
    {
        return;
    }

    {
        root_bone->set_transform_matrix(
            root_bone->CalculateLocalTransformMatrix());

        wsp::graphic::TransformMatrixF inv_bone_mat;
        bool success_obtain_bone_mat = root_bone->transform_matrix().GetInverseMatrix(&inv_bone_mat);
        WSP_ASSERT(success_obtain_bone_mat, "failed to obtaine bone matrix.");
        root_bone->set_normal_transform_matrix(inv_bone_mat.Transpose());
    }
    ConvertBindPoseLocalTransformMatricesToGlobal(root_bone);
}

void wsp::graphic::RenderSkeleton::ConvertBindPoseLocalTransformMatricesToGlobal(RenderBone* bone)
{
    for (int bone_index = 0; bone_index < bone->GetChildBoneCount(); ++bone_index)
    {
        RenderBone* child_bone = bone->GetChildBone(bone_index);
        child_bone->set_bind_pose_transform_matrix(
            child_bone->CalculateBindPoseLocalTransformMatrix() * bone->bind_pose_transform_matrix());

        // 法線用変換行列の計算
        {
            wsp::graphic::TransformMatrixF inv_bone_mat;
            bool success_obtain_bone_mat = child_bone->bind_pose_transform_matrix().GetInverseMatrix(&inv_bone_mat);
            WSP_ASSERT(success_obtain_bone_mat, "failed to obtaine bone matrix.");
            child_bone->set_bind_pose_normal_transform_matrix(inv_bone_mat.Transpose());
        }

        ConvertBindPoseLocalTransformMatricesToGlobal(child_bone);
    }
}

void wsp::graphic::RenderSkeleton::ConvertLocalTransformMatricesToGlobal()
{
    RenderBone* root_bone = FindRootBone();
    if (root_bone == nullptr)
    {
        return;
    }

    {
        root_bone->set_transform_matrix(
            root_bone->CalculateLocalTransformMatrix());

        wsp::graphic::TransformMatrixF inv_bone_mat;
        bool success_obtain_bone_mat = root_bone->transform_matrix().GetInverseMatrix(&inv_bone_mat);
        WSP_ASSERT(success_obtain_bone_mat, "failed to obtaine bone matrix.");
        root_bone->set_normal_transform_matrix(inv_bone_mat.Transpose());
    }

    ConvertLocalTransformMatricesToGlobal(root_bone);

    // スムーススキニング用にボーン空間への変換行列をかける
    for (int bone_index = 0; bone_index < bones_.length(); ++bone_index)
    {
        RenderBone* bone = &bones_[bone_index];
        bone->set_transform_matrix(
            bone->bind_pose_inverse_transform_matrix() * bone->transform_matrix());
        bone->set_normal_transform_matrix(
            bone->bind_pose_inverse_normal_transform_matrix() * bone->normal_transform_matrix());
    }
}

void wsp::graphic::RenderSkeleton::ConvertLocalTransformMatricesToGlobal(RenderBone* bone)
{
    for (int bone_index = 0; bone_index < bone->GetChildBoneCount(); ++bone_index)
    {
        RenderBone* child_bone = bone->GetChildBone(bone_index);
        child_bone->set_transform_matrix(
            child_bone->CalculateLocalTransformMatrix() * bone->transform_matrix());

        // 法線用変換行列の計算
        {
            wsp::graphic::TransformMatrixF inv_bone_mat;
            bool success_obtain_bone_mat = child_bone->transform_matrix().GetInverseMatrix(&inv_bone_mat);
            WSP_ASSERT(success_obtain_bone_mat, "failed to obtaine bone matrix.");
            child_bone->set_normal_transform_matrix(inv_bone_mat.Transpose());
        }

        ConvertLocalTransformMatricesToGlobal(child_bone);
    }
}

wsp::graphic::RenderBone* wsp::graphic::RenderSkeleton::FindRootBone()
{
    for (int bone_index = 0; bone_index < bones_.length(); ++bone_index)
    {
        RenderBone* current_bone = &bones_[bone_index];
        if (current_bone->parent_bone() == nullptr)
        {
            return current_bone;
        }
    }

    return nullptr;
}

void wsp::graphic::RenderSkeleton::ClearBones()
{
    bones_.Clear();
}

void wsp::graphic::RenderSkeleton::AddBone(const RenderBone& bone)
{
    WSP_ASSERT(GetBoneCount() < MaxBoneCount, "bone count is too large");
    bones_.Append(bone);
}

wsp::graphic::TransformMatrixF wsp::graphic::RenderBone::CalculateBindPoseLocalNormalTransformMatrix() const
{
    wsp::graphic::TransformMatrixF inv_mat;
    bool success = CalculateBindPoseLocalTransformMatrix().GetInverseMatrix(&inv_mat);
    WSP_FATAL_IF_FALSE(success, "failed to calculate inverse matrix of bone.");
    return inv_mat.Transpose();
}


wsp::graphic::TransformMatrixF wsp::graphic::RenderBone::CalculateBindPoseLocalTransformMatrix() const
{
    return CalculateBoneMatrix(
        bind_pose_translate_x(), bind_pose_translate_y(), bind_pose_translate_z(),
        bind_pose_rotate_x(), bind_pose_rotate_y(), bind_pose_rotate_z(),
        bind_pose_scale_x(), bind_pose_scale_y(), bind_pose_scale_z(),
        bind_pose_prerotate_x(), bind_pose_prerotate_y(), bind_pose_prerotate_z(),
        bind_pose_postrotate_x(), bind_pose_postrotate_y(), bind_pose_postrotate_z());
}

wsp::graphic::TransformMatrixF wsp::graphic::RenderBone::CalculateLocalTransformMatrix() const
{
    return CalculateBoneMatrix(
        translate_x(),  translate_y(),  translate_z(),
        rotate_x(),     rotate_y(),     rotate_z(),
        scale_x(),      scale_y(),      scale_z(),
        bind_pose_prerotate_x(), bind_pose_prerotate_y(), bind_pose_prerotate_z(),
        bind_pose_postrotate_x(), bind_pose_postrotate_y(), bind_pose_postrotate_z());
}

wsp::graphic::TransformMatrixF wsp::graphic::RenderBone::CalculateInverseBindPoseLocalTransformMatrix() const
{
    return CalculateInverseBoneMatrix(
        bind_pose_translate_x(), bind_pose_translate_y(), bind_pose_translate_z(),
        bind_pose_rotate_x(), bind_pose_rotate_y(), bind_pose_rotate_z(),
        bind_pose_scale_x(), bind_pose_scale_y(), bind_pose_scale_z(),
        bind_pose_prerotate_x(), bind_pose_prerotate_y(), bind_pose_prerotate_z(),
        bind_pose_postrotate_x(), bind_pose_postrotate_y(), bind_pose_postrotate_z());
}

wsp::graphic::TransformMatrixF wsp::graphic::RenderBone::CalculateInverseLocalTransformMatrix() const
{
    return CalculateInverseBoneMatrix(
        translate_x(), translate_y(), translate_z(),
        rotate_x(), rotate_y(), rotate_z(),
        scale_x(), scale_y(), scale_z(),
        bind_pose_prerotate_x(), bind_pose_prerotate_y(), bind_pose_prerotate_z(),
        bind_pose_postrotate_x(), bind_pose_postrotate_y(), bind_pose_postrotate_z());
}