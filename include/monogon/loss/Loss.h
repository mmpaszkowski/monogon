//
// Created by noname on 21.10.22.
//

#ifndef MATH_LOSS_H
#define MATH_LOSS_H

#include "../Array.h"
#include "../Variable.h"

template <typename T> class Loss
{
  public:
    virtual Variable<T> operator()(Variable<Array<T>> y_pred, Variable<Array<T>> y) const = 0;
};

#endif //MATH_LOSS_H
