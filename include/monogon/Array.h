//
// Created by Mateusz Paszkowski on 22.07.22.
//

#ifndef MONOGON_ARRAY_H
#define MONOGON_ARRAY_H

#include "monogon/tool/Index.h"
#include "tool/Shape.h"
#include "utils/low_level.h"
#include <cassert>
#include <omp.h>
#include <ostream>
#include <valarray>
#include <iostream>
//---------------------------------------------------- Class body ------------------------------------------------------

template <typename T = double>
class Array
{
public:
    using value_type = T;
    using size_type = std::size_t;
    Array();
    Array(Shape shape);
    Array(Shape shape, const T &value);
    ~Array();

    template <template <typename...> typename Container>
    explicit Array(const Container<value_type> &container);

    template <template <typename...> typename Container, template <typename...> typename NestedContainer>
    explicit Array(const Container<NestedContainer<value_type>> &container);

    Array(std::initializer_list<value_type> list);
    Array(std::initializer_list<std::initializer_list<value_type>> nasted_list);
    Array(std::initializer_list<std::initializer_list<std::initializer_list<value_type>>> nasted_list);
    Array(std::initializer_list<std::initializer_list<std::initializer_list<std::initializer_list<value_type>>>> nasted_list);

    Array(const Array<T> &);
    Array(Array<T> &&) noexcept;

    Array<T> &operator=(const Array<T> &);
    Array<T> &operator=(Array<T> &&) noexcept;

public:
    inline value_type &operator()(std::size_t idx);
    inline const value_type &operator()(std::size_t idx) const;

    inline value_type &operator()(std::size_t row, std::size_t column);
    inline const value_type &operator()(std::size_t row, std::size_t column) const;

    inline value_type &operator()(const Index& idx);
    inline const value_type &operator()(const Index& idx) const;

    auto operator-() const;
    template <typename U> auto operator+(const Array<U> &rhs) const;
    template <typename U> auto operator-(const Array<U> &rhs) const;
    template <typename U> auto operator*(const Array<U> &rhs) const;
    template <typename U> auto operator/(const Array<U> &rhs) const;

    template <typename U> auto operator+(const U &val) const;
    template <typename U> auto operator-(const U &val) const;
    template <typename U> auto operator*(const U &val) const;
    template <typename U> auto operator/(const U &val) const;

    template <typename U> bool operator==(const Array<U> &rhs) const;

    Array<T> transpose() const;
    template <typename U>
    auto dot(const Array<U> &rhs) const;

    auto exp() const;
    auto max(T value) const;
    auto avg() const;

public:
    const std::valarray<value_type> &get_data() const;
    const Shape& get_shape() const;

private:
    std::valarray<value_type> data;
    Shape shape;
    size_t last_shape;
public:
    template <typename U>
    friend std::ostream &operator<<(std::ostream &os, const Array<U> &variable);
};

//--------------------------------------------------- Constructors -----------------------------------------------------

template <typename T>
Array<T>::Array() : data(), shape(), last_shape()
{
//    std::cout << "Array()" << std::endl;
}

template <typename T>
Array<T>::Array(Shape sh) : data(sh.length()), shape(sh), last_shape(sh(-1))
{
//    std::cout << "Array(Shape sh)" << std::endl;
}

template <typename T>
Array<T>::Array(Shape sh, const T &value) : data(value, sh.length()), shape(sh), last_shape(sh(-1))
{
//    std::cout << "Array(Shape sh, const T &value)" << std::endl;
}

template <typename T>
Array<T>::~Array()
{
//    std::cout << "~Array()" << std::endl;
}

template <typename T>
Array<T>::Array(std::initializer_list<T> list) : shape({list.size()}), last_shape(list.size())
{
//    std::cout << "Array(std::initializer_list<T> list)" << std::endl;

    this->data.resize(list.size());
    size_t i = 0;
    for (const auto &item : list)
        data[i++] = item;
}

template <typename T>
Array<T>::Array(std::initializer_list<std::initializer_list<T>> nasted_list)
    : shape({nasted_list.size(), nasted_list.begin()->size()}), last_shape(nasted_list.begin()->size())
{
//    std::cout << "Array(std::initializer_list<std::initializer_list<T>> nasted_list)" << std::endl;

    this->data.resize(this->shape(-2) * this->shape(-1));
    size_t i = 0;
    for (const auto &list : nasted_list)
        for (const auto &item : list)
            data[i++] = item;
}

template <typename T>
Array<T>::Array(std::initializer_list<std::initializer_list<std::initializer_list<T>>> nasted_nasted_list)
    : shape({nasted_nasted_list.size(), nasted_nasted_list.begin()->size(), nasted_nasted_list.begin()->begin()->size()}), last_shape(nasted_nasted_list.begin()->begin()->size())
{
//    std::cout << "Array(std::initializer_list<T> list)" << std::endl;

    this->data.resize(nasted_nasted_list.size() * nasted_nasted_list.begin()->size() * nasted_nasted_list.begin()->begin()->size());
    size_t i = 0;
    for (auto nasted_list : nasted_nasted_list)
        for (auto &&list : nasted_list)
            for (auto &&item : list)
                data[i++] = item;
}

template <typename T>
Array<T>::Array(std::initializer_list<std::initializer_list<std::initializer_list<std::initializer_list<T>>>> nasted_nasted_nasted_list)
    : shape({nasted_nasted_nasted_list.size(), nasted_nasted_nasted_list.begin()->size(), nasted_nasted_nasted_list.begin()->begin()->size(), nasted_nasted_nasted_list.begin()->begin()->begin()->size()}), last_shape(nasted_nasted_nasted_list.begin()->begin()->begin()->size())
{
    this->data.resize(nasted_nasted_nasted_list.size() * nasted_nasted_nasted_list.begin()->size() * nasted_nasted_nasted_list.begin()->begin()->size() * nasted_nasted_nasted_list.begin()->begin()->begin()->size());
    size_t i = 0;
    for (auto nasted_nasted_list : nasted_nasted_nasted_list)
        for (auto nasted_list : nasted_nasted_list)
            for (auto &&list : nasted_list)
                for (auto &&item : list)
                    data[i++] = item;
}

template <typename T>
template <template <typename...> typename Container>
Array<T>::Array(const Container<T> &container) : shape({container.size()}), last_shape(container.size())
{
//    std::cout << "Array(const Container<T> &container)" << std::endl;

    this->data.resize(this->shape(-1));
    size_t i = 0;
    for (const auto &item : container)
        data[i++] = item;
}

template <typename T>
template <template <typename...> typename Container, template <typename...> typename NestedContainer>
Array<T>::Array(const Container<NestedContainer<T>> &container)
    : shape({container.size(), container.begin()->size()}), last_shape(container.begin()->size())
{
//    std::cout << "Array(const Container<NestedContainer<T>> &container)" << std::endl;

    this->data.resize(this->shape(-2) * this->shape(-1));

    for (size_t i = 0, k = 0; i < container.size(); ++i)
        for (size_t j = 0; j < container[i].size(); ++j)
            data[k++] = container[i][j];
}


template <typename T>
Array<T>::Array(const Array<T> &rhs) : data(rhs.data), shape(rhs.shape), last_shape(rhs.last_shape)
{
//    std::cout << "Array(const Array<T> &rhs)" << std::endl;
}

template <typename T>
Array<T>::Array(Array<T> &&rhs) noexcept
    : data(std::move(rhs.data)), shape(std::move(rhs.shape)), last_shape(rhs.last_shape)
{
//    std::cout << "Array(Array<T> &&rhs)" << std::endl;
}

template <typename T>
Array<T> &Array<T>::operator=(const Array<T> &rhs)
{
//    std::cout << "operator=(const Array<T> &rhs)" << std::endl;

    this->data = rhs.data;
    this->shape = rhs.shape;
    this->last_shape = rhs.last_shape;
    return *this;
}

template <typename T>
Array<T> &Array<T>::operator=(Array<T> &&rhs) noexcept
{
//    std::cout << "operator=(Array<T> &&rhs)" << std::endl;

    this->data = std::move(rhs.data);
    this->shape = std::move(rhs.shape);
    this->last_shape = rhs.last_shape;
    return *this;
}

//----------------------------------------------------- Operators ------------------------------------------------------

template <typename T>
inline T &Array<T>::operator()(std::size_t idx)
{
    return this->data[idx];
}

template <typename T>
inline const T &Array<T>::operator()(std::size_t idx) const
{
    return this->data[idx];
}

template <typename T>
inline T &Array<T>::operator()(std::size_t row, std::size_t column)
{
    return data[row * this->last_shape + column];
}

template <typename T>
inline const T &Array<T>::operator()(std::size_t row, std::size_t column) const
{
    return data[row * this->last_shape + column];
}

template <typename T>
inline T &Array<T>::operator()(const Index &index)
{
    size_t result_index = index[0];
    for (size_t i = 1; i < index.size(); i++)
        result_index = result_index * shape[i] + index[i];

    return data[result_index];
}

template <typename T>
inline const T &Array<T>::operator()(const Index &index) const
{
    size_t result_index = index[0];
    for (size_t i = 1; i < index.size(); i++)
        result_index = result_index * shape[i] + index[i];

    return data[result_index];
}

template <typename T>
auto Array<T>::operator-() const
{
    Array<T> result(Shape({this->shape(-2), this->shape(-1)}));
    result.data = -this->data;

    return result;
}

template <typename T>
template <typename U>
auto Array<T>::operator+(const Array<U> &rhs) const
{
    using result_val_type = decltype(std::declval<T>() + std::declval<U>());
    Array<result_val_type> result(Shape({std::max(this->shape(-2), rhs.shape(-2)),
                                  std::max(this->shape(-1), rhs.shape(-1))}));

    size_t result_rows = result.shape(-2);
    size_t result_cols = result.shape(-1);

    size_t rows = shape(-2);
    size_t cols = shape(-1);

    size_t rhs_rows = rhs.shape(-2);
    size_t rhs_cols = rhs.shape(-1);

    for (size_t i = 0; i < result_rows; i++)
    {
        for (size_t j = 0; j < result_cols; j++)
        {
            result(i, j) = this->operator()(i % rows, j % cols) + rhs(i % rhs_rows, j % rhs_cols);
        }
    }
    return result;
}

template <typename T>
template <typename U>
auto Array<T>::operator-(const Array<U> &rhs) const
{
    assert(this->shape(-2) == rhs.shape(-2) && this->shape(-1) == rhs.shape(-1));
    using result_val_type = decltype(std::declval<T>() - std::declval<U>());

    Array<result_val_type> result(Shape({this->shape(-2), this->shape(-1)}));
    result.data = this->data - rhs.data;

    return result;
}

template <typename T>
template <typename U>
auto Array<T>::operator*(const Array<U> &rhs) const
{
    assert(this->shape(-2) == rhs.shape(-2) && this->shape(-1) == rhs.shape(-1));
    using result_val_type = decltype(std::declval<T>() * std::declval<U>());

    Array<result_val_type> result(Shape({this->shape(-2), this->shape(-1)}));
    result.data = this->data * rhs.data;

    return result;
}

template <typename T>
template <typename U>
auto Array<T>::operator/(const Array<U> &rhs) const
{
    assert(this->shape(-2) == rhs.shape(-2) && this->shape(-1) == rhs.shape(-1));
    using result_val_type = decltype(std::declval<T>() / std::declval<U>());

    Array<result_val_type> result(Shape({this->shape(-2), this->shape(-1)}));
    result.data = this->data / rhs.data;

    return result;
}

template <typename T>
template <typename U>
auto Array<T>::operator+(const U &val) const
{
    using result_val_type = decltype(std::declval<T>() + std::declval<U>());
    Array<result_val_type> result(Shape({this->shape(-2), this->shape(-1)}));
    result.data = data + val;
    return result;
}

template <typename T>
template <typename U>
auto Array<T>::operator-(const U &val) const
{
    using result_val_type = decltype(std::declval<T>() - std::declval<U>());
    Array<result_val_type> result(Shape({this->shape(-2), this->shape(-1)}));
    result.data = data - val;
    return result;
}

template <typename T>
template <typename U>
auto Array<T>::operator*(const U &val) const
{
    using result_val_type = decltype(std::declval<T>() * std::declval<U>());
    Array<result_val_type> result(Shape({this->shape(-2), this->get_shape()(-1)}));
    result.data = data * val;
    return result;
}

//template <typename T> template <typename U> auto Array<T>::operator/(const U &val) const
//{
//    using result_val_type = decltype(std::declval<T>() / std::declval<U>());
//    Array<result_val_type> result(this->get_rows(), this->get_columns());
//    result.data = data / val;
//    return result;
//}

template <typename T>
template <typename U>
auto Array<T>::operator/(const U &val) const
{
    using result_val_type = decltype(std::declval<T>() / std::declval<U>());
    Array<result_val_type> result(Shape({this->shape(-2), this->shape(-1)}));

    size_t rows = this->shape(-2);
    size_t cols = this->shape(-1);

    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
        {
            result(i, j) = this->operator()(i, j) / val;
        }
    return result;
}


template <typename T>
template <typename U>
bool Array<T>::operator==(const Array<U> &rhs) const
{
    for(size_t i = 0; i < this->data.size(); i++)
        if (this->data[i] != rhs.data[i])
            return false;
    return true;
}

//------------------------------------------------------ Methods -------------------------------------------------------

template <typename T>
Array<T> Array<T>::transpose() const
{
    Array<T> result(Shape({this->shape(-1), this->shape(-2)}));

    for (size_t i = 0; i < this->shape(-2); i++)
        for (size_t j = 0; j < this->shape(-1); j++)
            result(j, i) = this->data[i * this->shape(-1) + j];
    return result;
}

template <typename T>
template <typename U>
auto Array<T>::dot(const Array<U> &rhs) const
{
    using result_val_type = decltype(std::declval<T>() + std::declval<T>() * std::declval<U>());

    Shape result_shape = this->shape;
    result_shape(-1) = rhs.shape(-1);
    Array<result_val_type> result(result_shape);

    if(shape.size() > 2)
    {
        Index last_index = shape.last_index();
        for(Index i = shape.index(); i < last_index; i.increment(-3))
        {
            mat_mat_mul(this->shape(-2),
                        this->shape(-1),
                        rhs.shape(-1),
                        &this->operator()(i),
                        &rhs(i),
                        &result(i));
        }
    }
    else if (shape.size() == 2)
    {
        mat_mat_mul(this->shape(-2),
                    this->shape(-1),
                    rhs.shape(-1),
                    &this->data[0],
                    &rhs.data[0],
                    &result.data[0]);
    }
    else
        assert(false);

    return result;
}

template <typename T>
auto Array<T>::exp() const
{
    using result_val_type = T;
    size_t rows = this->shape(-2);
    size_t cols = this->shape(-1);

    Array<result_val_type> result(Shape({rows, cols}));

    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            result(i, j) = std::exp(this->operator()(i, j));

    return result;
}

template <typename T>
auto Array<T>::max(T value) const
{
    using result_val_type = T;

    size_t rows = this->shape(-2);
    size_t cols = this->shape(-1);

    Array<result_val_type> result(Shape({rows, cols}));

    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            result(i, j) = std::max(value, this->operator()(i, j));
    return result;
}

template <typename T>
auto Array<T>::avg() const
{
    T sum{};
    for (const auto &item : data)
        sum += item;

    return sum / static_cast<T>(data.size());
}

//----------------------------------------------------- Accessors ------------------------------------------------------

template <typename T>
const std::valarray<T> &Array<T>::get_data() const
{
    return this->data;
}

template <typename T>
const Shape& Array<T>::get_shape() const
{
    return this->shape;
}

template <typename U>
void print(std::ostream &os, const Array<U> &array, size_t shape_index, std::vector<size_t> index)
{
    if(shape_index == (array.get_shape().size()-1))
    {
        index.push_back(0);
        for(size_t j = 0; j < (array.get_shape()(array.get_shape().size()-1)); j++)
        {
            os << array(Index(index));
            index.back()++;
            if(index.back() < array.get_shape()(array.get_shape().size()-1))
                os << ", ";
        }
    }
    else
    {
        for(size_t i = 0; i < array.get_shape()(shape_index); i++)
        {
            os << "{";
            std::vector<size_t> new_index = index;
            new_index.push_back(i);
            print(os, array, shape_index+1, new_index);
            os << "}";
            if(i < array.get_shape()(shape_index)-1)
                os << ", ";
        }
    }
};

template <typename U> std::ostream &operator<<(std::ostream &os, const Array<U> &array)
{
    std::vector<size_t> idx{};
    os << "{";
    print(os, array, 0, idx);
    os << "}";
    return os;
}

template<typename T> std::vector<std::vector<T>> to_vector_2d(const Array<T>& array)
{
    std::vector<std::vector<size_t>> result(array.get_shape()(0), std::vector<T>(array.get_shape()(1)));

    for(size_t i = 0; i < result.size(); i++)
        for(size_t j = 0; j < result.size(); j++)
    {
        result[i][j] = array(i, j);
    }
    return result;
}

#endif //MONOGON_ARRAY_H
