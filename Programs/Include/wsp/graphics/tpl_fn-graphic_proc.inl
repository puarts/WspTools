/**
 * @file tpl_fn-graphic_proc.inl
 * 
 */

#ifndef __WSP_GRAPHIC_TEMPLATE_FUNCTION_GRAPHIC_PROC_INL__
#define __WSP_GRAPHIC_TEMPLATE_FUNCTION_GRAPHIC_PROC_INL__

#include <wsp/common/_tpl_fn-sort.hpp>

#include "fn-graphic_proc.h"
#include "_tpl_fn-graphic_proc.hpp"

template<typename IndexType>
wsp::State wsp::graphic::Reindex(
    const IndexType *in_vtx_indices,
    IndexType *io_vtx_connects,
    int num_vtcs, 
    int num_vtx_connects
)
{
    size_t mem_size_connects = sizeof(IndexType) * num_vtx_connects;

    IndexType *src_vtx_connects = (IndexType*)malloc( mem_size_connects );
    if(src_vtx_connects==NULL){ 
        return WSP_STATE_INSUFFICIENT_MEMORY; 
    }

    memcpy( src_vtx_connects, io_vtx_connects, mem_size_connects );

    const IndexType *src_vid_ptr = in_vtx_indices;
    const IndexType *end_src = src_vid_ptr + num_vtcs;
    for(int new_idx=0; src_vid_ptr!=end_src; ++src_vid_ptr, ++new_idx)
    {
        /* Reindex vertex connects */
        IndexType *dst_connect_ptr = io_vtx_connects;
        IndexType *src_connect_ptr = src_vtx_connects;
        IndexType *end_connect_ptr = src_connect_ptr + num_vtx_connects;
        for( ; src_connect_ptr!=end_connect_ptr; ++src_connect_ptr, ++dst_connect_ptr)
        {
            if(*src_connect_ptr == *src_vid_ptr)
            {
                *dst_connect_ptr = new_idx;
            }
        }
    }

    free(src_vtx_connects);

    return WSP_STATE_SUCCESS;
}

namespace {
    template<typename T>
    inline void AppendEdgePoint(
        const T *in_top_pt_ptr,
        T **io_last_pt_ptr,
        const T in_append_edge_pt[3],
        int **o_edge_vtx_id,
        int *io_num_vtx, 
        int num_channels
    )
    {
        int idx = 0;
        const T *ptr = in_top_pt_ptr;
        const T *end_ptr = (*io_last_pt_ptr);
        for( ; ptr!=end_ptr; ptr+=num_channels, ++idx){
            if( ptr[0]==in_append_edge_pt[0]
             && ptr[1]==in_append_edge_pt[1]
             && ptr[2]==in_append_edge_pt[2]
            ){ 
                break; 
            }
        }

        /* Add vertex id on edge */
        *(*o_edge_vtx_id) = idx;
        ++(*o_edge_vtx_id);

        /* Add vertex */
        if( ptr == end_ptr ){
            wsp::CopyArray( (*io_last_pt_ptr), in_append_edge_pt, 3 ); 
            (*io_last_pt_ptr) += num_channels;
            ++(*io_num_vtx);
        }
    }

    template<typename T>
    inline void AppendExistingPoint(
        const T in_append_pt[3],
        int vid_to_append,
        T **io_pt_ptr,
        int **io_vid_ptr,
        uchar *io_vid_check_list,
        int **o_connected_vid_ptr,
        int *io_num_vtx, 
        int num_channels
    )
    {
        /* Add vertex id on edge */
        *(*o_connected_vid_ptr) = vid_to_append;
        ++(*o_connected_vid_ptr);

        /* Return if current vertex id already exists */
        if( io_vid_check_list[vid_to_append] == 1 ){
            return;
        }

        /* Add new vertex */
        io_vid_check_list[vid_to_append] = 1;
        wsp::CopyArray( (*io_pt_ptr), in_append_pt, 3 );
        (*io_pt_ptr) += num_channels;
        *(*io_vid_ptr) = vid_to_append;
        ++(*io_vid_ptr);
        ++(*io_num_vtx);
    }

    template<typename T>
    inline void AppendNewPoint(
        const T in_append_pt[3],
        const int *in_local_intersect_ids,
        T **io_pt_ptr,
        int **io_vid_ptr,
        int **io_connected_vid_ptr,
        int **io_intersect_ptr,
        int *io_num_vtx, 
        int *io_num_intersects,
        int top_intersect_vid,
        int num_channels
    )
    {
        int vid_to_append = top_intersect_vid;
        const int *ip_ptr = in_local_intersect_ids;
        const int *end_ip_ptr = ip_ptr + *io_num_intersects;
        for( ; ip_ptr!=end_ip_ptr; ++vid_to_append, ++ip_ptr ){
            if( vid_to_append == *ip_ptr){
                break;
            }
        }

        /* Add vertex id on edge */
        *(*io_connected_vid_ptr) = vid_to_append;
        ++(*io_connected_vid_ptr);

        /* Add vertex id on intersection list */
        *(*io_intersect_ptr) = vid_to_append;
        ++(*io_intersect_ptr);
        ++(*io_num_intersects);

        /* Add new vertex point and index*/
        wsp::CopyArray( (*io_pt_ptr), in_append_pt, 3 );
        (*io_pt_ptr) += num_channels;
        *(*io_vid_ptr) = vid_to_append;
        ++(*io_vid_ptr);
        ++(*io_num_vtx);
    }

    template<typename T>
    inline void BisectPolygonalMesh_DebugPrint(
        T *p_points, int p_num_vtcs,
        T *n_points, int n_num_vtcs,
        int *p_local_edge_vtx_ids, int p_local_num_edge,
        int *n_local_edge_vtx_ids, int n_local_num_edge,
        int *p_local_intersect_ids,
        int *n_local_intersect_ids,
        int intersect_cnt,
        int p_top_fvtx,
        int n_top_fvtx,
        int nc
    )
    {
        WSP_COMMON_DEBUG_LOG(
            "*****************\n"
            "DEBUG PRINT\n\n"
            "intersect_cnt: %d\n"
            "p_local_num_vtx: %d, n_local_num_vtx: %d\n"
            "p_local_num_edge: %d, n_local_num_edge: %d\n"
            , intersect_cnt
            , p_num_vtcs, n_num_vtcs
            , p_local_num_edge, n_local_num_edge
        );
        WSP_COMMON_DEBUG_LOG("\n");
        for(int vidx=p_top_fvtx; vidx<p_num_vtcs; ++vidx){
            WSP_COMMON_DEBUG_LOG("p pt: %2f %2f %2f\n", p_points[vidx*nc], p_points[vidx*nc+1], p_points[vidx*nc+2]);
        }
        WSP_COMMON_DEBUG_LOG("\n");
        for(int vidx=n_top_fvtx; vidx<n_num_vtcs; ++vidx){
            WSP_COMMON_DEBUG_LOG("n pt: %2f %2f %2f\n", n_points[vidx*nc], n_points[vidx*nc+1], n_points[vidx*nc+2]);
        }
        WSP_COMMON_DEBUG_LOG("\n");
        for(int eidx=0; eidx<p_local_num_edge; ++eidx){
            WSP_COMMON_DEBUG_LOG("p edge: %d %d\n", p_local_edge_vtx_ids[eidx*2], p_local_edge_vtx_ids[eidx*2+1]);
        }
        WSP_COMMON_DEBUG_LOG("\n");
        for(int eidx=0; eidx<n_local_num_edge; ++eidx){
            WSP_COMMON_DEBUG_LOG("n edge: %d %d\n", n_local_edge_vtx_ids[eidx*2], n_local_edge_vtx_ids[eidx*2+1]);
        }
        WSP_COMMON_DEBUG_LOG("\n");
        WSP_COMMON_DEBUG_LOG("p intersect id: %d %d\n", p_local_intersect_ids[0], p_local_intersect_ids[1]);
        WSP_COMMON_DEBUG_LOG("n intersect id: %d %d\n", n_local_intersect_ids[0], n_local_intersect_ids[1]);

        WSP_COMMON_DEBUG_LOG("*****************\n");
    }
}

template<typename T>
inline wsp::State wsp::graphic::BisectPolygonalMesh(
    const wsp::Vector<T> &src_points, 
    const wsp::Vector<int> &src_fvtx_counts,
    const wsp::Vector<int> &src_fvtx_connects,
    wsp::Vector<T> &p_points, wsp::Vector<int> &p_fvtx_counts, wsp::Vector<int> &p_fvtx_connects, 
    wsp::Vector<T> &n_points, wsp::Vector<int> &n_fvtx_counts, wsp::Vector<int> &n_fvtx_connects,
    const T in_voronoi_point_a[3], const T in_voronoi_point_b[3]
)
{
    wsp::math::PlaneParam<T> bisec_plane;
    wsp::math::GetBisectorPlaneOfTwoPoints(
        in_voronoi_point_a, 
        in_voronoi_point_b,
        bisec_plane
    );
    return wsp::graphic::BisectPolygonalMesh(
        src_points, src_fvtx_counts, src_fvtx_connects,
        p_points, p_fvtx_counts, p_fvtx_connects, 
        n_points, n_fvtx_counts, n_fvtx_connects,
        bisec_plane
    );
}

template<typename T>
wsp::State wsp::graphic::BisectPolygonalMesh(
    const wsp::Vector<T> &src_points, 
    const wsp::Vector<int> &src_fvtx_counts,
    const wsp::Vector<int> &src_fvtx_connects,
    wsp::Vector<T> &p_points, 
    wsp::Vector<int> &p_fvtx_counts, 
    wsp::Vector<int> &p_fvtx_connects, 
    wsp::Vector<T> &n_points,
    wsp::Vector<int> &n_fvtx_counts,
    wsp::Vector<int> &n_fvtx_connects,
    wsp::math::PlaneParam<T> &bisec_plane
)
{
    wsp::State wstate;
    wsp::math::State wstate_math;
    int num_src_vtcs = src_points.length();
    int nc = src_points.num_of_channels();
    int num_src_poly = src_fvtx_counts.length();

    /* ----------------------------------------------------------------------
     * Calculate dot products for all points in srouce mesh and plane normal
     * to determine whether each point is positive or negative direction 
     * against bisector plane 
     * ----------------------------------------------------------------------
     */
    wsp::Vector<T> dot_list;
    dot_list.SetLength(num_src_vtcs, 1);
    const T dot_round_mult_f = (T)(1<<12);//1.0e+5;
    const T dot_round_mult_b = (T)1.0/dot_round_mult_f;
    {
        T edge_pt_vec[3];
        const T *pt_ptr = src_points.data();
        T *dot_ptr = dot_list.data();
        T *end_ptr = dot_ptr + num_src_vtcs;
        for(; dot_ptr!=end_ptr; ++dot_ptr, pt_ptr+=nc)
        {
            wsp::SustractArray(pt_ptr, bisec_plane.pivot, edge_pt_vec, 3);
            *dot_ptr = wsp::math::GetDotProduct(bisec_plane.normal, edge_pt_vec);
            *dot_ptr = (T)((int)((*dot_ptr) * dot_round_mult_f + 0.5));
            *dot_ptr = (*dot_ptr) * dot_round_mult_b;
        }
    }

    int enough_dst_vtx_length = num_src_vtcs * 2;

    wsp::Vector<uchar> p_vid_check_list, n_vid_check_list;
    wsp::Vector<int> p_vid_list, n_vid_list;
    wsp::Vector<T> intersect_points;

    int num_fvtx, num_fedge;    
    int p_num_vtcs, n_num_vtcs;
    int p_num_poly, n_num_poly;
    int p_num_connects, n_num_connects;

    /* Allocate enough memory to store result */
    p_vid_check_list.SetLength( enough_dst_vtx_length );
    n_vid_check_list.SetLength( enough_dst_vtx_length );
    p_vid_list.SetLength( enough_dst_vtx_length );
    n_vid_list.SetLength( enough_dst_vtx_length );
    intersect_points.SetLength( enough_dst_vtx_length, nc );

    p_points.SetLength( enough_dst_vtx_length, nc );
    n_points.SetLength( enough_dst_vtx_length, nc );
    p_fvtx_counts.SetLength( src_fvtx_counts.length()*2 );
    n_fvtx_counts.SetLength( src_fvtx_counts.length()*2 );
    p_fvtx_connects.SetLength( src_fvtx_connects.length()*2 );
    n_fvtx_connects.SetLength( src_fvtx_connects.length()*2 );

    /* Initialize variables */
    p_num_vtcs = 0;
    n_num_vtcs = 0;
    p_num_poly = 0;
    n_num_poly = 0;
    p_num_connects = 0;
    n_num_connects = 0;
    int intersect_cnt=0;

    memset( p_vid_check_list.data(), 0, sizeof(uchar)*enough_dst_vtx_length );
    memset( n_vid_check_list.data(), 0, sizeof(uchar)*enough_dst_vtx_length );
    memset( p_vid_list.data(), 0, sizeof(uchar)*enough_dst_vtx_length );
    memset( n_vid_list.data(), 0, sizeof(uchar)*enough_dst_vtx_length );
    memset( intersect_points.data(), 0, sizeof(T)*enough_dst_vtx_length*nc );

    int top_intersect_vid = num_src_vtcs;
    int p_intersect_vid = top_intersect_vid;
    int n_intersect_vid = top_intersect_vid;

    int p_local_edge_vtx_ids[30], n_local_edge_vtx_ids[30];

    /* ----------------------------------
        Iterate entire polygons
       ----------------------------------*/
    const int *fvtx_id_ptr = src_fvtx_connects.data();
    const int *fvtx_count_ptr = src_fvtx_counts.data();
    const int *end_ptr = fvtx_count_ptr + num_src_poly;
    T *p_pt_ptr = p_points.data();
    T *n_pt_ptr = n_points.data();
    T *intersect_pt_ptr = intersect_points.data();
    int *p_connect_ptr = p_fvtx_connects.data();
    int *n_connect_ptr = n_fvtx_connects.data();
    int *p_count_ptr = p_fvtx_counts.data();
    int *n_count_ptr = n_fvtx_counts.data();
    int *p_vid_ptr = p_vid_list.data();
    int *n_vid_ptr = n_vid_list.data();

    for( ; fvtx_count_ptr!=end_ptr; ++fvtx_count_ptr )
    {
        WSP_COMMON_DEBUG_LOG("=======================\n");

        /* Number of vertices in current polygon. */
        num_fvtx = num_fedge = *fvtx_count_ptr;

        if( num_fvtx == 0 ){
            WSP_COMMON_WARNING_LOG("Number of vertices is zero\n");
            continue;
        }

        /* 
         * Destination vertex index for pointing the top component 
         * of new polygon
         */
        int p_top_fvtx = p_num_vtcs;
        int n_top_fvtx = n_num_vtcs;
        T *p_top_fvtx_ptr = p_pt_ptr;
        T *n_top_fvtx_ptr = n_pt_ptr;

        /* Declare edge vertex list variables */
        int p_local_num_edge=0;
        int n_local_num_edge=0;

        /* Declare intersection variables */
        int p_local_intersect_ids[10], n_local_intersect_ids[10];
        int local_intersect_cnt = 0;

        /* ---------------------------------------------------------------------- */

        /* Initialize variables for pointer interation */
        const int *fvtx_id_end_ptr = fvtx_id_ptr + num_fvtx;
        const int *orig_fvtx_id_ptr = fvtx_id_ptr;

        /* -----------------------------------------------------------------------
         * Iterate whole vertices in polygon to check if bisection is applicable.
         * Bisection is not applicable for following types of relationship 
         * between polygon and bisector line.
         *    a.           b.            c.
         *     ___          ___           ___|
         *    |   | /      |   |   /     |   |
         *    |___|/       |___|  /      |___|
         *        /              /           |
         * -----------------------------------------------------------------------
         */
        {
            int num_bisect_pt_on_vtx = 0;
            T sign_check;
            T dot = (T)dot_list[*fvtx_id_ptr];
            if( dot_list[*fvtx_id_ptr] == 0.0 ){
                ++num_bisect_pt_on_vtx;
            }
            ++fvtx_id_ptr;

            /*
             * Iterate dot product list to find out applicable polygons.
             */
            for( ; fvtx_id_ptr!=fvtx_id_end_ptr; ++fvtx_id_ptr)
            {
                sign_check = dot * dot_list[*fvtx_id_ptr];
                WSP_COMMON_DEBUG_LOG("sign_check: %f = %f * %f\n", sign_check, dot, dot_list[*fvtx_id_ptr]);
                if( sign_check <  (T)0.0 ){ 
                    WSP_COMMON_DEBUG_LOG("  Bisection is applicable\n");
                    break; 
                }
                if( sign_check == (T)0.0 ){ 
                    if( dot_list[*fvtx_id_ptr] == 0.0 ){
                        ++num_bisect_pt_on_vtx;
                    }
                    if( dot == (T)0.0 && dot_list[*fvtx_id_ptr] == 0.0 ){
                        dot = dot_list[*fvtx_id_ptr];
                    }else if( dot == (T)0.0 ){
                        dot = dot_list[*fvtx_id_ptr];
                    }
                    continue; 
                }
                dot = dot_list[*fvtx_id_ptr];
                WSP_COMMON_DEBUG_LOG("dot[%d] = %f\n", *fvtx_id_ptr, dot);
            }
            
            /*
             * Skip if entire vertices are on bisecter plane. (NOT APPLICABLE)
             */
            // NOT COMPLETED YET
            if( num_bisect_pt_on_vtx > 2 )
            {
                WSP_COMMON_WARNING_LOG("Too many vertices are on bisecter plane, skipped\n");
                continue;
            }

            /*
             * Copy entire vertices in current poly 
             * if less than two intersection points are found. (NOT APPLICABLE)
             */
            if( fvtx_id_ptr==fvtx_id_end_ptr )
            {
                fvtx_id_ptr = orig_fvtx_id_ptr;
                const T *src_pt_ptr;

                /* Copy entire vertices */
                if(dot<0.0)
                {
                    WSP_COMMON_DEBUG_LOG("Copying entire vertices into negative side mesh\n");
                    for( ; fvtx_id_ptr!=fvtx_id_end_ptr; ++fvtx_id_ptr)
                    {
                        src_pt_ptr = src_points.data() + (*fvtx_id_ptr) * nc;
                        AppendExistingPoint(
                            src_pt_ptr, *fvtx_id_ptr, 
                            &n_pt_ptr, &n_vid_ptr, n_vid_check_list.data(), 
                            &n_connect_ptr, &n_num_vtcs, nc
                        );
                    }
                    /* gain count list */
                    *n_count_ptr = num_fvtx;
                    ++n_count_ptr;
                    ++n_num_poly;
                    n_num_connects += num_fvtx;
                }
                else
                {
                    WSP_COMMON_DEBUG_LOG("Copying entire vertices into positive side mesh\n");
                    for( ; fvtx_id_ptr!=fvtx_id_end_ptr; ++fvtx_id_ptr )
                    {
                        src_pt_ptr = src_points.data() + (*fvtx_id_ptr) * nc;
                        AppendExistingPoint(
                            src_pt_ptr, *fvtx_id_ptr, 
                            &p_pt_ptr, &p_vid_ptr, p_vid_check_list.data(), 
                            &p_connect_ptr, &p_num_vtcs, nc
                        );
                    }
                    /* gain count list */
                    *p_count_ptr = num_fvtx;
                    ++p_count_ptr;
                    ++p_num_poly;
                    p_num_connects += num_fvtx;
                }

                BisectPolygonalMesh_DebugPrint(
                    p_points.data(), p_num_vtcs,
                    n_points.data(), n_num_vtcs,
                    p_local_edge_vtx_ids, p_local_num_edge,
                    n_local_edge_vtx_ids, n_local_num_edge,
                    p_local_intersect_ids,
                    n_local_intersect_ids,
                    local_intersect_cnt,
                    p_top_fvtx,
                    n_top_fvtx,
                    nc
                );
                continue;
            }
        }
        /* ---------------------------------------------------------------------- */

        /* ----------------------------------
            Iterate edges in each face
            ----------------------------------*/
        /* Restore original data to pointer */
        fvtx_id_ptr = orig_fvtx_id_ptr;

        /* ID of top vertex in vertex connections */
        int top_vid = *fvtx_id_ptr;

        /* ID of last vertex in vertex connections */
        int tail_vid = *(fvtx_id_end_ptr-1); 

        int *p_local_edge_ptr = p_local_edge_vtx_ids;
        int *n_local_edge_ptr = n_local_edge_vtx_ids;
        int *p_local_intersect_ptr = p_local_intersect_ids;
        int *n_local_intersect_ptr = n_local_intersect_ids;

        for( ; fvtx_id_ptr!=fvtx_id_end_ptr; ++fvtx_id_ptr)
        {
            WSP_COMMON_DEBUG_LOG("---------\n");

            T intersect_pt[3];
            const T *edge_pt0, *edge_pt1;
            T dot0, dot1;
            int edge_vid0, edge_vid1;

            /* -----------------------------------------------------------
                Calculate vertex id and vertex coordinate of current edge
                -----------------------------------------------------------*/
            edge_vid0 = *fvtx_id_ptr;
            if( *fvtx_id_ptr == tail_vid ){
                edge_vid1 = top_vid;
            }else{
                edge_vid1 = *(fvtx_id_ptr+1);
            }
            WSP_COMMON_DEBUG_LOG("Edge: %d %d\n", edge_vid0, edge_vid1);

            edge_pt0 = &(src_points.data()[edge_vid0*nc]);
            edge_pt1 = &(src_points.data()[edge_vid1*nc]);

            /* --------------------------------------------------------------
                Calculate intersection point of voronoi plane and each edge 
                --------------------------------------------------------------*/
            wstate_math = wsp::math::GetIntersectionPointOfPlaneAndLine(    
                edge_pt0,
                edge_pt1,
                bisec_plane,
                intersect_pt
            );

            dot0 = dot_list[edge_vid0];
            dot1 = dot_list[edge_vid1];

            WSP_COMMON_DEBUG_LOG("dot = %f %f\n", dot0, dot1);

            /* ----------------------------------
                    Append existing points
                ----------------------------------*/
            if( dot0 * dot1 >= 0.0 )
            {
                if( dot0 + dot1 > 0.0 )
                {    /* For positive face */                            
                    /* Find current vertex in vtx array, if not found than add vertex */
                    AppendExistingPoint(
                        edge_pt0, edge_vid0, 
                        &p_pt_ptr, &p_vid_ptr, p_vid_check_list.data(), 
                        &p_local_edge_ptr, &p_num_vtcs, nc
                    );
                    AppendExistingPoint(
                        edge_pt1, edge_vid1, 
                        &p_pt_ptr, &p_vid_ptr, p_vid_check_list.data(), 
                        &p_local_edge_ptr, &p_num_vtcs, nc
                    );
                    ++p_local_num_edge;
                }
                else
                {    /* For negative face */    
                    /* Find current vertex in vtx array, if not found than add vertex */
                    AppendExistingPoint(
                        edge_pt0, edge_vid0, 
                        &n_pt_ptr, &n_vid_ptr, n_vid_check_list.data(), 
                        &n_local_edge_ptr, &n_num_vtcs, nc
                    );
                    AppendExistingPoint(
                        edge_pt1, edge_vid1, 
                        &n_pt_ptr, &n_vid_ptr, n_vid_check_list.data(), 
                        &n_local_edge_ptr, &n_num_vtcs, nc
                    );
                    ++n_local_num_edge;
                }



                if( dot0!=0.0 && dot1!=0.0 )
                {
                    /* --------------------------------------
                        If no intersection points are found
                       --------------------------------------*/
                    WSP_COMMON_DEBUG_LOG("No intersections are found\n");
                }
                else if( dot0==0.0 )
                {
                    /* --------------------------------------
                        If intersection point is on existing point
                       --------------------------------------*/
                    WSP_COMMON_DEBUG_LOG("Point on bisector line is found\n");
                    int k;
                    for(k=0; k<local_intersect_cnt; ++k){
                        WSP_COMMON_DEBUG_LOG("%d==%d\n", p_local_intersect_ids[k], edge_vid0 );
                        if( p_local_intersect_ids[k]==edge_vid0 ){ break; }
                    }
                    if(k!=local_intersect_cnt){
                        WSP_COMMON_DEBUG_LOG("Intersection point has already been registered\n");
                        continue;
                    }

                    AppendExistingPoint(
                        edge_pt0, edge_vid0, 
                        &p_pt_ptr, &p_vid_ptr, p_vid_check_list.data(), 
                        &p_local_intersect_ptr, &p_num_vtcs, nc
                    );
                    AppendExistingPoint(
                        edge_pt0, edge_vid0, 
                        &n_pt_ptr, &n_vid_ptr, n_vid_check_list.data(), 
                        &n_local_intersect_ptr, &n_num_vtcs, nc
                    );
                    ++local_intersect_cnt;
                }
                else if( dot1==0.0 )
                {
                    /* --------------------------------------
                        If intersection point is on existing point
                       --------------------------------------*/
                    WSP_COMMON_DEBUG_LOG("Point on bisector line is found\n");
                    int k;
                    for(k=0; k<local_intersect_cnt; ++k){
                        WSP_COMMON_DEBUG_LOG("%d==%d\n", p_local_intersect_ids[k], edge_vid1 );
                        if( p_local_intersect_ids[k]==edge_vid1 ){ break; }
                    }
                    if(k!=local_intersect_cnt){
                        WSP_COMMON_DEBUG_LOG("Intersection point has already been registered\n");
                        continue;
                    }

                    AppendExistingPoint(
                        edge_pt1, edge_vid1, 
                        &p_pt_ptr, &p_vid_ptr, p_vid_check_list.data(), 
                        &p_local_intersect_ptr, &p_num_vtcs, nc
                    );
                    AppendExistingPoint(
                        edge_pt1, edge_vid1, 
                        &n_pt_ptr, &n_vid_ptr, n_vid_check_list.data(), 
                        &n_local_intersect_ptr, &n_num_vtcs, nc
                    );

                    ++local_intersect_cnt;
                }else{
                    WSP_COMMON_ERROR_LOG("Unrecognized pattern\n");
                    return WSP_STATE_FAILURE;
                }
            }
            else
            {
                /* --------------------------------------
                    Append new intersection point
                   --------------------------------------*/
                WSP_COMMON_DEBUG_LOG("A new intersection point is found\n");
                if(local_intersect_cnt > 2){
                    WSP_COMMON_WARNING_LOG("More than two intersectin points are found.\n");
                    continue;
                }

                if( dot0 < 0.0 )
                {
                    AppendExistingPoint(
                        edge_pt1, edge_vid1, 
                        &p_pt_ptr, &p_vid_ptr, p_vid_check_list.data(), 
                        &p_local_edge_ptr, &p_num_vtcs, nc
                    );
                    AppendExistingPoint(
                        edge_pt0, edge_vid0, 
                        &n_pt_ptr, &n_vid_ptr, n_vid_check_list.data(), 
                        &n_local_edge_ptr, &n_num_vtcs, nc
                    );
                }
                else
                {
                    AppendExistingPoint(
                        edge_pt0, edge_vid0, 
                        &p_pt_ptr, &p_vid_ptr, p_vid_check_list.data(), 
                        &p_local_edge_ptr, &p_num_vtcs, nc
                    );
                    AppendExistingPoint(
                        edge_pt1, edge_vid1, 
                        &n_pt_ptr, &n_vid_ptr, n_vid_check_list.data(), 
                        &n_local_edge_ptr, &n_num_vtcs, nc
                    );
                }

                /* -------------------------
                 * Add vertices 
                 * -------------------------
                 */
                int vid_to_append = top_intersect_vid;
                T *ip_ptr = intersect_points.data();
                for( ; ip_ptr!=intersect_pt_ptr; ++vid_to_append, ip_ptr+=nc ){
                    if( ip_ptr[0] == intersect_pt[0]
                     && ip_ptr[1] == intersect_pt[1]
                     && ip_ptr[2] == intersect_pt[2]
                    ){
                        break;
                    }
                }
                if( ip_ptr == intersect_pt_ptr ){
                    wsp::CopyArray( intersect_pt_ptr, intersect_pt, 3 );
                    intersect_pt_ptr += nc;
                    ++intersect_cnt;

                    /* Add new vertex point and index*/
                    wsp::CopyArray( (p_pt_ptr), intersect_pt, 3 );
                    (p_pt_ptr) += nc;
                    (*p_vid_ptr) = vid_to_append;
                    ++(p_vid_ptr);
                    ++(p_num_vtcs);

                    /* Add new vertex point and index*/
                    wsp::CopyArray( (n_pt_ptr), intersect_pt, 3 );
                    (n_pt_ptr) += nc;
                    (*n_vid_ptr) = vid_to_append;
                    ++(n_vid_ptr);
                    ++(n_num_vtcs);
                }
                /* Add vertex id on edge */
                (*p_local_edge_ptr) = vid_to_append;
                ++(p_local_edge_ptr);

                /* Add vertex id on intersection list */
                (*p_local_intersect_ptr) = vid_to_append;
                ++(p_local_intersect_ptr);


                /* -------------------------*/
                /* for negative side */
                /* Add vertex id on edge */
                (*n_local_edge_ptr) = vid_to_append;
                ++(n_local_edge_ptr);

                /* Add vertex id on intersection list */
                (*n_local_intersect_ptr) = vid_to_append;
                ++(n_local_intersect_ptr);

                /* -------------------------*/
                ++p_local_num_edge;
                ++n_local_num_edge;
                ++local_intersect_cnt;
                // wsp::maya::CreateLocator(intersect_ptr);
            }

            /* ---------------------------------------------------------
                Debug
               ---------------------------------------------------------*/
            BisectPolygonalMesh_DebugPrint(
                p_points.data(), p_num_vtcs,
                n_points.data(), n_num_vtcs,
                p_local_edge_vtx_ids, p_local_num_edge,
                n_local_edge_vtx_ids, n_local_num_edge,
                p_local_intersect_ids,
                n_local_intersect_ids,
                local_intersect_cnt,
                p_top_fvtx,
                n_top_fvtx,
                nc
            );
            //std::cout<<"p_vid_list:"<<std::endl<<p_vid_list<<std::endl;
            //std::cout<<"n_vid_list:"<<std::endl<<n_vid_list<<std::endl;            
        }

        if(local_intersect_cnt > 2)
        {
            WSP_COMMON_ERROR_LOG("More than two intersectin points are found, %d points.\n", local_intersect_cnt);
            return WSP_STATE_FAILURE;
        }
        else if(local_intersect_cnt == 2)
        {
            /* Add edge of intersection line */
            p_local_edge_vtx_ids[p_local_num_edge*2]   = p_local_intersect_ids[0];
            p_local_edge_vtx_ids[p_local_num_edge*2+1] = p_local_intersect_ids[1];
            ++p_local_num_edge;

            n_local_edge_vtx_ids[n_local_num_edge*2]   = n_local_intersect_ids[0];
            n_local_edge_vtx_ids[n_local_num_edge*2+1] = n_local_intersect_ids[1];
            ++n_local_num_edge;
        }

        /* Number of vertices in a face */
        int p_local_num_vtx = p_local_num_edge;
        int n_local_num_vtx = n_local_num_edge;

        /* ---------------------------------------------------------
            Debug
           ---------------------------------------------------------*/
        BisectPolygonalMesh_DebugPrint(
            p_points.data(), p_num_vtcs,
            n_points.data(), n_num_vtcs,
            p_local_edge_vtx_ids, p_local_num_edge,
            n_local_edge_vtx_ids, n_local_num_edge,
            p_local_intersect_ids,
            n_local_intersect_ids,
            local_intersect_cnt,
            p_top_fvtx,
            n_top_fvtx,
            nc
        );
        /* -------------------------------------- */
                
        /* --------------------------------------
            Append face data to whole mesh data
           --------------------------------------*/
        *p_count_ptr = p_local_num_vtx;
        ++p_count_ptr;
        ++p_num_poly;

        wstate = wsp::graphic::GetVtxConnectionsFromEdgeVertexIndices(
            (int*)p_local_edge_vtx_ids, 
            (int*)p_connect_ptr,
            p_local_num_edge
        );
        if(wstate!=WSP_STATE_SUCCESS){
            WSP_COMMON_ERROR_LOG("Could not calculate vertex connections\n");
            return wstate;
        }
        p_num_connects += p_local_num_vtx;
        p_connect_ptr += p_local_num_vtx;

        //std::cout<<"p connects: "<<p_fvtx_connects<<std::endl;

        /* --------------------------------------*/
        *n_count_ptr = n_local_num_vtx;
        ++n_count_ptr;
        ++n_num_poly;

        wstate = wsp::graphic::GetVtxConnectionsFromEdgeVertexIndices(
            (int*)n_local_edge_vtx_ids, 
            (int*)n_connect_ptr,
            n_local_num_edge
        );
        if(wstate!=WSP_STATE_SUCCESS){
            WSP_COMMON_ERROR_LOG("Could not calculate vertex connections\n");
            return wstate;
        }
        n_num_connects += n_local_num_vtx;
        n_connect_ptr += n_local_num_vtx;

        //std::cout<<"n connects: "<<n_fvtx_connects<<std::endl;
    }

    /* Reallocate memories to fit to actual data size */
    p_points.SetLength(p_num_vtcs, nc);
    n_points.SetLength(n_num_vtcs, nc);

    p_fvtx_counts.SetLength(p_num_poly);
    n_fvtx_counts.SetLength(n_num_poly);

    p_fvtx_connects.SetLength(p_num_connects);
    n_fvtx_connects.SetLength(n_num_connects);

    p_vid_list.SetLength(p_num_vtcs);
    n_vid_list.SetLength(n_num_vtcs);

    WSP_COMMON_DEBUG_LOG("Reindexing..\n");
    std::cout<<"p_vid_list:"<<std::endl<<p_vid_list<<std::endl;
    std::cout<<"n_vid_list:"<<std::endl<<n_vid_list<<std::endl;
    std::cout<<"p_fvtx_connects:"<<std::endl<<p_fvtx_connects<<std::endl;
    std::cout<<"n_fvtx_connects:"<<std::endl<<n_fvtx_connects<<std::endl;
    wsp::graphic::Reindex( p_vid_list.data(), p_fvtx_connects.data(), p_num_vtcs, p_num_connects );
    wsp::graphic::Reindex( n_vid_list.data(), n_fvtx_connects.data(), n_num_vtcs, n_num_connects );
    std::cout<<"reindexed p_fvtx_connects:"<<std::endl<<p_fvtx_connects<<std::endl;
    std::cout<<"reindexed n_fvtx_connects:"<<std::endl<<n_fvtx_connects<<std::endl;

    return WSP_STATE_SUCCESS;
}


template<typename _DepthType>
wsp::State wsp::graphic::CreateTriMeshFromDepthMap(
    const wsp::Image<_DepthType> &in_depth,
    wsp::Vector<wsp::Point<float>> &o_vtx_array, 
    wsp::Vector<int> &o_vtx_count_on_face, 
    wsp::Vector<int> &o_vtx_list_on_faces, 
    wsp::Vector<int> *o_edge_indices,
    wsp::Vector<wsp::Vector3<float>> *o_normals,
    wsp::Vector<float> *o_uvs,
    const wsp::graphic::PerspViewParam &view_param,
    double face_depth_thre, 
    int sampling_step, 
    uchar alpha_thre, 
    double scale,
    bool invert_depth,
    float offset_x, float offset_y, float offset_z,
    _DepthType depth_min, _DepthType depth_max
)
{
    int itr_x, itr_y;
    //int v1, v2, v3, v4;
    int width, height, length;
    //WspString wstr;
    wsp::Image<_DepthType> depth_map(in_depth);
    
    if(invert_depth==true){
        depth_map.Invert();
    }
    if(depth_min==depth_max){
        depth_max = depth_map.GetMax();
        depth_min = depth_map.GetMin();
    }
    _DepthType range = depth_max - depth_min;
    //depth_map.Normalize(0.0, range);
    //depth_map.Normalize(0.0, 1.0);

    width = (int)depth_map.width();
    height =(int)depth_map.height();
    length = width*height;
    _DepthType aspect = static_cast<_DepthType>(width)/static_cast<_DepthType>(height);


    int w_mesh = static_cast<int>(ceil(width/(float)sampling_step));
    int h_mesh = static_cast<int>(ceil(height/(float)sampling_step));


    // initializing arrays for mesh
    o_vtx_array.SetLength(length);
    o_vtx_count_on_face.SetLength(length*2);
    o_vtx_list_on_faces.SetLength(length*2*3);
    if(o_edge_indices){
        o_edge_indices->SetLength(((width-1)*height+(height-1)*width+(height-1)*(width-1))*2);
    }
    if(o_normals){
        o_normals->SetLength(length*2);
    }
    if(o_uvs){
        o_uvs->SetLength(length*2);
    }

    face_depth_thre/=255.0; // normalize threshold

    _DepthType dz;
    float vpos[4][3]; // buffer for vertex position
    int pix_indices[4]; // buffer for vertex index 
    int numFaces=0;
    int num_edge=0;
    int numVtx=0;
    int numVtxOnFaces=0;
    int num_normal = 0;
    int num_uvs = 0;
    wsp::Vector<int> vtx_index_map(width*height);
    u32 i, itr_v, itr_vi, itr_vj;
    _DepthType x, y, xstep, ystep;
    _DepthType w_2=static_cast<_DepthType>(width)/2.0;
    _DepthType h_2=static_cast<_DepthType>(height)/2.0;
    vtx_index_map.Reset(-1);
    bool skip;
    int step=sampling_step;
    int nc = static_cast<int>(depth_map.num_of_channels());
    _DepthType sx, sy, sz, tan_fov_2;//, scale=0.01;

    tan_fov_2 = tan(view_param.field_of_view*0.5f);
    sy = - tan_fov_2;
    sx = tan_fov_2 * aspect;
    sz = (view_param.far_clip - view_param.near_clip)/range; // / view_param.far_clip;    //_DepthType mult = 1.0/range;
    // set up an array for vertex position
    // set up an array to assign vertices from vexArray to each face
    for(itr_y = 0; itr_y<height-step; itr_y+=step){
        for(itr_x = 0; itr_x<width-step; itr_x+=step){
            if(depth_map.GetAlpha(itr_y*width+itr_x)<alpha_thre){ continue; }

        
            // normalize size to be range as -1 to 1
            x = (static_cast<_DepthType>(itr_x) - w_2)/w_2;
            y = (static_cast<_DepthType>(itr_y) - h_2)/h_2;
            xstep = (static_cast<_DepthType>(itr_x + step) - w_2)/w_2;
            ystep = (static_cast<_DepthType>(itr_y + step) - h_2)/h_2;

            // indices of pix_indices are like this 
            //**
            //        0--1
            //        | /|
            //        |/ |
            //        3--2
            //**
            pix_indices[0]=itr_y*width+itr_x;
            pix_indices[1]=itr_y*width+itr_x+step;
            pix_indices[2]=(itr_y+step)*width+itr_x+step;
            pix_indices[3]=(itr_y+step)*width+itr_x;
            //pix_indices[0]=itr_y*w_mesh+itr_x;
            //pix_indices[1]=pix_indices[0]+step;
            //pix_indices[3]=(itr_y+step)*w_mesh+itr_x;
            //pix_indices[2]=pix_indices[3]+step;
            //WSP_COMMON_DEBUG_LOG("%d %d %d %d\n", pix_indices[0], pix_indices[1], pix_indices[2], pix_indices[3]);

            // z pos
            vpos[0][2] = depth_map[pix_indices[0]*nc]*sz + view_param.near_clip - depth_min;
            vpos[1][2] = depth_map[pix_indices[1]*nc]*sz + view_param.near_clip - depth_min;
            vpos[2][2] = depth_map[pix_indices[2]*nc]*sz + view_param.near_clip - depth_min;
            vpos[3][2] = depth_map[pix_indices[3]*nc]*sz + view_param.near_clip - depth_min;

            //vpos[0][2] = depth_map[pix_indices[0]*nc]*sz + view_param.near_clip + offset_z;
            //vpos[1][2] = depth_map[pix_indices[1]*nc]*sz + view_param.near_clip + offset_z;
            //vpos[2][2] = depth_map[pix_indices[2]*nc]*sz + view_param.near_clip + offset_z;
            //vpos[3][2] = depth_map[pix_indices[3]*nc]*sz + view_param.near_clip + offset_z;


            // x and y pos
            if(view_param.field_of_view!=0){
                vpos[0][0] = x * sx * vpos[0][2] + offset_x;//depth_img[pix_indices[0]*nc];
                vpos[0][1] = y * sy * vpos[0][2] + offset_y;//depth_img[pix_indices[0]*nc];

                vpos[1][0] = (xstep) * sx  * vpos[1][2] + offset_x;//* depth_img[pix_indices[1]*nc];
                vpos[1][1] = y * sy * vpos[1][2]        + offset_y;//depth_img[pix_indices[1]*nc];

                vpos[2][0] = (xstep) * sx  * vpos[2][2] + offset_x;//* depth_img[pix_indices[2]*nc];    
                vpos[2][1] = (ystep) * sy  * vpos[2][2] + offset_y;//* depth_img[pix_indices[2]*nc];

                vpos[3][0] = x * sx  * vpos[3][2]       + offset_x;//* depth_img[pix_indices[3]*nc];
                vpos[3][1] = (ystep) * sy  * vpos[3][2] + offset_y;//* depth_img[pix_indices[3]*nc];    
            }else{
                vpos[0][0] = x + offset_x;
                vpos[0][1] = y + offset_y;

                vpos[1][0] = xstep + offset_x;
                vpos[1][1] = y     + offset_y;

                vpos[2][0] = xstep + offset_x;    
                vpos[2][1] = ystep + offset_y;

                vpos[3][0] = x     + offset_x;
                vpos[3][1] = ystep + offset_y;    
            }

            // add offset
            vpos[0][2] += offset_z;
            vpos[1][2] += offset_z;
            vpos[2][2] += offset_z;
            vpos[3][2] += offset_z;


            // move model to origin
            //vpos[0][2] -= view_param.near_clip;
            //vpos[1][2] -= view_param.near_clip;
            //vpos[2][2] -= view_param.near_clip;
            //vpos[3][2] -= view_param.near_clip;


            //vi1[0]=itr_y*width+itr_x;
            //vi1[1]=itr_y*width+itr_x+1;
            //vi1[2]=(itr_y+1)*width+itr_x+1;
            //vi1[3]=(itr_y+1)*width+itr_x;

            // find long z-distance verteces
            skip=false;
            for(itr_vi=0; itr_vi<4; itr_vi++){
                for(itr_vj=itr_vi+1; itr_vj<4; itr_vj++){
                    dz=fabs((float)(depth_map[pix_indices[itr_vi]*nc] - depth_map[pix_indices[itr_vj]*nc]));
                    if(dz>face_depth_thre){ skip=true; break; }
                }
                if(skip){ break; }
            }
            //for(itr_vi=0; itr_vi<4; itr_vi++){
            //    for(itr_vj=itr_vi+1; itr_vj<4; itr_vj++){
            //        dz=fabs(depth_map[vi1[itr_vi]] - depth_map[vi1[itr_vj]]);
            //        if(dz>face_depth_thre){ skip=true; break; }
            //    }
            //    if(skip){ break; }
            //}
            // skip if aspective ratio of face is not near to 1.0
            if(skip){ continue; }

            // apend verteces --------------------------
            for(itr_v=0; itr_v<4; itr_v++){
                i=pix_indices[itr_v];
                if(vtx_index_map[i]==-1){
                    //depth = depth_map[i*nc];
                    //vpos[itr_v][2] = depth_map[i*nc];
                    o_vtx_array[numVtx]=wsp::Point<float>((float)vpos[itr_v][0], (float)vpos[itr_v][1], (float)vpos[itr_v][2]);
                    vtx_index_map[i]=numVtx;
                    ++numVtx;

                }
            }

            // set 3 verteces per face
            o_vtx_list_on_faces[numVtxOnFaces]=vtx_index_map[pix_indices[0]];
            ++numVtxOnFaces;
            o_vtx_list_on_faces[numVtxOnFaces]=vtx_index_map[pix_indices[1]];
            ++numVtxOnFaces;
            o_vtx_list_on_faces[numVtxOnFaces]=vtx_index_map[pix_indices[3]];
            ++numVtxOnFaces;

            o_vtx_list_on_faces[numVtxOnFaces]=vtx_index_map[pix_indices[1]];
            ++numVtxOnFaces;
            o_vtx_list_on_faces[numVtxOnFaces]=vtx_index_map[pix_indices[2]];
            ++numVtxOnFaces;
            o_vtx_list_on_faces[numVtxOnFaces]=vtx_index_map[pix_indices[3]];
            ++numVtxOnFaces;

            wsp::Vec3F vec0, vec1;
            //if(o_normals!=NULL){
            //    vec0 = o_vtx_array[pix_indices[1]] - o_vtx_array[pix_indices[0]];
            //    vec1 = o_vtx_array[pix_indices[3]] - o_vtx_array[pix_indices[1]];
            //    (*o_normals)[num_normal] = vec0.GetCrossProduct(vec1);
            //    ++num_normal;

            //    vec0 = o_vtx_array[pix_indices[3]] - o_vtx_array[pix_indices[0]];
            //    vec1 = o_vtx_array[pix_indices[2]] - o_vtx_array[pix_indices[3]];
            //    (*o_normals)[num_normal] = vec0.GetCrossProduct(vec1);;
            //    ++num_normal;
            //}

            if(o_uvs!=NULL){
                (*o_uvs)[vtx_index_map[pix_indices[0]]*2] = itr_x/(float)(width);
                (*o_uvs)[vtx_index_map[pix_indices[0]]*2+1] = itr_y/(float)(height);

                (*o_uvs)[vtx_index_map[pix_indices[1]]*2] = (itr_x+1)/(float)(width);
                (*o_uvs)[vtx_index_map[pix_indices[1]]*2+1] = (itr_y)/(float)(height);

                (*o_uvs)[vtx_index_map[pix_indices[2]]*2] = (itr_x+1)/(float)(width);
                (*o_uvs)[vtx_index_map[pix_indices[2]]*2+1] = (itr_y+1)/(float)(height);

                (*o_uvs)[vtx_index_map[pix_indices[3]]*2] = (itr_x)/(float)(width);
                (*o_uvs)[vtx_index_map[pix_indices[3]]*2+1] = (itr_y+1)/(float)(height);
                num_uvs += 8;
            }


            if(o_edge_indices!=NULL){
                if(itr_x==0){
                    (*o_edge_indices)[num_edge] = vtx_index_map[pix_indices[0]];
                    ++num_edge;
                    (*o_edge_indices)[num_edge] = vtx_index_map[pix_indices[3]];
                    ++num_edge;
                }
                if(itr_y==0){
                    (*o_edge_indices)[num_edge] = vtx_index_map[pix_indices[1]];
                    ++num_edge;
                    (*o_edge_indices)[num_edge] = vtx_index_map[pix_indices[0]];
                    ++num_edge;
                }

                // draw horizontal line
                (*o_edge_indices)[num_edge] = vtx_index_map[pix_indices[3]];
                ++num_edge;
                (*o_edge_indices)[num_edge] = vtx_index_map[pix_indices[2]];
                ++num_edge;

                // draw vertical line
                (*o_edge_indices)[num_edge] = vtx_index_map[pix_indices[2]];
                ++num_edge;
                (*o_edge_indices)[num_edge] = vtx_index_map[pix_indices[1]];
                ++num_edge;

                // draw oblique line
                (*o_edge_indices)[num_edge] = vtx_index_map[pix_indices[1]];
                ++num_edge;
                (*o_edge_indices)[num_edge] = vtx_index_map[pix_indices[3]];
                ++num_edge;


            }
            // --------------------------------
            
            // set up an array countaining the number of vertices for each of faces (3 vertices per face)
            o_vtx_count_on_face[numFaces]=3;
            ++numFaces;
            o_vtx_count_on_face[numFaces]=3;
            ++numFaces;
        }
    }
    o_vtx_array.SetLength(numVtx);
    //vtx_index_map.SetLength(numVtx);
    o_vtx_list_on_faces.SetLength(numVtxOnFaces);
    o_vtx_count_on_face.SetLength(numFaces);

    //std::cout<<"points:"<<o_vtx_array<<std::endl;
    //std::cout<<"vtx list on faces:"<<o_vtx_list_on_faces<<std::endl;
    //std::cout<<"vtx count on faces:"<<o_vtx_count_on_face<<std::endl;

    if(o_edge_indices!=NULL){
        o_edge_indices->SetLength(num_edge);
        //std::cout<<"edge:"<<o_edge_indices<<std::endl;
    }
    if(o_normals!=NULL){
        wsp::graphic::CreateVtxNormalsOnTriangleMesh(
                o_vtx_array,
                o_vtx_list_on_faces,
                *o_normals
            );
        //o_normals->SetLength(o_vtx_array.length());
        //o_normals->Reset(wsp::Vector3<float>(0,0,0));
        //int len = o_vtx_list_on_faces.length();
        //for(int i=0; i<len; i+=3) {
        //    const wsp::Point<float> a = o_vtx_array[o_vtx_list_on_faces[i]];
        //    const wsp::Point<float> b = o_vtx_array[o_vtx_list_on_faces[i+1]];
        //    const wsp::Point<float> c = o_vtx_array[o_vtx_list_on_faces[i+2]];

        //    wsp::Vector3<float> normal = wsp::Point<float>(b-a).GetCrossProduct(c-a);
        //    normal.NormalizeLength();

        //    for (int j = 0; j < 3; ++j){
        //        (*o_normals)[o_vtx_list_on_faces[i + j]] += normal;
        //    }
        //}
    }
    if(o_uvs!=NULL){
        o_uvs->SetLength(num_uvs);
    }


    return WSP_STATE_SUCCESS;
}

#endif