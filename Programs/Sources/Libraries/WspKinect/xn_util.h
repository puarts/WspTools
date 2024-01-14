/**
 * @file xnstatus_result.h
 * @author Jun-ichi Nishikata
 */

#ifndef WSP_KINECT_XN_UTIL_H_
#define WSP_KINECT_XN_UTIL_H_

#include <wsp/kinect/define_kinect.h>
#ifdef USE_XN

//#include <XnStatusCodes.h>
#include <XnTypes.h>

namespace wsp{ namespace kinect{
    inline const XnChar* GetCalibrationErrorString(XnCalibrationStatus error)
    {
	    switch (error)
	    {
	    case XN_CALIBRATION_STATUS_OK:
		    return "OK";
	    case XN_CALIBRATION_STATUS_NO_USER:
		    return "NoUser";
	    case XN_CALIBRATION_STATUS_ARM:
		    return "Arm";
	    case XN_CALIBRATION_STATUS_LEG:
		    return "Leg";
	    case XN_CALIBRATION_STATUS_HEAD:
		    return "Head";
	    case XN_CALIBRATION_STATUS_TORSO:
		    return "Torso";
	    case XN_CALIBRATION_STATUS_TOP_FOV:
		    return "Top FOV";
	    case XN_CALIBRATION_STATUS_SIDE_FOV:
		    return "Side FOV";
	    case XN_CALIBRATION_STATUS_POSE:
		    return "Pose";
	    default:
		    return "Unknown";
	    }
    }
    inline const XnChar* GetPoseErrorString(XnPoseDetectionStatus error)
    {
	    switch (error)
	    {
	    case XN_POSE_DETECTION_STATUS_OK:
		    return "OK";
	    case XN_POSE_DETECTION_STATUS_NO_USER:
		    return "NoUser";
	    case XN_POSE_DETECTION_STATUS_TOP_FOV:
		    return "Top FOV";
	    case XN_POSE_DETECTION_STATUS_SIDE_FOV:
		    return "Side FOV";
	    case XN_POSE_DETECTION_STATUS_ERROR:
		    return "General error";
	    default:
		    return "Unknown";
	    }
    }
}}

#endif
#endif
