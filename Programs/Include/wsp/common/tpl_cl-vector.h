/**
 * @file tpl_cl-vector.h
 * 
 * @brief Base class for vector type class
 */

#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#define __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__


//#include <omp.h>

#include <iostream>
#include <iomanip>

#ifndef __WSP_COMMONCORE_RESULT_H__
#include "result.h"
#endif
//#include "_define_qt.h"
#include "_tpl_fn-mem_manag.hpp"
#ifndef __WSP_COMMONUTIL_FN_UTIL_H__
#include "fn-util.h"
#endif

#include "_tpl_fn-sort.hpp"
#include "_tpl_fn-array.hpp"


namespace wsp{
    // function using in this class
    template<typename T> inline T mult(const T& left, const T& right){ return left * right; }
    template<typename T> inline T div (const T& left, const T& right){ return left / right; }
    template<typename T> inline T sub (const T& left, const T& right){ return left - right; }
    template<typename T> inline T add (const T& left, const T& right){ return left + right; }
    template<typename T> inline T NoOperation(T value){ return value; }
    template<typename T> inline T SquareOperation(T value){ return value*value; }
    template<typename T> inline T SquareRootOperation(T value){ return (T)sqrt((double)value); }


    //******************************************************************************
    //* CLASS DECLARATION (wsp::Vector)
    //! \ingroup WspCommonUtil
    //! \brief Vector manipulation 
    /*!
      wsp::Vector is a template class which provides access to some of vector
      manipulation functionality. 
    */
    template<class T, int MemAllocStep=50>
    class WSP_DLL_EXPORT Vector
    {
    public:
        Vector();
        explicit Vector(int length, int num_of_channels=1);
        explicit Vector(const T* arry, int length, int num_of_channels=1);
        Vector( const wsp::Vector<T, MemAllocStep> &rhs );
        ~Vector();


        // Accessors: Getter --------------------------
        inline int  length() const;
        inline int  num_of_channels() const;
        inline int  num_of_elem() const{ return num_of_elem_; }
        inline int* length_ptr(){ return &length_; }
        inline int* num_of_channels_ptr(){ return &num_of_channels_; }

        //inline void GetData(T *o_dest) const;
        inline T*       data();
        inline const T* data() const;
        inline T        GetElem(int index) const;
        inline T        GetMin(int channel=-1) const; //! channel=-1 means that search all channels
        inline T        GetMax(int channel=-1) const; //! channel=-1 means that search all channels
        inline T        GetAverage(int channel=-1) const; //! channel=-1 means that search all channels
        inline T        GetSum(int channel=-1) const; //! channel=-1 means that search all channels
        inline bool     HasElem(T elem) const;
        inline int      GetIndex(T elem) const; //! return -1 if elem doesn't exist

        // Accessors: Setter --------------------------
        void                 Append(const T& element, int length=1);
        T                    Pop();
        template<typename U> 
        wsp::State           CopyFrom(const U* ary, int length, int num_of_channels=1, int skipStep=0);
        void                 SetDataFrom(T* data, int length, int num_of_channels=1);
        void                 SetDataInto(T** o_data, int *length, int *num_of_channels);
        void                 SetElem(const T& element, int index, int channel=0);
        wsp::State           SetLength(int length, int num_of_channels=1);
        wsp::State           SetNumOfChannels(int num_of_channels);
        wsp::State           Reset(const T& value);
        wsp::State           Reset(int startIndex, int endIndex, const T& value);
        void                 ResetElems(const T *store_elem, size_t elem_size);
        void                 Reverse();
        inline wsp::State    Clear();
        void                 SortAscending();
        void                 SortDescending();
        void                 ShiftElements(double shiftPixels);
        void                 SetRandNoise(double min, double max);
        void                 Abs();
        void                 Normalize(T min, T max);
        /** 
         * @brief Combine all channels in one channel by using given function
         */
        void                 CombineChannelsByFunc (T (*func)(T), T(*post_func)(T)=wsp::NoOperation); 
        void                 CombineChannelsByNorm ();
        void                 CombineChannelsBySum  ();
        wsp::State           ExtendsNumOfChannels  ( int extended_num_of_channels );
        wsp::State           LeaveOneChannel       ( int channel_index);
        wsp::State           RemoveExceptOneChannel( int channel_index );

        /** @brief Take all matched elements. */
        s32 TakeAllMatchedElems( T target_elem );

        /** @brief Take a matched element which is found first. */
        s32 TakeFirstMatchedElem( T target_elem );

        /** @brief Insert an element to specified location */
        wsp::Result InsertElem( s32 index, T elem );

        // I/O ----------------------------------------
        wsp::State SaveRawData(const char *in_fileNameFormat,...) const;
        wsp::State LoadRawData(const char *fileName);

        //#ifdef USE_QT
        //inline wsp::State SaveRawDataDialog(const char *title="Save Raw Data") const;
        //inline wsp::State LoadRawDataDialog(const char *title="Open Raw Data");
        //#endif

        // Operators ----------------------------------
        inline wsp::Vector<T, MemAllocStep>& operator=(const wsp::Vector<T, MemAllocStep>& other);
        inline bool operator==(const wsp::Vector<T, MemAllocStep>& other) const;
        inline bool operator!=(const wsp::Vector<T, MemAllocStep>& other) const;

        inline T& operator[](int index);
        //inline T& operator[](int index);
        inline const T& operator[](int index) const;
        //inline const T operator[](int index) const;

        inline wsp::Vector<T, MemAllocStep>  operator+ (const wsp::Vector<T, MemAllocStep> &p) const;
        inline wsp::Vector<T, MemAllocStep>  operator+ (const T& right) const;
        inline wsp::Vector<T, MemAllocStep>  operator- (const wsp::Vector<T, MemAllocStep> &p) const;
        inline wsp::Vector<T, MemAllocStep>  operator- (const T& right) const;
        inline wsp::Vector<T, MemAllocStep>  operator* (const wsp::Vector<T, MemAllocStep> &p) const;
        inline wsp::Vector<T, MemAllocStep>  operator* (const T& right) const;
        inline wsp::Vector<T, MemAllocStep>  operator/ (const wsp::Vector<T, MemAllocStep> &p) const;
        inline wsp::Vector<T, MemAllocStep>  operator/ (const T& right) const;

        inline wsp::Vector<T, MemAllocStep>& operator+=(const wsp::Vector<T, MemAllocStep> &p);
        inline wsp::Vector<T, MemAllocStep>& operator+=(const T& right);

        inline wsp::Vector<T, MemAllocStep>& operator-=(const wsp::Vector<T, MemAllocStep> &p);
        inline wsp::Vector<T, MemAllocStep>& operator-=(const T& right);

        inline wsp::Vector<T, MemAllocStep>& operator*=(const wsp::Vector<T, MemAllocStep> &p);
        inline wsp::Vector<T, MemAllocStep>& operator*=(const T& right);

        inline wsp::Vector<T, MemAllocStep>& operator/=(const wsp::Vector<T, MemAllocStep> &p);
        inline wsp::Vector<T, MemAllocStep>& operator/=(const T& right);
        inline wsp::Vector<T, MemAllocStep>& operator<<(const T &rhs){ Append(rhs); return *this; }
        inline wsp::Vector<T, MemAllocStep>& operator<<(const wsp::Vector<T, MemAllocStep> &rhs){
            for(int i=0; i<rhs.length(); i++){ Append(rhs[i]); }
            return *this; 
        }


    protected:
        wsp::Vector<T, MemAllocStep> operation(const T& value, T(*op)(const T& left, const T& right)) const;
        wsp::Vector<T, MemAllocStep> operation(const wsp::Vector<T, MemAllocStep>& ary, T(*op)(const T& left, const T& right)) const;
        wsp::Vector<T, MemAllocStep> division(const T& value) const;
        wsp::Vector<T, MemAllocStep> division(const wsp::Vector<T, MemAllocStep>& ary) const;

        inline void InitMemberVariables()
        {
            length_=0;
            num_of_channels_=0;
            num_of_elem_=0;
            mem_size_=0;
        }

    protected:
        /** @brief  length of array. num_of_channels_*length_=num_of_elems_ */
        int length_;

        /** @brief  number of channels.  */
        int num_of_channels_;

        /** @brief  number of elements in array. num_of_channels_*length_=num_of_elems_ */
        int num_of_elem_;

        int mem_size_;

        T* data_;

        bool has_own_data_;    
    };

    template class Vector<u8>;
}

#include "tpl_cl-vector.inl"

#endif