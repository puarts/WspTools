/**
 * @file _midi_types.h
 * 
 */

#ifndef __WSP_SOUNDCORE_MIDI_TYPES_H__
#define __WSP_SOUNDCORE_MIDI_TYPES_H__

#ifndef __WSP_COMMONCORE_CTYPE_H__
#include <wsp/common/_ctypes.h>
#endif


typedef enum WSP_MidiEventType {
    WSP_MIDI_EVENT_TYPE_UNDEFINED,
    WSP_MIDI_EVENT_TYPE_NOTEOFF,
    WSP_MIDI_EVENT_TYPE_NOTEON,
    WSP_MIDI_EVENT_TYPE_POLYPHONIC_KEY_PRESSURE,
    WSP_MIDI_EVENT_TYPE_CONTROL_CHANGE,
    WSP_MIDI_EVENT_TYPE_PROGRAM_CHANGE,
    WSP_MIDI_EVENT_TYPE_CHANNEL_PRESSURE,
    WSP_MIDI_EVENT_TYPE_PITCH_BEND,
    WSP_MIDI_EVENT_TYPE_SYSTEM_EXCLUSIVE_MESSAGE_F0,
    WSP_MIDI_EVENT_TYPE_TIME_CODE,
    WSP_MIDI_EVENT_TYPE_SONG_POSITION_POINTER,
    WSP_MIDI_EVENT_TYPE_SONG_SELECT,
    WSP_MIDI_EVENT_TYPE_TUNE_REQUEST,
    WSP_MIDI_EVENT_TYPE_SYSTEM_EXCLUSIVE_MESSAGE_F7,
    WSP_MIDI_EVENT_TYPE_TIMING_CLOCK,
    WSP_MIDI_EVENT_TYPE_START,
    WSP_MIDI_EVENT_TYPE_CONTINUE,
    WSP_MIDI_EVENT_TYPE_STOP,
    WSP_MIDI_EVENT_TYPE_ACTIVE_SENSING,
    WSP_MIDI_EVENT_TYPE_META_EVENT,
} WSP_MidiEventType;

typedef enum WSP_MidiMetaEventType {
    WSP_MIDI_META_EVENT_TYPE_UNKNOWN,
    WSP_MIDI_META_EVENT_TYPE_SEQUENCE_NUMBER,
    WSP_MIDI_META_EVENT_TYPE_TEXT,
    WSP_MIDI_META_EVENT_TYPE_COPYRIGHT,
    WSP_MIDI_META_EVENT_TYPE_SEQUENCE_NAME,
    WSP_MIDI_META_EVENT_TYPE_INSTRUMENT,
    WSP_MIDI_META_EVENT_TYPE_LYRICS,
    WSP_MIDI_META_EVENT_TYPE_MARKER,
    WSP_MIDI_META_EVENT_TYPE_CUE_POINT,
    WSP_MIDI_META_EVENT_TYPE_PROGRAM_NAME,
    WSP_MIDI_META_EVENT_TYPE_DEVICE_NAME,
    WSP_MIDI_META_EVENT_TYPE_MIDI_CHANNEL_PREFIX,
    WSP_MIDI_META_EVENT_TYPE_PORT,
    WSP_MIDI_META_EVENT_TYPE_END_OF_TRACK ,
    WSP_MIDI_META_EVENT_TYPE_TEMPO        ,
    WSP_MIDI_META_EVENT_TYPE_SMPTE_OFFSET,
    WSP_MIDI_META_EVENT_TYPE_TIME,
    WSP_MIDI_META_EVENT_TYPE_KEY,
    WSP_MIDI_META_EVENT_TYPE_SEQUENCER_SPECIFIC
} WSP_MidiMetaEventType;

typedef u8 MidiTrackChunkData;
#define SIZE_OF_CHUNK_TYPE 4

// @brief トラックチャンクのデータを格納する構造体
typedef struct {
    // @brief チャンクタイプを示す文字列を格納。「MTrk」が入るはず。[4byte]
    u8 type[SIZE_OF_CHUNK_TYPE]; 
    // @brief トラックチャンクデータのサイズ [4byte]
    u32 size;     
    // @brief トラックデータ（イベントの羅列）へのポインタ
    MidiTrackChunkData *data;   
} WSP_MidiTrackChunk;

typedef struct {
    // @brief チャンクタイプを示す文字列を格納。「MThd」が入るはず。[4byte]
    u8  header_chunk_type[SIZE_OF_CHUNK_TYPE]; 
    // @brief ヘッダチャンクデータのサイズ [4byte]
    u32 header_chunk_size;
    // @brief SMFのフォーマットタイプ（0か1か2） [2byte]
    u16 smf_format;     
    // @brief トラックチャンク総数 [2byte]
    u16 num_tracks;      
    // @brief 四分音符あたりの分解能（ここではデルタタイム） [2byte]
    short division;       
    WSP_MidiTrackChunk *track_chunks;
} WSP_MidiData;

#ifdef __cplusplus
namespace wsp{ namespace sound{
    typedef WSP_MidiEventType     MidiEventType;
    typedef WSP_MidiMetaEventType MidiMetaEventType;
    typedef WSP_MidiTrackChunk        TrackChunk;
    typedef WSP_MidiData          MidiData;
}}
#endif

#endif
