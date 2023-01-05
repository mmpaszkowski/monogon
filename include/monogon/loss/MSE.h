//
// Created by Mateusz Paszkowski on 21.10.22.
//

#ifndef MONOGON_MSE_H
#define MONOGON_MSE_H

#include "../Array.h"
#include "../Variable.h"
#include "Loss.h"

template <typename T = double>
class MSE : public Loss<T>
{
public:
    MSE() = default;
    MSE(const MSE<T> &mse) = default;
    MSE(MSE<T> &&mse) noexcept = default;

    MSE &operator=(const MSE<T> &mse) = default;
    MSE &operator=(MSE<T> &&mse) noexcept = default;

    virtual ~MSE() = default;
public:
    Variable<T> operator()(Variable<Array<T>> y_pred, Variable<Array<T>> y) const;
};

template <typename T>
Variable<T> MSE<T>::operator()(Variable<Array<T>> y_pred, Variable<Array<T>> y) const
{
    return ((y - y_pred) * (y - y_pred)).avg();
}

#endif //MONOGON_MSE_H
