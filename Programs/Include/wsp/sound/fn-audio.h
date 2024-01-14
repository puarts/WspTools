/**
 * @file fn-audio.h
 * 
 */

#ifndef __WSP_SOUNDCORE_FUNCTION_AUDIO_H__
#define __WSP_SOUNDCORE_FUNCTION_AUDIO_H__

#include <wsp/common/_define_commoncore.h>
#include <vector>
#include <string>

WSP_DLL_EXPORT void WSP_PrintInputAudioDevices(void);
WSP_DLL_EXPORT void WSP_PrintOutputAudioDevices(void);

// çÏÇËÇ©ÇØï˙íu
WSP_DLL_EXPORT void WSP_OutputSoundToSpeaker(int device_id);

namespace wsp {
    namespace sound {
        WSP_DLL_EXPORT void GetInputAudioDevices(
            char* o_devices,
            int* o_num_devices,
            int unit_string_buffer_size,
            int max_device_buffer_size);
    }
}

#endif
