// @file fn-graphic_io.h
// 

#ifndef __WSP_GRAPHIC_FUNCTION_GRAPHIC_IO_H__
#define __WSP_GRAPHIC_FUNCTION_GRAPHIC_IO_H__

#include <vector>
#include <map>
#include <string>

#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#include <wsp/common/tpl_cl-vector.h>
#endif
#include "_define_graphic.h"
#include "fn-graphic_log.h"
#include "fn-graphic_proc.h"
#include "RenderGeometry.h"
#include "WavefrontObj.h"
#include <wsp/graphics/RenderSkeleton.h>

namespace wsp{ namespace graphic{
    
    //! @brief ���_���W�A�@���A�e�N�X�`�����W���ɃC���f�b�N�X�t������Ă���f�[�^��
    //!        ���_���W�A�@���A�e�N�X�`�����W�̃Z�b�g�ŃC���f�b�N�X�t�����ꂽ�f�[�^�ɕϊ����܂��B
    WSP_DLL_EXPORT void ConvertObjToIndexedElementData(
        int* o_unique_vertex_count,
        WspVec3F* o_vertices,
        WspVec2F* o_tex_coords,
        WspVec3F* o_normals,
        int* o_draw_element_index,
        int array_length,
        const wsp::graphic::Obj& in_obj);

    WSP_DLL_EXPORT void ConvertObjToIndexedElementData(
        int* o_unique_vertex_count,
        WspVec3F* o_vertices,
        WspVec2F* o_tex_coords,
        WspVec3F* o_normals,
        WspIntVec4* o_skinning_bone_indices,
        WspVec4F* o_skinning_weights,
        int* o_draw_element_index,
        int array_length,
        const wsp::graphic::Obj& in_obj);

    //! @brief Obj�̃C���f�b�N�X�z��͈ʒu�AUV���W�A�@�����ꂼ��ŕ�����Ă��āAglDrawElements�ŕ`��ł��Ȃ��̂ŁA�����C���f�b�N�X�z��ɂ܂Ƃ߂邽�߂̕ϊ����s���܂��B
    WSP_DLL_EXPORT void ConvertObjToIndexedElementData(
        std::vector<WspVec3F>& o_vertices,
        std::vector<WspVec2F>& o_tex_coords,
        std::vector<WspVec3F>& o_normals,
        std::vector<int>& o_draw_element_index,
        const Obj& in_obj);

    WSP_DLL_EXPORT void ConvertObjToIndexedElementData(
        std::vector<WspVec3F>& o_vertices,
        std::vector<WspVec2F>& o_tex_coords,
        std::vector<WspVec3F>& o_normals,
        std::vector<WspIntVec4>& o_skinning_indices,
        std::vector<WspVec4F>& o_skinning_weights,
        std::vector<int>& o_draw_element_index,
        const Obj& in_obj);

    inline void ConvertObjToIndexedElementData(
        wsp::Vector<WspVec3F>& o_vertices,
        wsp::Vector<WspVec2F>& o_tex_coords,
        wsp::Vector<WspVec3F>& o_normals,
        wsp::Vector<int>& o_draw_element_index,
        const Obj& in_obj);

    inline void ConvertObjToIndexedElementData(
        wsp::Vector<WspVec3F>& o_vertices,
        wsp::Vector<WspVec2F>& o_tex_coords,
        wsp::Vector<WspVec3F>& o_normals,
        wsp::Vector<WspIntVec4>& o_skinning_indices,
        wsp::Vector<WspVec4F>& o_skinning_weights,
        wsp::Vector<int>& o_draw_element_index,
        const Obj& in_obj);

    //! @brief �X���[�X�X�L�j���O�p�̒��_���W�A�@���ɕϊ����܂��B
    WSP_DLL_EXPORT void ConvertToSmoothSkinnedPositionsAndNormals(
        wsp::Vector<WspVec3F>* o_skinned_positions,
        wsp::Vector<WspVec3F>* o_skinned_normals,
        const wsp::Vector<WspVec3F>& in_positions,
        const wsp::Vector<WspVec3F>& in_normals,
        const wsp::Vector<WspIntVec4>& in_skinning_indices,
        const wsp::Vector<WspVec4F>& in_skinning_weights,
        const wsp::graphic::RenderSkeleton& in_skeleton);

    //! @brief �X���[�X�X�L�j���O�p�̒��_���W�ɕϊ����܂��B
    WSP_DLL_EXPORT void ConvertToSmoothSkinnedPositions(
        wsp::Vector<WspVec3F>* o_skinned_positions,
        const wsp::Vector<WspVec3F>& in_positions,
        const wsp::Vector<WspIntVec4>& in_skinning_indices,
        const wsp::Vector<WspVec4F>& in_skinning_weights,
        const wsp::graphic::RenderSkeleton& in_skeleton);

    //! @brief �X���[�X�X�L�j���O�p�̒��_���W�ɕϊ����܂��B
    WSP_DLL_EXPORT void ConvertToSmoothSkinnedPositions(
        float* o_skinned_positions, int skinned_pos_component_count,
        const float* in_positions, int in_pos_component_count,
        const int* in_skinning_indices, const float* in_skinning_weights, int skinning_weight_component_count,
        int vertex_count,
        const wsp::graphic::RenderSkeleton& in_skeleton);

    //! @brief �X���[�X�X�L�j���O�p�̒��_���W�ɕϊ����܂��B
    inline void ConvertToSmoothSkinnedPositions(
        float* io_positions, int pos_component_count,
        const int* in_skinning_indices, const float* in_skinning_weights, int skinning_weight_component_count,
        int vertex_count,
        const wsp::graphic::RenderSkeleton& in_skeleton)
    {
        ConvertToSmoothSkinnedPositions(
            io_positions, pos_component_count,
            io_positions, pos_component_count,
            in_skinning_indices,
            in_skinning_weights,
            skinning_weight_component_count,
            vertex_count,
            in_skeleton);
    }

    //! @brief �X���[�X�X�L�j���O�p�̖@���ɕϊ����܂��B
    WSP_DLL_EXPORT void ConvertToSmoothSkinnedNormals(
        wsp::Vector<WspVec3F>* o_skinned_normals,
        const wsp::Vector<WspVec3F>& in_normals,
        const wsp::Vector<WspIntVec4>& in_skinning_indices,
        const wsp::Vector<WspVec4F>& in_skinning_weights,
        const wsp::graphic::RenderSkeleton& in_skeleton);

    WSP_DLL_EXPORT void ConvertToSmoothSkinnedPositionsAndNormals(
        wsp::graphic::RenderGeometry* io_geom,
        const wsp::graphic::RenderSkeleton& in_skeleton);
    
    //! Load .obj file
    inline bool LoadObj(wsp::graphic::Obj* o_obj, const char* filename);


    inline bool LoadObj(wsp::graphic::RenderGeometry* o_geom, const char* file_path)
    {
        wsp::graphic::Obj obj_data;
        bool success = wsp::graphic::LoadObj(&obj_data, file_path);
        if (!success)
        {
            return false;
        }

        if (obj_data.num_normals == 0)
        {
            obj_data.num_normals = obj_data.num_vertices;
            obj_data.normals.resize(obj_data.num_vertices);
            wsp::graphic::CreateNormalsFromVertices(
                (wsp::Vector3<float>*)obj_data.normals.data(), obj_data.num_normals,
                (wsp::Vector3<float>*)obj_data.vertex_positions.data(), obj_data.num_vertices,
                obj_data.triangle_vertex_position_indices.data(), obj_data.num_triangles * 3);

            obj_data.triangle_normal_indices.resize(obj_data.triangle_vertex_position_indices.size());
            memcpy(obj_data.triangle_normal_indices.data(), obj_data.triangle_vertex_position_indices.data(), sizeof(int) * obj_data.triangle_vertex_position_indices.size());
        }

        wsp::graphic::ConvertObjToIndexedElementData(
            o_geom->vertex_positions(),
            o_geom->vertex_texture_coords(),
            o_geom->vertex_normals(),
            o_geom->draw_element_indices(),
            obj_data);

        return true;
    }

    //! Load .mtl file
    //inline void LoadMtl(const char* filename, wsp::graphic::Obj* obj);

    //! Scale Object (return height after obj scaled�j
    WSP_DLL_EXPORT float ScaleObj(wsp::graphic::Obj* obj, float max_size);

    //! Render .obj data
    WSP_DLL_EXPORT void RenderObj(wsp::graphic::Obj* obj);

    inline wsp::graphic::Obj* CreateRectangleObj()
    {
        wsp::graphic::Obj* obj = WSP_NEW wsp::graphic::Obj();
        obj->vertex_positions.push_back(WspVec3F(-1.0f, -1.0f, 0.0f));
        obj->vertex_positions.push_back(WspVec3F(1.0f, -1.0f, 0.0f));
        obj->vertex_positions.push_back(WspVec3F(1.0f, 1.0f, 0.0f));
        obj->vertex_positions.push_back(WspVec3F(-1.0f, 1.0f, 0.0f));
        obj->num_vertices = 4;

        obj->tex_coords.push_back(WspVec2F(0.0f, 1.0f));
        obj->tex_coords.push_back(WspVec2F(1.0f, 1.0f));
        obj->tex_coords.push_back(WspVec2F(1.0f, 0.0f));
        obj->tex_coords.push_back(WspVec2F(0.0f, 0.0f));
        obj->num_tex_coords = 4;

        obj->triangle_vertex_position_indices.push_back(0);
        obj->triangle_vertex_position_indices.push_back(1);
        obj->triangle_vertex_position_indices.push_back(2);

        obj->triangle_vertex_position_indices.push_back(3);
        obj->triangle_vertex_position_indices.push_back(0);
        obj->triangle_vertex_position_indices.push_back(2);
        obj->num_triangles = 2;

        return obj;
    }

    WSP_DLL_EXPORT void LoadObjSequence(wsp::graphic::Obj** o_objs, int* o_loaded_count, int out_array_length, const wsp::SeqInfo& seq_head_file_path);
    WSP_DLL_EXPORT void FreeObjSequence(wsp::graphic::Obj** o_objs, int length);

    // std::vector �� resize ����R�[�h�� inline ����Ȃ��ƃA�v����dll�̃q�[�v�̊Ǘ��̈Ⴂ?���炩�������j�����Ɏ���
    inline void LoadObjSequence(std::vector<wsp::graphic::Obj*>* o_objs, const char* seq_head_file_path)
    {
        wsp::SeqInfo sequence_info;
        wsp::GetSeqInfo(sequence_info, seq_head_file_path);
        o_objs->resize(sequence_info.end_frame - sequence_info.start_frame + 1);
        int loaded_count = 0;
        LoadObjSequence(o_objs->data(), &loaded_count, (int)o_objs->size(), sequence_info);
    }

    // std::vector �� resize ����R�[�h�� inline ����Ȃ��ƃA�v����dll�̃q�[�v�̊Ǘ��̈Ⴂ?���炩�������j�����Ɏ���
    inline void FreeObjSequence(std::vector<wsp::graphic::Obj*>* io_objs)
    {
        FreeObjSequence(io_objs->data(), (int)io_objs->size());
        io_objs->clear();
    }

    // std::vector �� resize ����R�[�h�� inline ����Ȃ��ƃA�v����dll�̃q�[�v�̊Ǘ��̈Ⴂ?���炩�������j�����Ɏ���
    inline void LoadObjSequence(std::vector<wsp::graphic::RenderGeometry*>* o_geometries, const char* filepath)
    {
        char path[MAX_LENGTH_OF_PATH];
        wsp::SeqInfo sequence_info;
        wsp::GetSeqInfo(sequence_info, filepath);
        for (int frame = sequence_info.start_frame; frame <= sequence_info.end_frame; ++frame)
        {
            wsp::GetSeqPath(path, sequence_info, frame);
            wsp::graphic::RenderGeometry* geom = WSP_NEW wsp::graphic::RenderGeometry();
            WSP_GRAPHIC_LOG("Loading %s\r", path);
            bool success = wsp::graphic::LoadObj(geom, path);
            WSP_ASSERT(success, "loading file failed: %s", path);
            o_geometries->push_back(geom);
        }

        WSP_GRAPHIC_LOG("\n");
    }

    // std::vector �� resize ����R�[�h�� inline ����Ȃ��ƃA�v����dll�̃q�[�v�̊Ǘ��̈Ⴂ?���炩�������j�����Ɏ���
    inline void FreeRenderGeometrySequence(std::vector<wsp::graphic::RenderGeometry*>* o_geometries)
    {
        for (int i = 0, end = (int)o_geometries->size(); i < end; ++i)
        {
            delete (*o_geometries)[i];
        }

        o_geometries->clear();
    }
}}





///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

// �o�b�t�@���i�T�C�Y�͓K���j
#define  BUFFER_LENGTH      1024
#define  MAX_VECTOR_SIZE    65535
#define  MAX_TRIANGLE_SIZE  65535
#define  MAX_MTL_SIZE         32

void wsp::graphic::ConvertObjToIndexedElementData(
    wsp::Vector<WspVec3F>& o_vertices,
    wsp::Vector<WspVec2F>& o_tex_coords,
    wsp::Vector<WspVec3F>& o_normals,
    wsp::Vector<int>& o_draw_element_index,
    const Obj& in_obj)
{
    int array_length = (int)in_obj.triangle_normal_indices.size();
    o_vertices.SetLength(array_length);
    o_tex_coords.SetLength(array_length);
    o_normals.SetLength(array_length);
    o_draw_element_index.SetLength(array_length);
    int unique_vertex_count;
    ConvertObjToIndexedElementData(
        &unique_vertex_count,
        o_vertices.data(),
        o_tex_coords.data(),
        o_normals.data(),
        o_draw_element_index.data(),
        array_length,
        in_obj);
    o_vertices.SetLength(unique_vertex_count);
    o_tex_coords.SetLength(unique_vertex_count);
    o_normals.SetLength(unique_vertex_count);
}

void wsp::graphic::ConvertObjToIndexedElementData(
    wsp::Vector<WspVec3F>& o_vertices,
    wsp::Vector<WspVec2F>& o_tex_coords,
    wsp::Vector<WspVec3F>& o_normals,
    wsp::Vector<WspIntVec4>& o_skinning_indices,
    wsp::Vector<WspVec4F>& o_skinning_weights,
    wsp::Vector<int>& o_draw_element_index,
    const Obj& in_obj)
{
    int array_length = (int)in_obj.triangle_normal_indices.size();
    o_vertices.SetLength(array_length);
    o_tex_coords.SetLength(array_length);
    o_normals.SetLength(array_length);
    o_skinning_indices.SetLength(array_length);
    o_skinning_weights.SetLength(array_length);
    o_draw_element_index.SetLength(array_length);
    int unique_vertex_count;
    ConvertObjToIndexedElementData(
        &unique_vertex_count,
        o_vertices.data(),
        o_tex_coords.data(),
        o_normals.data(),
        o_skinning_indices.data(),
        o_skinning_weights.data(),
        o_draw_element_index.data(),
        array_length,
        in_obj);
    o_vertices.SetLength(unique_vertex_count);
    o_tex_coords.SetLength(unique_vertex_count);
    o_normals.SetLength(unique_vertex_count);
	if (in_obj.num_skinning_weights == 0)
	{
		o_skinning_indices.SetLength(0);
		o_skinning_weights.SetLength(0);
	}
	else
	{
		o_skinning_indices.SetLength(unique_vertex_count);
		o_skinning_weights.SetLength(unique_vertex_count);
	}
}


//
//  Obj�t�@�C���̓ǂݍ���
//
bool wsp::graphic::LoadObj(wsp::graphic::Obj* o_obj, const char * filename)
{
    FILE *  fp;
    char    line[BUFFER_LENGTH];
    char    name[BUFFER_LENGTH];
    const int TOKEN_BUFFER_SIZE = MAXLEN_TOKEN * 10;
    char line_tokens[TOKEN_BUFFER_SIZE];
    char index_tokens[TOKEN_BUFFER_SIZE];
    int  i, j;
    WspVec3F  vec;
    wsp::graphic::Mtl curr_mtl;

    // �t�@�C�����J��
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        return false;
    }

    // Obj�\���̂��������i�ЂƂ܂��Œ�T�C�Y�̔z������蓖�Ă�j
    o_obj->num_vertices = 0;
    o_obj->num_normals = 0;
    o_obj->num_tex_coords = 0;
    o_obj->num_triangles = 0;
    o_obj->vertex_positions.resize(MAX_VECTOR_SIZE);
    o_obj->normals.resize(MAX_VECTOR_SIZE);
    o_obj->tex_coords.resize(MAX_VECTOR_SIZE);
    o_obj->triangle_vertex_position_indices.resize(MAX_TRIANGLE_SIZE * 3);
    o_obj->triangle_normal_indices.resize(MAX_TRIANGLE_SIZE * 3);
    o_obj->triangle_tex_coord_indices.resize(MAX_TRIANGLE_SIZE * 3);


    // �t�@�C������P�s���ǂݍ���
    while (fgets(line, BUFFER_LENGTH, fp) != NULL)
    {
        if (strncmp(line, "mtllib", 6) == 0)
        {
            // �}�e���A���̓ǂݍ���
            // �e�L�X�g�����
            sscanf(line, "mtllib %1023s", name);

            // �w�肳�ꂽ�t�@�C�����̃}�e���A���f�[�^��ǂݍ���
            if (strlen(name) > 0)
            {
                //wsp::graphic::LoadMtl(name, o_obj);
            }
        }
        else if (strncmp(line, "usemtl", 6) == 0)
        {
            // �}�e���A���̕ύX
            // �e�L�X�g�����
            sscanf(line, "usemtl %1023s", name);
        }
        else if (line[0] == 'v')
        {
            // ���_�f�[�^�̓ǂݍ���
            // �@���x�N�g���ivn�j
            if (line[1] == 'n')
            {
                // �e�L�X�g�����
                sscanf(line, "vn %f %f %f", &vec.x, &vec.y, &vec.z);

                // �@���x�N�g���z��̖����Ɋi�[
                if (o_obj->num_normals < MAX_VECTOR_SIZE)
                {
                    o_obj->normals[o_obj->num_normals] = vec;
                    o_obj->num_normals++;
                }
            }
            // �e�N�X�`�����W�ivt�j
            else if (line[1] == 't')
            {
                // �e�L�X�g�����
                sscanf(line, "vt %f %f %f", &vec.x, &vec.y, &vec.z);

                // �e�N�X�`�����W�z��̖����Ɋi�[
                if (o_obj->num_tex_coords < MAX_VECTOR_SIZE)
                {
                    o_obj->tex_coords[o_obj->num_tex_coords].x = vec.x;
                    o_obj->tex_coords[o_obj->num_tex_coords].y = vec.y;
                    o_obj->num_tex_coords++;
                }
            }
            else
            {
                // ���_���W�iv�j
                // �e�L�X�g�����
                sscanf(line, "v %f %f %f", &vec.x, &vec.y, &vec.z);

                // �@���x�N�g���z��̖����Ɋi�[
                if (o_obj->num_vertices < MAX_VECTOR_SIZE)
                {
                    o_obj->vertex_positions[o_obj->num_vertices] = vec;
                    o_obj->num_vertices++;
                }
            }
        }
        else if (line[0] == 'f')
        {
            // �|���S���f�[�^�̓ǂݍ���
            int num_tokens = WSP_Tokenize(line_tokens, line, " ", 5);
            WSP_ASSERT(num_tokens != -1, "not triangle polygon");

            {
                i = o_obj->num_triangles * 3;
                bool has_triangle = false;
                for (j = 0; j<3; j++)
                {
                    const char* line_token = WSP_GetToken(line_tokens, j + 1);
                    num_tokens = WSP_Tokenize(index_tokens, line_token, "/", 4);
                    {
                        const char* index_token = WSP_GetToken(index_tokens, 0);
                        if (index_token[0] != '\0')
                        {
                            int index;
                            sscanf(index_token, "%i", &index);
                            o_obj->triangle_vertex_position_indices[i + j] = index - 1;  // Obj�`���ł̓C���f�b�N�X�ԍ��͂P����n�܂�̂ŁA�|�P���ĂO����n�܂�悤�ɂ���

                            has_triangle = true;
                        }
                    }

                    {
                        const char* index_token = WSP_GetToken(index_tokens, 1);
                        if (index_token[0] != '\0')
                        {
                            int index;
                            sscanf(index_token, "%i", &index);
                            o_obj->triangle_tex_coord_indices[i + j] = index - 1;
                        }
                    }

                    {
                        const char* index_token = WSP_GetToken(index_tokens, 2);
                        if (index_token[0] != '\0')
                        {
                            int index;
                            sscanf(index_token, "%i", &index);
                            o_obj->triangle_normal_indices[i + j] = index - 1;
                        }
                    }
                }

                if (has_triangle)
                {
                    o_obj->num_triangles++;
                }
            }

            // �|���S�������m�ۂ����z��̑傫���𒴂����狭���I��
            if (o_obj->num_triangles >= MAX_TRIANGLE_SIZE)
            {
                break;
            }
        }
        else if (line[0] == 's')
        {
            // smooth shading
        }
    };

    // �K�v�Ȕz����m�ۂ��Ȃ���
    o_obj->vertex_positions.resize(o_obj->num_vertices);
    o_obj->normals.resize(o_obj->num_normals);
    o_obj->tex_coords.resize(o_obj->num_tex_coords);
    o_obj->triangle_vertex_position_indices.resize(o_obj->num_triangles * 3);
    o_obj->triangle_tex_coord_indices.resize(o_obj->num_triangles * 3);
    o_obj->triangle_normal_indices.resize(o_obj->num_triangles * 3);

    // �t�@�C�������
    fclose(fp);

    // �ǂݍ��񂾃I�u�W�F�N�g�f�[�^��Ԃ�
    return  true;
}

//
//
////
////  Mtl�t�@�C���̓ǂݍ���
////
//void wsp::graphic::LoadMtl(const char * filename, wsp::graphic::Obj * obj)
//{
//    FILE *  fp;
//    char    line[BUFFER_LENGTH];
//    char    name[BUFFER_LENGTH];
//    WspColor3F   color;
//    wsp::graphic::Mtl*   curr_mtl = NULL;
//
//    // �t�@�C�����J��
//    fp = fopen(filename, "r");
//    if (fp == NULL)
//    {
//        return;
//    }
//
//    // Mtl�z����������i�ЂƂ܂��Œ�T�C�Y�̔z������蓖�Ă�j
//    obj->num_materials = 0;
//    obj->materials.resize(MAX_MTL_SIZE);
//
//    // �t�@�C������P�s���ǂݍ���
//    while (fgets(line, BUFFER_LENGTH, fp) != NULL)
//    {
//        // �}�e���A���f�[�^�̒ǉ�
//        if (strncmp(line, "new_mtl", 6) == 0)
//        {
//            // �e�L�X�g�����
//            sscanf(line, "new_mtl %s", name);
//            if (strlen(name) == 0)
//                continue;
//
//            // �}�e���A���f�[�^�̍쐬
//            curr_mtl = &obj->materials[obj->num_materials];
//            curr_mtl->name = name;
//            curr_mtl->kd.r = 0.8f;
//            curr_mtl->kd.g = 0.8f;
//            curr_mtl->kd.b = 0.8f;
//
//            // �}�e���A���f�[�^��z��ɋL�^
//            obj->num_materials++;
//        }
//
//        // ���˓����f�[�^�̓ǂݍ���
//        if (line[0] == 'K')
//        {
//            // �g�U���˓����iKd�j
//            if (line[1] == 'd')
//            {
//                // �e�L�X�g�����
//                sscanf(line, "Kd %f %f %f", &color.r, &color.g, &color.b);
//
//                // �g�U���˓����iKd�j���L�^
//                if (curr_mtl)
//                {
//                    curr_mtl->kd = color;
//                }
//            }
//        }
//
//    }
//
//    // �t�@�C�������
//    fclose(fp);
//}


#endif
