//
// Created by Mateusz Paszkowski on 01.11.22.
//

#ifndef MONOGON_ACTIVATION_FUNCTION_H
#define MONOGON_ACTIVATION_FUNCTION_H

#include "../Array.h"
#include "../Variable.h"

template <typename T>
class ActivationFunction
{
public:
    ActivationFunction() = default;
    ActivationFunction(const ActivationFunction<T> &activation_function) = default;
    ActivationFunction(ActivationFunction<T> &&activation_function) noexcept = default;

    ActivationFunction &operator=(const ActivationFunction<T> &activation_function) = default;
    ActivationFunction &operator=(ActivationFunction<T> &&activation_function) noexcept = default;

    virtual ~ActivationFunction() = default;
public:
    virtual Variable<Array<T>> operator()(Variable<Array<T>> x) const = 0;
};

//------------------------------------------------- Class Definition ---------------------------------------------------

template <typename T = double>
class ReLu : public ActivationFunction<T>
{
public:
    ReLu(T alpha = 0.0, T threshold = 0.0, std::optional<T> max_value = std::optional<T>());
    ReLu(const ReLu<T> &reLu) = default;
    ReLu(ReLu<T> &&reLu) noexcept = default;

    ReLu &operator=(const ReLu<T> &reLu) = default;
    ReLu &operator=(ReLu<T> &&reLu) noexcept = default;

    virtual ~ReLu() = default;

public:
    Variable<Array<T>> operator()(Variable<Array<T>> x) const override;

private:
    T alpha;
    std::optional<T> max_value;
    T threshold;
};

template <typename T>
ReLu<T>::ReLu(T a, T t, std::optional<T> mv)
    : alpha(a), max_value(mv), threshold(t)
{
}

template <typename T>
Variable<Array<T>> ReLu<T>::operator()(Variable<Array<T>> x) const
{
    return x.max(Variable(T(0.0)));
}

//------------------------------------------------- Class Definition ---------------------------------------------------

template <typename T = double>
class Sigmoid : public ActivationFunction<T>
{
public:
    Sigmoid() = default;
    Sigmoid(const Sigmoid<T> &sigmoid) = default;
    Sigmoid(Sigmoid<T> &&sigmoid) noexcept = default;

    Sigmoid &operator=(const Sigmoid<T> &sigmoid) = default;
    Sigmoid &operator=(Sigmoid<T> &&sigmoid) noexcept = default;

    virtual ~Sigmoid() = default;
public:
    Variable<Array<T>> operator()(Variable<Array<T>> x) const override;

private:
    Init<Array<T>> initializer;
};

template <typename T>
Variable<Array<T>> Sigmoid<T>::operator()(Variable<Array<T>> x) const
{
    initializer.initialize(x.get_value(), T(1.0));
    return Variable(initializer.initialize(x.get_value(), T(1.0))) /
           (Variable(initializer.initialize(x.get_value(), T(1.0))) +
            (Variable(initializer.initialize(x.get_value(), T(-1.0))) * x).exp());
}

#endif //MONOGON_ACTIVATION_FUNCTION_H
