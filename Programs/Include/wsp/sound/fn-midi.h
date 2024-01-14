/**
 * @file fn-midi.h
 * 
 */

#ifndef __WSP_SOUNDCORE_FUNCTION_MIDI_H__
#define __WSP_SOUNDCORE_FUNCTION_MIDI_H__

#include <wsp/sound/_define_soundcore.h>
#include <wsp/sound/_midi_types.h>

#ifdef __cplusplus
extern "C" {
#endif

    /** @brief Copy MIDI data */
    WSP_DLL_EXPORT void WSP_CopyMidiData(WSP_MidiData *o_data, const WSP_MidiData *in_data);

    /** @brief Compare MIDI data */
    WSP_DLL_EXPORT bool WSP_IsEqualMidiData(WSP_MidiData *o_data, const WSP_MidiData *in_data);

    /**
     * @brief Read the standard MIDI file
     */
    WSP_DLL_EXPORT WSP_MidiData* WSP_ReadStandardMidiFile(const char *in_smf_path);

    /**
     * @brief Release MIDI data obtained by WSP_ReadStandardMidiFile
     */
    WSP_DLL_EXPORT void WSP_ReleaseMidiData(WSP_MidiData *io_midi_data);

    /**
     * @brief Get MIDI event type from byte.
     */
    WSP_DLL_EXPORT enum WSP_MidiEventType WSP_GetMidiEventType(u8 status_byte);

    /**
     * @brief Get MIDI meta event type from byte.
     */
    WSP_DLL_EXPORT enum WSP_MidiMetaEventType WSP_GetMidiMetaEventType(u8 meta_event_type_byte);

    WSP_DLL_EXPORT int WSP_GetMidiNoteFromFrequency(float frequency_as_hertz);

    WSP_DLL_EXPORT void WSP_GetMidiNoteAsString(char *o_midi_note_as_string, int midi_note);

#ifdef __cplusplus
}
#endif

#endif
