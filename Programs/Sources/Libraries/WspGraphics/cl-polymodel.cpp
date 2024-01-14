/**
 * @file cl-polymodel.cpp
 * 
 */

#include "cl-polymodel.h"

#include <wsp/common/cl-text_stream.h>
#include <wsp/common/_tpl_cl-stackarray.h>
#include <wsp/graphics/fn-graphic_proc.h>

#ifndef __WSP_COMMONUTIL_FN_UTIL_H__
#include <wsp/common/fn-util.h>
#endif

#include <stdio.h>
#include <stdlib.h>


// Constructor, Destructor ----------------------------------------------
#define INIT_WSP_POLYMODEL \
    display_type_(wsp::graphic::POLY_DISPLAY_TRIANGLES),\
    view_mtx_(NULL),\
    use_self_transform_(false)

wsp::graphic::PolygonModel::PolygonModel()
    : INIT_WSP_POLYMODEL
{ 
    Clear(); 
}
wsp::graphic::PolygonModel::PolygonModel(wsp::graphic::TransformMatrix<float> *view_mtx)
    : INIT_WSP_POLYMODEL
{ 
    set_view_mtx(view_mtx); 
}
wsp::graphic::PolygonModel::PolygonModel(char const *filepath_obj)
    : INIT_WSP_POLYMODEL
{
    wsp::graphic::PolygonModel::LoadObjFile(filepath_obj);
}

// Mutator --------------------------------------------------------------
//void wsp::PolygonModel::CreatePlanePrimitive(){
void wsp::graphic::PolygonModel::CreatePrimitive(wsp::graphic::PolyPrimitiveType prim_type)
{
    Clear();
    switch(prim_type)
    {
    case wsp::graphic::POLY_PRIMITIVE_PLANE:
        {
            int len = points_.length();
            float scale = 1.0;
            points_.Append(wsp::Point<float>(wsp::Point<float>(1.0, 0.0, 1.0)*scale));
            points_.Append(wsp::Point<float>(wsp::Point<float>(1.0, 0.0, -1.0)*scale));
            points_.Append(wsp::Point<float>(wsp::Point<float>(-1.0, 0.0, -1.0)*scale));
            points_.Append(wsp::Point<float>(wsp::Point<float>(-1.0, 0.0, 1.0)*scale));

            vtx_point_indices_.Append(0);
            vtx_point_indices_.Append(1);
            vtx_point_indices_.Append(2);
            vtx_point_indices_.Append(3);

            edge_point_indices_<<0<<1;
            edge_point_indices_<<1<<2;
            edge_point_indices_<<2<<3;
            edge_point_indices_<<0<<3;

            switch(display_type_){
            case wsp::graphic::POLY_DISPLAY_WIREFRAME:
            case wsp::graphic::POLY_DISPLAY_TRIANGLES:
                face_point_indices_.Append(len);
                face_point_indices_.Append(len+1);
                face_point_indices_.Append(len+2);

                face_point_indices_.Append(len+3);
                face_point_indices_.Append(len+0);
                face_point_indices_.Append(len+2);

                edge_point_indices_<<1<<3;

                normals_.Append(wsp::Vector3<float>(0.0, 1.0, 0.0));
                normals_.Append(wsp::Vector3<float>(0.0, 1.0, 0.0));
                break;
            case wsp::graphic::POLY_DISPLAY_QUADS:
                face_point_indices_.Append(len);
                face_point_indices_.Append(len+1);
                face_point_indices_.Append(len+2);
                face_point_indices_.Append(len+3);

                normals_.Append(wsp::Vector3<float>(0.0, 1.0, 0.0));
                break;
            default:
                WSP_COMMON_ERROR_LOG("Unknown display type\n");
            }
        }
        break;
    case wsp::graphic::POLY_PRIMITIVE_SPHERE:
        WSP_COMMON_WARNING_LOG("Sphere primitive not has been completed yet\n");
        break;
    case wsp::graphic::POLY_PRIMITIVE_TORUS:
        {
            int blocks = 36;
            int size = (blocks+1)*(blocks+1);
            points_.SetLength(size);
            normals_.SetLength(size);
            uvs_.SetLength(size*2);
            face_point_indices_.SetLength(size*2);
            wsp::graphic::CreateTaurusPoly(
                (float*)points_.data(), 
                (float*)normals_.data(),
                (float*)uvs_.data(),
                (int*)face_point_indices_.data(), 
                //(int*)indices,
                36
            );
        }
        break;
    default:
        WSP_COMMON_ERROR_LOG("Unknow primitive type\n");
    }
}

// I/O ------------------------------------------------------------------
wsp::State wsp::graphic::PolygonModel::LoadDepthMap(
    const char *filename, 
    double face_depth_thre, 
    int samplingStep, 
    uchar alpha_thre, 
    double scale,
    bool invert_depth
)
{
    Clear();

    wsp::Vector<int> vtx_count_list;
    double fov = 0.0;
    double near_clip_ratio = 0.8;
    scale=10;

    face_depth_thre = 255;
    alpha_thre = 0;
    samplingStep = 4;
    float near_pos = near_clip_ratio*scale;
    float far_pos = scale;
    float offset_z = -scale;
    wsp::graphic::PerspViewParam param = { fov, near_pos, far_pos, 0 };
    wsp::State state = wsp::graphic::CreateTriMeshFromDepthMap(
            points_, 
            vtx_count_list, 
            face_point_indices_, 
            &edge_point_indices_,
            &normals_,
            &uvs_,
            filename, 
            param,
            face_depth_thre, 
            samplingStep, 
            alpha_thre, 
            scale, 
            invert_depth,
            0.0f, 0.0f, offset_z
    );
    //wsp::State state = wsp::graphic::CreateQuadMeshFromDepthMap(
    //    points_, 
    //    vtx_count_list, 
    //    face_point_indices_, 
    //    &edge_point_indices_,
    //    filename, 
    //    param,
    //    face_depth_thre, 
    //    samplingStep, 
    //    alpha_thre,
    //    false
    //);

    if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Creating mesh failed\n"); return state; }
    printf("num of points=%d\n", points_.length());
    printf("num of faces=%d\n", vtx_count_list.length());
    printf("num of vtxs on face=%d\n", face_point_indices_.length());

    // set up point indices
    vtx_point_indices_.SetLength(points_.length());
    int len = points_.length();
    for(int i=0; i<len; i++){
        //vertices_[i].index=i;
        vtx_point_indices_[i] = i;
        //points_[i].set_z(0.0f);
    }

    //display_type_ = wsp::graphic::POLY_DISPLAY_TRIANGLES;

    //normals_.SetLength(vtx_count_list.length());
    //uvs_.SetLength(points_.length());
    //uvs_.Reset(0);
    //for (int i = 0; i < face_point_indices_.length(); i += 3) {
    //    const wsp::Point<float> a = points_[face_vertices_[i].index];
    //    const wsp::Point<float> b = points_[face_point_indices_[i+1]];
    //    const wsp::Point<float> c = points_[face_point_indices_[i+2]];

    //    wsp::Point<float> normal = wsp::Point<float>(b-a).GetCrossProduct(c-a);
    //    normal.Normalize(0.0, 1.0);

    //    for (int j = 0; j < 3; ++j){
    //        normals_[face_point_indices_[i + j]] += normal;
    //    }
    //}

    //for (int i = 0; i < normals_.length(); ++i){
    //    normals_[i].Normalize(0.0, 1.0);
    //}
    printf("%s successfully loaded\n", filename);
    return WSP_STATE_SUCCESS;
}

wsp::State wsp::graphic::PolygonModel::LoadObjFile(const char *filepath)
{
    char line[MAX_BUFFER];

    FILE *fp = fopen(filepath, "r");
    if(fp==NULL)
    {
        WSP_COMMON_ERROR_LOG("Can't load %s", filepath);
        return WSP_STATE_FAILURE;
    }

    int len;

    Clear();

    // points for max and min of bounding box
    wsp::Point<float> boundsMin( 1e9, 1e9, 1e9);
    wsp::Point<float> boundsMax(-1e9,-1e9,-1e9);

    while(fgets(line, MAX_BUFFER, fp)!=NULL)
    {
        // skip null line and comment line
        if(strlen(line)==0 || line[0]=='#'){ continue; }

        wsp::TextStream ts(line);
        wsp::String id, vertex;

        // get top id('v', 'f' etc) for what information a line includes
        ts>>id;

        if(id == "v")
        {
            wsp::Point<float> p;
            for (int i = 0; i < 3; ++i) 
            {
                ts>>p[i];
                if( p[i] < boundsMin[i] )
                {
                    boundsMin[i] = p[i];
                }

                if( p[i] > boundsMax[i] )
                {
                    boundsMax[i] = p[i];
                }
            }
            points_.Append(p);
            //vertices_ << wsp::Vertex(p);
        }
        else if (id == "vt")
        {
            // TODO: UV‚ÌŽÀ‘•
        }
        else if(id == "f" || id == "fo")
        {
            wsp::StackArray<int, 4> p;

            while (!ts.AtEnd())
            {
                ts >> vertex;
                const int vertexIndex = atoi(vertex.Split('/', 0).data());
                if (vertexIndex)
                {
                    p.Append(vertexIndex>0? vertexIndex - 1:points_.length() + vertexIndex);
                }
            }

            len = p.length();
            for (int i=0; i<len; ++i)
            {
                const int edgeA = p[i];
                const int edgeB = p[(i + 1) % p.length()];

                if (edgeA < edgeB)
                {
                    edge_point_indices_ << edgeA << edgeB;
                }
            }

            for (int i = 0; i < 3; ++i)
            {
                face_point_indices_ << p[i];
            }

            if (p.length() == 4)
            {
                for (int i = 0; i < 3; ++i)
                {
                    face_point_indices_ << p[(i + 2) % 4];
                }
            }
        }
    }

    const wsp::Point<float> bounds(boundsMax - boundsMin);
    const double scale = 1 / wsp::Max(bounds.x(), bounds.y(), bounds.z());
    vtx_point_indices_.SetLength(points_.length());
    len = points_.length();
    for (int i = 0; i < len; ++i){
        points_[i] = wsp::Point<float>((points_[i] - (boundsMin + bounds * 0.5)) * scale);
        vtx_point_indices_[i] = i;
    }

    normals_.SetLength(points_.length());
    normals_.Reset(wsp::Vector3<float>(0,0,0));
    len = face_point_indices_.length();
    for (int i=0; i<len; i+=3) {
        const wsp::Point<float> a = points_[face_point_indices_[i]];
        const wsp::Point<float> b = points_[face_point_indices_[i+1]];
        const wsp::Point<float> c = points_[face_point_indices_[i+2]];

        wsp::Vector3<float> normal = wsp::Point<float>(b-a).GetCrossProduct(wsp::Vector3<float>(c-a));
        normal.NormalizeLength();

        for (int j = 0; j < 3; ++j){
            normals_[face_point_indices_[i + j]] += normal;
        }
    }

    len = normals_.length();
    for (int i = 0; i<len; ++i){
        normals_[i].NormalizeLength();
    }

    printf("%s successfully loaded\n", filepath);
    fclose(fp);
    return WSP_STATE_SUCCESS;
}
