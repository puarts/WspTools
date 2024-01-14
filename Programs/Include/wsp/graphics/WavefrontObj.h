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

        // マテリアルデータ（Mtl形式用）
        struct Mtl
        {
            WspColor3F kd;            // 拡散反射光（とりあえず拡散反射光を glColor3f() で使用する）
            char name[MaxMtlNameLength];          // マテリアル名
            char texture_name[MaxMtlTexNameLength];  // テクスチャ画像のファイル名

            Mtl()
            {
                strcpy(name, "");
                strcpy(texture_name, "");
            }
        };

        // 幾何形状データ（Obj形式用）
        struct Obj
        {
            int num_vertices;  // 頂点数
            std::vector<WspVec3F> vertex_positions;      // 頂点座標配列 [num_vertices]

            int num_normals;
            std::vector<WspVec3F> normals;       // 法線ベクトル配列 [num_normals]

            int num_tex_coords;
            std::vector<WspVec2F> tex_coords;    // テクスチャ座標配列 [num_tex_coords]

            int num_skinning_weights;
            std::vector<WspVec4F> skinning_weights;
            std::vector<WspIntVec4> skinning_indices;

            int num_triangles; // 三角面数
            std::vector<int> triangle_vertex_position_indices;      // 三角面の各頂点の頂点座標番号配列 [num_triangles*3]
            std::vector<int> triangle_normal_indices;     // 三角面の各頂点の法線ベクトル番号配列 [num_triangles*3]
            std::vector<int> triangle_tex_coord_indices;     // 三角面の各頂点のテクスチャ座標番号配列 [num_triangles*3]
            std::vector<int> triangle_skinning_index_indices;
            std::vector<int> triangle_skinning_weight_indices;

            int parent_bone_index;

            wsp::graphic::Mtl material; // マテリアル
        };
    }
}

#endif
