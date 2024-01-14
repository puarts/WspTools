#pragma once

#include <wsp/common/tpl_cl-vector2.h>

namespace wsp {
    namespace math {

        WSP_DLL_EXPORT void InterpolateKeys(
            float* o_y_values,
            int y_values_length,
            float begin_x,
            float x_step,
            const wsp::Vector2<float>* keys,
            int key_count,
            float(*interpolate_func)(float x_value));

        inline void InterpolateLinear(
            float* o_y_values,
            int y_values_length,
            float begin_x,
            float x_step,
            const wsp::Vector2<float>* keys,
            int key_count)
        {
            InterpolateKeys(
                o_y_values,
                y_values_length,
                begin_x,
                x_step,
                keys,
                key_count,
                [](float x_value) { return x_value; });
        }


        inline void InterpolateCubic(
            float* o_y_values,
            int y_values_length,
            float begin_x,
            float x_step,
            const wsp::Vector2<float>* keys,
            int key_count)
        {
            InterpolateKeys(
                o_y_values,
                y_values_length,
                begin_x,
                x_step,
                keys,
                key_count,
                [](float x_value) { return x_value * x_value * (3 - 2 * x_value); });
        }

        class WSP_DLL_EXPORT SplineInterpolator
        {
        public:
            SplineInterpolator();
            ~SplineInterpolator();

            void SetKeys(const wsp::Vector2<float> *keys, int key_count);
            float CalculateInterpolatedValue(float x_value) const;

        private:
            class Impl;
            Impl* impl_;
        };
    }
}
