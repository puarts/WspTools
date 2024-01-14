#pragma once

#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#include <wsp/common/tpl_cl-vector.h>
#endif
#include <wsp/common/tpl_cl-vector2.h>
#include <wsp/common/tpl_cl-vector3.h>
#include <wsp/image/tpl_cl-image.h>
#include <wsp/graphics/tpl_cl-transform_matrix.h>

struct WSP_DepthToMeshParam;

namespace wsp{
    namespace graphic{
        typedef struct WSP_DepthToMeshParam DepthToMeshParam;

        struct Vertex{
            wsp::Point<float> position;
            wsp::Vector3<float> normal;
            int index;
        };

        class WSP_DLL_EXPORT PolygonModel
        {
        public:
            PolygonModel();
            PolygonModel(wsp::graphic::TransformMatrix<float> *view_mtx);
            PolygonModel(char const *filepath_obj);

            // accessors --------
            inline wsp::Vector<wsp::Point<float>>* points();
            inline wsp::Vector<wsp::Vector3<float>>* normals();
            inline wsp::Vector<float>* uvs()
            {
                return &uvs_;
            }
            inline wsp::Vector<int>* face_point_indices()
            {
                return &face_point_indices_;
            }

            inline int GetNumFaces() const;
            inline int GetNumEdges() const;
            inline int GetNumPoints() const;
            inline bool use_self_transform();
            float translate_x() const
            {
                return translate_[0];
            }

            float translate_y() const
            {
                return translate_[1];
            }

            float translate_z() const
            {
                return translate_[2];
            }

            float scale_x() const
            {
                return scale_[0];
            }

            float scale_y() const
            {
                return scale_[1];
            }

            float scale_z() const
            {
                return scale_[2];
            }

            // mutators ---------------
            inline void set_use_self_transform(bool use_self_transform);
            inline void set_view_mtx(wsp::graphic::TransformMatrix<float> *view_mtx);
            inline void set_display_type(wsp::graphic::PolyDisplayType disp_type);
            inline void set_model_mtx(const wsp::graphic::TransformMatrix<float> &model_mat);
            inline void set_model_mtx(
                const wsp::Vec3F &axis_x, 
                const wsp::Vec3F &axis_y,
                const wsp::Vec3F &axis_z,
                const wsp::Vec3F &translate
            );
            inline void Clear();
            void CreatePrimitive(wsp::graphic::PolyPrimitiveType prim_type);

            template<typename _DepthType>
            wsp::State CreateDepthMesh(
                const wsp::Image<_DepthType> &depth_map,
                float fov=0.0,
                float near_clip_ratio=0.8,
                float scale=10,
                float face_depth_thre=255,
                float alpha_thre=0,
                float samplingStep=4,
                bool invert_depth=false, 
                float offset_x=0.0f, float offset_y=0.0f, float offset_z=0.0f, 
                _DepthType depth_min=0.0, _DepthType depth_max=0.0
            );

            void set_translate_x(float x)
            {
                translate_[0] = x;
            }

            void set_translate_y(float y)
            {
                translate_[1] = y;
            }

            void set_translate_z(float z)
            {
                translate_[2] = z;
            }

            void set_scale_x(float x)
            {
                scale_[0] = x;
            }

            void set_scale_y(float y)
            {
                scale_[1] = y;
            }

            void set_scale_z(float z)
            {
                scale_[2] = z;
            }

            // I/O -----------
            wsp::State LoadObjFile(const char *filepath);
            wsp::State LoadDepthMap(
                const char *filepath, 
                double face_depth_thre=20.0, 
                int samplingStep=1,    
                uchar alpha_thre=0, 
                double scale = 1.0,
                bool invert_depth=false
            );
            inline wsp::State LoadDepthMap(
                const char *filepath, 
                const wsp::graphic::DepthToMeshParam *param
            );

        private:
            //wsp::Vector<wsp::graphic::Vertex> verteces_;
            wsp::Vector<wsp::Point<float>> points_;
            wsp::Vector<wsp::Vector3<float>> normals_;
            wsp::Vector<float> uvs_;
            wsp::Vector<int> vtx_point_indices_;
            wsp::Vector<int> edge_point_indices_;
            wsp::Vector<int> face_point_indices_;
            wsp::graphic::PolyDisplayType display_type_;
            wsp::Vector3<float> translate_, scale_, rotate_;

            /** @brief  model matrix. */
            wsp::graphic::TransformMatrixF model_mtx_;

            /** @brief  using view matrix pointer(external data). */
            wsp::graphic::TransformMatrixF *view_mtx_;

            /** @brief  whether using view model matrix. */
            bool use_self_transform_;
        };
    }
}

#include "cl-polymodel.inl"
