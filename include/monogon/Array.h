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

//---------------------------------------------------- Class body ------------------------------------------------------

template <typename T = double>
class Array
{
public:
    using value_type = T;
    using size_type = std::size_t;
    Array();
    Array(size_t row, size_t column);
    Array(Shape shape);
    Array(size_t row, size_t column, const T &value);

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
    value_type &operator()(std::size_t idx);
    const value_type &operator()(std::size_t idx) const;

    value_type &operator()(std::size_t row, std::size_t column);
    const value_type &operator()(std::size_t row, std::size_t column) const;

    value_type &operator()(const Index& idx);
    const value_type &operator()(const Index& idx) const;

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
    size_type get_columns() const;
    const Shape& get_shape() const;

private:
    std::valarray<value_type> data;
    size_t column_size;
    Shape shape;
public:
    template <typename U>
    friend std::ostream &operator<<(std::ostream &os, const Array<U> &variable);
};

//--------------------------------------------------- Constructors -----------------------------------------------------

template <typename T>
Array<T>::Array() : data(), column_size(0)
{
}

template <typename T>
Array<T>::Array(size_t row, size_t column) : data(row * column), column_size(column), shape({row, column})
{
}

template <typename T>
Array<T>::Array(Shape sh) : data(sh.length()), shape(sh)
{
    column_size = sh(-1);
}

template <typename T>
Array<T>::Array(size_t row, size_t column, const T &value) : data(row * column), column_size(column), shape({row, column})
{
    for (auto &&item : data)
        item = value;
}

template <typename T>
Array<T>::~Array() = default;

template <typename T>
Array<T>::Array(std::initializer_list<T> list) : column_size(1), shape({list.size()})
{
    this->data.resize(list.size());
    size_t i = 0;
    for (const auto &item : list)
        data[i++] = item;
}

template <typename T>
Array<T>::Array(std::initializer_list<std::initializer_list<T>> nasted_list)
    : column_size(nasted_list.begin()->size()), shape({nasted_list.size(), nasted_list.begin()->size()})
{
    this->data.resize(this->shape(-2) * this->column_size);
    size_t i = 0;
    for (const auto &list : nasted_list)
        for (const auto &item : list)
            data[i++] = item;
}

template <typename T>
Array<T>::Array(std::initializer_list<std::initializer_list<std::initializer_list<T>>> nasted_nasted_list)
    : shape({nasted_nasted_list.size(), nasted_nasted_list.begin()->size(), nasted_nasted_list.begin()->begin()->size()})
{
    this->data.resize(nasted_nasted_list.size() * nasted_nasted_list.begin()->size() * nasted_nasted_list.begin()->begin()->size());
    size_t i = 0;
    for (auto nasted_list : nasted_nasted_list)
        for (auto &&list : nasted_list)
            for (auto &&item : list)
                data[i++] = item;
}

template <typename T>
Array<T>::Array(std::initializer_list<std::initializer_list<std::initializer_list<std::initializer_list<T>>>> nasted_nasted_nasted_list)
    : shape({nasted_nasted_nasted_list.size(), nasted_nasted_nasted_list.begin()->size(), nasted_nasted_nasted_list.begin()->begin()->size(), nasted_nasted_nasted_list.begin()->begin()->begin()->size()})
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
Array<T>::Array(const Container<T> &container) :  column_size(1), shape({container.size()})

{
    this->data.resize(this->shape(-2) * this->column_size);
    size_t i = 0;
    for (const auto &item : container)
        data[i++] = item;
}

template <typename T>
template <template <typename...> typename Container, template <typename...> typename NestedContainer>
Array<T>::Array(const Container<NestedContainer<T>> &container)
    : column_size(container.begin()->size()), shape({container.size(), container.begin()->size()})
{
    this->data.resize(this->shape(-2) * this->column_size);

    for (size_t i = 0, k = 0; i < container.size(); ++i)
        for (size_t j = 0; j < container[i].size(); ++j)
            data[k++] = container[i][j];
}


template <typename T>
Array<T>::Array(const Array<T> &rhs) : data(rhs.data), column_size(rhs.column_size), shape(rhs.shape)
{
}

template <typename T>
Array<T>::Array(Array<T> &&rhs) noexcept
    : data(std::move(rhs.data)), column_size(std::move(rhs.column_size)), shape(std::move(rhs.shape))
{
}

template <typename T>
Array<T> &Array<T>::operator=(const Array<T> &rhs)
{
    this->data = rhs.data;
    this->column_size = rhs.column_size;
    this->shape = rhs.shape;
    return *this;
}

template <typename T>
Array<T> &Array<T>::operator=(Array<T> &&rhs) noexcept
{
    this->data = std::move(rhs.data);
    this->column_size = std::move(rhs.column_size);
    this->shape = std::move(rhs.shape);
    return *this;
}

//----------------------------------------------------- Operators ------------------------------------------------------

template <typename T>
T &Array<T>::operator()(std::size_t idx)
{
    return this->data[idx];
}

template <typename T>
const T &Array<T>::operator()(std::size_t idx) const
{
    return this->data[idx];
}

template <typename T>
T &Array<T>::operator()(std::size_t row, std::size_t column)
{
    return data[row * column_size + column];
}

template <typename T>
const T &Array<T>::operator()(std::size_t row, std::size_t column) const
{
    return data[row * column_size + column];
}

template <typename T>
T &Array<T>::operator()(const Index &index)
{
    size_t result_index = index[0];
    for (size_t i = 1; i < index.size(); i++)
        result_index = result_index * shape[i] + index[i];

    return data[result_index];
}

template <typename T>
const T &Array<T>::operator()(const Index &index) const
{
    size_t result_index = index[0];
    for (size_t i = 1; i < index.size(); i++)
        result_index = result_index * shape[i] + index[i];

    return data[result_index];
}

template <typename T>
auto Array<T>::operator-() const
{
    Array<T> result(this->shape(-2), this->column_size);
    result.data = -this->data;

    return result;
}

template <typename T>
template <typename U>
auto Array<T>::operator+(const Array<U> &rhs) const
{
    using result_val_type = decltype(std::declval<T>() + std::declval<U>());
    Array<result_val_type> result(std::max(this->shape(-2), rhs.shape(-2)),
                                  std::max(this->column_size, rhs.get_columns()));

    //    #pragma omp parallel for private(i,j) shared(result,*this,rhs)
    for (size_t i = 0; i < result.shape(-2); i++)
    {
        for (size_t j = 0; j < result.get_columns(); j++)
        {
            result(i, j) = this->operator()(i % this->shape(-2), j % this->get_columns()) +
                           rhs(i % rhs.shape(-2), j % rhs.get_columns());
        }
    }
    return result;
}

template <typename T>
template <typename U>
auto Array<T>::operator-(const Array<U> &rhs) const
{
    assert(this->shape(-2) == rhs.shape(-2) && this->column_size == rhs.column_size);
    using result_val_type = decltype(std::declval<T>() - std::declval<U>());

    Array<result_val_type> result(this->shape(-2), this->column_size);
    result.data = this->data - rhs.data;

    return result;
}

template <typename T>
template <typename U>
auto Array<T>::operator*(const Array<U> &rhs) const
{
    assert(this->shape(-2) == rhs.shape(-2) && this->column_size == rhs.column_size);
    using result_val_type = decltype(std::declval<T>() * std::declval<U>());

    Array<result_val_type> result(this->shape(-2), this->column_size);
    result.data = this->data * rhs.data;

    return result;
}

template <typename T>
template <typename U>
auto Array<T>::operator/(const Array<U> &rhs) const
{
    assert(this->shape(-2) == rhs.shape(-2) && this->column_size == rhs.column_size);
    using result_val_type = decltype(std::declval<T>() / std::declval<U>());

    Array<result_val_type> result(this->shape(-2), this->column_size);
    result.data = this->data / rhs.data;

    return result;
}

template <typename T>
template <typename U>
auto Array<T>::operator+(const U &val) const
{
    using result_val_type = decltype(std::declval<T>() + std::declval<U>());
    Array<result_val_type> result(this->shape(-2), this->get_columns());
    result.data = data + val;
    return result;
}

template <typename T>
template <typename U>
auto Array<T>::operator-(const U &val) const
{
    using result_val_type = decltype(std::declval<T>() - std::declval<U>());
    Array<result_val_type> result(this->shape(-2), this->get_columns());
    result.data = data - val;
    return result;
}

template <typename T>
template <typename U>
auto Array<T>::operator*(const U &val) const
{
    using result_val_type = decltype(std::declval<T>() * std::declval<U>());
    Array<result_val_type> result(this->shape(-2), this->get_columns());
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
    Array<result_val_type> result(this->shape(-2), this->shape(-1));

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
    Array<T> result(this->shape(-1), this->shape(-2));

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

    Array<result_val_type> result(rows, cols);

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

    Array<result_val_type> result(rows, cols);

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
size_t Array<T>::get_columns() const
{
    return this->column_size;
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

#endif //MONOGON_ARRAY_H
