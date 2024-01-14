/**
 * @file tpl_cl-matrix_n.h
 * 
 * @brief Stack matrix template class
 */

#ifndef _WSP_MATH_TEMPLATE_CLASS_MATRIX_N_H__
#define _WSP_MATH_TEMPLATE_CLASS_MATRIX_N_H__

#include <math.h>
#include <iostream>

#include <wsp/common/tpl_cl-vector_n.h>
#ifdef USE_QT
    #include <QtGui/QtGui>
#endif

namespace wsp{
    //! \ingroup WspMath
    //! \brief N x N matrix manipulation class allocated in stack memory
    /*!
        T is type, N_ELEM is number of elements
    */
    template<class T=float, int N_ROW=4, int N_COLUMN=4>
    class MatrixN
        : public wsp::VectorN<T, N_ROW*N_COLUMN>
    {
    public:
        // Constructors ----------------------------------------
        inline MatrixN(): wsp::VectorN<T, N_ROW*N_COLUMN>(){}
        inline MatrixN(const MatrixN<T, N_ROW, N_COLUMN> &mat): wsp::VectorN<T, N_ROW*N_COLUMN>(mat){}
        inline MatrixN(const T mat[N_ROW*N_COLUMN]): wsp::VectorN<T, N_ROW*N_COLUMN>(mat){}

        // Accessors -------------------------------------------
        inline void CopyToArray(T o_dest[N_ROW*N_COLUMN]) const{
            for(int i=0; i < N_ROW*N_COLUMN; i++)
            {
                o_dest[i] = wsp::VectorN<T, N_ROW*N_COLUMN>::data_[i];
            }
        }
        inline wsp::MatrixN<T, N_COLUMN, N_ROW> Transpose() const{
            wsp::MatrixN<T, N_COLUMN, N_ROW> res;
            u32 row, col;
            for(row=0; row<N_COLUMN; row++){
                for(col=0; col<N_ROW; col++){
                    //SetElem(mat_tmp.GetElem(itr_c, itr_r), itr_r, itr_c); 
                    res[row*N_ROW + col] = wsp::VectorN<T, N_ROW*N_COLUMN>::data_[col*N_COLUMN + row];
                }
            }
            return res;
        }

        // Mutators --------------------------------------------
        inline wsp::MatrixN<T, N_ROW, N_COLUMN>& SetToIdentity()
        {
            int length = N_ROW*N_COLUMN;
            memset(wsp::VectorN<T, N_ROW*N_COLUMN>::data_, 0, sizeof(T)*length);
            for(int i=0; i<length; i+=N_COLUMN+1){
                wsp::VectorN<T, N_ROW*N_COLUMN>::data_[i] = 1;
            }
            return *this;
        }
        // Operators -------------------------------------------
        //inline wsp::MatrixN<T>& operator=(const wsp::MatrixN<T> &rhs){
        //    //wsp::VectorN<T, N_ROW*N_COLUMN>::operator=(rhs);
        //    memcpy(wsp::VectorN<T, N_ROW*N_COLUMN>::data_, rhs.data(), sizeof(T)*N_ROW*N_COLUMN);
        //    return *this;
        //}
        //inline bool operator==(const wsp::TransformMatrix<T> &right) const{
        //    for(int i=0; i<16; i++){ if(wsp::VectorN<T, N_ROW*N_COLUMN>::data_[i]!=right[i]){ return false; } }
        //    return true;
        //}
        //inline bool operator!=(const wsp::TransformMatrix<T> &right) const{ return !(*this==right); }
        inline T operator()(int row, int col) const{
            if(row>N_ROW || col>N_COLUMN){ WSP_COMMON_ERROR_LOG("row or col exceeds size\n"); }
            return wsp::VectorN<T, N_ROW*N_COLUMN>::data_[row*N_COLUMN + col];
        }
        //inline T operator[](u32 index) const{
        //    if(row>16){ WSP_COMMON_ERROR_LOG("index exceeds 16\n"); }
        //    return wsp::VectorN<T, N_ROW*N_COLUMN>::data_[index]; 
        //}
        //inline T& operator[](u32 index){
        //    if(row>16){ WSP_COMMON_ERROR_LOG("index exceeds 16\n"); }
        //    return wsp::VectorN<T, N_ROW*N_COLUMN>::data_[index]; 
        //}
        //inline wsp::TransformMatrix<T>& operator+=(const wsp::TransformMatrix<T> &right){
        //    for(int i=0; i<16; i++){ wsp::VectorN<T, N_ROW*N_COLUMN>::data_[i] += right[i]; }
        //    return *this;
        //}
        //inline wsp::TransformMatrix<T> operator+(const wsp::TransformMatrix<T> &right)const{
        //    wsp::TransformMatrix mat(*this);
        //    for(int i=0; i<16; i++){ mat[i] += right[i]; }
        //    return mat;
        //}
        //inline wsp::TransformMatrix<T>& operator-=(const wsp::TransformMatrix<T> &right){
        //    for(int i=0; i<16; i++){ wsp::VectorN<T, N_ROW*N_COLUMN>::data_[i] -= right[i]; }
        //    return *this;
        //}
        //inline wsp::TransformMatrix<T> operator-(const wsp::TransformMatrix<T> &right)const{
        //    wsp::TransformMatrix mat(*this);
        //    for(int i=0; i<16; i++){ mat[i] -= right[i]; }
        //    return mat;
        //}

        wsp::MatrixN<T, N_ROW, N_COLUMN> operator*(const wsp::MatrixN<T, N_ROW, N_COLUMN> &rhs)const;
        wsp::MatrixN<T, N_ROW, N_COLUMN> operator*(const T &rhs)const;
        wsp::MatrixN<T, N_ROW, N_COLUMN>& operator*= (const wsp::MatrixN<T, N_ROW, N_COLUMN> &rhs);
    };
}

template<class T, int N_ROW, int N_COLUMN>
wsp::VectorN<T, N_ROW> operator*(
    const wsp::MatrixN<T, N_ROW, N_COLUMN> &lhs,
    const wsp::VectorN<T, N_ROW> &rhs)
{
    wsp::VectorN<T, N_ROW> dest;
    T sum;
    int row, col;
    for (row = 0; row<N_ROW; ++row)
    {
        sum = 0;
        for (col = 0; col<N_COLUMN; ++col)
        {
            sum += lhs[row*N_COLUMN + col] * rhs[col];
        }
        dest[row] = sum;
    }
    return dest;
}

template<class T, int N_ROW, int N_COLUMN>
wsp::VectorN<T, N_COLUMN> operator*(
    const wsp::VectorN<T, N_COLUMN> &lhs,
    const wsp::MatrixN<T, N_ROW, N_COLUMN> &rhs)
{
    wsp::VectorN<T, N_ROW> dest;
    T sum;
    int row, col;
    for (row = 0; row<N_ROW; ++row)
    {
        sum = 0;
        for (col = 0; col<N_COLUMN; ++col)
        {
            T value_lhs = lhs[col];
            T value_rhs = rhs[col*N_COLUMN + row];
            sum += value_lhs * value_rhs;
        }
        dest[row] = sum;
    }
    return dest;
}


// Operators ----------------------
template<class T, int N_ROW, int N_COLUMN>
wsp::MatrixN<T, N_ROW, N_COLUMN> wsp::MatrixN<T, N_ROW, N_COLUMN>::operator*(const wsp::MatrixN<T, N_ROW, N_COLUMN> &right)const
{
#if 0
    // not optimized version
    wsp::MatrixN<T, N_ROW, N_COLUMN> dest;
    T sum;
    int row, col, i;
    for(row=0; row<N_ROW; ++row)
    {
        for(col=0; col<N_COLUMN; ++col)
        {
            sum = 0;
            for(i=0; i<N_COLUMN; ++i)
            {
                sum += wsp::VectorN<T, N_ROW*N_COLUMN>::data_[row*N_COLUMN + i] * right[i*N_COLUMN + col];
            }
            dest[row * N_COLUMN + col] = sum;
        }
    }
    return dest;
#else
    wsp::MatrixN<T, N_ROW, N_COLUMN> dest;

    const int N_LENGTH = N_ROW * N_COLUMN;
    for (int dest_index = 0; dest_index < N_LENGTH; ++dest_index)
    {
        int row_offset = dest_index & ~(N_COLUMN - 1); // 4x4 ‚¾‚Æ 0, 0, 0, 0, 4, 4, 4, 4, 8, 8, 8, 8, 12, 12, 12, 12
        int col_offset = dest_index & (N_ROW - 1);  // 4x4 ‚¾‚Æ 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,  0,  1,  2,  3

        T* ptr = &dest[dest_index];
        *ptr = static_cast<T>(0);
        for (int i = 0; i < N_ROW; ++i)
        {
            *ptr += wsp::VectorN<T, N_LENGTH>::data_[row_offset + i] * right[i * N_COLUMN + col_offset];
        }
    }

    return dest;
#endif
}
template<class T, int N_ROW, int N_COLUMN>
wsp::MatrixN<T, N_ROW, N_COLUMN> wsp::MatrixN<T, N_ROW, N_COLUMN>::operator*(const T &rhs)const
{
    wsp::MatrixN<T, N_ROW, N_COLUMN> dest;
    for (int i = 0; i < N_ROW * N_COLUMN; ++i)
    {
        dest[i] = this->data_[i] * rhs;
    }
    return dest;
}

template<class T, int N_ROW, int N_COLUMN>
wsp::MatrixN<T, N_ROW, N_COLUMN>& wsp::MatrixN<T, N_ROW, N_COLUMN>::operator*= (const wsp::MatrixN<T, N_ROW, N_COLUMN> &rhs)
{
    *this = *this * rhs;
    return *this;
}

// I/O --------------------------------------------------------

template<class T, int N_ROW, int N_COLUMN>
inline std::ostream& operator<<(std::ostream& lhs, const wsp::MatrixN<T, N_ROW, N_COLUMN> &rhs)
{
    int row, col;
    for(row=0; row<N_ROW; row++){ 
        for(col=0; col<N_COLUMN-1; col++){ 
            lhs<<rhs[row*N_COLUMN+col]<<", "; 
        }
        lhs<<rhs[row*N_COLUMN+col]<<std::endl;
    }
    return lhs;
}

template<class T, int N_ROW, int N_COLUMN>
inline std::ostream& operator<<(std::ostream& lhs, const wsp::MatrixN<T, N_ROW, N_COLUMN> *rhs)
{
    lhs<<*rhs;
    return lhs;
}

#endif