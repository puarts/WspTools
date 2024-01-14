/**
 * @file WspSkeltonHandler.h
 * @author Jun-ichi Nishikata
 */

#ifndef __WspSkeltonHandler_H__
#define __WspSkeltonHandler_H__

#include <vector>
#include <string>
#include <wsp/common/_tpl_fn-array.hpp>
#include "WspJoint.h"

using namespace  std;

class WspSkeltonHandler
{
private:
    //! hierarchal structure info -------
    vector<WspJoint *> m_joints; // 関節情報 [パーツ番号]
    vector<wsp::anim::Channel *> m_channels;

    //! m_motionData data info ----------------
    int m_numFrame; //! number of frames
    double m_interval; //! second per frame
    double *m_motionData; //! [frame][channel number]

private:
    inline void Init(); //! initialize all member variables as default
    inline void Clear(); //! clear all data

public:
    WSP_DLL_EXPORT WspSkeltonHandler();
    WSP_DLL_EXPORT WspSkeltonHandler( const char * bvhFileName );
    WSP_DLL_EXPORT ~WspSkeltonHandler();

    // I/O ----------------------------------------------
    WSP_DLL_EXPORT wsp::State LoadBVH(const char *bvhFileName);
    WSP_DLL_EXPORT wsp::State SaveAsBVH(const char *bvhFileName) const;
    WSP_DLL_EXPORT wsp::State SaveCurrentChannelsToBVH(const char *bvhFileName) const;
    WSP_DLL_EXPORT void PrintJoints() const;
    WSP_DLL_EXPORT void PrintChannels() const;

    // Getter Methods -------------------------
    //! Hierarcal info
    inline int GetNumJoint() const;
    inline int GetNumChannel() const;
    inline int GetNumFrame() const;
    //inline const wsp::anim::Channel* GetChannel( int no ) const;
    inline const char* GetChannelName(wsp::anim::ChannelType type) const;
    inline int GetMotionDataLength() const;

    inline const WspJoint* GetJoint(u32 index) const;
    inline const WspJoint* GetJoint(const string &jointName) const;
    inline const WspJoint* GetJoint(const char *jointName) const;

    //! Motion data
    inline double GetInterval() const;
    inline double GetMotionElem( int frame, int channel ) const;
    inline double GetMotionElem(int frame, int joint, int channel) const;
    inline const double* GetMotionPtr() const;

    // Setter Methods --------------------------
    inline void SetMotion( int frame, int channel, double value );
    inline void SetVector3(double o_vec[3], double v1, double v2, double v3);
    WSP_DLL_EXPORT void AddFrame(int addAmount=1);
    WSP_DLL_EXPORT void CreateDefaultTPoseSkelton();
    WSP_DLL_EXPORT void CreateKinectTPoseSkelton(bool makePositionChannel=0);
    WSP_DLL_EXPORT void ConvertWorldToLocal();


public:
    //! Methods for drowing skelton graphics
    //// 指定フレームの姿勢を描画
    //void  RenderFigure( int frame_no, float scale = 1.0f );
    //// 指定されたBVH骨格・姿勢を描画（クラス関数）
    //static void  RenderFigure( const WspJoint * root, const double * data, float scale = 1.0f );
    //// BVH骨格の１本のリンクを描画（クラス関数）
    //static void  RenderBone( float x0, float y0, float z0, float x1, float y1, float z1 );
};


inline void WspSkeltonHandler::Clear(){
    int i, numJt=(int)m_joints.size();
    for(i=0; i<numJt; i++){
        delete m_joints[i];
    }
    m_joints.clear();
    m_channels.clear();

    delete[] m_motionData;
    m_motionData = NULL;
}
inline void WspSkeltonHandler::Init(){
    Clear();
    m_numFrame = 0;
    m_interval = 1.0/30.0;
}


// Getter Methods ------------------------------------------------------------------
inline int WspSkeltonHandler::GetNumJoint()  const{ return m_joints.size(); }
inline int WspSkeltonHandler::GetNumChannel() const{
    int nChannels = 0;
    for(u32 i=0; i<m_joints.size(); i++){
        nChannels += m_joints[i]->m_channels.size();
    }
    return nChannels;
}
inline int WspSkeltonHandler::GetNumFrame() const{ return  m_numFrame; }
inline double WspSkeltonHandler::GetInterval() const{ return  m_interval; }
inline double WspSkeltonHandler::GetMotionElem(int frame, int channel) const{
    if(frame>=GetNumFrame() || frame<0 || channel>=GetNumChannel() || channel<0){ return 0; }
    return m_motionData[frame*GetNumChannel() + channel];
}
inline double WspSkeltonHandler::GetMotionElem(int frame, int joint, int channel) const{
    if(frame>=GetNumFrame() || frame<0 ||
        joint>=GetNumJoint() || joint<0 ||
        channel>=(int)m_joints[joint]->m_channels.size() || channel<0){ return 0; }
    int index=0;
    for(int i=0; i<joint; i++){
        index += m_joints[i]->m_channels.size();
    }
    return m_motionData[frame*GetNumChannel() + index + channel];
}
inline const double* WspSkeltonHandler::GetMotionPtr() const{ return m_motionData; }
inline int WspSkeltonHandler::GetMotionDataLength() const{
    return GetNumChannel() * GetNumFrame();
}
inline const WspJoint* WspSkeltonHandler::GetJoint( u32 index ) const{
    if(index>=m_joints.size()){ return NULL; }
    return  m_joints[index];
}
inline const WspJoint* WspSkeltonHandler::GetJoint(const string &jointName) const{
    WspJoint *jt=NULL;
    for(u32 i=0; i<m_joints.size(); i++){
        if(m_joints[i]->m_name==jointName){
            jt = m_joints[i];
            break;
        }
    }
    return jt;
}
inline const WspJoint* WspSkeltonHandler::GetJoint(const char *jointName) const{
    return WspSkeltonHandler::GetJoint(string(jointName));
}
inline const char* WspSkeltonHandler::GetChannelName(wsp::anim::ChannelType type) const{
    switch(type){
        case wsp::anim::X_ROTATION:
            return "Xrotation";
        case wsp::anim::Y_ROTATION:
            return "Yrotation";
        case wsp::anim::Z_ROTATION:
            return "Zrotation";
        case wsp::anim::X_POSITION:
            return "Xposition";
        case wsp::anim::Y_POSITION:
            return "Yposition";
        case wsp::anim::Z_POSITION:
            return "Zposition";
    }
    return NULL;
}





// Setter Methods --------------------------------------------------------------------
inline void WspSkeltonHandler::SetMotion(int frame, int channel, double value) {
    m_motionData[frame*GetNumChannel() + channel] = value;
}
inline void WspSkeltonHandler::SetVector3(double o_vec[3], double v1, double v2, double v3){
    o_vec[0] = v1; o_vec[1] = v2; o_vec[2] = v3;
}



#endif
