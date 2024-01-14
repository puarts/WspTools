//! WavefrontObj.h
// 

#ifndef _WavefrontObj_H_
#define _WavefrontObj_H_

#include <wsp/common/tpl_cl-vector3.h>
#include <vector>

namespace wsp {
    namespace graphic {
        static const int MaxMtlNameLength = 64;
        static const int MaxMtlTexNameLength = 256;

        // �}�e���A���f�[�^�iMtl�`���p�j
        struct Mtl
        {
            WspColor3F kd;            // �g�U���ˌ��i�Ƃ肠�����g�U���ˌ��� glColor3f() �Ŏg�p����j
            char name[MaxMtlNameLength];          // �}�e���A����
            char texture_name[MaxMtlTexNameLength];  // �e�N�X�`���摜�̃t�@�C����

            Mtl()
            {
                strcpy(name, "");
                strcpy(texture_name, "");
            }
        };

        // �􉽌`��f�[�^�iObj�`���p�j
        struct Obj
        {
            int num_vertices;  // ���_��
            std::vector<WspVec3F> vertex_positions;      // ���_���W�z�� [num_vertices]

            int num_normals;
            std::vector<WspVec3F> normals;       // �@���x�N�g���z�� [num_normals]

            int num_tex_coords;
            std::vector<WspVec2F> tex_coords;    // �e�N�X�`�����W�z�� [num_tex_coords]

            int num_skinning_weights;
            std::vector<WspVec4F> skinning_weights;
            std::vector<WspIntVec4> skinning_indices;

            int num_triangles; // �O�p�ʐ�
            std::vector<int> triangle_vertex_position_indices;      // �O�p�ʂ̊e���_�̒��_���W�ԍ��z�� [num_triangles*3]
            std::vector<int> triangle_normal_indices;     // �O�p�ʂ̊e���_�̖@���x�N�g���ԍ��z�� [num_triangles*3]
            std::vector<int> triangle_tex_coord_indices;     // �O�p�ʂ̊e���_�̃e�N�X�`�����W�ԍ��z�� [num_triangles*3]
            std::vector<int> triangle_skinning_index_indices;
            std::vector<int> triangle_skinning_weight_indices;

            int parent_bone_index;

            wsp::graphic::Mtl material; // �}�e���A��
        };
    }
}

#endif
