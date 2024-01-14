/**
 * @file cl-kinect_handler.h
 * @author Jun-ichi Nishikata
 * @brief class for handling Kinect.
 */

#include "cl-kinect_handler.h"

#ifdef USE_XN
#include "xnstatus_result.h"
#include "kinect_result.h"

#include <wsp/common/fn-debug.h>

wsp::KinectHandler *wsp::KinectHandler::instance_ = NULL;

wsp::KinectHandler& wsp::KinectHandler::GetInstance()
{
    if( instance_ == NULL )
    {
        instance_ = new wsp::KinectHandler();
        WSP_FATAL_IF_RESULT_FAILED( instance_->Initialize(), "Initializing KinectHandler failed");
    }
    return *instance_;
}

void wsp::KinectHandler::DestroyInstance()
{
    if( instance_ != NULL )
    {
        instance_->Finalize();
    }
    delete instance_;
}

#define XN_CALIBRATION_FILE_NAME "UserCalibration.bin"
#define SAMPLE_XML_PATH "./SamplesConfig.xml"

#define CHECK_RC(nRetVal, what)										\
	if (nRetVal != XN_STATUS_OK)									\
	{																\
		printf("%s failed: %s\n", what, xnGetStatusString(nRetVal));\
		return wsp::kinect::MakeResultFromXnStatus(nRetVal);\
	}


XnBool wsp::KinectHandler::need_pose_ = FALSE;
XnChar wsp::KinectHandler::pose_str_[20] = {0};
xn::UserGenerator wsp::KinectHandler::user_generator_;
std::map<XnUInt32, std::pair<XnCalibrationStatus, XnPoseDetectionStatus> > wsp::KinectHandler::error_ = std::map<XnUInt32, std::pair<XnCalibrationStatus, XnPoseDetectionStatus> >();

void wsp::KinectHandler::WaitUpdate()
{
    context_.WaitOneUpdateAll(user_generator_);
}

// Callback: New user was detected
void XN_CALLBACK_TYPE wsp::KinectHandler::User_NewUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/)
{
	XnUInt32 epochTime = 0;
	xnOSGetEpochTime(&epochTime);
	printf("%d New User %d\n", epochTime, nId);
	// New user found
	if (need_pose_)
	{
		user_generator_.GetPoseDetectionCap().StartPoseDetection(pose_str_, nId);
	}
	else
	{
		user_generator_.GetSkeletonCap().RequestCalibration(nId, TRUE);
	}
}
// Callback: An existing user was lost
void XN_CALLBACK_TYPE wsp::KinectHandler::User_LostUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/)
{
	XnUInt32 epochTime = 0;
	xnOSGetEpochTime(&epochTime);
	printf("%d Lost user %d\n", epochTime, nId);	
}
// Callback: Detected a pose
void XN_CALLBACK_TYPE wsp::KinectHandler::UserPose_PoseDetected(xn::PoseDetectionCapability& /*capability*/, const XnChar* strPose, XnUserID nId, void* /*pCookie*/)
{
	XnUInt32 epochTime = 0;
	xnOSGetEpochTime(&epochTime);
	printf("%d Pose %s detected for user %d\n", epochTime, strPose, nId);
	user_generator_.GetPoseDetectionCap().StopPoseDetection(nId);
	user_generator_.GetSkeletonCap().RequestCalibration(nId, TRUE);
}
// Callback: Started calibration
void XN_CALLBACK_TYPE wsp::KinectHandler::UserCalibration_CalibrationStart(xn::SkeletonCapability& /*capability*/, XnUserID nId, void* /*pCookie*/)
{
	XnUInt32 epochTime = 0;
	xnOSGetEpochTime(&epochTime);
	printf("%d Calibration started for user %d\n", epochTime, nId);
}
// Callback: Finished calibration
void XN_CALLBACK_TYPE wsp::KinectHandler::UserCalibration_CalibrationComplete(xn::SkeletonCapability& /*capability*/, XnUserID nId, XnCalibrationStatus eStatus, void* /*pCookie*/)
{
	XnUInt32 epochTime = 0;
	xnOSGetEpochTime(&epochTime);
	if (eStatus == XN_CALIBRATION_STATUS_OK)
	{
		// Calibration succeeded
		printf("%d Calibration complete, start tracking user %d\n", epochTime, nId);		
		user_generator_.GetSkeletonCap().StartTracking(nId);
	}
	else
	{
		// Calibration failed
		printf("%d Calibration failed for user %d\n", epochTime, nId);
        if(eStatus==XN_CALIBRATION_STATUS_MANUAL_ABORT)
        {
            printf("Manual abort occured, stop attempting to calibrate!");
            return;
        }
		if (need_pose_)
		{
			user_generator_.GetPoseDetectionCap().StartPoseDetection(pose_str_, nId);
		}
		else
		{
			user_generator_.GetSkeletonCap().RequestCalibration(nId, TRUE);
		}
	}
}



void XN_CALLBACK_TYPE wsp::KinectHandler::MyCalibrationInProgress(xn::SkeletonCapability& /*capability*/, XnUserID id, XnCalibrationStatus calibrationError, void* /*pCookie*/)
{
	error_[id].first = calibrationError;
}
void XN_CALLBACK_TYPE wsp::KinectHandler::MyPoseInProgress(xn::PoseDetectionCapability& /*capability*/, const XnChar* /*strPose*/, XnUserID id, XnPoseDetectionStatus poseError, void* /*pCookie*/)
{
	error_[id].second = poseError;
}




wsp::KinectHandler::KinectHandler()
{
}

wsp::Result wsp::KinectHandler::Initialize()
{
    XnStatus nRetVal = XN_STATUS_OK;
    xn::EnumerationErrors errors;

    nRetVal = context_.InitFromXmlFile(SAMPLE_XML_PATH, script_node_, &errors);
	if (nRetVal == XN_STATUS_NO_NODE_PRESENT)
	{
		XnChar strError[1024];
		errors.ToString(strError, 1024);
		printf("%s\n", strError);
        return wsp::kinect::MakeResultFromXnStatus(nRetVal);
	}
	else if (nRetVal != XN_STATUS_OK)
	{
		printf("Open %s failed: %s\n", SAMPLE_XML_PATH, xnGetStatusString(nRetVal));
		return wsp::kinect::MakeResultFromXnStatus(nRetVal);
	}

    nRetVal = context_.FindExistingNode(::XN_NODE_TYPE_IMAGE, image_generator_);
    if(nRetVal != XN_STATUS_OK)
    {
        fprintf(stderr, "XN_NODE_TYPE_IMAGE context was not found.\n");
        return wsp::kinect::MakeResultFromXnStatus(nRetVal);
    }

	nRetVal = context_.FindExistingNode(XN_NODE_TYPE_DEPTH, depth_generator_);
	if (nRetVal != XN_STATUS_OK)
	{
        return wsp::kinect::MakeResultFromXnStatus(nRetVal);
	}
    depth_generator_.GetAlternativeViewPointCap().SetViewPoint( image_generator_ );

	nRetVal = context_.FindExistingNode(XN_NODE_TYPE_USER, user_generator_);
	if (nRetVal != XN_STATUS_OK)
	{
		nRetVal = user_generator_.Create(context_);
		CHECK_RC(nRetVal, "Find user generator");
	}

    XnBool is_skelton_supported = user_generator_.IsCapabilitySupported(XN_CAPABILITY_SKELETON);
	if ( is_skelton_supported==false )
	{
		printf("Supplied user generator doesn't support skeleton\n");
        return wsp::kinect::ResultSkeltonIsUnsupported();
	}

	XnCallbackHandle hUserCallbacks, hCalibrationStart, hCalibrationComplete, hPoseDetected, hCalibrationInProgress, hPoseInProgress;

	nRetVal = user_generator_.RegisterUserCallbacks(wsp::KinectHandler::User_NewUser, wsp::KinectHandler::User_LostUser, NULL, hUserCallbacks);
	CHECK_RC(nRetVal, "Register to user callbacks");
	nRetVal = user_generator_.GetSkeletonCap().RegisterToCalibrationStart(UserCalibration_CalibrationStart, NULL, hCalibrationStart);
	CHECK_RC(nRetVal, "Register to calibration start");
	nRetVal = user_generator_.GetSkeletonCap().RegisterToCalibrationComplete(UserCalibration_CalibrationComplete, NULL, hCalibrationComplete);
	CHECK_RC(nRetVal, "Register to calibration complete");

	if (user_generator_.GetSkeletonCap().NeedPoseForCalibration())
	{
		need_pose_ = TRUE;
		if (!user_generator_.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION))
		{
			printf("Pose required, but not supported\n");
            return wsp::kinect::ResultPoseDetectionIsUnsupported();
		}
		nRetVal = user_generator_.GetPoseDetectionCap().RegisterToPoseDetected(UserPose_PoseDetected, NULL, hPoseDetected);
		CHECK_RC(nRetVal, "Register to Pose Detected");
		user_generator_.GetSkeletonCap().GetCalibrationPose(pose_str_);

		nRetVal = user_generator_.GetPoseDetectionCap().RegisterToPoseInProgress(MyPoseInProgress, NULL, hPoseInProgress);
		CHECK_RC(nRetVal, "Register to pose in progress");
	}

	user_generator_.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);

	nRetVal = user_generator_.GetSkeletonCap().RegisterToCalibrationInProgress(MyCalibrationInProgress, NULL, hCalibrationInProgress);
	CHECK_RC(nRetVal, "Register to calibration in progress");

    return wsp::ResultSuccess();
}

void wsp::KinectHandler::Finalize()
{
	script_node_.Release();
	depth_generator_.Release();
	user_generator_.Release();
	player_.Release();
	context_.Release();

}

xn::UserGenerator&  wsp::KinectHandler::GetUserGenerator(){  return user_generator_; }

xn::DepthGenerator& wsp::KinectHandler::GetDepthGenerator()
{ 
    WSP_ASSERT(instance_!=NULL, "Instance is not initialized"); 
    return instance_->depth_generator_; 
}
xn::ImageGenerator& wsp::KinectHandler::GetImageGenerator()
{ 
    WSP_ASSERT(instance_!=NULL, "Instance is not initialized"); 
    return instance_->image_generator_; 
}


wsp::Result wsp::KinectHandler::StartGenerating()
{
	XnStatus nRetVal = context_.StartGeneratingAll();
	CHECK_RC(nRetVal, "StartGenerating");
    return wsp::ResultSuccess();
}

// Save calibration to file
void wsp::KinectHandler::SaveCalibration()
{
	XnUserID aUserIDs[20] = {0};
	XnUInt16 nUsers = 20;
	user_generator_.GetUsers(aUserIDs, nUsers);
	for (int i = 0; i < nUsers; ++i)
	{
		// Find a user who is already calibrated
		if (user_generator_.GetSkeletonCap().IsCalibrated(aUserIDs[i]))
		{
			// Save user's calibration to file
			user_generator_.GetSkeletonCap().SaveCalibrationDataToFile(aUserIDs[i], XN_CALIBRATION_FILE_NAME);
			break;
		}
	}
}
// Load calibration from file
void wsp::KinectHandler::LoadCalibration()
{
	XnUserID aUserIDs[20] = {0};
	XnUInt16 nUsers = 20;
	user_generator_.GetUsers(aUserIDs, nUsers);
	for (int i = 0; i < nUsers; ++i)
	{
		// Find a user who isn't calibrated or currently in pose
		if (user_generator_.GetSkeletonCap().IsCalibrated(aUserIDs[i])) continue;
		if (user_generator_.GetSkeletonCap().IsCalibrating(aUserIDs[i])) continue;

		// Load user's calibration from file
		XnStatus rc = user_generator_.GetSkeletonCap().LoadCalibrationDataFromFile(aUserIDs[i], XN_CALIBRATION_FILE_NAME);
		if (rc == XN_STATUS_OK)
		{
			// Make sure state is coherent
			user_generator_.GetPoseDetectionCap().StopPoseDetection(aUserIDs[i]);
			user_generator_.GetSkeletonCap().StartTracking(aUserIDs[i]);
		}
		break;
	}
}

wsp::Result wsp::KinectHandler::GetSkeltonPosition( 
    s32 *o_px, s32 *o_py, 
    XnUserID player, XnSkeletonJoint joint_type
)
{
    WSP_ASSERT_NOT_NULL( o_px );
    WSP_ASSERT_NOT_NULL( o_py );

	if (!user_generator_.GetSkeletonCap().IsTracking(player))
	{
        return wsp::kinect::ResultSkeltonNotTracked();
	}

	if (!user_generator_.GetSkeletonCap().IsJointActive(joint_type))
	{
		return wsp::kinect::ResultTargetSkeltonNotActive();
	}

	XnSkeletonJointPosition joint;
	user_generator_.GetSkeletonCap().GetSkeletonJointPosition(player, joint_type, joint);

    const f32 CONFIDENT_RATIO = 0.5f;
	if (joint.fConfidence < CONFIDENT_RATIO)
	{
        return wsp::kinect::ResultTrackedJointNotConfident();
	}

	XnPoint3D pt;
	pt = joint.position;
	depth_generator_.ConvertRealWorldToProjective(1, &pt, &pt);

    *o_px = pt.X;
    *o_py = pt.Y;

    return wsp::ResultSuccess();
}
#endif