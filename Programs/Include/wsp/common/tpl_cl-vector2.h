#pragma once

#include "tpl_cl-vector_n.h"

namespace wsp{
    template<class T=double>
    class Vector2
        :public VectorN<T, 2>
    {
    public:
        inline Vector2()
            : wsp::VectorN<T, 2>()
        {}

        template<typename U>
        inline Vector2(U in_x, U in_y)
        {
            wsp::VectorN<T, 2>::data_[0]=static_cast<T>(in_x); 
            wsp::VectorN<T, 2>::data_[1]=static_cast<T>(in_y); 
        }

        inline Vector2(const Vector2<T>& data);

        inline explicit Vector2(const VectorN<T, 2>& data)
            : wsp::VectorN<T, 2>(data)
        {}

        T x() const
        {
            return wsp::VectorN<T, 2>::data_[0];
        }

        T y() const
        {
            return wsp::VectorN<T, 2>::data_[1];
        }

        void set_x(T x)
        {
            wsp::VectorN<T, 2>::data_[0] = x;
        }

        void set_y(T y)
        {
            wsp::VectorN<T, 2>::data_[1] = y;
        }

        // Operators --------------------
        inline Vector2<T>& operator=(const VectorN<T, 2>& rhs)
        {
            memcpy(wsp::VectorN<T, 2>::data_, rhs.data(), sizeof(T) * 2);
            return *this;
        }
    };
}

template<class T>
inline wsp::Vector2<T>::Vector2(const wsp::Vector2<T> &data)
{
    this->operator=(data);
}
