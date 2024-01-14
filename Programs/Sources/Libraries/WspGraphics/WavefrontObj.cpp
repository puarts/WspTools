//! WavefrontObj.cpp
//! 


#include <fstream>
//#include <GL/glut.h>
#include "WavefrontObj.h"

// バッファ長
#define  BUFFER_LENGTH  1024


#include <vector>
#include <map>
#include <string>

using namespace std;

class  WavefrontObj
{
public:
    /*  構造体  */

    // 頂点
    struct  Vertex
    {
        double   x, y, z;
    };

    // カラー
    struct  Color
    {
        double   r, g, b;
    };

    struct  Face;

    // グループ
    struct  Group
    {
        string            name;
        vector< int >     faces;
    };

    // マテリアル
    struct  Material
    {
        string            name;
        Color             ambient;
        Color             diffuse;
        Color             specular;
        string            diffuse_map;
        vector< int >     faces;
    };

    // エレメント
    struct  Element
    {
        Group  *    group;
        Material *  material;
    };

    // 頂点
    struct  Face : public Element
    {
        vector< int >   data;

        int  NumVertex() const { return  data.size() / 3; }
        int  GetVertex(int n) const { return  data[n * 3]; }
        int  GetTexture(int n) const { return  data[n * 3 + 1]; }
        int  GetNormal(int n) const { return  data[n * 3 + 2]; }
    };


private:
    /*  内部データ  */
    vector< Group * >     groups;
    vector< Material * >  materials;
    vector< Vertex >      vertices;
    vector< Vertex >      normals;
    vector< Vertex >      t_coords;
    vector< Face * >      faces;

    map< string, Group * >  group_index;
    map< string, Material * >  mtl_index;


public:
    /*  コンストラクタ・デストラクタ  */
    WavefrontObj(const char * file_name);
    ~WavefrontObj();

protected:
    // マテリアルファイルの読み込み
    void  LoadMaterialFile(const char * file_name);

public:
    /*  アクセサ  */
    inline const int  NumGroups() const { return groups.size(); }
    inline const Group* GetGroup(int n) const { return groups[n]; }
    inline const Group* GetGroup(const string & s) const {
        return ((group_index.find(s) != group_index.end()) ? (*group_index.find(s)).second : NULL);
    }
    inline const int  NumMaterials() const { return materials.size(); }
    inline const Material* GetMaterial(int n) const { return materials[n]; }

    inline const int  NumVertices() const { return vertices.size(); }
    inline const Vertex& GetVertex(int n) const { return vertices[n]; }

    inline const int  NumNormals() const { return normals.size(); }
    inline const Vertex& GetNormal(int n) const { return normals[n]; }

    inline const int  NumTextureCoords() const { return t_coords.size(); }
    inline const Vertex& GetTextureCoords(int n) const { return t_coords[n]; }

    inline const int  NumFaces() const { return faces.size(); }
    inline const Face* GetFace(int n) const { return faces[n]; }

    // OpenGLを使用してオブジェクトを描画
    void  Draw();
};


//
//  コンストラクタ（ローダ）
//
WavefrontObj::WavefrontObj( const char * file_name )
{
    ifstream  file;
    char      line[ BUFFER_LENGTH ];
    char *    token;
    char *    data;
    Group *     curr_group = NULL;
    Material *  curr_mtl = NULL;
    vector< int >   face_data;

    // ファイルのオープン
    file.open( file_name, ios::in );
    if ( file.is_open() == 0 )  return; // ファイルが開けなかったら終了

    // ファイルを先頭から１行ずつ順に読み込み
    while ( ! file.eof() )
    {
        // １行読み込み、先頭の単語を取得
        file.getline( line, BUFFER_LENGTH );
        token = strtok( line, " " );

        // 空行・コメント行の場合は次の行へ
        if ( (token == NULL ) || ( *token == '#' ) )
            continue;

        // 点データの読み込み
        if ( *token == 'v' )
        {
            Vertex  v;
            data = strtok( NULL, " " );  v.x = data ? atof( data ) : 0.0;
            data = strtok( NULL, " " );  v.y = data ? atof( data ) : 0.0;
            data = strtok( NULL, " " );  v.z = data ? atof( data ) : 0.0;

            token ++;
            if ( *token == 't' )  
                t_coords.push_back( v );
            else if ( *token == 'n' )  
                normals.push_back( v );
            else  
                vertices.push_back( v );
            continue;
        }

        // 面データの読み込み
        if ( *token == 'f' )
        {
            face_data.clear();
            while ( (data = strtok( NULL, " /" )) != NULL )
            {
                // テクスチャ番号が省略された時（'/' が続いた時）は -1 を設定
                if ( *(data - 1) == '/' )
                    face_data.push_back( -1 );

                // Objファイルでは頂点番号は１から始まっているので -1して０から開始にする
                face_data.push_back( atoi( data ) - 1 );

                // 法線データがない場合は、自動的に法線ベクトル番号に -1 を設定
                if ( ( ( face_data.size() % 3 ) == 2 ) && ( normals.size() == 0 ) )
                    face_data.push_back( face_data[ face_data.size() - 2 ] );
            }

            Face *  face = WSP_NEW Face();
            face->group = curr_group;
            face->material = curr_mtl;
            face->data = face_data;
            faces.push_back( face );

            if ( curr_group )  curr_group->faces.push_back( faces.size() - 1 );
            if ( curr_mtl )  curr_mtl->faces.push_back( faces.size() - 1 );
            continue;
        }

        // グループ名の読み込み
        if ( *token == 'g' )
        {
            data = strtok( NULL, " " );
            if ( data == NULL )  { curr_group = NULL;  continue; }

            map< string, Group * >::iterator  it;
            it = group_index.find( data );
            if ( it != group_index.end() )
                curr_group = (*it).second;
            else
            {
                Group *  group = WSP_NEW Group();
                group->name = data;
                groups.push_back( group );
                group_index[ group->name ] = group;
                curr_group = group;
            }
            continue;
        }

        // マテリアル名の読み込み
        if ( strcmp( token, "usemtl" ) == 0 )
        {
            data = strtok( NULL, " " );
            if ( data == NULL )  { curr_mtl = NULL;  continue; }

            map< string, Material * >::iterator  it;
            it = mtl_index.find( data );
            if ( it != mtl_index.end() )
                curr_mtl = (*it).second;
            else
            {
                Material *  material = WSP_NEW Material();
                material->name = data;
                material->ambient.r = 0.6f;
                material->ambient.g = 0.6f;
                material->ambient.b = 0.6f;
                material->diffuse.r = 0.6f;
                material->diffuse.g = 0.6f;
                material->diffuse.b = 0.6f;
                material->specular.r = 0.1f;
                material->specular.g = 0.1f;
                material->specular.b = 0.1f;
                materials.push_back( material );
                mtl_index[ material->name ] = material;
                curr_mtl = material;
            }
            continue;
        }

        // マテリアルファイルの読み込み
        if ( strcmp( token, "mtllib" ) == 0 )
        {
            data = strtok( NULL, " " );
            if ( data == NULL )  { curr_mtl = NULL;  continue; }

            string  mtl_file_name( data );
            const char *  path = strrchr( file_name, '\\' );
            if ( path != NULL )
            {
                mtl_file_name.assign( file_name, path + 1 );
                mtl_file_name.append( data );
            }

            LoadMaterialFile( mtl_file_name.c_str() );
        }
        
    }
}


//
//  マテリアルファイルの読み込み
//
void WavefrontObj::LoadMaterialFile( const char * file_name )
{
    // 省略（各自作成）
}


//
//  OpenGLを使用してオブジェクトを描画
//
void WavefrontObj::Draw()
{
    // 省略（各自作成）

    int  i, j, no;
    WavefrontObj::Material *curr_mtl = NULL;

//    glBegin( GL_TRIANGLES );
//    for ( i=0; i<faces.size(); i++ )
//    {
//        // マテリアルの切り替え
//        if ( ( materials.size() > 0 ) && ( materials[ i ] != curr_mtl ) )
//        {
//            curr_mtl = materials[ i ];
//            glColor3f( curr_mtl->diffuse.r, curr_mtl->diffuse.g, curr_mtl->diffuse.b );
//        }
//
//        // 三角面の各頂点データの指定
//        for ( j=0; j<faces[i]->NumVertex(); j++ )
//        {
//            // 法線ベクトル
//            const Vertex& vn = normals[faces[i]->GetNormal(j)];
//            glNormal3f( vn.x, vn.y, vn.z );
//
//            // 頂点座標
//            const Vertex& v = vertices[faces[i]->GetVertex(i)];
//            glVertex3f( v.x, v.y, v.z );
//        }
//    }
//    glEnd();
}

