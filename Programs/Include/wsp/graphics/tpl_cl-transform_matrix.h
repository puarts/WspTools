/**
 * @file tpl_cl-transform_matrix.h
 * 
 */

#pragma once

#include <wsp/common/fn-debug.h>
#include <wsp/common/tpl_cl-vector3.h>
#include <wsp/common/tpl_cl-vector4.h>
#include "_define_graphic.h"
#include <wsp/math/tpl_cl-matrix_n.h>
#include <wsp/math/_tpl_fn-math.h>

#include <iostream>
#include <iomanip>

namespace wsp {
    namespace graphic {
        template<typename _Type = double>
        class WSP_DLL_EXPORT TransformMatrix
            : public wsp::MatrixN < _Type, 4, 4 >
        {
        public:
            // Constructors ----------------------------------------
            inline TransformMatrix();
            explicit inline TransformMatrix(const _Type mat[16]);
            inline TransformMatrix(
                const wsp::Vector3<_Type> &ax,
                const wsp::Vector3<_Type> &ay,
                const wsp::Vector3<_Type> &az,
                const wsp::Vector3<_Type> &pos
                );
            inline TransformMatrix(
                _Type e00, _Type e01, _Type e02, _Type e03,
                _Type e10, _Type e11, _Type e12, _Type e13,
                _Type e20, _Type e21, _Type e22, _Type e23,
                _Type e30, _Type e31, _Type e32, _Type e33
                );
            inline TransformMatrix(const wsp::graphic::TransformMatrix<_Type> &mat);
            inline TransformMatrix(const wsp::MatrixN<_Type, 4, 4> &mat);

            // Accessors -------------------------------------------
            _Type GetData(int row, int column) const
            {
                WSP_ASSERT(row < 4, "row must be less then 4: row = %d", row);
                WSP_ASSERT(column < 4, "column must be less then 4: column = %d", column);
                return wsp::VectorN<_Type, 16>::data_[row * 4 + column];
            }

            void GetDataAsMatrix3x3(_Type* o_mat3x3) const
            {
                for (int i = 0; i < 3; ++i)
                {
                    for (int j = 0; j < 3; ++j)
                    {
                        o_mat3x3[i * 3 + j] = GetData(i, j);
                    }
                }
            }

            /** @brief  Compute and return the inverse of this instance. */
            bool GetInverseMatrix(wsp::graphic::TransformMatrix<_Type>* o_inverse_mat) const;

            /** @brief  Compute and return the adjoint of this instance. */
            wsp::graphic::TransformMatrix<_Type> Adjoint() const;

            /** @brief  Compute and return a homogenized version of this instance. */
            wsp::graphic::TransformMatrix<_Type> Homogenize() const;

            /** @brief  Compute and return the determinant of this instance. */
            _Type Det4x4() const;

            /** @brief  Compute and return the determinant of the upper left 3x3 submatrix of this instance. */
            _Type Det3x3() const;


            // Mutators --------------------------------------------
            void SetToProduct(wsp::graphic::TransformMatrix<_Type> &left, wsp::graphic::TransformMatrix<_Type> &right);

            inline wsp::graphic::TransformMatrix<_Type>& operator=(const wsp::MatrixN<_Type, 4, 4> &rhs)
            {
                //wsp::VectorN<T, N_ROW*N_COLUMN>::operator=(rhs);
                memcpy(wsp::VectorN<_Type, 4 * 4>::data_, rhs.data(), sizeof(_Type) * 16);
                return *this;
            }
        };

        template class TransformMatrix<float>;

        typedef TransformMatrix<float> TransformMatrixF;
        typedef TransformMatrix<float> Mat44F;
    }
}

template<typename _Type>
wsp::graphic::TransformMatrix<_Type>::TransformMatrix()
    : wsp::MatrixN<_Type, 4, 4>()
{
    wsp::VectorN<_Type, 4 * 4>::data_[0] = wsp::VectorN<_Type, 16>::data_[5] = wsp::VectorN<_Type, 16>::data_[10] = wsp::VectorN<_Type, 16>::data_[15] = 1.0;
}

template<typename _Type>
wsp::graphic::TransformMatrix<_Type>::TransformMatrix(const _Type mat[16])
    : wsp::MatrixN<_Type, 4, 4>(mat)
{
}

template<typename _Type>
wsp::graphic::TransformMatrix<_Type>::TransformMatrix(
    const wsp::Vector3<_Type> &ax,
    const wsp::Vector3<_Type> &ay,
    const wsp::Vector3<_Type> &az,
    const wsp::Vector3<_Type> &pos
    )
{
    wsp::VectorN<_Type, 16>::data_[0] = ax[0]; wsp::VectorN<_Type, 16>::data_[1] = ax[1]; wsp::VectorN<_Type, 16>::data_[2] = ax[2]; wsp::VectorN<_Type, 16>::data_[3] = pos[0];
    wsp::VectorN<_Type, 16>::data_[4] = ay[0]; wsp::VectorN<_Type, 16>::data_[5] = ay[1]; wsp::VectorN<_Type, 16>::data_[6] = ay[2]; wsp::VectorN<_Type, 16>::data_[7] = pos[1];
    wsp::VectorN<_Type, 16>::data_[8] = az[0]; wsp::VectorN<_Type, 16>::data_[9] = az[1]; wsp::VectorN<_Type, 16>::data_[10] = az[2]; wsp::VectorN<_Type, 16>::data_[11] = pos[2];
    wsp::VectorN<_Type, 16>::data_[12] = 0; wsp::VectorN<_Type, 16>::data_[13] = 0; wsp::VectorN<_Type, 16>::data_[14] = 0; wsp::VectorN<_Type, 16>::data_[15] = 1;
}

template<typename _Type>
wsp::graphic::TransformMatrix<_Type>::TransformMatrix(
    _Type e00, _Type e01, _Type e02, _Type e03,
    _Type e10, _Type e11, _Type e12, _Type e13,
    _Type e20, _Type e21, _Type e22, _Type e23,
    _Type e30, _Type e31, _Type e32, _Type e33
    )
{
    wsp::VectorN<_Type, 16>::data_[0] = e00; wsp::VectorN<_Type, 16>::data_[1] = e01; wsp::VectorN<_Type, 16>::data_[2] = e02; wsp::VectorN<_Type, 16>::data_[3] = e03;
    wsp::VectorN<_Type, 16>::data_[4] = e10; wsp::VectorN<_Type, 16>::data_[5] = e11; wsp::VectorN<_Type, 16>::data_[6] = e12; wsp::VectorN<_Type, 16>::data_[7] = e13;
    wsp::VectorN<_Type, 16>::data_[8] = e20; wsp::VectorN<_Type, 16>::data_[9] = e21; wsp::VectorN<_Type, 16>::data_[10] = e22; wsp::VectorN<_Type, 16>::data_[11] = e23;
    wsp::VectorN<_Type, 16>::data_[12] = e30; wsp::VectorN<_Type, 16>::data_[13] = e31; wsp::VectorN<_Type, 16>::data_[14] = e32; wsp::VectorN<_Type, 16>::data_[15] = e33;
}

template<typename _Type>
wsp::graphic::TransformMatrix<_Type>::TransformMatrix(const wsp::graphic::TransformMatrix<_Type> &mat)
    : wsp::MatrixN<_Type, 4, 4>(mat)
{
}

template<typename _Type>
wsp::graphic::TransformMatrix<_Type>::TransformMatrix(const wsp::MatrixN<_Type, 4, 4> &mat)
    : wsp::MatrixN<_Type, 4, 4>(mat)
{
}

// I/O ----------------------
template<typename _Type>
std::ostream& operator<<(std::ostream& lhs, const wsp::graphic::TransformMatrix<_Type> &rhs)
{
    for(int itr_r=0; itr_r<4; ++itr_r)
    {
        for(int itr_c=0; itr_c<4; ++itr_c)
        {
            lhs<<std::setw(PRINT_COLW)<<std::setprecision(PRINT_PREC);
            lhs<<PRINT_TYPE;
            lhs<<rhs[itr_r*4+itr_c];
        }
        lhs<<std::endl;
    }
    return lhs;
}

template<typename _Type>
std::ostream& operator<<(std::ostream& lhs, const wsp::graphic::TransformMatrix<_Type> *rhs)
{
    lhs<<*rhs;
    return lhs;
}

template<typename _Type>
bool wsp::graphic::TransformMatrix<_Type>::GetInverseMatrix(wsp::graphic::TransformMatrix<_Type>* o_inverse_mat) const
{
    return wsp::math::GetInvertMatrix4x4<_Type>(data_, o_inverse_mat->data());
}

template<typename _Type>
wsp::graphic::TransformMatrix<_Type> wsp::graphic::TransformMatrix<_Type>::Adjoint() const
{
    WSP_FATAL_NOT_IMPLEMENTED;
}

template<typename _Type>
wsp::graphic::TransformMatrix<_Type> wsp::graphic::TransformMatrix<_Type>::Homogenize() const
{
    WSP_FATAL_NOT_IMPLEMENTED;
}

template<typename _Type>
_Type wsp::graphic::TransformMatrix<_Type>::Det4x4() const
{
    WSP_FATAL_NOT_IMPLEMENTED;
}

template<typename _Type>
_Type wsp::graphic::TransformMatrix<_Type>::Det3x3() const
{
    WSP_FATAL_NOT_IMPLEMENTED;
}

template<typename _Type>
void wsp::graphic::TransformMatrix<_Type>::SetToProduct(wsp::graphic::TransformMatrix<_Type> &left, wsp::graphic::TransformMatrix<_Type> &right)
{
    wsp::VectorN<_Type, 16>::data_[0] = left[0] * right[0] + left[1] * right[4] + left[2] * right[8] + left[3] * right[12];
    wsp::VectorN<_Type, 16>::data_[1] = left[0] * right[1] + left[1] * right[5] + left[2] * right[9] + left[3] * right[13];
    wsp::VectorN<_Type, 16>::data_[2] = left[0] * right[2] + left[1] * right[6] + left[2] * right[10] + left[3] * right[14];
    wsp::VectorN<_Type, 16>::data_[3] = left[0] * right[3] + left[1] * right[7] + left[2] * right[11] + left[3] * right[15];

    wsp::VectorN<_Type, 16>::data_[4] = left[4] * right[0] + left[5] * right[4] + left[6] * right[8] +  left[7] * right[12];
    wsp::VectorN<_Type, 16>::data_[5] = left[4] * right[1] + left[5] * right[5] + left[6] * right[9] +  left[7] * right[13];
    wsp::VectorN<_Type, 16>::data_[6] = left[4] * right[2] + left[5] * right[6] + left[6] * right[10] + left[7] * right[14];
    wsp::VectorN<_Type, 16>::data_[7] = left[4] * right[3] + left[5] * right[7] + left[6] * right[11] + left[7] * right[15];

    wsp::VectorN<_Type, 16>::data_[8]  = left[8] * right[0] + left[9] * right[4] + left[10] * right[8] +  left[11] * right[12];
    wsp::VectorN<_Type, 16>::data_[9]  = left[8] * right[1] + left[9] * right[5] + left[10] * right[9] +  left[11] * right[13];
    wsp::VectorN<_Type, 16>::data_[10] = left[8] * right[2] + left[9] * right[6] + left[10] * right[10] + left[11] * right[14];
    wsp::VectorN<_Type, 16>::data_[11] = left[8] * right[3] + left[9] * right[7] + left[10] * right[11] + left[11] * right[15];

    wsp::VectorN<_Type, 16>::data_[12] = left[12] * right[0] + left[13] * right[4] + left[14] * right[8] +  left[15] * right[12];
    wsp::VectorN<_Type, 16>::data_[13] = left[12] * right[1] + left[13] * right[5] + left[14] * right[9] +  left[15] * right[13];
    wsp::VectorN<_Type, 16>::data_[14] = left[12] * right[2] + left[13] * right[6] + left[14] * right[10] + left[15] * right[14];
    wsp::VectorN<_Type, 16>::data_[15] = left[12] * right[3] + left[13] * right[7] + left[14] * right[11] + left[15] * right[15];
}
