// @file tpl_cl-matrix.h
// 

#ifndef __WSP_CORE_CLASS_MATRIX_H__
#define __WSP_CORE_CLASS_MATRIX_H__

#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#include <wsp/common/tpl_cl-vector.h>
#endif
#include "cl-function.h"

#ifdef USE_OPENCV
#include <load_opencv.h>
#endif

namespace wsp{
    //! \ingroup WspMath
    //! \brief Matrix manipulation class allocated in heap memory
    template<typename T>
    class Matrix
        : public wsp::Vector<T>
    {
    protected:
        int column_size_, row_size_;

    protected:
        void Init();
        wsp::Matrix<T> multiply(const wsp::Matrix<T>& mult) const;

    public:
        Matrix();
        Matrix(u32 nRow, u32 nColumn);
        Matrix(const wsp::Matrix<T>& mat);
        Matrix(const T **mat, u32 row, u32 col);
        Matrix(const T *mat, u32 row, u32 col);
        ~Matrix();

        // Getter -------------------------------------------
        inline u32 row_size() const{ return row_size_; }
        inline u32 column_size() const{ return column_size_; }
        inline T GetElem(u32 row, u32 col) const{
            return wsp::Vector<T>::GetElem((row*column_size_+col)*num_of_channels_);
        }
        inline wsp::State CopyIntoArray(T *ary, u32 length) const;
        inline wsp::State CopyInto2dArray(T **mat, u32 row, u32 col) const;
        bool is_upperTriangular() const;
        bool is_lowerTriangular() const;

        // Setter --------------------------------------------
        inline void SetElem(T value, u32 row, u32 col){
            data_[(row*column_size_ + col)*num_of_channels_] = value;
        }
        inline wsp::State CopyFromArray(const T *mat, u32 row, u32 col);
        inline wsp::State CopyFrom2dArray(const T **mat, u32 row, u32 col);
        wsp::State SetSize(u32 nRow, u32 nColumn);

        //! Matrix operations
        void SetToIdentity(u32 size);
        void Transpose();
        void SwapRow(u32 row1, u32 row2);
        void SwapColumn(u32 col1, u32 col2);
        void Invertrow_size();
        void Invertcolumn_size();
        void SplitVertical(u32 col_start, u32 col_end);
        void MergeRight(const wsp::Matrix<T>& mat);
        void SumRow(wsp::Vector<T>& dest);
        void SumRowWeighted(wsp::Vector<T>& dest, const wsp::Vector<T>& weights);
        void DivideByPivot();
        void SortRowByVector(const wsp::Vector<T> &sortSrc, wsp::Order order = wsp::Ascending);
        void Reverserow_size();

        // Elimination Methods
        //   Gaussian Elimination
        void GaussianElimination();
        void Eliminate(u32 rowEliminated, u32 rowEliminator);

        void PivotSelection(u32 pivotIndex);
        void PivotSelection();
        void ForwardSubstitution();
        void backSubstitution();

        //   LU Decomposition
        void LUDecomposition();
        void LUDecomposition(wsp::Matrix<T>& L, wsp::Matrix<T>& U) const;
        void LUElimination();
        void LUElimination(const wsp::Matrix<T>& LU);
        void LUElimination(const wsp::Matrix<T>& L, const wsp::Matrix<T>& U);

        // Principle Component Analysis
        void GetVarianceCovarianceMat();
        void GetVarianceCovarianceMat(const wsp::Vector<T> &mean);
        void GetVarianceCovarianceMat(const T *mean, u32 length);
        #ifdef USE_OPENCV
        void GetEigen(wsp::Matrix<T> &out_eigenVec, wsp::Function &out_eigenVal) const;
        void PCA(wsp::Matrix<T> &out_eigenVec, wsp::Matrix<T> &out_eigenVal) const;
        void CopyIntoArrayCvMat(CvMat **out_mat)const;
        #endif

        // Integration Methods
        void RungeKutta4th(u32 destIndex, const wsp::Vector<T>& initValues, void(*dfunc)(T t, const T* y, T* dest));
        void RungeKuttaGill(u32 destIndex, const wsp::Vector<T>& initValues, void(*dfunc)(T t, const T* y, T* dest));


        // Operators
        wsp::Matrix<T>& operator=(const wsp::Matrix<T>& mat);
        bool operator==(const wsp::Matrix<T> &mat) const;
        T operator()(u32 row, u32 col) const;
        T operator()(int row, int col) const;
        wsp::Matrix<T> operator*(const wsp::Matrix<T>& mat) const;
        wsp::Matrix<T>& operator*=(const wsp::Matrix<T>& mat);

        #ifdef USE_OPENCV
        wsp::Matrix<T>& operator=(const CvMat *mat);
        #endif
    };

}

template<typename T>
inline std::ostream& operator<<(std::ostream& lhs, const wsp::Matrix<T> &rhs)
{
    for(u32 itr_r=0; itr_r<rhs.row_size(); itr_r++){
        for(u32 itr_c=0; itr_c<rhs.column_size(); itr_c++){
            lhs<<setw(PRINT_COLW)<<setprecision(PRINT_PREC);
            lhs<<PRINT_TYPE;
            lhs<<rhs.GetElem(itr_r, itr_c);
        }
        lhs<<endl;
    }
    return lhs;
}
template<typename T>
inline std::ostream& operator<<(std::ostream& lhs, const wsp::Matrix<T> *rhs)
{
    lhs<<*rhs;
    return lhs;
}


// Constructor, Destructor -------------------------------------------------
#define INIT_WSP_MATRIX column_size_(0), row_size_(0)


template<typename T> wsp::Matrix<T>::Matrix()
    :INIT_WSP_MATRIX
{}
template<typename T> wsp::Matrix<T>::Matrix(u32 nRow, u32 nColumn)
    :INIT_WSP_MATRIX
{
    SetSize(nRow, nColumn);
}
// Copy Constructor
template<typename T> wsp::Matrix<T>::Matrix(const wsp::Matrix<T>& mat)
    :INIT_WSP_MATRIX, wsp::Vector<T>(mat)
{
    column_size_ = mat.column_size();
    row_size_ = mat.row_size();
}
template<typename T> wsp::Matrix<T>::Matrix(const T **mat, u32 row, u32 col)
    :INIT_WSP_MATRIX
{
    CopyFrom2dArray(mat, row, col);
}
template<typename T> wsp::Matrix<T>::Matrix(const T *mat, u32 row, u32 col)
    :INIT_WSP_MATRIX
{
    CopyFromArray(mat, row, col);
}
template<typename T> wsp::Matrix<T>::~Matrix(){}

// end Constructor, Destructor ---------------------------------------------

// Getter ------------------------------------------

template <typename T>
inline wsp::State wsp::Matrix<T>::CopyIntoArray(T *ary, u32 length) const{
    if(length != length_)
    { WSP_COMMON_ERROR_LOG("length must be the same as matrix length\n"); return WSP_STATE_FAILURE; }
    for(u32 itr_i=0; itr_i<length; itr_i++){
        ary[itr_i] = data_[itr_i];
    }
    return WSP_STATE_SUCCESS;
}
template <typename T>
inline wsp::State wsp::Matrix<T>::CopyInto2dArray(T **ary, u32 row, u32 col) const{
    if(row_size_ != row || column_size_ != col)
    { WSP_COMMON_ERROR_LOG("row and column must be the same as matrix length\n"); return WSP_STATE_FAILURE; }
    for(u32 itr_i=0, itr_r=0; itr_r<row; itr_r++){
        for(u32 itr_c=0; itr_c<col; itr_c++){
            ary[itr_r][itr_c] = data_[itr_i];
            itr_i++;
        }
    }
    return WSP_STATE_SUCCESS;
}


// Setter ------------------------------------------------------------------

template <typename T>
inline wsp::State wsp::Matrix<T>::CopyFromArray(const T *mat, u32 row, u32 col){
    if(mat==NULL){ WSP_COMMON_ERROR_LOG("argument pointer is NULL\n"); return WSP_STATE_FAILURE; }
    SetSize(row, col);
    for(u32 itr_i=0; itr_i<length_; itr_i++){
        data_[itr_i] = mat[itr_i];
    }
    return WSP_STATE_SUCCESS;
}
template <typename T>
inline wsp::State wsp::Matrix<T>::CopyFrom2dArray(const T **mat, u32 row, u32 col){
    if(mat==NULL){ WSP_COMMON_ERROR_LOG("argument pointer is NULL\n"); return WSP_STATE_FAILURE; }
    column_size_=col;  length_=row;
    for(u32 itr_i=0, itr_r=0; itr_r<row; itr_r++){
        for(u32 itr_c=0; itr_c<col; itr_c++){
            data_[itr_i] = mat[itr_r][itr_c];
            itr_i++;
        }
    }
    return WSP_STATE_SUCCESS;
}

template<typename T> wsp::State wsp::Matrix<T>::SetSize(u32 nRow, u32 nColumn){
    if(nRow==0 || nColumn==0){
        delete[] data_;
        data_ = NULL;
        length_ = 0;
        column_size_ = 0;
        row_size_ = 0;
        return WSP_STATE_FAILURE;
    }

    T start, end;
    u32 itr_i, itr_j, len_old;
    wsp::Matrix<T> tmp(*this);

    len_old = length_;
    length_ = nRow * nColumn;
    column_size_ = nColumn;
    row_size_ = nRow;
    delete[] data_;
    data_ = WSP_NEW T[length_ + RESERVE_MEM];

    start = 0.0;
    end = 1.0;

    for(itr_i=0; itr_i<length_; itr_i++){
        if(itr_i < len_old){
            data_[itr_i] = tmp[itr_i];
            continue;
        }
        data_[itr_i] = 0.0;
    }
    return WSP_STATE_SUCCESS;
}

//! Matrix operations 
template<typename T> void wsp::Matrix<T>::SetToIdentity(u32 size){
    if(size==0){ return; }
    SetSize(size, size);
    for(u32 itr_i=0; itr_i<size; itr_i++)
    for(u32 itr_j=0; itr_j<size; itr_j++){
        if(itr_i!=itr_j){
            SetElem(0.0, itr_i, itr_j);
            continue;
        }
        SetElem(1.0, itr_i, itr_j);
    }
}
template<typename T> void wsp::Matrix<T>::Transpose(){
    if(row_size_==0 || column_size_==0){ return; }

    int len = column_size_ * row_size_;
    T *tmp_data = WSP_NEW T[len];
    // copy data
    {
        T *dst_ptr = tmp_data;
        T *src_ptr = data_;
        T *end_ptr = dst_ptr + len;
        for(;dst_ptr!=end_ptr; ++dst_ptr, ++src_ptr){
            *dst_ptr = *src_ptr;
        }
    }
    // transpose
    {
        T *src_ptr = tmp_data;
        T *end_src = tmp_data + len - 1;
        T *dst_ptr = data_;
        T *end_dst = data_ + len;
        int back_pix = len - 1;
        //int i=0, j=0;
        for(; dst_ptr!=end_dst; ++dst_ptr, src_ptr+=row_size_){
            if(src_ptr>end_src){
                src_ptr -= back_pix;
                //i -=back_pix;
            }
            *dst_ptr = *src_ptr;
            //printf("[%d] = [%d]\n", j, i);
            //i+=row_size_, ++j;
        }
    }
    // swap column and row size
    int tmp = column_size_;
    column_size_ = row_size_;
    row_size_ = tmp;

    delete[] tmp_data;
}
template<typename T> void wsp::Matrix<T>::SwapRow(u32 row1, u32 row2){
    if(row1>=row_size_ || row2>=row_size_ || row1==row2){ return;}
    T tmp;
    for(int itr_c=0; itr_c<column_size_; itr_c++){
        tmp = GetElem(row1, itr_c);
        SetElem(GetElem(row2, itr_c), row1, itr_c);
        SetElem(tmp, row2, itr_c);
    }
}
template<typename T> void wsp::Matrix<T>::SwapColumn(u32 col1, u32 col2){
    if(col1>=column_size_ || col2>=column_size_ || col1==col2){ return;}
    Transpose();
    SwapRow(col1, col2);
    Transpose();
}
template<typename T> void wsp::Matrix<T>::Invertrow_size(){
    for(u32 itr_i=0; itr_i<length_/2; itr_i++){
        SwapRow(itr_i, length_-itr_i-1);
    }
}
template<typename T> void wsp::Matrix<T>::Invertcolumn_size(){
    for(u32 itr_i=0; itr_i<column_size_/2; itr_i++){
        SwapColumn(itr_i, column_size_-itr_i-1);
    }
}
template<typename T> void wsp::Matrix<T>::SplitVertical(u32 col_start, u32 col_end){
    int colw = (int)col_end - (int)col_start;
    if(colw < 0 || col_end>=column_size_){
        WSP_COMMON_ERROR_LOG("col_end must be equal to or larger than col_start.\n");
        return;
    }
    wsp::Matrix<T> mat(*this);
    SetSize(row_size_, colw+1);
    for(u32 itr_r=0; itr_r<length_; itr_r++)
    for(u32 itr_c=0; itr_c<column_size_; itr_c++){
        SetElem(mat.GetElem(itr_r, itr_c+col_start), itr_r, itr_c);
    }
}
template<typename T> void wsp::Matrix<T>::MergeRight(const wsp::Matrix<T>& mat){
    u32 col, col_pre;
    if(row_size_ != mat.row_size()){
        WSP_COMMON_ERROR_LOG("2 matrices have to have the same number of rows.\n");
        return;
    }
    col_pre = column_size_;
    col = col_pre + mat.column_size();
    SetSize(row_size_, col);
    for(u32 itr_i=0; itr_i<length_; itr_i++){
        for(u32 itr_j=0; itr_j<mat.column_size(); itr_j++){
            SetElem(mat.GetElem(itr_i,itr_j), itr_i, itr_j+col_pre);
        }
    }
}
template<typename T> void wsp::Matrix<T>::SumRowWeighted(wsp::Vector<T>& dest, const wsp::Vector<T>& weights){
    if(weights.length()!=column_size_ || length_==0){
        WSP_COMMON_ERROR_LOG("length of weights and destination array are different.\ndest=%d, weights=%d\n",
                  length_, weights.length());
        return;
    }
    dest.SetLength(column_size_);
    dest.Reset(0);
    for(u32 itr_r=0; itr_r<row_size_; itr_r++){
        for(u32 itr_c=0; itr_c<column_size_; itr_c++){
            dest[itr_c] += GetElem(itr_r,itr_c) * weights[itr_c];
        }
    }
}
template<typename T> void wsp::Matrix<T>::SumRow(wsp::Vector<T>& dest)
{
    if(length_ == 0){
        return;
    }
    wsp::Vector<T> weights(column_size_);
    weights.Reset(1.0);
    SumRowWeighted(dest, weights);
}
template<typename T> void wsp::Matrix<T>::DivideByPivot(){
    wsp::Matrix<T> mat(*this);
    T pivot;
    for(u32 itr_r=0; itr_r<length_; itr_r++){
        //mat.PivotSelection(itr_i);
        pivot = mat.GetElem(itr_r, itr_r);
        if(pivot != 0){
            for(u32 itr_c=0; itr_c<row_size_; itr_c++){
                mat[itr_r*column_size_+itr_c] /= pivot;
            }
        }
    }
    operator=(mat);
}


template<typename T> void wsp::Matrix<T>::SortRowByVector(const wsp::Vector<T> &in_sortSrc, wsp::Order order){
    wsp::Vector<T> sortSrc(in_sortSrc);
    int beg[MAX_BUFFER], end[MAX_BUFFER], i, L, R;
    T piv;
    T tmpvec[MAX_BUFFER];
    beg[0] = 0;  end[0] = (int)row_size();  i = 0;
    while(i>=0)
    {
        L=beg[i];  R=end[i];
        if(L>=R) {
            i--;  continue;
        }
        piv=sortSrc[L];
        wsp::CopyArray<T>(tmpvec, &data_[L*column_size_], row_size_);
        while (L<R){
            while(sortSrc[R]>=piv && L<R){ R--; }
            if(L<R){
                sortSrc[L] = sortSrc[R];
                wsp::CopyArray<T>(&data_[L*column_size_], &data_[R*column_size_], row_size_);
                L++;
            }
            while(sortSrc[L]<=piv && L<R){ L++; }
            if(L<R){
                sortSrc[R] = sortSrc[L];
                wsp::CopyArray<T>(&data_[R*column_size_], &data_[L*column_size_], row_size_);
                R--;
            }
        }
        sortSrc[L] = piv;
        wsp::CopyArray<T>(&data_[L*column_size_], tmpvec, row_size_);
        beg[i+1] = L+1;  end[i+1] = end[i];  end[i++] = L-1;
    }
    if(order==wsp::Ascending){
        Reverserow_size();
    }
}
template<typename T> void wsp::Matrix<T>::Reverserow_size(){
    int halfRow = (int)floor((float)row_size_/2);
    for(int itr_r=0; itr_r<halfRow; itr_r++){
        template<typename T> wsp::Matrix<T>::SwapRow(itr_r, row_size_-itr_r-1);
    }
}



// end Setter --------------------------------------------------------------


//! Elimination methods -----------------------------------------------------

template<typename T> void wsp::Matrix<T>::GaussianElimination(){
    if(length_>=column_size_){ return; }
    float pivot;
    PivotSelection();
    for(u32 itr_i=0; itr_i<length_; itr_i++){
        data_[itr_i] /= data_[itr_i][itr_i];
        count+=length_;
        for(u32 itr_j=itr_i+1; itr_j<length_; itr_j++){
            if(itr_i==itr_j){
                continue;
            }
            Eliminate(itr_j, itr_i);
        }
    }
    //cout<<*this<<endl;
    backSubstitution();
}
template<typename T> void wsp::Matrix<T>::PivotSelection(u32 pivotIndex){
    T max;
    u32 itr_i, maxIndex;
    if(pivotIndex>=length_){ return; }
    max = data_[pivotIndex][pivotIndex];
    maxIndex = pivotIndex;
    // finding max
    for(itr_i=pivotIndex+1; itr_i<length_; itr_i++){
        if(max < data_[itr_i][pivotIndex]){
            max = data_[itr_i][pivotIndex];
            maxIndex = itr_i;
        }
    }
    SwapRow(pivotIndex, maxIndex);
}
template<typename T> void wsp::Matrix<T>::PivotSelection(){
    for(u32 itr_i=0; itr_i<length_; itr_i++){
        PivotSelection(itr_i);
    }
}

template<typename T> void wsp::Matrix<T>::Eliminate(u32 rowEliminated, u32 pivot){
    T pivotValueL;
    T sub;
    if(rowEliminated>=length_ || pivot>=length_){
        return;
    }

    pivotValueL = data_[rowEliminated][pivot];

    if(pivotValueL != 0.0){
        for(u32 itr_i=0; itr_i<column_size_; itr_i++){
            sub = data_[pivot][itr_i] * pivotValueL;
            data_[rowEliminated][itr_i] -= sub;
            count++;
            count++;
        }
    }
}

template<typename T> void wsp::Matrix<T>::backSubstitution()
{
    u32 itr_i, row, col, col_result;
    if((int)column_size_-(int)length_<1 && !is_upperTriangular()){ return; }

    col_result = length_;
    //cout<<"back: "<<endl<<*this<<endl;
    for(col=length_-1; col>=0; col--)
    {
        if(data_[col][col] == 0){ return; }
        for(itr_i=col_result; itr_i<(int)column_size_; itr_i++){
            data_[col][itr_i] /= data_[col][col];
            count++;
        }
        data_[col][col] = 1.0; // /= data_[col][col];
        count++;
        for(row=col-1; row>=0; row--)
        {
            //cout<<row<<", "<<col<<endl;
            for(itr_i=col_result; itr_i<(int)column_size_; itr_i++){
                data_[row][itr_i] -= data_[col][itr_i] * data_[row][col];
                count++;
            }
            data_[row][col] = 0.0; // -= data_[col][col] * data_[row][col];
            count++;
            //cout<<*this<<endl;
        }
    }
}

template<typename T> void wsp::Matrix<T>::ForwardSubstitution()
{
    if((int)column_size_-(int)length_<1 && !is_lowerTriangular()){ return;}
    u32 itr_i, row, col, col_result;

    col_result = column_size_-1;
    //cout<<"forward: "<<endl<<*this<<endl;
    for(col=0; col<length_; col++)
    {
        if(data_[col][col] == 0){ return; }
        for(itr_i=col_result; itr_i<column_size_; itr_i++){
            data_[col][itr_i] /= data_[col][col];
            count++;
        }
        data_[col][col] = 1.0; // /= data_[col][col];
        count++;
        for(row=col+1; row<length_; row++)
        {
            //cout<<row<<", "<<col<<endl;
            for(itr_i=col_result; itr_i<column_size_; itr_i++){
                data_[row][itr_i] -= data_[col][itr_i] * data_[row][col];
                count++;
            }
            data_[row][col] = 0.0; // -= data_[col][col] * data_[row][col];
            count++;
            //cout<<*this<<endl;
        }
    }
}

template<typename T> bool wsp::Matrix<T>::is_upperTriangular() const
{
    if(length_!=column_size_){
        return false;
    }
    for(u32 itr_i=0; itr_i<length_; itr_i++)
    for(u32 itr_j=0; itr_j<=itr_i; itr_j++){
        if(itr_i==itr_j){
            //if(data_[(itr_i*column_size_+itr_i)*num_of_channels_]!=1){
            //    return false;
            //}
            continue;
        }
        if(data_[(itr_i*column_size_+itr_i)*num_of_channels_]!=0){
            return false;
        }
    }
    return true;
}

template<typename T> bool wsp::Matrix<T>::is_lowerTriangular() const
{
    bool ret;
    wsp::Matrix<T> mat(*this);
    mat.Transpose();
    ret = mat.is_upperTriangular();
    return ret;
}

template<typename T> void wsp::Matrix<T>::LUDecomposition(wsp::Matrix<T>& L, wsp::Matrix<T>& U) const
{
    wsp::Matrix<T> mat(*this);
    u32 itr_i, itr_j, itr_k;
    u32 col, row;

    // initializing L and U matrix
    L.SetSize(length_, length_);
    U.SetSize(length_, length_);
    L.Reset(0.0);
    U.Reset(0.0);

    //--------------------------------------------
    // Lower and Upper matrix will be like below
    //
    // U=|  1  u12 u13 |    L=| l11  0   0  |
    //   |  0   1  u23 |      | l21 l22  0  |
    //   |  0   0   1  |      | l31 l32 l33 |
    //--------------------------------------------
    for(itr_i=0; itr_i<length_; itr_i++){
        for(itr_j=0; itr_j<itr_i+1; itr_j++)
        {
            // Lower Matrix Calculation:
            //     Lij = Aij - sum(Lik*Ukj, k=[1, j-1])
            row = itr_i;
            col = itr_j;
            L[row][col] = mat[row][col];
            for(itr_k=0; itr_k<col; itr_k++){
                L[row][col] -= L[row][itr_k] * U[itr_k][col];
            }
            //cout<<"L:"<<row<<" "<<col<<L<<endl;

            // Upper Matrix Calculation:
            //     Uij =( Aij - sum(Lik*Ukj, k=[1, i-1]) )/Lii
            row = itr_j;
            col = itr_i;
            U[row][col] = mat[row][col];
            for(itr_k=0; itr_k<row; itr_k++){
                U[row][col] -= L[row][itr_k] * U[itr_k][col];
            }
            U[row][col] /= L[row][row];

            //cout<<"U:"<<row<<" "<<col<<U<<endl;
        }
    }
}

// put result into myself
template<typename T> void wsp::Matrix<T>::LUDecomposition()
{
    u32 itr_i, itr_j, itr_k;
    u32 col, row;

    SetSize(length_, length_);

    for(itr_i=0; itr_i<length_; itr_i++)
    {
        for(itr_j=0; itr_j<itr_i+1; itr_j++)
        {
            // Lower Matrix Calculation:
            row = itr_i;
            col = itr_j;
            for(itr_k=0; itr_k<col; itr_k++){
                data_[row][col] -= data_[row][itr_k] * data_[itr_k][col];
            }

            // Upper Matrix Calculation:
            if(row==col){ continue; }
            row = itr_j;
            col = itr_i;
            for(itr_k=0; itr_k<row; itr_k++){
                data_[row][col] -= data_[row][itr_k] * data_[itr_k][col];
            }
            data_[row][col] /= data_[row][row];
        }
    }
}
template<typename T> void wsp::Matrix<T>::LUElimination(const wsp::Matrix<T>& L, const wsp::Matrix<T>& U){
    if(column_size_-length_!=1){ return; }

    wsp::Matrix<T> mat(*this), Ld, Ud;
    u32 col_res = column_size_-1;

    Ld = L;
    Ud = U;

    mat.SplitVertical(col_res, col_res);
    Ld.MergeRight(mat);
    Ld.ForwardSubstitution();
    //cout<<"fw\n"<<Ld<<endl;

    Ld.SplitVertical(col_res, col_res);
    Ud.MergeRight(Ld);
    Ud.backSubstitution();
    //cout<<"back\n"<<Ud<<endl;

    Copy(Ud);
}
template<typename T> void wsp::Matrix<T>::LUElimination(){
    if(column_size_-length_!=1){ return; }

    wsp::Matrix<T> mat(*this), L, U;
    u32 col_res = column_size_-1;

    mat.LUDecomposition(L, U);
    //cout<<"L\n"<<L<<"U\n"<<U<<endl;

    mat.SplitVertical(col_res, col_res);
    L.MergeRight(mat);
    L.ForwardSubstitution();
    //cout<<"fw\n"<<L<<endl;

    L.SplitVertical(col_res, col_res);
    U.MergeRight(L);
    U.backSubstitution();
    //cout<<"back\n"<<U<<endl;

    Copy(U);
}
template<typename T> void wsp::Matrix<T>::LUElimination(const wsp::Matrix<T>& LU){
    if(column_size_-length_!=1 || LU.row_size()!= length_ || LU.row_size()!=LU.column_size()){ return; }

    wsp::Matrix<T> mat(*this);
    u32 col_result = column_size_-1;
    mat.SplitVertical(col_result, col_result);

    Copy(LU);

    MergeRight(mat);
    //cout<<"merge\n"<<this;
    ForwardSubstitution();
    //cout<<"fw\n"<<this;
    backSubstitution();
    //cout<<"back\n"<<this;
}


// end Elimination Methods -------------------------------------------------

// Integration Methods -----------------------------------------------------

template<typename T> void wsp::Matrix<T>::RungeKutta4th(u32 destIndex, const wsp::Vector<T>& initValues, void(*dfunc)(T t, const T* y, T* dest))
{
    T *y, *ydumb, *dy_dt, *k1, *k2, *k3, *k4;
    T t, h;
    u32 itr_i, itr_j, dataLength, numOfResult, memsize;

    numOfResult = initValues.length();

    if(numOfResult==0 || destIndex+numOfResult > length_){
        cerr<<"Error in RungeKutta4th: too few length to put the results."<<endl;
        return;
    }

    // allocating memory
    memsize = numOfResult + RESERVE_MEM;
    y = WSP_NEW T[memsize];
    ydumb = WSP_NEW T[memsize];
    dy_dt = WSP_NEW T[memsize];
    k1 = WSP_NEW T[memsize];
    k2 = WSP_NEW T[memsize];
    k3 = WSP_NEW T[memsize];
    k4 = WSP_NEW T[memsize];

    // initializing
    h = data_[destIndex].GetStep();
    t = data_[destIndex].GetStart();
    dataLength = data_[destIndex].length();
    for(itr_j=0; itr_j<numOfResult; itr_j++){
        y[itr_j] = initValues[itr_j];
        data_[destIndex+itr_j][0] = y[itr_j];
        dy_dt[itr_j] = y[itr_j];
    }

    // main
    for(itr_i=1; itr_i<dataLength; itr_i++)
    {
        dfunc(t, y, dy_dt);
        for(itr_j=0; itr_j<numOfResult; itr_j++){
            k1[itr_j] = h * dy_dt[itr_j];
            ydumb[itr_j] = y[itr_j] + k1[itr_j]/2.0;
        }
        dfunc(t + h/2.0, ydumb, dy_dt);
        for(itr_j=0; itr_j<numOfResult; itr_j++){
            k2[itr_j] = h * dy_dt[itr_j];
            ydumb[itr_j] = y[itr_j] + k2[itr_j]/2.0;
        }
        dfunc(t + h/2.0, ydumb, dy_dt);
        for(itr_j=0; itr_j<numOfResult; itr_j++){
            k3[itr_j] = h * dy_dt[itr_j];
            ydumb[itr_j] = y[itr_j] + k3[itr_j];
        }
        dfunc(t + h, ydumb, dy_dt);
        for(itr_j=0; itr_j<numOfResult; itr_j++){
            k4[itr_j] = h * dy_dt[itr_j];
            y[itr_j] += (k1[itr_j] + 2*(k2[itr_j] + k3[itr_j]) + k4[itr_j]) / 6.0;
            data_[destIndex+itr_j][itr_i] = y[itr_j];
        }
        t += h;
    }

    // freeing memory
    delete[] y;    delete[] ydumb;    delete[] dy_dt;
    delete[] k1;    delete[] k2;    delete[] k3;    delete[] k4;
}
template<typename T> void wsp::Matrix<T>::RungeKuttaGill(u32 destIndex, const wsp::Vector<T>& initValues, void(*dfunc)(T t, const T* y, T* dest))
{
    T *y, *dy_dt, *k, *q;
    T t, h;
    u32 itr_i, itr_j, dataLength, numOfResult, memsize;

    numOfResult = initValues.length();

    if(numOfResult==0 || destIndex+numOfResult > length_){
        cerr<<"Error in RungeKutta4th: too few length to put the results."<<endl;
        return;
    }

    // allocating memory
    memsize = numOfResult + RESERVE_MEM;
    y = WSP_NEW T[memsize];
    dy_dt = WSP_NEW T[memsize];
    k = WSP_NEW T[memsize];
    q = WSP_NEW T[memsize];

    // initializing
    h = data_[destIndex].GetStep();
    t = data_[destIndex].GetStart();
    dataLength = data_[destIndex].length();
    for(itr_j=0; itr_j<numOfResult; itr_j++){
        y[itr_j] = initValues[itr_j];
        data_[destIndex+itr_j][0] = y[itr_j];
        dy_dt[itr_j] = y[itr_j];
    }

    for(itr_i=1; itr_i<dataLength; itr_i++)
    {
        // step 1
        dfunc(t, y, dy_dt);
        for(itr_j=0; itr_j<numOfResult; itr_j++){
            k[itr_j] = h * dy_dt[itr_j];
            y[itr_j] += k[itr_j]/2.0;
            q[itr_j] = k[itr_j];
        }

        // step 2
        dfunc(t + h/2.0, y, dy_dt);
        for(itr_j=0; itr_j<numOfResult; itr_j++){
            k[itr_j] = h * dy_dt[itr_j];
            y[itr_j] += (1.0-sqrt(0.5))*(k[itr_j]-q[itr_j]);
            q[itr_j] = (2.0-sqrt(2.0))*k[itr_j] + (-2.0+3.0*sqrt(0.5))*q[itr_j];
        }

        // step 3
        dfunc(t + h/2.0, y, dy_dt);
        for(itr_j=0; itr_j<numOfResult; itr_j++){
            k[itr_j] = h * dy_dt[itr_j];
            y[itr_j] += (1.0+sqrt(0.5))*(k[itr_j]-q[itr_j]);
            q[itr_j] = (2.0+sqrt(2.0))*k[itr_j] + (-2.0-3.0*sqrt(0.5))*q[itr_j];
        }

        // step 4
        dfunc(t + h, y, dy_dt);
        for(itr_j=0; itr_j<numOfResult; itr_j++){
            k[itr_j] = h * dy_dt[itr_j];
        }

        // last step
        for(itr_j=0; itr_j<numOfResult; itr_j++){
            y[itr_j] += k[itr_j]/6.0 - q[itr_j]/3.0;
            data_[destIndex+itr_j][itr_i] = y[itr_j];
        }

        t += h;
    }

    // freeing memory
    delete[] y;    delete[] dy_dt;
    delete[] k;    delete[] q;
}

// end Integration Methods -------------------------------------------------

// Principle Component Analysis --------------------------------------------

// calculation of variance-covariance matrix
template<typename T> void wsp::Matrix<T>::GetVarianceCovarianceMat()
{
    u32 cols = column_size(), rows = row_size();
    T *mean = WSP_NEW T[cols];
    for(u32 j=0; j<cols; j++){
        mean[j]=0;
        for(u32 i=0; i<rows; i++){
            mean[j] += data_[i][j];
        }
        mean[j] /= rows;
    }
    GetVarianceCovarianceMat(mean, cols);
    delete[] mean;
}
template<typename T> void wsp::Matrix<T>::GetVarianceCovarianceMat(const wsp::Vector<T> &mean)
{
    GetVarianceCovarianceMat(mean.data(), mean.length());
}
template<typename T> void wsp::Matrix<T>::GetVarianceCovarianceMat(const T *mean, u32 length)
{
    u32 cols=column_size(), rows=row_size();
    u32 i, j;

    if(length != cols){
        return;
    }

    wsp::Matrix<T> vmat(rows, cols); // variant-covariant matrix
    for(j=0; j<cols; j++){
        for(i=0; i<rows; i++){
            vmat[i][j] = data_[i][j]-mean[j];
        }
    }
    
    wsp::Matrix<T> vmatT(vmat);
    vmatT.Transpose();

    wsp::Matrix<T> out_vc = vmatT*vmat;

    for(j=0; j<cols; j++){
        for(i=0; i<cols; i++){
            out_vc[i][j] /= (T)rows;
        }
    }
    *this = out_vc;
}



#ifdef USE_OPENCV
template<typename T> void wsp::Matrix<T>::GetEigen(wsp::Matrix<T> &out_eigenVec, wsp::Function &out_eigenVal) const{
    if(row_size()!=column_size()){ return; }
    int p = (int)column_size();
    CvMat *mat=NULL;
    CopyIntoArrayCvMat(&mat);
    CvMat *Veigenv = cvCreateMat(p, p, CV_32F); //! matrix for eigen vectors
    CvMat *Veigen = cvCreateMat(p, 1, CV_32F); //! matrix for eigen values
    cvmEigenVV (mat, Veigenv, Veigen, 0.00001);

    out_eigenVec = Veigenv;

    out_eigenVal.SetLength(p);
    for(int i=0; i<p; i++){
        out_eigenVal[i] = cvmGet(Veigen, i, 0);
    }

    cvReleaseMat(&Veigenv);
    cvReleaseMat(&Veigen);    
    cvReleaseMat(&mat);
}

template<typename T> void wsp::Matrix<T>::PCA(wsp::Matrix<T> &out_eigenVec, wsp::Matrix<T> &out_eigenVal) const{
    int p = (int)column_size(); // parameter dimension of data
    int count = (int)row_size(); // number of data

    // matrices for variance-covariance matrix (P*P)
    wsp::Matrix<T> mfmVC(*this);
    //CvMat *VC=cvCreateMat(p,p, CV_32F); 
    CvMat *VC=NULL;

    // calculate variance-covariance matrix (arguments are source matrix, num of data, dest matrixÅj
    mfmVC.GetVarianceCovarianceMat();
    mfmVC.CopyIntoArrayCvMat(&VC);
    //GetVarianceCovarianceMat(VC);

    CvMat *Veigenv = cvCreateMat(p, p, CV_32F); // matrix for eigen vectors
    CvMat *Veigen = cvCreateMat(p, 1, CV_32F); // matrix for eigen values

    // calculate eigen values from variance-covariance matrix
    cvmEigenVV (VC, Veigenv, Veigen, 0.00001);

    out_eigenVec = Veigenv;
    out_eigenVal = Veigen;

    cvReleaseMat(&Veigenv);
    cvReleaseMat(&Veigen);
    cvReleaseMat(&VC);
}
template<typename T> void wsp::Matrix<T>::CopyIntoArrayCvMat(CvMat **out_mat) const{
    int c=(int)column_size();
    int r=(int)row_size();
    if(c<=0 || r<=0 )
    { return; }

    if(*out_mat!=NULL){
        cvReleaseMat(out_mat);
    }
    *out_mat = cvCreateMat(r, c, CV_32F);
    for(int itr_r=0; itr_r<r; itr_r++){
        for(int itr_c=0; itr_c<c; itr_c++){
            cvmSet(*out_mat, itr_r, itr_c, data_[itr_r][itr_c]);
        }
    }
}
#endif
// -------------------------------------------------------------------------


// Operators ---------------------------------------------------------------
template<typename T> wsp::Matrix<T>& wsp::Matrix<T>::operator=(const wsp::Matrix<T>& rhs){
    if(this != &rhs){
        wsp::Vector<T>::operator =(rhs);
        column_size_ = rhs.column_size();
        row_size_ = rhs.row_size();
    }
    return *this;
}
template<typename T> bool wsp::Matrix<T>::operator==(const wsp::Matrix<T> &other) const{
    if(other.data()==NULL && data_==NULL){
        return true;
    }
    if(row_size_!=other.row_size() || column_size_!=other.column_size()){
        return false;
    }
    for(u32 itr_i=0; itr_i<length_; itr_i++){
        if(data_[itr_i]!=other.wsp::Vector<T>::GetElem(itr_i)){
            return false;
        }
    }
    return true;
}
template<typename T> T wsp::Matrix<T>::operator()(u32 row, u32 col) const{
    if(row>=length_ || col>=column_size_){ return 0.0; }
    return data_[row*column_size_+col];
}
template<typename T> T wsp::Matrix<T>::operator()(int row, int col) const{
    if(col<0 || row<0){ return data_[0]; }
    return operator()((u32)row, (u32)col);
}
template<typename T> wsp::Matrix<T> wsp::Matrix<T>::operator*(const wsp::Matrix<T>& mult) const{
    wsp::Matrix<T> dest;
    if(column_size_ != mult.row_size()){
        dest.operator=(*this);
        return dest;
    }

    T sum;
    dest.SetSize(row_size_, mult.column_size());

    for(u32 row=0; row<length_; row++)
    for(u32 col=0; col<dest.column_size(); col++){
        sum = 0.0;
        for(u32 itr_i=0; itr_i<column_size_; itr_i++){
            sum += data_[row*column_size_+itr_i] * mult[itr_i*column_size_+col];
        }
        dest.SetElem(sum, row, col);
    }
    return dest;
}
template<typename T> wsp::Matrix<T>& wsp::Matrix<T>::operator*=(const wsp::Matrix<T>& mat){
    operator=(*this)*mat;
    return *this;
}
//#ifdef USE_OPENCV
//template<typename T> wsp::Matrix<T>& wsp::Matrix<T>::operator=(const CvMat *mat){
//    int col=mat->cols, row=mat->rows;
//    if(col<=0 || row<=0){ return *this; }
//    SetSize(row, col);
//    for(int itr_c=0; itr_c<col; itr_c++){
//        for(int itr_r=0; itr_r<row; itr_r++){
//            data_[itr_r][itr_c] = cvmGet(mat, itr_r, itr_c);
//        }
//    }
//    return *this;
//}
//
//
//#endif
//









#endif