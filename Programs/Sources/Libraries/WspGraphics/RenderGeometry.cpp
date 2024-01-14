
#include <wsp/graphics/RenderGeometry.h>

void wsp::graphic::RenderGeometry::Clear()
{
    vertex_positions_->Clear();
    vertex_normals_->Clear();
    vertex_texture_coords_->Clear();

    skinned_vertex_positions_->Clear();
    skinned_vertex_normals_->Clear();

    vertex_skinning_bone_indices_->Clear();
    vertex_skinning_weights_->Clear();

    draw_element_indices_->Clear();
}

wsp::graphic::RenderGeometry::RenderGeometry()
    : vertex_positions_(WSP_NEW wsp::Vector<WspVec3F>())
    , vertex_normals_(WSP_NEW wsp::Vector<WspVec3F>())
    , vertex_texture_coords_(WSP_NEW wsp::Vector<WspVec2F>())
    , draw_element_indices_(WSP_NEW wsp::Vector<int>())
    , vertex_skinning_bone_indices_(WSP_NEW wsp::Vector<WspIntVec4>())
    , vertex_skinning_weights_(WSP_NEW wsp::Vector<WspVec4F>())
    , skinned_vertex_positions_(WSP_NEW wsp::Vector<WspVec3F>())
    , skinned_vertex_normals_(WSP_NEW wsp::Vector<WspVec3F>())
    , parent_bone_index_(-1)
{
}

wsp::graphic::RenderGeometry::RenderGeometry(const RenderGeometry& rhs)
    : RenderGeometry()
{
    vertex_positions_->CopyFrom(rhs.vertex_positions().data(), rhs.vertex_positions().length(), rhs.vertex_positions().num_of_channels());
    vertex_normals_->CopyFrom(rhs.vertex_normals().data(), rhs.vertex_normals().length(), rhs.vertex_normals().num_of_channels());
    vertex_texture_coords_->CopyFrom(rhs.vertex_texture_coords().data(), rhs.vertex_texture_coords().length(), rhs.vertex_texture_coords().num_of_channels());

    vertex_skinning_bone_indices_->CopyFrom(rhs.vertex_skinning_bone_indices().data(), rhs.vertex_skinning_bone_indices().length(), rhs.vertex_skinning_bone_indices().num_of_channels());
    vertex_skinning_weights_->CopyFrom(rhs.vertex_skinning_weights().data(), rhs.vertex_skinning_weights().length(), rhs.vertex_skinning_weights().num_of_channels());

    skinned_vertex_positions_->CopyFrom(rhs.skinned_vertex_positions_->data(), rhs.skinned_vertex_positions_->length(), rhs.skinned_vertex_positions_->num_of_channels());
    skinned_vertex_normals_->CopyFrom(rhs.skinned_vertex_normals_->data(), rhs.skinned_vertex_normals_->length(), rhs.skinned_vertex_normals_->num_of_channels());

    draw_element_indices_->CopyFrom(rhs.draw_element_indices().data(), rhs.draw_element_indices().length(), rhs.draw_element_indices().num_of_channels());
}

wsp::graphic::RenderGeometry::RenderGeometry(
    const wsp::Vector<WspVec3F>& in_positions,
    const wsp::Vector<WspVec2F>& in_uvs)
    : RenderGeometry()
{
    vertex_positions_->SetLength(in_positions.length());
    memcpy(vertex_positions_->data(), in_positions.data(), sizeof(float) * 3 * in_positions.length());

    skinned_vertex_positions_->SetLength(in_positions.length());
    memcpy(skinned_vertex_positions_->data(), in_positions.data(), sizeof(float) * 3 * in_positions.length());

    vertex_texture_coords_->SetLength(in_uvs.length());
    memcpy(vertex_texture_coords_->data(), in_uvs.data(), sizeof(float) * 2 * in_uvs.length());
}

wsp::graphic::RenderGeometry::RenderGeometry(
    const wsp::Vector<WspVec3F>& in_positions,
    const wsp::Vector<WspVec2F>& in_uvs,
    const wsp::Vector<WspVec3F>& in_normals)
    : RenderGeometry(in_positions, in_uvs)
{
    vertex_normals_->SetLength(in_normals.length());
    memcpy(vertex_normals_->data(), in_normals.data(), sizeof(float) * 3 * in_normals.length());

    skinned_vertex_normals_->SetLength(in_normals.length());
    memcpy(skinned_vertex_normals_->data(), in_normals.data(), sizeof(float) * 3 * in_normals.length());
}

wsp::graphic::RenderGeometry::RenderGeometry(
    const wsp::Vector<WspVec3F>& in_positions,
    const wsp::Vector<WspVec2F>& in_uvs,
    const wsp::Vector<WspVec3F>& in_normals,
    const wsp::Vector<int>& in_indices)
    : RenderGeometry(in_positions, in_uvs, in_normals)
{
    draw_element_indices_->SetLength(in_indices.length());
    memcpy(draw_element_indices_->data(), in_indices.data(), sizeof(int) * in_indices.length());
}

wsp::graphic::RenderGeometry::~RenderGeometry()
{
    delete vertex_positions_;
    delete vertex_normals_;
    delete vertex_texture_coords_;
    delete draw_element_indices_;
    delete vertex_skinning_bone_indices_;
    delete vertex_skinning_weights_;
    delete skinned_vertex_positions_;
    delete skinned_vertex_normals_;
}
