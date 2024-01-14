/**
 * @file sound_result.h
 * 
 */

#ifndef WSP_IMAGE_SOUND_RESULT_H_
#define WSP_IMAGE_SOUND_RESULT_H_

#ifndef __WSP_COMMONCORE_RESULT_H__
#include <wsp/common/result.h>
#endif

/*
 *   Result range for sound module: 0x00005000 - 0x00005fff
 */
#ifdef __cplusplus
namespace wsp{ namespace sound{
    WSP_DEFINE_CPP_RESULT_RANGE( ResultInvalidUsage, 0x00005000, 0x000050ff, WSP_RESULT_LEVEL_USAGE );

    WSP_DEFINE_CPP_RESULT_RANGE( ResultFatalError, 0x00005100, 0x000051ff, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( ResultReadingMidiFileFailed, 0x00001001, 0x00001001, WSP_RESULT_LEVEL_FATAL );
}}
#endif


#endif