/**
 * @file _tpl_fn-graphic_proc.hpp
 * 
 */

#ifndef WSP_GRAPHIC_BASE_TEMPLATE_FUNCTION_GRAPHIC_PROC_HPP__
#define WSP_GRAPHIC_BASE_TEMPLATE_FUNCTION_GRAPHIC_PROC_HPP__

#include "_define_graphic.h"

namespace wsp{ namespace graphic{
    /**
     * Get vertex connection list from edges within a face
     * @param in_edge_vtx_ids array for edge index list within a face
     * @param o_vtx_connections array for storing vertex connectins, 
     *   enough memory for storing vertex data must be allocated before calling this function 
     * @param num_face_edge number of edges within a face, the size of array is ([number of verteces] * 2)
     */
    template<typename T>
    wsp::State GetVtxConnectionsFromEdgeVertexIndices(
        const T *in_edge_vtx_ids
        , T *o_vtx_connections
        , int num_face_edge
    );

}}

#include "_tpl_fn-graphic_proc.inl"

#endif
