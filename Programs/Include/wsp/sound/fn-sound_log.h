/**
 * @file fn-sound_log.h
 * @brief Definition of functionality of audio.
 * 
 */

#ifndef _WSP_SOUND_SOUNDCORE_LOG_H_
#define _WSP_SOUND_SOUNDCORE_LOG_H_

#include <wsp/common/fn-io.h>

#define WSP_SOUND_LOG_PREFIX "[WSP/SOUND] "

#define WSP_SOUND_ERROR_LOG(...)    WSP_ERROR_LOG   (WSP_SOUND_LOG_PREFIX, __VA_ARGS__)
#define WSP_SOUND_WARNING_LOG(...)  WSP_WARNING_LOG (WSP_SOUND_LOG_PREFIX, __VA_ARGS__)
#define WSP_SOUND_LOG(...)          WSP_LOG         (WSP_SOUND_LOG_PREFIX, __VA_ARGS__)
#define WSP_SOUND_DEBUG_LOG(...)    WSP_DEBUG_LOG   (WSP_SOUND_LOG_PREFIX, __VA_ARGS__)

#endif
