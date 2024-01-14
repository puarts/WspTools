
#include <wsp/sound/wave.h>

#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <cstring>

#include <wsp/common/fn-debug.h>

using namespace wsp::sound;

namespace {
    enum WaveFormat {
        WAVE_FORMAT_PCM = 1,
        WAVE_FORMAT_MS_ADPCM = 2,
        WAVE_FORMAT_IEEE_FLOAT = 3,
        WAVE_FORMAT_ALAW = 6,
        WAVE_FORMAT_MULAW = 7,
    };

    typedef struct {
        u8  chunk_id[4];
        u32 chunk_size;
        u16 format_id;
        u16 number_of_channels;
        u32 sampling_rate;
        u32 byte_per_sec;
        u16 block_size;
        u16 bit_depth;
    } FormatInfoChunk;

    typedef struct {
        u8  chunk_id[4];
        u32 data_size;
    } DataChunk;

    typedef struct {
        u8  chunk_id[4];
        u32 size_8;
        u8 wave_id[4];
        FormatInfoChunk format_info;
        DataChunk data;
    } WaveHeader;


    class WaveImpl
    {
    public:
        WaveImpl();
        virtual ~WaveImpl();

        void PrintHeader() const;    // �w�b�_�[�\��
        int GetNumberOfDataBlocks() const;       // �f�[�^�u���b�N��)
        double GetDuration() const;    // �f�[�^����(�b)

        int GetNumOfChannels() const
        {
            return header_.format_info.number_of_channels;
        }

        int GetSamplingRate() const
        {
            return header_.format_info.sampling_rate;
        }

        int GetBitDepth() const
        {
            return header_.format_info.bit_depth;
        }

        WaveFormat GetWaveFormat() const
        {
            return static_cast<WaveFormat>(header_.format_info.format_id);
        }

        bool Open(const char* file_path);
        bool Save(const char* file_path);

        void Clear();

    protected:
        WaveHeader header_;
    };


    WaveImpl::WaveImpl()
    {
        memset(&header_, 0, sizeof(WaveHeader));
    }

    WaveImpl::~WaveImpl()
    {
    }

    void WaveImpl::PrintHeader() const
    {
        std::cout << "[[HEADER]]" << std::endl;
        std::cout << "" << std::endl;

        std::cout << "[RIFF WAVE]" << std::endl;
        std::cout << "Chunk ID          : " << (char)header_.chunk_id[0]
            << (char)header_.chunk_id[1]
            << (char)header_.chunk_id[2]
            << (char)header_.chunk_id[3] << std::endl;
        std::cout << "Size - 8          : " << header_.size_8 << " Byte" << std::endl;
        std::cout << "WaveImpl ID           : " << (char)header_.wave_id[0]
            << (char)header_.wave_id[1]
            << (char)header_.wave_id[2]
            << (char)header_.wave_id[3] << std::endl;
        std::cout << "" << std::endl;

        std::cout << "[FMT CHUNK]" << std::endl;
        std::cout << "Chunk ID          : " << (char)header_.format_info.chunk_id[0]
            << (char)header_.format_info.chunk_id[1]
            << (char)header_.format_info.chunk_id[2]
            << (char)header_.format_info.chunk_id[3] << std::endl;
        std::cout << "Chunk Size        : " << header_.format_info.chunk_size << " Byte" << std::endl;
        std::cout << "Format ID         : " << header_.format_info.format_id << std::endl;
        std::cout << "Channels          : " << header_.format_info.number_of_channels << std::endl;
        std::cout << "Sampling rate     : " << header_.format_info.sampling_rate << " Hz" << std::endl;
        std::cout << "Byte per second   : " << header_.format_info.byte_per_sec << " B/s" << std::endl;
        std::cout << "Block size        : " << header_.format_info.block_size << " Byte" << std::endl;
        std::cout << "Bit depth         : " << header_.format_info.bit_depth << " bit" << std::endl;
        std::cout << "" << std::endl;

        std::cout << "[DATA CHUNK]" << std::endl;
        std::cout << "Chunk ID          : " << (char)header_.data.chunk_id[0]
            << (char)header_.data.chunk_id[1]
            << (char)header_.data.chunk_id[2]
            << (char)header_.data.chunk_id[3] << std::endl;
        std::cout << "Data size         : " << header_.data.data_size << " Byte" << std::endl;
        std::cout << "" << std::endl;

        return;
    }


    int WaveImpl::GetNumberOfDataBlocks() const
    {
        return header_.data.data_size / header_.format_info.block_size;
    }

    double WaveImpl::GetDuration() const
    {
        return (double)header_.data.data_size / header_.format_info.byte_per_sec;
    }

    void WaveImpl::Clear()
    {

    }

    class WaveInImpl
        : public WaveImpl
    {
    public:
        WaveInImpl();
        ~WaveInImpl();

        bool has_file;

        void OpenFile(const char* file_path);
        void CloseFile();

        // ���m�����f�[�^�͂��̂܂܁C�X�e���I�f�[�^�͑S���ǂ�ŉE�ƍ��𑫂���2�Ŋ���B

        // �X�e���I�f�[�^�͂��̂܂܁C���m�����f�[�^�͗����ɓ����l
        // �Е������ł����Ȃ炢��Ȃ��ق���nullptr�ɂ���

        template<typename SampleType>
        void ReadData(SampleType** data_list, int num_channels);

        // �t�@�C���̔C�Ӊӏ��̃f�[�^��ǂݎ��B
        // �t�@�C���𒴂����ʂ��w�肵���Ƃ��Ɋm���ɃG���[
        // ���Ԃł͂Ȃ��f�[�^�̃u���b�N�ʒu�̂��ߎg�����肪����

        template<typename SampleType>
        void ReadDataBuffer(SampleType** data, int num_channels, int head, int data_length);

    private:
        FILE *in;
        u32 data_beg;   // ���f�[�^�J�n�ʒu(�擪����̃o�C�g��)
    };

    WaveInImpl::WaveInImpl()
        : in(NULL)
        , data_beg(0)
    {
        has_file = false;
    }

    WaveInImpl::~WaveInImpl()
    {
    }

    void WaveInImpl::OpenFile(const char* file_path)
    {
        // ���łɓǂ񂾃t�@�C�����N���[�Y���Ă��Ȃ��B
        if (has_file)
        {
            throw std::domain_error("already opened");
        }

        // ���s������false
        in = fopen(file_path, "rb");
        if (in == NULL)
        {
            has_file = false;
            throw std::domain_error("fopen failed");
        }

        // �w�b�_�ǂݎ��B
        // ��{�`���Ȃ�riff�`�����N(12)+format_info�`�����N(24)+data�`�����N���f�[�^�ȊO(8)���f�[�^�̊J�n�n�_
        fread(&header_, sizeof(WaveHeader), 1, in);
        data_beg = ftell(in);//12 + 24 + 8 = �f�[�^���݈ʒu;

        // �`���`�F�b�N
        if ((memcmp(&header_.chunk_id, "RIFF", 4) != 0)
            || (memcmp(&header_.wave_id, "WAVE", 4) != 0))
        {
            CloseFile();
            throw std::domain_error("This file is not \"RIFF waveform Audio Format\"");
        }

        // data�`�����N�T��
        // riff,format_info�Ƃ��Ď���data�`�����N�Ŗ����ꍇformat_info�̒���܂Ŗ߂���data�`�����N��T��
        // ������data_beg���X�V����
        if (memcmp(&header_.data.chunk_id, "data", 4) != 0)
        {
            u8 chunk_id_temp[4];
            u32 chunk_size_temp;

            // �擪���� riff�`�����N��(12) + format_info�`�����N��(8+chunk_size)
            fseek(in, 12 + 8 + header_.format_info.chunk_size, SEEK_SET);

            // ���������`�����N���T�C�Y���擾���Ă���ǂݔ�΂��Ď��̃`�����N��ID���擾
            // while���[�v�𔲂������_��data�`�����N�ɂ��ǂ���Ă���B
            // ���̌��̃f�[�^�T�C�Y���擾����data�`�����N�ւƑ��
            fread(chunk_id_temp, 1, 4, in);
            
            while (memcmp(chunk_id_temp, "data", 4) != 0)
            {
                fread(&chunk_size_temp, 4, 1, in);
                fseek(in, chunk_size_temp, SEEK_CUR);
                fread(chunk_id_temp, 1, 4, in);
            }

            fread(&chunk_size_temp, 4, 1, in);

            data_beg = ftell(in); // �f�[�^�ʒu�m��

            //data�`�����N�X�V
            for (int i = 0; i<4; i++){
                header_.data.chunk_id[i] = chunk_id_temp[i];
            }
            header_.data.data_size = chunk_size_temp;
        }

        has_file = true;
    }


    void WaveInImpl::CloseFile()
    {
        fclose(in);
        in = NULL;
        has_file = false;
    }

    template<typename SampleType>
    void WaveInImpl::ReadData(SampleType** data, int num_channels)
    {
        ReadDataBuffer(data, num_channels, 0, GetNumberOfDataBlocks());
    }
    
    template<typename SampleType>
    void WaveInImpl::ReadDataBuffer(
        SampleType** o_data, int num_channels, int head, int data_length)
    {
        fseek(in, data_beg + head, SEEK_SET);

        // �X�e���I�f�[�^�͂��̂܂܁C���m�����f�[�^�͗����ɓ����l
        int src_num_channels = header_.format_info.number_of_channels;

        SampleType *data_temp = WSP_NEW SampleType[data_length * src_num_channels];
        fread(data_temp, sizeof(SampleType), data_length * src_num_channels, in);

        int channel = 0;
        int smaller_num_channels = num_channels > src_num_channels ? src_num_channels : num_channels;
        for (; channel < smaller_num_channels; ++channel)
        {
            SampleType* channel_data = o_data[channel];
            for (int i = 0; i < data_length; ++i)
            {
                channel_data[i] = data_temp[i * src_num_channels + channel];
            }
        }

        int last_channel = channel;
        for (; channel < num_channels; ++channel)
        {
            SampleType* channel_data = o_data[channel];
            for (int i = 0; i < data_length; ++i)
            {
                channel_data[i] = data_temp[i * src_num_channels + last_channel];
            }
        }

        delete[] data_temp;
    }

    // ------------------------------------------------
    class WaveOutImpl : public WaveImpl
    {
    public:
        WaveOutImpl();
        ~WaveOutImpl();

        bool has_file;
        bool has_header_;

        bool OpenFile(const char* file_path);
        void SetHeader(u32 wav_length,
            u16 ch,
            u32 sampling_rate,
            u16 bit_depth);

        // SetHeader�Ń��m�����f�[�^���w�肵���ꍇ�ɑ����������Ă����������B
        // ���������w�肵�Ȃ��Ă�null������B
        // �X�e���I�w��̏ꍇ�Ɉ���������Ȃ��Ƃ���������B
        bool WriteFile(u8 *L_data, u8 *R_data = nullptr);
        bool WriteFile(int16 *L_data, int16 *R_data = nullptr);

        void CloseFile();

    private:
        FILE *out;
    };


    WaveOutImpl::WaveOutImpl()
    {
        has_file = false;
        has_header_ = false;
        out = NULL;
    }

    WaveOutImpl::~WaveOutImpl()
    {
        if (out != NULL)
        {
            CloseFile();
        }
    }

    bool WaveOutImpl::OpenFile(const char* file_path)
    {
        out = fopen(file_path, "wb");
        if (out == NULL){
            std::cerr << "FILE OPEN ERROR : " << file_path << std::endl;
            has_file = false;
            return false;
        }

        has_file = true;
        return true;
    }

    void WaveOutImpl::SetHeader(
        u32 wav_length,
        u16 num_channels,
        u32 sampling_rate,
        u16 bit_depth)
    {
        u16 byte_depth = bit_depth / 8;

        // �`�����N�\��
        // �d������v�Z�����邯�ǌ��ʂ��D��
        // data�`�����N
        memcpy(header_.data.chunk_id, "data", 4);
        header_.data.data_size = byte_depth * num_channels * wav_length;

        // format_info�`�����N
        memcpy(header_.format_info.chunk_id, "fmt ", 4);
        header_.format_info.chunk_size = 16; // 16��format_info.format_id�ȍ~�̃T�C�Y�Bformat_info�`�����N���̂��̂�8+16
        header_.format_info.format_id = 1;
        header_.format_info.number_of_channels = num_channels;
        header_.format_info.sampling_rate = sampling_rate;
        header_.format_info.byte_per_sec = byte_depth * num_channels * sampling_rate;
        header_.format_info.block_size = byte_depth * num_channels;
        header_.format_info.bit_depth = bit_depth;

        // riff�`�����N
        memcpy(header_.chunk_id, "RIFF", 4);
        header_.size_8 = header_.data.data_size + 44 - 8;
        memcpy(header_.wave_id, "WAVE", 4);


        // �w�b�_�\�z����
        has_header_ = true;
    }


    bool WaveOutImpl::WriteFile(u8 *L_data, u8 *R_data)
    {
        if (!has_file){
            std::cerr << "NO OUTPUT FILE" << std::endl;
            return false;
        }

        if (!has_header_){
            std::cerr << "NO DATA" << std::endl;
            return false;
        }


        fseek(out, 0, SEEK_SET);

        // �w�b�_
        fwrite(&header_, sizeof(WaveHeader), 1, out);

        //�f�[�^
        u32 length_temp = GetNumberOfDataBlocks();

        if (header_.format_info.number_of_channels == 1){
            fwrite(L_data, 1, length_temp, out);

        }
        else{
            u8 *data_temp = WSP_NEW u8[length_temp * 2];
            for (u32 i = 0; i<length_temp; i++){
                data_temp[2 * i] = L_data[i];
                data_temp[2 * i + 1] = R_data[i];
            }
            fwrite(data_temp, 1, length_temp * 2, out);
            delete[] data_temp;
        }

        return true;
    }

    bool WaveOutImpl::WriteFile(int16 *L_data, int16 *R_data)
    {
        if (!has_file){
            std::cerr << "NO OUTPUT FILE" << std::endl;
            return false;
        }

        if (!has_header_){
            std::cerr << "NO DATA" << std::endl;
            return false;
        }


        fseek(out, 0, SEEK_SET);

        // �w�b�_
        fwrite(&header_, sizeof(WaveHeader), 1, out);

        //�f�[�^
        u32 length_temp = GetNumberOfDataBlocks();

        int unit_data_byte = sizeof(int16);

        if (header_.format_info.number_of_channels == 1)
        {
            fwrite(L_data, unit_data_byte, length_temp, out);
        }
        else
        {
            int16 *data_temp = WSP_NEW int16[length_temp * 2];
            for (u32 i = 0; i<length_temp; i++){
                data_temp[2 * i] = L_data[i];
                data_temp[2 * i + 1] = R_data[i];
            }
            fwrite(data_temp, unit_data_byte, length_temp * 2, out);
            delete[] data_temp;
        }

        return true;
    }

    void WaveOutImpl::CloseFile()
    {
        fclose(out);
        has_file = false;
    }
}


#include <exception> 

class Wave::Impl
{
public:
    Impl()
    {
        for (int i = 0; i < MaxNumChannels; ++i)
        {
            wave_data_[i] = NULL;
        }
    }
    ~Impl()
    {
        ClearWaveData();
    }

    int GetNumberOfChannels() const
    {
        return wave_in_.GetNumOfChannels();
    }

    int GetBitDepth() const
    {
        return wave_in_.GetBitDepth();
    }

    int GetDataLength() const
    {
        return wave_in_.GetNumberOfDataBlocks();
    }

    int GetSamplingRate() const
    {
        return wave_in_.GetSamplingRate();
    }

    void Open(const char* file_path)
    {
        try
        {
            wave_in_.OpenFile(file_path);
        }
        catch (std::exception)
        {
            throw;
        }

        ClearWaveData();

        int num_channels = wave_in_.GetNumOfChannels();
        WSP_ASSERT(num_channels < MaxNumChannels, "unsupported number of channels");
        int length = wave_in_.GetNumberOfDataBlocks();
        int bit_depth = wave_in_.GetBitDepth();

        switch (bit_depth)
        {
        case 8:
            {
                for (int i = 0; i < num_channels; ++i)
                {
                    wave_data_[i] = WSP_NEW u8[length];
                    memset(wave_data_[i], 0, sizeof(u8) * length);
                }

                u8* data[2];
                data[0] = (u8*)wave_data_[0];
                data[1] = (u8*)wave_data_[1];
                wave_in_.ReadData(data, num_channels);
            }
            break;
        case 16:
            {
                for (int i = 0; i < num_channels; ++i)
                {
                    wave_data_[i] = WSP_NEW int16[length];
                    memset(wave_data_[i], 0, sizeof(int16) * length);
                }

                int16* data[2];
                data[0] = (int16*)wave_data_[0];
                data[1] = (int16*)wave_data_[1];
                wave_in_.ReadData(data, num_channels);
            }
            break;
        case 32:
            {
                switch (wave_in_.GetWaveFormat())
                {
                case WAVE_FORMAT_IEEE_FLOAT:
                    {
                        for (int i = 0; i < num_channels; ++i)
                        {
                            wave_data_[i] = WSP_NEW f32[length];
                            memset(wave_data_[i], 0, sizeof(f32) * length);
                        }

                        f32* data[2];
                        data[0] = (f32*)wave_data_[0];
                        data[1] = (f32*)wave_data_[1];
                        wave_in_.ReadData(data, num_channels);
                    }
                    break;
                default:
                    WSP_FATAL_NOT_IMPLEMENTED;
                }
            }
            break;
        case 48:
            WSP_FATAL_NOT_IMPLEMENTED;
            break;
        case 64:
            WSP_FATAL_NOT_IMPLEMENTED;
            break;
        default:
            WSP_FATAL_UNEXPECTED_DEFAULT;
        }

        wave_in_.CloseFile();
    }

    void Save(const char* file_path)
    {
        WSP_ASSERT_NOT_NULL(wave_data_[0]);

        // �o�̓t�@�C��
        // �t�@�C�����[�h
        bool success = wave_out_.OpenFile(file_path);
        if (!success)
        {
            throw std::domain_error("failed to open file");
        }

        // �w�b�_�\�z
        wave_out_.SetHeader(
            wave_in_.GetNumberOfDataBlocks(),
            wave_in_.GetNumOfChannels(),
            wave_in_.GetSamplingRate(),
            wave_in_.GetBitDepth());

        // �����o��
        int num_channels = wave_in_.GetNumOfChannels();
        WSP_ASSERT(num_channels < MaxNumChannels, "unsupported number of channels");
        int length = wave_in_.GetNumberOfDataBlocks();

        int bit_depth = wave_in_.GetBitDepth();
        switch (bit_depth)
        {
        case 8:
            {
                success = wave_out_.WriteFile(
                    (u8*)wave_data_[0], (u8*)wave_data_[1]);
            }
            break;
        case 16:
            {
                success = wave_out_.WriteFile(
                    (int16*)wave_data_[0], (int16*)wave_data_[1]);
            }
            break;
        case 32:
            WSP_FATAL_NOT_IMPLEMENTED;
            break;
        case 48:
            WSP_FATAL_NOT_IMPLEMENTED;
            break;
        case 64:
            WSP_FATAL_NOT_IMPLEMENTED;
            break;
        default:
            WSP_FATAL_UNEXPECTED_DEFAULT;
        }

        wave_out_.CloseFile();
        if (!success)
        {
            throw std::domain_error("failed to write file");
        }
    }

    void* GetData(int channel)
    {
        return wave_data_[channel];
    }

    const void* GetData(int channel) const
    {
        return wave_data_[channel];
    }

private:
    void ClearWaveData()
    {
        for (int i = 0; i < MaxNumChannels; ++i)
        {
            void* data = wave_data_[i];
            if (data != NULL)
            {
                delete[] data;
                wave_data_[i] = NULL;
            }
        }
    }

private:
    WaveInImpl wave_in_;
    WaveOutImpl wave_out_;

    // �Ƃ肠�����ő�6�`�����l���ɂ��Ƃ�
    static const int MaxNumChannels = 6;
    void *wave_data_[MaxNumChannels];
};

Wave::Wave()
    : impl_(WSP_NEW Wave::Impl())
{
}

Wave::~Wave()
{
    delete impl_;
}

void Wave::Open(const char* file_path)
{
    try
    {
        impl_->Open(file_path);
    }
    catch (std::exception)
    {
        throw;
    }
}

void Wave::Save(const char* file_path)
{
    impl_->Save(file_path);
}

void* Wave::GetData(int channel)
{
    return impl_->GetData(channel);
}

const void* Wave::GetData(int channel) const
{
    return impl_->GetData(channel);
}

int Wave::GetNumberOfChannels() const
{
    return impl_->GetNumberOfChannels();
}

int Wave::GetBitDepth() const
{
    return impl_->GetBitDepth();
}

int Wave::GetDataLength() const
{
    return impl_->GetDataLength();
}

int Wave::GetSamplingRate() const
{
    return impl_->GetSamplingRate();
}
