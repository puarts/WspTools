//! WspSkeltonHandler.cpp
// @author Jun-ichi Nishikata


#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


//#ifdef _WIN32
//    #include <windows.h>
//#endif

//#include <gl/glut.h>

#include <wsp/math/fn-math-gpl.h>
#include "WspSkeltonHandler.h"

using namespace std;



// Constructor, Destructor -------------------------------------
#define INIT_WspSkeltonHandler m_motionData(NULL), m_numFrame(0), m_interval(1.0/30.0)

WspSkeltonHandler::WspSkeltonHandler()
    :INIT_WspSkeltonHandler
{
    m_joints.clear();
    Init();
}
WspSkeltonHandler::WspSkeltonHandler( const char * bvhFileName )
    :INIT_WspSkeltonHandler
{
    m_joints.clear();
    Init();
    LoadBVH( bvhFileName );
}
WspSkeltonHandler::~WspSkeltonHandler()
{
    int i, nJoints=m_joints.size();
    for(i=0; i<nJoints; i++){
        delete m_joints[i];
    }
    delete[] m_motionData;
}
// ------------------------------------- end Constructor, Destructor

// Setter ---------------------------------------------------------

void WspSkeltonHandler::ConvertWorldToLocal()
{
    double parentAngle[3], localAngle[3];
    int nFrame=GetNumFrame(), nJoint=GetNumJoint(), nChannel=GetNumChannel();
    WspJoint *jt, *jt_p;

    for(int frame=0; frame<nFrame; frame++){
        for(int joint=0; joint<nJoint; joint++){
            jt = m_joints[joint];
            jt_p = m_joints[joint]->m_parent;
            if(jt->m_channels.size()==3){
                localAngle[0] = GetMotionElem(frame, joint, 0);
                localAngle[1] = GetMotionElem(frame, joint, 1);
                localAngle[2] = GetMotionElem(frame, joint, 2);
            }else{
                localAngle[0] = GetMotionElem(frame, joint, 3);
                localAngle[1] = GetMotionElem(frame, joint, 4);
                localAngle[2] = GetMotionElem(frame, joint, 5);
            }
            //printf("%s | ", jt->m_name.data());
            while(jt_p!=NULL){
                if(jt->m_channels.size()==3){
                    parentAngle[0] = GetMotionElem(frame, jt_p->m_index, 0);
                    parentAngle[1] = GetMotionElem(frame, jt_p->m_index, 1);
                    parentAngle[2] = GetMotionElem(frame, jt_p->m_index, 2);
                }else{
                    parentAngle[0] = GetMotionElem(frame, jt_p->m_index, 3);
                    parentAngle[1] = GetMotionElem(frame, jt_p->m_index, 4);
                    parentAngle[2] = GetMotionElem(frame, jt_p->m_index, 5);
                }
                localAngle[0] -= parentAngle[0];
                localAngle[1] -= parentAngle[1];
                localAngle[2] -= parentAngle[2];
                //printf("%s | ", jt_p->m_name.data());
                if( jt_p->m_index <= jt->m_index){ break; }
                jt = jt_p;
                jt_p = jt->m_parent;
            }
            //printf("\n");
        }
    }
}
void WspSkeltonHandler::AddFrame(int addAmount){
    double *tmp_motion;
    int nChannel = (int)GetNumChannel();
    int lenMotion = (int)GetMotionDataLength();
    if(m_motionData==NULL){
        m_motionData = new double[addAmount*nChannel];
        memset(m_motionData, 0, addAmount*nChannel*sizeof(double));
        m_numFrame = addAmount;
        return;
    }
    tmp_motion = new double[lenMotion];
    wsp::CopyArray<double>(tmp_motion, m_motionData, lenMotion);

    delete[] m_motionData;
    m_numFrame += addAmount;
    m_motionData = new double[nChannel * m_numFrame];
    wsp::CopyArray<double>(m_motionData, tmp_motion, lenMotion);
    memset(m_motionData+lenMotion, 0, addAmount*nChannel*sizeof(double));
    delete[] tmp_motion;
}

void WspSkeltonHandler::CreateKinectTPoseSkelton(bool makePositionChannel)
{
    int i, j, cnt, nChannels, nJoints;
    WspJoint *joint=NULL;

    Init();

    //! 0
    joint = new WspJoint("Torso",NULL,0,0,0);
    joint->CreatePositionChannels();
    joint->CreateRotationChannels();
    m_joints.push_back(joint);

    //! 1 - 3
    joint = new WspJoint("LeftHip",m_joints[0], 5, -15, 0);
    //joint = new WspJoint("LeftHip",m_joints[0]);
    if(makePositionChannel){ joint->CreatePositionChannels(); }
    joint->CreateRotationChannels();
    m_joints.push_back(joint);
    joint = new WspJoint("LeftKnee",m_joints[m_joints.size()-1], 0, -20, 0);
    //joint = new WspJoint("LeftKnee",m_joints[m_joints.size()-1]);
    if(makePositionChannel){ joint->CreatePositionChannels(); }
    joint->CreateRotationChannels();
    m_joints.push_back(joint);
    joint = new WspJoint("LeftFoot",m_joints[m_joints.size()-1], 0, -20, 0, true, 0, -5, 5);
    //joint = new WspJoint("LeftFoot",m_joints[m_joints.size()-1]);
    joint->m_hasSite=true;
    if(makePositionChannel){ joint->CreatePositionChannels(); }
    joint->CreateRotationChannels();
    m_joints.push_back(joint);

    //! 4 - 6
    joint = new WspJoint("RightHip",m_joints[0], -5, -15, 0);
    //joint = new WspJoint("RightHip",m_joints[0]);
    if(makePositionChannel){ joint->CreatePositionChannels(); }
    joint->CreateRotationChannels();
    m_joints.push_back(joint);
    joint = new WspJoint("RightKnee",m_joints[m_joints.size()-1], 0, -20, 0);
    //joint = new WspJoint("RightKnee",m_joints[m_joints.size()-1]);
    if(makePositionChannel){ joint->CreatePositionChannels(); }
    joint->CreateRotationChannels();
    m_joints.push_back(joint);
    joint = new WspJoint("RightFoot",m_joints[m_joints.size()-1], 0, -20, 0, true, 0, -5, 5);
    //joint = new WspJoint("RightFoot",m_joints[m_joints.size()-1]);
    joint->m_hasSite=true;
    if(makePositionChannel){ joint->CreatePositionChannels(); }
    joint->CreateRotationChannels();
    m_joints.push_back(joint);

    //! 7 - 9
    joint = new WspJoint("LeftShoulder",m_joints[0], 6, 20, 0);
    //joint = new WspJoint("LeftShoulder",m_joints[0]);
    if(makePositionChannel){ joint->CreatePositionChannels(); }
    joint->CreateRotationChannels();
    m_joints.push_back(joint);
    joint = new WspJoint("LeftElbow",m_joints[m_joints.size()-1], 20, 0, 0);
    //joint = new WspJoint("LeftElbow",m_joints[m_joints.size()-1]);
    if(makePositionChannel){ joint->CreatePositionChannels(); }
    joint->CreateRotationChannels();
    m_joints.push_back(joint);
    joint = new WspJoint("LeftHand",m_joints[m_joints.size()-1], 20, 0, 0, true, 5, 0, 0);
    //joint = new WspJoint("LeftHand",m_joints[m_joints.size()-1]);
    joint->m_hasSite=true;
    if(makePositionChannel){ joint->CreatePositionChannels(); }
    joint->CreateRotationChannels();
    m_joints.push_back(joint);

    //! 10 - 12
    joint = new WspJoint("RightShoulder",m_joints[0], -6, 20, 0);
    //joint = new WspJoint("RightShoulder",m_joints[0]);
    if(makePositionChannel){ joint->CreatePositionChannels(); }
    joint->CreateRotationChannels();
    m_joints.push_back(joint);
    joint = new WspJoint("RightElbow",m_joints[m_joints.size()-1], -20, 0, 0);
    //joint = new WspJoint("RightElbow",m_joints[m_joints.size()-1]);
    if(makePositionChannel){ joint->CreatePositionChannels(); }
    joint->CreateRotationChannels();
    m_joints.push_back(joint);
    joint = new WspJoint("RightHand",m_joints[m_joints.size()-1], -20, 0, 0, true, -5, 0, 0);
    //joint = new WspJoint("RightHand",m_joints[m_joints.size()-1]);
    joint->m_hasSite=true;
    if(makePositionChannel){ joint->CreatePositionChannels(); }
    joint->CreateRotationChannels();
    m_joints.push_back(joint);

    //! 13 - 14
    joint = new WspJoint("Neck",m_joints[0], 0, 24, 0);
    //joint = new WspJoint("Neck",m_joints[0]);
    if(makePositionChannel){ joint->CreatePositionChannels(); }
    joint->CreateRotationChannels();
    m_joints.push_back(joint);
    joint = new WspJoint("Head",m_joints[m_joints.size()-1], 0, 8, 0, true, 0, 4, 0);
    //joint = new WspJoint("Head",m_joints[m_joints.size()-1]);
    joint->m_hasSite=true;
    if(makePositionChannel){ joint->CreatePositionChannels(); }
    joint->CreateRotationChannels();
    m_joints.push_back(joint);


    //! Set children joints
    m_joints[0]->m_children.push_back(m_joints[1]);
    m_joints[0]->m_children.push_back(m_joints[4]);
    m_joints[0]->m_children.push_back(m_joints[7]);
    m_joints[0]->m_children.push_back(m_joints[10]);
    m_joints[0]->m_children.push_back(m_joints[13]);

    m_joints[1]->m_children.push_back(m_joints[2]);
    m_joints[2]->m_children.push_back(m_joints[3]);

    m_joints[4]->m_children.push_back(m_joints[5]);
    m_joints[5]->m_children.push_back(m_joints[6]);

    m_joints[7]->m_children.push_back(m_joints[8]);
    m_joints[8]->m_children.push_back(m_joints[9]);

    m_joints[10]->m_children.push_back(m_joints[11]);
    m_joints[11]->m_children.push_back(m_joints[12]);

    m_joints[13]->m_children.push_back(m_joints[14]);

    //! Set index for each joint and channels
    nJoints = m_joints.size();
    cnt = 0;
    for(i=0; i<nJoints; i++){
        m_joints[i]->m_index = i;
        nChannels = m_joints[i]->m_channels.size();
        for(j=0; j<nChannels; j++){
            m_joints[i]->m_channels[j]->m_index = cnt;
            m_channels.push_back(m_joints[i]->m_channels[j]);
            cnt++;
        }
    }


    //! Set motion
    AddFrame(1);
}


void WspSkeltonHandler::CreateDefaultTPoseSkelton()
{
    int i, j, cnt, nChannels, nJoints;
    WspJoint* joint=NULL;

    Init();

    //! 0
    joint = new WspJoint("Hips",NULL,0,0,0);
    joint->CreatePositionChannels();
    joint->CreateRotationChannels();
    m_joints.push_back(joint);

    //! 1 - 3
    joint = new WspJoint("LeftHip",m_joints[0], 5, -15, 0);
    joint->CreateRotationChannels();
    m_joints.push_back(joint);
    joint = new WspJoint("LeftKnee",m_joints[m_joints.size()-1], 0, -20, 0);
    joint->CreateRotationChannels();
    m_joints.push_back(joint);
    joint = new WspJoint("LeftAnkle",m_joints[m_joints.size()-1], 0, -20, 0, true, 0, -5, 5);
    joint->CreateRotationChannels();
    m_joints.push_back(joint);

    //! 4 - 6
    joint = new WspJoint("RightHip",m_joints[0], -5, -15, 0);
    joint->CreateRotationChannels();
    m_joints.push_back(joint);
    joint = new WspJoint("RightKnee",m_joints[m_joints.size()-1], 0, -20, 0);
    joint->CreateRotationChannels();
    m_joints.push_back(joint);
    joint = new WspJoint("RightAnkle",m_joints[m_joints.size()-1], 0, -20, 0, true, 0, -5, 5);
    joint->CreateRotationChannels();
    m_joints.push_back(joint);

    //! 7
    joint = new WspJoint("Chest",m_joints[0], 0, 5, 0);
    joint->CreateRotationChannels();
    m_joints.push_back(joint);

    //! 8 - 11
    joint = new WspJoint("LeftCollar",m_joints[7], 1, 5, 0);
    joint->CreateRotationChannels();
    m_joints.push_back(joint);
    joint = new WspJoint("LeftShoulder",m_joints[m_joints.size()-1], 6, 0, 0);
    joint->CreateRotationChannels();
    m_joints.push_back(joint);
    joint = new WspJoint("LeftElbow",m_joints[m_joints.size()-1], 20, 0, 0);
    joint->CreateRotationChannels();
    m_joints.push_back(joint);
    joint = new WspJoint("LeftWrist",m_joints[m_joints.size()-1], 20, 0, 0, true, 5, 0, 0);
    joint->CreateRotationChannels();
    m_joints.push_back(joint);

    //! 12 - 15
    joint = new WspJoint("RightCollar",m_joints[7], -1, 5, 0);
    joint->CreateRotationChannels();
    m_joints.push_back(joint);
    joint = new WspJoint("RightShoulder",m_joints[m_joints.size()-1], -6, 0, 0);
    joint->CreateRotationChannels();
    m_joints.push_back(joint);
    joint = new WspJoint("RightElbow",m_joints[m_joints.size()-1], -20, 0, 0);
    joint->CreateRotationChannels();
    m_joints.push_back(joint);
    joint = new WspJoint("RightWrist",m_joints[m_joints.size()-1], -20, 0, 0, true, -5, 0, 0);
    joint->CreateRotationChannels();
    m_joints.push_back(joint);

    //! 16 - 17
    joint = new WspJoint("Neck",m_joints[7], 0, 4, 0);
    joint->CreateRotationChannels();
    m_joints.push_back(joint);
    joint = new WspJoint("Head",m_joints[m_joints.size()-1], 0, 20, 0, true, 0, 4, 0);
    joint->CreateRotationChannels();
    m_joints.push_back(joint);


    //! Set children joints
    m_joints[0]->m_children.push_back(m_joints[1]);
    m_joints[0]->m_children.push_back(m_joints[4]);
    m_joints[0]->m_children.push_back(m_joints[7]);

    m_joints[1]->m_children.push_back(m_joints[2]);
    m_joints[2]->m_children.push_back(m_joints[3]);

    m_joints[4]->m_children.push_back(m_joints[5]);
    m_joints[5]->m_children.push_back(m_joints[6]);

    m_joints[7]->m_children.push_back(m_joints[8]);
    m_joints[7]->m_children.push_back(m_joints[12]);
    m_joints[7]->m_children.push_back(m_joints[16]);

    m_joints[8]->m_children.push_back(m_joints[9]);
    m_joints[9]->m_children.push_back(m_joints[10]);
    m_joints[10]->m_children.push_back(m_joints[11]);

    m_joints[12]->m_children.push_back(m_joints[13]);
    m_joints[13]->m_children.push_back(m_joints[14]);
    m_joints[14]->m_children.push_back(m_joints[15]);

    m_joints[16]->m_children.push_back(m_joints[17]);

    //! Set index for each joint and channels
    nJoints = m_joints.size();
    cnt = 0;
    for(i=0; i<nJoints; i++){
        m_joints[i]->m_index = i;
        nChannels = m_joints[i]->m_channels.size();
        for(j=0; j<nChannels; j++){
            m_joints[i]->m_channels[j]->m_index = cnt;
            m_channels.push_back(m_joints[i]->m_channels[j]);
            cnt++;
        }
    }

    //! Set motion
    AddFrame(1);
}


// --------------------------------------------------------- end Setter




//! Methods for drawing skelton and poses --------------------------------------------
//// 指定フレームの姿勢を描画
//void  WspSkeltonHandler::RenderFigure( int frame_no, float scale )
//{
//    // BVH骨格・姿勢を指定して描画
//    RenderFigure( m_joints[ 0 ], m_motionData + frame_no * GetNumChannel(), scale );
//}
//
//
//// 指定されたBVH骨格・姿勢を描画（クラス関数）
//void  WspSkeltonHandler::RenderFigure( const WspJoint * joint, const double * data, float scale )
//{
//    glPushMatrix();
//
//    // ルート関節の場合は平行移動を適用
//    if ( joint->m_parent == NULL )
//    {
//        glTranslatef( data[ 0 ] * scale, data[ 1 ] * scale, data[ 2 ] * scale );
//    }
//    // 子関節の場合は親関節からの平行移動を適用
//    else
//    {
//        glTranslatef( joint->m_offset[ 0 ] * scale, joint->m_offset[ 1 ] * scale, joint->m_offset[ 2 ] * scale );
//    }
//
//    // 親関節からの回転を適用（ルート関節の場合はワールド座標からの回転）
//    int  i, j;
//    for ( i=0; i<joint->m_channels.size(); i++ )
//    {
//        wsp::anim::Channel *  channel = joint->m_channels[ i ];
//        if ( channel->m_type == wsp::anim::X_ROTATION )
//            glRotatef( data[ channel->m_index ], 1.0f, 0.0f, 0.0f );
//        else if ( channel->m_type == wsp::anim::Y_ROTATION )
//            glRotatef( data[ channel->m_index ], 0.0f, 1.0f, 0.0f );
//        else if ( channel->m_type == wsp::anim::Z_ROTATION )
//            glRotatef( data[ channel->m_index ], 0.0f, 0.0f, 1.0f );
//    }
//
//    // リンクを描画
//    // 関節座標系の原点から末端点へのリンクを描画
//    if ( joint->m_children.size() == 0 )
//    {
//        RenderBone( 0.0f, 0.0f, 0.0f, joint->m_site[ 0 ] * scale, joint->m_site[ 1 ] * scale, joint->m_site[ 2 ] * scale );
//    }
//    // 関節座標系の原点から次の関節への接続位置へのリンクを描画
//    if ( joint->m_children.size() == 1 )
//    {
//        WspJoint *  child = joint->m_children[ 0 ];
//        RenderBone( 0.0f, 0.0f, 0.0f, child->m_offset[ 0 ] * scale, child->m_offset[ 1 ] * scale, child->m_offset[ 2 ] * scale );
//    }
//    // 全関節への接続位置への中心点から各関節への接続位置へ円柱を描画
//    if ( joint->m_children.size() > 1 )
//    {
//        // 原点と全関節への接続位置への中心点を計算
//        float  center[ 3 ] = { 0.0f, 0.0f, 0.0f };
//        for ( i=0; i<joint->m_children.size(); i++ )
//        {
//            WspJoint *  child = joint->m_children[ i ];
//            center[ 0 ] += child->m_offset[ 0 ];
//            center[ 1 ] += child->m_offset[ 1 ];
//            center[ 2 ] += child->m_offset[ 2 ];
//        }
//        center[ 0 ] /= joint->m_children.size() + 1;
//        center[ 1 ] /= joint->m_children.size() + 1;
//        center[ 2 ] /= joint->m_children.size() + 1;
//
//        // 原点から中心点へのリンクを描画
//        RenderBone(    0.0f, 0.0f, 0.0f, center[ 0 ] * scale, center[ 1 ] * scale, center[ 2 ] * scale );
//
//        // 中心点から次の関節への接続位置へのリンクを描画
//        for ( i=0; i<joint->m_children.size(); i++ )
//        {
//            WspJoint *  child = joint->m_children[ i ];
//            RenderBone(    center[ 0 ] * scale, center[ 1 ] * scale, center[ 2 ] * scale,
//                child->m_offset[ 0 ] * scale, child->m_offset[ 1 ] * scale, child->m_offset[ 2 ] * scale );
//        }
//    }
//
//    // 子関節に対して再帰呼び出し
//    for ( i=0; i<joint->m_children.size(); i++ )
//    {
//        RenderFigure( joint->m_children[ i ], data, scale );
//    }
//
//    glPopMatrix();
//}
//
//
//// BVH骨格の１本のリンクを描画（クラス関数）
//void WspSkeltonHandler::RenderBone( float x0, float y0, float z0, float x1, float y1, float z1 )
//{
//    // 与えられた２点を結ぶ円柱を描画
//
//    // 円柱の２端点の情報を原点・向き・長さの情報に変換
//    GLdouble  dir_x = x1 - x0;
//    GLdouble  dir_y = y1 - y0;
//    GLdouble  dir_z = z1 - z0;
//    GLdouble  bone_length = sqrt( dir_x*dir_x + dir_y*dir_y + dir_z*dir_z );
//
//    // 描画パラメタの設定
//    static GLUquadricObj *  quad_obj = NULL;
//    if ( quad_obj == NULL )
//        quad_obj = gluNewQuadric();
//    gluQuadricDrawStyle( quad_obj, GLU_FILL );
//    gluQuadricNormals( quad_obj, GLU_SMOOTH );
//
//    glPushMatrix();
//
//    // 平行移動を設定
//    glTranslated( x0, y0, z0 );
//
//    // 以下、円柱の回転を表す行列を計算
//
//    // ｚ軸を単位ベクトルに正規化
//    double  length;
//    length = sqrt( dir_x*dir_x + dir_y*dir_y + dir_z*dir_z );
//    if ( length < 0.0001 ) { 
//        dir_x = 0.0; dir_y = 0.0; dir_z = 1.0;  length = 1.0;
//    }
//    dir_x /= length;  dir_y /= length;  dir_z /= length;
//
//    // 基準とするｙ軸の向きを設定
//    GLdouble  up_x, up_y, up_z;
//    up_x = 0.0;
//    up_y = 1.0;
//    up_z = 0.0;
//
//    // ｚ軸とｙ軸の外積からｘ軸の向きを計算
//    double  side_x, side_y, side_z;
//    side_x = up_y * dir_z - up_z * dir_y;
//    side_y = up_z * dir_x - up_x * dir_z;
//    side_z = up_x * dir_y - up_y * dir_x;
//
//    // ｘ軸を単位ベクトルに正規化
//    length = sqrt( side_x*side_x + side_y*side_y + side_z*side_z );
//    if ( length < 0.0001 ) {
//        side_x = 1.0; side_y = 0.0; side_z = 0.0;  length = 1.0;
//    }
//    side_x /= length;  side_y /= length;  side_z /= length;
//
//    // ｚ軸とｘ軸の外積からｙ軸の向きを計算
//    up_x = dir_y * side_z - dir_z * side_y;
//    up_y = dir_z * side_x - dir_x * side_z;
//    up_z = dir_x * side_y - dir_y * side_x;
//
//    // 回転行列を設定
//    GLdouble  m[16] = { side_x, side_y, side_z, 0.0,
//                        up_x,   up_y,   up_z,   0.0,
//                        dir_x,  dir_y,  dir_z,  0.0,
//                        0.0,    0.0,    0.0,    1.0 };
//    glMultMatrixd( m );
//
//    // 円柱の設定
//    GLdouble radius= 0.01; // 円柱の太さ
//    GLdouble slices = 8.0; // 円柱の放射状の細分数（デフォルト12）
//    GLdouble stack = 3.0;  // 円柱の輪切りの細分数（デフォルト１）
//
//    // 円柱を描画
//    gluCylinder( quad_obj, radius, radius, bone_length, slices, stack ); 
//
//    glPopMatrix();
//}



// I/O -------------------------------------------------------------------
wsp::State WspSkeltonHandler::LoadBVH(const char * bvhFileName)
{
    double x, y ,z;
    int i, j;
    int numChannel;
    FILE *fp;
    WspJoint *joint=NULL, *new_joint=NULL;
    vector<WspJoint *> joint_stack;
    char line[MAX_BUFFER];
    char *token;
    char separater[] = " :,\t:,\n";
    bool is_site = false;

    printf("Loading %s\n", bvhFileName);

    //! init
    Init();
    delete[] m_motionData;
    m_motionData = NULL;
    numChannel = 0;

    fp = fopen(bvhFileName, "r");
    if(fp == NULL){ WSP_COMMON_ERROR_LOG("Opening %s failed\n", bvhFileName); return WSP_STATE_FAILURE; }
    if(feof(fp)){ fclose(fp); WSP_COMMON_ERROR_LOG("file is empty\n"); return WSP_STATE_FAILURE; } 

    //! Loading hierarcal info ----------------------------------------
    while(feof(fp)==0)
    {
        if(feof(fp)){ fclose(fp); WSP_COMMON_ERROR_LOG("Motion data is not defined\n"); return WSP_STATE_FAILURE; }

        //! read line and get a first word
        fgets(line, MAX_BUFFER, fp);
        token = strtok(line, separater);
        if(token == NULL){ continue; }
        printf("%s", token);

        //! Starting joint block process, push current joint from stack
        if(strcmp( token, "{" ) == 0){
            joint_stack.push_back( joint );
            joint = new_joint;
            continue;
        }
        // end joint block process, pop current joint from stack
        if(strcmp( token, "}" ) == 0){
            joint = joint_stack.back();
            joint_stack.pop_back();
            is_site = false;
            continue;
        }

        //! Read joint info
        if ( ( strcmp( token, "ROOT" ) == 0 ) ||
             ( strcmp( token, "JOINT" ) == 0 ) )
        {
            new_joint = new WspJoint();
            new_joint->m_index = m_joints.size();
            new_joint->m_parent = joint;
            m_joints.push_back( new_joint );
            if ( joint!=NULL ){
                joint->m_children.push_back( new_joint );
            }

            //! read joint name
            token = strtok(NULL, "");
            while( *token==' '){ token++; }
            new_joint->m_name = token;

            //! add new joint to index
            //m_jointIndex[ new_joint->m_name ] = new_joint;
            continue;
        }

        // end joint process
        if(( strcmp( token, "End" ) == 0 )){
            new_joint = joint;
            is_site = true;
            continue;
        }

        //! Offset or site information
        if( strcmp( token, "OFFSET" ) == 0 )
        {
            //! read coordinations
            token = strtok( NULL, separater );
            x = token ? atof( token ) : 0.0;
            token = strtok( NULL, separater );
            y = token ? atof( token ) : 0.0;
            token = strtok( NULL, separater );
            z = token ? atof( token ) : 0.0;
            
            //! Set offset to joint offset or site
            if(is_site){
                joint->m_hasSite = true;
                joint->m_site[0]=x;   joint->m_site[1]=y;   joint->m_site[2]=z;
            }else{
                joint->m_offset[0]=x; joint->m_offset[1]=y; joint->m_offset[2]=z;
            }
            continue;
        }

        //! Read channels
        if ( strcmp( token, "CHANNELS" ) == 0 )
        {
            //! Get number of channels
            token = strtok( NULL, separater );
            joint->m_channels.resize( token ? atoi( token ) : 0 );

            //! Get chanenl info
            for ( i=0; i<joint->m_channels.size(); i++ )
            {
                wsp::anim::Channel * channel = new wsp::anim::Channel();
                //channel->joint = joint;
                channel->m_index = numChannel;
                numChannel++;
                //channel->m_index = m_channels.size();
                //m_channels.push_back( channel );
                joint->m_channels[i] = channel;

                //! Get channel type
                token = strtok( NULL, separater );
                if     (strcmp( token, "Xrotation" ) == 0){ channel->m_type = wsp::anim::X_ROTATION; }
                else if(strcmp( token, "Yrotation" ) == 0){ channel->m_type = wsp::anim::Y_ROTATION; }
                else if(strcmp( token, "Zrotation" ) == 0){ channel->m_type = wsp::anim::Z_ROTATION; }
                else if(strcmp( token, "Xposition" ) == 0){ channel->m_type = wsp::anim::X_POSITION; }
                else if(strcmp( token, "Yposition" ) == 0){ channel->m_type = wsp::anim::Y_POSITION; }
                else if(strcmp( token, "Zposition" ) == 0){ channel->m_type = wsp::anim::Z_POSITION; }
            }
        }

        if (strcmp(token, "MOTION")==0){ break; }
    }
    // ---------------------------------------- end Loading hierarcal info 


    //! Reading motion data -------------------------------------------
    //! Number of frames
    fgets(line, MAX_BUFFER, fp);
    token = strtok( line, separater );
    if(strcmp( token, "Frames" ) != 0){goto BVH_LOAD_ERROR; }
    token = strtok( NULL, separater );
    if(token == NULL){ goto BVH_LOAD_ERROR; }
    m_numFrame = atoi( token );

    //! Second per frame
    fgets(line, MAX_BUFFER, fp);
    token = strtok( line, ":" );
    if(strcmp( token, "Frame Time" ) != 0 ){ goto BVH_LOAD_ERROR; }
    token = strtok( NULL, separater );
    if(token == NULL){ goto BVH_LOAD_ERROR; }
    m_interval = atof( token );

    int channelNum = GetNumChannel();

    m_motionData = new double[ m_numFrame * channelNum ];

    //! motion data
    for ( i=0; i<m_numFrame; i++ ){
        fgets(line, MAX_BUFFER, fp);
        token = strtok( line, separater );
        for ( j=0; j<channelNum; j++ ){
            if( token == NULL){ goto BVH_LOAD_ERROR; }
            m_motionData[i*channelNum + j] = atof(token);
            token = strtok(NULL, separater);
        }
    }
    // ------------------------------------------- end Reading motion data 
    
    fclose(fp);
    printf("Loading BVH file has successfully done\n");
    return WSP_STATE_SUCCESS;

BVH_LOAD_ERROR:
    fclose(fp);
    WSP_COMMON_ERROR_LOG("Loading motion failed\n");
    return WSP_STATE_FAILURE;
}

wsp::State WspSkeltonHandler::SaveAsBVH(const char *bvhFileName) const
{
    FILE *fp;
    int i, j, ch, frame; //! iterators
    int nJoints, nChannels;
    vector<int> jtIndex_stack;
    vector<int> childrens;
    char fmt[128];
    int indent;

    printf("Saving %s..\n", bvhFileName);
    
    nJoints = (int)m_joints.size();

    fp=fopen(bvhFileName, "w");
    if (fp == NULL){ WSP_COMMON_ERROR_LOG("Opening %s failed\n", bvhFileName); return WSP_STATE_FAILURE; }

    //! Write Hierarchal Info -------------------------
    indent=0;
    fprintf(fp, "HIERARCHY\n");
    for(i=0; i<nJoints; i++)
    {
        fprintf(fp, "    ");
        if(i!=0){
            fprintf(fp, "JOINT %s\n", m_joints[i]->m_name.data());
        }else{
            fprintf(fp, "ROOT %s\n", m_joints[i]->m_name.data());
        }
        fprintf(fp, "    ");
        fprintf(fp, "{\n");
        jtIndex_stack.push_back(i);
        childrens.push_back(m_joints[i]->m_children.size());

        //! Calculate indent size
        indent = jtIndex_stack.size() * INDENT_SPACES;

        fprintf(fp, "    ");
        fprintf(fp, "OFFSET %f %f %f\n", m_joints[i]->m_offset[0], m_joints[i]->m_offset[1], m_joints[i]->m_offset[2]);
        nChannels=(int)m_joints[i]->m_channels.size();
        fprintf(fp, "    ");
        fprintf(fp, "CHANNELS %d", nChannels);
        for(j=0; j<nChannels; j++){
            fprintf(fp, " %s", GetChannelName(m_joints[i]->m_channels[j]->m_type));
        }
        fprintf(fp,"\n");
        if(m_joints[i]->m_hasSite){
            fprintf(fp, "    ");
            fprintf(fp, "End Site\n");
            fprintf(fp, "    ");
            fprintf(fp, "{\n");
            fprintf(fp, "    ");
            fprintf(fp, "OFFSET %f %f %f\n", m_joints[i]->m_site[0], m_joints[i]->m_site[1], m_joints[i]->m_site[2]);
            fprintf(fp, "    ");
            fprintf(fp, "}\n");

            //! Draw closing tag "}" --------------------
            while(true)
            {
                if(childrens.size()==0){ break; }

                childrens[childrens.size()-1]--;
                if(childrens[childrens.size()-1]>0){ break; }

                jtIndex_stack.pop_back();
                childrens.pop_back();
                indent = jtIndex_stack.size() * INDENT_SPACES;
                fprintf(fp, "    ");
                fprintf(fp, "}\n");
            }
            // -------------------- end Draw closing tag
        }
    }
    // ------------------------- end Write Hierarchal Info

    //! Write Motion data ---------------------------------
    fprintf(fp, "MOTION\nFrames: %d\nFrame Time: %f\n", m_numFrame, m_interval);
    nChannels = GetNumChannel();
    for(frame=0; frame<m_numFrame; frame++){
        for(ch=0; ch<nChannels; ch++){
            fprintf(fp, "%f ", m_motionData[frame*nChannels + ch]);
        }
        fprintf(fp, "\n");
    }
    // --------------------------------- end Write Motion data
    fclose(fp);
    printf("Saving motion as BVH has successfully done\n");
    return WSP_STATE_SUCCESS;
}

wsp::State WspSkeltonHandler::SaveCurrentChannelsToBVH(const char *fileName) const
{
    int frame, ch;
    int nChannels;
    FILE *fp;
    fp = fopen(fileName, "a");
    if (fp == NULL){ WSP_COMMON_ERROR_LOG("Opening %s failed\n", fileName); return WSP_STATE_FAILURE; }

    //! Write Motion data ---------------------------------
    nChannels = GetNumChannel();
    for(frame=0; frame<m_numFrame; frame++){
        for(ch=0; ch<nChannels; ch++){
            fprintf(fp, "%f ", m_motionData[frame*nChannels + ch]);
        }
        fprintf(fp, "\n");
    }
    return WSP_STATE_SUCCESS;
}


void WspSkeltonHandler::PrintJoints() const
{
    u32 i;
    u32 nJoints = (u32)m_joints.size();
    for(i=0;i<nJoints;i++){
        cout<<m_joints[i]->m_name<<endl;
    }
}
void WspSkeltonHandler::PrintChannels() const
{
    u32 i, j;
    u32 nJoints, nChannels;
    nJoints = (u32)GetNumJoint();
    for(i=0;i<nJoints;i++){
        nChannels = m_joints[i]->m_channels.size();
        for(j=0; j<nChannels; j++){
            cout<<m_joints[i]->m_channels[j]->m_index<<" ";
            cout<<m_joints[i]->m_channels[j]->m_type<<endl;
        }
    }
}
