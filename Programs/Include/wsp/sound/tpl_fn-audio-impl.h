#pragma once

#include "tpl_fn-audio.h"
#include <wsp/common/fn-debug.h>
#include <wsp/common/_tpl_fn-array.hpp>

#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

template<typename _TOutElem, typename _TInElem>
void wsp::sound::ApplyAverageFilter(
    _TOutElem *o_samples, const _TInElem *in_samples, int num_samples, int block_size)
{
    WSP_REQUIRES(block_size >= 3, "too small block size.");
    WSP_REQUIRES(o_samples != in_samples, "input and output array must not be the same array.");

    memset(o_samples, 0, num_samples * sizeof(_TOutElem));
    int half_block_size = (block_size - 1) / 2;

    // ブロックサイズより小さい先頭部
    {
        for (int i = 0; i < half_block_size; ++i)
        {
            double sum = 0.0;
            int end = i + half_block_size;
            int j = 0;
            for (j = 0; j < end; ++j)
            {
                sum += in_samples[j];
            }

            o_samples[i] = static_cast<_TOutElem>(sum / (double)j);
        }
    }

    // ブロックサイズに収まる範囲
    {
        double mult = 1.0 / static_cast<double>(block_size);
        const _TInElem *in_ptr = in_samples + half_block_size;
        const _TInElem *end = in_ptr + num_samples - half_block_size;
        _TOutElem *out_ptr = o_samples + half_block_size;
        for (; in_ptr != end; ++in_ptr, ++out_ptr)
        {
            const _TInElem *block_ptr = in_ptr - half_block_size;
            const _TInElem *block_end = in_ptr + half_block_size;
            double sum = 0.0;
            int average_size = 0;
            for (; block_ptr != block_end; ++block_ptr, ++average_size)
            {
                sum += static_cast<double>(*block_ptr);
            }

            *out_ptr = sum / average_size;
        }
    }

    // ブロックサイズより小さい配列の後ろ側
    {
        for (int i = num_samples - half_block_size; i < num_samples; ++i)
        {
            double sum = 0.0;
            int j = 0;
            for (j = i - half_block_size; j < num_samples; ++j)
            {
                sum += in_samples[j];
            }

            o_samples[i] = static_cast<_TOutElem>(sum / (double)j);
        }
    }
}


template<typename _TElem>
int wsp::sound::GetFirstAutoCorrelationPeakIndex(const _TElem *in_samples, int num_samples)
{
    _TElem average = wsp::GetAverage(in_samples, num_samples);
    _TElem peak_threshold = average * 1.3;
    int num_decreased_samples = 0;
    {
        // 最初の最小ピークを見つける
        _TElem prev_value = in_samples[0];
        const _TElem *in_ptr = in_samples + 1;
        const _TElem *end = in_samples + num_samples;
        for (; in_ptr != end; ++in_ptr)
        {
            bool is_increasing = *in_ptr > prev_value;
            if (is_increasing)
            {
                // 値が増加したらノイズでないか(本当に最小ピークか)チェック
                // 本当のピークなら減少分のサンプル数分足すと最大ピーク付近になるはず
                if (num_decreased_samples * 2 >= num_samples)
                {
                    return num_samples - 1;
                }

                _TElem peak_value = *(in_ptr + num_decreased_samples);
                bool is_larger = *in_ptr < peak_value;
                if (is_larger && (peak_threshold < peak_value))
                {
                    break;
                }
            }

            ++num_decreased_samples;
            prev_value = *in_ptr;
        }
    }

    // 最初の最大ピークを見つける
    // 減少したサンプル数 x 2付近が大体最大ピークになるはずなのでその付近の最大値を探す
    int estimated_peak_index = num_decreased_samples * 2;
    if (estimated_peak_index >= num_samples)
    {
        estimated_peak_index = num_samples - 1;
        return estimated_peak_index;
    }

    {
        const int peak_search_range = 5;
        if (estimated_peak_index + peak_search_range >= num_samples)
        {
            estimated_peak_index = num_samples - peak_search_range - 1;
        }

        const _TElem *in_ptr = in_samples + estimated_peak_index - peak_search_range;
        int max_index = wsp::GetMaxIndex(in_ptr, peak_search_range * 2 + 1);
        estimated_peak_index = estimated_peak_index - peak_search_range + max_index;
    }

    return estimated_peak_index;
}

namespace {
    template<typename _CalcType>
    void AddSineWaveInternal(
        _CalcType* io_waves,
        int length,
        _CalcType sampling_rate,
        _CalcType frequency,
        _CalcType amplitude,
        _CalcType time_offset)
    {
        _CalcType time_step = 1.0 / sampling_rate;
        _CalcType *ptr = io_waves;
        _CalcType *end_ptr = ptr + length;
        _CalcType mult = M_PI * 2.0 * frequency;
        _CalcType add = M_PI * 2.0 * time_offset;
        for (_CalcType time = 0.0; ptr != end_ptr; ++ptr, time += time_step)
        {
            *ptr += sin(time * mult + add) * amplitude;
        }
    }
}

template<typename _TElem>
void wsp::sound::AddSineWave(
    _TElem* io_waves,
    int length,
    double sampling_rate,
    double frequency,
    double amplitude,
    double time_offset)
{
    AddSineWaveInternal(
        io_waves, length,
        sampling_rate, frequency, amplitude, time_offset);
}

template<typename _TOutElem, typename _TInElem>
void wsp::sound::AutoCorrelation(
    _TOutElem* o_correlation,
    const _TInElem* in_waves,
    int length,
    int auto_correlation_step)
{
    double max = 0.0;
    int out_length = length / auto_correlation_step;
    {
        _TOutElem* out_corr_ptr = o_correlation;
        _TOutElem* corr_end = out_corr_ptr + out_length;
        for (int diff = 0; diff < length; diff += auto_correlation_step, ++out_corr_ptr)
        {
            const _TInElem *ptr = in_waves;
            const _TInElem *diff_ptr = in_waves + diff;
            const _TInElem *end_ptr = ptr + length;
            double sum = 0;
            for (; diff_ptr < end_ptr;
                ptr += auto_correlation_step,
                diff_ptr += auto_correlation_step)
            {
                double diff_value = *ptr - *diff_ptr;
                sum += diff_value * diff_value;
            }

            diff_ptr -= length;

            for (; ptr < end_ptr;
                ptr += auto_correlation_step,
                diff_ptr += auto_correlation_step)
            {
                double diff_value = *ptr - *diff_ptr;
                sum += diff_value * diff_value;
            }

            *out_corr_ptr = sum;
            if (sum > max)
            {
                max = sum;
            }
        }
    }

    // 非相関値になっているので相関値に直す
    // ついでに正規化
    {
        _TOutElem* ptr = o_correlation;
        _TOutElem* end = ptr + out_length;
        double mult = 1.0 / max;
        for (; ptr != end; ++ptr)
        {
            *ptr = static_cast<_TOutElem>(1.0 - (*ptr * mult));
        }
    }
}
