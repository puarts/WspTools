// @file fn-graphic_io.cpp
// 



// 基本的なヘッダファイルのインクルード


// 幾何形状データの定義のインクルード
#include "fn-graphic_io.h"
#include "fn-graphic_log.h"
#include "fn-graphic_proc.h"

#include <wsp/common/fn-debug.h>
#include <wsp/common/fn-util.h>
#include <wsp/common/tpl_cl-vector4.h>
#include <wsp/graphics/RenderSkeleton.h>

void wsp::graphic::ConvertObjToIndexedElementData(
    int* o_unique_vertex_count,
    WspVec3F* o_vertices,
    WspVec2F* o_tex_coords,
    WspVec3F* o_normals,
    int* o_draw_element_index,
    int array_length,
    const wsp::graphic::Obj& in_obj)
{
    ConvertObjToIndexedElementData(
        o_unique_vertex_count,
        o_vertices,
        o_tex_coords,
        o_normals,
        nullptr, nullptr,
        o_draw_element_index,
        array_length,
        in_obj);
}

void wsp::graphic::ConvertObjToIndexedElementData(
    int* o_unique_vertex_count,
    WspVec3F* o_vertices,
    WspVec2F* o_tex_coords,
    WspVec3F* o_normals,
    WspIntVec4* o_skinning_bone_indices,
    WspVec4F* o_skinning_weights,
    int* o_draw_element_index,
    int array_length,
    const wsp::graphic::Obj& in_obj)
{
    WSP_REQUIRES(in_obj.triangle_vertex_position_indices.size() == array_length, "polygons must be traiangulated.");
    int current_element_array_index = 0;
    bool is_skinning_data_available = o_skinning_bone_indices != nullptr && in_obj.skinning_indices.size() > 0;
    for (int index_array_index = 0; index_array_index < array_length; ++index_array_index)
    {
        int pos_index = in_obj.triangle_vertex_position_indices[index_array_index];
        int tex_coord_index = in_obj.triangle_tex_coord_indices[index_array_index];
        int normal_index = in_obj.triangle_normal_indices[index_array_index];
        const WspVec3F* pos = &in_obj.vertex_positions[pos_index];
        const WspVec2F* tex_coord = &in_obj.tex_coords[tex_coord_index];
        const WspVec3F* normal = &in_obj.normals[normal_index];

        const WspIntVec4* skinning_index = nullptr;
        const WspVec4F* skinning_weight = nullptr;
        if (is_skinning_data_available)
        {
            int skinning_index_index = in_obj.triangle_skinning_index_indices[index_array_index];
            int skinning_weight_index = in_obj.triangle_skinning_weight_indices[index_array_index];
            skinning_index = &in_obj.skinning_indices[skinning_index_index];
            skinning_weight = &in_obj.skinning_weights[skinning_index_index];
        }

        bool is_the_same_element_found = false;
        for (int elem_index = 0; elem_index < current_element_array_index; ++elem_index)
        {
            bool is_skinning_data_same = true;
            if (is_skinning_data_available)
            {
                is_skinning_data_same =
                    (*skinning_index) == o_skinning_bone_indices[elem_index] &&
                    (*skinning_weight) == o_skinning_weights[elem_index];
            }

            if ((*pos) == o_vertices[elem_index] &&
                (*tex_coord) == o_tex_coords[elem_index] &&
                (*normal) == o_normals[elem_index] &&
                is_skinning_data_same)
            {
                o_draw_element_index[index_array_index] = elem_index;
                is_the_same_element_found = true;
                break;
            }
        }

        if (!is_the_same_element_found)
        {
            o_vertices[current_element_array_index] = *pos;
            o_tex_coords[current_element_array_index] = *tex_coord;
            o_normals[current_element_array_index] = *normal;
            if (is_skinning_data_available)
            {
                o_skinning_bone_indices[current_element_array_index] = *skinning_index;
                o_skinning_weights[current_element_array_index] = *skinning_weight;
            }

            o_draw_element_index[index_array_index] = current_element_array_index;

            ++current_element_array_index;
        }
    }

    *o_unique_vertex_count = current_element_array_index;
}

void wsp::graphic::ConvertObjToIndexedElementData(
    std::vector<WspVec3F>& o_vertices,
    std::vector<WspVec2F>& o_tex_coords,
    std::vector<WspVec3F>& o_normals,
    std::vector<WspIntVec4>& o_skinning_indices,
    std::vector<WspVec4F>& o_skinning_weights,
    std::vector<int>& o_draw_element_index,
    const Obj& in_obj)
{
    int array_length = in_obj.triangle_normal_indices.size();
    o_vertices.resize(array_length);
    o_tex_coords.resize(array_length);
    o_normals.resize(array_length);
    o_skinning_indices.resize(array_length);
    o_skinning_weights.resize(array_length);
    o_draw_element_index.resize(array_length);

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

    o_vertices.resize(unique_vertex_count);
    o_tex_coords.resize(unique_vertex_count);
    o_normals.resize(unique_vertex_count);
    o_skinning_indices.resize(unique_vertex_count);
    o_skinning_weights.resize(unique_vertex_count);
}

void wsp::graphic::ConvertToSmoothSkinnedPositionsAndNormals(
    wsp::graphic::RenderGeometry* io_geom,
    const wsp::graphic::RenderSkeleton& in_skeleton)
{
    ConvertToSmoothSkinnedPositionsAndNormals(
        &io_geom->skinned_vertex_positions(),
        &io_geom->skinned_vertex_normals(),
        io_geom->vertex_positions(),
        io_geom->vertex_normals(),
        io_geom->vertex_skinning_bone_indices(),
        io_geom->vertex_skinning_weights(),
        in_skeleton);
}

void wsp::graphic::ConvertToSmoothSkinnedPositions(
    float* o_skinned_positions, int skinned_pos_component_count,
    const float* in_positions, int in_pos_component_count,
    const int* in_skinning_indices, const float* in_skinning_weights, int skinning_weight_component_count,
    int vertex_count,
    const wsp::graphic::RenderSkeleton& in_skeleton)
{
    WSP_REQUIRES(in_pos_component_count == 3 || in_pos_component_count == 4, "invalid component count");
    WSP_REQUIRES(skinned_pos_component_count == 3 || skinned_pos_component_count == 4, "invalid component count");
    WSP_REQUIRES(in_skeleton.GetBoneCount() > 0, "invalid bone count %d", in_skeleton.GetBoneCount());

    const float* weight_ptr = in_skinning_weights;
    const int* bone_index_ptr = in_skinning_indices;

    const float* pos_ptr = in_positions;
    const float* pos_end = pos_ptr + vertex_count * in_pos_component_count;
    float* skinned_pos_ptr = o_skinned_positions;
    for (; pos_ptr < pos_end;
        pos_ptr += in_pos_component_count,
        bone_index_ptr += skinning_weight_component_count,
        weight_ptr += skinning_weight_component_count,
        skinned_pos_ptr += skinned_pos_component_count)
    {
        TransformMatrixF transform_mat(
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f
        );

        for (int bone_index = 0; bone_index < 4; ++bone_index)
        {
            int skinned_bone_index = bone_index_ptr[bone_index];
            if (skinned_bone_index < 0)
            {
                // 使われてない
                continue;
            }

            float weight = weight_ptr[bone_index];
            const wsp::graphic::RenderBone* bone = in_skeleton.GetBone(skinned_bone_index);
            TransformMatrixF weighted_mat = in_skeleton.CalculateBoneTransformMatrix(bone) * weight;
            transform_mat += weighted_mat;
        }

        TransformMatrixF inv_mat;
        bool success = transform_mat.GetInverseMatrix(&inv_mat);
        WSP_FATAL_IF_FALSE(success, "could not calculate inverse matrix");

        wsp::Vector4F vertex_pos(*pos_ptr, *(pos_ptr + 1), *(pos_ptr + 2), 1.0f);
        wsp::Vector4F transformed_pos = wsp::Vector4F(vertex_pos * inv_mat);

        //for (int bone_index = 0; bone_index < 4; ++bone_index)
        //{
        //    int skinned_bone_index = bone_index_ptr[bone_index];
        //    if (skinned_bone_index < 0)
        //    {
        //        // 使われてない
        //        continue;
        //    }

        //    const wsp::graphic::RenderBone* bone = in_skeleton.GetBone(skinned_bone_index);
        //    const wsp::graphic::TransformMatrixF& bone_mat = bone->bind_pose_transform_matrix();
        //    float weight = weight_ptr[bone_index];

        //    wsp::graphic::TransformMatrixF inv_bone_mat;
        //    bool success_obtain_bone_mat = bone_mat.GetInverseMatrix(&inv_bone_mat);
        //    WSP_ASSERT(success_obtain_bone_mat, "failed to obtaine bone matrix.");
        //    transformed_pos += (vertex_pos * inv_bone_mat) * weight;
        //}

        *(skinned_pos_ptr + 0) = transformed_pos.x() / transformed_pos.w();
        *(skinned_pos_ptr + 1) = transformed_pos.y() / transformed_pos.w();
        *(skinned_pos_ptr + 2) = transformed_pos.z() / transformed_pos.w();
        //*(skinned_pos_ptr + 0) = transformed_pos.x();
        //*(skinned_pos_ptr + 1) = transformed_pos.y();
        //*(skinned_pos_ptr + 2) = transformed_pos.z();
    }
}

void wsp::graphic::ConvertToSmoothSkinnedPositions(
    wsp::Vector<WspVec3F>* o_skinned_positions,
    const wsp::Vector<WspVec3F>& in_positions,
    const wsp::Vector<WspIntVec4>& in_skinning_indices,
    const wsp::Vector<WspVec4F>& in_skinning_weights,
    const wsp::graphic::RenderSkeleton& in_skeleton)
{
    const float* weight_ptr = reinterpret_cast<const float*>(in_skinning_weights.data());
    const int* bone_index_ptr = reinterpret_cast<const int*>(in_skinning_indices.data());

    o_skinned_positions->SetLength(in_positions.length());
    const float* pos_ptr = reinterpret_cast<const float*>(in_positions.data());
    float* skinned_pos_ptr = reinterpret_cast<float*>(o_skinned_positions->data());
    ConvertToSmoothSkinnedPositions(
        skinned_pos_ptr, 3,
        pos_ptr, 3,
        bone_index_ptr, weight_ptr, 4,
        in_positions.length(),
        in_skeleton);
}


void wsp::graphic::ConvertToSmoothSkinnedNormals(
    wsp::Vector<WspVec3F>* o_skinned_normals,
    const wsp::Vector<WspVec3F>& in_normals,
    const wsp::Vector<WspIntVec4>& in_skinning_indices,
    const wsp::Vector<WspVec4F>& in_skinning_weights,
    const wsp::graphic::RenderSkeleton& in_skeleton)
{
    const float* weight_ptr = reinterpret_cast<const float*>(in_skinning_weights.data());
    const int* bone_index_ptr = reinterpret_cast<const int*>(in_skinning_indices.data());

    o_skinned_normals->SetLength(in_normals.length());
    const float* normal_ptr = reinterpret_cast<const float*>(in_normals.data());
    const float* normal_end = normal_ptr + in_normals.length() * 3;
    float* skinned_normal_ptr = reinterpret_cast<float*>(o_skinned_normals->data());
    for (; normal_ptr < normal_end;
        normal_ptr += 3,
        bone_index_ptr += 4,
        weight_ptr += 4,
        skinned_normal_ptr += 3)
    {
        wsp::Vector4F vertex_normal(*normal_ptr, *(normal_ptr + 1), *(normal_ptr + 2), 0.0f);
        wsp::Vector4F transformed_normal(0.0f, 0.0f, 0.0f, 0.0f);
        for (int bone_index = 0; bone_index < 4; ++bone_index)
        {
            int skinned_bone_index = bone_index_ptr[bone_index];
            if (skinned_bone_index < 0)
            {
                // 使われてない
                continue;
            }

            const wsp::graphic::RenderBone* bone = in_skeleton.GetBone(skinned_bone_index);
            const wsp::graphic::TransformMatrixF& inverse_transposed_bone_mat = bone->bind_pose_normal_transform_matrix();

            float weight = weight_ptr[bone_index];

            {
                wsp::graphic::TransformMatrixF inv_bone_mat;
                bool success_obtain_bone_mat = inverse_transposed_bone_mat.GetInverseMatrix(&inv_bone_mat);
                WSP_ASSERT(success_obtain_bone_mat, "failed to obtaine bone matrix.");
                transformed_normal += (vertex_normal * inv_bone_mat) * weight;
            }
        }

        wsp::math::NormalizeLengthVec3(transformed_normal.data());
        *(skinned_normal_ptr + 0) = transformed_normal.x();
        *(skinned_normal_ptr + 1) = transformed_normal.y();
        *(skinned_normal_ptr + 2) = transformed_normal.z();
    }
}

void wsp::graphic::ConvertToSmoothSkinnedPositionsAndNormals(
    wsp::Vector<WspVec3F>* o_skinned_positions,
    wsp::Vector<WspVec3F>* o_skinned_normals,
    const wsp::Vector<WspVec3F>& in_positions,
    const wsp::Vector<WspVec3F>& in_normals,
    const wsp::Vector<WspIntVec4>& in_skinning_indices,
    const wsp::Vector<WspVec4F>& in_skinning_weights,
    const wsp::graphic::RenderSkeleton& in_skeleton)
{
    ConvertToSmoothSkinnedPositions(
        o_skinned_positions,
        in_positions,
        in_skinning_indices,
        in_skinning_weights,
        in_skeleton);

    ConvertToSmoothSkinnedNormals(
        o_skinned_normals,
        in_normals,
        in_skinning_indices,
        in_skinning_weights,
        in_skeleton);
}

//
//  オブジェクトのスケーリング（スケーリング後の中心の高さを返す）
//
float wsp::graphic::ScaleObj(wsp::graphic::Obj * obj, float max_size)
{
    if ( !obj || ( obj->num_vertices == 0 ) )
        return  0.0;

    // サイズ計算
    WspVec3F  min, max;
    min = max = obj->vertex_positions[ 0 ];
    int  i;
    for ( i=0; i<obj->num_vertices; i++ )
    {
        const WspVec3F &  p = obj->vertex_positions[ i ];
        if ( p.x < min.x )  min.x = p.x;
        if ( p.y < min.y )  min.y = p.y;
        if ( p.z < min.z )  min.z = p.z;
        if ( p.x > max.x )  max.x = p.x;
        if ( p.y > max.y )  max.y = p.y;
        if ( p.z > max.z )  max.z = p.z;
    }

    // スケーリング
    float  size, scale;
    size = ( ( max.x - min.x ) > ( max.z - min.z ) ) ? ( max.x - min.x ) : ( max.z - min.z );
    scale = max_size / size;
    for ( i=0; i<obj->num_vertices; i++ )
    {
        obj->vertex_positions[ i ].x *= scale;
        obj->vertex_positions[ i ].y *= scale;
        obj->vertex_positions[ i ].z *= scale;
    }

    // スケーリング後の中心の高さを返す
    float  object_y = min.y * scale;
    return  object_y;
}


//
//  Obj形状データの描画
//
void wsp::graphic::RenderObj(wsp::graphic::Obj * obj)
{
//    #ifdef USE_GL
//    int  i, j, no;
//    Mtl *  curr_mtl = NULL;
//
//    glBegin( GL_TRIANGLES );
//    for ( i=0; i<obj->num_triangles; i++ )
//    {
//        // マテリアルの切り替え
//        if ( ( obj->num_materials > 0 ) && ( obj->tri_material[ i ] != curr_mtl ) )
//        {
//            curr_mtl = obj->tri_material[ i ];
//            glColor3f( curr_mtl->kd.r, curr_mtl->kd.g, curr_mtl->kd.b );
//        }
//
//        // 三角面の各頂点データの指定
//        for ( j=0; j<3; j++ )
//        {
//            // 法線ベクトル
//            no = obj->tri_vn_no[ i*3 + j ];
//            const WspVec3F &  vn = obj->normals[ no ];
//            glNormal3f( vn.x, vn.y, vn.z );
//
//            // 頂点座標
//            no = obj->tri_v_no[ i*3 + j ];
//            const WspVec3F &  v = obj->vertices[ no ];
//            glVertex3f( v.x, v.y, v.z );
//        }
//    }
//    glEnd();
//    #endif
}

void wsp::graphic::LoadObjSequence(wsp::graphic::Obj** o_objs, int* o_loaded_count, int out_array_length, const wsp::SeqInfo& sequence_info)
{
    char path[MAX_LENGTH_OF_PATH];
    int index = 0;
    for (int frame = sequence_info.start_frame; frame <= sequence_info.end_frame; ++frame, ++index)
    {
        if (index >= out_array_length)
        {
            break;
        }

        wsp::GetSeqPath(path, sequence_info, frame);
        wsp::graphic::Obj* obj = WSP_NEW wsp::graphic::Obj();
        WSP_GRAPHIC_LOG("Loading %s\r", path);
        bool success = wsp::graphic::LoadObj(obj, path);
        WSP_ASSERT(success, "loading file failed: %s", path);
        o_objs[index] = obj;
    }

    WSP_GRAPHIC_LOG("\n");

    *o_loaded_count = index;
}



void wsp::graphic::FreeObjSequence(wsp::graphic::Obj** io_objs, int length)
{
    for (int i = 0; i < length; ++i)
    {
        delete io_objs[i];
    }
}


