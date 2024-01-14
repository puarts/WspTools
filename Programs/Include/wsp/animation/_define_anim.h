/**
 * @file _define_anim.h
 * @author Jun-ichi Nishikata
 */

#ifndef _WspAnimDefine_H_
#define _WspAnimDefine_H_

#include <vector>
#include <string>
#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include <wsp/common/_define_commoncore.h>
#endif
using namespace std;

namespace wsp{
/**
 * @brief Collection of functions and classes for WspAnim module
 */
namespace anim{
    enum ChannelType{
        X_ROTATION, Y_ROTATION, Z_ROTATION,
        X_POSITION, Y_POSITION, Z_POSITION,
        X_SCALE,    Y_SCALE,    Z_SCALE
    };
    //! Channel info
    struct Channel{
        ChannelType m_type;
        int m_index;
        Channel(){
            m_index=0;
        }
        Channel(wsp::anim::ChannelType type){
            m_type = type;
        }
    };
}
}
#endif