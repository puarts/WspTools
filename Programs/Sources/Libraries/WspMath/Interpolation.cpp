
#include <wsp/math/Interpolation.h>
#include <vector>

void wsp::math::InterpolateKeys(
    float* o_y_values,
    int y_values_length,
    float begin_x,
    float x_step,
    const wsp::Vector2<float>* keys,
    int key_count,
    float(*interpolate_func)(float time))
{
    float* o_data_ptr = o_y_values;
    float* end_ptr = o_data_ptr + y_values_length;
    float current_x = begin_x;
    const wsp::Vector2<float>* first_key = &keys[0];
    const wsp::Vector2<float>* last_key = &keys[key_count - 1];
    const wsp::Vector2<float>* key_ptr = keys;
    const wsp::Vector2<float>* key_end = keys + (key_count - 1);
    float last_value = 0.0f;
    for (; o_data_ptr != end_ptr; ++o_data_ptr, current_x += x_step)
    {
        do
        {
            const wsp::Vector2<float>* key = key_ptr;
            const wsp::Vector2<float>* next_key = key_ptr + 1;
            if (current_x < first_key->x())
            {
                // 先頭のキーより前
                *o_data_ptr = first_key->y();
                break;
            }
            else if (key->x() <= current_x && current_x <= next_key->x())
            {
                // 現在のキーの範囲内
                float delta_time = next_key->x() - key->x();
                float next_key_rate = (current_x - key->x()) / delta_time;
                next_key_rate = interpolate_func(next_key_rate);
                *o_data_ptr = ((next_key_rate * next_key->y() + (1.0f - next_key_rate) * key->y()));
                last_value = *o_data_ptr;
                break;
            }
            else if (last_key->x() < current_x)
            {
                // 最後のキーより後ろ
                *o_data_ptr = last_key->y();
                break;
            }
            else
            {
                // 次のキー
                ++key_ptr;
            }
        } while (true);
    }
}

class wsp::math::SplineInterpolator::Impl
{
public:
    Impl()
        : key_count_(0)
        , keys_(nullptr)
    {
    }

    ~Impl()
    {
    }

    void SetKeys(const wsp::Vector2<float> *keys, int key_count)
    {
        WSP_REQUIRES_NOT_NULL(keys);
        WSP_REQUIRES(key_count > 1, "invalid args");

        keys_ = keys;
        key_count_ = key_count;
        coefs_q_.resize(key_count);
        coefs_s_.resize(key_count);
        coefs_r_.resize(key_count + 1);


        std::vector<float> delta_times, tmp_b, tmp_d, tmp_g, tmp_u;
        delta_times.resize(key_count);
        tmp_b.resize(key_count);
        tmp_d.resize(key_count);
        tmp_g.resize(key_count);
        tmp_u.resize(key_count);

        // ステップ１
        for (int key_index = 0; key_index < key_count_ - 1; key_index++)
        {
            delta_times[key_index] = keys[key_index + 1].x() - keys[key_index].x();
        }

        for (int key_index = 1; key_index < key_count_ - 1; key_index++)
        {
            float right_delta_time = keys[key_index + 1].x() - keys[key_index].x();
            float left_delta_time = keys[key_index].x() - keys[key_index - 1].x();
            tmp_b[key_index] = 2.0f * (right_delta_time + left_delta_time);

            float right_delta_value = keys[key_index + 1].y() - keys[key_index].y();
            float left_delta_value = keys[key_index].y() - keys[key_index - 1].y();
            tmp_d[key_index] = 3.0f * (right_delta_value / right_delta_time - left_delta_value / left_delta_time);
        }

        // ステップ２
        tmp_g[1] = delta_times[1] / tmp_b[1];
        for (int key_index = 2; key_index < key_count_ - 1; key_index++)
        {
            tmp_g[key_index] = delta_times[key_index] / (tmp_b[key_index] - delta_times[key_index - 1] * tmp_g[key_index - 1]);
        }

        tmp_u[1] = tmp_d[1] / tmp_b[1];
        for (int key_index = 2; key_index < key_count_; key_index++)
        {
            tmp_u[key_index] = (tmp_d[key_index] - delta_times[key_index - 1] * tmp_u[key_index - 1]) / (tmp_b[key_index] - delta_times[key_index - 1] * tmp_g[key_index - 1]);
        }

        // ステップ３
        coefs_r_[0] = 0.0;
        coefs_r_[key_count_] = 0.0;
        coefs_r_[key_count_ - 1] = tmp_u[key_count_ - 1];
        for (int key_index = key_count_ - 2; key_index >= 1; key_index--)
        {
            coefs_r_[key_index] = tmp_u[key_index] - tmp_g[key_index] * coefs_r_[key_index + 1];
        }

        // ステップ４
        for (int key_index = 0; key_index < key_count_ - 1; key_index++)
        {
            float delta_time = delta_times[key_index];
            float delta_value = keys[key_index + 1].y() - keys[key_index].y();
            coefs_q_[key_index] = delta_value / delta_time - delta_time * (coefs_r_[key_index + 1] + 2.0f * coefs_r_[key_index]) / 3.0f;
            coefs_s_[key_index] = (coefs_r_[key_index + 1] - coefs_r_[key_index]) / (3.0f * delta_time);
        }
    }

    float CalculateInterpolatedValue(float time) const
    {
        WSP_REQUIRES(keys_ != nullptr, "must set keys");
        int left_key_index = -1;

        // 区間の決定
        for (int key_index = 1; key_index < key_count_ && left_key_index < 0; key_index++)
        {
            if (time < keys_[key_index].x())
            {
                left_key_index = key_index - 1;
            }
        }
        if (left_key_index < 0)
        {
            left_key_index = key_count_ - 1;
        }

        // 計算
        float delta_time = time - keys_[left_key_index].x();
        float interpolated_value = keys_[left_key_index].y() + delta_time * (coefs_q_[left_key_index] + delta_time * (coefs_r_[left_key_index] + coefs_s_[left_key_index] * delta_time));

        return interpolated_value;
    }

private:
    int key_count_;
    const wsp::Vector2<float>* keys_;
    std::vector<float> coefs_r_;
    std::vector<float> coefs_q_;
    std::vector<float> coefs_s_;
};

wsp::math::SplineInterpolator::SplineInterpolator()
    : impl_(WSP_NEW Impl())
{
}

wsp::math::SplineInterpolator::~SplineInterpolator()
{
    delete impl_;
}

void wsp::math::SplineInterpolator::SetKeys(const wsp::Vector2<float> *keys, int key_count)
{
    impl_->SetKeys(keys, key_count);
}

float wsp::math::SplineInterpolator::CalculateInterpolatedValue(float time) const
{
    return impl_->CalculateInterpolatedValue(time);
}
