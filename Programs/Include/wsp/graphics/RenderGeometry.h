// @file RenderGeometry.h
// 

#ifndef __WSP_GRAPHIC_RENDER_GEOMETRY_H__
#define __WSP_GRAPHIC_RENDER_GEOMETRY_H__

#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#include <wsp/common/tpl_cl-vector.h>
#endif

#include "Material.h"

namespace wsp { namespace graphic {
    // glDrawElements •`‰æ—p
    class WSP_DLL_EXPORT RenderGeometry
    {
    public:
        RenderGeometry();
        RenderGeometry(const RenderGeometry& rhs);

        RenderGeometry(
            const wsp::Vector<WspVec3F>& in_positions,
            const wsp::Vector<WspVec2F>& in_uvs);

        RenderGeometry(
            const wsp::Vector<WspVec3F>& in_positions,
            const wsp::Vector<WspVec2F>& in_uvs,
            const wsp::Vector<WspVec3F>& in_normals);

        RenderGeometry(
            const wsp::Vector<WspVec3F>& in_positions,
            const wsp::Vector<WspVec2F>& in_uvs,
            const wsp::Vector<WspVec3F>& in_normals,
            const wsp::Vector<int>& in_indices);

        ~RenderGeometry();

        void Clear();

        wsp::Vector<WspVec3F>& vertex_positions()
        {
            return *vertex_positions_;
        }

        const wsp::Vector<WspVec3F>& vertex_positions() const
        {
            return *vertex_positions_;
        }

        wsp::Vector<WspVec3F>& vertex_normals()
        {
            return *vertex_normals_;
        }

        const wsp::Vector<WspVec3F>& vertex_normals() const
        {
            return *vertex_normals_;
        }

        wsp::Vector<WspVec2F>& vertex_texture_coords()
        {
            return *vertex_texture_coords_;
        }

        const wsp::Vector<WspVec2F>& vertex_texture_coords() const
        {
            return *vertex_texture_coords_;
        }

        wsp::Vector<int>& draw_element_indices()
        {
            return *draw_element_indices_;
        }

        const wsp::Vector<int>& draw_element_indices() const
        {
            return *draw_element_indices_;
        }

        const wsp::Vector<WspIntVec4>& vertex_skinning_bone_indices() const
        {
            return *vertex_skinning_bone_indices_;
        }

        wsp::Vector<WspIntVec4>& vertex_skinning_bone_indices()
        {
            return *vertex_skinning_bone_indices_;
        }

        const wsp::Vector<WspVec4F>& vertex_skinning_weights() const
        {
            return *vertex_skinning_weights_;
        }

        wsp::Vector<WspVec4F>& vertex_skinning_weights()
        {
            return *vertex_skinning_weights_;
        }

        wsp::Vector<WspVec3F>& skinned_vertex_positions()
        {
            return *skinned_vertex_positions_;
        }

        const wsp::Vector<WspVec3F>& skinned_vertex_positions() const
        {
            return *skinned_vertex_positions_;
        }

        wsp::Vector<WspVec3F>& skinned_vertex_normals()
        {
            return *skinned_vertex_normals_;
        }

        const wsp::Vector<WspVec3F>& skinned_vertex_normals() const
        {
            return *skinned_vertex_normals_;
        }

        bool IsSkinned() const
        {
            return vertex_skinning_bone_indices_->length() > 0;
        }

        int parent_bone_index() const
        {
            return parent_bone_index_;
        }

        void set_parent_bone_index(int index)
        {
            parent_bone_index_ = index;
        }

        Material& material()
        {
            return material_;
        }

        const Material& material() const
        {
            return material_;
        }

        void set_material(const Material& material)
        {
            material_ = material;
        }

    private:
        wsp::Vector<WspVec3F>* vertex_positions_;
        wsp::Vector<WspVec3F>* vertex_normals_;
        wsp::Vector<WspVec2F>* vertex_texture_coords_;
        wsp::Vector<int>* draw_element_indices_;
        wsp::Vector<WspIntVec4>* vertex_skinning_bone_indices_;
        wsp::Vector<WspVec4F>* vertex_skinning_weights_;
        wsp::Vector<WspVec3F>* skinned_vertex_positions_;
        wsp::Vector<WspVec3F>* skinned_vertex_normals_;
        int parent_bone_index_;
        Material material_;
    };
}}

#endif
