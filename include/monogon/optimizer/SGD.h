//
// Created by Mateusz Paszkowski on 21.10.22.
//

#ifndef MONOGON_SGD_H
#define MONOGON_SGD_H

#include "../Array.h"
#include "../Variable.h"
#include "Optimizer.h"
#include <iostream>

template <typename T = double>
class SGD : public Optimizer<T>
{
public:
    SGD(T learning_rate = 0.01, T momentum = 0.0, bool nesterov = false);

    void minimize(Variable<Array<T>> &weights) const override;

private:
    T learning_rate;
    T momentum;
    bool nesterov; //# Todo
};

template <typename T>
SGD<T>::SGD(T lr, T m, bool n)
    : learning_rate(lr), momentum(m), nesterov(n)
{
}

template <typename T>
void SGD<T>::minimize(Variable<Array<T>> &weights) const
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

#endif //MONOGON_SGD_H
