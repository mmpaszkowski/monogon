//
// Created by noname on 21.10.22.
//

#ifndef MATH_LOSS_H
#define MATH_LOSS_H

#include "../Matrix.h"
#include "../Variable.h"

template <typename T> class Loss
{
  public:
    virtual Variable<T> operator()(Variable<Matrix<T>> y_pred, Variable<Matrix<T>> y) const = 0;
};

#endif //MATH_LOSS_H
