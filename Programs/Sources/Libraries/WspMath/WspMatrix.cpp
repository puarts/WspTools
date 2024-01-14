//** wsp::Matrix<T>.cpp
//** Author: Junichi Nishikata


#include <iostream>


#include "WspMatrix.h"

using namespace std;


// Constructor, Destructor -------------------------------------------------
#define INIT_WSP_MATRIX m_column(0), m_row(0)


template<typename T> wsp::Matrix<T>::Matrix()
	:INIT_WSP_MATRIX
{}
template<typename T> wsp::Matrix<T>::Matrix(uint nRow, uint nColumn)
	:INIT_WSP_MATRIX
{
	SetSize(nRow, nColumn);
}
// Copy Constructor
template<typename T> wsp::Matrix<T>::Matrix(const wsp::Matrix<T>& mat)
	:INIT_WSP_MATRIX, wsp::Vector<double>(mat)
{
	m_column = mat.Column();
	m_row = mat.Row();
}
template<typename T> wsp::Matrix<T>::Matrix(const double **mat, uint row, uint col)
	:INIT_WSP_MATRIX
{
	CopyFrom2dArray(mat, row, col);
}
template<typename T> wsp::Matrix<T>::Matrix(const double *mat, uint row, uint col)
	:INIT_WSP_MATRIX
{
	CopyFromArray(mat, row, col);
}
template<typename T> wsp::Matrix<T>::~Matrix(){}

// end Constructor, Destructor ---------------------------------------------

// Setter ------------------------------------------------------------------
template<typename T> wsp::State wsp::Matrix<T>::SetSize(uint nRow, uint nColumn){
	if(nRow==0 || nColumn==0){
		delete[] data_;
		data_ = NULL;
		length_ = 0;
		m_column = 0;
		m_row = 0;
		return wsp::STATE_FAILURE;
	}

	double start, end;
	uint itr_i, itr_j, len_old;
	wsp::Matrix<T> tmp(*this);

	len_old = length_;
	length_ = nRow * nColumn;
	m_column = nColumn;
	m_row = nRow;
	delete[] data_;
	data_ = new double[length_ + RESERVE_MEM];

	start = 0.0;
	end = 1.0;

	for(itr_i=0; itr_i<length_; itr_i++){
		if(itr_i < len_old){
			data_[itr_i] = tmp[itr_i];
			continue;
		}
		data_[itr_i] = 0.0;
	}
	return wsp::STATE_SUCCESS;
}

//** Matrix operations 
template<typename T> void wsp::Matrix<T>::SetToIdentity(uint size){
	if(size==0){ return; }
	SetSize(size, size);
	for(uint itr_i=0; itr_i<size; itr_i++)
	for(uint itr_j=0; itr_j<size; itr_j++){
		if(itr_i!=itr_j){
			SetElem(0.0, itr_i, itr_j);
			continue;
		}
		SetElem(1.0, itr_i, itr_j);
	}
}
template<typename T> void wsp::Matrix<T>::Transpose(){
	if(m_row==0 || m_column==0){ return; }
	wsp::Matrix<T> mat_tmp(*this);

	SetSize(m_column, m_row);
	for(uint itr_r=0; itr_r<m_row; itr_r++)
	for(uint itr_c=0; itr_c<m_column; itr_c++){
		SetElem(mat_tmp.GetElem(itr_c, itr_r), itr_r, itr_c); 
	}
}
template<typename T> void wsp::Matrix<T>::SwapRow(uint row1, uint row2){
	if(row1>=m_row || row2>=m_row || row1==row2){ return;}
	double tmp;
	for(int itr_c=0; itr_c<m_column; itr_c++){
		tmp = GetElem(row1, itr_c);
		SetElem(GetElem(row2, itr_c), row1, itr_c);
		SetElem(tmp, row2, itr_c);
	}
}
template<typename T> void wsp::Matrix<T>::SwapColumn(uint col1, uint col2){
	if(col1>=m_column || col2>=m_column || col1==col2){ return;}
	Transpose();
	SwapRow(col1, col2);
	Transpose();
}
template<typename T> void wsp::Matrix<T>::InvertRow(){
	for(uint itr_i=0; itr_i<length_/2; itr_i++){
		SwapRow(itr_i, length_-itr_i-1);
	}
}
template<typename T> void wsp::Matrix<T>::InvertColumn(){
	for(uint itr_i=0; itr_i<m_column/2; itr_i++){
		SwapColumn(itr_i, m_column-itr_i-1);
	}
}
template<typename T> void wsp::Matrix<T>::SplitVertical(uint col_start, uint col_end){
	int colw = (int)col_end - (int)col_start;
	if(colw < 0 || col_end>=m_column){
		WSPERROR("col_end must be equal to or larger than col_start.\n");
		return;
	}
	wsp::Matrix<T> mat(*this);
	SetSize(m_row, colw+1);
	for(uint itr_r=0; itr_r<length_; itr_r++)
	for(uint itr_c=0; itr_c<m_column; itr_c++){
		SetElem(mat.GetElem(itr_r, itr_c+col_start), itr_r, itr_c);
	}
}
template<typename T> void wsp::Matrix<T>::MergeRight(const wsp::Matrix<T>& mat){
	uint col, col_pre;
	if(m_row != mat.Row()){
		WSPERROR("2 matrices have to have the same number of rows.\n");
		return;
	}
	col_pre = m_column;
	col = col_pre + mat.Column();
	SetSize(m_row, col);
	for(uint itr_i=0; itr_i<length_; itr_i++){
		for(uint itr_j=0; itr_j<mat.Column(); itr_j++){
			SetElem(mat.GetElem(itr_i,itr_j), itr_i, itr_j+col_pre);
		}
	}
}
template<typename T> void wsp::Matrix<T>::SumRowWeighted(wsp::Vector<double>& dest, const wsp::Vector<double>& weights){
	if(weights.length()!=m_column || length_==0){
		WSPERROR("length of weights and destination array are different.\ndest=%d, weights=%d\n",
				  length_, weights.length());
		return;
	}
	dest.SetLength(m_column);
	dest.Reset(0);
	for(uint itr_r=0; itr_r<m_row; itr_r++){
		for(uint itr_c=0; itr_c<m_column; itr_c++){
			dest[itr_c] += GetElem(itr_r,itr_c) * weights[itr_c];
		}
	}
}
template<typename T> void wsp::Matrix<T>::SumRow(wsp::Vector<double>& dest)
{
	if(length_ == 0){
		return;
	}
	wsp::Vector<double> weights(m_column);
	weights.Reset(1.0);
	SumRowWeighted(dest, weights);
}
template<typename T> void wsp::Matrix<T>::DivideByPivot(){
	wsp::Matrix<T> mat(*this);
	double pivot;
	for(uint itr_r=0; itr_r<length_; itr_r++){
		//mat.pivotSelection(itr_i);
		pivot = mat.GetElem(itr_r, itr_r);
		if(pivot != 0){
			for(uint itr_c=0; itr_c<m_row; itr_c++){
				mat[itr_r*m_column+itr_c] /= pivot;
			}
		}
	}
	operator=(mat);
}


template<typename T> void wsp::Matrix<T>::SortRowByVector(const wsp::Vector<double> &in_sortSrc, wsp::Order order){
	wsp::Vector<double> sortSrc(in_sortSrc);
	int beg[MAX_BUFFER], end[MAX_BUFFER], i, L, R;
	double piv;
	double tmpvec[MAX_BUFFER];
	beg[0] = 0;  end[0] = (int)Row();  i = 0;
	while(i>=0)
	{
		L=beg[i];  R=end[i];
		if(L>=R) {
			i--;  continue;
		}
		piv=sortSrc[L];
		wsp::CopyArray<double>(tmpvec, &data_[L*m_column], m_row);
		while (L<R){
			while(sortSrc[R]>=piv && L<R){ R--; }
			if(L<R){
				sortSrc[L] = sortSrc[R];
				wsp::CopyArray<double>(&data_[L*m_column], &data_[R*m_column], m_row);
				L++;
			}
			while(sortSrc[L]<=piv && L<R){ L++; }
			if(L<R){
				sortSrc[R] = sortSrc[L];
				wsp::CopyArray<double>(&data_[R*m_column], &data_[L*m_column], m_row);
				R--;
			}
		}
		sortSrc[L] = piv;
		wsp::CopyArray<double>(&data_[L*m_column], tmpvec, m_row);
		beg[i+1] = L+1;  end[i+1] = end[i];  end[i++] = L-1;
	}
	if(order==wsp::Ascending){
		ReverseRow();
	}
}
template<typename T> void wsp::Matrix<T>::ReverseRow(){
	int halfRow = (int)floor((float)m_row/2);
	for(int itr_r=0; itr_r<halfRow; itr_r++){
		template<typename T> wsp::Matrix<T>::SwapRow(itr_r, m_row-itr_r-1);
	}
}



//** end Setter --------------------------------------------------------------


//** Elimination methods -----------------------------------------------------

//template<typename T> void wsp::Matrix<T>::gaussianElimination(){
//	if(length_>=m_column){ return; }
//	float pivot;
//	pivotSelection();
//	for(uint itr_i=0; itr_i<length_; itr_i++){
//		data_[itr_i] /= data_[itr_i][itr_i];
//		count+=length_;
//		for(uint itr_j=itr_i+1; itr_j<length_; itr_j++){
//			if(itr_i==itr_j){
//				continue;
//			}
//			eliminate(itr_j, itr_i);
//		}
//	}
//	//cout<<*this<<endl;
//	backSubstitution();
//}
//template<typename T> void wsp::Matrix<T>::pivotSelection(uint pivotIndex){
//	double max;
//	uint itr_i, maxIndex;
//	if(pivotIndex>=length_){ return; }
//	max = data_[pivotIndex][pivotIndex];
//	maxIndex = pivotIndex;
//	// finding max
//	for(itr_i=pivotIndex+1; itr_i<length_; itr_i++){
//		if(max < data_[itr_i][pivotIndex]){
//			max = data_[itr_i][pivotIndex];
//			maxIndex = itr_i;
//		}
//	}
//	SwapRow(pivotIndex, maxIndex);
//}
//template<typename T> void wsp::Matrix<T>::pivotSelection(){
//	for(uint itr_i=0; itr_i<length_; itr_i++){
//		pivotSelection(itr_i);
//	}
//}

//template<typename T> void wsp::Matrix<T>::eliminate(uint rowEliminated, uint pivot){
//	double pivotValueL;
//	double sub;
//	if(rowEliminated>=length_ || pivot>=length_){
//		return;
//	}
//
//	pivotValueL = data_[rowEliminated][pivot];
//
//	if(pivotValueL != 0.0){
//		for(uint itr_i=0; itr_i<m_column; itr_i++){
//			sub = data_[pivot][itr_i] * pivotValueL;
//			data_[rowEliminated][itr_i] -= sub;
//			count++;
//			count++;
//		}
//	}
//}
//
//template<typename T> void wsp::Matrix<T>::backSubstitution()
//{
//	uint itr_i, row, col, col_result;
//	if((int)m_column-(int)length_<1 && !is_upperTriangular()){ return; }
//
//	col_result = length_;
//	//cout<<"back: "<<endl<<*this<<endl;
//	for(col=length_-1; col>=0; col--)
//	{
//		if(data_[col][col] == 0){ return; }
//		for(itr_i=col_result; itr_i<(int)m_column; itr_i++){
//			data_[col][itr_i] /= data_[col][col];
//			count++;
//		}
//		data_[col][col] = 1.0; // /= data_[col][col];
//		count++;
//		for(row=col-1; row>=0; row--)
//		{
//			//cout<<row<<", "<<col<<endl;
//			for(itr_i=col_result; itr_i<(int)m_column; itr_i++){
//				data_[row][itr_i] -= data_[col][itr_i] * data_[row][col];
//				count++;
//			}
//			data_[row][col] = 0.0; // -= data_[col][col] * data_[row][col];
//			count++;
//			//cout<<*this<<endl;
//		}
//	}
//}
//
//template<typename T> void wsp::Matrix<T>::forwardSubstitution()
//{
//	if((int)m_column-(int)length_<1 && !is_lowerTriangular()){ return;}
//	uint itr_i, row, col, col_result;
//
//	col_result = m_column-1;
//	//cout<<"forward: "<<endl<<*this<<endl;
//	for(col=0; col<length_; col++)
//	{
//		if(data_[col][col] == 0){ return; }
//		for(itr_i=col_result; itr_i<m_column; itr_i++){
//			data_[col][itr_i] /= data_[col][col];
//			count++;
//		}
//		data_[col][col] = 1.0; // /= data_[col][col];
//		count++;
//		for(row=col+1; row<length_; row++)
//		{
//			//cout<<row<<", "<<col<<endl;
//			for(itr_i=col_result; itr_i<m_column; itr_i++){
//				data_[row][itr_i] -= data_[col][itr_i] * data_[row][col];
//				count++;
//			}
//			data_[row][col] = 0.0; // -= data_[col][col] * data_[row][col];
//			count++;
//			//cout<<*this<<endl;
//		}
//	}
//}
//
//template<typename T> bool wsp::Matrix<T>::is_upperTriangular() const
//{
//	if(length_!=m_column){
//		return false;
//	}
//	for(uint itr_i=0; itr_i<length_; itr_i++)
//	for(uint itr_j=0; itr_j<=itr_i; itr_j++){
//		if(itr_i==itr_j){
//			//if(data_[itr_i][itr_j]!=1){
//			//	return false;
//			//}
//			continue;
//		}
//		if(data_[itr_i][itr_j]!=0){
//			return false;
//		}
//	}
//	return true;
//}
//
//template<typename T> bool wsp::Matrix<T>::is_lowerTriangular() const
//{
//	bool ret;
//	Transpose();
//	ret = is_upperTriangular();
//	Transpose();
//	return ret;
//}
//
//template<typename T> void wsp::Matrix<T>::LUDecomposition(wsp::Matrix<T>& L, wsp::Matrix<T>& U) const
//{
//	wsp::Matrix<T> mat(*this);
//	uint itr_i, itr_j, itr_k;
//	uint col, row;
//
//	// initializing L and U matrix
//	L.SetSize(length_, length_);
//	U.SetSize(length_, length_);
//	L.Reset(0.0);
//	U.Reset(0.0);
//
//	//--------------------------------------------
//	// Lower and Upper matrix will be like below
//	//
//	// U=|  1  u12 u13 |	L=| l11  0   0  |
//	//   |  0   1  u23 |	  | l21 l22  0  |
//	//   |  0   0   1  |	  | l31 l32 l33 |
//	//--------------------------------------------
//	for(itr_i=0; itr_i<length_; itr_i++){
//		for(itr_j=0; itr_j<itr_i+1; itr_j++)
//		{
//			// Lower Matrix Calculation:
//			//	 Lij = Aij - sum(Lik*Ukj, k=[1, j-1])
//			row = itr_i;
//			col = itr_j;
//			L[row][col] = mat[row][col];
//			for(itr_k=0; itr_k<col; itr_k++){
//				L[row][col] -= L[row][itr_k] * U[itr_k][col];
//			}
//			//cout<<"L:"<<row<<" "<<col<<L<<endl;
//
//			// Upper Matrix Calculation:
//			//	 Uij =( Aij - sum(Lik*Ukj, k=[1, i-1]) )/Lii
//			row = itr_j;
//			col = itr_i;
//			U[row][col] = mat[row][col];
//			for(itr_k=0; itr_k<row; itr_k++){
//				U[row][col] -= L[row][itr_k] * U[itr_k][col];
//			}
//			U[row][col] /= L[row][row];
//
//			//cout<<"U:"<<row<<" "<<col<<U<<endl;
//		}
//	}
//}
//
//// put result into myself
//template<typename T> void wsp::Matrix<T>::LUDecomposition()
//{
//	uint itr_i, itr_j, itr_k;
//	uint col, row;
//
//	SetSize(length_, length_);
//
//	for(itr_i=0; itr_i<length_; itr_i++)
//	{
//		for(itr_j=0; itr_j<itr_i+1; itr_j++)
//		{
//			// Lower Matrix Calculation:
//			row = itr_i;
//			col = itr_j;
//			for(itr_k=0; itr_k<col; itr_k++){
//				data_[row][col] -= data_[row][itr_k] * data_[itr_k][col];
//			}
//
//			// Upper Matrix Calculation:
//			if(row==col){ continue; }
//			row = itr_j;
//			col = itr_i;
//			for(itr_k=0; itr_k<row; itr_k++){
//				data_[row][col] -= data_[row][itr_k] * data_[itr_k][col];
//			}
//			data_[row][col] /= data_[row][row];
//		}
//	}
//}
//template<typename T> void wsp::Matrix<T>::LUElimination(const wsp::Matrix<T>& L, const wsp::Matrix<T>& U){
//	if(m_column-length_!=1){ return; }
//
//	wsp::Matrix<T> mat(*this), Ld, Ud;
//	uint col_res = m_column-1;
//
//	Ld = L;
//	Ud = U;
//
//	mat.SplitVertical(col_res, col_res);
//	Ld.MergeRight(mat);
//	Ld.forwardSubstitution();
//	//cout<<"fw\n"<<Ld<<endl;
//
//	Ld.SplitVertical(col_res, col_res);
//	Ud.MergeRight(Ld);
//	Ud.backSubstitution();
//	//cout<<"back\n"<<Ud<<endl;
//
//	Copy(Ud);
//}
//template<typename T> void wsp::Matrix<T>::LUElimination(){
//	if(m_column-length_!=1){ return; }
//
//	wsp::Matrix<T> mat(*this), L, U;
//	uint col_res = m_column-1;
//
//	mat.LUDecomposition(L, U);
//	//cout<<"L\n"<<L<<"U\n"<<U<<endl;
//
//	mat.SplitVertical(col_res, col_res);
//	L.MergeRight(mat);
//	L.forwardSubstitution();
//	//cout<<"fw\n"<<L<<endl;
//
//	L.SplitVertical(col_res, col_res);
//	U.MergeRight(L);
//	U.backSubstitution();
//	//cout<<"back\n"<<U<<endl;
//
//	Copy(U);
//}
//template<typename T> void wsp::Matrix<T>::LUElimination(const wsp::Matrix<T>& LU){
//	if(m_column-length_!=1 || LU.Row()!= length_ || LU.Row()!=LU.Column()){ return; }
//
//	wsp::Matrix<T> mat(*this);
//	uint col_result = m_column-1;
//	mat.SplitVertical(col_result, col_result);
//
//	Copy(LU);
//
//	MergeRight(mat);
//	//cout<<"merge\n"<<this;
//	forwardSubstitution();
//	//cout<<"fw\n"<<this;
//	backSubstitution();
//	//cout<<"back\n"<<this;
//}
//
//
//// end Elimination Methods -------------------------------------------------
//
//// Integration Methods -----------------------------------------------------
//
//template<typename T> void wsp::Matrix<T>::RungeKutta4th(uint destIndex, const wsp::Vector<double>& initValues, void(*dfunc)(double t, const double* y, double* dest))
//{
//	double *y, *ydumb, *dy_dt, *k1, *k2, *k3, *k4;
//	double t, h;
//	uint itr_i, itr_j, dataLength, numOfResult, memsize;
//
//	numOfResult = initValues.length();
//
//	if(numOfResult==0 || destIndex+numOfResult > length_){
//		cerr<<"Error in RungeKutta4th: too few length to put the results."<<endl;
//		return;
//	}
//
//	// allocating memory
//	memsize = numOfResult + RESERVE_MEM;
//	y = new double[memsize];
//	ydumb = new double[memsize];
//	dy_dt = new double[memsize];
//	k1 = new double[memsize];
//	k2 = new double[memsize];
//	k3 = new double[memsize];
//	k4 = new double[memsize];
//
//	// initializing
//	h = data_[destIndex].getStep();
//	t = data_[destIndex].getStart();
//	dataLength = data_[destIndex].length();
//	for(itr_j=0; itr_j<numOfResult; itr_j++){
//		y[itr_j] = initValues[itr_j];
//		data_[destIndex+itr_j][0] = y[itr_j];
//		dy_dt[itr_j] = y[itr_j];
//	}
//
//	// main
//	for(itr_i=1; itr_i<dataLength; itr_i++)
//	{
//		dfunc(t, y, dy_dt);
//		for(itr_j=0; itr_j<numOfResult; itr_j++){
//			k1[itr_j] = h * dy_dt[itr_j];
//			ydumb[itr_j] = y[itr_j] + k1[itr_j]/2.0;
//		}
//		dfunc(t + h/2.0, ydumb, dy_dt);
//		for(itr_j=0; itr_j<numOfResult; itr_j++){
//			k2[itr_j] = h * dy_dt[itr_j];
//			ydumb[itr_j] = y[itr_j] + k2[itr_j]/2.0;
//		}
//		dfunc(t + h/2.0, ydumb, dy_dt);
//		for(itr_j=0; itr_j<numOfResult; itr_j++){
//			k3[itr_j] = h * dy_dt[itr_j];
//			ydumb[itr_j] = y[itr_j] + k3[itr_j];
//		}
//		dfunc(t + h, ydumb, dy_dt);
//		for(itr_j=0; itr_j<numOfResult; itr_j++){
//			k4[itr_j] = h * dy_dt[itr_j];
//			y[itr_j] += (k1[itr_j] + 2*(k2[itr_j] + k3[itr_j]) + k4[itr_j]) / 6.0;
//			data_[destIndex+itr_j][itr_i] = y[itr_j];
//		}
//		t += h;
//	}
//
//	// freeing memory
//	delete[] y;	delete[] ydumb;	delete[] dy_dt;
//	delete[] k1;	delete[] k2;	delete[] k3;	delete[] k4;
//}
//template<typename T> void wsp::Matrix<T>::RungeKuttaGill(uint destIndex, const wsp::Vector<double>& initValues, void(*dfunc)(double t, const double* y, double* dest))
//{
//	double *y, *dy_dt, *k, *q;
//	double t, h;
//	uint itr_i, itr_j, dataLength, numOfResult, memsize;
//
//	numOfResult = initValues.length();
//
//	if(numOfResult==0 || destIndex+numOfResult > length_){
//		cerr<<"Error in RungeKutta4th: too few length to put the results."<<endl;
//		return;
//	}
//
//	// allocating memory
//	memsize = numOfResult + RESERVE_MEM;
//	y = new double[memsize];
//	dy_dt = new double[memsize];
//	k = new double[memsize];
//	q = new double[memsize];
//
//	// initializing
//	h = data_[destIndex].getStep();
//	t = data_[destIndex].getStart();
//	dataLength = data_[destIndex].length();
//	for(itr_j=0; itr_j<numOfResult; itr_j++){
//		y[itr_j] = initValues[itr_j];
//		data_[destIndex+itr_j][0] = y[itr_j];
//		dy_dt[itr_j] = y[itr_j];
//	}
//
//	for(itr_i=1; itr_i<dataLength; itr_i++)
//	{
//		// step 1
//		dfunc(t, y, dy_dt);
//		for(itr_j=0; itr_j<numOfResult; itr_j++){
//			k[itr_j] = h * dy_dt[itr_j];
//			y[itr_j] += k[itr_j]/2.0;
//			q[itr_j] = k[itr_j];
//		}
//
//		// step 2
//		dfunc(t + h/2.0, y, dy_dt);
//		for(itr_j=0; itr_j<numOfResult; itr_j++){
//			k[itr_j] = h * dy_dt[itr_j];
//			y[itr_j] += (1.0-sqrt(0.5))*(k[itr_j]-q[itr_j]);
//			q[itr_j] = (2.0-sqrt(2.0))*k[itr_j] + (-2.0+3.0*sqrt(0.5))*q[itr_j];
//		}
//
//		// step 3
//		dfunc(t + h/2.0, y, dy_dt);
//		for(itr_j=0; itr_j<numOfResult; itr_j++){
//			k[itr_j] = h * dy_dt[itr_j];
//			y[itr_j] += (1.0+sqrt(0.5))*(k[itr_j]-q[itr_j]);
//			q[itr_j] = (2.0+sqrt(2.0))*k[itr_j] + (-2.0-3.0*sqrt(0.5))*q[itr_j];
//		}
//
//		// step 4
//		dfunc(t + h, y, dy_dt);
//		for(itr_j=0; itr_j<numOfResult; itr_j++){
//			k[itr_j] = h * dy_dt[itr_j];
//		}
//
//		// last step
//		for(itr_j=0; itr_j<numOfResult; itr_j++){
//			y[itr_j] += k[itr_j]/6.0 - q[itr_j]/3.0;
//			data_[destIndex+itr_j][itr_i] = y[itr_j];
//		}
//
//		t += h;
//	}
//
//	// freeing memory
//	delete[] y;	delete[] dy_dt;
//	delete[] k;	delete[] q;
//}
//
//// end Integration Methods -------------------------------------------------
//
//// Principle Component Analysis --------------------------------------------
//
//// calculation of variance-covariance matrix
//template<typename T> void wsp::Matrix<T>::getVarianceCovarianceMat()
//{
//	uint cols = Column(), rows = Row();
//	double *mean = new double[cols];
//	for(uint j=0; j<cols; j++){
//		mean[j]=0;
//		for(uint i=0; i<rows; i++){
//			mean[j] += data_[i][j];
//		}
//		mean[j] /= rows;
//	}
//	getVarianceCovarianceMat(mean, cols);
//	delete[] mean;
//}
//template<typename T> void wsp::Matrix<T>::getVarianceCovarianceMat(const WspFunction &mean)
//{
//	getVarianceCovarianceMat(mean.data(), mean.length());
//}
//template<typename T> void wsp::Matrix<T>::getVarianceCovarianceMat(const double *mean, uint length)
//{
//	uint cols=Column(), rows=Row();
//	uint i, j;
//
//	if(length != cols){
//		return;
//	}
//
//	wsp::Matrix<T> vmat(rows, cols); // variant-covariant matrix
//	for(j=0; j<cols; j++){
//		for(i=0; i<rows; i++){
//			vmat[i][j] = data_[i][j]-mean[j];
//		}
//	}
//	
//	wsp::Matrix<T> vmatT(vmat);
//	vmatT.Transpose();
//
//	wsp::Matrix<T> out_vc = vmatT*vmat;
//
//	for(j=0; j<cols; j++){
//		for(i=0; i<cols; i++){
//			out_vc[i][j] /= (double)rows;
//		}
//	}
//	*this = out_vc;
//}
//
//
//
//#ifdef USE_OPENCV
//template<typename T> void wsp::Matrix<T>::getEigen(wsp::Matrix<T> &out_eigenVec, WspFunction &out_eigenVal) const{
//	if(Row()!=Column()){ return; }
//	int p = (int)Column();
//	CvMat *mat=NULL;
//	CopyIntoArrayCvMat(&mat);
//	CvMat *Veigenv = cvCreateMat(p, p, CV_32F); //** matrix for eigen vectors
//	CvMat *Veigen = cvCreateMat(p, 1, CV_32F); //** matrix for eigen values
//	cvmEigenVV (mat, Veigenv, Veigen, 0.00001);
//
//	out_eigenVec = Veigenv;
//
//	out_eigenVal.SetLength(p);
//	for(int i=0; i<p; i++){
//		out_eigenVal[i] = cvmGet(Veigen, i, 0);
//	}
//
//	cvReleaseMat(&Veigenv);
//	cvReleaseMat(&Veigen);	
//	cvReleaseMat(&mat);
//}
//
//template<typename T> void wsp::Matrix<T>::PCA(wsp::Matrix<T> &out_eigenVec, wsp::Matrix<T> &out_eigenVal) const{
//	int p = (int)Column(); // parameter dimension of data
//	int count = (int)Row(); // number of data
//
//	// matrices for variance-covariance matrix (P*P)
//	wsp::Matrix<T> mfmVC(*this);
//	//CvMat *VC=cvCreateMat(p,p, CV_32F); 
//	CvMat *VC=NULL;
//
//	// calculate variance-covariance matrix (arguments are source matrix, num of data, dest matrixÅj
//	mfmVC.getVarianceCovarianceMat();
//	mfmVC.CopyIntoArrayCvMat(&VC);
//	//getVarianceCovarianceMat(VC);
//
//	CvMat *Veigenv = cvCreateMat(p, p, CV_32F); // matrix for eigen vectors
//	CvMat *Veigen = cvCreateMat(p, 1, CV_32F); // matrix for eigen values
//
//	// calculate eigen values from variance-covariance matrix
//	cvmEigenVV (VC, Veigenv, Veigen, 0.00001);
//
//	out_eigenVec = Veigenv;
//	out_eigenVal = Veigen;
//
//	cvReleaseMat(&Veigenv);
//	cvReleaseMat(&Veigen);
//	cvReleaseMat(&VC);
//}
//template<typename T> void wsp::Matrix<T>::CopyIntoArrayCvMat(CvMat **out_mat) const{
//	int c=(int)Column();
//	int r=(int)Row();
//	if(c<=0 || r<=0 )
//	{ return; }
//
//	if(*out_mat!=NULL){
//		cvReleaseMat(out_mat);
//	}
//	*out_mat = cvCreateMat(r, c, CV_32F);
//	for(int itr_r=0; itr_r<r; itr_r++){
//		for(int itr_c=0; itr_c<c; itr_c++){
//			cvmSet(*out_mat, itr_r, itr_c, data_[itr_r][itr_c]);
//		}
//	}
//}
//#endif
//// -------------------------------------------------------------------------
//
//
// Operators ---------------------------------------------------------------
template<typename T> wsp::Matrix<T>& wsp::Matrix<T>::operator=(const wsp::Matrix<T>& rhs){
	if(this != &rhs){
		wsp::Vector<double>::operator =(rhs);
		m_column = rhs.Column();
		m_row = rhs.Row();
	}
	return *this;
}
template<typename T> bool wsp::Matrix<T>::operator==(const wsp::Matrix<T> &other) const{
	if(other.data()==NULL && data_==NULL){
		return true;
	}
	if(m_row!=other.Row() || m_column!=other.Column()){
		return false;
	}
	for(uint itr_i=0; itr_i<length_; itr_i++){
		if(data_[itr_i]!=other.wsp::Vector<double>::GetElem(itr_i)){
			return false;
		}
	}
	return true;
}
template<typename T> T wsp::Matrix<T>::operator()(uint row, uint col) const{
	if(row>=length_ || col>=m_column){ return 0.0; }
	return data_[row*m_column+col];
}
template<typename T> T wsp::Matrix<T>::operator()(int row, int col) const{
	if(col<0 || row<0){ return data_[0]; }
	return operator()((uint)row, (uint)col);
}
template<typename T> wsp::Matrix<T> wsp::Matrix<T>::operator*(const wsp::Matrix<T>& mult) const{
	wsp::Matrix<T> dest;
	if(m_column != mult.Row()){
		dest.operator=(*this);
		return dest;
	}

	double sum;
	dest.SetSize(m_row, mult.Column());

	for(uint row=0; row<length_; row++)
	for(uint col=0; col<dest.Column(); col++){
		sum = 0.0;
		for(uint itr_i=0; itr_i<m_column; itr_i++){
			sum += data_[row*m_column+itr_i] * mult[itr_i*m_column+col];
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
//	int col=mat->cols, row=mat->rows;
//	if(col<=0 || row<=0){ return *this; }
//	SetSize(row, col);
//	for(int itr_c=0; itr_c<col; itr_c++){
//		for(int itr_r=0; itr_r<row; itr_r++){
//			data_[itr_r][itr_c] = cvmGet(mat, itr_r, itr_c);
//		}
//	}
//	return *this;
//}
//
//
//#endif
//





ostream& operator<<(ostream& lhs, const wsp::Matrix<T> &rhs)
{
	for(uint itr_r=0; itr_r<rhs.Row(); itr_r++){
		for(uint itr_c=0; itr_c<rhs.Column(); itr_c++){
			lhs<<setw(PRINT_COLW)<<setprecision(PRINT_PREC);
			lhs<<PRINT_TYPE;
			lhs<<rhs.GetElem(itr_r, itr_c);
		}
		lhs<<endl;
	}
	return lhs;
}
ostream& operator<<(ostream& lhs, const wsp::Matrix<T> *rhs)
{
	lhs<<*rhs;
	return lhs;
}
