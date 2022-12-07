//
// Created by noname on 03.11.22.
//

#ifndef MATH_TENSOR_H
#define MATH_TENSOR_H

#include "tool/Index.h"
#include "tool/Shape.h"
#include <cstdarg>
#include <numeric>
#include <type_traits>
#include <valarray>

//------------------------------------------------- Class Definition ---------------------------------------------------

template <typename T> class Tensor
{
  public:
    using value_type = T;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = size_t;

  public:
    Tensor(const std::initializer_list<T> &list);
    Tensor(const std::initializer_list<std::initializer_list<T>> &list);
    Tensor(const std::initializer_list<std::initializer_list<std::initializer_list<T>>> &list);
    Tensor(const std::initializer_list<std::initializer_list<std::initializer_list<std::initializer_list<T>>>> &list);
    Tensor(const Shape &shape, T value);

  private:
    Tensor(const std::vector<size_type> &shape, std::valarray<T> &&data);
    Tensor(const std::vector<size_type> &shape);

    Tensor(const Shape &shape, std::valarray<T> &&data);
    Tensor(const Shape &shape);

  public:
    template <typename... Ts> reference operator()(Ts... indexes);
    template <typename... Ts> const_reference operator()(Ts... indexes) const;

    reference operator()(const Index &index);
    const_reference operator()(const Index &index) const;

    auto operator-() const;
    template <typename U> auto operator+(const Tensor<U> &rhs) const;
    template <typename U> auto operator-(const Tensor<U> &rhs) const;
    template <typename U> auto operator*(const Tensor<U> &rhs) const;
    template <typename U> auto operator/(const Tensor<U> &rhs) const;

    template <typename U> bool operator==(const Tensor<U> &rhs) const;

    Tensor<T> transpose(const std::vector<size_type> &indexes) const;
    template <typename U> Tensor<T> dot(const Tensor<U> &rhs) const;

  public:
    const Shape &shape() const;

  private:
    std::valarray<value_type> data;
    Shape __shape;

  public:
    template <typename U> friend std::ostream &operator<<(std::ostream &os, const Tensor<U> &variable);
};

//--------------------------------------------------- Constructors -----------------------------------------------------

template <typename T> Tensor<T>::Tensor(const std::initializer_list<T> &list)
{
    std::vector<size_t> shape_data;

    shape_data.push_back(list.size());

    this->__shape = Shape(shape_data);

    this->data.resize(std::accumulate(__shape.begin(), __shape.end(), 1ULL, std::multiplies<>()));

    size_t i = 0;

    for (const auto &item_i : list)
    {
        this->data[i++] = item_i;
    }
}

template <typename T> Tensor<T>::Tensor(const std::initializer_list<std::initializer_list<T>> &list)
{
    std::vector<size_t> shape_data;

    shape_data.push_back(list.size());
    shape_data.push_back(list.begin()->size());

    this->__shape = Shape(shape_data);

    this->data.resize(std::accumulate(__shape.begin(), __shape.end(), 1ULL, std::multiplies<>()));

    size_t i = 0;
    size_t j = 0;

    for (const auto &item_i : list)
    {
        for (const auto &item_j : item_i)
        {
            this->data[j++] = item_j;
        }
        i++;
    }
}

template <typename T>
Tensor<T>::Tensor(const std::initializer_list<std::initializer_list<std::initializer_list<T>>> &list)
{
    std::vector<size_t> shape_data;

    shape_data.push_back(list.size());
    shape_data.push_back(list.begin()->size());
    shape_data.push_back(list.begin()->begin()->size());

    this->__shape = Shape(shape_data);

    this->data.resize(std::accumulate(__shape.begin(), __shape.end(), 1ULL, std::multiplies<>()));

    size_t i = 0;
    size_t j = 0;
    size_t k = 0;

    for (const auto &item_i : list)
    {
        for (const auto &item_j : item_i)
        {
            for (const auto &item_k : item_j)
            {
                this->data[k++] = item_k;
            }
            j++;
        }
        i++;
    }
}

template <typename T>
Tensor<T>::Tensor(
    const std::initializer_list<std::initializer_list<std::initializer_list<std::initializer_list<T>>>> &list)
{
    std::vector<size_t> shape_data;

    shape_data.push_back(list.size());
    shape_data.push_back(list.begin()->size());
    shape_data.push_back(list.begin()->begin()->size());
    shape_data.push_back(list.begin()->begin()->begin()->size());

    this->__shape = Shape(shape_data);

    this->data.resize(std::accumulate(__shape.begin(), __shape.end(), 1ULL, std::multiplies<>()));

    size_t i = 0;
    size_t j = 0;
    size_t k = 0;
    size_t l = 0;

    for (const auto &item_i : list)
    {
        for (const auto &item_j : item_i)
        {
            for (const auto &item_k : item_j)
            {
                for (const auto &item_l : item_k)
                {
                    this->data[l++] = item_l;
                }
                k++;
            }
            j++;
        }
        i++;
    }
}

template <typename T>
Tensor<T>::Tensor(const std::vector<size_type> &shape, std::valarray<T> &&data) : __shape(shape), data(std::move(data))
{
}

template <typename T>
Tensor<T>::Tensor(const std::vector<size_type> &shape)
    : __shape(shape), data(std::accumulate(shape.begin(), shape.end(), 1ULL, std::multiplies<>()))
{
}

template <typename T>
Tensor<T>::Tensor(const Shape &shape, std::valarray<T> &&data) : __shape(shape), data(std::move(data))
{
}

template <typename T>
Tensor<T>::Tensor(const Shape &shape)
    : __shape(shape), data(std::accumulate(shape.begin(), shape.end(), 1ULL, std::multiplies<>()))
{
}

template <typename T> Tensor<T>::Tensor(const Shape &shape, T value)
    : __shape(shape), data(value, std::accumulate(shape.begin(), shape.end(), 1ULL, std::multiplies<>()))
{
}


//----------------------------------------------------- Operators ------------------------------------------------------

template <typename T> template <typename... Ts> typename Tensor<T>::reference Tensor<T>::operator()(Ts... indexes)
{
    std::vector<size_t> index = {indexes...};
    size_t result_index = index.front();

    for (size_t i = 1; i < index.size(); i++)
    {
        result_index = result_index * __shape[i] + index[i];
    }
    result_index += index.back();
    return data[result_index];
}

template <typename T>
template <typename... Ts>
typename Tensor<T>::const_reference Tensor<T>::operator()(Ts... indexes) const
{
    std::vector<size_t> index = {indexes...};
    size_t result_index = index.front();

    for (size_t i = 1; i < index.size(); i++)
        result_index = result_index * __shape[i] + index[i];
    result_index += index.back();

    return data[result_index];
}

template <typename T> typename Tensor<T>::reference Tensor<T>::operator()(const Index &index)
{
    size_t result_index = index[0];
    for (size_t i = 1; i < index.size(); i++)
        result_index = result_index * __shape[i] + index[i];

    return data[result_index];
}

template <typename T> typename Tensor<T>::const_reference Tensor<T>::operator()(const Index &index) const
{
    size_t result_index = index[0];
    for (size_t i = 1; i < index.size(); i++)
        result_index = result_index * __shape[i] + index[i];

    return data[result_index];
}

template <typename T> auto Tensor<T>::operator-() const
{
    Tensor result(__shape, -data);
    return result;
}

template <typename T> template <typename U> auto Tensor<T>::operator+(const Tensor<U> &rhs) const
{
    Tensor result(__shape, this->data + rhs.data);
    return result;
}

template <typename T> template <typename U> auto Tensor<T>::operator-(const Tensor<U> &rhs) const
{
    Tensor result(__shape, this->data - rhs.data);
    return result;
}

template <typename T> template <typename U> auto Tensor<T>::operator*(const Tensor<U> &rhs) const
{
    Tensor result(__shape, this->data * rhs.data);
    return result;
}

template <typename T> template <typename U> auto Tensor<T>::operator/(const Tensor<U> &rhs) const
{
    Tensor result(__shape, this->data / rhs.data);
    return result;
}

template <typename T> template <typename U> bool Tensor<T>::operator==(const Tensor<U> &rhs) const
{
    return ((this->data == rhs.data).min() && this->__shape == rhs.__shape);
}

//----------------------------------------------------- Methods --------------------------------------------------------

template <typename T> Tensor<T> Tensor<T>::transpose(const std::vector<size_type> &axes) const //#TODO Refactor!!!!
{
    assert(axes.size() == __shape.size());

    auto to_index = [](const Shape &shape, size_type i) -> Index
    {
        std::vector<size_type> result;
        size_type temp_i = i;
        for (auto it = shape.begin(); it != shape.end(); ++it)
        {
            if (it + 1 == shape.end())
                result.push_back(i % *it);
            else
            {
                auto mul = std::accumulate(it + 1, shape.end(), 1ULL, std::multiplies<>());
                result.push_back(temp_i / mul);
                temp_i -= result.back() * mul;
            }
        }
        return Index(result);
    };

    auto swap_index = [](const Index index, const std::vector<size_type> &axes) -> Index {
        std::vector<size_type> result(index.size());

        for (size_t i = 0; i < axes.size(); i++)
        {
            result[i] = index[axes[i]];
        }
        return Index(result);
    };

    std::vector<size_type> result_shape;
    for (const auto &index : axes)
        result_shape.push_back(__shape[index]);

    Shape result_s(result_shape);

    Tensor<T> result(result_s);

    for (size_t i = 0; i < data.size(); i++)
    {
        result(swap_index(to_index(__shape, i), axes)) = this->data[i];
    }

    return Tensor<T>(result);
}

template <typename T> template <typename U> Tensor<T> Tensor<T>::dot(const Tensor<U> &rhs) const
{
    Shape result_shape = this->shape();
    result_shape[result_shape.size() - 1] = rhs.shape()[result_shape.size() - 1];
    Tensor<T> result(result_shape);

    Index end(this->shape());
    end[0] = shape()[0];

    for(Index index(this->shape()); index < end; index.increment(-3))
    {
        Index sub_index = index;

        for (size_t i = 0; i < this->__shape(-2); i++)
        {
            for (size_t j = 0; j < rhs.shape()(-1); j++)
            {
                for (size_t k = 0; k < this->__shape(-1); ++k)
                {
                    Index sub_index_i_j = sub_index;
                    sub_index_i_j(-2) = i;
                    sub_index_i_j(-1) = j;
                    Index sub_index_k_j = sub_index;
                    sub_index_k_j(-2) = k;
                    sub_index_k_j(-1) = j;
                    Index sub_index_i_k = sub_index;
                    sub_index_i_k(-2) = i;
                    sub_index_i_k(-1) = k;
                    result(sub_index_i_j) += this->operator()(sub_index_i_k) * rhs(sub_index_k_j);
                }
            }
        }
    }

    return result;
}

template <typename U> std::ostream &operator<<(std::ostream &os, const Tensor<U> &tensor)
{
    for (const auto &item : tensor.data)
        os << item << ", ";
    return os;
}

//---------------------------------------------------- Accessors -------------------------------------------------------

template <typename T> const Shape &Tensor<T>::shape() const
{
    return this->__shape;
}

#endif //MATH_TENSOR_H
