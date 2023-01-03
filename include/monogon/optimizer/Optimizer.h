//
// Created by Mateusz Paszkowski on 21.10.22.
//

#ifndef MONOGON_OPTIMIZER_H
#define MONOGON_OPTIMIZER_H

#include "../Variable.h"
template <typename T>
class Optimizer
{
public:
    Optimizer() = default;
    Optimizer(const Optimizer<T> &optimizer) = default;
    Optimizer(Optimizer<T> &&optimizer) noexcept = default;

    Optimizer &operator=(const Optimizer<T> &optimizer) = default;
    Optimizer &operator=(Optimizer<T> &&optimizer) noexcept = default;

    virtual ~Optimizer() = default;
public:
    virtual void minimize(Variable<Array<T>> &weights) const = 0;
};

#endif //MONOGON_OPTIMIZER_H
