/**
 * @file fn-midiutil.cpp
 * 
 */

#include <wsp/common/tpl_cl-vector3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C"{
    #include <wsp/sound/fn-midi.h>
}

#include <wsp/sound/cl-midi.hpp>
#include <wsp/sound/fn-midiutil.hpp>
#include <wsp/sound/fn-sound_log.h>


#ifdef _WIN32
#include <Windows.h>
#elif defined __APPLE__
#include <CoreMIDI/CoreMIDI.h>
//#include <Foundation/NSString.h>
#endif



namespace{

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


    short mergeChar7bit(char x, char y){
        // char�̉�7bit������������short�^�ŏo��
        // �y�����z�F�����ΏۂƂȂ�2��char�^�ϐ�x, y
        short s;
        s = (unsigned char)x; // ��ʃo�C�g���ɓ���Ă���
        s <<= 7;              // 7bit���V�t�g
        s = (s | (unsigned char)(y & 0x7f));   // ���ʃo�C�g�̉�7bit�������B�B�B
        return s;
    }

    bool IsRunningStatus( u8 byte_data )
    {
        return byte_data & 0x80;
    }


    f32 GetFrameFromSec( f32 sec, f32 fps )
    {
        return sec*fps;
    }

    void DefaultLogFunc(const char* message)
    {
        WSP_SOUND_LOG(message);
    }
}

#ifdef __APPLE__

static CFStringRef GetEndpointDisplayName(MIDIEndpointRef endpoint)
{
    CFStringRef result = CFSTR(""); // default
    
    MIDIObjectGetStringProperty(endpoint, kMIDIPropertyDisplayName, &result);
    
    return result;
}

static CFStringRef GetMidiObjectDisplayName(MIDIObjectRef object)
{
    CFStringRef name = nil;
    if (noErr != MIDIObjectGetStringProperty(object, kMIDIPropertyName, &name))
        return nil;
    
    return name;
}

static bool GetMidiObjectDisplayName(char* o_name, size_t buffer_size, MIDIObjectRef object)
{
    CFStringRef name_ref = GetMidiObjectDisplayName(object);
    if (name_ref == nil)
    {
        return false;
    }
    
    GetMidiObjectDisplayName(object);
    CFStringGetCString(name_ref, o_name, buffer_size, kCFStringEncodingMacRoman);
    if (o_name == NULL)
    {
        return false;
    }
    
    return true;
}

#endif


void wsp::sound::PrintMidiInputDevices(wsp::sound::LogFunc log_func)
{
#ifdef _WIN32
    
    MMRESULT result;
    MIDIINCAPS midi_in_caps;
    const s32 BUFF_SIZE = 32;
    u32 num_devices =  midiInGetNumDevs();

    LogFunc log = log_func;
    if (log == nullptr) {
        log = DefaultLogFunc;
    }

    std::string message = "number of devices: " + std::to_string(num_devices);
    log(message.c_str());

    for( u32 dev_id=0; dev_id<num_devices; ++dev_id )
    {
        UINT cbMidiInCaps;
        result = midiInGetDevCaps(
                        dev_id,
                        &midi_in_caps,
                        sizeof(midi_in_caps)
                    );
        if (result != MMSYSERR_NOERROR) 
        {
            WSP_SOUND_ERROR_LOG("failed to get MIDI device of ID %u", dev_id);
            continue; 
        }

        message = "ID=" + std::to_string(dev_id) + ": " + midi_in_caps.szPname;
        log(message.c_str());
    }

#elif defined __APPLE__
    
    ItemCount num_devices = MIDIGetNumberOfDevices();
    WSP_SOUND_LOG("number of devices: %lu", num_devices);
    
    for (ItemCount i = 0; i < num_devices; ++i)
    {
        char name[256];
        MIDIDeviceRef device = MIDIGetDevice(i);
        bool result = GetMidiObjectDisplayName(name, 256, device);
        if (result == false)
        {
            WSP_SOUND_ERROR_LOG("failed to get MIDI device name of ID %lu", i);
            continue;
        }
        
        WSP_SOUND_LOG("ID=%lu: %s", i, name);
    }
    
    
//    // How many MIDI devices do we have?
//    ItemCount deviceCount = MIDIGetNumberOfDevices();
//    
//    // Iterate through all MIDI devices
//    for (ItemCount i = 0 ; i < deviceCount ; ++i) {
//        
//        // Grab a reference to current device
//        MIDIDeviceRef device = MIDIGetDevice(i);
//        NSLog(@"Device: %@", getName(device));
//        
//        // Is this device online? (Currently connected?)
//        SInt32 isOffline = 0;
//        MIDIObjectGetIntegerProperty(device, kMIDIPropertyOffline, &isOffline);
//        NSLog(@"Device is online: %s", (isOffline ? "No" : "Yes"));
//        
//        // How many entities do we have?
//        ItemCount entityCount = MIDIDeviceGetNumberOfEntities(device);
//        
//        // Iterate through this device's entities
//        for (ItemCount j = 0 ; j < entityCount ; ++j) {
//            
//            // Grab a reference to an entity
//            MIDIEntityRef entity = MIDIDeviceGetEntity(device, j);
//            NSLog(@"  Entity: %@", getName(entity));
//            
//            // Iterate through this device's source endpoints (MIDI In)
//            ItemCount sourceCount = MIDIEntityGetNumberOfSources(entity);
//            for (ItemCount k = 0 ; k < sourceCount ; ++k) {
//                
//                // Grab a reference to a source endpoint
//                MIDIEndpointRef source = MIDIEntityGetSource(entity, k);
//                NSLog(@"    Source: %@", getName(source));
//            }
//            
//            // Iterate through this device's destination endpoints
//            ItemCount destCount = MIDIEntityGetNumberOfDestinations(entity);
//            for (ItemCount k = 0 ; k < destCount ; ++k) {
//                
//                // Grab a reference to a destination endpoint
//                MIDIEndpointRef dest = MIDIEntityGetDestination(entity, k);
//                NSLog(@"    Destination: %@", getName(dest));
//            }
//        }
//        NSLog(@"------");
//    }
    
    
    
    
//    ItemCount num_devices = MIDIGetNumberOfSources();
//    WSP_SOUND_LOG("number of devices: %lu", num_devices);
//    
//    for (ItemCount i = 0; i < num_devices; ++i)
//    {
//        MIDIEndpointRef sourceEndpointRef = MIDIGetSource(i);
//        CFStringRef name_ref = GetEndpointDisplayName(sourceEndpointRef);
//        const char* name = CFStringGetCStringPtr(name_ref, kCFStringEncodingUTF8);
//        if (name == NULL)
//        {
//            WSP_SOUND_ERROR_LOG("failed to get MIDI device of ID %lu", i);
//            continue;
//        }
//        WSP_SOUND_LOG("ID=%lu: %s", i, name);
//    }

#else
    WSP_FATAL_NOT_IMPLEMENTED;
#endif
}

u32 wsp::sound::GetFirstTempo( const wsp::Vector<WSP_MidiEvent> &tempo_track )
{
    u32 tempo = 0;

    /* get tempo */
    const WSP_MidiEvent *tempo_ptr = tempo_track.data();
    const WSP_MidiEvent *tempo_end = tempo_ptr + tempo_track.length();
    for( ; tempo_ptr!=tempo_end; ++tempo_ptr)
    {
        if( tempo_ptr->type==WSP_MIDI_EVENT_TYPE_META_EVENT )
        {
            WSP_SOUND_LOG("data len = %d\n", tempo_ptr->data.length());
            if( tempo_ptr->data.length()>0 )
            {
                WSP_MidiMetaEventType meta_type = WSP_GetMidiMetaEventType( tempo_ptr->data[0] );
                WSP_SOUND_LOG("meta type = %d (%u)\n", meta_type, tempo_ptr->data[0]);
                if( meta_type==WSP_MIDI_META_EVENT_TYPE_TEMPO )
                {
                    tempo = tempo_ptr->tempo;
                    return tempo;
                }
            }
        }
    }
    return 0;
}

void wsp::sound::PlaceImageSequenceOnNoteonFrame(
    wsp::Image<u8> *o_image, 
    const wsp::Vector<WSP_MidiEvent> &rithm_track,
    u32 bpm,
    f32 fps,
    s32 target_frame,
    const wsp::ImageSeq<u8> &imgseq_to_place
)
{
    u32 target_delta_time = ConvertSecondToMidiDeltaTime( target_frame/fps, bpm );
    u32 tolerant_time = ConvertSecondToMidiDeltaTime( 1.0f/fps, bpm )/2;
    u32 target_range_max = target_delta_time + tolerant_time;
    u32 target_range_min = target_delta_time - tolerant_time;
    if(target_delta_time < tolerant_time)
    {
        target_range_min = 0;
    }

    WSP_SOUND_DEBUG_LOG(
            "\n"
            "bpm               = %u\n"
            "tolerant_time     = %u\n"
            "target_delta_time = %u (%u - %u)\n"
            , bpm
            , tolerant_time
            , target_delta_time, target_range_min, target_range_max
        );


    {
        const WSP_MidiEvent *event_ptr = rithm_track.data();
        const WSP_MidiEvent *event_end = event_ptr + rithm_track.length();
        u32 current_time = 0;
        for( ; event_ptr<event_end; ++event_ptr )
        {
            current_time += event_ptr->delta_time;

            if( (event_ptr->type != WSP_MIDI_EVENT_TYPE_NOTEON) )
            {
                continue;
            }


            if( current_time > target_range_max )
            {
                /* is over the target frame */
                break;
            }

            if( target_range_min <= current_time && current_time <= target_range_max )
            {
                f32 noteon_sec = ConvertMidiDeltaTimeToSecond(current_time, bpm);
                f32 noteon_frame = GetFrameFromSec(noteon_sec, fps);
                //WSP_SOUND_DEBUG_LOG("current_time = %u", current_time);
                WSP_SOUND_DEBUG_LOG("noteon_sec   = %f sec", noteon_sec);
                //WSP_SOUND_DEBUG_LOG("noteon_frame = %f frame", noteon_frame);
                WSP_SOUND_DEBUG_LOG("FOUND");

                /* is target frame */
                //o_image->ResetElems( fill_color.data(), 3 );
                return;
            }
        }
        f32 noteon_sec = ConvertMidiDeltaTimeToSecond(current_time, bpm);
        WSP_SOUND_DEBUG_LOG("noteon_sec   = %f sec", noteon_sec);
        WSP_SOUND_DEBUG_LOG("NOT FOUND");
    }
}

void wsp::sound::FillUpMidiNoteonFrame( 
            wsp::Image<u8> *o_image, 
            const wsp::Vector<WSP_MidiEvent> &tempo_track,
            const wsp::Vector<WSP_MidiEvent> &rithm_track,
            s32 target_frame,
            f32 fps,
            wsp::Color<u8>& fill_color
)
{
    u32 tempo = wsp::sound::GetFirstTempo( tempo_track );
    f32 bpm = GetBpmFromTempo( tempo );
    u32 target_delta_time = ConvertSecondToMidiDeltaTime( target_frame/fps, bpm );
    u32 tolerant_time = ConvertSecondToMidiDeltaTime( 1.0f/fps, bpm )/2;
    u32 target_range_max = target_delta_time + tolerant_time;
    u32 target_range_min = target_delta_time - tolerant_time;
    if(target_delta_time < tolerant_time)
    {
        target_range_min = 0;
    }

    WSP_SOUND_DEBUG_LOG(
            "\n"
            "tempo             = %u\n"
            "bpm               = %f\n"
            "tolerant_time     = %u\n"
            "target_delta_time = %u (%u - %u)\n"
            , tempo
            , bpm
            , tolerant_time
            , target_delta_time, target_range_min, target_range_max
        );


    {
        const WSP_MidiEvent *event_ptr = rithm_track.data();
        const WSP_MidiEvent *event_end = event_ptr + rithm_track.length();
        u32 current_time = 0;
        for( ; event_ptr<event_end; ++event_ptr )
        {
            current_time += event_ptr->delta_time;

            if( (event_ptr->type != WSP_MIDI_EVENT_TYPE_NOTEON) )
            {
                continue;
            }


            if( current_time > target_range_max )
            {
                /* is over the target frame */
                break;
            }

            if( target_range_min <= current_time && current_time <= target_range_max )
            {
                f32 noteon_sec = ConvertMidiDeltaTimeToSecond(current_time, bpm);
                f32 noteon_frame = GetFrameFromSec(noteon_sec, fps);
                //WSP_SOUND_DEBUG_LOG("current_time = %u", current_time);
                WSP_SOUND_DEBUG_LOG("noteon_sec   = %f sec", noteon_sec);
                //WSP_SOUND_DEBUG_LOG("noteon_frame = %f frame", noteon_frame);
                WSP_SOUND_DEBUG_LOG("FOUND");
                /* is target frame */
                o_image->ResetElems( fill_color.data(), 3 );
                return;
            }
        }
        f32 noteon_sec = ConvertMidiDeltaTimeToSecond(current_time, bpm);
        WSP_SOUND_DEBUG_LOG("noteon_sec   = %f sec", noteon_sec);
        WSP_SOUND_DEBUG_LOG("NOT FOUND");
    }
}



u32 wsp::sound::GetTotalDeltaTimeOfMidiEvents( const wsp::Vector<WSP_MidiEvent> &track )
{
    const WSP_MidiEvent *event_ptr = track.data();
    const WSP_MidiEvent *event_end = event_ptr + track.length();
    u32 total_delta_time = 0;
    for( ; event_ptr<event_end; ++event_ptr )
    {
        total_delta_time += event_ptr->delta_time;
    }
    WSP_SOUND_DEBUG_LOG("total_delta_time = %u, ", total_delta_time);
    return total_delta_time;
}

void wsp::sound::GetMidiEventList( 
    wsp::Vector<WSP_MidiEvent> *o_eventlist,
    const WSP_MidiTrackChunk &track_chunk 
)
{
    int i, j, k, cnt;   // �J�E���^
    unsigned char c;
    unsigned char status;   // �X�e�[�^�X�o�C�g�p�̈ꎞ�ϐ�
    unsigned int delta;   // �f���^�^�C��

    WSP_SOUND_LOG(
        "# Track info =====================\n"
        "track_chunk.type : %c%c%c%c\n"
        "track_chunk.size : %d\n"
        "track_chunk.data\n"
        , track_chunk.type[0], track_chunk.type[1], track_chunk.type[2], track_chunk.type[3]
    , track_chunk.size
        );
    u32 bpm = 120;

    u32 total_time = 0;
    for(j=0; j<track_chunk.size; j++)
    {   // �f�[�^�������J�Ԃ�
        WSP_MidiEvent midi_event;
        delta = 0;   // ������
        u32 delta_byte = 0;
        while((c = (unsigned char)track_chunk.data[j++]) & 0x80)
        {
            delta = delta | (c & 0x7F);   // ����
            delta <<= 7;   // 7bit���V�t�g

            delta_byte = delta_byte | c;
            delta_byte <<= 8;
        }
        delta = delta | c;   // ����
        delta_byte = delta_byte | c;
        total_time += delta;
        midi_event.delta_time = delta;

        printf("(%08x) ", delta_byte);
        printf("%7d(%.2f sec) %7d:: ", total_time, ConvertMidiDeltaTimeToSecond(total_time, bpm), delta);   // �f���^�^�C���o��


        // �����j���O�X�e�[�^�X���[���ɑ΂��鏈��
        if((track_chunk.data[j] & 0x80) == 0x80){
            // �����j���O�X�e�[�^�X���[�����K�p����Ȃ��ꍇ�́A�X�e�[�^�X�o�C�g�p�ϐ����X�V�B
            status = (unsigned char)track_chunk.data[j];   // �X�e�[�^�X�o�C�g��ێ����Ă���
        } else {
            printf("@");   // �����j���O�X�e�[�^�X���[���K�p�̂��邵
            --j;   // �f�[�^�o�C�g�̒��O�̃o�C�g�i�f���^�^�C�����ȁH�j���w���悤�ɂ��Ă����B
            // ���̏�����j++����͂��Ȃ̂ŁA��������΃f�[�^�o�C�g�ɃA�N�Z�X�ł���B
        }


        WSP_MidiEventType event_type = WSP_GetMidiEventType( status );
        midi_event.type = event_type;

        // �f�[�^����
        printf(" (%02x) ", status);

        int data_len = 0;

        switch( event_type )
        {
        case WSP_MIDI_EVENT_TYPE_NOTEOFF:
            {
                ++j;
                u8 note = (unsigned char)track_chunk.data[j];
                ++j;
                u8 velocity = (unsigned char)track_chunk.data[j];

                WSP_SOUND_LOG("Note Off [%02dch] : ", (status & 0x0f));
                printf("Note %d", note);
                printf("[0x%02x] ", note);
                printf("Velocity=%d", velocity);

                midi_event.data.Append(note);
                midi_event.data.Append(velocity);
                midi_event.note = note;
                midi_event.velocity = velocity;
            }
            break;
        case WSP_MIDI_EVENT_TYPE_NOTEON:
            {
                ++j;
                u8 note = (unsigned char)track_chunk.data[j];
                ++j;
                u8 velocity = (unsigned char)track_chunk.data[j];

                WSP_SOUND_LOG("Note On  [%02dch] : ", (status & 0x0f));
                printf("Note %d", note);
                printf("[0x%02x] ", note);
                printf("Velocity=%d", velocity);

                midi_event.data.Append(note);
                midi_event.data.Append(velocity);
                midi_event.note = note;
                midi_event.velocity = velocity;
            }
            break;
        case WSP_MIDI_EVENT_TYPE_POLYPHONIC_KEY_PRESSURE:
            {
                ++j;
                u8 note = (unsigned char)track_chunk.data[j];
                ++j;
                u8 key_pressure = (unsigned char)track_chunk.data[j];

                WSP_SOUND_LOG("Polyphonic Key Pressure [%02dch] : ", (status & 0x0f));
                printf("Note %d", note);
                printf("[0x%02x] ", note);
                printf("Pressure=%d", key_pressure);

                midi_event.data.Append(note);
                midi_event.data.Append(key_pressure);
                midi_event.note = note;
                midi_event.key_pressure = key_pressure;
            }
            break;
        case WSP_MIDI_EVENT_TYPE_CONTROL_CHANGE:
            ++j;
            {
                WSP_SOUND_LOG("Control Change [%02dch] : ", (status & 0x0f));
                c = (unsigned char)track_chunk.data[j];
                ++j;
                if(c<=63){   // �A���σ^�C�v�̃G�t�F�N�g�Ɋւ���R���g���[�����iMSB��LSB���j
                    // �i�z���g�́u0<=c && c<=63�v�Ə����������ǁAwarning���o��̂Łuc<=63�v�ɂ���j
                    printf("VariableEffect(");
                    switch(c){
                    case 0:    // ��ʃo�C�g[MSB]
                    case 32:   // ���ʃo�C�g[LSB]
                        printf("BankSelect[%s]", (c==0)?"MSB":"LSB");   // �o���N�E�Z���N�g
                        break;
                    case 1:
                    case 33:
                        printf("ModulationDepth[%s]", (c==1)?"MSB":"LSB");   // ���W�����[�V�����E�f�v�X
                        break;
                    case 2:
                    case 34:
                        printf("BreathType[%s]", (c==2)?"MSB":"LSB");   // �u���X�E�^�C�v
                        break;
                    case 4:
                    case 36:
                        printf("FootType[%s]", (c==4)?"MSB":"LSB");   // �t�b�g�E�^�C�v
                        break;
                    case 5:
                    case 37:
                        printf("PortamentoTime[%s]", (c==5)?"MSB":"LSB");   // �|���^�����g�E�^�C��
                        break;
                    case 6:
                    case 38:
                        printf("DataEntry[%s]", (c==6)?"MSB":"LSB");   // �f�[�^�E�G���g���[
                        break;
                    case 7:
                    case 39:
                        printf("MainVolume[%s]", (c==7)?"MSB":"LSB");   // ���C���E�{�����[��
                        break;
                    case 8:
                    case 40:
                        printf("BalanceControl[%s]", (c==8)?"MSB":"LSB");   // �o�����X�E�R���g���[��
                        break;
                    case 10:
                    case 42:
                        printf("Panpot[%s]", (c==10)?"MSB":"LSB");   // �p���|�b�g
                        break;
                    case 11:
                    case 43:
                        printf("Expression[%s]", (c==11)?"MSB":"LSB");   // �G�N�X�v���b�V����
                        break;
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                        printf("SomethingElse_No.%d[%s]", c, (c==16)?"MSB":"LSB");   // �ėp����q
                        break;
                    default:
                        printf("##UndefinedType_No.%d[%s]", (c<32)?c:c-32, (c<32)?"MSB":"LSB");   // �悭�킩��Ȃ��R���g���[��
                    }

                    printf(")=%d", (unsigned char)track_chunk.data[j]);
                } else if(64<=c && c<=95){   // �A���ςłȂ��^�C�v�̃G�t�F�N�g�Ɋւ���R���g���[�����
                    printf("InvariableEffect(");
                    switch(c){
                    case 64:
                        printf("Hold1(Damper)");   // �z�[���h�P�i�_���p�[�j
                        break;
                    case 65:
                        printf("Portamento");   // �|���^�����g
                        break;
                    case 66:
                        printf("Sostenuto");   // �\�X�e�k�[�g
                        break;
                    case 67:
                        printf("SoftPedal");   // �\�t�g�E�y�_��
                        break;
                    case 69:
                        printf("Hold2(Freeze)");   // �z�[���h�Q�i�t���[�Y�j
                        break;
                    case 71:
                        printf("HarmonicIntensity");   // �n�[���j�b�N�E�C���e���V�e�B
                        break;
                    case 72:
                        printf("ReleaseTime");   // �����[�X�E�^�C��
                        break;
                    case 73:
                        printf("AttackTime");   // �A�^�b�N�E�^�C��
                        break;
                    case 74:
                        printf("Brightness");   // �u���C�g�l�X
                        break;
                    case 80:
                    case 81:
                    case 82:
                    case 83:
                        printf("SomethingElse_No.%d", c);   // �ėp����q
                        break;
                    case 91:
                        printf("ExternalEffect");   // �O���G�t�F�N�g
                        break;
                    case 92:
                        printf("Tremolo");   // �g������
                        break;
                    case 93:
                        printf("Chorus");   // �R�[���X
                        break;
                    case 94:
                        printf("Celeste");   // �Z���X�e
                        break;
                    case 95:
                        printf("Phaser");   // �t�F�C�U�[
                        break;
                    default:
                        printf("##UndefinedType_No.%d", c);   // �悭�킩��Ȃ��R���g���[��
                    }
                    printf(")=%d", (unsigned char)track_chunk.data[j]);
                } else if(96<=c && c<=119){   // ����ȏ��
                    printf("SpecialPurpose(");
                    switch(c){
                    case 96:
                        printf("DataIncrement");   // �f�[�^�E�C���N�������g
                        break;
                    case 97:
                        printf("DataDecrement");   // �f�N�������g
                        break;
                    case 98:
                        printf("NRPN[LSB]");   // NRPN��LSB
                        break;
                    case 99:
                        printf("NRPN[MSB]");   // NRPN��MSB
                        break;
                    case 100:
                        printf("RPN[LSB]");   // RPN��LSB
                        break;
                    case 101:
                        printf("RPN[MSB]");   // RPN��MSB
                        break;
                    default:
                        printf("##UndefinedType_No.%d", c);   // �悭�킩��Ȃ��R���g���[��
                    }
                    printf(")=%d", (unsigned char)track_chunk.data[j]);
                } else if(120<=c && c<=127){   // ���[�h�E���b�Z�[�W
                    printf("ModeMessage(");
                    switch(c){
                    case 120:
                        printf("AllSoundOff");   // �I�[���E�T�E���h�E�I�t
                        break;
                    case 121:
                        printf("ResetAllController");   // ���Z�b�g�E�I�[���E�R���g���[���[
                        break;
                    case 122:
                        printf("LocalControl");   // ���[�J���E�R���g���[��
                        break;
                    case 123:
                        printf("AllNoteOff");   // �I�[���E�m�[�g�E�I�t
                        break;
                    case 124:
                        printf("OmniOn");   // �I���j�E�I��
                        break;
                    case 125:
                        printf("OmniOff");   // �I���j�E�I�t
                        break;
                    case 126:
                        printf("MonoModeOn");   // ���m���[�h�E�I���i�|�����[�h�E�I�t�j
                        break;
                    case 127:
                        printf("PolyModeOn");   // �|�����[�h�E�I���i���m���[�h�E�I�t�j
                        break;
                    default:
                        printf("##UndefinedType_No.%d", c);   // �悭�킩��Ȃ��R���g���[��
                    }
                    printf(")=%d", c, (unsigned char)track_chunk.data[j]);
                }
            }
            break;
        case WSP_MIDI_EVENT_TYPE_PROGRAM_CHANGE:
            WSP_SOUND_LOG("Program Change [%02dch] : ", (status & 0x0f));
            ++j;
            printf("ToneNo=%d", (unsigned char)track_chunk.data[j]);
            break;
        case WSP_MIDI_EVENT_TYPE_CHANNEL_PRESSURE:
            WSP_SOUND_LOG("Channel Pressure [%02dch] : ", (status & 0x0f));
            ++j;
            printf("Pressure=%d", (unsigned char)track_chunk.data[j]);
            break;
        case WSP_MIDI_EVENT_TYPE_PITCH_BEND:
            WSP_SOUND_LOG("Pitch Bend Change [%02dch] : ", (status & 0x0f));
            ++j;
            printf("Bend=%hd", mergeChar7bit(track_chunk.data[j+1], track_chunk.data[j]) - 8192);
            printf(" (LSB:%d", (unsigned char)track_chunk.data[j++]);
            printf(", MSB:%d)", (unsigned char)track_chunk.data[j]);
            break;
        case WSP_MIDI_EVENT_TYPE_SYSTEM_EXCLUSIVE_MESSAGE_F0:
            {
                WSP_SOUND_LOG("F0 Exclusive Message : ");
                ++j;

                // SysEx�̃f�[�^�����擾
                cnt = 0;   // ������
                while((c = (unsigned char)track_chunk.data[j++]) & 0x80){   // �t���O�r�b�g��1�̊Ԃ̓��[�v
                    cnt = cnt | c;   // ����
                    cnt <<= 7;   // 7bit���V�t�g
                }
                cnt = cnt | c;   // ����
                printf(" Length=%u", (unsigned int)cnt);   // SysEx�̃f�[�^�����擾

                for(k=1; k<=cnt; k++){   // �����̕������f�[�^�擾
                    printf("[%02x]", (unsigned char)track_chunk.data[j++]);
                }
                --j;   // �s���߂��������f�N�������g
            }
            break;
        case WSP_MIDI_EVENT_TYPE_TIME_CODE:
            //case 0x01:   // MIDI�^�C���R�[�h
            WSP_SOUND_LOG("MIDI Time Code : ");
            ++j;
            printf("(frame/sec/min/hour)=%d", (unsigned char)track_chunk.data[j]);
            break;
        case WSP_MIDI_EVENT_TYPE_SONG_POSITION_POINTER:
            //case 0x02:   // �\���O�E�|�W�V�����E�|�C���^
            WSP_SOUND_LOG("Song Position Pointer : ");
            ++j;
            printf("Position=%hd[MIDI beat]", mergeChar7bit(track_chunk.data[j+1], track_chunk.data[j]));
            printf(" (LSB:%d", (unsigned char)track_chunk.data[j++]);
            printf(", MSB:%d)", (unsigned char)track_chunk.data[j]);
            break;
        case WSP_MIDI_EVENT_TYPE_SONG_SELECT:
            //case 0x03:   // �\���O�E�Z���N�g
            WSP_SOUND_LOG("Song Select : ");
            ++j;
            printf("SelectNo=%d", (unsigned char)track_chunk.data[j]);
            break;
        case WSP_MIDI_EVENT_TYPE_TUNE_REQUEST:
            //case 0x06:   // �`���[���E���N�G�X�g
            WSP_SOUND_LOG("Tune Request");
            break;
        case WSP_MIDI_EVENT_TYPE_SYSTEM_EXCLUSIVE_MESSAGE_F7:
            {
                //printf("@End of Exclusive");
                WSP_SOUND_LOG("F7 Exclusive Message : ");
                ++j;

                // SysEx�̃f�[�^�����擾
                cnt = 0;   // ������
                while((c = (unsigned char)track_chunk.data[j++]) & 0x80){   // �t���O�r�b�g��1�̊Ԃ̓��[�v
                    cnt = cnt | c;   // ����
                    cnt <<= 7;   // 7bit���V�t�g
                }
                cnt = cnt | c;   // ����
                printf(" Length=%u", (unsigned int)cnt);   // SysEx�̃f�[�^�����擾

                for(k=1; k<=cnt; k++){   // �����̕������f�[�^�擾
                    printf("[%02x]", (unsigned char)track_chunk.data[j++]);
                }
                --j;   // �s���߂��������f�N�������g

                break;
            }
        case WSP_MIDI_EVENT_TYPE_TIMING_CLOCK:
            WSP_SOUND_LOG("Timing Clock");
            break;
        case WSP_MIDI_EVENT_TYPE_START:
            WSP_SOUND_LOG("Start");
            break;
        case WSP_MIDI_EVENT_TYPE_CONTINUE:
            WSP_SOUND_LOG("Continue");
            break;
        case WSP_MIDI_EVENT_TYPE_STOP:
            WSP_SOUND_LOG("Stop");
            break;
        case WSP_MIDI_EVENT_TYPE_ACTIVE_SENSING:
            WSP_SOUND_LOG("Active Sensing");
            break;
        case WSP_MIDI_EVENT_TYPE_META_EVENT:
            {
                ++j;
                u8 meta_type_byte = track_chunk.data[j];
                WSP_SOUND_LOG("Meta Event : (%02x) ", meta_type_byte);

                midi_event.data.Append( meta_type_byte );
                WSP_MidiMetaEventType meta_type = WSP_GetMidiMetaEventType( meta_type_byte );

                switch(meta_type)
                {
                case WSP_MIDI_META_EVENT_TYPE_SEQUENCE_NUMBER:   // �V�[�P���X�ԍ�
                    printf("Sequence Number=");
                    j += 2;   // �f�[�^���̕���ʂ�z��
                    data_len += 2;
                    cnt = (unsigned char)track_chunk.data[j];
                    midi_event.data.Append( track_chunk.data[j] );
                    ++j;
                    data_len++;
                    cnt <<= 8;   // 8bit���V�t�g
                    cnt = cnt | (unsigned char)track_chunk.data[j];
                    midi_event.data.Append( track_chunk.data[j] );
                    printf("%d", cnt);
                    break;
                case WSP_MIDI_META_EVENT_TYPE_TEXT:   // �e�L�X�g[�ϒ�]
                    printf("Text=");
                    cnt = 0;
                    j += 1;
                    ++data_len;
                    while((c = (unsigned char)track_chunk.data[j++]) & 0x80){
                        cnt = cnt | (c & 0x7F);   // ����
                        cnt <<= 7;   // 7bit���V�t�g
                        ++data_len;
                    }
                    ++data_len;
                    cnt = cnt | (c & 0x7F);   // ����
                    for(k=1; k<=cnt; k++){
                        printf("%c", track_chunk.data[j]);
                        ++j;
                        ++data_len;
                    }
                    --j;   // �s���߂��������f�N�������g
                    --data_len;
                    break;
                case WSP_MIDI_META_EVENT_TYPE_COPYRIGHT:   // ���쌠�\��[�ϒ�]
                    printf("Copyright=");
                    cnt = 0;
                    j += 1;
                    while((c = (unsigned char)track_chunk.data[j++]) & 0x80){
                        cnt = cnt | (c & 0x7F);   // ����
                        cnt <<= 7;   // 7bit���V�t�g
                    }
                    cnt = cnt | (c & 0x7F);   // ����
                    for(k=1; k<=cnt; k++){
                        printf("%c", track_chunk.data[j++]);
                    }
                    --j;   // �s���߂��������f�N�������g
                    break;
                case WSP_MIDI_META_EVENT_TYPE_SEQUENCE_NAME:   // �V�[�P���X���i�ȃ^�C�g���j�E�g���b�N��[�ϒ�]
                    printf("Title=");
                    cnt = 0;
                    j += 1;
                    while((c = (unsigned char)track_chunk.data[j++]) & 0x80){
                        cnt = cnt | (c & 0x7F);   // ����
                        cnt <<= 7;   // 7bit���V�t�g
                    }
                    cnt = cnt | (c & 0x7F);   // ����
                    for(k=1; k<=cnt; k++){
                        printf("%c", track_chunk.data[j++]);
                    }
                    --j;   // �s���߂��������f�N�������g
                    break;
                case WSP_MIDI_META_EVENT_TYPE_INSTRUMENT:   // �y�햼[�ϒ�]
                    printf("InstName=");
                    cnt = 0;
                    j += 1;
                    while((c = (unsigned char)track_chunk.data[j++]) & 0x80){
                        cnt = cnt | (c & 0x7F);   // ����
                        cnt <<= 7;   // 7bit���V�t�g
                    }
                    cnt = cnt | (c & 0x7F);   // ����
                    for(k=1; k<=cnt; k++){
                        printf("%c", track_chunk.data[j++]);
                    }
                    --j;   // �s���߂��������f�N�������g
                    break;
                case WSP_MIDI_META_EVENT_TYPE_LYRICS:   // �̎�[�ϒ�]
                    printf("Words=");
                    cnt = 0;
                    j += 1;
                    while((c = (unsigned char)track_chunk.data[j++]) & 0x80){
                        cnt = cnt | (c & 0x7F);   // ����
                        cnt <<= 7;   // 7bit���V�t�g
                    }
                    cnt = cnt | (c & 0x7F);   // ����
                    for(k=1; k<=cnt; k++){
                        printf("%c", track_chunk.data[j++]);
                    }
                    --j;   // �s���߂��������f�N�������g
                    break;
                case WSP_MIDI_META_EVENT_TYPE_MARKER:   // �}�[�J�[[�ϒ�]
                    printf("Marker=");
                    cnt = 0;
                    j += 1;
                    while((c = (unsigned char)track_chunk.data[j++]) & 0x80){
                        cnt = cnt | (c & 0x7F);   // ����
                        cnt <<= 7;   // 7bit���V�t�g
                    }
                    cnt = cnt | (c & 0x7F);   // ����
                    for(k=1; k<=cnt; k++){
                        printf("%c", track_chunk.data[j++]);
                    }
                    --j;   // �s���߂��������f�N�������g
                    break;
                case WSP_MIDI_META_EVENT_TYPE_CUE_POINT:   // �L���[�|�C���g[�ϒ�]
                    printf("CuePoint=");
                    cnt = 0;
                    j += 1;
                    while((c = (unsigned char)track_chunk.data[j++]) & 0x80){
                        cnt = cnt | (c & 0x7F);   // ����
                        cnt <<= 7;   // 7bit���V�t�g
                    }
                    cnt = cnt | (c & 0x7F);   // ����
                    for(k=1; k<=cnt; k++){
                        printf("%c", track_chunk.data[j++]);
                    }
                    --j;   // �s���߂��������f�N�������g
                    break;
                case WSP_MIDI_META_EVENT_TYPE_PROGRAM_NAME:   // �v���O�������i���F���j[�ϒ�]
                    printf("ProgramName=");
                    cnt = 0;
                    j += 1;
                    while((c = (unsigned char)track_chunk.data[j++]) & 0x80){
                        cnt = cnt | (c & 0x7F);   // ����
                        cnt <<= 7;   // 7bit���V�t�g
                    }
                    cnt = cnt | (c & 0x7F);   // ����
                    for(k=1; k<=cnt; k++){
                        printf("%c", track_chunk.data[j++]);
                    }
                    --j;   // �s���߂��������f�N�������g
                    break;
                case WSP_MIDI_META_EVENT_TYPE_DEVICE_NAME:   // �f�o�C�X���i�������j[�ϒ�]
                    printf("DeviceName=");
                    cnt = 0;
                    j += 1;
                    while((c = (unsigned char)track_chunk.data[j++]) & 0x80){
                        cnt = cnt | (c & 0x7F);   // ����
                        cnt <<= 7;   // 7bit���V�t�g
                    }
                    cnt = cnt | (c & 0x7F);   // ����
                    for(k=1; k<=cnt; k++){
                        printf("%c", track_chunk.data[j++]);
                    }
                    --j;   // �s���߂��������f�N�������g
                    break;
                case WSP_MIDI_META_EVENT_TYPE_MIDI_CHANNEL_PREFIX:   // MIDI�`�����l���v���t�B�b�N�X[1byte]
                    printf("MidiChannelPrefix=");
                    j += 2;   // �f�[�^���̕���ʂ�z��
                    cnt = (unsigned char)track_chunk.data[j];
                    printf("%d", cnt);
                    break;
                case WSP_MIDI_META_EVENT_TYPE_PORT:   // �|�[�g�w��[1byte]
                    printf("Port=");
                    j += 2;   // �f�[�^���̕���ʂ�z��
                    cnt = (unsigned char)track_chunk.data[j];
                    printf("%d", cnt);
                    break;
                case WSP_MIDI_META_EVENT_TYPE_END_OF_TRACK:   // �g���b�N�I�[[0byte]
                    printf("End of Track");
                    j += 1;   // �f�[�^���̕���ʂ�z��
                    break;
                case WSP_MIDI_META_EVENT_TYPE_TEMPO:   // �e���|�ݒ�[3byte]
                    {
                        ++j;   // FF 51 03 �Ńe���|�����ʂ���̂�03�̕��C���N�������g

                        ++j;
                        u8 *tempo_ptr = (u8*)&track_chunk.data[j];
                        u32 tempo = ((u32)(*tempo_ptr)<<16) | ((u32)(*(tempo_ptr+1))<<8) | ((u32)(*(tempo_ptr+2)));
                        midi_event.data.Append( *tempo_ptr );
                        midi_event.data.Append( *(++tempo_ptr) );
                        midi_event.data.Append( *(++tempo_ptr) );
                        j += 2;

                        midi_event.tempo = tempo;

                        printf("Temp=%u (%u bpm)", tempo, GetBpmFromTempo( tempo ));
                    }
                    break;
                case WSP_MIDI_META_EVENT_TYPE_SMPTE_OFFSET:   // SMPTE�I�t�Z�b�g[5byte]
                    printf("SMPTE_Offset=");
                    j += 2;   // �f�[�^���̕���ʂ�z��
                    cnt = (unsigned char)track_chunk.data[j++];
                    switch(cnt & 0xC0){   // �t���[�����x
                    case 0x00:
                        printf("24fps");
                        break;
                    case 0x01:
                        printf("25fps");
                        break;
                    case 0x10:
                        printf("30fps(DropFrame)");
                        break;
                    case 0x11:
                        printf("30fps");
                        break;
                    }
                    printf(",Hour:%d", (cnt & 0x3F));   // ����
                    printf(",Min:%d", (unsigned char)track_chunk.data[j++]);   // ��
                    printf(",Sec:%d", (unsigned char)track_chunk.data[j++]);   // �b
                    printf(",Frame:%d", (unsigned char)track_chunk.data[j++]);   // �t���[��
                    printf(",SubFrame:%d", (unsigned char)track_chunk.data[j]);   // �T�u�t���[��
                    break;
                case WSP_MIDI_META_EVENT_TYPE_TIME:   // ���q�ݒ�[4byte]
                    printf("Rhythm=");
                    j += 2;   // �f�[�^���̕���ʂ�z��
                    printf("%d", (unsigned char)track_chunk.data[j++]);
                    cnt = 1;
                    for(k=0; k<=(int)track_chunk.data[j]; k++){   // ���q�̕�����Z�o����
                        cnt *= 2;   // 2�̗ݏ�
                    }
                    printf("/%d", cnt);
                    ++j;
                    printf(" ClockPerBeat=%d", (unsigned char)track_chunk.data[j++]);
                    printf(" 32NotePer4Note=%d", (unsigned char)track_chunk.data[j]);
                    break;
                case WSP_MIDI_META_EVENT_TYPE_KEY:   // ���ݒ�[2byte]
                    printf("Key=");
                    j += 2;   // �f�[�^���̕���ʂ�z��
                    cnt = (int)track_chunk.data[j++];
                    if(c > 0){
                        printf("Sharp[%d]", c);
                    } else if(c == 0){
                        printf("C");
                    } else {
                        printf("Flat[%d]", c);
                    }
                    cnt = (int)track_chunk.data[j];
                    if(c == 0){
                        printf("_Major");
                    } else {
                        printf("_Minor");
                    }

                    break;
                case WSP_MIDI_META_EVENT_TYPE_SEQUENCER_SPECIFIC:   // �V�[�P���T���胁�^�C�x���g
                    printf("SpecialIvent=");
                    cnt = 0;
                    j += 1;
                    while((c = (unsigned char)track_chunk.data[j++]) & 0x80){
                        cnt = cnt | (c & 0x7F);   // ����
                        cnt <<= 7;   // 7bit���V�t�g
                    }
                    cnt = cnt | (c & 0x7F);   // ����
                    for(k=1; k<=cnt; k++){
                        printf("[%02x]", (unsigned char)track_chunk.data[j++]);
                    }
                    --j;   // �s���߂��������f�N�������g
                    break;
                default:
                    printf("## Undefined meta event..[%02x]", meta_type);   // ���m��ʃX�e�[�^�X�Ȃ�;
                }
            }
            break;
        default:
            WSP_SOUND_LOG("## Undefined midi event type...[Status:%02x]", status);   // ���m��ʃX�e�[�^�X�Ȃ�
        }
        printf("\n");
        o_eventlist->Append( midi_event );
    }
}

wsp::Result wsp::sound::PlaceImageSequenceOnMidiNoteonFrame( 
    wsp::ImageSeq<u8> *o_imgseq,
    const wsp::Vector<WSP_MidiEvent> &rithm_track,
    u32 bpm,
    f32 fps,
    const wsp::ImageSeq<u8> &in_imgseq_to_place
)
{
    WSP_SOUND_DEBUG_LOG("bpm = %u\n", bpm);

    u32 total_delta = wsp::sound::GetTotalDeltaTimeOfMidiEvents( rithm_track );
    WSP_SOUND_DEBUG_LOG("tatal_delata = %u", total_delta);

    f32 total_sec = wsp::sound::ConvertMidiDeltaTimeToSecond(total_delta, bpm);
    WSP_SOUND_DEBUG_LOG("tatal_sec = %f", total_sec);

    u32 total_frame = total_sec * fps  + in_imgseq_to_place.SeqSize();
    WSP_SOUND_DEBUG_LOG("tatal_frame = %u", total_frame);

    o_imgseq->SetSeqSize( total_frame );

    u32 width   = o_imgseq->width();
    u32 height  = o_imgseq->height();

    s32 shift_max_x = 3;
    s32 shift_max_y = height/20;
    f32 scale_max = 1.0f;
    f32 scale_min = 0.3f;


    s32 shift_x = -(s32)width/2;
    s32 shift_y = 0;
    s32 increment = height/70;

    const WSP_MidiEvent *event_ptr = rithm_track.data();
    const WSP_MidiEvent *event_end = event_ptr + rithm_track.length();
    u32 current_time = 0;
    for( ; event_ptr<event_end; ++event_ptr )
    {
        current_time += event_ptr->delta_time;
        f32 noteon_sec = ConvertMidiDeltaTimeToSecond(current_time, bpm);
        s32 frame = GetFrameFromSec(noteon_sec, fps);

        WSP_SOUND_DEBUG_LOG("current_time = %u (%d frame)", current_time, frame);

        if( (event_ptr->type != WSP_MIDI_EVENT_TYPE_NOTEON) )
        {
            continue;
        }

        s32 note = (event_ptr->note - 50) * 13;
        WSP_SOUND_DEBUG_LOG("Noteon %u event was found", note);

        /* is target frame */
        shift_x = (rand()/(f32)RAND_MAX) * shift_max_x - shift_max_x/2;
        shift_y = - note + height/2;//shift_y + (rand()/(f32)RAND_MAX) * shift_max_y - shift_max_y/2;
        f32 scale   = (rand()/(f32)RAND_MAX) * (scale_max - scale_min) + scale_min;
        u32 scaled_w = width *scale;
        u32 scaled_h = height *scale;
        WSP_SOUND_DEBUG_LOG( "shift_x = %d, shift_y = %d", shift_x, shift_y );
        WSP_SOUND_DEBUG_LOG( "scale = %f, scaled_w = %u, scaled_h = %u", scale, scaled_w, scaled_h );

        wsp::ImageSeq<u8> tmp_imgseq = in_imgseq_to_place;
        wsp::Image<u8> *img_ptr      = tmp_imgseq.GetRawData();
        wsp::Image<u8> *img_end_ptr  = img_ptr + tmp_imgseq.SeqSize();
        for( ; img_ptr<img_end_ptr && frame < total_frame; ++img_ptr, ++frame )
        {
            WSP_SOUND_DEBUG_LOG("frame = %d", frame);
            WSP_RETURN_IF_FAILED( 
                img_ptr->ScaleImage( scaled_w, scaled_h ));
            img_ptr->ResizeCanvas( width, height );
            WSP_RETURN_IF_FAILED( 
                img_ptr->Shift( shift_x + (width - scaled_w)/2, shift_y + (height - scaled_h)/2 ));
            WSP_RETURN_IF_FAILED( 
                (*o_imgseq)[frame].BlendImage( *img_ptr, WSP_IMAGE_BLEND_MODE_SCREEN ) );
        }
        //o_image->ResetElems( fill_color.data(), 3 );
    }
    return wsp::ResultSuccess();
}


void wsp::sound::MidiNoteonToSequence( 
            wsp::ImageSeq<uchar> *o_imgseq,
            const char *in_midi_path,
            f32 fps
)
{
    WSP_MidiData *midi_data = WSP_ReadStandardMidiFile(in_midi_path);
    WSP_MidiTrackChunk *track_chunks = midi_data->track_chunks;

    wsp::Vector<WSP_MidiEvent> tempo_track, rithm_track;
    wsp::sound::GetMidiEventList( &tempo_track, track_chunks[0] );
    wsp::sound::GetMidiEventList( &rithm_track, track_chunks[2] );

    u32 tempo = wsp::sound::GetFirstTempo( tempo_track );

    printf("tempo = %u\n", tempo);
    f32 bpm = GetBpmFromTempo( tempo );

    {
        /* get time for note on */
        WSP_MidiEvent *event_ptr = rithm_track.data();
        WSP_MidiEvent *event_end = event_ptr + rithm_track.length();
        u32 total_time = 0;
        for( ; event_ptr!=event_end; ++event_ptr)
        {
            total_time += event_ptr->delta_time;
            if( event_ptr->type==WSP_MIDI_EVENT_TYPE_NOTEON )
            {
                f32 sec = ConvertMidiDeltaTimeToSecond(total_time, bpm);
                printf("%f sec (%f frame)\n", sec, GetFrameFromSec(sec, fps) );
            }
        }
    }

    {
        /* get time for note on */
        wsp::Image<u8> image;
        image.SetSize( 640, 540 );
        WSP_MidiEvent *event_ptr = rithm_track.data();
        WSP_MidiEvent *event_end = event_ptr + rithm_track.length();
        u32 total_time = 0;
        char filename[1024];

        /* find first noteon event */
        while( event_ptr->type!=WSP_MIDI_EVENT_TYPE_NOTEON )
        {
            total_time += event_ptr->delta_time;
            ++event_ptr;
            if(event_ptr>=event_end){
                break;
            }
        }
        total_time += event_ptr->delta_time;

        for( int frame=0; event_ptr<event_end; ++frame)
        {
            while( event_ptr->type!=WSP_MIDI_EVENT_TYPE_NOTEON )
            {
                ++event_ptr;
                total_time += event_ptr->delta_time;
                if(event_ptr>=event_end){
                    break;
                }
            }
            if(event_ptr>=event_end){
                break;
            }

            f32 noteon_sec = ConvertMidiDeltaTimeToSecond(total_time, bpm);
            f32 noteon_frame = GetFrameFromSec(noteon_sec, fps);

            printf("%d frame, trigger %f sec (%f frame)\n", frame, noteon_sec, noteon_frame );
            if( (int)noteon_frame<=frame )
            {
                printf("triggered\n");
                image.Reset( 255 );
                ++event_ptr;
                total_time += event_ptr->delta_time;
            }
            else
            {
                image.Reset( 0 );
            }

            sprintf( filename, "release_of_frustration-%04d.png", (int)frame );
            o_imgseq->Append( image );
            //image.SaveImage(filename);
        }
    }

    WSP_ReleaseMidiData( midi_data );
}

