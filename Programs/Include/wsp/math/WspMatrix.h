//** WspMatrix.h
//** Author: Junichi Nishikata

#ifndef __WspMatrix_H__
#define __WspMatrix_H__

#include <_WspVector.h>
#include <WspMath/WspFunction.h>

#ifdef USE_OPENCV
#include <load_opencv.h>
#endif



class WSP_DLL_EXPORT WspMatrix: public wsp::Vector<double>
{
protected:
	uint m_column, m_row;

protected:
	void Init();
	WspMatrix multiply(const WspMatrix& mult) const;

public:
	WspMatrix();
	WspMatrix(uint nRow, uint nColumn);
	WspMatrix(const WspMatrix& mat);
	WspMatrix(const double **mat, uint row, uint col);
	WspMatrix(const double *mat, uint row, uint col);
	~WspMatrix();

	//** Getter -------------------------------------------
	inline uint Row() const;
	inline uint Column() const;
	inline double GetElem(uint row, uint col) const;
	inline wsp::State CopyIntoArray(double *ary, uint length) const;
	inline wsp::State CopyInto2dArray(double **mat, uint row, uint col) const;
	bool is_upperTriangular() const;
	bool is_lowerTriangular() const;

	//** Setter --------------------------------------------
	inline void SetElem(double value, uint row, uint col);
	inline wsp::State CopyFromArray(const double *mat, uint row, uint col);
	inline wsp::State CopyFrom2dArray(const double **mat, uint row, uint col);
	wsp::State SetSize(uint nRow, uint nColumn);

	//** Matrix operations
	void SetToIdentity(uint size);
	void Transpose();
	void SwapRow(uint row1, uint row2);
	void SwapColumn(uint col1, uint col2);
	void InvertRow();
	void InvertColumn();
	void SplitVertical(uint col_start, uint col_end);
	void MergeRight(const WspMatrix& mat);
	void SumRow(wsp::Vector<double>& dest);
	void SumRowWeighted(wsp::Vector<double>& dest, const wsp::Vector<double>& weights);
	void DivideByPivot();
	void SortRowByVector(const wsp::Vector<double> &sortSrc, wsp::Order order = wsp::Ascending);
	void ReverseRow();

	// Elimination Methods
	//   Gaussian Elimination
	void gaussianElimination();
	void eliminate(uint rowEliminated, uint rowEliminator);

	void pivotSelection(uint pivotIndex);
	void pivotSelection();
	void forwardSubstitution();
	void backSubstitution();

	//   LU Decomposition
	void LUDecomposition();
	void LUDecomposition(WspMatrix& L, WspMatrix& U) const;
	void LUElimination();
	void LUElimination(const WspMatrix& LU);
	void LUElimination(const WspMatrix& L, const WspMatrix& U);

	// Principle Component Analysis
	void getVarianceCovarianceMat();
	void getVarianceCovarianceMat(const wsp::Vector<double> &mean);
	void getVarianceCovarianceMat(const double *mean, uint length);
	#ifdef USE_OPENCV
	void getEigen(WspMatrix &out_eigenVec, WspFunction &out_eigenVal) const;
	void PCA(WspMatrix &out_eigenVec, WspMatrix &out_eigenVal) const;
	void CopyIntoArrayCvMat(CvMat **out_mat)const;
	#endif

	// Integration Methods
	void RungeKutta4th(uint destIndex, const wsp::Vector<double>& initValues, void(*dfunc)(double t, const double* y, double* dest));
	void RungeKuttaGill(uint destIndex, const wsp::Vector<double>& initValues, void(*dfunc)(double t, const double* y, double* dest));


	// Operators
	WspMatrix& operator=(const WspMatrix& mat);
	bool operator==(const WspMatrix &mat) const;
	double operator()(uint row, uint col) const;
	double operator()(int row, int col) const;
	WspMatrix operator*(const WspMatrix& mat) const;
	WspMatrix& operator*=(const WspMatrix& mat);
	#ifdef USE_OPENCV
	WspMatrix& operator=(const CvMat *mat);
	#endif
};

std::ostream& operator<<(std::ostream& lhs, const WspMatrix &rhs);
std::ostream& operator<<(std::ostream& lhs, const WspMatrix *rhs);


//** Getter ------------------------------------------
inline uint WspMatrix::Column() const{ return m_column; }
inline uint WspMatrix::Row() const{ return m_row; }
inline double WspMatrix::GetElem(uint row, uint col) const{
	return wsp::Vector<double>::GetElem(row*m_column+col);
}
inline wsp::State WspMatrix::CopyIntoArray(double *ary, uint length) const{
	if(length != length_)
	{ WSPERROR("length must be the same as matrix length\n"); return wsp::STATE_FAILURE; }
	for(uint itr_i=0; itr_i<length; itr_i++){
		ary[itr_i] = data_[itr_i];
	}
	return wsp::STATE_SUCCESS;
}
inline wsp::State WspMatrix::CopyInto2dArray(double **ary, uint row, uint col) const{
	if(m_row != row || m_column != col)
	{ WSPERROR("row and column must be the same as matrix length\n"); return wsp::STATE_FAILURE; }
	for(uint itr_i=0, itr_r=0; itr_r<row; itr_r++){
		for(uint itr_c=0; itr_c<col; itr_c++){
			ary[itr_r][itr_c] = data_[itr_i];
			itr_i++;
		}
	}
	return wsp::STATE_SUCCESS;
}

//** Setter ------------------------------------------
inline void WspMatrix::SetElem(double value, uint row, uint col){
	data_[row*m_column + col] = value;
}
inline wsp::State WspMatrix::CopyFromArray(const double *mat, uint row, uint col){
	if(mat==NULL){ WSPERROR("argument pointer is NULL\n"); return wsp::STATE_FAILURE; }
	SetSize(row, col);
	for(uint itr_i=0; itr_i<length_; itr_i++){
		data_[itr_i] = mat[itr_i];
	}
	return wsp::STATE_SUCCESS;
}
inline wsp::State WspMatrix::CopyFrom2dArray(const double **mat, uint row, uint col){
	if(mat==NULL){ WSPERROR("argument pointer is NULL\n"); return wsp::STATE_FAILURE; }
	m_column=col;  length_=row;
	for(uint itr_i=0, itr_r=0; itr_r<row; itr_r++){
		for(uint itr_c=0; itr_c<col; itr_c++){
			data_[itr_i] = mat[itr_r][itr_c];
			itr_i++;
		}
	}
	return wsp::STATE_SUCCESS;
}
#endif