/**
 * @file _define_multimedia.h
 * @author Jun-ichi Nishikata
 * @brief Base definition of multimedia module.
 */


#ifndef _WSP_MULTIMEDIA_DEFINE_H_
#define _WSP_MULTIMEDIA_DEFINE_H_

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include <wsp/common/_define_commoncore.h>
#endif
#include <wsp/math/_define_mathutil.h>
#ifndef __WSP_IMAGEUTIL_DEFINE_IMAGEUTIL_H__
#include <wsp/image/_define_image.h>
#endif
#include <wsp/video/_define_video.h>

#define SIGNED_MEDIA_DATA_TYPE SIGNED_DATA_TYPE
#define UNSIGNED_MEDIA_DATA_TYPE UNSIGNED_DATA_TYPE

#ifdef __cplusplus
namespace wsp{ 
/**
 * @brief Collection of functions and classes for WspMultimedia module.
 *        Depending on CommonCore, CommonUtil, Math, Image, Video, Sound libraries.
 */
    namespace mm{
#endif
        /**
         * @brief Media types
         */
        enum WSP_MediaType{
            /** @brief  unrecognized data type */
            MEDIATYPE_UNKNOWN,
            /** @brief  single image data type */
            MEDIATYPE_IMAGE,
            /** @brief  image sequence data type */
            MEDIATYPE_IMAGE_SEQUENCE,
            /** @brief  video data type */
            MEDIATYPE_VIDEO,
            /** @brief  sound data type */
            MEDIATYPE_SOUND,
            /** @brief  multimedia data type composited with images, sounds and others */
            MEDIATYPE_COMPOSITION,
            /** @brief  midi data type */
            MEDIATYPE_MIDI
        };
    
        typedef enum WSP_MediaType MediaType;


#ifdef __cplusplus
    }
}
#endif

#endif
