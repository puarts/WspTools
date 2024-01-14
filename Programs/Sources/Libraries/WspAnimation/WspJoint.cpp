//! WspJoint.cpp
// @author Jun-ichi Nishikata

#include "WspJoint.h"

// Constructor, Destructor ----------------------------------------
#define INIT_WspJoint m_name("DefaultJoint"), m_index(0), m_parent(NULL), m_hasSite(false)
WspJoint::WspJoint()
    :INIT_WspJoint
{
    Init();
}
WspJoint::WspJoint(string name, WspJoint *parent, double offsetX, double offsetY, double offsetZ,
        bool hasSite, double siteX, double siteY, double siteZ)
    :INIT_WspJoint
{
    Init();
    m_parent = parent;
    m_name = name;
    m_offset[0] = offsetX;    m_offset[1] = offsetY;    m_offset[2] = offsetZ;
    m_site[0] = siteX;        m_site[1] = siteY;        m_site[2] = siteZ;
    m_hasSite = hasSite;
}
WspJoint::~WspJoint(){
    int i, nChannels = m_channels.size();
    for(i=0; i<nChannels; i++){ delete m_channels[i]; }
}
// ---------------------------------------- end Constructor, Destructor
