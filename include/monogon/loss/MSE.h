//
// Created by noname on 21.10.22.
//

#ifndef MATH_MSE_H
#define MATH_MSE_H

#include "../Matrix.h"
#include "../Variable.h"
#include "Loss.h"

template <typename T = double> class MSE : public Loss<T>
{
  public:
    Variable<T> operator()(Variable<Matrix<T>> y_pred, Variable<Matrix<T>> y) const;
};

template <typename T> Variable<T> MSE<T>::operator()(Variable<Matrix<T>> y_pred, Variable<Matrix<T>> y) const
{
    return ((y - y_pred) * (y - y_pred)).avg();
}

#endif //MATH_MSE_H
