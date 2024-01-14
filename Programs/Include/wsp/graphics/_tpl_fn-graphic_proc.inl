/**
 * @file _tpl_fn-graphic_proc.inl
 * 
 */

#ifndef WSP_GRAPHIC_BASE_TEMPLATE_FUNCTION_GRAPHIC_PROC_INL__
#define WSP_GRAPHIC_BASE_TEMPLATE_FUNCTION_GRAPHIC_PROC_INL__

#include "_tpl_fn-graphic_proc.hpp"

template<typename T>
wsp::State wsp::graphic::GetVtxConnectionsFromEdgeVertexIndices(
    const T *in_edge_vtx_ids
    , T *o_vtx_connections
    , int num_face_edge
)
{
    /* 
     * Maximum number of edge. 
     * you can increase kMaxNumEdge if you want to handle more edges. 
     */
    const int kMaxNumEdge = 30; 
    if(num_face_edge > kMaxNumEdge){
        WSP_COMMON_ERROR_LOG("Too many number of edges, must be less than %d\n", kMaxNumEdge);
        return WSP_STATE_FAILURE;
    }
    /* ------------------------------------------
         Unoptimized code
       ------------------------------------------*/
    int check_list[kMaxNumEdge];
    memset(check_list, 0, sizeof(int)*kMaxNumEdge);
    T current_vtx_id = in_edge_vtx_ids[1];
    T end_vtx_id = in_edge_vtx_ids[0];
    int connection_cnt=0;
    o_vtx_connections[connection_cnt] = in_edge_vtx_ids[0];
    ++connection_cnt;
    check_list[0] = 1;

    /* 
     * Find vertex connections 
     * until connected vtx reaches to end of edge loop around face.
     */
    while(current_vtx_id != end_vtx_id)
    {
        int i;
        int vtx_id_0, vtx_id_1;
        for(i=0; i<num_face_edge; ++i){
            if(check_list[i]==1){
                continue;
            }
            vtx_id_0 = in_edge_vtx_ids[i*2];
            vtx_id_1 = in_edge_vtx_ids[i*2+1];
            if(vtx_id_0==current_vtx_id){
                o_vtx_connections[connection_cnt] = vtx_id_0;
                ++connection_cnt;
                current_vtx_id = vtx_id_1;

                check_list[i] = 1;
                break;
            }
            if(vtx_id_1==current_vtx_id){
                o_vtx_connections[connection_cnt] = vtx_id_1;
                ++connection_cnt;
                current_vtx_id = vtx_id_0;

                check_list[i] = 1;
                break;
            }
        }
        if(i==num_face_edge && current_vtx_id != end_vtx_id){
            WSP_COMMON_ERROR_LOG("Given edge vertex list does not have looped edge\n");
            return WSP_STATE_FAILURE;
        }
    }
    return WSP_STATE_SUCCESS;
}

#endif
