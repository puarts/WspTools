/**
 * @file _tpl_fn-mem_manag.hpp
 * 
 * @brief Memory management module.
 */

#ifndef WSP_CORE_BASE_TEMPLATE_FUNC_MEM_MANAG_H__
#define WSP_CORE_BASE_TEMPLATE_FUNC_MEM_MANAG_H__

#include "_tpl_fn-core.hpp"

namespace wsp{
    /*--------------------------------------------------
       Memory management 
      --------------------------------------------------*/

    inline int GetMemSize(int length, int alloc_step);

    //! allocate memory by malloc or new, it depends on whether data type is fundamental or not
    template<typename T> 
    inline wsp::State ReallocArrayMem(
        T **io_data, 
        size_t length_new, 
        size_t length=0, 
        int reset_mem_val=0
    );

    //! Realloc memory by any steps of array length
    template<typename T> 
    inline wsp::State ReallocArrayMemByStep(
        T **io_data, 
        size_t length_new, 
        size_t length, 
        int reset_mem_val, 
        size_t alloc_step
    );

    template<typename T> 
    inline wsp::State AllocArrayMem(T **io_data, size_t length, int reset_mem_val=0);

    template<typename T> 
    inline wsp::State FreeArrayMem(T **io_data);
    // --------------------------------------------------------------------------
}

#include "_tpl_fn-mem_manag.inl"

#endif
