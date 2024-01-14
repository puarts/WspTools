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
        // エンディアン変換をおこなう関数
        // stdlib.hをインクルードしてください。
        // 【引数】: void *input...エンディアン変換対象へのポインタ
        // 【引数】: size_t    s...変換対象のバイト数

        int i;   // カウンタ
        char *temp;   // 変換時に用いる一時的配列

        if((temp = (char *)calloc(s, sizeof(char))) == NULL){
            perror("Error: Cannot get memory for temp.");
            return 0;   // 領域確保できず（失敗）
        }

        for(i=0; i<s; i++){   // inputデータをtempに一時保管
            temp[i] = ((char *)input)[i];
        }

        for(i=1; i<=s; i++){   // tempデータを逆方向にしてinputへ代入
            ((char *)input)[i-1] = temp[s-i];
        }

        free(temp);   // 確保した領域を解放

        return 1;   // 正常終了
    }


    short mergeChar7bit(char x, char y){
        // charの下7bitずつを結合してshort型で出力
        // 【引数】：結合対象となる2つのchar型変数x, y
        short s;
        s = (unsigned char)x; // 上位バイトを先に入れておく
        s <<= 7;              // 7bit左シフト
        s = (s | (unsigned char)(y & 0x7f));   // 下位バイトの下7bitを合成。。。
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
    int i, j, k, cnt;   // カウンタ
    unsigned char c;
    unsigned char status;   // ステータスバイト用の一時変数
    unsigned int delta;   // デルタタイム

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
    {   // データ分だけ繰返し
        WSP_MidiEvent midi_event;
        delta = 0;   // 初期化
        u32 delta_byte = 0;
        while((c = (unsigned char)track_chunk.data[j++]) & 0x80)
        {
            delta = delta | (c & 0x7F);   // 合成
            delta <<= 7;   // 7bit左シフト

            delta_byte = delta_byte | c;
            delta_byte <<= 8;
        }
        delta = delta | c;   // 合成
        delta_byte = delta_byte | c;
        total_time += delta;
        midi_event.delta_time = delta;

        printf("(%08x) ", delta_byte);
        printf("%7d(%.2f sec) %7d:: ", total_time, ConvertMidiDeltaTimeToSecond(total_time, bpm), delta);   // デルタタイム出力


        // ランニングステータスルールに対する処理
        if((track_chunk.data[j] & 0x80) == 0x80){
            // ランニングステータスルールが適用されない場合は、ステータスバイト用変数を更新。
            status = (unsigned char)track_chunk.data[j];   // ステータスバイトを保持しておく
        } else {
            printf("@");   // ランニングステータスルール適用のしるし
            --j;   // データバイトの直前のバイト（デルタタイムかな？）を指すようにしておく。
            // 次の処理でj++するはずなので、そうすればデータバイトにアクセスできる。
        }


        WSP_MidiEventType event_type = WSP_GetMidiEventType( status );
        midi_event.type = event_type;

        // データ判別
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
                if(c<=63){   // 連続可変タイプのエフェクトに関するコントロール情報（MSBもLSBも）
                    // （ホントは「0<=c && c<=63」と書きたいけど、warningが出るので「c<=63」にする）
                    printf("VariableEffect(");
                    switch(c){
                    case 0:    // 上位バイト[MSB]
                    case 32:   // 下位バイト[LSB]
                        printf("BankSelect[%s]", (c==0)?"MSB":"LSB");   // バンク・セレクト
                        break;
                    case 1:
                    case 33:
                        printf("ModulationDepth[%s]", (c==1)?"MSB":"LSB");   // モジュレーション・デプス
                        break;
                    case 2:
                    case 34:
                        printf("BreathType[%s]", (c==2)?"MSB":"LSB");   // ブレス・タイプ
                        break;
                    case 4:
                    case 36:
                        printf("FootType[%s]", (c==4)?"MSB":"LSB");   // フット・タイプ
                        break;
                    case 5:
                    case 37:
                        printf("PortamentoTime[%s]", (c==5)?"MSB":"LSB");   // ポルタメント・タイム
                        break;
                    case 6:
                    case 38:
                        printf("DataEntry[%s]", (c==6)?"MSB":"LSB");   // データ・エントリー
                        break;
                    case 7:
                    case 39:
                        printf("MainVolume[%s]", (c==7)?"MSB":"LSB");   // メイン・ボリューム
                        break;
                    case 8:
                    case 40:
                        printf("BalanceControl[%s]", (c==8)?"MSB":"LSB");   // バランス・コントロール
                        break;
                    case 10:
                    case 42:
                        printf("Panpot[%s]", (c==10)?"MSB":"LSB");   // パンポット
                        break;
                    case 11:
                    case 43:
                        printf("Expression[%s]", (c==11)?"MSB":"LSB");   // エクスプレッション
                        break;
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                        printf("SomethingElse_No.%d[%s]", c, (c==16)?"MSB":"LSB");   // 汎用操作子
                        break;
                    default:
                        printf("##UndefinedType_No.%d[%s]", (c<32)?c:c-32, (c<32)?"MSB":"LSB");   // よくわからないコントロール
                    }

                    printf(")=%d", (unsigned char)track_chunk.data[j]);
                } else if(64<=c && c<=95){   // 連続可変でないタイプのエフェクトに関するコントロール情報
                    printf("InvariableEffect(");
                    switch(c){
                    case 64:
                        printf("Hold1(Damper)");   // ホールド１（ダンパー）
                        break;
                    case 65:
                        printf("Portamento");   // ポルタメント
                        break;
                    case 66:
                        printf("Sostenuto");   // ソステヌート
                        break;
                    case 67:
                        printf("SoftPedal");   // ソフト・ペダル
                        break;
                    case 69:
                        printf("Hold2(Freeze)");   // ホールド２（フリーズ）
                        break;
                    case 71:
                        printf("HarmonicIntensity");   // ハーモニック・インテンシティ
                        break;
                    case 72:
                        printf("ReleaseTime");   // リリース・タイム
                        break;
                    case 73:
                        printf("AttackTime");   // アタック・タイム
                        break;
                    case 74:
                        printf("Brightness");   // ブライトネス
                        break;
                    case 80:
                    case 81:
                    case 82:
                    case 83:
                        printf("SomethingElse_No.%d", c);   // 汎用操作子
                        break;
                    case 91:
                        printf("ExternalEffect");   // 外部エフェクト
                        break;
                    case 92:
                        printf("Tremolo");   // トレモロ
                        break;
                    case 93:
                        printf("Chorus");   // コーラス
                        break;
                    case 94:
                        printf("Celeste");   // セレステ
                        break;
                    case 95:
                        printf("Phaser");   // フェイザー
                        break;
                    default:
                        printf("##UndefinedType_No.%d", c);   // よくわからないコントロール
                    }
                    printf(")=%d", (unsigned char)track_chunk.data[j]);
                } else if(96<=c && c<=119){   // 特殊な情報
                    printf("SpecialPurpose(");
                    switch(c){
                    case 96:
                        printf("DataIncrement");   // データ・インクリメント
                        break;
                    case 97:
                        printf("DataDecrement");   // デクリメント
                        break;
                    case 98:
                        printf("NRPN[LSB]");   // NRPNのLSB
                        break;
                    case 99:
                        printf("NRPN[MSB]");   // NRPNのMSB
                        break;
                    case 100:
                        printf("RPN[LSB]");   // RPNのLSB
                        break;
                    case 101:
                        printf("RPN[MSB]");   // RPNのMSB
                        break;
                    default:
                        printf("##UndefinedType_No.%d", c);   // よくわからないコントロール
                    }
                    printf(")=%d", (unsigned char)track_chunk.data[j]);
                } else if(120<=c && c<=127){   // モード・メッセージ
                    printf("ModeMessage(");
                    switch(c){
                    case 120:
                        printf("AllSoundOff");   // オール・サウンド・オフ
                        break;
                    case 121:
                        printf("ResetAllController");   // リセット・オール・コントローラー
                        break;
                    case 122:
                        printf("LocalControl");   // ローカル・コントロール
                        break;
                    case 123:
                        printf("AllNoteOff");   // オール・ノート・オフ
                        break;
                    case 124:
                        printf("OmniOn");   // オムニ・オン
                        break;
                    case 125:
                        printf("OmniOff");   // オムニ・オフ
                        break;
                    case 126:
                        printf("MonoModeOn");   // モノモード・オン（ポリモード・オフ）
                        break;
                    case 127:
                        printf("PolyModeOn");   // ポリモード・オン（モノモード・オフ）
                        break;
                    default:
                        printf("##UndefinedType_No.%d", c);   // よくわからないコントロール
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

                // SysExのデータ長を取得
                cnt = 0;   // 初期化
                while((c = (unsigned char)track_chunk.data[j++]) & 0x80){   // フラグビットが1の間はループ
                    cnt = cnt | c;   // 合成
                    cnt <<= 7;   // 7bit左シフト
                }
                cnt = cnt | c;   // 合成
                printf(" Length=%u", (unsigned int)cnt);   // SysExのデータ長を取得

                for(k=1; k<=cnt; k++){   // 長さの分だけデータ取得
                    printf("[%02x]", (unsigned char)track_chunk.data[j++]);
                }
                --j;   // 行き過ぎた分をデクリメント
            }
            break;
        case WSP_MIDI_EVENT_TYPE_TIME_CODE:
            //case 0x01:   // MIDIタイムコード
            WSP_SOUND_LOG("MIDI Time Code : ");
            ++j;
            printf("(frame/sec/min/hour)=%d", (unsigned char)track_chunk.data[j]);
            break;
        case WSP_MIDI_EVENT_TYPE_SONG_POSITION_POINTER:
            //case 0x02:   // ソング・ポジション・ポインタ
            WSP_SOUND_LOG("Song Position Pointer : ");
            ++j;
            printf("Position=%hd[MIDI beat]", mergeChar7bit(track_chunk.data[j+1], track_chunk.data[j]));
            printf(" (LSB:%d", (unsigned char)track_chunk.data[j++]);
            printf(", MSB:%d)", (unsigned char)track_chunk.data[j]);
            break;
        case WSP_MIDI_EVENT_TYPE_SONG_SELECT:
            //case 0x03:   // ソング・セレクト
            WSP_SOUND_LOG("Song Select : ");
            ++j;
            printf("SelectNo=%d", (unsigned char)track_chunk.data[j]);
            break;
        case WSP_MIDI_EVENT_TYPE_TUNE_REQUEST:
            //case 0x06:   // チューン・リクエスト
            WSP_SOUND_LOG("Tune Request");
            break;
        case WSP_MIDI_EVENT_TYPE_SYSTEM_EXCLUSIVE_MESSAGE_F7:
            {
                //printf("@End of Exclusive");
                WSP_SOUND_LOG("F7 Exclusive Message : ");
                ++j;

                // SysExのデータ長を取得
                cnt = 0;   // 初期化
                while((c = (unsigned char)track_chunk.data[j++]) & 0x80){   // フラグビットが1の間はループ
                    cnt = cnt | c;   // 合成
                    cnt <<= 7;   // 7bit左シフト
                }
                cnt = cnt | c;   // 合成
                printf(" Length=%u", (unsigned int)cnt);   // SysExのデータ長を取得

                for(k=1; k<=cnt; k++){   // 長さの分だけデータ取得
                    printf("[%02x]", (unsigned char)track_chunk.data[j++]);
                }
                --j;   // 行き過ぎた分をデクリメント

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
                case WSP_MIDI_META_EVENT_TYPE_SEQUENCE_NUMBER:   // シーケンス番号
                    printf("Sequence Number=");
                    j += 2;   // データ長の分を通り越す
                    data_len += 2;
                    cnt = (unsigned char)track_chunk.data[j];
                    midi_event.data.Append( track_chunk.data[j] );
                    ++j;
                    data_len++;
                    cnt <<= 8;   // 8bit左シフト
                    cnt = cnt | (unsigned char)track_chunk.data[j];
                    midi_event.data.Append( track_chunk.data[j] );
                    printf("%d", cnt);
                    break;
                case WSP_MIDI_META_EVENT_TYPE_TEXT:   // テキスト[可変長]
                    printf("Text=");
                    cnt = 0;
                    j += 1;
                    ++data_len;
                    while((c = (unsigned char)track_chunk.data[j++]) & 0x80){
                        cnt = cnt | (c & 0x7F);   // 合成
                        cnt <<= 7;   // 7bit左シフト
                        ++data_len;
                    }
                    ++data_len;
                    cnt = cnt | (c & 0x7F);   // 合成
                    for(k=1; k<=cnt; k++){
                        printf("%c", track_chunk.data[j]);
                        ++j;
                        ++data_len;
                    }
                    --j;   // 行き過ぎた分をデクリメント
                    --data_len;
                    break;
                case WSP_MIDI_META_EVENT_TYPE_COPYRIGHT:   // 著作権表示[可変長]
                    printf("Copyright=");
                    cnt = 0;
                    j += 1;
                    while((c = (unsigned char)track_chunk.data[j++]) & 0x80){
                        cnt = cnt | (c & 0x7F);   // 合成
                        cnt <<= 7;   // 7bit左シフト
                    }
                    cnt = cnt | (c & 0x7F);   // 合成
                    for(k=1; k<=cnt; k++){
                        printf("%c", track_chunk.data[j++]);
                    }
                    --j;   // 行き過ぎた分をデクリメント
                    break;
                case WSP_MIDI_META_EVENT_TYPE_SEQUENCE_NAME:   // シーケンス名（曲タイトル）・トラック名[可変長]
                    printf("Title=");
                    cnt = 0;
                    j += 1;
                    while((c = (unsigned char)track_chunk.data[j++]) & 0x80){
                        cnt = cnt | (c & 0x7F);   // 合成
                        cnt <<= 7;   // 7bit左シフト
                    }
                    cnt = cnt | (c & 0x7F);   // 合成
                    for(k=1; k<=cnt; k++){
                        printf("%c", track_chunk.data[j++]);
                    }
                    --j;   // 行き過ぎた分をデクリメント
                    break;
                case WSP_MIDI_META_EVENT_TYPE_INSTRUMENT:   // 楽器名[可変長]
                    printf("InstName=");
                    cnt = 0;
                    j += 1;
                    while((c = (unsigned char)track_chunk.data[j++]) & 0x80){
                        cnt = cnt | (c & 0x7F);   // 合成
                        cnt <<= 7;   // 7bit左シフト
                    }
                    cnt = cnt | (c & 0x7F);   // 合成
                    for(k=1; k<=cnt; k++){
                        printf("%c", track_chunk.data[j++]);
                    }
                    --j;   // 行き過ぎた分をデクリメント
                    break;
                case WSP_MIDI_META_EVENT_TYPE_LYRICS:   // 歌詞[可変長]
                    printf("Words=");
                    cnt = 0;
                    j += 1;
                    while((c = (unsigned char)track_chunk.data[j++]) & 0x80){
                        cnt = cnt | (c & 0x7F);   // 合成
                        cnt <<= 7;   // 7bit左シフト
                    }
                    cnt = cnt | (c & 0x7F);   // 合成
                    for(k=1; k<=cnt; k++){
                        printf("%c", track_chunk.data[j++]);
                    }
                    --j;   // 行き過ぎた分をデクリメント
                    break;
                case WSP_MIDI_META_EVENT_TYPE_MARKER:   // マーカー[可変長]
                    printf("Marker=");
                    cnt = 0;
                    j += 1;
                    while((c = (unsigned char)track_chunk.data[j++]) & 0x80){
                        cnt = cnt | (c & 0x7F);   // 合成
                        cnt <<= 7;   // 7bit左シフト
                    }
                    cnt = cnt | (c & 0x7F);   // 合成
                    for(k=1; k<=cnt; k++){
                        printf("%c", track_chunk.data[j++]);
                    }
                    --j;   // 行き過ぎた分をデクリメント
                    break;
                case WSP_MIDI_META_EVENT_TYPE_CUE_POINT:   // キューポイント[可変長]
                    printf("CuePoint=");
                    cnt = 0;
                    j += 1;
                    while((c = (unsigned char)track_chunk.data[j++]) & 0x80){
                        cnt = cnt | (c & 0x7F);   // 合成
                        cnt <<= 7;   // 7bit左シフト
                    }
                    cnt = cnt | (c & 0x7F);   // 合成
                    for(k=1; k<=cnt; k++){
                        printf("%c", track_chunk.data[j++]);
                    }
                    --j;   // 行き過ぎた分をデクリメント
                    break;
                case WSP_MIDI_META_EVENT_TYPE_PROGRAM_NAME:   // プログラム名（音色名）[可変長]
                    printf("ProgramName=");
                    cnt = 0;
                    j += 1;
                    while((c = (unsigned char)track_chunk.data[j++]) & 0x80){
                        cnt = cnt | (c & 0x7F);   // 合成
                        cnt <<= 7;   // 7bit左シフト
                    }
                    cnt = cnt | (c & 0x7F);   // 合成
                    for(k=1; k<=cnt; k++){
                        printf("%c", track_chunk.data[j++]);
                    }
                    --j;   // 行き過ぎた分をデクリメント
                    break;
                case WSP_MIDI_META_EVENT_TYPE_DEVICE_NAME:   // デバイス名（音源名）[可変長]
                    printf("DeviceName=");
                    cnt = 0;
                    j += 1;
                    while((c = (unsigned char)track_chunk.data[j++]) & 0x80){
                        cnt = cnt | (c & 0x7F);   // 合成
                        cnt <<= 7;   // 7bit左シフト
                    }
                    cnt = cnt | (c & 0x7F);   // 合成
                    for(k=1; k<=cnt; k++){
                        printf("%c", track_chunk.data[j++]);
                    }
                    --j;   // 行き過ぎた分をデクリメント
                    break;
                case WSP_MIDI_META_EVENT_TYPE_MIDI_CHANNEL_PREFIX:   // MIDIチャンネルプリフィックス[1byte]
                    printf("MidiChannelPrefix=");
                    j += 2;   // データ長の分を通り越す
                    cnt = (unsigned char)track_chunk.data[j];
                    printf("%d", cnt);
                    break;
                case WSP_MIDI_META_EVENT_TYPE_PORT:   // ポート指定[1byte]
                    printf("Port=");
                    j += 2;   // データ長の分を通り越す
                    cnt = (unsigned char)track_chunk.data[j];
                    printf("%d", cnt);
                    break;
                case WSP_MIDI_META_EVENT_TYPE_END_OF_TRACK:   // トラック終端[0byte]
                    printf("End of Track");
                    j += 1;   // データ長の分を通り越す
                    break;
                case WSP_MIDI_META_EVENT_TYPE_TEMPO:   // テンポ設定[3byte]
                    {
                        ++j;   // FF 51 03 でテンポを識別するので03の分インクリメント

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
                case WSP_MIDI_META_EVENT_TYPE_SMPTE_OFFSET:   // SMPTEオフセット[5byte]
                    printf("SMPTE_Offset=");
                    j += 2;   // データ長の分を通り越す
                    cnt = (unsigned char)track_chunk.data[j++];
                    switch(cnt & 0xC0){   // フレーム速度
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
                    printf(",Hour:%d", (cnt & 0x3F));   // 時間
                    printf(",Min:%d", (unsigned char)track_chunk.data[j++]);   // 分
                    printf(",Sec:%d", (unsigned char)track_chunk.data[j++]);   // 秒
                    printf(",Frame:%d", (unsigned char)track_chunk.data[j++]);   // フレーム
                    printf(",SubFrame:%d", (unsigned char)track_chunk.data[j]);   // サブフレーム
                    break;
                case WSP_MIDI_META_EVENT_TYPE_TIME:   // 拍子設定[4byte]
                    printf("Rhythm=");
                    j += 2;   // データ長の分を通り越す
                    printf("%d", (unsigned char)track_chunk.data[j++]);
                    cnt = 1;
                    for(k=0; k<=(int)track_chunk.data[j]; k++){   // 拍子の分母を算出する
                        cnt *= 2;   // 2の累乗
                    }
                    printf("/%d", cnt);
                    ++j;
                    printf(" ClockPerBeat=%d", (unsigned char)track_chunk.data[j++]);
                    printf(" 32NotePer4Note=%d", (unsigned char)track_chunk.data[j]);
                    break;
                case WSP_MIDI_META_EVENT_TYPE_KEY:   // 調設定[2byte]
                    printf("Key=");
                    j += 2;   // データ長の分を通り越す
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
                case WSP_MIDI_META_EVENT_TYPE_SEQUENCER_SPECIFIC:   // シーケンサ特定メタイベント
                    printf("SpecialIvent=");
                    cnt = 0;
                    j += 1;
                    while((c = (unsigned char)track_chunk.data[j++]) & 0x80){
                        cnt = cnt | (c & 0x7F);   // 合成
                        cnt <<= 7;   // 7bit左シフト
                    }
                    cnt = cnt | (c & 0x7F);   // 合成
                    for(k=1; k<=cnt; k++){
                        printf("[%02x]", (unsigned char)track_chunk.data[j++]);
                    }
                    --j;   // 行き過ぎた分をデクリメント
                    break;
                default:
                    printf("## Undefined meta event..[%02x]", meta_type);   // 見知らぬステータスなら;
                }
            }
            break;
        default:
            WSP_SOUND_LOG("## Undefined midi event type...[Status:%02x]", status);   // 見知らぬステータスなら
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

