//#define USE_OPENCV21
//#define USE_OPENCV229
#define USE_OPENCV230

#pragma warning(disable : 4819) // disables warning messages

#if defined USE_OPENCV21
    #include <cv.h>
    #include <highgui.h>
    #include <cvaux.h>
    #include <cvwimage.h>
    #ifndef OPENCV21_LOADED
        #define OPENCV21_LOADED
        #ifdef _DEBUG
            #pragma comment(lib, "cxcore210d.lib") 
            #pragma comment(lib, "cvaux210d.lib") 
            #pragma comment(lib, "cv210d.lib") 
            #pragma comment(lib, "highgui210d.lib") 
        #else
            #pragma comment(lib, "cxcore210.lib") 
            #pragma comment(lib, "cvaux210.lib") 
            #pragma comment(lib, "cv210.lib") 
            #pragma comment(lib, "highgui210.lib") 
        #endif
    #endif
#elif defined USE_OPENCV220
    #include <load_qt.h>
    #include <opencv2/opencv.hpp>
    #include <opencv2/core/core.hpp>
    #include <opencv2/highgui/highgui.hpp>
    //#include <opencv2/gpu/gpu.hpp>
    #include <opencv2/imgproc/imgproc.hpp>
    //#include <opencv2/calib3d/calib3d.hpp>
    #ifndef OPENCV22_LOADED
        #define OPENCV220_LOADED
        #ifdef _DEBUG
            #pragma comment(lib, "opencv_core220d.lib") 
            #pragma comment(lib, "opencv_highgui220d.lib")
            //#pragma comment(lib, "opencv_gpu220d.lib")
            #pragma comment(lib, "opencv_imgproc220d.lib")
            //#pragma comment(lib, "opencv_calib3d220d.lib") 
        #else
            #pragma comment(lib, "opencv_core220.lib") 
            #pragma comment(lib, "opencv_highgui220.lib")
            //#pragma comment(lib, "opencv_gpu220.lib")
            #pragma comment(lib, "opencv_imgproc220.lib")
            //#pragma comment(lib, "opencv_calib3d220.lib") 
        #endif
    #endif
#elif defined USE_OPENCV229
    #include <opencv2/opencv.hpp>
    #include <opencv2/core/core.hpp>
    #include <opencv2/highgui/highgui.hpp>
    //#include <opencv2/gpu/gpu.hpp>
    #include <opencv2/imgproc/imgproc.hpp>
    //#include <opencv2/calib3d/calib3d.hpp>
    #ifndef OPENCV229_LOADED
        #define OPENCV229_LOADED
        #ifdef _DEBUG
            #pragma comment(lib, "opencv_video229d.lib")
            #pragma comment(lib, "opencv_core229d.lib") 
            #pragma comment(lib, "opencv_highgui229d.lib")
            //#pragma comment(lib, "opencv_gpu229d.lib")
            #pragma comment(lib, "opencv_imgproc229d.lib")
            //#pragma comment(lib, "opencv_calib3d229d.lib") 
        #else
            //#pragma comment(lib, "cv.lib")
            #pragma comment(lib, "opencv_video229.lib")
            #pragma comment(lib, "opencv_core229.lib") 
            #pragma comment(lib, "opencv_highgui229.lib")
            //#pragma comment(lib, "opencv_gpu229.lib")
            #pragma comment(lib, "opencv_imgproc229.lib")
            //#pragma comment(lib, "opencv_calib3d229.lib") 
        #endif
    #endif
#elif defined USE_OPENCV230
    #include <opencv2/opencv.hpp>
    #include <opencv2/core/core.hpp>
    #include <opencv2/highgui/highgui.hpp>
    #include <opencv2/gpu/gpu.hpp>
    #include <opencv2/imgproc/imgproc.hpp>
    #include <opencv2/calib3d/calib3d.hpp>
    #ifndef OPENCV230_LOADED
        #define OPENCV230_LOADED
        #ifdef _DEBUG
            #pragma comment(lib, "opencv_video230d.lib")
            #pragma comment(lib, "opencv_core230d.lib") 
            #pragma comment(lib, "opencv_highgui230d.lib")
            #pragma comment(lib, "opencv_gpu230d.lib")
            #pragma comment(lib, "opencv_imgproc230d.lib")
            #pragma comment(lib, "opencv_calib3d230d.lib") 
        #else
            #pragma comment(lib, "opencv_video230.lib")
            #pragma comment(lib, "opencv_core230.lib") 
            #pragma comment(lib, "opencv_highgui230.lib")
            #pragma comment(lib, "opencv_gpu230.lib")
            #pragma comment(lib, "opencv_imgproc230.lib")
            #pragma comment(lib, "opencv_calib3d230.lib") 
        #endif
    #endif
#endif
