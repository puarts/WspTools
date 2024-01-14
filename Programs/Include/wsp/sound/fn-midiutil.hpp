/**
 * @file fn-midiutil.hpp
 * 
 */

#ifndef __WSP_SOUND_FUNCTION_MIDIUTIL_H__
#define __WSP_SOUND_FUNCTION_MIDIUTIL_H__

#include <wsp/sound/_define_sound.h>

extern "C"{
#include <wsp/sound/_define_soundcore.h>
#include <wsp/sound/_midi_types.h>
}

#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#include <wsp/common/tpl_cl-vector.h>
#endif

#include <wsp/image/tpl_cl-imageseq.h>
#include <wsp/common/tpl_cl-vector3.h>
#include <wsp/common/result.h>

namespace wsp{ namespace sound{
    class Midi;
}}

typedef struct WSP_MidiEvent {
    /* shared param */
    u32               delta_time;
    WSP_MidiEventType type;
    wsp::Vector<u8>   data;

    /* param for specific event */
    u32 tempo;
    u8  note;
    u8  velocity;
    u8  key_pressure;
} WSP_MidiEvent;

namespace wsp{ namespace sound{
    inline f32 ConvertMidiDeltaTimeToSecond( u32 delta_time, u32 bpm )
    {
        return delta_time/(f32)(8*bpm);
    }
    inline u32 ConvertSecondToMidiDeltaTime( f32 seconds, u32 bpm )
    {
        return static_cast<u32>(seconds * ( 8 * bpm ));
    }
    inline u32 GetBpmFromTempo( u32 tempo )
    {
        return 60*1000000/tempo;
    }

    using LogFunc = void(*)(const char*);
    WSP_DLL_EXPORT void PrintMidiInputDevices(LogFunc log_func = nullptr);



    /**
     * @brief Get MIDI event list from MIDI track chunk.
     * @param[out] o_eventlist MIDI event list for output.
     * @param[in]  track_chunk Track chunk for input.
     */
    WSP_DLL_EXPORT void GetMidiEventList( 
            wsp::Vector<WSP_MidiEvent> *o_eventlist,
            const WSP_MidiTrackChunk &track_chunk 
        );

    /**
     * @brief Convert MIDI noteon event to image sequence.
     */
    WSP_DLL_EXPORT void MidiNoteonToSequence( 
            wsp::ImageSeq<u8> *o_imgseq,
            const char *in_midi_path,
            f32 fps
        );

    /**
     * @brief Convert MIDI noteon event to image sequence.
     */
    WSP_DLL_EXPORT wsp::Result PlaceImageSequenceOnMidiNoteonFrame( 
            wsp::ImageSeq<u8> *o_imgseq,
            const wsp::Vector<WSP_MidiEvent> &rithm_track,
            u32 bpm,
            f32 fps,
            const wsp::ImageSeq<u8> &in_imgseq_to_place
        );

    WSP_DLL_EXPORT void PlaceImageSequenceOnNoteonFrame(
            wsp::Image<u8> *o_image, 
            const wsp::Vector<WSP_MidiEvent> &rithm_track,
            u32 bpm,
            f32 fps,
            s32 target_frame,
            const wsp::ImageSeq<u8> &imgseq_to_place
        );

    /**
     * @brief Convert MIDI noteon event to image sequence.
     */
    WSP_DLL_EXPORT void FillUpMidiNoteonFrame( 
            wsp::Image<u8> *o_image, 
            const wsp::Vector<WSP_MidiEvent> &tempo_track,
            const wsp::Vector<WSP_MidiEvent> &rithm_track,
            s32 target_frame,
            f32 fps,
            wsp::Color<u8>& fill_color = wsp::Color<u8>( 0xff, 0xff, 0xff )
        );

    /**
     * @brief Get total time of MIDI track chunk data as seconds.
     */
    WSP_DLL_EXPORT u32 GetTotalDeltaTimeOfMidiEvents( const wsp::Vector<WSP_MidiEvent> &track );

    WSP_DLL_EXPORT u32 GetFirstTempo( const wsp::Vector<WSP_MidiEvent> &tempo_track );
    inline u32 GetFirstBpm( const wsp::Vector<WSP_MidiEvent> &tempo_track )
    {
        u32 tempo = wsp::sound::GetFirstTempo(tempo_track);
        return wsp::sound::GetBpmFromTempo( tempo );
    }
    inline f32 GetTotalSecondsOfMidiEvents( const wsp::Vector<WSP_MidiEvent> &track, u32 bpm )
    {
        u32 delta_time = wsp::sound::GetTotalDeltaTimeOfMidiEvents(track);
        return wsp::sound::ConvertMidiDeltaTimeToSecond( delta_time, bpm );
    }
    inline f32 GetTotalSecondsOfMidiEvents( const wsp::Vector<WSP_MidiEvent> &track, const wsp::Vector<WSP_MidiEvent> &tempo_track )
    {
        return wsp::sound::GetTotalSecondsOfMidiEvents( track, wsp::sound::GetFirstBpm( tempo_track ) );
    }

}}

#endif
