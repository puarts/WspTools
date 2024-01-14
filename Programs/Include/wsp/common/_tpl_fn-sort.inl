/**
 * @file _tpl_fn-sort.inl
 * 
 */

#ifndef WSP_CORE_TEMPLATE_FUNCTION_SORT_INL_
#define WSP_CORE_TEMPLATE_FUNCTION_SORT_INL_

#include "_tpl_fn-sort.hpp"

namespace{
    template <typename T> void QuickSortRecursive_(T *io_ary, int left, int right)
    {
        //! argument check
        if (left >= right)
        {
            return;
        }

        int center, iter_l, iter_r;
        T median_value = io_ary[left];
        iter_l = left;
        iter_r = right;
        while (iter_l<iter_r)
        {
            while (iter_l<iter_r && median_value<io_ary[iter_r])
            {
                iter_r--;
            }

            if (iter_l<iter_r){
                io_ary[iter_l] = io_ary[iter_r];
                iter_l++;
            }

            while (iter_l<iter_r && io_ary[iter_l]<median_value)
            {
                iter_l++;
            }

            if (iter_l<iter_r){
                io_ary[iter_r] = io_ary[iter_l];
                iter_r--;
            }
        }
        center = iter_l;
        io_ary[center] = median_value;

        QuickSortRecursive_<T>(io_ary, left, center - 1);
        QuickSortRecursive_<T>(io_ary, center + 1, right);
    }

    template <typename T>
    void QuickSortRecursive_(
        T *io_ary,
        int left_index, int right_index,
        int(*comp)(const void *lhs, const void *rhs))
    {
        //! argument check
        if (left_index >= right_index)
        {
            return;
        }

        int center, left_iter, right_iter;
        T median_value = io_ary[left_index];
        left_iter = left_index;
        right_iter = right_index;
        while (left_iter < right_iter)
        {
            while (left_iter < right_iter && comp(median_value, io_ary[right_iter]) < 0)
            {
                --right_iter;
            }

            if (left_iter < right_iter)
            {
                io_ary[left_iter] = io_ary[right_iter];
                ++left_iter;
            }

            while (left_iter < right_iter && comp(io_ary[left_iter], median_value) < 0)
            {
                ++left_iter;
            }

            if (left_iter < right_iter)
            {
                io_ary[right_iter] = io_ary[left_iter];
                --right_iter;
            }
        }
        center = left_iter;
        io_ary[center] = median_value;

        QuickSortRecursive_<T>(io_ary, left_index, center - 1, comp);
        QuickSortRecursive_<T>(io_ary, center + 1, right_index, comp);
    }
}

// --------------------------------------------------------------------
template<typename T> inline int wsp::CompA(const void *_a, const void *_b){
    T a = *(T*)_a, b = *(T*)_b;
    if(a < b){ return -1; }
    if(a > b){ return 1; }
    return 0;
}
template<typename T> inline int wsp::CompB(const void *_a, const void *_b){
    T a = *(T*)_a, b = *(T*)_b;
    if(a < b){ return 1; }  if(a > b){ return -1; }  return 0;
}
template<typename T> inline void wsp::Swap(T *io_ary, u32 index1, u32 index2){
    T tmp=io_ary[index1];  io_ary[index1]=io_ary[index2];  io_ary[index2]=tmp;
}
template<typename T> inline void wsp::Swap(T **io_ary, u32 index1, u32 index2, u32 len2d){
    for(int i=0; i<len2d; i++){
        T tmp=io_ary[index1][i];  io_ary[index1][i]=io_ary[index2][i];  io_ary[index2][i]=tmp;
    }
}    

template<typename T> inline void wsp::ReverseOrder(T *io_arr, u32 length){
    u32 L=0, R=length-1;
    while(L<R){
        Swap<T>(io_arr, L, R);
        ++L;  --R;
    }
}
template<typename T> inline void wsp::ReverseOrder2D(T **io_arr, u32 len1d, u32 len2d){
    u32 L=0, R=len1d-1;
    while(L<R){
        Swap<T>(io_arr, L, R, len2d);
        ++L;  --R;
    }
}



//! Sorting Algorithms --------------------------------------------------------------------
template <typename T> void wsp::ShellSort(T *io_ary, u32 length)
{
    int step, i, initStep;
    //! decide initial step for insertion sort
    for(step=1; step<(int)length; step=step*3+1){ initStep = step;}

    //! iterate insertion sort with any steps
    for(step=initStep; step>1; step/=3){
        for(i=0; i<step; i++){
            wsp::InsertionSort<T>(&(io_ary[i]), (u32)((int)length-i), step);
            //printf("step=%d, i=%d\n", step, i);
            //wsp::PrintArray(io_ary, (int)length);
            //printf("\n");
        }
    }
    //! finally, apply normal insertion sort
    wsp::InsertionSort(io_ary, length);
}
template <typename T> inline void wsp::InsertionSort(T *io_ary, u32 length, u32 step)
{
    int i, j;
    for(i=1; i<(int)length; i+=(int)step){
        for(j=i+step-1; (j<(u32)length)&&(j>0); j-=(int)step){
            if(io_ary[j]<io_ary[j-(int)step]){ break; }
            wsp::Swap<T>(io_ary, j, j-(int)step);
        }
    }
}
template <typename T> void wsp::SelectionSort(T *io_ary, u32 length)
{
    int i, j;
    T max;
    for(i=0; i<(int)length-1; i++)
    {
        max = io_ary[i];
        int maxIndex = i;
        //! find max
        for(j=i; j<(int)length; j++){
            if(io_ary[j]>max){
                max = io_ary[j];
                maxIndex = j;
            }
        }
        wsp::Swap<T>(io_ary, i, maxIndex);
    }
}

template <typename T> void wsp::BubbleSort(T *io_ary, u32 length)
{
    int i = 0, end=(int)length-1, clearfied=0;
    while(end>0){
        if(io_ary[i]>io_ary[i+1]){
            wsp::Swap<T>(io_ary, i, i+1);
            clearfied=0;
        }else{
            //! decided number of elements
            clearfied++;
        }
        ++i;

        if(i>=end){
            end = end - clearfied;
            i=0;
        }
    }
}

//! QuickSort ----------------------------------------------------------

template <typename T> void wsp::QuickSortRecursive(T *io_ary, u32 length){
    if(length==0)
    {
        return;
    }

    QuickSortRecursive_<T>(io_ary, 0, (int)length-1);
}

//! QuickSort ----------------------------------------------------------
//! Example calls:
//!    QuickSort<int>(&myArray[0],5); // sorts elements 0, 1, 2, 3, and 4
//!    QuickSort<int>(&myArray[3],5); // sorts elements 3, 4, 5, 6, and 7
template <typename T> void wsp::QuickSort(T *io_arr, u32 length)
{
    int left_stack[MAX_BUFFER], right_stack[MAX_BUFFER];
    left_stack[0] = 0;
    right_stack[0] = (int)length;
    int stack_depth = 0;
    while (stack_depth >= 0)
    {
        int left_index = left_stack[stack_depth];
        int right_index = right_stack[stack_depth];

        //! pop from stack
        if (left_index >= right_index)
        {
            --stack_depth;
            continue;
        }

        //! replace left and right values based on pivot value
        T pivot_value = io_arr[left_index];
        while (left_index < right_index)
        {
            while (io_arr[right_index] >= pivot_value && left_index < right_index)
            {
                --right_index;
            }
            if (left_index < right_index)
            {
                io_arr[left_index] = io_arr[right_index];
                ++left_index;
            }
            while (left_index < right_index && io_arr[left_index] <= pivot_value)
            {
                ++left_index;
            }
            if (left_index < right_index)
            {
                io_arr[right_index] = io_arr[left_index];
                --right_index;
            }
        }
        int center = left_index; //! now, left == right == center
        io_arr[center] = pivot_value;

        //! push left and right into stack (0, center-1) (center+1, length)
        left_stack[stack_depth + 1] = center + 1;
        right_stack[stack_depth + 1] = right_stack[stack_depth];
        right_stack[stack_depth] = center - 1;
        ++stack_depth;
    }
}


//! Example calls:
//!        ary = WSP_NEW double*[5];
//!        for(int i=0; i<5; i++){
//!            ary[i] = WSP_NEW double[2]; ary[0]=i; ary[1]=i*3;
//!        }
//!        QuickSort2d<T>(ary, 1, 5, 2);
template <typename T> void wsp::QuickSort2d(T **io_arr, u32 sortIndex, u32 len1d, u32 len2d){
    u32 i, j, k;
    T acpy[MAX_BUFFER][MAX_BUFFER], abuf[MAX_BUFFER];

    //! Copy 2d array to buffer array and swap dimensions
    for(i=0; i<len1d; i++){
        for(u32 j=0; j<len2d; j++){
            acpy[j][i] = io_arr[i][j];
        }
        abuf[i] = acpy[sortIndex][i];
    }
    QuickSort<T>(&abuf[0], len1d);

    //! Swap all data of acpy following to abuf
    for(i=0; i<len1d; i++){
        for(j=i; j<len1d; j++){
            if(abuf[i]==acpy[sortIndex][j]){
                for(k=0; k<len2d; k++){
                    Swap<T>(acpy[k], i, j);
                }
            }
        }
        //std::cout<<acpy[1][i]<<endl;
    }

    //! Copy data and swap dimensions
    for(i=0; i<len2d; i++){
        for(j=0; j<len1d; j++){
            io_arr[j][i] = acpy[i][j];
            //std::cout<<io_arr[j][i]<<" ";
        }
        //std::cout<<endl;
    }
}

template<typename T>
void wsp::QuickSortRecursive(
    T *io_array, int length,
    int(*comp)(const void *lhs, const void *rhs))
{
    if (length == 0)
    {
        return;
    }

    QuickSortRecursive_<T>(io_array, 0, (int)length - 1, comp);
}

#endif