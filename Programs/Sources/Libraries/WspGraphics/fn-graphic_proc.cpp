/**
 * @file fn-graphic_proc.cpp
 * 
 */

#include "_define_graphic.h"
#include <wsp/common/_tpl_fn-sort.hpp>
#include <wsp/common/tpl_cl-vector3.h>
#include <wsp/common/cl-string.h>
#include <wsp/image/tpl_cl-image.h>

#ifdef USE_OPENMAYA
    #include <wsp/maya/_define_maya.h>
#endif

#include "fn-graphic_proc.h"

#include <wsp/math/_fn-math.h>

#undef LoadImage

void CreateNormalsFromVertices(
    wsp::Vector<wsp::Vector3<float>>& o_normals,
    const wsp::Vector<wsp::Vector3<float>>& positions,
    const wsp::Vector<int>& face_point_indices)
{

}

void wsp::graphic::CreateNormalsFromVertices(
    wsp::Vector<wsp::Vector3<float>>& o_normals,
    const wsp::Vector<wsp::Vector3<float>>& positions,
    const wsp::Vector<int>& face_point_indices
    )
{
    o_normals.SetLength(positions.length());
    o_normals.Reset(wsp::Vector3<float>(0, 0, 0));
    int len = face_point_indices.length();
    for (int i = 0; i<len; i += 3) {
        const wsp::Point<float> a(positions[face_point_indices[i]]);
        const wsp::Point<float> b(positions[face_point_indices[i + 1]]);
        const wsp::Point<float> c(positions[face_point_indices[i + 2]]);

        wsp::Vector3<float> normal = wsp::Point<float>(b - a).GetCrossProduct(wsp::Vector3<float>(c - a));
        normal.NormalizeLength();

        for (int j = 0; j < 3; ++j) {
            o_normals[face_point_indices[i + j]] += normal;
        }
    }

    len = o_normals.length();
    for (int i = 0; i<len; ++i) {
        o_normals[i].NormalizeLength();
    }
}

void wsp::graphic::CreateNormalsFromVertices(
    wsp::Vector3<float>* o_normal_array, int normal_array_length,
    const wsp::Vector3<float>* in_position_array, int position_array_length,
    const int* in_face_vertex_indiex_array, int face_vertex_index_array_length)
{
    WSP_REQUIRES(normal_array_length == position_array_length, "Invalid argument");

    wsp::Vector<wsp::Vector3<float>> o_normals;
    wsp::Vector<wsp::Vector3<float>> positions(in_position_array, position_array_length);
    wsp::Vector<int> face_point_indices(in_face_vertex_indiex_array, face_vertex_index_array_length);

    wsp::graphic::CreateNormalsFromVertices(o_normals, positions, face_point_indices);

    memcpy(o_normal_array, o_normals.data(), sizeof(wsp::Vector3<float>) * normal_array_length);
}


void wsp::graphic::CreateVtxNormalsOnTriangleMesh(
    const wsp::Vector<wsp::Point<float>> &in_vtx_pos, 
    const wsp::Vector<int> &in_vid_for_faces,
    wsp::Vector<wsp::Vector3<float>> &o_normals
)
{
    o_normals.SetLength(in_vtx_pos.length());
    o_normals.Reset(wsp::Vector3<float>(0,0,0));
    int len = in_vid_for_faces.length();
    for(int i=0; i<len; i+=3) {
        const wsp::Point<float> a = in_vtx_pos[in_vid_for_faces[i]];
        const wsp::Point<float> b = in_vtx_pos[in_vid_for_faces[i+1]];
        const wsp::Point<float> c = in_vtx_pos[in_vid_for_faces[i+2]];

        wsp::Vector3<float> normal = wsp::Point<float>(b-a).GetCrossProduct(wsp::Vector3<float>(c-a));
        normal.NormalizeLength();

        for (int j = 0; j < 3; ++j){
            o_normals[in_vid_for_faces[i + j]] += normal;
        }
    }
}

void wsp::graphic::CreateRectanglePolygon(
    wsp::Vector<wsp::Vector3<float>>& positions,
    wsp::Vector<wsp::Vector2<float>>& uvs,
    wsp::Vector<int>& indexes)
{
    positions.Clear();
    positions.Append(wsp::Vector3<float>(-1.0f, -1.0f, 0.0f));
    positions.Append(wsp::Vector3<float>(1.0f, -1.0f, 0.0f));
    positions.Append(wsp::Vector3<float>(1.0f, 1.0f, 0.0f));
    positions.Append(wsp::Vector3<float>(-1.0f, 1.0f, 0.0f));

    uvs.Clear();
    uvs.Append(wsp::Vector2<float>(0.0f, 1.0f));
    uvs.Append(wsp::Vector2<float>(1.0f, 1.0f));
    uvs.Append(wsp::Vector2<float>(1.0f, 0.0f));
    uvs.Append(wsp::Vector2<float>(0.0f, 0.0f));

    indexes.Clear();
    indexes.Append(0);
    indexes.Append(1);
    indexes.Append(2);

    indexes.Append(3);
    indexes.Append(0);
    indexes.Append(2);
}

void wsp::graphic::CreateCircleEdgePolygon(
    wsp::Vector<wsp::Vector3<float>>& positions,
    wsp::Vector<wsp::Vector2<float>>& uvs,
    wsp::Vector<int>& indexes)
{
    int kNumCircleSplit = 16;
    float kCircleRadius = 1.0f;
    float kAngularInterval = static_cast<float>(M_PI * 2.0f / (float)kNumCircleSplit);
    wsp::Vector<wsp::Vector3<float>> circle_positions;
    wsp::Vector<wsp::Vector2<float>> circle_uvs;
    float z = 2.0f;
    for (int i = 0; i < kNumCircleSplit; ++i)
    {
        float angle = kAngularInterval * i;
        float cos_angle = cos(angle);
        float sin_angle = sin(angle);
        float x = kCircleRadius * cos_angle;
        float y = kCircleRadius * sin_angle;
        circle_positions.Append(wsp::Vector3<float>(x, y, z));

        float u = cos_angle / 2.0f + 1.0f;
        float v = -sin_angle / 2.0f + 1.0f;
        circle_uvs.Append(wsp::Vector2<float>(u, v));
    }

    positions.Clear();
    uvs.Clear();
    for (int i = 0, end = circle_positions.length() - 1; i < end; ++i)
    {
        positions.Append(circle_positions[i]);
        positions.Append(circle_positions[i + 1]);

        uvs.Append(circle_uvs[i]);
        uvs.Append(circle_uvs[i + 1]);
    }

    {
        int last_index = circle_positions.length() - 1;
        positions.Append(circle_positions[last_index]);
        positions.Append(circle_positions[0]);
        uvs.Append(circle_uvs[last_index]);
        uvs.Append(circle_uvs[0]);
    }
}

void wsp::graphic::CreateCirclePolygon(
    wsp::Vector<wsp::Vector3<float>>& positions,
    wsp::Vector<wsp::Vector2<float>>& uvs,
    wsp::Vector<int>& indexes)
{
    int kNumCircleSplit = 16;
    float kCircleRadius = 1.0f;
    float kAngularInterval = static_cast<float>(M_PI * 2.0f / (float)kNumCircleSplit);
    wsp::Vector<wsp::Vector3<float>> circle_positions;
    wsp::Vector<wsp::Vector2<float>> circle_uvs;
    float z = 2.0f;
    for (int i = 0; i < kNumCircleSplit; ++i)
    {
        float angle = kAngularInterval * i;
        float cos_angle = cos(angle);
        float sin_angle = sin(angle);
        float x = kCircleRadius * cos_angle;
        float y = kCircleRadius * sin_angle;
        circle_positions.Append(wsp::Vector3<float>(x, y, z));

        float u = cos_angle / 2.0f + 1.0f;
        float v = -sin_angle / 2.0f + 1.0f;
        circle_uvs.Append(wsp::Vector2<float>(u, v));
    }

    positions.Clear();
    uvs.Clear();
    for (int i = 0, end = circle_positions.length() - 1; i < end; ++i)
    {
        positions.Append(circle_positions[i]);
        positions.Append(circle_positions[i + 1]);
        positions.Append(wsp::Vector3<float>(0.0f, 0.0f, z));

        uvs.Append(circle_uvs[i]);
        uvs.Append(circle_uvs[i + 1]);
        uvs.Append(wsp::Vector2<float>(0.5f, 0.5f));
    }

    {
        int last_index = circle_positions.length() - 1;
        positions.Append(circle_positions[last_index]);
        positions.Append(circle_positions[0]);
        positions.Append(wsp::Vector3<float>(0.0f, 0.0f, z));

        uvs.Append(circle_uvs[last_index]);
        uvs.Append(circle_uvs[0]);
        uvs.Append(wsp::Vector2<float>(0.5f, 0.5f));
    }

}

void wsp::graphic::CreateTaurusPoly(
    float *o_vtx, 
    float *o_nomals,
    float *o_uvs,
    int *o_indices, 
    int blocks
){
    const float r0 = 1.0f;
    const float r1 = 0.3f;
    static int flag = 0;

    /*
     *  setup taurus
     */
    int idx;
    int i, j;
    float theta, pai;

    idx = 0;
    {
        float inc_angle = (360.0f / (float)blocks);
        float to_rad = M_PI / 180.0f;

        float *pt_ptr = (float*)o_vtx;
        float *n_ptr = (float*)o_nomals;
        float *uv_ptr = (float*)o_uvs;
        for (theta = 0.0f; theta <= 360.0f; theta += inc_angle) {
            float co0 = (float)cos(theta * to_rad);
            float si0 = (float)sin(theta * to_rad);

            for (pai = 0.0f; pai <= 360.0f; 
                pai += inc_angle, ++pt_ptr, ++n_ptr, ++uv_ptr) 
            {
                float co1 = (float)cos(pai * to_rad);
                float si1 = (float)sin(pai * to_rad);

                /* vertex */
                *pt_ptr = (r0 + r1 * co1) * co0;
                *(++pt_ptr) = r1 *si1;
                *(++pt_ptr) = - (r0 + r1 * co1) * si0;

                /* normal */
                *n_ptr = co1 * co0;
                *(++n_ptr) = si1;
                *(++n_ptr) = - co1 * si0;

                /* texture */
                *uv_ptr = theta / 360.0f;
                *(++uv_ptr) = pai / 360.0f;
            }
        }
    }

    {
        int k = 0;
        int *idx_ptr = o_indices;
        for (i = 0; i < blocks; ++i) {
            for (j = 0; j < blocks + 1; ++j, ++idx_ptr) {
                *idx_ptr = i * (blocks + 1) + j;
                *(++idx_ptr) = (i + 1) * (blocks + 1) + j;
            }
        }
    }
}



wsp::State wsp::graphic::CreateTriMeshFromDepthMap(
        wsp::Vector<wsp::Point<float>> &o_vtx_array, 
        wsp::Vector<int> &o_vtx_count_on_face, 
        wsp::Vector<int> &o_vtx_list_on_faces, 
        wsp::Vector<int> *o_edge_indices,
        wsp::Vector<wsp::Vector3<float>> *o_normals,
        wsp::Vector<float> *o_uvs,
        const char *filename, 
        const wsp::graphic::PerspViewParam &view_param,
        double face_depth_thre, 
        int sampling_step, 
        uchar alpha_thre, 
        double scale,
        bool invert_depth,
        float offset_x, float offset_y, float offset_z
){
    double depth;
    int itr_x, itr_y, itr_i;
    //int v1, v2, v3, v4;
    int width, height, length;
    wsp::Image<double> depth_img;
    wsp::State state;
    //WspString wstr;
    
    // loading depth image
    state=depth_img.Load(filename);
    if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Loading depth image failed\n"); return state; }
    if(invert_depth==false){
        depth_img.Invert();
    }
    depth_img.Normalize(0.0, 1.0);

    width = (int)depth_img.width();
    height =(int)depth_img.height();
    length = width*height;
    double aspect = static_cast<double>(width)/static_cast<double>(height);


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

    double dz;
    double vpos[4][3]; // buffer for vertex position
    int pix_indices[4]; // buffer for vertex index 
    int numFaces=0;
    int num_edge=0;
    int numVtx=0;
    int numVtxOnFaces=0;
    int num_normal = 0;
    int num_uvs = 0;
    wsp::Vector<int> vtx_index_map(width*height);
    u32 i, itr_v, itr_vi, itr_vj;
    double x, y, xstep, ystep;
    double w_2=static_cast<double>(width)/2.0;
    double h_2=static_cast<double>(height)/2.0;
    vtx_index_map.Reset(-1);
    bool skip;
    int step=sampling_step;
    int nc = static_cast<int>(depth_img.num_of_channels());
    double sx, sy, sz, tan_fov_2;//, scale=0.01;


    // set up an array for vertex position
    // set up an array to assign vertices from vexArray to each face
    for(itr_y = 0; itr_y<height-step; itr_y+=step){
        for(itr_x = 0; itr_x<width-step; itr_x+=step){
            if(depth_img.GetAlpha(itr_y*width+itr_x)<alpha_thre){ continue; }

            tan_fov_2 = tan(view_param.field_of_view*0.5);
            sy = - tan_fov_2;
            sx = tan_fov_2 * aspect;
            sz = (view_param.far_clip - view_param.near_clip); // / view_param.far_clip;
            
            // normalize size to be range as -1 to 1
            x = (static_cast<double>(itr_x) - w_2)/w_2;
            y = (static_cast<double>(itr_y) - h_2)/h_2;
            xstep = (static_cast<double>(itr_x + step) - w_2)/w_2;
            ystep = (static_cast<double>(itr_y + step) - h_2)/h_2;

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
            //printf("%d %d %d %d\n", pix_indices[0], pix_indices[1], pix_indices[2], pix_indices[3]);

            // z pos
            vpos[0][2] = depth_img[pix_indices[0]*nc]*sz + view_param.near_clip + offset_z;
            vpos[1][2] = depth_img[pix_indices[1]*nc]*sz + view_param.near_clip + offset_z;
            vpos[2][2] = depth_img[pix_indices[2]*nc]*sz + view_param.near_clip + offset_z;
            vpos[3][2] = depth_img[pix_indices[3]*nc]*sz + view_param.near_clip + offset_z;

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
                    dz=fabs(depth_img[pix_indices[itr_vi]*nc] - depth_img[pix_indices[itr_vj]*nc]);
                    if(dz>face_depth_thre){ skip=true; break; }
                }
                if(skip){ break; }
            }
            //for(itr_vi=0; itr_vi<4; itr_vi++){
            //    for(itr_vj=itr_vi+1; itr_vj<4; itr_vj++){
            //        dz=fabs(depth_img[vi1[itr_vi]] - depth_img[vi1[itr_vj]]);
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
                    //depth = depth_img[i*nc];
                    //vpos[itr_v][2] = depth_img[i*nc];
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

    //cout<<"points:"<<o_vtx_array<<endl;
    //cout<<"vtx list on faces:"<<o_vtx_list_on_faces<<endl;
    //cout<<"vtx count on faces:"<<o_vtx_count_on_face<<endl;

    if(o_edge_indices!=NULL){
        o_edge_indices->SetLength(num_edge);
        //cout<<"edge:"<<o_edge_indices<<endl;
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

wsp::State wsp::graphic::CreateQuadMeshFromDepthMap(
    wsp::Vector<wsp::Point<float>> &o_vtx_array, 
    wsp::Vector<int> &o_vtx_count_on_face, 
    wsp::Vector<int> &o_vtx_list_on_faces, 
    wsp::Vector<int> *o_edge_indices,
    const char *filename, 
    const wsp::graphic::PerspViewParam &view_param,
    double face_depth_thre, 
    int sampling_step, 
    uchar alpha_thre,
    bool invert_depth
){
    double depth;
    int itr_x, itr_y, itr_i;
    //int v1, v2, v3, v4;
    int width, height, length;
    wsp::Image<double> depth_img;
    wsp::State state;
    //WspString wstr;




    // loading depth image
    state=depth_img.Load(filename);
    if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Loading depth image failed\n"); return state; }
    if(invert_depth==false){
        depth_img.Invert();
    }
    depth_img.Normalize(0.0, 1.0);

    width = (int)depth_img.width();
    height =(int)depth_img.height();
    length = width*height;
    double aspect = static_cast<double>(width)/static_cast<double>(height);


    int w_mesh = static_cast<int>(ceil(width/(float)sampling_step));
    int h_mesh = static_cast<int>(ceil(height/(float)sampling_step));


    // initializing arrays for mesh
    o_vtx_array.SetLength(length);
    o_vtx_count_on_face.SetLength(length);
    o_vtx_list_on_faces.SetLength(length*4);
    if(o_edge_indices){
        o_edge_indices->SetLength(((width-1)*height+(height-1)*width) * 2);
    }

    face_depth_thre/=255.0; //! normalize threshold

    double dz;
    double vpos[4][3]; //! buffer for vertex position
    int pix_indices[4]; //! buffer for vertex index 
    int numFaces=0;
    int num_edge=0;
    int numVtx=0;
    int numVtxOnFaces=0;
    wsp::Vector<int> vtx_index_map(width*height);
    u32 i, itr_v, itr_vi, itr_vj;
    double x, y, xstep, ystep;
    double w_2=static_cast<double>(width)/2.0;
    double h_2=static_cast<double>(height)/2.0;
    vtx_index_map.Reset(-1);
    bool skip;
    int step=sampling_step;
    int nc = static_cast<int>(depth_img.num_of_channels());
    double sx, sy, sz, tan_fov_2;//, scale=0.01;
    // set up an array for vertex position
    // set up an array to assign vertices from vexArray to each face
    for(itr_y = 0; itr_y<height-step; itr_y+=step){
        for(itr_x = 0; itr_x<width-step; itr_x+=step){
            if(depth_img.GetAlpha(itr_y*width+itr_x)<alpha_thre){ continue; }

            tan_fov_2 = tan(view_param.field_of_view*0.5);
            sy = - tan_fov_2;
            sx = tan_fov_2 * aspect;
            sz = (view_param.far_clip - view_param.near_clip); // / view_param.far_clip;
            
            // normalize size to be range as -1 to 1
            x = (static_cast<double>(itr_x) - w_2)/w_2;
            y = (static_cast<double>(itr_y) - h_2)/h_2;
            xstep = (static_cast<double>(itr_x + step) - w_2)/w_2;
            ystep = (static_cast<double>(itr_y + step) - h_2)/h_2;

            // indices of pix_indices are like this 
            //**
            //        0------1
            //        |      |
            //        |      |
            //        3------2
            //**
            pix_indices[0]=itr_y*width+itr_x;
            pix_indices[1]=itr_y*width+itr_x+step;
            pix_indices[2]=(itr_y+step)*width+itr_x+step;
            pix_indices[3]=(itr_y+step)*width+itr_x;
            //pix_indices[0]=itr_y*w_mesh+itr_x;
            //pix_indices[1]=pix_indices[0]+step;
            //pix_indices[3]=(itr_y+step)*w_mesh+itr_x;
            //pix_indices[2]=pix_indices[3]+step;

            // z pos
            vpos[0][2] = depth_img[pix_indices[0]*nc]*sz + view_param.near_clip;
            vpos[1][2] = depth_img[pix_indices[1]*nc]*sz + view_param.near_clip;
            vpos[2][2] = depth_img[pix_indices[2]*nc]*sz + view_param.near_clip;
            vpos[3][2] = depth_img[pix_indices[3]*nc]*sz + view_param.near_clip;

            // x and y pos
            if(view_param.field_of_view!=0){
                vpos[0][0] = x * sx * vpos[0][2];//depth_img[pix_indices[0]*nc];
                vpos[0][1] = y * sy * vpos[0][2];//depth_img[pix_indices[0]*nc];

                vpos[1][0] = (xstep) * sx  * vpos[1][2];//* depth_img[pix_indices[1]*nc];
                vpos[1][1] = y * sy * vpos[1][2];//depth_img[pix_indices[1]*nc];

                vpos[2][0] = (xstep) * sx  * vpos[2][2];//* depth_img[pix_indices[2]*nc];    
                vpos[2][1] = (ystep) * sy  * vpos[2][2];//* depth_img[pix_indices[2]*nc];

                vpos[3][0] = x * sx  * vpos[3][2];//* depth_img[pix_indices[3]*nc];
                vpos[3][1] = (ystep) * sy  * vpos[3][2];//* depth_img[pix_indices[3]*nc];    
            }else{
                vpos[0][0] = x;
                vpos[0][1] = y;

                vpos[1][0] = xstep;
                vpos[1][1] = y;

                vpos[2][0] = xstep;    
                vpos[2][1] = ystep;

                vpos[3][0] = x;
                vpos[3][1] = ystep;    
            }


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
                    dz=fabs(depth_img[pix_indices[itr_vi]*nc] - depth_img[pix_indices[itr_vj]*nc]);
                    if(dz>face_depth_thre){ skip=true; break; }
                }
                if(skip){ break; }
            }
            //for(itr_vi=0; itr_vi<4; itr_vi++){
            //    for(itr_vj=itr_vi+1; itr_vj<4; itr_vj++){
            //        dz=fabs(depth_img[vi1[itr_vi]] - depth_img[vi1[itr_vj]]);
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
                    //depth = depth_img[i*nc];
                    //vpos[itr_v][2] = depth_img[i*nc];
                    o_vtx_array[numVtx]=wsp::Point<float>((float)vpos[itr_v][0], (float)vpos[itr_v][1], (float)vpos[itr_v][2]);
                    vtx_index_map[i]=numVtx;
                    ++numVtx;
                }
                // set 4 verteces per face
                o_vtx_list_on_faces[numVtxOnFaces]=vtx_index_map[i];
                ++numVtxOnFaces;
            }
            if(o_edge_indices!=NULL){
                (*o_edge_indices)[num_edge] = pix_indices[3];
                ++num_edge;
                (*o_edge_indices)[num_edge] = pix_indices[2];
                ++num_edge;
                (*o_edge_indices)[num_edge] = pix_indices[1];
                ++num_edge;
                (*o_edge_indices)[num_edge] = pix_indices[2];
                ++num_edge;
                if(itr_x==0){
                    (*o_edge_indices)[num_edge] = pix_indices[0];
                    ++num_edge;
                    (*o_edge_indices)[num_edge] = pix_indices[3];
                    ++num_edge;
                }
                if(itr_y==0){
                    (*o_edge_indices)[num_edge] = pix_indices[0];
                    ++num_edge;
                    (*o_edge_indices)[num_edge] = pix_indices[1];
                    ++num_edge;
                }
            }
            // --------------------------------
            
            // set up an array countaining the number of vertices for each of faces (4 vertices per face)
            o_vtx_count_on_face[numFaces]=4;
            ++numFaces;
        }
    }
    o_vtx_array.SetLength(numVtx);
    //vtx_index_map.SetLength(numVtx);
    o_vtx_list_on_faces.SetLength(numVtxOnFaces);
    o_vtx_count_on_face.SetLength(numFaces);

    return WSP_STATE_SUCCESS;
}
