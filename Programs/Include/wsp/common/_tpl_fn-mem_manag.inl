/**
 * @file _tpl_fn-mem_manag.inl
 * 
 * @brief Memory management module.
 */

#ifndef WSP_CORE_BASE_TEMPLATE_FUNC_MEM_MANAG_INL__
#define WSP_CORE_BASE_TEMPLATE_FUNC_MEM_MANAG_INL__

#include "_tpl_fn-mem_manag.hpp"

#include "fn-debug.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG_MEM_ALLOC_FUNC
#undef DEBUG_MEM_ALLOC_FUNC

namespace{
    template<typename T>
    void _AllocFoundamentalTypeArray(T **io_data, size_t length, u8 reset_mem_val)
    {
#ifdef DEBUG_MEM_ALLOC_FUNC
        WSP_COMMON_DEBUG_LOG("malloc %x", *io_data);
#endif
        size_t mem_size = length * sizeof(T);
        *io_data = reinterpret_cast<T*>(malloc(mem_size + 1));
        
        WSP_ASSERT(*io_data != NULL, "malloc failed.");
        
        memset(*io_data, reset_mem_val, mem_size);
    }

    template<typename T>
    void _AllocArray(T **io_data, size_t length, u8 reset_mem_val)
    {
#ifdef DEBUG_MEM_ALLOC_FUNC
        WSP_COMMON_DEBUG_LOG("new %x", *io_data);
#endif
        *io_data = WSP_NEW T[length];
        WSP_ASSERT(*io_data != NULL, "new failed.");
    }

    template<typename T>
    void _ReallocFoundamentalTypeArray(
        T **io_data, size_t current_length, size_t new_length, u8 reset_mem_val)
    {
#ifdef DEBUG_MEM_ALLOC_FUNC
        WSP_COMMON_DEBUG_LOG("realloc %x", *io_data);
#endif
        *io_data = reinterpret_cast<T*>(realloc(*io_data, new_length * sizeof(T)));
        WSP_ASSERT(*io_data != NULL, "realloc failed.");
        if (new_length > current_length)
        {
            size_t reset_length = new_length - current_length;
            memset(*io_data + current_length, reset_mem_val, reset_length * sizeof(T));
        }
    }

    template<typename T>
    void _ReallocArray(
        T **io_data, size_t current_length, size_t new_length, u8 reset_mem_val)
    {
        T *tmp_data = WSP_NEW T[current_length];
        WSP_ASSERT_NOT_NULL(tmp_data);

        // コピー用に一時保存
        {
            T *ptr = (*io_data);
            T *tmp_ptr = tmp_data;
            T *end_ptr = ptr + current_length;
            for (; ptr != end_ptr; ++ptr, ++tmp_ptr)
            {
                *tmp_ptr = *ptr;
            }
        }

#ifdef DEBUG_MEM_ALLOC_FUNC
        WSP_COMMON_DEBUG_LOG("delete %x", *io_data);
#endif
        delete[] * io_data;
#ifdef DEBUG_MEM_ALLOC_FUNC
        WSP_COMMON_DEBUG_LOG("new %x", *io_data);
#endif
        *io_data = WSP_NEW T[new_length];
        WSP_ASSERT(*io_data != NULL, "new failed.");

        size_t copy_len = new_length > current_length ? current_length : new_length;
        {
            T *ptr = (*io_data);
            T *tmp_ptr = tmp_data;
            T *end_ptr = ptr + copy_len;
            for (; ptr != end_ptr; ++ptr, ++tmp_ptr)
            {
                *ptr = *tmp_ptr;
            }
        }

        delete[] tmp_data;
    }
}

inline int wsp::GetMemSize(int length, int alloc_step)
{
    return static_cast<int>(ceil(static_cast<float>(length)/static_cast<float>(alloc_step)))*alloc_step;
}

//! allocate memory by malloc or new, it depends on whether data type is fundamental or not
template<typename T> 
inline wsp::State wsp::ReallocArrayMem(
    T **io_data, 
    size_t length_new, 
    size_t current_length, 
    int reset_mem_val
)
{
    WSP_REQUIRES_NOT_NULL(io_data);
    WSP_REQUIRES(length_new > 0, "Invalid argument.");

    if(*io_data==NULL)
    {
        wsp::AllocArrayMem(io_data, length_new, reset_mem_val);
        return WSP_STATE_SUCCESS;
    }
    else
    { 
        if(wsp::IsFundamentalDataType<T>())
        {
            _ReallocFoundamentalTypeArray(io_data, current_length, length_new, reset_mem_val);
        }
        else
        {
            _ReallocArray(io_data, current_length, length_new, reset_mem_val);
        }
        return WSP_STATE_SUCCESS;
    }
}


//! Realloc memory by any steps of array length
template<typename T>
inline wsp::State wsp::ReallocArrayMemByStep(
    T **io_data, 
    size_t length_new, 
    size_t length, 
    int reset_mem_val, 
    size_t alloc_step
)
{
    int mem_size = wsp::GetMemSize((int)length, (int)alloc_step);
    int mem_size_new = wsp::GetMemSize((int)length_new, (int)alloc_step);
    if(mem_size_new==mem_size){ return WSP_STATE_SUCCESS; }
    return wsp::ReallocArrayMem(io_data, length_new, length, reset_mem_val);
}

template<typename T> 
inline wsp::State wsp::AllocArrayMem(T **io_data, size_t length, int reset_mem_val)
{
    WSP_REQUIRES_NOT_NULL(io_data);
    WSP_REQUIRES_IS_NULL(*io_data);

    if (wsp::IsFundamentalDataType<T>())
    {
        _AllocFoundamentalTypeArray(io_data, length, reset_mem_val);
        return WSP_STATE_SUCCESS;
    }
    else
    {
        _AllocArray(io_data, length, reset_mem_val);
        return WSP_STATE_SUCCESS;
    }
}

template<typename T> 
inline wsp::State wsp::FreeArrayMem(T **io_data)
{
    WSP_REQUIRES_NOT_NULL(io_data);

    if(wsp::IsFundamentalDataType<T>())
    {
        #ifdef DEBUG_MEM_ALLOC_FUNC
        WSP_COMMON_DEBUG_LOG("free\n");
        #endif
        free(*io_data); 
        *io_data = NULL;
        return WSP_STATE_SUCCESS;
    }
    else
    {
        #ifdef DEBUG_MEM_ALLOC_FUNC
        WSP_COMMON_DEBUG_LOG("delete\n");
        #endif
        delete[] *io_data;
        *io_data = NULL;
        return WSP_STATE_SUCCESS;
    }
}


#endif
