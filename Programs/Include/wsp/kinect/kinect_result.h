/**
 * @file xnstatus_result.h
 * @author Jun-ichi Nishikata
 */

#ifndef WSP_KINECT_KINECT_RESULT_H_
#define WSP_KINECT_KINECT_RESULT_H_

#ifndef __WSP_COMMONCORE_RESULT_H__
#include <wsp/common/result.h>
#endif

/*
 *   Result range for image module: 0x00001000 - 0x00001fff
 */
#ifdef __cplusplus
namespace wsp{ namespace kinect{
    WSP_DEFINE_CPP_RESULT_RANGE( ResultFatalError, 0x00002000, 0x000020ff, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( ResultUnknown, 0x00002001, 0x00002001, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( ResultSkeltonIsUnsupported, 0x00002002, 0x00002002, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( ResultPoseDetectionIsUnsupported, 0x00002003, 0x00002003, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( ResultSkeltonNotTracked, 0x00002004, 0x00002004, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( ResultTargetSkeltonNotActive, 0x00002005, 0x00002005, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( ResultTrackedJointNotConfident, 0x00002005, 0x00002005, WSP_RESULT_LEVEL_FATAL );
}}
#endif




#endif
