//
// Created by Mateusz Paszkowski on 03.11.22.
//

#ifndef MONOGON_INITIALIZER_H
#define MONOGON_INITIALIZER_H

#include <random>
#include "../Array.h"
#include "../Variable.h"

template <typename T = double>
class Initializer
{
public:
    Initializer() = default;
    Initializer(const Initializer<T> &initializer) = default;
    Initializer(Initializer<T> &&initializer) noexcept = default;

    Initializer &operator=(const Initializer<T> &initializer) = default;
    Initializer &operator=(Initializer<T> &&initializer) noexcept = default;
    virtual ~Initializer() = default;
public:
    virtual Variable<Array<T>> operator()(std::size_t rows, std::size_t cols) const = 0;
};

//------------------------------------------------- Class Definition ---------------------------------------------------

template <typename T = double>
class GlorotUniform : public Initializer<T>
{
public:
    GlorotUniform() = default;
    GlorotUniform(const GlorotUniform<T> &glorot_uniform) = default;
    GlorotUniform(GlorotUniform<T> &&glorot_uniform) noexcept = default;

    GlorotUniform &operator=(const GlorotUniform<T> &glorot_uniform) = default;
    GlorotUniform &operator=(GlorotUniform<T> &&glorot_uniform) noexcept = default;
    virtual ~GlorotUniform() = default;
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

    Array<T> result(Shape({rows, cols}));
    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
            result(i, j) = dis(gen);

    return Variable(std::move(result));
}

template <typename T = double>
class Ones : public Initializer<T>
{
public:
    Variable<Array<T>> operator()(std::size_t rows, std::size_t cols) const override;
};

template <typename T>
Variable<Array<T>> Ones<T>::operator()(std::size_t rows, std::size_t cols) const
{
    Array<T> result(Shape({rows, cols}), 1.0);
    return Variable(std::move(result));
}


template <typename T = double>
class Zeros : public Initializer<T>
{
public:
    Variable<Array<T>> operator()(std::size_t rows, std::size_t cols) const override;
};

template <typename T>
Variable<Array<T>> Zeros<T>::operator()(std::size_t rows, std::size_t cols) const
{
    Array<T> result(Shape({rows, cols}), 0.0);
    return Variable(std::move(result));
}

#endif //MONOGON_INITIALIZER_H
