/**
 * @file fn-audio.c
 * @brief Definition of functionality of audio.
 * 
 */


#include <wsp/sound/fn-audio.h>
#include <wsp/sound/fn-sound_log.h>
#include <wsp/common/fn-debug.h>

#ifdef _WIN32
    #include <windows.h>   /* required before including mmsystem.h */
    #include <mmsystem.h>  /* multimedia functions (such as MIDI) for Windows */
#endif

#include <stdio.h>
#include <locale.h>

#ifdef _WIN32
namespace {
    bool g_is_played_all_buffer = false;

    // waveOutOpenが利用するコールバック関数
    void CALLBACK WaveOutProc(
        HWAVEOUT waveout_handle,
        UINT uMsg,
        DWORD dwInstance,
        DWORD dwParam1,
        DWORD dwParam2
        )
    {
        switch (uMsg)
        {
        case WOM_OPEN:
            WSP_SOUND_LOG("waveOutOpen\n");
            break;
        case WOM_CLOSE:
            WSP_SOUND_LOG("waveOutClose\n");
            break;
        case WOM_DONE:
            WSP_SOUND_LOG("played all buffers\n");
            g_is_played_all_buffer = true;
            break;
        }
    }
}
#endif

void WSP_OutputSoundToSpeaker(int device_id)
{
#ifdef _WIN32
    // 出力デバイスオープン
    const int NUM_BUFFERS = 2;
    LPBYTE wave_buffers[NUM_BUFFERS];
    WAVEHDR wave_handles[NUM_BUFFERS];
    WAVEHDR current_handle;
    LPBYTE current_buffer = NULL;
    MMRESULT result;
    HWAVEIN wavein_handle = NULL;
    char error_msg[128];

    WAVEFORMATEX wf = { 0 };                    // デバイスオープン時に使う、設定構造体
    // デバイスオープン時の設定
    {
        wf.wFormatTag = WAVE_FORMAT_PCM;    // PCM形式
        wf.nChannels = 1;                    // ステレオかモノラルか
        wf.nSamplesPerSec = 22050;                // サンプリングレート 22.05KHz
        wf.wBitsPerSample = 8;                    // 量子化レベル
        wf.nBlockAlign = wf.wBitsPerSample * wf.nChannels / 8;    // バイトあたりのビット数[PCMの仕様]
        wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;    // 1 秒あたりのバイト数
        // モノラル+サンプル22050+量子化8bitなので 一秒間に22050byteのデータが発生する
        // サンプル x nBlockAlign = 一秒間のデータ量なので nBlockAlignは１
    }

    // 入力デバイスオープン
    {
        u32 dev_id = 0;
        result = ::waveInOpen(
            &wavein_handle,                   // ハンドル
            dev_id,                 // デバイスID。 WAVE_MAPPERデフォルトのデバイスを指定
            &wf,                    // オープン時の設定
            (DWORD_PTR)WaveOutProc,  // コールバック関数
            NULL,
            CALLBACK_FUNCTION       // コールバック関数の属性
            );
        waveInGetErrorTextA(result, error_msg, 128);
        WSP_ASSERT(result == MMSYSERR_NOERROR, "%s\n", error_msg);
    }


    WSP_PrintOutputAudioDevices();

    // 出力デバイスオープン
    HWAVEOUT waveout_handle = NULL;    // ハンドル
    {
        result = waveOutOpen(
            &waveout_handle,                  // ハンドル
            0,           // デバイスID。 
            &wf,                   // オープン時の設定
            (DWORD_PTR)WaveOutProc,// コールバック関数
            NULL,
            CALLBACK_FUNCTION      // コールバック関数の属性
            );
        waveInGetErrorTextA(result, error_msg, 128);
        WSP_ASSERT(result == MMSYSERR_NOERROR, "%s\n", error_msg);
    }

    DWORD wave_buffer_size;
    wave_buffer_size = (DWORD)(wf.nAvgBytesPerSec * 0.05f);    // 一秒間のデータ量 x 秒数
    printf("wave_buffer_size = %lu\n", wave_buffer_size);

    for (int i = 0; i<NUM_BUFFERS; ++i)
    {
        wave_buffers[i] = (LPBYTE)new BYTE[wave_buffer_size];
        wave_handles[i].lpData = (LPSTR)wave_buffers[i];
        wave_handles[i].dwBufferLength = wave_buffer_size;
        wave_handles[i].dwFlags = 0;

        // バッファの初期化
        result = waveInPrepareHeader(wavein_handle, &wave_handles[i], sizeof(WAVEHDR));
        waveInGetErrorTextA(result, error_msg, 128);
        WSP_ASSERT(result == MMSYSERR_NOERROR, "%s\n", error_msg);
    }

    // 最初のバッファの関連付け
    result = waveInAddBuffer(wavein_handle, &wave_handles[0], sizeof(WAVEHDR));
    waveInGetErrorTextA(result, error_msg, 128);
    WSP_ASSERT(result == MMSYSERR_NOERROR, "%s\n", error_msg);


    // 録音開始
    result = waveInStart(wavein_handle);
    waveInGetErrorTextA(result, error_msg, 128);
    WSP_ASSERT(result == MMSYSERR_NOERROR, "%s", error_msg);

    printf("Successfully started device\n");

    current_buffer = wave_buffers[0];
    current_handle = wave_handles[0];

    WAVEHDR out_wave_handle = { 0 };
    out_wave_handle.lpData = (LPSTR)current_buffer;
    out_wave_handle.dwBufferLength = wave_buffer_size;                        // 音声データのサイズ
    out_wave_handle.dwFlags = 0;//WHDR_BEGINLOOP | WHDR_ENDLOOP;    // 再生オプション
    out_wave_handle.dwLoops = 0;//1;                                // ループ回数

    while(true)
    {
        // 入力音声の出力
        {
            // 初期化
            result = ::waveOutPrepareHeader(waveout_handle, &out_wave_handle, sizeof(WAVEHDR));
            waveInGetErrorTextA(result, error_msg, 128);
            WSP_ASSERT(result == MMSYSERR_NOERROR, "%s", error_msg);

            // 音声データブロックの書き込み
            waveOutWrite(waveout_handle, &out_wave_handle, out_wave_handle.dwBufferLength);

            while (g_is_played_all_buffer == false)
            {
            }

            g_is_played_all_buffer = false;
            printf("played\n");

            // 音声ファイルのパラメータを設定
            out_wave_handle.lpData = (LPSTR)current_buffer; // 音声データの入ってるポインタ
            printf("out_wave_handle.lpData\n");
        }
    }
#else
    WSP_FATAL_NOT_IMPLEMENTED;
#endif
}

void WSP_PrintInputAudioDevices(void)
{
#ifdef _WIN32
    u32 num_devices = waveInGetNumDevs();
    WAVEINCAPS wave_in_caps;
    u32 i;
    CHAR error_msg[256];

    /* display wide char string as Japanese */
    setlocale( LC_ALL, "Japanese" );


    WSP_SOUND_LOG("num_devices = %u", num_devices);
    WSP_SOUND_LOG("%u: %s", WAVE_MAPPER, "Microsoft Sound Mapper");
    for ( i = 0; i < num_devices; ++i )
    {
        MMRESULT result;
        result = waveInGetDevCaps( i, &wave_in_caps, sizeof( WAVEINCAPS ) );
        if( result != MMSYSERR_NOERROR )
        {
            waveOutGetErrorTextA(result, error_msg, 256);
            WSP_SOUND_ERROR_LOG("waveInGetDevCaps failed: %u, %s", result, error_msg);
            continue;
        }

        WSP_SOUND_LOG("%u: %s", i, wave_in_caps.szPname);
    }
#else
    WSP_FATAL_NOT_IMPLEMENTED;
#endif
}

void wsp::sound::GetInputAudioDevices(
    char* o_devices,
    int* o_num_devices,
    int unit_buffer_size,
    int num_buffer_size)
{
#ifdef _WIN32
    u32 num_devices = waveInGetNumDevs();
    WAVEINCAPS wave_in_caps;
    u32 i;
    CHAR error_msg[256];

    /* display wide char string as Japanese */
    setlocale(LC_ALL, "Japanese");

    *o_num_devices = 0;
    for (i = 0; i < num_devices; ++i)
    {
        MMRESULT result;
        result = waveInGetDevCaps(i, &wave_in_caps, sizeof(WAVEINCAPS));
        if (result != MMSYSERR_NOERROR)
        {
            waveOutGetErrorTextA(result, error_msg, 256);
            WSP_SOUND_ERROR_LOG("waveInGetDevCaps failed: %u, %s", result, error_msg);
            continue;
        }

        if (i < num_buffer_size)
        {
            int index = *o_num_devices;
            char* dest = o_devices + index * unit_buffer_size;
            int copy_size = min(MAXPNAMELEN, unit_buffer_size);
            memcpy(dest, wave_in_caps.szPname, copy_size);
            *o_num_devices = *o_num_devices + 1;
        }
    }
#else
    WSP_FATAL_NOT_IMPLEMENTED;
#endif
}


void WSP_PrintOutputAudioDevices(void)
{
#ifdef _WIN32
    u32 num_devices = waveInGetNumDevs();
    WAVEOUTCAPS wave_out_caps;
    u32 i;
    CHAR error_msg[256];

    /* display wide char string as Japanese */
    setlocale( LC_ALL, "Japanese" );

    WSP_SOUND_LOG("num_devices = %u", num_devices);

    for ( i = 0; i < num_devices; ++i )
    {
        MMRESULT result;
        result = waveOutGetDevCaps( i, &wave_out_caps, sizeof( WAVEOUTCAPS ) );
        if( result != MMSYSERR_NOERROR )
        {
            waveOutGetErrorTextA(result, error_msg, 256);
            WSP_SOUND_ERROR_LOG("waveOutGetDevCaps failed: %u, %s", result, error_msg);
            continue;
        }

        wprintf( L"%u: %hs", i, wave_out_caps.szPname );
    }
#else
    WSP_FATAL_NOT_IMPLEMENTED;
#endif
}

