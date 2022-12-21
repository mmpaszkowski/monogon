//
// Created by noname on 01.11.22.
//

#ifndef MATH_ACTIVATIONFUNCTION_H
#define MATH_ACTIVATIONFUNCTION_H

#include "../Array.h"
#include "../Variable.h"

template <typename T> class ActivationFunction
{
  public:
    virtual Variable<Array<T>> operator()(Variable<Array<T>> x) const = 0;
};

//------------------------------------------------- Class Definition ---------------------------------------------------

template <typename T = double> class ReLu : public ActivationFunction<T>
{
  public:
    ReLu(T alpha = 0.0, T threshold = 0.0, std::optional<T> max_value = std::optional<T>());
    Variable<Array<T>> operator()(Variable<Array<T>> x) const override;

  private:
    T alpha;
    std::optional<T> max_value;
    T threshold;
};

template <typename T>
ReLu<T>::ReLu(T alpha, T threshold, std::optional<T> max_value)
    : alpha(alpha), max_value(max_value), threshold(threshold)
{
}

template <typename T> Variable<Array<T>> ReLu<T>::operator()(Variable<Array<T>> x) const
{
    return x.max(Variable(T(0.0)));
}

//------------------------------------------------- Class Definition ---------------------------------------------------

template <typename T = double> class Sigmoid : public ActivationFunction<T>
{
  public:
    Variable<Array<T>> operator()(Variable<Array<T>> x) const override;

  private:
    Init<Array<T>> initializer;
};

template <typename T> Variable<Array<T>> Sigmoid<T>::operator()(Variable<Array<T>> x) const
{
    initializer.initialize(x.get_value(), T(1.0));
    return Variable(initializer.initialize(x.get_value(), T(1.0))) / ( Variable(initializer.initialize(x.get_value(), T(1.0))) + (Variable(initializer.initialize(x.get_value(), T(-1.0))) * x).exp());
}

#endif //MATH_ACTIVATIONFUNCTION_H
