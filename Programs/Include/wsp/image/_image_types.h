/**
 * @file _image_types.h
 * @author 
 */

#ifndef __WSP_IMAGECORE_IMAGE_TYPES_H__
#define __WSP_IMAGECORE_IMAGE_TYPES_H__

#ifndef __WSP_COMMONCORE_CTYPE_H__
#include <wsp/common/_ctypes.h>
#endif

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include <wsp/common/_define_commoncore.h>
#endif

#ifndef __WSP_MATHCORE_MATH_TYPES_H__
#include <wsp/math/_math_types.h>
#endif


typedef enum WSP_ImageState{
    WSP_IMAGE_STATE_SUCCESS,
    WSP_IMAGE_STATE_COULDNT_OPEN,
    WSP_IMAGE_STATE_INVALID_FORMAT,
    WSP_IMAGE_STATE_UNSUPPORTED_FORMAT,
    WSP_IMAGE_STATE_READING_FILE_FAILED,
    WSP_IMAGE_STATE_WRITING_FILE_FAILED
} WSP_ImageState;

//! \ingroup WspImage
//! \brief Image formats.
typedef enum WSP_ImageFormat{
    WSP_IMAGE_FORMAT_BMP      , // .bmp
    WSP_IMAGE_FORMAT_CUT      , // .cut
    WSP_IMAGE_FORMAT_DDS      , // .dds
    WSP_IMAGE_FORMAT_DOOM     , // .doom
    WSP_IMAGE_FORMAT_OPENEXR  , // .exr
    WSP_IMAGE_FORMAT_HDR      , // .hdr
    WSP_IMAGE_FORMAT_GIF      , // .gif
    WSP_IMAGE_FORMAT_ICO      , // .ico
    WSP_IMAGE_FORMAT_JPEG2000 , // .jp2
    WSP_IMAGE_FORMAT_JPEG     , // .jpg
    WSP_IMAGE_FORMAT_IBM      , // .ibm
    WSP_IMAGE_FORMAT_MDL      , // .mdl
    WSP_IMAGE_FORMAT_MNG      , // .mng
    WSP_IMAGE_FORMAT_PAL      , // .pal
    WSP_IMAGE_FORMAT_PBM      , // .pbm
    WSP_IMAGE_FORMAT_PCD      , // .pcd
    WSP_IMAGE_FORMAT_PCX      , // .pcx
    WSP_IMAGE_FORMAT_PICT     , // .pic
    WSP_IMAGE_FORMAT_PNG      , // .png
    WSP_IMAGE_FORMAT_PGM      , // .pgm
    WSP_IMAGE_FORMAT_PPM      , // .ppm
    WSP_IMAGE_FORMAT_PHOTOSHOP, // .psd
    WSP_IMAGE_FORMAT_PSP      , // .psp
    WSP_IMAGE_FORMAT_RAW      , // .raw
    WSP_IMAGE_FORMAT_SGI      , // .sgi
    WSP_IMAGE_FORMAT_Targa    , // .tga
    WSP_IMAGE_FORMAT_TIFF     , // .tif
    WSP_IMAGE_FORMAT_ADRI     , // Adaptive Dynamic Range Image
    WSP_IMAGE_FORMAT_UNKNOWN
}WSP_ImageFormat;

#define MAX_LENGTH_OF_FORMATNAME 32

typedef struct WSP_ImageFormatInfo {
    WSP_ImageFormat format_id;
    char            format_name[MAX_LENGTH_OF_FORMATNAME];
    char            extension[MAX_LENGTH_OF_EXTENSION];
}WSP_ImageFormatInfo;

static const WSP_ImageFormatInfo WSP_IMAGE_FORMAT_INFO_LIST[] = {
    { WSP_IMAGE_FORMAT_BMP      ,"BMP"                         ,"bmp" },
    { WSP_IMAGE_FORMAT_CUT      ,"CUT"                         ,"cut" },
    { WSP_IMAGE_FORMAT_DDS      ,"DDS"                         ,"dds" },
    { WSP_IMAGE_FORMAT_DOOM     ,"DOOM"                        ,"doom"},
    { WSP_IMAGE_FORMAT_OPENEXR  ,"OpenEXR"                     ,"exr" },
    { WSP_IMAGE_FORMAT_HDR      ,"HDR"                         ,"hdr" },
    { WSP_IMAGE_FORMAT_GIF      ,"GIF"                         ,"gif" },
    { WSP_IMAGE_FORMAT_ICO      ,"ICO (Windows Icon)"          ,"ico" },
    { WSP_IMAGE_FORMAT_JPEG2000 ,"JPEG2000"                    ,"jp2" },
    { WSP_IMAGE_FORMAT_JPEG     ,"JPEG"                        ,"jpg" },
    { WSP_IMAGE_FORMAT_IBM      ,"IBM"                         ,"ibm" },
    { WSP_IMAGE_FORMAT_MDL      ,"MDL"                         ,"mdl" },
    { WSP_IMAGE_FORMAT_MNG      ,"MNG"                         ,"mng" },
    { WSP_IMAGE_FORMAT_PAL      ,"PAL"                         ,"pal" },
    { WSP_IMAGE_FORMAT_PBM      ,"PBM"                         ,"pbm" },
    { WSP_IMAGE_FORMAT_PCD      ,"PCD"                         ,"pcd" },
    { WSP_IMAGE_FORMAT_PCX      ,"PCX"                         ,"pcx" },
    { WSP_IMAGE_FORMAT_PICT     ,"PICT file"                   ,"pic" },
    { WSP_IMAGE_FORMAT_PNG      ,"Portable Network Graphics"   ,"png" },
    { WSP_IMAGE_FORMAT_PGM      ,"Portable Graymap"            ,"pgm" },
    { WSP_IMAGE_FORMAT_PPM      ,"Portable Pixmap"             ,"ppm" },
    { WSP_IMAGE_FORMAT_PHOTOSHOP,"Photoshop"                   ,"psd" },
    { WSP_IMAGE_FORMAT_PSP      ,"PSP"                         ,"psp" },
    { WSP_IMAGE_FORMAT_RAW      ,"RAW"                         ,"raw" },
    { WSP_IMAGE_FORMAT_SGI      ,"SGI"                         ,"sgi" },
    { WSP_IMAGE_FORMAT_Targa    ,"Targa"                       ,"tga" },
    { WSP_IMAGE_FORMAT_TIFF     ,"TIFF"                        ,"tif" },
    { WSP_IMAGE_FORMAT_ADRI     ,"Adaptive Dynamic Range Image","adri"},
};
static u32 WSP_IMAGE_FORMAT_INFO_LIST_SIZE = sizeof(WSP_IMAGE_FORMAT_INFO_LIST)/sizeof(WSP_ImageFormatInfo);


//! \ingroup WspImage
//! \brief Depth value types.
typedef enum WSP_DepthValueType{
    WSP_DEPTH_VALUE_TYPE_LARGER_VALUE_IS_NEARER=0,
    WSP_DEPTH_VALUE_TYPE_SMALLER_VALUE_IS_NEARER=1
}WSP_DepthValueType;


//! \ingroup WspImage
//! \brief Image data types.
typedef enum WSP_ImageDataType{
    WSP_IMAGE_DATA_TYPE_BINARY,
    WSP_IMAGE_DATA_TYPE_GRAY,
    WSP_IMAGE_DATA_TYPE_SHORT,
    WSP_IMAGE_DATA_TYPE_RGB,
    WSP_IMAGE_DATA_TYPE_RGBD,
    WSP_IMAGE_DATA_TYPE_LONG,
    WSP_IMAGE_DATA_TYPE_PTR,
    WSP_IMAGE_DATA_TYPE_FLOAT,
    WSP_IMAGE_DATA_TYPE_DOUBLE,
    WSP_IMAGE_DATA_TYPE_DOUBLE3
} WSP_ImageDataType;

typedef enum WSP_InterpolationAlgorithm{
    WSP_INTERPOLATION_NEAREST_NEIGHBOR,
    WSP_INTERPOLATION_BILINEAR
}WSP_InterpolationAlgorithm;


typedef enum WSP_StereoAlgorithm{
    WSP_STEREO_UNDEFINED,
    WSP_STEREO_BM,
    WSP_STEREO_HIERARCHICAL_POC,
    WSP_STEREO_BM_BASED_POC,
    WSP_STEREO_BM_BASED_FRONT_ONLY_POC
}WSP_StereoAlgorithm;

//! \ingroup WspImage
//! \brief State of pixel.
typedef enum WSP_PixelState{
    WSP_PIXEL_STATE_NORMAL=0,
    WSP_PIXEL_STATE_INNER_OCCLUDED=255,
    WSP_PIXEL_STATE_OCCLUDED=30,
    WSP_PIXEL_STATE_IGNORED=100,
    WSP_PIXEL_STATE_TRANSPARENT=150,
    WSP_PIXEL_STATE_MASKED=120,
    WSP_PIXEL_STATE_BACKGROUND=200
} WSP_PixelState;

//! \ingroup WspImage
typedef enum WSP_ChannelType{
    WSP_CHANNEL_TYPE_RED,
    WSP_CHANNEL_TYPE_GREEN,
    WSP_CHANNEL_TYPE_BLUE,
    WSP_CHANNEL_TYPE_SUM_OF_RGB
}WSP_ChannelType;

struct WSP_ImageParam{
    int width;
    int height;
    int num_channels;

#ifdef __cplusplus
    WSP_ImageParam(int in_width, int in_height, int in_num_channels){
        width = in_width;
        height = in_height;
        num_channels = in_num_channels;
    }
#endif
};


struct WSP_ImgFilterParam{
    int filter_size;
    int num_of_iteration;
    bool use_alpha_as_mask;

#ifdef __cplusplus
    WSP_ImgFilterParam(){
        filter_size=1; num_of_iteration=1; use_alpha_as_mask=true;
    }
    WSP_ImgFilterParam(int in_filter_size, int in_num_of_iteration, bool in_use_alpha_as_mask){
        filter_size = in_filter_size;
        num_of_iteration = in_num_of_iteration;
        use_alpha_as_mask = in_use_alpha_as_mask;
    }
#endif
};



//! \ingroup WspImage
//! \brief Parameters of block correlation stereo matching.
typedef struct WSP_StereoBMParams{
    double thre_std_deviation; // threshold of intensity variance within matching block
    double thre_corr; // threshold of normalized correlation value (SSD)
    double cross_check_fuzziness;
    double scale_thre_enough_edge_amount;
    double prescaling_factor;
    double default_corr;
    int dispx_min, dispy_min, dispx_max, dispy_max;
    int block_min, block_max;
    int coarse_to_fine_depth;
    int subpixel_sample_range;
    bool show_progress;
    bool save_debug_data;
    bool show_debug_info;
    bool cross_check; /* do cross-checking for detecting error matching pixels */
    bool subpixel_estimation; /* do subpixel estimation by function fitting */
    bool presubtract_average; /* to be robust for intensity difference among two images */

    //double (*eval_func)(double, double);
    WSP_FittingFunction subpixel_fitting_function;
    WSP_CorrelationFunction eval_func;

#ifdef __cplusplus
    WSP_StereoBMParams(){
        thre_std_deviation = 20;
        thre_corr= 0.1;
        scale_thre_enough_edge_amount=1.0;
        cross_check_fuzziness = 0.0;
        dispx_min=-20, dispx_max=20;
        dispy_min=0, dispy_max=0;
        block_min=3, block_max=10;
        coarse_to_fine_depth=1;
        subpixel_sample_range=1;
        show_progress=false;
        save_debug_data=false;
        show_debug_info=false;
        cross_check=true;
        subpixel_estimation = false;
        //eval_func=wsp::math::GetSquaredDiff;
        eval_func = WSP_CORRELATION_FUNCTION_SSD;
        subpixel_fitting_function = WSP_FITTING_FUNCTION_UNDEFINED;
        prescaling_factor = 1.0;
        presubtract_average = false;
        default_corr = 0.0;
            
    }
    WSP_StereoBMParams(const WSP_StereoBMParams &params){
        *this = params;
    }
    WSP_StereoBMParams& operator=(const WSP_StereoBMParams &rhs){
        thre_std_deviation = rhs.thre_std_deviation;
        thre_corr= rhs.thre_corr;
        scale_thre_enough_edge_amount = rhs.scale_thre_enough_edge_amount;
        cross_check_fuzziness = rhs.cross_check_fuzziness;
        dispx_min=rhs.dispx_min, dispx_max=rhs.dispx_max;
        dispy_min=rhs.dispy_min, dispy_max=rhs.dispy_max;
        block_min=rhs.block_min, block_max=rhs.block_max;
        coarse_to_fine_depth=rhs.coarse_to_fine_depth;
        show_progress=rhs.show_progress;
        save_debug_data=rhs.save_debug_data;
        show_debug_info=rhs.show_debug_info;
        cross_check=rhs.cross_check;
        subpixel_estimation = rhs.subpixel_estimation;
        eval_func=rhs.eval_func;
        subpixel_fitting_function = rhs.subpixel_fitting_function;
        subpixel_sample_range = rhs.subpixel_sample_range;
        prescaling_factor = rhs.prescaling_factor;
        presubtract_average = rhs.presubtract_average;
        default_corr = rhs.default_corr;
        return *this;
    }
#endif
}WSP_StereoBMParams;




//! \ingroup WspImage
//! \brief Parameters of stereo matching based on POC.
typedef struct WSP_StereoPOCParams{
    double thre_std_deviation;
    double thre_poc_peak;
    double low_pass_ratio;
    double cross_check_fuzziness;
    int dispx_min, dispy_min, dispx_max, dispy_max;
    int block_width, block_height;
    int search_depth; 
    int sample_pix_distance;
    int sample_range;
    bool use_2d_poc;
    bool show_progress;
    bool save_debug_data;
    bool show_debug_info;
    bool cross_check;
    bool nearer_is_brighter;
    WSP_StereoBMParams bm_params;
    WSP_StereoAlgorithm algorithm;

    // params for back ground estimation in front-only POC
    double bg_estim_thre_pixel_amount_ratio;
    int bg_estim_cont_fuzziness;
    int bg_estim_num_bg_segm; /* how many segm will be assumed as back ground */

#ifdef __cplusplus
    WSP_StereoPOCParams(){
        thre_std_deviation = 30.0;
        thre_poc_peak = 0.1;
        low_pass_ratio = 0.75;
        cross_check_fuzziness=1.0;
        dispx_min = -20, dispx_max = 20;
        dispy_min = 0, dispy_max = 0;
        block_width = 32, block_height = 11;
        search_depth = 2;
        sample_pix_distance = 1;
        sample_range = 4;
        use_2d_poc=false;
        show_progress=false;
        save_debug_data=false;
        show_debug_info=false;
        cross_check=true;
        nearer_is_brighter=false;
        algorithm=WSP_STEREO_UNDEFINED;
        bg_estim_thre_pixel_amount_ratio = 0.2;
        bg_estim_cont_fuzziness = 0;
        bg_estim_num_bg_segm = 1;
    }
    WSP_StereoPOCParams(const WSP_StereoPOCParams &params){
        *this = params;
    }
    WSP_StereoPOCParams& operator=(const WSP_StereoPOCParams &rhs){
        thre_std_deviation = rhs.thre_std_deviation;
        thre_poc_peak = rhs.thre_poc_peak;
        low_pass_ratio = rhs.low_pass_ratio;
        cross_check_fuzziness = rhs.cross_check_fuzziness;
        dispx_min = -20, dispx_max = rhs.dispx_min;
        dispy_min = 0, dispy_max = rhs.dispy_min;
        block_width = 32, block_height = rhs.block_width;
        search_depth = rhs.search_depth;
        sample_pix_distance = rhs.sample_pix_distance;
        sample_range = rhs.sample_range;
        use_2d_poc = rhs.use_2d_poc;
        show_progress = rhs.show_progress;
        save_debug_data = rhs.save_debug_data;
        show_debug_info = rhs.show_debug_info;
        cross_check = rhs.cross_check;
        bm_params = rhs.bm_params;
        algorithm = rhs.algorithm;
        bg_estim_thre_pixel_amount_ratio = rhs.bg_estim_thre_pixel_amount_ratio;
        nearer_is_brighter = rhs.nearer_is_brighter;
        bg_estim_cont_fuzziness = rhs.bg_estim_cont_fuzziness;
        bg_estim_num_bg_segm = rhs.bg_estim_num_bg_segm;
        return *this;
    }
#endif
}WSP_StereoPOCParams;



#ifdef __cplusplus
namespace wsp{ namespace img{

    typedef u8*  ImgPtr8U;
    typedef u16* ImgPtr16U;
    typedef u32* ImgPtr32U;

    typedef enum   WSP_ImageState             ImageState;
    typedef enum   WSP_ImageFormat            ImageFormat;
    typedef enum   WSP_DepthValueType         DepthValueType;
    typedef enum   WSP_ImageDataType          ImageDataType;
    typedef enum   WSP_InterpolationAlgorithm InterpolationAlgorithm;
    typedef enum   WSP_StereoAlgorithm        StereoAlgorithm;
    typedef enum   WSP_PixelState             PixelState;
    typedef enum   WSP_ChannelType            ChannelType;
    
    typedef struct WSP_ImageFormatInfo        ImageFormatInfo;
    typedef struct WSP_ImageParam             ImageParam;
    typedef struct WSP_ImgFilterParam         ImgFilterParam;
    typedef struct WSP_StereoBMParams         StereoBMParams;
    typedef struct WSP_StereoPOCParams        StereoPOCParams;

}}
#endif


#endif
