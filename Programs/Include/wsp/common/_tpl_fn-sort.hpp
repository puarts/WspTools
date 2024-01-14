/**
 * @file _tpl_fn-sort.hpp
 * 
 */

#ifndef WSP_CORE_TEMPLATE_FUNCTION_SORT_HPP_
#define WSP_CORE_TEMPLATE_FUNCTION_SORT_HPP_

#ifndef __WSP_COMMONUTIL_DEFINE_COMMONUTIL_H__
#include "_define_commonutil.h"
#endif

namespace wsp{
    typedef enum{
        Ascending, 
        Descending
    }Order;

    template<typename T> inline int CompA(const void *_a, const void *_b);
    template<typename T> inline int CompB(const void *_a, const void *_b);
    template<typename T> inline void Swap(T *io_ary, u32 index1, u32 index2);
    template<typename T> inline void Swap(T **io_ary, u32 index1, u32 index2, u32 len2d);
    template<typename T> inline void ReverseOrder(T *io_arr, u32 length);
    template<typename T> inline void ReverseOrder2D(T **io_arr, u32 len1d, u32 len2d);

    // Sorting Algorithms ------------------------------------------
    //! The fastest Sorting Algorithm but when the current center value is far from actual median value
    //! None Recursive version, it is unsafe
    template<typename T> void QuickSort(T *io_arr, u32 length); 

    //! Recursive version, it is safe
    template<typename T> void QuickSortRecursive(T *io_ary, u32 length); 
    template<typename T> void QuickSort2d(T **io_arr, u32 sortIndex, u32 len1d, u32 len2d);
    template<typename T>
    void QuickSortRecursive(
        T *io_array, int length,
        int(*comp)(const void *lhs, const void *rhs));

    //! Better than insertion, selection, bubble sort
    template<typename T> void ShellSort(T *io_ary, u32 length);

    //! Insertion sort is very effective if ary is almost sorted, but shell sort is better
    template<typename T> inline void InsertionSort(T *io_ary, u32 length, u32 step=1);

    //! No Merit Sorting Algorithms(Just for learning)
    template<typename T> void SelectionSort(T *io_ary, u32 length);
    template<typename T> void BubbleSort(T *io_ary, u32 length);

}

#include "_tpl_fn-sort.inl"

#endif