//! WspJoint.h
// @author Jun-ichi Nishikata

#include <string>
#include <vector>
#include "_define_anim.h"
#include <wsp/common/_ctypes.h>

using namespace std;

class WspJoint
{
public:
    string m_name;
    int m_index;
    double m_offset[3]; //! connection position
    double m_site[3]; //! position of end joint
    WspJoint *m_parent;
    vector<WspJoint *> m_children;
    vector<wsp::anim::Channel *> m_channels; //! rotation axis
    bool m_hasSite; //! flag for end joint or not

private:
    inline void Init();

public:
    WSP_DLL_EXPORT WspJoint();
    WSP_DLL_EXPORT WspJoint(string name, WspJoint *parent=NULL, double offsetX=0, double offsetY=0, double offsetZ=0,
            bool hasSite=false, double siteX=0, double siteY=0, double siteZ=0);
    WSP_DLL_EXPORT ~WspJoint();

    inline void CreateRotationChannels();
    inline void CreatePositionChannels();
    inline void ClearChannels();
};

inline void WspJoint::Init(){
    m_offset[0]=0;    m_offset[1]=0;    m_offset[2]=0;
    m_site[0]=0;      m_site[1]=0;      m_site[2]=0;
    m_channels.clear();
    m_children.clear();
}

inline void WspJoint::ClearChannels(){
    for(u32 i=0; i<m_channels.size(); i++){ delete m_channels[i]; }
    m_channels.clear();
}

inline void WspJoint::CreateRotationChannels(){
    wsp::anim::Channel *channel;

    channel = new wsp::anim::Channel(wsp::anim::X_ROTATION);
    m_channels.push_back(channel);
    channel = new wsp::anim::Channel(wsp::anim::Y_ROTATION);
    m_channels.push_back(channel);
    channel = new wsp::anim::Channel(wsp::anim::Z_ROTATION);
    m_channels.push_back(channel);
}
inline void WspJoint::CreatePositionChannels(){
    wsp::anim::Channel *channel;

    channel = new wsp::anim::Channel(wsp::anim::X_POSITION);
    m_channels.push_back(channel);
    channel = new wsp::anim::Channel(wsp::anim::Y_POSITION);
    m_channels.push_back(channel);
    channel = new wsp::anim::Channel(wsp::anim::Z_POSITION);
    m_channels.push_back(channel);
}