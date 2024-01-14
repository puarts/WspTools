/**
 * @file cl-kinect_handler.h
 * @author Jun-ichi Nishikata
 * @brief class for handling Kinect.
 */

#ifndef __WSP_KINECT_CLASS_KINECT_HANDLER_H__
#define __WSP_KINECT_CLASS_KINECT_HANDLER_H__

#include <wsp/kinect/define_kinect.h>

#ifdef USE_XN

#include <wsp/common/fn-io.h>
#include <wsp/common/result.h>

#include <XnCppWrapper.h>
#include <map>

namespace wsp{
    class WSP_DLL_EXPORT KinectHandler
    {
    public:
	    // Singleton
        static wsp::KinectHandler& GetInstance();
	    static void DestroyInstance();

        wsp::Result StartGenerating();

        xn::DepthGenerator& GetDepthGenerator();
        xn::ImageGenerator& GetImageGenerator();
        static xn::UserGenerator&  GetUserGenerator();

        void WaitUpdate();

        /** @brief Save calibration to file */
        void SaveCalibration();

        /** @brief Load calibration from file */
        void LoadCalibration();

        wsp::Result GetSkeltonPosition( s32 *o_px, s32 *o_py, XnUserID player, XnSkeletonJoint joint_type);

    private:
        KinectHandler();
        KinectHandler(const KinectHandler& rhs);
        KinectHandler& operator=(const KinectHandler& rhs);

        wsp::Result Initialize();
        void Finalize();

        /* call back functions */

        // Callback: New user was detected
        static void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/);

        // Callback: An existing user was lost
        static void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/);

        // Callback: Detected a pose
        static void XN_CALLBACK_TYPE UserPose_PoseDetected(xn::PoseDetectionCapability& /*capability*/, const XnChar* strPose, XnUserID nId, void* /*pCookie*/);

        // Callback: Started calibration
        static void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(xn::SkeletonCapability& /*capability*/, XnUserID nId, void* /*pCookie*/);

        // Callback: Finished calibration
        static void XN_CALLBACK_TYPE UserCalibration_CalibrationComplete(xn::SkeletonCapability& /*capability*/, XnUserID nId, XnCalibrationStatus eStatus, void* /*pCookie*/);

        static void XN_CALLBACK_TYPE MyCalibrationInProgress(xn::SkeletonCapability& capability, XnUserID id, XnCalibrationStatus calibrationError, void* pCookie);
        static void XN_CALLBACK_TYPE MyPoseInProgress(xn::PoseDetectionCapability& capability, const XnChar* strPose, XnUserID id, XnPoseDetectionStatus poseError, void* pCookie);

    private:
        xn::Context              context_;
        xn::ScriptNode           script_node_;
        xn::DepthGenerator       depth_generator_;
        static xn::UserGenerator user_generator_;
        xn::ImageGenerator       image_generator_;
	    xn::GestureGenerator     gesture_generator_;
	    xn::HandsGenerator       hands_generator_;

        xn::Player               player_;

        static XnBool need_pose_;
        static XnChar pose_str_[20];

        static std::map<XnUInt32, std::pair<XnCalibrationStatus, XnPoseDetectionStatus> > error_;

        static wsp::KinectHandler*	instance_;
    };
}

#endif

#endif
