//
// Created by noname on 21.10.22.
//

#ifndef MATH_OPTIMIZER_H
#define MATH_OPTIMIZER_H

#include "../Variable.h"
template <typename T> class Optimizer
{
  public:
    virtual void minimize(Variable<Matrix<T>>& weights) const = 0;
};

#endif //MATH_OPTIMIZER_H
