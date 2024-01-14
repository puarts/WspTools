#pragma once

#include <wsp/common/_ctypes.h>
#include <wsp/common/_define_commoncore.h>
#include <wsp/common/fn-debug.h>
#include <wsp/common/_cl-noncopyable.hpp>

namespace wsp {
    namespace sound {
        class WSP_DLL_EXPORT Wave
        {
            WSP_NONCOPYABLE(Wave);
        public:
            Wave();
            ~Wave();

            int GetNumberOfChannels() const;
            int GetBitDepth() const;
            int GetDataLength() const;
            int GetSamplingRate() const;

            void* GetData(int channel);
            const void* GetData(int channel) const;

            template<typename _DataType>
            void GetDataAs(_DataType* o_data, int length, int channel);

            void Open(const char* file_path);
            void Save(const char* file_path);

        private:
            class Impl;
            Impl* impl_;
        };
    }
}

#include <wsp/common/_ctypes.h>

template<typename _DataType>
void wsp::sound::Wave::GetDataAs(_DataType* o_data, int length, int channel)
{
    void* data = GetData(channel);
    int bit_depth = GetBitDepth();
    int src_length = GetDataLength();
    switch (bit_depth)
    {
    case 8:
        {
            _DataType *out_ptr = o_data;
            u8* ptr = reinterpret_cast<u8*>(data);
            int copy_length = src_length < length ? src_length : length;
            _DataType *end_ptr = out_ptr + copy_length;
            for (; out_ptr != end_ptr; ++ptr, ++out_ptr)
            {
                *out_ptr = static_cast<_DataType>(*ptr);
            }
        }
        return;
    case 16:
        {
            _DataType *out_ptr = o_data;
            s16* ptr = reinterpret_cast<s16*>(data);
            int copy_length = src_length < length ? src_length : length;
            _DataType *end_ptr = out_ptr + copy_length;
            for (; out_ptr != end_ptr; ++ptr, ++out_ptr)
            {
                *out_ptr = static_cast<_DataType>(*ptr);
            }
        }
        return;
    case 32:
        {
            _DataType *out_ptr = o_data;
            s32* ptr = reinterpret_cast<s32*>(data);
            int copy_length = src_length < length ? src_length : length;
            _DataType *end_ptr = out_ptr + copy_length;
            for (; out_ptr != end_ptr; ++ptr, ++out_ptr)
            {
                *out_ptr = static_cast<_DataType>(*ptr);
            }
        }
        return;
    case 64:
        {
            _DataType *out_ptr = o_data;
            s64* ptr = reinterpret_cast<s64*>(data);
            int copy_length = src_length < length ? src_length : length;
            _DataType *end_ptr = out_ptr + copy_length;
            for (; out_ptr != end_ptr; ++ptr, ++out_ptr)
            {
                *out_ptr = static_cast<_DataType>(*ptr);
            }
        }
        return;
    default:
        WSP_FATAL_UNEXPECTED_DEFAULT;
    }
}
