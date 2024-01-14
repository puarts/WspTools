#pragma once

#include <wsp/common/common_types.h>

namespace wsp{
    namespace image{
        template<typename _TElem = u8, int MemAllocStep = 50>
        class Image
        {
        public:
            Image();
            explicit Image(const wsp::image::Image<_TElem, MemAllocStep> &image);
            template<typename _TInputElem>
            explicit Image(
                const _TInputElem *in_data,
                u32 width,
                u32 height,
                u32 num_of_channels,
                u32 width_step);
            ~Image();

            // Accessors -------------------------------
            u32 width() const;
            u32 height() const;
            u32 num_of_channels() const;
            u32 width_step() const;
            _TElem* data();
            const _TElem* data() const;

            // Mutator ---------------------------------
            template<typename _TInputElem>
            void CopyFrom(
                const _TInputElem* data,
                u32 width,
                u32 height,
                u32 num_of_channels,
                u32 width_step);

            void ReallocateMemory(
                u32 width, 
                u32 height, 
                u32 num_of_channels, 
                u32 width_step);

            void ReallocateMemory(
                u32 width,
                u32 height,
                u32 num_of_channels);

        private:
            u32 width_;
            u32 height_;
            u32 num_of_channels_;
            u32 width_step_;
            u32 allocated_mem_size_;
            _TElem* data_;
        };
    }
}

#include "_tpl_cl-image.inl"
