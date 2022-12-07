//
// Created by noname on 21.10.22.
//

#ifndef MATH_SGD_H
#define MATH_SGD_H

#include "../Matrix.h"
#include "../Variable.h"
#include "Optimizer.h"
#include <iostream>

template <typename T = double> class SGD : public Optimizer<T>
{
  public:
    SGD(T learning_rate = 0.01, T momentum = 0.0, bool nesterov = false);

    void minimize(Variable<Matrix<T>>& weights) const override;

  private:
    T learning_rate;
    T momentum;
    bool nesterov;
};

template <typename T>
SGD<T>::SGD(T learning_rate, T momentum, bool nesterov)
    : learning_rate(learning_rate), momentum(momentum), nesterov(nesterov)
{
}

template <typename T> void SGD<T>::minimize(Variable<Matrix<T>>& weights) const
{

    if (momentum == 0.0)
    {
        weights.set_value(weights.get_value() - weights.get_grad() * learning_rate);
    }
    else
    {
        //#TODO implement momentum
    }
}

#endif //MATH_SGD_H
