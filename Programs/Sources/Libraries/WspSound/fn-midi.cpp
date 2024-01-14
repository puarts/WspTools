/**
 * @file fn-midi.c
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef __WSP_COMMONCORE_CTYPE_H__
#include <wsp/common/_ctypes.h>
#endif
#include <wsp/sound/fn-midi.h>
#include <wsp/common/fn-debug.h>

MidiTrackChunkData* _AllocMidiTrackChunkData( size_t size )
{
    return (MidiTrackChunkData*)malloc( size );
}
void _FreeMidiTrackChunkData( MidiTrackChunkData **data )
{
    if( data )
    {
        free( *data );
        *data = NULL;
    }
}

WSP_MidiTrackChunk* _AllocMidiTrackChunk( u32 num_tracks )
{
    return (WSP_MidiTrackChunk*)malloc( num_tracks*sizeof(WSP_MidiTrackChunk) );
}
void _FreeMidiTrackChunk( WSP_MidiTrackChunk **data )
{
    if( data )
    {
        free( *data );
        *data = NULL;
    }
}

/*===============================================*/
/*===============================================*/
/*===============================================*/

int WSP_GetMidiNoteFromFrequency(float frequency_as_hertz)
{
    // 0.5f �͎l�̌ܓ��p
    return (int)(69.0f + 12.0f * log2f(frequency_as_hertz / 440.0f) + 0.5f);
}

void WSP_GetMidiNoteAsString(char *o_midi_note_as_string, int midi_note)
{
    if (midi_note <= 0)
    {
        strcpy(o_midi_note_as_string, "Invalid");
        return;
    }

    const char* ScaleAsString[] = {
        "C",
        "C#",
        "D",
        "D#",
        "E",
        "F",
        "F#",
        "G",
        "G#",
        "A",
        "A#",
        "B",
    };

    int scale_index = midi_note % 12;
    int octave = midi_note / 12 - 1; // ���ێ���-1�AYAMAHA����-2
    sprintf(o_midi_note_as_string, "%s%d", ScaleAsString[scale_index], octave);
}


void WSP_CopyMidiData( WSP_MidiData *o_data, const WSP_MidiData *in_data )
{
    u32 track_chunk_size;
    u32 num_tracks;
    WSP_MidiTrackChunk *trk_ptr, *end_ptr;

    /* free track chunks */
    if( o_data->track_chunks )
    {
        if( o_data->track_chunks->data )
        {
            _FreeMidiTrackChunkData( &o_data->track_chunks->data );
        }
        _FreeMidiTrackChunk( &o_data->track_chunks );
    }

    
    track_chunk_size = in_data->track_chunks->size;
    num_tracks = in_data->num_tracks;

    /* allocate and copy track chunks */
    if( in_data->track_chunks )
    {
        /* allocate track chunk memory */
        o_data->track_chunks = _AllocMidiTrackChunk( num_tracks );

        trk_ptr = in_data->track_chunks;
        end_ptr = trk_ptr + num_tracks;
        for( ; trk_ptr!=end_ptr; ++trk_ptr )
        {
            /* allocate data memory */
            if( in_data->track_chunks->data )
            {
                o_data->track_chunks->data = _AllocMidiTrackChunkData( track_chunk_size );
                /* copy */
                memcpy( o_data->track_chunks->data, in_data->track_chunks->data, track_chunk_size );
                o_data->track_chunks->size = in_data->track_chunks->size;
                memcpy( o_data->track_chunks->type, in_data->track_chunks->type, SIZE_OF_CHUNK_TYPE );
            }
            else
            {
                o_data->track_chunks->data = NULL;
            }
        }
    }
    else
    {
        o_data->track_chunks = NULL;
    }

    /* copy WSP_MidiData members */
    o_data->division = in_data->division;
    o_data->header_chunk_size = in_data->header_chunk_size;
    memcpy( o_data->header_chunk_type, in_data->header_chunk_type, SIZE_OF_CHUNK_TYPE);
    o_data->num_tracks = in_data->num_tracks;
    o_data->smf_format = in_data->smf_format;
}

bool WSP_IsEqualMidiData( WSP_MidiData *o_data, const WSP_MidiData *in_data )
{
    u32 track_chunk_size;
    u32 num_tracks;
    WSP_MidiTrackChunk *trk_ptr, *end_ptr;
    bool isEqual = true;

    track_chunk_size = in_data->track_chunks->size;
    num_tracks = in_data->num_tracks;

    /* compare track chunks */
    if( in_data->track_chunks )
    {
        trk_ptr = in_data->track_chunks;
        end_ptr = trk_ptr + num_tracks;
        for( ; trk_ptr!=end_ptr; ++trk_ptr )
        {
            if( in_data->track_chunks->data )
            {
                isEqual = isEqual
                  && (memcmp( o_data->track_chunks->data, in_data->track_chunks->data, track_chunk_size )==0)
                  && (o_data->track_chunks->size == in_data->track_chunks->size)
                  && (memcmp( o_data->track_chunks->type, in_data->track_chunks->type, SIZE_OF_CHUNK_TYPE )==0);
                if( isEqual==false )
                {
                    return false;
                }
            }
            else
            {
                isEqual = isEqual && ( o_data->track_chunks->data == NULL );
                if( isEqual==false )
                {
                    return false;
                }
            }
        }
    }
    else
    {
        isEqual = isEqual && (o_data->track_chunks==NULL);
        if( isEqual==false )
        {
            return false;
        }
    }

    return isEqual
        && (o_data->division == in_data->division)
        && (o_data->header_chunk_size == in_data->header_chunk_size)
        && (memcmp( o_data->header_chunk_type, in_data->header_chunk_type, SIZE_OF_CHUNK_TYPE)==0)
        && (o_data->num_tracks == in_data->num_tracks)
        && (o_data->smf_format == in_data->smf_format);
}


enum WSP_MidiEventType WSP_GetMidiEventType( u8 status_byte )
{
    switch ( (int)(status_byte & 0xF0) )
    {
    case 0x80: return WSP_MIDI_EVENT_TYPE_NOTEOFF;
    case 0x90: return WSP_MIDI_EVENT_TYPE_NOTEON;
    case 0xA0: return WSP_MIDI_EVENT_TYPE_POLYPHONIC_KEY_PRESSURE;
    case 0xB0: return WSP_MIDI_EVENT_TYPE_CONTROL_CHANGE;
    case 0xC0: return WSP_MIDI_EVENT_TYPE_PROGRAM_CHANGE;
    case 0xD0: return WSP_MIDI_EVENT_TYPE_CHANNEL_PRESSURE;
    case 0xE0: return WSP_MIDI_EVENT_TYPE_PITCH_BEND;
    case 0xF0:
        switch ( (int)status_byte )
        {
        case 0xF0: return WSP_MIDI_EVENT_TYPE_SYSTEM_EXCLUSIVE_MESSAGE_F0;
        case 0xF1: return WSP_MIDI_EVENT_TYPE_TIME_CODE;
        case 0xF2: return WSP_MIDI_EVENT_TYPE_SONG_POSITION_POINTER;
        case 0xF3: return WSP_MIDI_EVENT_TYPE_SONG_SELECT;
        case 0xF6: return WSP_MIDI_EVENT_TYPE_TUNE_REQUEST;
        case 0xF7: return WSP_MIDI_EVENT_TYPE_SYSTEM_EXCLUSIVE_MESSAGE_F7;
        case 0xF8: return WSP_MIDI_EVENT_TYPE_TIMING_CLOCK;
        case 0xFA: return WSP_MIDI_EVENT_TYPE_START;
        case 0xFB: return WSP_MIDI_EVENT_TYPE_CONTINUE;
        case 0xFC: return WSP_MIDI_EVENT_TYPE_STOP;
        case 0xFE: return WSP_MIDI_EVENT_TYPE_ACTIVE_SENSING;
        case 0xFF: return WSP_MIDI_EVENT_TYPE_META_EVENT;
        }
    }
    return WSP_MIDI_EVENT_TYPE_UNDEFINED;
}

enum WSP_MidiMetaEventType WSP_GetMidiMetaEventType( u8 meta_event_type_byte )
{
    switch ( (int)meta_event_type_byte )
    {
    case 0x00: return WSP_MIDI_META_EVENT_TYPE_SEQUENCE_NUMBER;
    case 0x01: return WSP_MIDI_META_EVENT_TYPE_TEXT;
    case 0x02: return WSP_MIDI_META_EVENT_TYPE_COPYRIGHT;
    case 0x03: return WSP_MIDI_META_EVENT_TYPE_SEQUENCE_NAME;
    case 0x04: return WSP_MIDI_META_EVENT_TYPE_INSTRUMENT;
    case 0x05: return WSP_MIDI_META_EVENT_TYPE_LYRICS;
    case 0x06: return WSP_MIDI_META_EVENT_TYPE_MARKER;
    case 0x07: return WSP_MIDI_META_EVENT_TYPE_CUE_POINT;
    case 0x08: return WSP_MIDI_META_EVENT_TYPE_PROGRAM_NAME;
    case 0x09: return WSP_MIDI_META_EVENT_TYPE_DEVICE_NAME;
    case 0x20: return WSP_MIDI_META_EVENT_TYPE_MIDI_CHANNEL_PREFIX;
    case 0x21: return WSP_MIDI_META_EVENT_TYPE_PORT;
    case 0x2F: return WSP_MIDI_META_EVENT_TYPE_END_OF_TRACK;
    case 0x51: return WSP_MIDI_META_EVENT_TYPE_TEMPO;
    case 0x54: return WSP_MIDI_META_EVENT_TYPE_SMPTE_OFFSET;
    case 0x58: return WSP_MIDI_META_EVENT_TYPE_TIME;
    case 0x59: return WSP_MIDI_META_EVENT_TYPE_KEY;
    case 0x7F: return WSP_MIDI_META_EVENT_TYPE_SEQUENCER_SPECIFIC;
    }
    return WSP_MIDI_META_EVENT_TYPE_UNKNOWN;
}



int ConvertEndian(void *input, size_t s){
    // �G���f�B�A���ϊ��������Ȃ��֐�
    // stdlib.h���C���N���[�h���Ă��������B
    // �y�����z: void *input...�G���f�B�A���ϊ��Ώۂւ̃|�C���^
    // �y�����z: size_t    s...�ϊ��Ώۂ̃o�C�g��

    int i;   // �J�E���^
    char *temp;   // �ϊ����ɗp����ꎞ�I�z��

    if((temp = (char *)calloc(s, sizeof(char))) == NULL){
        perror("Error: Cannot get memory for temp.");
        return 0;   // �̈�m�ۂł����i���s�j
    }

    for(i=0; i<s; i++){   // input�f�[�^��temp�Ɉꎞ�ۊ�
        temp[i] = ((char *)input)[i];
    }

    for(i=1; i<=s; i++){   // temp�f�[�^���t�����ɂ���input�֑��
        ((char *)input)[i-1] = temp[s-i];
    }

    free(temp);   // �m�ۂ����̈�����

    return 1;   // ����I��
}



void WSP_ReleaseMidiData( WSP_MidiData *io_midi_data )
{
    int i;
    if( io_midi_data->track_chunks!=NULL )
    {
        for( i=0; i<io_midi_data->num_tracks; ++i )
        {
            free( io_midi_data->track_chunks[i].data );
        }
    }

    if( io_midi_data!=NULL)
    {
        free( io_midi_data->track_chunks );
    }
    free( io_midi_data );
}

WSP_MidiData* WSP_ReadStandardMidiFile( const char *in_midi_path )
{
    int i, j, k, cnt;   // �J�E���^
    FILE *fp;         // �t�@�C���|�C���^����

    WSP_MidiData *dest_midi_data = (WSP_MidiData*)malloc(sizeof(WSP_MidiData));

    int endian; //   �G���f�B�A������ɂ��낢��g�p�i0:BigEndian, 1:LittleEndian�j

    // �G���f�B�A������
    endian = 1;
    if(*(char *)&endian){   // ���g���G���f�B�A���Ȃ�...
        endian = 1;   // Little Endian
    } else {   // �r�b�O�G���f�B�A���Ȃ�...
        endian = 0;   // Big Endian
    }

    // MIDI�t�@�C�����J��
    if((fp = fopen(in_midi_path, "rb")) == NULL){   // �o�C�i���ǂݎ�胂�[�h�Ńt�@�C�����J��
        perror("Error: Cannot open the file.\n");   // ���s������G���[��f��
        return 0;
    }

    // �w�b�_�`�����N�擾
    fread(((*dest_midi_data).header_chunk_type), 1, 4, fp);   // �`�����N�^�C�v
    fread(&((*dest_midi_data).header_chunk_size), 4, 1, fp);  // �`�����N�f�[�^�T�C�Y
    fread(&((*dest_midi_data).smf_format), 2, 1, fp);   // SMF�t�H�[�}�b�g�^�C�v
    fread(&((*dest_midi_data).num_tracks), 2, 1, fp);       // �g���b�N�`�����N����
    fread(&((*dest_midi_data).division), 2, 1, fp);     // ����\�i�f���^�^�C���j

    // �K�v�Ȃ�G���f�B�A���ϊ�
    if(endian){   // ���g���G���f�B�A���Ȃ�v�ϊ�
        // �G���f�B�A���ϊ�����
        ConvertEndian(&((*dest_midi_data).header_chunk_size), sizeof(((*dest_midi_data).header_chunk_size)));
        ConvertEndian(&((*dest_midi_data).smf_format), sizeof(((*dest_midi_data).smf_format)));
        ConvertEndian(&((*dest_midi_data).num_tracks), sizeof(((*dest_midi_data).num_tracks)));
        ConvertEndian(&((*dest_midi_data).division), sizeof(((*dest_midi_data).division)));
    }

    // �ǂݎ�����w�b�_�`�����N�����o��
    printf("# Header ========================\n"); 
    printf("header_chunk_type : %c%c%c%c\n", 
        ((*dest_midi_data).header_chunk_type)[0], 
        ((*dest_midi_data).header_chunk_type)[1], 
        ((*dest_midi_data).header_chunk_type)[2], 
        ((*dest_midi_data).header_chunk_type)[3]);
    printf("header_chunk_size : %d\n", ((*dest_midi_data).header_chunk_size));
    printf("smf_format : %hd\n", ((*dest_midi_data).smf_format));
    printf("num_tracks : %hd\n", ((*dest_midi_data).num_tracks));
    printf("division   : %hd\n", ((*dest_midi_data).division));


    // �g���b�N�`�����N�擾
    if((dest_midi_data->track_chunks = (WSP_MidiTrackChunk *)calloc(((*dest_midi_data).num_tracks), sizeof(WSP_MidiTrackChunk))) == NULL){   // �g���b�N���ɉ����ė̈�m��
        perror("Error: Cannot get memory for track_chunks.");
        return 0;   // �̈�m�ۂł����i���s�j
    }
    for(i=0; i<((*dest_midi_data).num_tracks); i++){   // �g���b�N�������J�Ԃ�
        fread(dest_midi_data->track_chunks[i].type, 1, 4, fp);   // �`�����N�^�C�v
        fread(&dest_midi_data->track_chunks[i].size, 4, 1, fp);   // �`�����N�f�[�^�T�C�Y
        if(endian){   // ���g���G���f�B�A���Ȃ�v�ϊ�
            ConvertEndian(&dest_midi_data->track_chunks[i].size, sizeof(dest_midi_data->track_chunks[i].size));
        }
        if((dest_midi_data->track_chunks[i].data = _AllocMidiTrackChunkData(dest_midi_data->track_chunks[i].size )) == NULL){   // �f�[�^�T�C�Y�ɉ����ė̈�m��
            perror("Error: Cannot get memory for track_chunks[i].data .");
            return 0;   // �̈�m�ۂł����i���s�j
        }
        fread(dest_midi_data->track_chunks[i].data, dest_midi_data->track_chunks[i].size, sizeof(char), fp);   // �f�[�^�i�C�x���g�̗���j
    }

    return dest_midi_data;
}



