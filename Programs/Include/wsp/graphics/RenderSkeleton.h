#pragma once

#include <wsp/common/tpl_cl-vector.h>
#include <wsp/graphics/tpl_cl-transform_matrix.h>
#include <wsp/graphics/fn-transform.h>

namespace wsp {
    namespace graphic {

        class WSP_DLL_EXPORT RenderBone
        {
        public:
            RenderBone()
                : parent_bone_(nullptr)
                , smooth_skinning_matrix_table_index_(-1)
            {
                set_name("");
                set_parent_bone_name("");
                bind_pose_transform_matrix_.SetToIdentity();
                bind_pose_normal_transform_matrix_.SetToIdentity();
                transform_matrix_.SetToIdentity();
                normal_transform_matrix_.SetToIdentity();
            }

            RenderBone(
                const char* name,
                const char* parent_bone_name,
                float tx, float ty, float tz,
                float rx, float ry, float rz,
                float sx, float sy, float sz,
                float prerotate_x, float prerotate_y, float prerotate_z,
                float postrotate_x, float postrotate_y, float postrotate_z,
                int smooth_skinning_matrix_table_index)
                : RenderBone()
            {
                smooth_skinning_matrix_table_index_ = smooth_skinning_matrix_table_index;
                set_name(name);
                if (parent_bone_name != nullptr)
                {
                    set_parent_bone_name(parent_bone_name);
                }
                set_bind_pose_translate_x(tx);
                set_bind_pose_translate_y(ty);
                set_bind_pose_translate_z(tz);
                set_bind_pose_rotate_x(rx);
                set_bind_pose_rotate_y(ry);
                set_bind_pose_rotate_z(rz);
                set_bind_pose_scale_x(sx);
                set_bind_pose_scale_y(sy);
                set_bind_pose_scale_z(sz);
                set_bind_pose_prerotate_x(prerotate_x);
                set_bind_pose_prerotate_y(prerotate_y);
                set_bind_pose_prerotate_z(prerotate_z);
                set_bind_pose_postrotate_x(postrotate_x);
                set_bind_pose_postrotate_y(postrotate_y);
                set_bind_pose_postrotate_z(postrotate_z);
                ResetToBindPose();
            }

            int GetSmoothSkinningMatrixTableIndex() const
            {
                return smooth_skinning_matrix_table_index_;
            }

            RenderBone(const RenderBone& rhs)
            {
                CopyFrom(rhs);
            }

            ~RenderBone();

            float translate_x() const { return current_pose_.translate_x_; }
            float translate_y() const { return current_pose_.translate_y_; }
            float translate_z() const { return current_pose_.translate_z_; }
            float rotate_x()    const { return current_pose_.rotate_x_; }
            float rotate_y()    const { return current_pose_.rotate_y_; }
            float rotate_z()    const { return current_pose_.rotate_z_; }
            float scale_x()     const { return current_pose_.scale_x_; }
            float scale_y()     const { return current_pose_.scale_y_; }
            float scale_z()     const { return current_pose_.scale_z_; }
            float prerotate_x()    const { return current_pose_.prerotate_x_; }
            float prerotate_y()    const { return current_pose_.prerotate_y_; }
            float prerotate_z()    const { return current_pose_.prerotate_z_; }
            float postrotate_x()    const { return current_pose_.postrotate_x_; }
            float postrotate_y()    const { return current_pose_.postrotate_y_; }
            float postrotate_z()    const { return current_pose_.postrotate_z_; }

            void set_translate_x(float value) { current_pose_.translate_x_ = value; }
            void set_translate_y(float value) { current_pose_.translate_y_ = value; }
            void set_translate_z(float value) { current_pose_.translate_z_ = value; }
            void set_rotate_x   (float value) { current_pose_.rotate_x_ = value; }
            void set_rotate_y   (float value) { current_pose_.rotate_y_ = value; }
            void set_rotate_z   (float value) { current_pose_.rotate_z_ = value; }
            void set_scale_x    (float value) { current_pose_.scale_x_ = value; }
            void set_scale_y    (float value) { current_pose_.scale_y_ = value; }
            void set_scale_z    (float value) { current_pose_.scale_z_ = value; }
            void set_prerotate_x(float value) { current_pose_.prerotate_x_ = value; }
            void set_prerotate_y(float value) { current_pose_.prerotate_y_ = value; }
            void set_prerotate_z(float value) { current_pose_.prerotate_z_ = value; }
            void set_postrotate_x(float value) { current_pose_.postrotate_x_ = value; }
            void set_postrotate_y(float value) { current_pose_.postrotate_y_ = value; }
            void set_postrotate_z(float value) { current_pose_.postrotate_z_ = value; }

            float bind_pose_translate_x() const { return bind_pose_.translate_x_; }
            float bind_pose_translate_y() const { return bind_pose_.translate_y_; }
            float bind_pose_translate_z() const { return bind_pose_.translate_z_; }
            float bind_pose_rotate_x   () const { return bind_pose_.rotate_x_; }
            float bind_pose_rotate_y   () const { return bind_pose_.rotate_y_; }
            float bind_pose_rotate_z   () const { return bind_pose_.rotate_z_; }
            float bind_pose_scale_x    () const { return bind_pose_.scale_x_; }
            float bind_pose_scale_y    () const { return bind_pose_.scale_y_; }
            float bind_pose_scale_z    () const { return bind_pose_.scale_z_; }
            float bind_pose_prerotate_x() const { return bind_pose_.prerotate_x_; }
            float bind_pose_prerotate_y() const { return bind_pose_.prerotate_y_; }
            float bind_pose_prerotate_z() const { return bind_pose_.prerotate_z_; }
            float bind_pose_postrotate_x() const { return bind_pose_.postrotate_x_; }
            float bind_pose_postrotate_y() const { return bind_pose_.postrotate_y_; }
            float bind_pose_postrotate_z() const { return bind_pose_.postrotate_z_; }

            void set_bind_pose_translate_x(float value) { bind_pose_.translate_x_ = value; }
            void set_bind_pose_translate_y(float value) { bind_pose_.translate_y_ = value; }
            void set_bind_pose_translate_z(float value) { bind_pose_.translate_z_ = value; }
            void set_bind_pose_rotate_x   (float value) { bind_pose_.rotate_x_ = value; }
            void set_bind_pose_rotate_y   (float value) { bind_pose_.rotate_y_ = value; }
            void set_bind_pose_rotate_z   (float value) { bind_pose_.rotate_z_ = value; }
            void set_bind_pose_scale_x    (float value) { bind_pose_.scale_x_ = value; }
            void set_bind_pose_scale_y    (float value) { bind_pose_.scale_y_ = value; }
            void set_bind_pose_scale_z    (float value) { bind_pose_.scale_z_ = value; }
            void set_bind_pose_prerotate_x(float value) { bind_pose_.prerotate_x_ = value; }
            void set_bind_pose_prerotate_y(float value) { bind_pose_.prerotate_y_ = value; }
            void set_bind_pose_prerotate_z(float value) { bind_pose_.prerotate_z_ = value; }
            void set_bind_pose_postrotate_x(float value) { bind_pose_.postrotate_x_ = value; }
            void set_bind_pose_postrotate_y(float value) { bind_pose_.postrotate_y_ = value; }
            void set_bind_pose_postrotate_z(float value) { bind_pose_.postrotate_z_ = value; }

            const RenderBone* parent_bone() const
            {
                return parent_bone_;
            }

            void set_parent_bone(const RenderBone* parent_bone)
            {
                parent_bone_ = parent_bone;
            }

            const char* name() const
            {
                return name_;
            }

            void set_name(const char* name)
            {
                WSP_ASSERT(strlen(name) < MaxBoneNameSize, "Exceeds max bone name size.");
                strncpy(name_, name, MaxBoneNameSize);
            }

            const char* parent_bone_name() const
            {
                return parent_bone_name_;
            }

            void set_parent_bone_name(const char* name)
            {
                WSP_ASSERT(strlen(name) < MaxBoneNameSize, "Exceeds max bone name size.");
                strncpy(parent_bone_name_, name, MaxBoneNameSize);
            }

            int GetChildBoneCount() const
            {
                return child_bones_.length();
            }

            void AddChildBone(RenderBone* bone);

            RenderBone* GetChildBone(int index)
            {
                return child_bones_[index];
            }

            wsp::graphic::TransformMatrixF& bind_pose_transform_matrix()
            {
                return bind_pose_transform_matrix_;
            }

            const wsp::graphic::TransformMatrixF& bind_pose_transform_matrix() const
            {
                return bind_pose_transform_matrix_;
            }

            void set_bind_pose_transform_matrix(const wsp::graphic::TransformMatrixF& matrix)
            {
                bind_pose_transform_matrix_ = matrix;
            }

            wsp::graphic::TransformMatrixF& bind_pose_normal_transform_matrix()
            {
                return bind_pose_normal_transform_matrix_;
            }

            const wsp::graphic::TransformMatrixF& bind_pose_normal_transform_matrix() const
            {
                return bind_pose_normal_transform_matrix_;
            }

            void set_bind_pose_normal_transform_matrix(const wsp::graphic::TransformMatrixF& matrix)
            {
                bind_pose_normal_transform_matrix_ = matrix;
            }

            wsp::graphic::TransformMatrixF& bind_pose_inverse_normal_transform_matrix()
            {
                return bind_pose_inverse_normal_transform_matrix_;
            }

            const wsp::graphic::TransformMatrixF& bind_pose_inverse_normal_transform_matrix() const
            {
                return bind_pose_inverse_normal_transform_matrix_;
            }

            void set_bind_pose_inverse_normal_transform_matrix(const wsp::graphic::TransformMatrixF& matrix)
            {
                bind_pose_inverse_normal_transform_matrix_ = matrix;
            }

            wsp::graphic::TransformMatrixF& transform_matrix()
            {
                return transform_matrix_;
            }

            const wsp::graphic::TransformMatrixF& transform_matrix() const
            {
                return transform_matrix_;
            }

            void set_transform_matrix(const wsp::graphic::TransformMatrixF& matrix)
            {
                transform_matrix_ = matrix;
            }

            wsp::graphic::TransformMatrixF& bind_pose_inverse_transform_matrix()
            {
                return bind_pose_inverse_transform_matrix_;
            }

            const wsp::graphic::TransformMatrixF& bind_pose_inverse_transform_matrix() const
            {
                return bind_pose_inverse_transform_matrix_;
            }

            void set_bind_pose_inverse_transform_matrix(const wsp::graphic::TransformMatrixF& matrix)
            {
                bind_pose_inverse_transform_matrix_ = matrix;
            }

            wsp::graphic::TransformMatrixF& normal_transform_matrix()
            {
                return normal_transform_matrix_;
            }

            const wsp::graphic::TransformMatrixF& normal_transform_matrix() const
            {
                return normal_transform_matrix_;
            }

            void set_normal_transform_matrix(const wsp::graphic::TransformMatrixF& matrix)
            {
                normal_transform_matrix_ = matrix;
            }

            wsp::graphic::TransformMatrixF CalculateBindPoseLocalTransformMatrix() const;
            wsp::graphic::TransformMatrixF CalculateLocalTransformMatrix() const;
            wsp::graphic::TransformMatrixF CalculateInverseBindPoseLocalTransformMatrix() const;
            wsp::graphic::TransformMatrixF CalculateInverseLocalTransformMatrix() const;

            wsp::graphic::TransformMatrixF CalculateBindPoseLocalNormalTransformMatrix() const;


            void ResetToBindPose()
            {
                current_pose_ = bind_pose_;
            }

            RenderBone& operator=(const RenderBone& rhs)
            {
                CopyFrom(rhs);
                return *this;
            }

        private:
            struct Transform
            {
                float translate_x_;
                float translate_y_;
                float translate_z_;
                float rotate_x_;
                float rotate_y_;
                float rotate_z_;
                float scale_x_;
                float scale_y_;
                float scale_z_;
                float prerotate_x_;
                float prerotate_y_;
                float prerotate_z_;
                float postrotate_x_;
                float postrotate_y_;
                float postrotate_z_;

                Transform()
                    : translate_x_(0.0f)
                    , translate_y_(0.0f)
                    , translate_z_(0.0f)
                    , rotate_x_(0.0f)
                    , rotate_y_(0.0f)
                    , rotate_z_(0.0f)
                    , scale_x_(1.0f)
                    , scale_y_(1.0f)
                    , scale_z_(1.0f)
                    , prerotate_x_(0.0f)
                    , prerotate_y_(0.0f)
                    , prerotate_z_(0.0f)
                    , postrotate_x_(0.0f)
                    , postrotate_y_(0.0f)
                    , postrotate_z_(0.0f)
                {
                }
            };

            void CopyFrom(const RenderBone& rhs);

            static const size_t MaxBoneNameSize = 32;
            char name_[MaxBoneNameSize];
            char parent_bone_name_[MaxBoneNameSize];
            const RenderBone* parent_bone_;
            wsp::Vector<RenderBone*> child_bones_;
            wsp::graphic::TransformMatrixF bind_pose_transform_matrix_;
            wsp::graphic::TransformMatrixF bind_pose_inverse_transform_matrix_;
            wsp::graphic::TransformMatrixF bind_pose_normal_transform_matrix_;
            wsp::graphic::TransformMatrixF bind_pose_inverse_normal_transform_matrix_;
            wsp::graphic::TransformMatrixF transform_matrix_;
            wsp::graphic::TransformMatrixF normal_transform_matrix_;
            Transform current_pose_;
            Transform bind_pose_;
            int smooth_skinning_matrix_table_index_;
        };

        class WSP_DLL_EXPORT RenderSkeleton
        {
        public:
            RenderSkeleton()
            {
            }
            ~RenderSkeleton();

            RenderBone* GetBone(int index)
            {
                return &bones_[index];
            }

            const RenderBone* GetBone(int index) const
            {
                return &bones_[index];
            }

            void AddBone(const RenderBone& bone);

            int GetBoneCount() const
            {
                return bones_.length();
            }

            void ClearBones();

            void ResolveBoneLinks()
            {
                // 親の名前をポインタに変換
                ConvertParentBoneNameToAddress();

                // ローカル行列を親子階層含めたグローバルな行列に変換
                ConvertBindPoseLocalTransformMatricesToGlobal();
            }

            void ResetToBindPose()
            {
                for (int bone_index = 0; bone_index < bones_.length(); ++bone_index)
                {
                    RenderBone* bone = &bones_[bone_index];
                    bone->ResetToBindPose();
                }
            }

            void UpdateMatrixPalette()
            {
                ConvertLocalTransformMatricesToGlobal();
                StoreMaricesIntoMatrixPalette();
            }

            float* transform_matrix_palette()
            {
                return transform_matrix_palette_;
            }

            float* inverse_transpose_transform_matrix_palette()
            {
                return normal_transform_matrix_palette_;
            }

            RenderBone* FindBone(const char* bone_name);

            wsp::graphic::TransformMatrixF CalculateSmoothSkinningBoneTransformMatrix(const RenderBone* bone) const
            {
                wsp::graphic::TransformMatrixF bone_mat = CalculateBoneTransformMatrix(bone);
                wsp::graphic::TransformMatrixF inv_bindpose_mat = CalculateBindPoseBoneTransformMatrix(bone);
                return bone_mat * inv_bindpose_mat;
            }

            wsp::graphic::TransformMatrixF CalculateBoneTransformMatrix(const RenderBone* bone) const
            {
                const RenderBone* parent_bone = bone->parent_bone();
                if (parent_bone != nullptr)
                {
                    return bone->CalculateLocalTransformMatrix() * CalculateBoneTransformMatrix(parent_bone);
                }
                else
                {
                    return bone->CalculateLocalTransformMatrix();
                }
            }

            // スムーススキニングの変換行列を計算するためのバインドポーズの逆行列を事前計算しておく
            void CalculateAndStoreInverseBindPoseMatrices()
            {
                for (int bone_index = 0; bone_index < bones_.length(); ++bone_index)
                {
                    RenderBone* bone = &bones_[bone_index];
                    {
                        TransformMatrixF bone_transform_mat = CalculateBindPoseBoneTransformMatrix(bone);
                        TransformMatrixF inv_transform_mat;
                        bool success = bone_transform_mat.GetInverseMatrix(&inv_transform_mat);
                        WSP_FATAL_IF_FALSE(success, "failed to calculate inverse matrix of bone.");
                        bone->set_bind_pose_inverse_transform_matrix(inv_transform_mat);
                    }

                    {
                        TransformMatrixF normal_transform_mat = CalculateBindPoseBoneNormalTransformMatrix(bone);
                        TransformMatrixF inv_transform_mat;
                        bool success = normal_transform_mat.GetInverseMatrix(&inv_transform_mat);
                        WSP_FATAL_IF_FALSE(success, "failed to calculate inverse matrix of bone.");
                        bone->set_bind_pose_inverse_normal_transform_matrix(inv_transform_mat);
                    }
                }
            }

        private:

            wsp::graphic::TransformMatrixF CalculateBindPoseBoneTransformMatrix(const RenderBone* bone) const
            {
                const RenderBone* parent_bone = bone->parent_bone();
                if (parent_bone != nullptr)
                {
                    return bone->CalculateBindPoseLocalTransformMatrix() * CalculateBindPoseBoneTransformMatrix(parent_bone);
                }
                else
                {
                    return bone->CalculateBindPoseLocalTransformMatrix();
                }
            }

            wsp::graphic::TransformMatrixF CalculateBindPoseBoneNormalTransformMatrix(const RenderBone* bone) const
            {
                const RenderBone* parent_bone = bone->parent_bone();


                if (parent_bone != nullptr)
                {
                    return bone->CalculateBindPoseLocalNormalTransformMatrix() * CalculateBindPoseBoneTransformMatrix(parent_bone);
                }
                else
                {
                    return bone->CalculateBindPoseLocalNormalTransformMatrix();
                }
            }

            void StoreMaricesIntoMatrixPalette();


            void ConvertParentBoneNameToAddress();

            void ConvertBindPoseLocalTransformMatricesToGlobal();

            void ConvertBindPoseLocalTransformMatricesToGlobal(RenderBone* bone);

            void ConvertLocalTransformMatricesToGlobal();

            void ConvertLocalTransformMatricesToGlobal(RenderBone* bone);

            RenderBone* FindRootBone();

            wsp::Vector<RenderBone> bones_;
            static const int MaxBoneCount = 128;
            static const size_t BoneMatrixElementCount = 16;
            static const size_t NormalBoneMatrixElementCount = 9;
            float transform_matrix_palette_[MaxBoneCount * BoneMatrixElementCount];
            float normal_transform_matrix_palette_[MaxBoneCount * NormalBoneMatrixElementCount];
        };
    }
}
