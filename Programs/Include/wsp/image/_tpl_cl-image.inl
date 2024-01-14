
#include "_tpl_cl-image.h"
#include "_tpl_fn-img_mem_manag.hpp"
#include <wsp/common/fn-debug.h>

namespace{
    template<typename _TElem>
    u32 _CalculateWidthStep(u32 width, u32 num_of_channels, u32 alignment = 4)
    {
        u32 required_size = width * num_of_channels * sizeof(_TElem);
        int remainder = required_size % alignment;
        if (remainder == 0)
        {
            return required_size;
        }
        else
        {
            return required_size + alignment;
        }
    }
}

template<typename _TElem, int MemAllocStep>
wsp::image::Image<_TElem, MemAllocStep>::Image()
: data_(NULL)
, width_(0)
, height_(0)
, num_of_channels_(0)
, width_step_(0)
, allocated_mem_size_(0)
{
}

template<typename _TElem, int MemAllocStep>
wsp::image::Image<_TElem, MemAllocStep>::Image(
    const wsp::image::Image<_TElem, MemAllocStep> &image)
{
    ReallocateMemory(
        image.width(), image.height(), image.num_of_channels(), image.width_step());
}

template<typename _TElem, int MemAllocStep>
template<typename _TInputElem>
wsp::image::Image<_TElem, MemAllocStep>::Image(
    const _TInputElem *in_data,
    u32 width,
    u32 height,
    u32 num_of_channels,
    u32 width_step)
{
    ReallocateMemory(width, height, num_of_channels, width_step);
}

template<typename _TElem, int MemAllocStep>
wsp::image::Image<_TElem, MemAllocStep>::~Image()
{
    if (data_ != NULL)
    {
        wsp::img::FreeImageData<_TElem>(&data_, NULL, NULL);
    }
}

template<typename _TElem, int MemAllocStep>
u32 wsp::image::Image<_TElem, MemAllocStep>::width() const
{
    return width_;
}

template<typename _TElem, int MemAllocStep>
u32 wsp::image::Image<_TElem, MemAllocStep>::height() const
{
    return height_;
}

template<typename _TElem, int MemAllocStep>
u32 wsp::image::Image<_TElem, MemAllocStep>::num_of_channels() const
{
    return num_of_channels_;
}

template<typename _TElem, int MemAllocStep>
u32 wsp::image::Image<_TElem, MemAllocStep>::width_step() const
{
    return width_step_;
}

template<typename T, int MemAllocStep>
void wsp::image::Image<T, MemAllocStep>::ReallocateMemory(
    u32 width,
    u32 height,
    u32 num_of_channels)
{
    int width_step = _CalculateWidthStep<T>(width, num_of_channels);
    ReallocateMemory(
        width, 
        height,
        num_of_channels,
        width_step);
}

template<typename T, int MemAllocStep>
void wsp::image::Image<T, MemAllocStep>::ReallocateMemory(
    u32 width,
    u32 height,
    u32 num_of_channels,
    u32 width_step)
{
    WSP_REQUIRES(width != 0, "Invalid argument.");
    WSP_REQUIRES(height != 0, "Invalid argument.");
    WSP_REQUIRES(num_of_channels != 0, "Invalid argument.");
    WSP_REQUIRES(width_step >= width * num_of_channels, "Invalid argument.");

    bool is_the_same_size = width_ == width &&
                            height_ == height &&
                            num_of_channels_ == num_of_channels &&
                            width_step_ == width_step;
    if (is_the_same_size)
    {
        return;
    }

    u32 length_old, nc_old;
    //wsp::Image<T, MemAllocStep> tmpImg(*this);

    length_old = width_ * height_;
    nc_old = num_of_channels_;
    u32 length = width * height;
    u32 required_memory_size = height * width_step;

    //int mem_size_old = static_cast<int>(ceil(static_cast<float>(num_of_elem_old)/static_cast<float>(MemAllocStep))*MemAllocStep);
    u32 mem_size_new = static_cast<u32>(
        ceil(static_cast<float>(required_memory_size) / static_cast<float>(MemAllocStep))
        ) * MemAllocStep;
    if (mem_size_new == allocated_mem_size_)
    {
        return;
    }

    wsp::State state;
    bool set_new_channels_to_zero = false;
    state = wsp::img::ReallocImageData(
        &data_,
        NULL,
        NULL,
        mem_size_new,
        length,
        num_of_channels,
        length_old,
        nc_old,
        set_new_channels_to_zero);

    if (state != WSP_STATE_SUCCESS)
    {
        WSP_COMMON_ERROR_LOG("Memory allocation has failed\n");
        throw state;
    }

    width_ = width;
    height_ = height;
    num_of_channels_ = num_of_channels;
    width_step_ = width_step;
    allocated_mem_size_ = mem_size_new;
}

template<typename _TElem, int MemAllocStep>
template<typename _TInputElem>
void wsp::image::Image<_TElem, MemAllocStep>::CopyFrom(
    const _TInputElem* data,
    u32 width,
    u32 height,
    u32 num_of_channels,
    u32 width_step)
{
    WSP_REQUIRES_NOT_NULL(data);
    WSP_REQUIRES(width * num_of_channels < width_step, "Invalid argument.");

    ReallocateMemory(width, height, num_of_channels);

#define USE_OPTIMIZED_WspImageImageCopyFrom
#ifdef USE_OPTIMIZED_WspImageImageCopyFrom
    {
        _TElem* dest_ptr = data_;
        const _TInputElem src_ptr = data;
        const _TInputElem end_ptr = src_ptr + height * width_step;
        const _TInputElem end_x_ptr, end_c_ptr;
        u32 src_diff_width_step = width_step - width * num_of_channels;
        u32 dest_diff_width_step = width_step_ - width_ * num_of_channels_;
        for (; src_ptr < end_ptr;)
        {
            end_x_ptr = src_ptr + width_step;
            for (; end_x_ptr < src_ptr;)
            {
                end_c_ptr = src_ptr + num_of_channels;
                for (; src_ptr < end_c_ptr; ++src_ptr, ++dest_ptr)
                {
                    *src_ptr = static_cast<_TElem>(*dest_ptr);
                }
            }

            dest_ptr += dest_diff_width_step;
            src_ptr += src_diff_width_step;
        }
    }
#else
    for (u32 y = 0; y < height; ++y)
    {
        for (u32 x = 0; x < width; ++x)
        {
            u32 i = y * width_step_ + x * num_of_channels_;
            for (u32 c = 0; c < num_of_channels; ++c)
            {
                data_[i + c] = static_cast<_TElem>(data[i + c]);
            }
        }
    }
#endif
}
