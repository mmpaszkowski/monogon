//
// Created by noname on 03.11.22.
//

#ifndef MATH_INITIALIZER_H
#define MATH_INITIALIZER_H

#include <random>
template <typename T = double> class Initializer
{
  public:
    virtual Variable<Array<T>> operator()(std::size_t rows, std::size_t cols) const = 0;
};

//------------------------------------------------- Class Definition ---------------------------------------------------

template <typename T = double> class GlorotUniform : public Initializer<T>
{
public:
    Variable<Array<T>> operator()(std::size_t rows, std::size_t cols) const override;
};

template <typename T>
Variable<Array<T>> GlorotUniform<T>::operator()(std::size_t rows, std::size_t cols) const
{
    T limit = sqrt(6.0 / (static_cast<T>(rows) + static_cast<T>(cols)));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-limit, limit);

    Array<T> result(rows, cols);
    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
            result(i, j) = dis(gen);

    return Variable(result);
}

template <typename T = double> class Ones : public Initializer<T>
{
public:
    Variable<Array<T>> operator()(std::size_t rows, std::size_t cols) const override;
};

template <typename T>
Variable<Array<T>> Ones<T>::operator()(std::size_t rows, std::size_t cols) const
{
    Array<T> result(rows, cols, 1.0);
    return Variable(result);
}

#endif //MATH_INITIALIZER_H
