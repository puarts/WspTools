/**
 * @file fn-maya.cpp
 * @author Jun-ichi Nishikata
 * @brief OpenMaya wrapper functions.
 */

#include "fn-maya.hpp"


#include <maya/MFloatPointArray.h>
#include <maya/MIntArray.h>

#include <maya/MFnTransform.h>

#include <maya/MFnDagNode.h>
#include <maya/MFnMesh.h>



void wsp::maya::CreateMesh(
    float src_vtx_list[][4], int num_vtx,
    int *src_poly_cnt_list, int num_poly,
    int *src_poly_connects, int num_poly_connects,
    MObject *o_mesh_obj
)
{
    MFnMesh fn_mesh;

    MFloatPointArray vtx_list(src_vtx_list, num_vtx);
    MIntArray poly_cnt(src_poly_cnt_list, num_poly);
    MIntArray poly_connects(src_poly_connects, num_poly_connects);
    fn_mesh.create( 
        vtx_list.length(), 
        num_poly, 
        vtx_list, 
        poly_cnt, 
        poly_connects
    );

    *o_mesh_obj = fn_mesh.object();
}
void CreateMeshExample()
{
    MObject mesh_obj;
    float src_vtx_list[][4] = {
        {-1.0f, 0.0f, 1.0f, 0.0f},
        { 1.0f, 0.0f, 1.0f, 0.0f},
        { 1.0f, 0.0f,-1.0f, 0.0f},
        {-1.0f, 0.0f,-1.0f, 0.0f}
    };
    int num_poly = 2;
    int src_poly_cnt[] = {3, 3};
    int src_poly_connects[] = {
        0, 1, 3, 
        1, 2, 3
    };
    wsp::maya::CreateMesh(src_vtx_list, 4, src_poly_cnt, 2, src_poly_connects, 6, &mesh_obj);
}

