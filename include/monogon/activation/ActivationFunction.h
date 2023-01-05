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
    ActivationFunction();
    ActivationFunction(const ActivationFunction<T> &activation_function);
    ActivationFunction(ActivationFunction<T> &&activation_function) noexcept;

    ActivationFunction &operator=(const ActivationFunction<T> &activation_function);
    ActivationFunction &operator=(ActivationFunction<T> &&activation_function) noexcept;

    virtual ~ActivationFunction();

public:
    virtual Variable<Array<T>> operator()(Variable<Array<T>> x) const = 0;
};

template <typename T>
ActivationFunction<T>::ActivationFunction() = default;

template <typename T>
ActivationFunction<T>::ActivationFunction(const ActivationFunction<T> &activation_function) = default;

template <typename T>
ActivationFunction<T>::ActivationFunction(ActivationFunction<T> &&activation_function) noexcept = default;

template <typename T>
ActivationFunction<T> &ActivationFunction<T>::operator=(const ActivationFunction<T> &activation_function) = default;

template <typename T>
ActivationFunction<T> &ActivationFunction<T>::operator=(ActivationFunction<T> &&activation_function) noexcept = default;

template <typename T>
ActivationFunction<T>::~ActivationFunction() = default;

//------------------------------------------------- Class Definition ---------------------------------------------------

template <typename T = double>
class ReLu : public ActivationFunction<T>
{
public:
    ReLu(T alpha = 0.0, T threshold = 0.0, std::optional<T> max_value = std::optional<T>());
    ReLu(const ReLu<T> &reLu);
    ReLu(ReLu<T> &&reLu) noexcept;

    ReLu &operator=(const ReLu<T> &reLu);
    ReLu &operator=(ReLu<T> &&reLu) noexcept;

    virtual ~ReLu();

public:
    Variable<Array<T>> operator()(Variable<Array<T>> x) const override;

private:
    T alpha;
    std::optional<T> max_value;
    T threshold;
};

template <typename T>
ReLu<T>::ReLu(T a, T t, std::optional<T> mv) : alpha(a), max_value(mv), threshold(t)
{
}

template <typename T>
ReLu<T>::ReLu(const ReLu<T> &reLu) = default;

template <typename T>
ReLu<T>::ReLu(ReLu<T> &&reLu) noexcept = default;

template <typename T>
ReLu<T> &ReLu<T>::operator=(const ReLu<T> &reLu) = default;

template <typename T>
ReLu<T> &ReLu<T>::operator=(ReLu<T> &&reLu) noexcept = default;

template <typename T>
ReLu<T>::~ReLu() = default;

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
    Sigmoid();
    Sigmoid(const Sigmoid<T> &sigmoid);
    Sigmoid(Sigmoid<T> &&sigmoid) noexcept;

    Sigmoid &operator=(const Sigmoid<T> &sigmoid);
    Sigmoid &operator=(Sigmoid<T> &&sigmoid) noexcept;

    virtual ~Sigmoid();

public:
    Variable<Array<T>> operator()(Variable<Array<T>> x) const override;

private:
    Init<Array<T>> initializer;
};

template <typename T>
Sigmoid<T>::Sigmoid() = default;

template <typename T>
Sigmoid<T>::Sigmoid(const Sigmoid<T> &sigmoid) = default;

template <typename T>
Sigmoid<T>::Sigmoid(Sigmoid<T> &&sigmoid) noexcept = default;

template <typename T>
Sigmoid<T> &Sigmoid<T>::operator=(const Sigmoid<T> &sigmoid) = default;

template <typename T>
Sigmoid<T> &Sigmoid<T>::operator=(Sigmoid<T> &&sigmoid) noexcept = default;

template <typename T>
Sigmoid<T>::~Sigmoid() = default;

template <typename T>
Variable<Array<T>> Sigmoid<T>::operator()(Variable<Array<T>> x) const
{
    initializer.initialize(x.get_value(), T(1.0));
    return Variable(initializer.initialize(x.get_value(), T(1.0))) /
           (Variable(initializer.initialize(x.get_value(), T(1.0))) +
            (Variable(initializer.initialize(x.get_value(), T(-1.0))) * x).exp());
}

#endif //MONOGON_ACTIVATION_FUNCTION_H
