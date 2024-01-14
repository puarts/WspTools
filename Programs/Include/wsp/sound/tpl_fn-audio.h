#pragma once

namespace wsp {
    namespace sound {
        template<typename _TOutElem, typename _TInElem>
        void ApplyAverageFilter(
            _TOutElem *o_samples, const _TInElem *in_samples, int num_samples, int block_size);

        // @brief 自己相関関数の最初のピークのインデックスを取得します。
        template<typename _TElem>
        int GetFirstAutoCorrelationPeakIndex(const _TElem *in_samples, int num_samples);

        template<typename _TElem>
        void AddSineWave(
            _TElem* io_waves,
            int length,
            double sampling_rate,
            double frequency,
            double amplitude,
            double time_offset);

        template<typename _TOutElem, typename _TInElem>
        void AutoCorrelation(
            _TOutElem* o_correlation,
            const _TInElem* in_waves,
            int length,
            int auto_correlation_step);
    }
}

#include "tpl_fn-audio-impl.h"
